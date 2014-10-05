#include "libxml2_parser.h"

#define LIBXML_HTML_ENABLED
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>

#include "destruction_stack.h"


void Libxml2Parser::Parse(ParseJob job) {
	DestructionStack destructors;
	
	// Парсим документ
	htmlDocPtr html_document = htmlReadFile(
			job.document.save_path.c_str(),
			nullptr, // encoding
			HTML_PARSE_RECOVER | HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR
	);
	if(!html_document) {
		return;
	}
	destructors.push([html_document](){
		xmlFreeDoc(html_document);
	});
	
	// Создаем контекст xpath
	xmlXPathContextPtr xpath_context = xmlXPathNewContext(html_document);
	if(!xpath_context) {
		return;
	}
	destructors.push([xpath_context](){
		xmlXPathFreeContext(xpath_context);
	});
	
	// Ищем по xpath
	xmlXPathObjectPtr xpath_object = xmlXPathEvalExpression(
			(const xmlChar*)"//a",
			xpath_context
	);
    if(!xpath_object) {
        return;
    }
	destructors.push([xpath_object](){
		xmlXPathFreeObject(xpath_object);
	});

	// Обрабатываем результаты поиска
	xmlNodeSetPtr nodes = xpath_object->nodesetval;
	int size = (nodes) ? nodes->nodeNr : 0;
    for(int i=0; i<size; ++i) {
		xmlNodePtr node = nodes->nodeTab[i];
		xmlChar *href = xmlGetProp(node, (const xmlChar*)"href");
		if(href) {
			// Преобразуем относительные пути в абсолютные
			xmlChar *url = xmlBuildURI(
					href,
					(const xmlChar*)job.document.origin_url.c_str()
			);
			if(url) {
				feedback().AddHyperlink(job, (const char*)url);
				xmlFree(url);
			}
			xmlFree(href);
		}
	}
}
