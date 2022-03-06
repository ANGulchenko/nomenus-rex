#include "ruleDate.h"

RuleDate::RuleDate(const std::string& format)
	: RuleBase(RuleType::Date)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer, 80, format.c_str(), timeinfo);

	result = std::string(buffer);
}
