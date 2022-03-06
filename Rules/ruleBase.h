#ifndef RULEBASE_H
#define RULEBASE_H

#include <string>

enum class RuleType {Date, Text, Integer, Extension, Dir};

class RuleBase
{
public:
	explicit RuleBase(RuleType type);
	RuleType getType();
	std::string	getString();

protected:
	std::string result;

private:
	RuleType	type;

};

#endif // RULE_H
