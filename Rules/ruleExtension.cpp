#include "ruleExtension.h"
#include <iostream>
#include <unicode/unistr.h>

RuleExtension::RuleExtension(Mode _mode, const std::string& _ext)
	: RuleBase(RuleType::Extension)
	, mode{_mode}
	, ext {_ext}
{

}

void	RuleExtension::process(const std::filesystem::path& name)
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

void	RuleExtension::test()
{
	{
		RuleExtension rule(RuleExtension::Mode::lowercase, "");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.dAt"));
		testsCmp(1, rule, ".dat");
	}

	{
		RuleExtension rule(RuleExtension::Mode::lowercase, "");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.РаСширение"));
		testsCmp(2, rule, ".расширение");
	}

	{
		RuleExtension rule(RuleExtension::Mode::lowercase, "");
		rule.process(std::filesystem::path("dir/subdir1/subdir2/file.MiXМикС1"));
		testsCmp(3, rule, ".mixмикс1");
	}
}
