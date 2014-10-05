#include "url_info.h"

#include <libxml/uri.h>


UrlInfo ParseUrl(const std::string &url) {
	return ParseUrlLibxml2(url);
}


UrlInfo ParseUrlLibxml2(const std::string &url) {
	UrlInfo url_info;
	
	xmlURIPtr uriData = xmlParseURI(url.c_str());
	if(uriData->scheme)    url_info.schema = uriData->scheme;
	if(uriData->server)    url_info.domain = uriData->server;
	if(uriData->path)      url_info.path   = uriData->path;
	if(uriData->query_raw) url_info.query  = uriData->query_raw;
	if(uriData->fragment)  url_info.hash   = uriData->fragment;
	
	xmlFreeURI(uriData);
	
	return url_info;
}
