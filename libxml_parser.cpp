#include "libxml_parser.h"

#define LIBXML_HTML_ENABLED

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>

#include "destruction_stack.h"

void XmlParser::Parse(ParseJob job, ParseFeedbackInterface& feedback) {
	DestructionStack destructors;
	
	htmlDocPtr docPtr = htmlReadFile(
			job.document.savePath.c_str(),
			nullptr,
			HTML_PARSE_RECOVER | HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR
	);
	if(!docPtr) {
		return;
	}
	destructors.push([docPtr](){
		xmlFreeDoc(docPtr);
	});
	
	xmlXPathContextPtr xpathCtx = xmlXPathNewContext(docPtr);
	if(nullptr == xpathCtx) {
		return;
	}
	destructors.push([xpathCtx](){
		xmlXPathFreeContext(xpathCtx);
	});
	
	xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)"//a", xpathCtx);
    if(nullptr == xpathObj) {
        return;
    }
	destructors.push([xpathObj](){
		xmlXPathFreeObject(xpathObj);
	});

	xmlNodeSetPtr nodes = xpathObj->nodesetval;
	int size = (nodes) ? nodes->nodeNr : 0;
    
    for(int i=0; i<size; ++i) {
		xmlNodePtr node = nodes->nodeTab[i];
		xmlChar *href = xmlGetProp(node, (const xmlChar*)"href");
		if(href) {
			xmlChar * fullHref = xmlBuildURI(href, (const xmlChar*)job.document.originUrl.c_str());
			if(fullHref) {
				feedback.AddHyperlink(job, (char*)fullHref);
				xmlFree(fullHref);
			}
			xmlFree(href);
		}
	}
	
}
