#include "repository.h"
#include "url_info.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

RepositoryDocument RepositoryDocument::create(
		const std::string& origin_url,
		const std::string& save_path) {
	RepositoryDocument document;
	document.origin_url = origin_url;
	document.save_path = save_path;
	return document;
}


Repository::Repository(const std::string& base_path):
		base_path_(base_path) {
	fs::path path(base_path);
	if(fs::exists(path)) {
		if(!fs::is_directory(path)) {
			throw std::string("Base path is not a directory");
		}
	} else {
		if(!fs::create_directories(path)) {
			throw std::string("Cannot create base path directory");
		}
	}
}


std::string Repository::PathFromUrl(std::string url) {
	fs::path path(base_path_);
	
	UrlInfo url_info = ParseUrl(url);
	
	// добавляем директорию с схемой и доменом
	{
		std::string schema = url_info.schema;
		std::transform(schema.begin(), schema.end(), schema.begin(), tolower);
	
		std::string top_dir;
		if(schema == std::string("https")) {
			top_dir += "https_";
		} else {
			top_dir += "http_";
		}
		top_dir += url_info.domain;
		path /= SanitizePath(top_dir);
	}
	
	// добавляем директории с путем
	path /=  SanitizePath(url_info.path);
	if(path.filename()=="." || url_info.path.empty()) {
		// если путь в URL заканчивается на / или пуст (только домен)
		path /= "_";
	}
	
	// добавляем запрос
	if(!url_info.query.empty()) {
		path += '_';
		path += url_info.query;
	}
	
	// добавляем расширение файла
	if(path.extension().empty()) {
		path += ".html";
	}
	
	return path.native();
}


RepositoryDocument Repository::CreateDocument(std::string url) {
	std::string save_path = PathFromUrl(url);
	
	fs::path path(save_path);
	if(!fs::create_directories(path.parent_path())) {
		throw std::string("Cannot create document directories");
	}
	
	return RepositoryDocument::create(url, save_path);
}


std::string Repository::SanitizePath(const std::string &path) {
	// TODO экранировать или удалить потенциально нежелательные значения
	// Например, `..`
	return path;
}
