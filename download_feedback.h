#ifndef IDOWNLOADFEEDBACK_H
#define	IDOWNLOADFEEDBACK_H

#include "download_job.h"
#include "document.h"

struct IDownloadFeedback {
	virtual ~IDownloadFeedback() {}
	
	virtual void addRedirect(const DownloadJob &job, std::string url) = 0;
	virtual void addDocument(const DownloadJob &job, Document &document) = 0;
};

#endif	/* IDOWNLOADFEEDBACK_H */
