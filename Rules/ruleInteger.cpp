#include "ruleInteger.h"
#include <algorithm>

RuleInteger::RuleInteger(Mode _mode, int _start, int _step, int _padding)
	: RuleBase(RuleType::Integer)
	, number {_start}
	, step {_step}
	, mode {_mode}
	, zero_padding_size {_padding}
{

}

void	RuleInteger::process(const std::filesystem::path& name)
{
	switch (mode)
	{
		case Mode::global:
		{
			result = std::to_string(number);
			result.insert(0, std::max(0, zero_padding_size - static_cast<int>(result.size())), '0');
			number++;
		}break;
		case Mode::local_at_every_dir:
		{
			std::filesystem::path path = name;
			path.remove_filename();
			result = std::to_string(number_per_path[path]);
			result.insert(0, std::max(0, zero_padding_size - static_cast<int>(result.size())), '0');
			number_per_path[path]++;
		}
	}
}

void	RuleInteger::test()
{
	{
		RuleInteger rule(RuleInteger::Mode::global, 0, 1, 4);
		std::filesystem::path path("");
		rule.process(path);
		testsCmp(3, rule, "0000");
		rule.process(path);
		testsCmp(4, rule, "0001");
		rule.process(path);
		testsCmp(5, rule, "0002");
	}
}
