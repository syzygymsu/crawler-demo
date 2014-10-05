#include "curl_easy_downloader.h"

#include <fstream>

#include <curl/curl.h>

struct CurlUserData {
	std::ofstream writeStream;
};

void EasyDownloader::download(const DownloadJob &job, Repository &repository, IDownloadFeedback &feedback) {
	Document document = repository.createDocument(job.url);
	
	CurlUserData userData;
	userData.writeStream.open(document.savePath);
	
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, job.url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, static_cast<void*>(&userData));
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &EasyDownloader::curlWriteCallback);
	
	CURLcode res = curl_easy_perform(handle);
	
	if(CURLE_OK == res) {
		long httpCode = 0;
		curl_easy_getinfo (handle, CURLINFO_RESPONSE_CODE, &httpCode);
		
		switch(httpCode) {
			case 200:
				feedback.addDocument(job, document);
				break;

			case 301:
			case 302:
			case 303:
			case 307:
				char *redirectUrl;
				curl_easy_getinfo(handle, CURLINFO_REDIRECT_URL, &redirectUrl);
				feedback.addRedirect(job, redirectUrl);
				break;
				
			default:
				// игнорируем неизвестные коды ответа
				break;
		}
	} else {
		// TODO: добавить обработку ошибок скачивания
	}

	curl_easy_cleanup(handle);
}

size_t EasyDownloader::curlWriteCallback(char *buffer, size_t blockSize, size_t blocksCount, void *userDataPtr) {
	size_t totalBytes = blockSize*blocksCount;
	if(totalBytes) {
		CurlUserData &userData = *static_cast<CurlUserData*>(userDataPtr);
		userData.writeStream.write(buffer, totalBytes);
	}
	return totalBytes;
}
