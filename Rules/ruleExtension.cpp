#include "ruleExtension.h"

RuleExtension::RuleExtension(bool _lower_case)
	: RuleBase(RuleType::Extension)
	, lower_case(_lower_case)
{

}

void	RuleExtension::process(std::filesystem::path& name)
{
	result = name.extension();
}
