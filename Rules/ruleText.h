#ifndef RULETEXT_H
#define RULETEXT_H

#include "ruleBase.h"

class RuleText: public RuleBase
{
public:
	RuleText(const std::string& format);
	void	process(const RuleParams& params) override;
};

#endif
