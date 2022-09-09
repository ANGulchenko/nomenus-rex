#include "cfgvarsingleton.h"
#include <iostream>

CfgVarSingleton::CfgVarSingleton()
	: ask_confirmation_for_file_processing {true}
	, verbose {true}
	, amount_of_bijections_printed {10}
{
	nomenus_ver_str = std::to_string(VERSION_MAJOR)+"."+std::to_string(VERSION_MINOR)+"."+std::to_string(VERSION_PATCH);
}

void cfg::print(const std::string& str)
{
	if (CfgVarSingleton::Instance().verbose)
	{
		std::cout << str;
	}
}
