#ifndef XMLPARSER_H
#define	XMLPARSER_H

#include "parse_interface.h"

class XmlParser: public ParserBase {
public:
	void Parse(ParseJob job);
};

#endif	/* XMLPARSER_H */
