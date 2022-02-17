#include <vector>

#include "rule.h"


RuleBase::RuleBase(RuleType _type)
	: type(_type)
{

}

RuleType RuleBase::getType()
{
	return type;
}

std::string	RuleBase::getString()
{
	return result;
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

RuleText::RuleText(const std::string& text)
	: RuleBase(RuleType::Text)
{
	result = text;
}

//////////////////////////////////////////////////////////////////////

RuleInteger::RuleInteger(int _start, int _step, bool _reset_at_every_dir)
	: RuleBase(RuleType::Integer)
	, number(_start)
	, step(_step)
	, reset_at_every_dir(_reset_at_every_dir)
{

}

void	RuleInteger::process()
{
	result = std::to_string(number);
	number++;
}

//////////////////////////////////////////////////////////////////////

RuleExtension::RuleExtension(bool _lower_case)
	: RuleBase(RuleType::Extension)
	, lower_case(_lower_case)
{

}

void	RuleExtension::process(std::filesystem::path& name)
{
	result = name.extension();
}

//////////////////////////////////////////////////////////////////////

RuleDir::RuleDir()
	: RuleBase(RuleType::Dir)
{

}

void	RuleDir::process(std::filesystem::path& name)
{
	std::vector<std::string> path;
	for (auto it = name.begin(); it != name.end(); ++it)
	{
		path.push_back(*it);
	}

	if (path.size() > 1)
	{
		result = path[path.size()-2];
	}
}



