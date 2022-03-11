#ifndef RULEEXTENSION_H
#define RULEEXTENSION_H

#include "ruleBase.h"
#include <filesystem>

class RuleExtension: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	RuleExtension(Mode _mode, std::string _ext);
	void	process(std::filesystem::path& name);
private:
	Mode		mode;
	std::string ext;
};

#endif
