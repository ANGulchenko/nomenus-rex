#include "ruleDir.h"

#include <vector>

RuleDir::RuleDir(Mode _mode, std::string _separator)
	: RuleBase(RuleType::Dir)
	, mode {_mode}
	, separator {_separator}
{

}

void	RuleDir::process(std::filesystem::path& name)
{
	std::vector<std::string> path;
	for (auto it = name.begin(); it != name.end(); ++it)
	{
		path.push_back(*it);
	}

	if (mode == Mode::parent_only && path.size() > 1)
	{
		result = path[path.size()-2];
	}else
	if (mode == Mode::whole)
	{
		for (auto& dir: path)
		{
			result += dir + separator;
		}

		// Removing the separator from the end of the string
		result.erase(result.size() - separator.size());
	}



}
