#include "ruleReplace.h"

RuleReplace::RuleReplace(const std::string& _what, const std::string& _to)
	: RuleBase(RuleType::Replace)
	, what {_what}
	, to {_to}
{

}

void	RuleReplace::process(const RuleParams& params)
{
	if (what.empty())
	{
		// It doesn't make any sense, so someone inevitably WILL do it.
		result = params.name_in_process;
		return;
	}

	size_t pos = 0;
	while((pos = params.name_in_process.find(what, pos)) != std::string::npos)
	{
		params.name_in_process.replace(pos, what.length(), to);
		pos += what.length();
	}

	result = params.name_in_process;
}

