#ifndef IPARSEFEEDBACK_H
#define	IPARSEFEEDBACK_H

#include "ParseJob.h"

struct IParseFeedback {
	virtual ~IParseFeedback() {}
	
	virtual void addHyperlink(const ParseJob &job, std::string url) = 0;
};

#endif	/* IPARSEFEEDBACK_H */
