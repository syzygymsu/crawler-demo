#include "threaded_crawler.h"

#include <algorithm>
#include <thread>
#include <sys/unistd.h>

#include "url_info.h"


ThreadedCrawler::ThreadedCrawler(Repository &repository, CrawlerJob &job):
		repository_(repository), crawler_job_(job) {
	downloader_.init(repository, *this);
	parser_.init(*this);

	for(auto url: job.initial_urls) {
		AddUrl(url, 0);
	}
}


void ThreadedCrawler::Execute() {
	quit_.store(false);
	live_threads_.store(0);
	
	std::thread download_thread(&ThreadedCrawler::DownloadThread, this);
	std::thread parse_thread(&ThreadedCrawler::ParseThread, this);
	
	while(!quit_) {
		fprintf( stdout, "Found: %6d, Download queue: %6d, Parse queue: %6d\n",
				KnownUrlsSize(), DownloadQueueSize(), ParseQueueSize() );
		sleep(1);
	}
	
	download_thread.join();
	parse_thread.join();
}


void ThreadedCrawler::AddUrl(std::string url, int depth) {
	std::lock_guard<std::mutex> lock(download_mutex_);
	
	if(known_urls_.end() != known_urls_.find(url)) {
		// URL уже обработан
		return;
	}
	
	if(crawler_job_.max_count>0 && known_urls_.size()>=crawler_job_.max_count) {
		// достигнут лимит общего количества запросов
		return;
	}
	
	if(!crawler_job_.must_contain.empty()) {
		bool match = std::any_of(
			crawler_job_.must_contain.begin(), crawler_job_.must_contain.end(),
			[url](const std::string &substring) {
				return std::string::npos != url.find(substring);
			}
		);
		if(!match) {
			// URL не подходит ни под один фильтр
			return;
		}
	}
	
	UrlInfo url_info = ParseUrl(url);
	std::transform(url_info.schema.begin(), url_info.schema.end(), url_info.schema.begin(), tolower);
	if(url_info.schema!="http" && url_info.schema!="https") {
		// неизвестный протокол
		return;
	}
	
	known_urls_.insert(url);
	DownloadJob downloadJob;
	downloadJob.url = url;
	downloadJob.depth = depth;
	download_queue_.push(downloadJob);
}


void ThreadedCrawler::AddHyperlink(ParseJob job, std::string url) {
	AddUrl(url, 1+job.depth);
}


void ThreadedCrawler::AddRedirect(DownloadJob job, std::string url) {
	AddUrl(url, 1+job.depth);
}


void ThreadedCrawler::AddDocument(DownloadJob job, RepositoryDocument document) {
	ParseJob parse_job;
	parse_job.depth = job.depth;
	parse_job.document = document;
	
	std::lock_guard<std::mutex> lock(parse_mutex_);
	parse_queue_.push(parse_job);
	
	parse_condition_.notify_one();
}


void ThreadedCrawler::DownloadThread() {
	while(!quit_) {
		
		if(downloader_.count() < 10) {
			std::lock_guard<std::mutex> lock(download_mutex_);
			while(downloader_.count() < 10 && !download_queue_.empty()) {
				downloader_.Download(download_queue_.front());
				download_queue_.pop();
			}
		}

		downloader_.PerformOne();
		
		if(!downloader_.count()) {
			std::lock(download_mutex_, parse_mutex_);
			
			if(download_queue_.empty() && parse_queue_.empty() && 0==live_threads_) {
				quit_ = true;
				parse_condition_.notify_all();
			}
			
			download_mutex_.unlock();
			parse_mutex_.unlock();
		}
	}
}


void ThreadedCrawler::ParseThread() {
	while(!quit_) {
		ParseJob parse_job;
		bool job_found = false;
		{
			std::unique_lock<std::mutex> lock(parse_mutex_);
			if(parse_queue_.empty()) {
				parse_condition_.wait(lock);
			} else {
				parse_job = parse_queue_.front();
				parse_queue_.pop();
				job_found = true;
				++live_threads_;
			}
		}
		
		if(job_found) {
			parser_.Parse(parse_job);
			--live_threads_;
		}
	}
}


int ThreadedCrawler::DownloadQueueSize() {
	std::lock_guard<std::mutex> lock(download_mutex_);
	return download_queue_.size() + downloader_.count();
}


int ThreadedCrawler::ParseQueueSize() {
	std::lock_guard<std::mutex> lock(parse_mutex_);
	return parse_queue_.size();
}


int ThreadedCrawler::KnownUrlsSize() {
	std::lock_guard<std::mutex> lock(download_mutex_);
	return known_urls_.size();
}
