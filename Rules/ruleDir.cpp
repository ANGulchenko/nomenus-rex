#include "ruleDir.h"

#include <vector>

RuleDir::RuleDir()
	: RuleBase(RuleType::Dir)
{

}

void	RuleDir::process(std::filesystem::path& name)
{
	std::vector<std::string> path;
	for (auto it = name.begin(); it != name.end(); ++it)
	{
		path.push_back(*it);
	}

	if (path.size() > 1)
	{
		result = path[path.size()-2];
	}
}
