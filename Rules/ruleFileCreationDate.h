#ifndef RULEFILECREATIONDATE_H
#define RULEFILECREATIONDATE_H

#include "ruleBase.h"
#include <filesystem>

class RuleFileCreationDate: public RuleBase
{
public:
	RuleFileCreationDate(const std::string& format);
	void	process(const RuleParams& params) override;
private:
	std::string format;

};

#endif
