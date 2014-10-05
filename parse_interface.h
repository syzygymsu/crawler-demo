// Классы для взаимодействия парсера с вызывающим кодом.

#ifndef PARSE_INTERFACE_H_
#define	PARSE_INTERFACE_H_

#include "repository.h"

// Данные, определяющие отдельное задание для парсера
struct ParseJob {
	// Документ для парсинга
	RepositoryDocument document;
	
	// Глубина вложенности данного задания
	int depth;
};


// Интерфейс обратной связи, через который парсер должен уведомлять вызывающий
// код о результатах работы
class ParseFeedbackInterface {
public:
	virtual ~ParseFeedbackInterface() {}
	
	// Вызывается при обнаружении гиперссылки
	virtual void AddHyperlink(
			ParseJob job, // Оригинальное задание
			std::string url // Адрес найденной гиперссылки
	) = 0;
};


// Общий интерфейс для парсера
class ParserInterface {
public:
	virtual ~ParserInterface() {}
	
	// Задает необходимые зависимости. Вызывающий код обязан обеспечить
	// существование объектов зависимостей во время работы функции Parse
	virtual void init(
			ParseFeedbackInterface &feedback // Интерфейс обратной связи
	) = 0;
	
	// Выполняет задание на парсинг.
	virtual void Parse(
			ParseJob job // Задание на парсинг
	) = 0;
};


// Базовый абстрактный класс парсера. Реализовано сохранение зависимостей.
class ParserBase: virtual public ParserInterface {
public:
	inline void init(ParseFeedbackInterface &feedback ) override {
		feedback_ = &feedback;
	}
protected:
	inline ParseFeedbackInterface &feedback() {
		return *feedback_;
	}
private:
	ParseFeedbackInterface *feedback_;
};


#endif	/* PARSE_INTERFACE_H_ */
