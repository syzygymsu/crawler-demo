#ifndef LIBXML2PARSER_H_
#define	LIBXML2PARSER_H_

#include "parse_interface.h"


// Парсер на основе libxml2
class Libxml2Parser: public ParserBase {
public:
	void Parse(ParseJob job) override;
};


#endif	/* LIBXML2PARSER_H_ */
