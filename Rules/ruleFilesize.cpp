#include "ruleFilesize.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>

RuleFilesize::RuleFilesize(Dimension _dimension, const std::string& _decimal_separator, bool _show_dimension)
	: RuleBase(RuleType::Filesize)
	, dimension{_dimension}
	, decimal_separator {_decimal_separator}
	, show_dimension {_show_dimension}
{

}

void	RuleFilesize::process(std::filesystem::path& name)
{
	result.clear();

	std::map<Dimension, uintmax_t> bytes_per_dimension =
	{
		{Dimension::B, 1},
		{Dimension::KiB, 1024},
		{Dimension::MiB, 1024*1024},
		{Dimension::GiB, 1024*1024*1024},
	};

	std::map<Dimension, std::string> string_per_dimension =
	{
		{Dimension::B, "B"},
		{Dimension::KiB, "KiB"},
		{Dimension::MiB, "MiB"},
		{Dimension::GiB, "GiB"},
	};

	uintmax_t fsize =  std::filesystem::file_size(name);
	double dimensioned_size = (1.0*fsize) / (1.0*bytes_per_dimension[dimension]);

	double pre_dot;
	double post_dot = std::modf(dimensioned_size, &pre_dot);
	int  int_pre_dot = pre_dot;

	std::string  str_post_dot = std::to_string(post_dot);


	result += std::to_string(int_pre_dot);
	result += decimal_separator;
	result += str_post_dot.substr(2, 3);
	if (show_dimension)
	{
		result += string_per_dimension[dimension];
	}

}
