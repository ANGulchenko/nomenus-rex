#ifndef RULEFILENAME_H
#define RULEFILENAME_H

#include "ruleBase.h"
#include <filesystem>

class RuleFilename: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	RuleFilename(Mode _mode);
	void	process(std::filesystem::path& name);
private:
	Mode		mode;
};

#endif
