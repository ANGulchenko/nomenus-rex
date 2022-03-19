#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <iostream>
#include <filesystem>
#include "renamer.h"
#include <libconfig.h++>

using namespace libconfig;

class ConfigurationParser
{
public:
	ConfigurationParser(int argc, char *argv[], Renamer& renamer);

private:
	template<typename T>
	void getRuleVar(const Setting &rule,
				const std::string& var_name,
				const std::string& rule_name,
				T& var)
	{
		bool var_present = rule.lookupValue(var_name, var);
		if (!var_present)
		{
			std::cerr << "\nERROR: There is no '" << var_name << "' variable in the '" << rule_name <<"' rule." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	template<typename T>
	void getVar(const Setting &rule,
				const std::string& var_name,
				T& var)
	{
		bool var_present = rule.lookupValue(var_name, var);
		if (!var_present)
		{
			std::cerr << "\nERROR: There is no '" << var_name << "' variable" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void		makeSureThatConfigDirIsExist();
	std::string	getConfigPathString(std::string raw_parameter);
};

#endif // CONFIGURATIONPARSER_H
