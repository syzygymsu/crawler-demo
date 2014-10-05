// Классы для взаимодействия парсера с вызывающим кодом.

#ifndef PARSE_INTERFACE_H_
#define	PARSE_INTERFACE_H_

#include "document.h"


// Данные, определяющие отдельное задание для парсера
struct ParseJob {
	// Документ для парсинга
	Document document;
	
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
	
	// Выполняет задание на парсинг.
	// Вызывающий код обязан обеспечить существование объекта feedback
	// до окончания выполнения функции.
	virtual void Parse(
			ParseJob job, // Задание на парсинг
			ParseFeedbackInterface &feedback // Интерфейс обратной связи
	) = 0;
};


#endif	/* PARSE_INTERFACE_H_ */
