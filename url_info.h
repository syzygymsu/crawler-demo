#ifndef URLINFO_H
#define	URLINFO_H

#include <string>

class UrlInfo {
public:
	UrlInfo(const std::string &url);
	
	/*
	 * Общий вид URL
	 * schema://domain/path?query#hash
	 */
	std::string schema;
	std::string domain;
	std::string path;
	std::string query;
	std::string hash;

	std::string originUrl;
};

#endif	/* URLINFO_H */
