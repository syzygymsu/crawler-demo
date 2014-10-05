#include "simple_crawler.h"
#include "url_info.h"

#include <algorithm>
#include <iostream>

SimpleCrawler::SimpleCrawler(Repository& repository, CrawlerJob& job) :
		repository(repository), job(job) {
	for(auto url: job.initialUrls) {
		addUrl(url, 0);
	}
	downloader.init(repository, *this);
	parser.init(*this);
}


void SimpleCrawler::addUrl(std::string url, int depth) {
	if(knownUrls.end() != knownUrls.find(url)) {
		// URL уже обработан
		return;
	}
	
	if(job.maxCount>0 && knownUrls.size()>=job.maxCount) {
		// достигнут лимит общего количества запросов
		return;
	}
	
	if(!job.mustContain.empty()) {
		bool match = std::any_of(
			job.mustContain.begin(), job.mustContain.end(),
			[url](const std::string &substring){return std::string::npos != url.find(substring);}
		);
		if(!match) {
			// URL не подходит ни под один фильтр
			return;
		}
	}
	
	UrlInfo urlInfo(url);
	std::transform(urlInfo.schema.begin(), urlInfo.schema.end(), urlInfo.schema.begin(), tolower);
	if(urlInfo.schema!="http" && urlInfo.schema!="https") {
		// неизвестный протокол
		return;
	}
	
	knownUrls.insert(url);
	DownloadJob dlJob;
	dlJob.url = url;
	dlJob.depth = depth;
	downloadQueue.push(dlJob);
}

void SimpleCrawler::execute() {
	while(!downloadQueue.empty()) {
		DownloadJob dlJob = downloadQueue.front();
		downloadQueue.pop();
		
		std::cout << "Downloading: " << dlJob.url << std::endl;
		
		documentDownloaded = false;
		downloader.Download(dlJob);
		if(documentDownloaded) {
			if(job.maxDepth<0 || dlJob.depth<job.maxDepth) {
				ParseJob parseJob;
				parseJob.document = lastDocument;
				parseJob.depth = dlJob.depth;
				std::cout << "Parsing: " << parseJob.document.originUrl << std::endl;
				parser.Parse(parseJob);
			}
		}
	}
}

void SimpleCrawler::AddRedirect(DownloadJob job, std::string url) {
	addUrl(url, 1+job.depth);
}

void SimpleCrawler::AddDocument(DownloadJob job, Document document) {
	documentDownloaded = true;
	lastDocument = document;
}

void SimpleCrawler::AddHyperlink(ParseJob job, std::string url) {
	addUrl(url, 1+job.depth);
}
