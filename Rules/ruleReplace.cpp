#include "ruleReplace.h"

RuleReplace::RuleReplace(const std::string& _what, const std::string& _to)
	: what {_what}
	, to {_to}
{

}

std::string	RuleReplace::process(const RuleParams& params)
{
	//std::string result;
	if (what.empty())
	{
		// It doesn't make any sense, so someone inevitably WILL do it.
		return ""; //params.name_in_process;
	}

	size_t pos = 0;
	while((pos = params.name_in_process.find(what, pos)) != std::string::npos)
	{
		params.name_in_process.replace(pos, what.length(), to);
		pos += what.length();
	}

	//result = params.name_in_process;
	return "";//params.name_in_process;
}

