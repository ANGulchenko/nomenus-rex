#include <vector>
#include <iostream>
#include "ruleBase.h"

void testsCmp(int testIndex, RuleBase& rule, const std::string& correct_answer)
{
	if (std::string res = rule.getString(); res != correct_answer)
	{
		std::cerr << "Test №" << testIndex << ": "
				  << rule.getTypeStr()<<" getString() == \"" << res << "\""
				  << " but should be \""<< correct_answer <<"\"" << std::endl;
		exit(EXIT_FAILURE);
	}
}



RuleBase::RuleBase(RuleType _type)
	: type(_type)
{

}

RuleType RuleBase::getType() const
{
	return type;
}

std::string RuleBase::getTypeStr() const
{
	//{Date, Text, Integer, Extension, Dir, Filename, Filesize, SubStrReplace};
	switch (type)
	{
		case RuleType::Date:			return "RuleType::Date";
		case RuleType::Text:			return "RuleType::Text";
		case RuleType::Integer:			return "RuleType::Integer";
		case RuleType::Extension:		return "RuleType::Extension";
		case RuleType::Dir:				return "RuleType::Dir";
		case RuleType::Filename:		return "RuleType::Filename";
		case RuleType::Filesize:		return "RuleType::Filesize";
		case RuleType::Replace:			return "RuleType::Replace";
	}

	return "ERROR";
}

std::string	RuleBase::getString() const
{
	return result;
}


