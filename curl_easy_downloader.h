#ifndef EASYDOWNLOADER_H
#define	EASYDOWNLOADER_H

#include "download_job.h"
#include "repository.h"
#include "download_feedback.h"

class EasyDownloader {
public:
	void download(const DownloadJob &job, Repository &repository, IDownloadFeedback &feedback);


private:
	static size_t curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
};

#endif	/* EASYDOWNLOADER_H */

