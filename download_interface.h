// Классы для взаимодействия загрузчика с вызывающим кодом.

#ifndef DOWNLOAD_INTERFACE_H_
#define	DOWNLOAD_INTERFACE_H_

#include <string>

#include "document.h"
#include "repository.h"


// Данные, определяющие отдельное задание для загрузчика
struct DownloadJob {
	// Адрес загрузки
	std::string url;
	
	// Глубина вложенности данного задания
	int depth;
};


// Интерфейс обратной связи, через который загрузчик должен уведомлять
// вызывающий код о результатах работы
class DownloadFeedbackInteraface {
public:
	virtual ~DownloadFeedbackInteraface() {}
	
	// Вызывается при обнаружении перенаправления
	virtual void AddRedirect(
			DownloadJob job, // оригинальное задание
			std::string url // адрес перенаправления
	) = 0;
	
	// Вызывается при успешной загрузке документа
	virtual void AddDocument(
			DownloadJob job, // оригинальное задание
			Document document // загруженный документ
	) = 0;
};


// Общий интерфейс для загрузчика
class DownloaderInterface {
public:
	virtual ~DownloaderInterface() {}

	// Выполняет задание на загрузку.
	// Вызывающий код обязан обеспечить существование объектов repository
	// и feedback до окончания выполнения функции.
	virtual void Download(
			DownloadJob job, // Задание на загрузку
			Repository &repository, // Репозиторий, в который будут сохраняться
			                        // скачанные документы
			DownloadFeedbackInteraface &feedback // Интерфейс обратной связи
	) = 0;
};


#endif	/* DOWNLOAD_INTERFACE_H_ */
