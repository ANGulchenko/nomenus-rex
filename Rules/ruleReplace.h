#ifndef RULEREPLACE_H
#define RULEREPLACE_H

#include "ruleBase.h"

class RuleReplace: public RuleBase
{
public:
	RuleReplace(const std::string& _what, const std::string& _to);
	void	process(const RuleParams& params) override;
private:
	std::string what;
	std::string to;
};

#endif
