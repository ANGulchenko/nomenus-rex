#ifndef RULEDIR_H
#define RULEDIR_H

#include "ruleBase.h"
#include <filesystem>

class RuleDir: public RuleBase
{
public:
	enum class Mode {whole, parent_only};
	RuleDir(Mode _mode, std::string _separator);
	void	process(std::filesystem::path& name);
private:
	Mode		mode;
	std::string	separator;
};

#endif
