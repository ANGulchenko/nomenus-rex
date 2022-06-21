#include "ruleDir.h"

#include <vector>

RuleDir::RuleDir(Mode _mode, std::string _separator)
	: RuleBase(RuleType::Dir)
	, mode {_mode}
	, separator {_separator}
{

}

void	RuleDir::process(const std::filesystem::path& name)
{
	RuleBase::result.clear();
	std::vector<std::string> path;
	for (auto it = name.begin(); it != name.end(); ++it)
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

void	RuleDir::test()
{
	{
		RuleDir rule(RuleDir::Mode::whole, "_");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.dat"));
		testsCmp(1, rule, "dir_subdir1_subdir2");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.dat"));
		testsCmp(2, rule, "dirsubdir1subdir2");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "");
		rule.process(std::filesystem::path("file.dat"));
		testsCmp(3, rule, "");
	}
	{
		RuleDir rule(RuleDir::Mode::parent_only, "_");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.dat"));
		testsCmp(4, rule, "subdir2");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "_");
		rule.process(std::filesystem::path("dir/поддиректория/الدليل/file.dat"));
		testsCmp(5, rule, "dir_поддиректория_الدليل");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "[separator]");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.dat"));
		testsCmp(6, rule, "dir[separator]subdir1[separator]subdir2");
	}
}
