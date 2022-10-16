#include "ruleText.h"

RuleText::RuleText(const std::string& _text)
	: text {_text}
{

}

std::string	RuleText::process(const RuleParams&)
{
	return text;
}

