#ifndef SIMPLECRAWLER_H_
#define	SIMPLECRAWLER_H_

#include <queue>
#include <set>
#include <string>

#include "repository.h"
#include "crawler_job.h"
#include "curl_easy_downloader.h"
#include "libxml2_parser.h"


// Простой краулер с очередью на загрузку, сразу же парсит скачанные файлы.
// Сам же предоставляет интерфейсы обратной связи для парсера и загрузчика.
class SimpleCrawler:
		virtual public DownloadFeedbackInteraface,
		virtual public ParseFeedbackInterface {
public:
	// Конструктор с установкой зависимостей
	SimpleCrawler(Repository &repository, CrawlerJob &job);
	
	// Запуск краулера
	void Execute();
private:
	// реализация DownloadFeedbackInteraface
	void AddRedirect(DownloadJob job, std::string url) override;
	void AddDocument(DownloadJob job, Document document) override;
	
	// реализация ParseFeedbackInterface
	void AddHyperlink(ParseJob job, std::string url) override;

	// добавление URL в очередь с дополнительными проверками
	void AddUrl(std::string url, int depth);

	// Репозиторий для сохранения скачанных файлов
	Repository &repository_;
	// Исходная задача
	CrawlerJob &crawler_job_;
	
	// Загрузчик
	CurlEasyDownloader downloader_;
	// Парсер
	Libxml2Parser parser_;
	
	// Набор обработанных URL (чтобы не скачивать их повторно)
	std::set<std::string> known_urls_;
	// Очередь задач на загрузку
	std::queue<DownloadJob> download_queue_;
};


#endif	/* SIMPLECRAWLER_H_ */
