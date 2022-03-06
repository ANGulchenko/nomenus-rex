#include "ruleInteger.h"

RuleInteger::RuleInteger(Mode _mode, int _start, int _step)
	: RuleBase(RuleType::Integer)
	, number(_start)
	, step(_step)
	, mode(_mode)
{

}

void	RuleInteger::process(std::filesystem::path& name)
{
	switch (mode)
	{
		case Mode::global:
		{
			result = std::to_string(number);
			number++;
		}break;
		case Mode::local_at_every_dir:
		{
			std::filesystem::path path = name.remove_filename();
			result = std::to_string(number_per_path[path]);
			number_per_path[path]++;
		}
	}
}
