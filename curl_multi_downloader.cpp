#include "curl_multi_downloader.h"

#include <fstream>

#include "download_interface.h"

namespace {


// Локальный вспомогательный класс для передачи данных в callback
struct CurlUserData {
	std::ofstream write_stream;
	DownloadJob job;
	RepositoryDocument document;
};


// Локальная функция для сохранения данных из curl
size_t CurlWriteCallback(char *buffer, size_t block_size, size_t blocks_count, void *user_data_ptr) {
	size_t total_bytes = block_size*blocks_count;
	if(total_bytes) {
		CurlUserData &user_data = *static_cast<CurlUserData*>(user_data_ptr);
		if(!user_data.write_stream.is_open()) {
			user_data.write_stream.open(user_data.document.save_path);
		}
		user_data.write_stream.write(buffer, total_bytes);
	}
	return total_bytes;
}


} // anonymous namespace


CurlMultiDownloader::CurlMultiDownloader() {
	curl_multi_handle_ = curl_multi_init();
}


CurlMultiDownloader::~CurlMultiDownloader() {
	// TODO освободить оставшиеся easy-хендлы.
	// Для этого понадобится отдельно держать их список.
	// И при штатном завершении этот список должен быть пустым.
	
	curl_multi_cleanup(curl_multi_handle_);
}


void CurlMultiDownloader::Download(DownloadJob job) {
	CurlUserData &user_data = *new CurlUserData();
	user_data.job = job;
	user_data.document = repository().CreateDocument(job.url);
	
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, job.url.c_str());
	curl_easy_setopt(handle, CURLOPT_PRIVATE, static_cast<void*>(&user_data));
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, static_cast<void*>(&user_data));
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &CurlWriteCallback);
	
	curl_multi_add_handle(curl_multi_handle_, handle);
	
	++count_;
}


void CurlMultiDownloader::PerformOne() {
	int running_handles;
	curl_multi_perform(curl_multi_handle_, &running_handles);
	
	// Обрабатываем сообщения от отдельных загрузок
	int messages_left;
	CURLMsg *message;
	while(message = curl_multi_info_read(curl_multi_handle_, &messages_left)) {
		if(CURLMSG_DONE == message->msg) {
			void *user_data_ptr;
			curl_easy_getinfo(message->easy_handle, CURLINFO_PRIVATE, &user_data_ptr);
			CurlUserData &user_data = *static_cast<CurlUserData*>(user_data_ptr);
			
			if(user_data.write_stream.is_open()) {
				user_data.write_stream.close();
			}
			
			if(CURLE_OK == message->data.result) {
				long response_code = 0;
				CURLcode result = curl_easy_getinfo(message->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
				if(CURLE_OK == result) {
					switch(response_code) {
						case 200: // OK
							feedback().AddDocument(user_data.job, user_data.document);
							break;

						case 301: // Moved Permanently
						case 302: // Moved Temporarily
						case 303: // See Other
						case 307: // Temporary Redirect
							char *redirect_url;
							result = curl_easy_getinfo(message->easy_handle, CURLINFO_REDIRECT_URL, &redirect_url);
							if(CURLE_OK == result) {
								feedback().AddRedirect(user_data.job, redirect_url);
							}
							break;

						default:
							// игнорируем неизвестные коды ответа
							break;
					}
				}
			} else {
				// TODO: добавить обработку ошибок скачивания
			}
			
			curl_multi_remove_handle(curl_multi_handle_, message->easy_handle);
			delete &user_data;
			--count_;
		}
	}
}
