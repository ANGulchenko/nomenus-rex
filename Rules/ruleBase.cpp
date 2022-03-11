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

std::string	RuleBase::getString()
{
	return result;
}


