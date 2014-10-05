#ifndef XMLPARSER_H
#define	XMLPARSER_H

#include "parse_interface.h"

class XmlParser: virtual public ParserInterface {
public:
	void Parse(ParseJob job, ParseFeedbackInterface& feedback);
};

#endif	/* XMLPARSER_H */
