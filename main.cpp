#include <iostream>
#include <filesystem>
#include <time.h>
#include <vector>
#include <map>
#include <set>

#include "renamer.h"
#include "configurationparser.h"
#include "Tests/tests.h"


namespace fs = std::filesystem;


int main(int argc, char *argv[])
{
	//tests();

	Renamer renamer;
	ConfigurationParser cfg_parser(argc, argv, renamer);

	renamer.createRenameBijection();
	renamer.printRenameBijection();
	renamer.testRenameBijection();

	std::cout << "Press 'y' to process the files. Other button to cancel." << std::endl;
	char approvement;
	std::cin >> approvement;
	if (approvement == 'y')
	{
		renamer.executeRenameBijection();
		std::cout << "The process is done." << std::endl;
	}else
	{
		std::cout << "The process is canceled." << std::endl;
	}


	return 0;
}
