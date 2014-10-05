#ifndef XMLPARSER_H
#define	XMLPARSER_H

#include "document.h"
#include "parse_feedback.h"

class XmlParser {
public:
	void parseDocument(ParseJob &job, IParseFeedback& feedback);

};

#endif	/* XMLPARSER_H */
