#ifndef RULEREPLACE_H
#define RULEREPLACE_H

#include "ruleBase.h"

class RuleReplace: public RuleBase
{
public:
	RuleReplace(const std::string& _what, const std::string& _to);
	void	process(std::string& _unfinished_filename);
private:
	std::string what;
	std::string to;
};

#endif
