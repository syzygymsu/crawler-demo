#include "simple_crawler.h"

#include <algorithm>
#include <iostream>

#include "url_info.h"


SimpleCrawler::SimpleCrawler(Repository& repository, CrawlerJob& job) :
		repository_(repository), crawler_job_(job) {
	downloader_.init(repository, *this);
	parser_.init(*this);

	for(auto url: job.initial_urls) {
		AddUrl(url, 0);
	}
}


void SimpleCrawler::AddUrl(std::string url, int depth) {
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


void SimpleCrawler::Execute() {
	while(!download_queue_.empty()) {
		DownloadJob download_job = download_queue_.front();
		download_queue_.pop();
		
		std::cout << "Downloading: " << download_job.url << std::endl;
		downloader_.Download(download_job);
	}
}


void SimpleCrawler::AddRedirect(DownloadJob job, std::string url) {
	AddUrl(url, 1+job.depth);
}


void SimpleCrawler::AddDocument(DownloadJob download_job, RepositoryDocument document) {
	if(crawler_job_.max_depth>=0 && download_job.depth>=crawler_job_.max_depth) {
		// Превышена глубина сканирования
		return ;
	}
	
	ParseJob parseJob;
	parseJob.document = document;
	parseJob.depth = download_job.depth;
	std::cout << "Parsing: " << parseJob.document.origin_url << std::endl;
	parser_.Parse(parseJob);
}


void SimpleCrawler::AddHyperlink(ParseJob job, std::string url) {
	AddUrl(url, 1+job.depth);
}
