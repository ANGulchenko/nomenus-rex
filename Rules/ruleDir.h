#ifndef RULEDIR_H
#define RULEDIR_H

#include "ruleBase.h"
#include <filesystem>

class RuleDir: public RuleBase
{
public:
	RuleDir();
	void	process(std::filesystem::path& name);
};

#endif
