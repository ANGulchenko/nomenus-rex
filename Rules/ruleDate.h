#ifndef RULEDATE_H
#define RULEDATE_H

#include "ruleBase.h"
#include <filesystem>

class RuleDate: public RuleBase
{
public:
	RuleDate(const std::string& format);
};

#endif
