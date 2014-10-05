#ifndef SIMPLECRAWLER_H
#define	SIMPLECRAWLER_H

#include <queue>
#include <set>
#include <string>

#include "repository.h"
#include "crawler_job.h"
#include "parse_interface.h"
#include "download_interface.h"
#include "curl_easy_downloader.h"
#include "libxml_parser.h"

class SimpleCrawler: virtual DownloadFeedbackInteraface, virtual ParseFeedbackInterface {
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

	// реализация DownloadFeedbackInteraface
	virtual void AddRedirect(DownloadJob job, std::string url);
	virtual void AddDocument(DownloadJob job, Document document);
	
	// реализация ParseFeedbackInterface
	virtual void AddHyperlink(ParseJob job, std::string url);

};

#endif	/* SIMPLECRAWLER_H */

