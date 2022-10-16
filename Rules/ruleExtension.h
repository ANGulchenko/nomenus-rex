#ifndef RULEEXTENSION_H
#define RULEEXTENSION_H

#include "ruleBase.h"
#include <filesystem>

class RuleExtension: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	RuleExtension(Mode _mode, const std::string& _ext);
	std::string	process(const RuleParams&) override;
private:
	Mode		mode;
	std::string ext;
};

#endif
