#include <vector>
#include <iostream>
#include "ruleBase.h"

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
		case RuleType::FileCreationDate:return "RuleType::FileCreationDate";
		case RuleType::Replace:			return "RuleType::Replace";
	}

	return "ERROR";
}

std::string	RuleBase::getString() const
{
	return result;
}


