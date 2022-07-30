#ifndef RULEBASE_H
#define RULEBASE_H

#include <filesystem>
#include <string>

struct RuleParams
{
	std::filesystem::path& absolute_path;
	std::filesystem::path& relative_path;
	std::string& name_in_process;
};

class RuleBase
{
public:
	std::string		getString() const;
	virtual void	process(const RuleParams&) = 0;

protected:
	std::string result;
};

#endif // RULE_H
