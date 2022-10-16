#ifndef RULEINTEGER_H
#define RULEINTEGER_H

#include "ruleBase.h"
#include <filesystem>
#include <map>

class RuleInteger: public RuleBase
{
public:
	enum class Mode {global, local_at_every_dir};

	RuleInteger(Mode _mode, int _start, int _step, int _padding);
	std::string	process(const RuleParams&) override;
private:
	int			number;
	std::map<std::filesystem::path, int>	number_per_path;
	int			step;
	Mode		mode;
	int			zero_padding_size;
};

#endif
