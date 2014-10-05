#ifndef EASYDOWNLOADER_H
#define	EASYDOWNLOADER_H

#include "DonloadJob.h"
#include "Repository.h"
#include "IDownloadFeedback.h"

class EasyDownloader {
public:
	void download(const DownloadJob &job, Repository &repository, IDownloadFeedback &feedback);


private:
	static size_t curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
};

#endif	/* EASYDOWNLOADER_H */

