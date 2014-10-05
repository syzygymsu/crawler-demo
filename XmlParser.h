#ifndef XMLPARSER_H
#define	XMLPARSER_H

#include "Document.h"
#include "IParseFeedback.h"

class XmlParser {
public:
	void parseDocument(ParseJob &job, IParseFeedback& feedback);

};

#endif	/* XMLPARSER_H */
