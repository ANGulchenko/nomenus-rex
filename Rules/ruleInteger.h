#ifndef RULEINTEGER_H
#define RULEINTEGER_H

#include "ruleBase.h"
#include <filesystem>
#include <map>

class RuleInteger: public RuleBase
{
public:
	enum class Mode {global, local_at_every_dir};

	RuleInteger(Mode _mode, int _start, int _step);
	void	process(const std::filesystem::path& name);
private:
	int			number;
	std::map<std::filesystem::path, int>	number_per_path;
	int			step;
	Mode		mode;
};

#endif
