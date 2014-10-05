#ifndef CURLEASYDOWNLOADER_H_
#define	CURLEASYDOWNLOADER_H_

#include "repository.h"
#include "download_interface.h"


// Простой загрузчик на основе curl-easy
class CurlEasyDownloader: public DownloaderBase {
public:
	void Download(DownloadJob job) override;
};


#endif	/* CURLEASYDOWNLOADER_H_ */
