#include "ruleFileCreationDate.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>


RuleFileCreationDate::RuleFileCreationDate(const std::string& _format)
	: format{_format}
{

}

std::string	RuleFileCreationDate::process(const RuleParams& params)
{
	//std::string result;
	struct stat st;
	const size_t buffer_size = 80;
	char buffer [buffer_size];

	stat(params.absolute_path.c_str(), &st);
	time_t rawtime = st.st_mtim.tv_sec;

	struct tm* timeinfo = localtime (&rawtime);

	size_t res = strftime (buffer, buffer_size, format.c_str(), timeinfo);
	if (res == 0)
	{
		std::cerr<< "Result string for date is too long and exceeds the size of the buffer("<<buffer_size<<")\n";
		exit(EXIT_FAILURE);
	}

	//result = std::string(buffer);
	return std::string(buffer);

}
