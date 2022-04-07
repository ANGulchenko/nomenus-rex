#include "ruleText.h"

RuleText::RuleText(const std::string& text)
	: RuleBase(RuleType::Text)
{
	result = text;
}

void	RuleText::test()
{
	{
		RuleText rule("Test");
		testsCmp(1, rule, "Test");
	}
	{
		RuleText rule("");
		testsCmp(2, rule, "");
	}
}
