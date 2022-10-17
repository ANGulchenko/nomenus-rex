#ifndef CFGVARSINGLETON_H
#define CFGVARSINGLETON_H

#include <string>
#include "version.h"

class CfgVarSingleton
{
public:
		static CfgVarSingleton& Instance()
		{
				static CfgVarSingleton theSingleInstance;
				return theSingleInstance;
		}

		const std::string	nomenus_ver_str;
		bool				ask_confirmation_for_file_processing;
		bool				verbose;
		int					amount_of_bijections_printed;

private:
		CfgVarSingleton();
		CfgVarSingleton(const CfgVarSingleton& root) = delete;
		CfgVarSingleton& operator=(const CfgVarSingleton&) = delete;
};

namespace cfg
{
	void print(const std::string& str);
}

#endif
