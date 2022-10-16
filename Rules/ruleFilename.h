#ifndef RULEFILENAME_H
#define RULEFILENAME_H

#include "ruleBase.h"
#include <filesystem>

class RuleFilename: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	explicit RuleFilename(Mode _mode);
	std::string	process(const RuleParams&) override;
private:
	Mode		mode;
};

#endif
