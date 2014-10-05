#ifndef URLINFO_H_
#define	URLINFO_H_

#include <string>


// Структура для хранения разбитого на части URL
// Общий вид URL:   schema://domain/path?query#hash
struct UrlInfo {
	std::string schema; // Компоненты URL
	std::string domain; //
	std::string path;   //
	std::string query;  //
	std::string hash;   //
};


// Функция по-умолчанию для разбиения URL. Передает вызов одной из реализаций.
UrlInfo ParseUrl(const std::string &url);


// Разбиение URL при помощи libxml2
UrlInfo ParseUrlLibxml2(const std::string &url);


#endif	/* URLINFO_H_ */
