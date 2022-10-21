#include "cfgvarsingleton.h"

CfgVarSingleton::CfgVarSingleton()
	: nomenus_ver_str { std::to_string(VERSION_MAJOR)+"."
					   +std::to_string(VERSION_MINOR)+"."
					   +std::to_string(VERSION_PATCH)
					  }
	, ask_confirmation_for_file_processing {true}
	, verbose {true}
	, amount_of_bijections_printed {10}
{

}

vstream vcout;


