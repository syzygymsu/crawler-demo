#ifndef CRAWLERJOB_H
#define	CRAWLERJOB_H

#include <vector>
#include <string>

struct CrawlerJob {
	std::vector<std::string> initialUrls;
	std::vector<std::string> mustContain;
	
	int maxCount = -1;
	int maxDepth = -1;
};

#endif	/* CRAWLERJOB_H */
