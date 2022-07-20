#include "ruleFilename.h"
#include <iostream>
#include <unicode/unistr.h>

RuleFilename::RuleFilename(Mode _mode)
	: RuleBase(RuleType::Filename)
	, mode{_mode}
{

}

void	RuleFilename::process(const RuleParams& params)
{
	result = params.relative_path.stem();

	switch (mode)
	{
		case Mode::lowercase:
		{
			icu::UnicodeString icu_str(result.c_str());
			result.clear();
			icu_str.toLower();
			icu_str.toUTF8String(result);
		}break;
		case Mode::uppercase:
		{
			icu::UnicodeString icu_str(result.c_str());
			result.clear();
			icu_str.toUpper();
			icu_str.toUTF8String(result);
		}break;
		case Mode::sic:
		{

		}break;
	}
}

