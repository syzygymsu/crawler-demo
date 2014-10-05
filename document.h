#ifndef DOCUMENT_H
#define	DOCUMENT_H

#include <string>

class Document {
public:
	Document() {}
	Document(const std::string& originUrl, const std::string& savePath):
			originUrl(originUrl), savePath(savePath) {}
	
	std::string originUrl;
	std::string savePath;
};

#endif	/* DOCUMENT_H */

