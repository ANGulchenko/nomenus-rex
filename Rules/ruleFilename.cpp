#include "ruleFilename.h"
#include <iostream>
#include <algorithm>

RuleFilename::RuleFilename(Mode _mode)
	: RuleBase(RuleType::Filename)
	, mode{_mode}
{

}

void	RuleFilename::process(std::filesystem::path& name)
{
	result = name.stem();

	switch (mode)
	{
		case Mode::lowercase:
		{
			std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		}break;
		case Mode::uppercase:
		{
			std::transform(result.begin(), result.end(), result.begin(), ::toupper);
		}break;
		case Mode::sic:
		{

		}break;
	}
}
