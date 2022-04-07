#ifndef RULEEXTENSION_H
#define RULEEXTENSION_H

#include "ruleBase.h"
#include <filesystem>

class RuleExtension: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	RuleExtension(Mode _mode, const std::string& _ext);
	void	process(const std::filesystem::path& name);
	static void	test();
private:
	Mode		mode;
	std::string ext;
};

#endif
