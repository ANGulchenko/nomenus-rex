#ifndef RULEEXTENSION_H
#define RULEEXTENSION_H

#include "ruleBase.h"
#include <filesystem>

class RuleExtension: public RuleBase
{
public:
	RuleExtension(bool lower_case = true);
	void	process(std::filesystem::path& name);
private:
	bool lower_case;
};

#endif
