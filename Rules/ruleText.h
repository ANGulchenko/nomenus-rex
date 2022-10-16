#ifndef RULETEXT_H
#define RULETEXT_H

#include "ruleBase.h"

class RuleText: public RuleBase
{
public:
	explicit RuleText(const std::string& text);
	std::string	process(const RuleParams&) override;
private:
	std::string text;
};

#endif
