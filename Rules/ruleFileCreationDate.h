#ifndef RULEFILECREATIONDATE_H
#define RULEFILECREATIONDATE_H

#include "ruleBase.h"
#include <filesystem>

class RuleFileCreationDate: public RuleBase
{
public:
	explicit RuleFileCreationDate(const std::string& format);
	std::string	process(const RuleParams&) override;
private:
	std::string format;

};

#endif
