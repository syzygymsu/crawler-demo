#include "UrlInfo.h"

#include <libxml/uri.h>

UrlInfo::UrlInfo(const std::string &url): originUrl(url) {
	xmlURIPtr uriData = xmlParseURI(url.c_str());
	if(uriData->scheme)    schema = uriData->scheme;
	if(uriData->server)    domain = uriData->server;
	if(uriData->path)      path = uriData->path;
	if(uriData->query_raw) query = uriData->query_raw;
	if(uriData->fragment)  hash = uriData->fragment;
	
	xmlFreeURI(uriData);
}
