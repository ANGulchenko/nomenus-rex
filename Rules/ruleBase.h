#ifndef RULEBASE_H
#define RULEBASE_H

#include <string>

class RuleBase;
void testsCmp(int testIndex, RuleBase& rule, const std::string& correct_answer);

enum class RuleType {Date, Text, Integer, Extension, Dir, Filename, Filesize, Replace};

class RuleBase
{
public:
	explicit	RuleBase(RuleType type);
	RuleType	getType();
	std::string getTypeStr();
	std::string	getString();

protected:
	std::string result;

private:
	RuleType	type;

};

#endif // RULE_H
