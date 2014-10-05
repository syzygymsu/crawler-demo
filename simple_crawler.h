#ifndef SIMPLECRAWLER_H
#define	SIMPLECRAWLER_H

#include <queue>
#include <set>
#include <string>

#include "repository.h"
#include "crawler_job.h"
#include "curl_easy_downloader.h"
#include "libxml2_parser.h"


class SimpleCrawler: virtual public DownloadFeedbackInteraface, virtual public ParseFeedbackInterface {
public:
	SimpleCrawler(Repository &repository, CrawlerJob &job);
	
	void execute();
private:
	// реализация DownloadFeedbackInteraface
	virtual void AddRedirect(DownloadJob job, std::string url);
	virtual void AddDocument(DownloadJob job, Document document);
	
	// реализация ParseFeedbackInterface
	virtual void AddHyperlink(ParseJob job, std::string url);

	void addUrl(std::string url, int depth);

	Repository &repository;
	CrawlerJob &job;
	
	CurlEasyDownloader downloader;
	Libxml2Parser parser;
	
	std::set<std::string> knownUrls;
	std::queue<DownloadJob> downloadQueue;
	
	Document lastDocument;
	bool documentDownloaded;

};

#endif	/* SIMPLECRAWLER_H */

