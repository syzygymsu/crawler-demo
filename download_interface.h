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
	
	// Задает необходимые зависимости. Вызывающий код обязан обеспечить
	// существование объектов зависимостей во время работы функции Download
	virtual void init(
			Repository &repository, // Репозиторий, в который будут сохраняться
			                        // скачанные документы
			DownloadFeedbackInteraface &feedback // Интерйес обратной связи
	) = 0;

	// Выполняет задание на загрузку.
	virtual void Download(
			DownloadJob job // Задание на загрузку
	) = 0;
};


// Базовый абстраутный класс загрузчика. Реализовано сохранение зависимостей.
class DownloaderBase: virtual public DownloaderInterface {
public:
	inline void init(
			Repository &repository,
			DownloadFeedbackInteraface &feedback ) {
		repository_ = &repository;
		feedback_ = &feedback;
	}
protected:
	inline Repository &repository() {
		return *repository_;
	}
	inline DownloadFeedbackInteraface &feedback() {
		return *feedback_;
	}
private:
	Repository *repository_;
	DownloadFeedbackInteraface *feedback_;
};

#endif	/* DOWNLOAD_INTERFACE_H_ */
