#ifndef REPOSITORY_H
#define	REPOSITORY_H

#include <string>

#include "Document.h"

class Repository {
public:
	/**
     * @param basePath Корневая директория для сохранения файлов
     */
	Repository(const std::string &basePath);

	/**
	 * Генерирует путь для сохранения файла на основе URL
     * @param url Исходный URL
     * @return Уникальный безопасный путь
     */
	std::string pathFromUrl(std::string url);
	
	Document createDocument(std::string url);
	
	std::string sanitizePath(const std::string &path);
private:
	std::string basePath;
};

#endif	/* REPOSITORY_H */

