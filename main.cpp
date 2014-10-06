#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <curl/curl.h>
#include <libxml/parser.h>
#include <boost/program_options.hpp>

#include "repository.h"
#include "crawler_job.h"
#include "simple_crawler.h"

namespace po = boost::program_options;

void boot() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
	xmlInitParser();
}

void shutdown() {
	curl_global_cleanup();
	xmlCleanupParser();
}

int main(int arguments_count, char** arguments) {
	boot();

	try {
		std::string base_path;

		CrawlerJob job;
		
		po::options_description description("Allowed options");
		description.add_options()
			("help,h", "show help message")
			("init,i", po::value<std::vector<std::string> >(&job.initial_urls), "initial URLs")
			("contains,c", po::value<std::vector<std::string> >(&job.must_contain), "download only URLs, containing one of this substrings")
			("depth,d", po::value<int>(&job.max_depth)->default_value(5), "Maximum depth, -1 for no limit")
			("max,m", po::value<int>(&job.max_count)->default_value(100), "Maximum number of URLs, -1 for no limit")
			("save,s", po::value<std::string>(&base_path)->default_value("sites"), "Save path")
		;

		po::variables_map variables;
		po::store(po::parse_command_line(arguments_count, arguments, description), variables);
		po::notify(variables);

		if (variables.count("help")) {
			std::cout << description << std::endl;
			return 1;
		}

		// непосредственно запуск
		Repository repo(base_path);
		SimpleCrawler crawler(repo, job);
		crawler.Execute();
	} catch(std::string s) {
		std::cout << "Exception: " << s << std::endl;
	} catch(std::runtime_error e) {
		std::cout << "Runtime error: " << e.what() << std::endl;
	}

	shutdown();
	
	return 0;
}

