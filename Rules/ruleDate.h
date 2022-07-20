#ifndef RULEDATE_H
#define RULEDATE_H

#include "ruleBase.h"
#include <filesystem>

class RuleDate: public RuleBase
{
public:
	explicit RuleDate(const std::string& format);
	void	process(const RuleParams& params) override;
};

#endif
