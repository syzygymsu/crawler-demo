#include "curl_easy_downloader.h"

#include <fstream>

#include <curl/curl.h>


namespace {

	
// Локальный вспомогательный класс для передачи данных в callback
struct CurlUserData {
	std::ofstream write_stream;
};


// Локальная функция для сохранения данных из curl
size_t CurlWriteCallback(char *buffer, size_t block_size, size_t blocks_count, void *user_data_ptr) {
	size_t total_bytes = block_size*blocks_count;
	if(total_bytes) {
		CurlUserData &user_data = *static_cast<CurlUserData*>(user_data_ptr);
		user_data.write_stream.write(buffer, total_bytes);
	}
	return total_bytes;
}


} // anonymous namespace


void CurlEasyDownloader::Download(DownloadJob job) {
	CURLcode result;
	
	RepositoryDocument document = repository().CreateDocument(job.url);
	
	CurlUserData user_data;
	user_data.write_stream.open(document.save_path);
	
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, job.url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, static_cast<void*>(&user_data));
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &CurlWriteCallback);
	
	result = curl_easy_perform(handle);
	
	user_data.write_stream.close();
	
	if(CURLE_OK == result) {
		long response_code = 0;
		result = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response_code);
		if(CURLE_OK == result) {
			switch(response_code) {
				case 200: // OK
					feedback().AddDocument(job, document);
					break;

				case 301: // Moved Permanently
				case 302: // Moved Temporarily
				case 303: // See Other
				case 307: // Temporary Redirect
					char *redirect_url;
					result = curl_easy_getinfo(handle, CURLINFO_REDIRECT_URL, &redirect_url);
					if(CURLE_OK == result) {
						feedback().AddRedirect(job, redirect_url);
					}
					break;

				default:
					// игнорируем неизвестные коды ответа
					break;
			}
		}
	} else { // curl_easy_perform failed
		// TODO: добавить обработку ошибок скачивания
	}

	curl_easy_cleanup(handle);
}
