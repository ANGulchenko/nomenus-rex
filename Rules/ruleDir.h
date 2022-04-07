#ifndef RULEDIR_H
#define RULEDIR_H

#include "ruleBase.h"
#include <filesystem>

class RuleDir: public RuleBase
{
public:
	enum class Mode {whole, parent_only};
	RuleDir(Mode _mode, std::string _separator);
	void	process(const std::filesystem::path& name);
	static void	test();
private:
	Mode		mode;
	std::string	separator;
};

#endif
