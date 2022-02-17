#include <iostream>
#include <filesystem>
#include <time.h>
#include <vector>
#include <map>
#include <set>

#include "renamer.h"
#include "version.h"
#include "CLIparser.h"


namespace fs = std::filesystem;


int main(int argc, char *argv[])
{
	// report version
	std::cout << "Version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
	std::cout << "Use -h or --help parameter for help." << std::endl;


	std::string source_dir = {""};
	std::string destination_dir = {""};
	std::string config_file = {""};

	CLIparser(argc, argv, source_dir, destination_dir, config_file);

	if (config_file == "")
	{
		std::cerr << "No config file specified!" << std::endl;
		exit(EXIT_FAILURE);
	}



	Renamer renamer(fs::path{source_dir}, fs::path{destination_dir}, fs::path{config_file});

	renamer.createRenameBijectionMap();
	renamer.testRenameBijectionMap();
	renamer.printRenameBijectionMap();


	return 0;
}
