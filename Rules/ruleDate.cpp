#include "ruleDate.h"
#include <iostream>

RuleDate::RuleDate(const std::string& format)
	: RuleBase(RuleType::Date)
{
	const size_t buffer_size = 80;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [buffer_size];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	size_t res = strftime (buffer, buffer_size, format.c_str(), timeinfo);
	if (res == 0)
	{
		std::cerr<< "Result string for date is too long and exceeds the size of the buffer("<<buffer_size<<")\n";
		exit(EXIT_FAILURE);
	}

	result = std::string(buffer);
}
