#include "repository.h"
#include "url_info.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

Repository::Repository(const std::string& basePath):
		basePath(basePath) {
	fs::path p(basePath);
	if(fs::exists(p)) {
		if(!fs::is_directory(p)) {
			throw std::string("Base path is not a directory");
		}
	} else {
		if(!fs::create_directories(p)) {
			throw std::string("Cannot create base path directory");
		}
	}
}

std::string Repository::pathFromUrl(std::string url) {
	fs::path path(basePath);
	
	UrlInfo urlInfo = ParseUrl(url);
	
	{
		std::string schema = urlInfo.schema;
		std::transform(schema.begin(), schema.end(), schema.begin(), tolower);
	
		std::string topDir;
		if(schema == std::string("https")) {
			topDir += "https_";
		} else {
			topDir += "http_";
		}
		topDir += urlInfo.domain;
		path /= sanitizePath(topDir);
	}
	
	path /=  sanitizePath(urlInfo.path);
	if(path.filename()=="." || urlInfo.path.empty()) {
		path /= "_";
	}
	
	if(!urlInfo.query.empty()) {
		path += '_';
		path += urlInfo.query;
	}
	
	if(path.extension().empty()) {
		path += ".html";
	}
	
	return path.native();
}

Document Repository::createDocument(std::string url) {
	std::string savePath = pathFromUrl(url);
	
	fs::path path(savePath);
	fs::create_directories(path.parent_path());
	
	return Document(url, savePath);
}

std::string Repository::sanitizePath(const std::string &path) {
	// TODO: экранировать или удалить потенциально нежелательные значения
	// Например, `..`
	return path;
}
