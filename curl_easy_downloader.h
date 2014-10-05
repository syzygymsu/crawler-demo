#ifndef EASYDOWNLOADER_H
#define	EASYDOWNLOADER_H

#include "repository.h"
#include "download_interface.h"

class EasyDownloader: public DownloaderBase {
public:
	void Download(DownloadJob job);
private:
	static size_t curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
};

#endif	/* EASYDOWNLOADER_H */
