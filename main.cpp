#include <iostream>
#include <string>

#include "renamer.h"
#include "configurationparser.h"
#include "cfgvarsingleton.h"
#include "Tests/tests.h"

int main(int argc, char *argv[])
{
	#if !defined(NDEBUG)
	tests();
	#endif

	Renamer renamer;
	// ConfigurationParser also fills CfgVarSingleton
	ConfigurationParser cfg_parser(argc, argv, renamer);

	vcout << "Nomenus-rex(" << CfgVarSingleton::Instance().nomenus_ver_str <<")\n";

	renamer.createRenameBijection();
	renamer.testRenameBijection();
	renamer.printRenameBijection();


	if (CfgVarSingleton::Instance().ask_confirmation_for_file_processing)
	{
		//v1cout new_cout;
		std::cerr << "Press 'y' to process the files. Other button to cancel." << std::endl;
		char approvement;
		std::cin >> approvement;
		if (approvement == 'y')
		{
			renamer.executeRenameBijection();
			vcout << "The process is done." << std::endl;
		}else
		{
			vcout << "The process is canceled." << std::endl;
		}
	}else
	{
		renamer.executeRenameBijection();
	}

	return 0;
}
