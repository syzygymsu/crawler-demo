#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <curl/curl.h>
#include <libxml/parser.h>
#include <boost/program_options.hpp>

#include "Repository.h"
#include "CrawlerJob.h"
#include "SimpleCrawler.h"

namespace po = boost::program_options;

void boot() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
	xmlInitParser();
}

void shutdown() {
	curl_global_cleanup();
	xmlCleanupParser();
}

int main(int argc, char** argv) {
	boot();

	try {
		std::string basePath;
		int maxDepth, maxCount;
		
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "show help message")
			("init,i", po::value<std::vector<std::string> >(), "initial URLs")
			("contains,c", po::value<std::vector<std::string> >(), "download only URLs, containing one of this substrings")
			("depth,d", po::value<int>(&maxDepth)->default_value(5), "Maximum depth, -1 for no limit")
			("max,m", po::value<int>(&maxCount)->default_value(100), "Maximum number of URLs, -1 for no limit")
			("save,s", po::value<std::string>(&basePath)->default_value("sites"), "Save path")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			return 1;
		}

		CrawlerJob job;
		
		job.maxDepth = maxDepth;
		job.maxCount = maxCount;
		
		if(vm.count("init")) {
			
			for(auto &url: vm["init"].as<std::vector<std::string>>()) {
				job.initialUrls.push_back(url);
			}
		}
		if(vm.count("contains")) {
			for(auto & url: vm["contains"].as<std::vector<std::string>>()) {
				job.mustContain.push_back(url);
			}
		}
		
		
		// непосредственно запуск
		Repository repo(basePath);
		SimpleCrawler crawler(repo, job);
		crawler.execute();
	} catch(std::string s) {
		std::cout << "Exception: " << s << std::endl;
	} catch(std::runtime_error e) {
		std::cout << "Runtime error: " << e.what() << std::endl;
	}

	shutdown();
	
	return 0;
}

