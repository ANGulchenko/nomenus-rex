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

void	RuleInteger::process(const RuleParams& params)
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
			std::filesystem::path path = params.relative_path;
			path.remove_filename();
			result = std::to_string(number_per_path[path]);
			result.insert(0, std::max(0, zero_padding_size - static_cast<int>(result.size())), '0');
			number_per_path[path]++;
		}
	}
}


