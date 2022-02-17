#ifndef RULE_H
#define RULE_H

#include <string>
#include <filesystem>

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

class RuleDate: public RuleBase
{
public:
	RuleDate(const std::string& format);
};

class RuleText: public RuleBase
{
public:
	RuleText(const std::string& format);
};

class RuleInteger: public RuleBase
{
public:
	RuleInteger(int _start = 0, int _step = 1, bool _reset_at_every_dir  = false);
	void	process();
private:
	int			number;
	int			step;
	bool		reset_at_every_dir;
};

class RuleExtension: public RuleBase
{
public:
	RuleExtension(bool lower_case = true);
	void	process(std::filesystem::path& name);
private:
	bool lower_case;
};

class RuleDir: public RuleBase
{
public:
	RuleDir();
	void	process(std::filesystem::path& name);
};

#endif // RULE_H
