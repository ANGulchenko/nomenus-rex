#include "ruleDir.h"

#include <vector>

RuleDir::RuleDir(Mode _mode, const std::string& _separator)
	: RuleBase(RuleType::Dir)
	, mode {_mode}
	, separator {_separator}
{

}

void	RuleDir::process(const RuleParams& params)
{
	RuleBase::result.clear();
	std::vector<std::string> path;
	for (auto it = params.relative_path.begin(); it != params.relative_path.end(); ++it)
	{
		path.push_back(*it);
	}

	path.pop_back(); // remove filename. We need only dir names.

	if (mode == Mode::parent_only && path.size() >= 1)
	{
		result = path[path.size()-1];
	}else
	if (mode == Mode::whole && path.size() > 0)
	{
		for (auto& dir: path)
		{
			result += dir + separator;
		}

		// Removing the separator from the end of the string
		result.erase(result.size() - separator.size());
	}
}

