#include <iostream>
#include <filesystem>
#include <time.h>
#include <vector>
#include <map>
#include <set>

#include "renamer.h"
#include "version.h"
#include "configurationparser.h"


namespace fs = std::filesystem;


int main(int argc, char *argv[])
{
	// report version
	std::cout << "Version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
	std::cout << "Use -h or --help parameter for help." << std::endl;

	Renamer renamer;
	ConfigurationParser cfg_parser(argc, argv, renamer);

	renamer.createRenameBijectionMap();
	renamer.testRenameBijectionMap();
	renamer.printRenameBijectionMap();
	//renamer.executeRenameBijectionMap();


	return 0;
}
