#include "ruleExec.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <array>


RuleExec::RuleExec(const std::string& _command, const std::string& _placeholder)
	: command{_command}
	, placeholder{_placeholder}
{

}

std::string	RuleExec::process(const RuleParams& params)
{
	std::string result;

	/// CMD GENERATION /////////
	std::string finished_command = command;
	size_t pos = 0;
	while((pos = finished_command.find(placeholder, pos)) != std::string::npos)
	{
		finished_command.replace(pos, placeholder.length(), params.absolute_path);
		pos += placeholder.length();
	}
	//command = "echo '<Placeholder>' | grep -Eo '[0-9]+'";
	/// CMD EXECUTION //////////


	std::array<char, 128> buffer;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(finished_command.c_str(), "r"), pclose);
	if (!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}

	if (result.size() > 0 && result[result.size()-1] == '\n')
	{
		result.pop_back();
	}

	return result;
}
