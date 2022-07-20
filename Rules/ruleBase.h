#ifndef RULEBASE_H
#define RULEBASE_H

#include <filesystem>
#include <string>

class RuleBase;


enum class RuleType {Date, Text, Integer, Extension,
					 Dir, Filename, Filesize, Replace,
					 FileCreationDate};

struct RuleParams
{
	std::filesystem::path& absolute_path;
	std::filesystem::path& relative_path;
	std::string& name_in_process;
};

class RuleBase
{
public:
	explicit		RuleBase(RuleType type);
	RuleType		getType() const;
	std::string		getTypeStr() const;
	std::string		getString() const;
	virtual void	process(const RuleParams&) = 0;

protected:
	std::string result;

private:
	RuleType	type;

};

#endif // RULE_H
