#ifndef RULEFILESIZE_H
#define RULEFILESIZE_H

#include "ruleBase.h"
#include <filesystem>

class RuleFilesize: public RuleBase
{
public:
	enum class Dimension {B, KiB, MiB, GiB};
	RuleFilesize(Dimension _dimesion, const std::string& _decimal_separator, bool _show_dimension);
	void	process(std::filesystem::path& name);
	static void	test();
private:
	Dimension	dimension;
	std::string decimal_separator;
	bool		show_dimension;
};

#endif
