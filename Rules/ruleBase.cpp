#include <vector>

#include "ruleBase.h"


RuleBase::RuleBase(RuleType _type)
	: type(_type)
{

}

RuleType RuleBase::getType()
{
	return type;
}

std::string RuleBase::getTypeStr()
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

std::string	RuleBase::getString()
{
	return result;
}


