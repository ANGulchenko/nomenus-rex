#ifndef RULEEXEC_H
#define RULEEXEC_H

#include "ruleBase.h"
#include <filesystem>

class RuleExec: public RuleBase
{
public:
	RuleExec(const std::string& command, const std::string& placeholder);
	std::string	process(const RuleParams&) override;
private:
	std::string command;
	std::string placeholder;

};

#endif

