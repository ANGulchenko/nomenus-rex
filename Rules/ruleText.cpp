#include "ruleText.h"

RuleText::RuleText(const std::string& text)
	: RuleBase(RuleType::Text)
{
	result = text;
}
