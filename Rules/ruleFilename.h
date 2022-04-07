#ifndef RULEFILENAME_H
#define RULEFILENAME_H

#include "ruleBase.h"
#include <filesystem>

class RuleFilename: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	RuleFilename(Mode _mode);
	void	process(const std::filesystem::path& name);
	static void	test();
private:
	Mode		mode;
};

#endif
