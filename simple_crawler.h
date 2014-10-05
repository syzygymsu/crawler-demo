#ifndef SIMPLECRAWLER_H
#define	SIMPLECRAWLER_H

#include <queue>
#include <set>
#include <string>

#include "repository.h"
#include "crawler_job.h"
#include "download_feedback.h"
#include "curl_easy_downloader.h"
#include "parse_interface.h"
#include "libxml_parser.h"

class SimpleCrawler: virtual IDownloadFeedback, virtual ParseFeedbackInterface {
public:
	SimpleCrawler(Repository &repository, CrawlerJob &job);
	
	void execute();
private:
	Repository &repository;
	CrawlerJob &job;
	
	EasyDownloader downloader;
	XmlParser parser;
	
	std::set<std::string> knownUrls;
	std::queue<DownloadJob> downloadQueue;
	
	void addUrl(std::string url, int depth);
	
	Document lastDocument;
	bool documentDownloaded;

	// реализация IDownloaderFeedback
	virtual void addRedirect(const DownloadJob& job, std::string url);
	virtual void addDocument(const DownloadJob& job, Document& document);
	
	// реализация IParseFeedback
	virtual void addHyperlink(const ParseJob& job, std::string url);

};

#endif	/* SIMPLECRAWLER_H */

