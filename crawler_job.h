#ifndef CRAWLERJOB_H_
#define	CRAWLERJOB_H_

#include <vector>
#include <string>


// Данные, определяющие задание для краулера
struct CrawlerJob {
	// Список начальных URL.
	std::vector<std::string> initial_urls;
	// Список фильтров (загружаемые URL должны содержать хотя бы одну из данных
	// подстрок). Если список пуст, то фильтрация не применяется.
	std::vector<std::string> must_contain;
	
	// Максимальное общее количество загруженных файлов. -1 -- без ограничений.
	int max_count = -1;
	// Максимальная вложенность поиска. -1 -- без ограничений.
	int max_depth = -1;
};


#endif	/* CRAWLERJOB_H_ */
