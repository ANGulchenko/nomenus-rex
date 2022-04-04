#include "ruleReplace.h"

RuleReplace::RuleReplace(const std::string& _what, const std::string& _to)
	: RuleBase(RuleType::Replace)
	, what {_what}
	, to {_to}
{

}

void	RuleReplace::process(std::string& _unfinished_filename)
{
	if (what.empty())
	{
		// It doesn't make any sense, so someone inevitably WILL do it.
		result = _unfinished_filename;
		return;
	}

	size_t pos = 0;
	while((pos = _unfinished_filename.find(what, pos)) != std::string::npos)
	{
		_unfinished_filename.replace(pos, what.length(), to);
		pos += what.length();
	}

	result = _unfinished_filename;
}
