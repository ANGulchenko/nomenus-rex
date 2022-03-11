#include "ruleExtension.h"
#include <iostream>
#include <algorithm>

RuleExtension::RuleExtension(Mode _mode, std::string _ext)
	: RuleBase(RuleType::Extension)
	, mode{_mode}
	, ext {_ext}
{

}

void	RuleExtension::process(std::filesystem::path& name)
{
	if (ext == "")
	{
		result = name.extension();
	}else
	{
		result = ext;
	}

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
