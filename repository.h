#ifndef REPOSITORY_H_
#define	REPOSITORY_H_

#include <string>


// Отдельный сохраненный файл
class RepositoryDocument {
public:
	static RepositoryDocument create(
			const std::string& origin_url,
			const std::string& save_path
	);
	
	std::string origin_url;
	std::string save_path;
};


// Репозиторий для сохранения файлов
class Repository {
public:
	// Конструктор с заданием базовой директории для сохранения файлов
	explicit Repository(const std::string &base_path);

	// Генерирует путь для сохранения файла на основе URL
	std::string PathFromUrl(std::string url);

	// Создает документ на основе URL
	RepositoryDocument CreateDocument(std::string url);
	
	// Экранирует элемент пути файла
	std::string SanitizePath(const std::string &path);
private:
	// Базовая директория для сохранения файлов
	std::string base_path_;
};


#endif	/* REPOSITORY_H_ */
