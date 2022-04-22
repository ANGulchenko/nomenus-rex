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
	ConfigurationParser(int argc, char *argv[], bool& askConfirmationBeforeFileProcessing, Renamer& renamer);

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

	template<typename T>
	T enumParser(const Setting &settings,
				 const std::string& rule_type,
				 const std::string& var_name,
				 T /*useless_var*/, /*to help template to deduct the T type*/
				 const std::initializer_list<std::pair<const std::string, T>>& init_list
				 )
	{
		std::string str_mode;
		getRuleVar(settings, var_name, rule_type, str_mode);

		std::map<std::string, T> str2enum =  std::move(init_list);

		if (str2enum.find(str_mode) != str2enum.end())
		{
			T res = str2enum.at(str_mode);
			return res;
		}

		std::cerr << "\nERROR: Unknown value '" << str_mode << "' " <<
					 " of the variable '" << var_name << "' " <<
					 " in the rule '" << rule_type << "' \n";
		exit(EXIT_FAILURE);
	}

	void		makeSureThatConfigDirIsExist();
	std::string	getConfigPathString(std::string raw_parameter);
};

#endif // CONFIGURATIONPARSER_H
