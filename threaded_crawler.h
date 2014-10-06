#ifndef THREADEDCRAWLER_H_
#define	THREADEDCRAWLER_H_

#include <queue>
#include <set>
#include <string>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "repository.h"
#include "crawler_job.h"
#include "curl_multi_downloader.h"
#include "libxml2_parser.h"


class ThreadedCrawler:
		virtual public DownloadFeedbackInteraface,
		virtual public ParseFeedbackInterface {
public:
	// Конструктор с установкой зависимостей
	ThreadedCrawler(Repository &repository, CrawlerJob &job);
	
	// Запуск краулера
	void Execute();
private:
	// реализация DownloadFeedbackInteraface
	void AddRedirect(DownloadJob job, std::string url) override;
	void AddDocument(DownloadJob job, RepositoryDocument document) override;
	
	// реализация ParseFeedbackInterface
	void AddHyperlink(ParseJob job, std::string url) override;
	
	// добавление URL в очередь с дополнительными проверками
	void AddUrl(std::string url, int depth);

	// Стартовая функция треда парсинга
	void ParseThread();
	// Стартовая функция треда загрузки
	void DownloadThread();
	
	// Репозиторий для сохранения скачанных файлов
	Repository &repository_;
	// Исходная задача
	CrawlerJob &crawler_job_;
	
	// Загрузчик
	CurlMultiDownloader downloader_;
	// Парсер
	Libxml2Parser parser_;
	
	// Набор обработанных URL (чтобы не скачивать их повторно)
	std::set<std::string> known_urls_;
	// Очередь задач на загрузку
	std::queue<DownloadJob> download_queue_;
	// Очередь задач на парсинг
	std::queue<ParseJob> parse_queue_;
	
	// true если все треды должны выходить
	std::atomic<bool> quit_;
	// Количество живых тредов, которые потенциально могут добавить задачу
	std::atomic<int> live_threads_;
	
	// Мьютекс на очередь загрузки и список известных URL
	std::mutex download_mutex_;
	// Мьютекс на очередь парсинга
	std::mutex parse_mutex_;

	std::condition_variable download_condition_;
	std::condition_variable parse_condition_;
};


#endif	/* THREADEDCRAWLER_H_ */
