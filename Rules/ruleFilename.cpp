#include "ruleFilename.h"
#include <iostream>
#include <unicode/unistr.h>

RuleFilename::RuleFilename(Mode _mode)
	: RuleBase(RuleType::Filename)
	, mode{_mode}
{

}

void	RuleFilename::process(const std::filesystem::path& name)
{
	result = name.stem();

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

void	RuleFilename::test()
{
	{
		RuleFilename rule(RuleFilename::Mode::lowercase);
		rule.process(std::filesystem::path("dir/subdir1/subdir2/fIlE.dAt"));
		testsCmp(1, rule, "file");
	}

	{
		RuleFilename rule(RuleFilename::Mode::uppercase);
		rule.process(std::filesystem::path("dir/subdir1/subdir2/fileфайл.dat"));
		testsCmp(2, rule, "FILEФАЙЛ");
	}

}
