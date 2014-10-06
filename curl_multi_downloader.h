#ifndef CURLMULTIDOWNLOADER_H_
#define	CURLMULTIDOWNLOADER_H_

#include <curl/curl.h>

#include "download_interface.h"


// Реализация загрузчика на основе curl-multi
class CurlMultiDownloader: public DownloaderBase {
public:
	// Конструктор с инициализацией хендла
	CurlMultiDownloader();
	// Деструктор с освобождением хендла
	~CurlMultiDownloader();
	
	virtual void Download(DownloadJob job);
	
	inline size_t count() const { return count_; }
	
	void PerformOne();
private:
	// основной хендл curl-multi
	CURLM *curl_multi_handle_;
	// количество текущих загрузок
	size_t count_ = 0;
};


#endif	/* CURLMULTIDOWNLOADER_H_ */
