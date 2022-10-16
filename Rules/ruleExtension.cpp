#include "ruleExtension.h"
#include <iostream>
#include <unicode/unistr.h>

RuleExtension::RuleExtension(Mode _mode, const std::string& _ext)
	: mode{_mode}
	, ext {_ext}
{

}

std::string	RuleExtension::process(const RuleParams& params)
{
	std::string result;

	if (ext == "")
	{
		result = params.relative_path.extension();
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

	return result;
}

