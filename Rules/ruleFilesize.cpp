#include "ruleFilesize.h"
#include <iostream>
#include <algorithm>
#include <cmath>

RuleFilesize::RuleFilesize(Dimension _dimension, const std::string& _decimal_separator, bool _show_dimension)
	: dimension{_dimension}
	, decimal_separator {_decimal_separator}
	, show_dimension {_show_dimension}
	, dimension_data {	{Dimension::B,   {.name = "B",   .size = 1}},
						{Dimension::KiB, {.name = "KiB", .size = 1_KiB}},
						{Dimension::MiB, {.name = "MiB", .size = 1_MiB}},
						{Dimension::GiB, {.name = "GiB", .size = 1_GiB}}
					 }
{

}

std::string	RuleFilesize::process(const RuleParams& params)
{
	std::string result;

	uintmax_t fsize =  std::filesystem::file_size(params.absolute_path);
	double dimensioned_size = (1.0*fsize) / (1.0*dimension_data[dimension].size);

	double pre_dot;
	double post_dot = std::modf(dimensioned_size, &pre_dot);
	int  int_pre_dot = pre_dot;

	std::string  str_post_dot = std::to_string(post_dot);


	result += std::to_string(int_pre_dot);
	result += decimal_separator;
	result += str_post_dot.substr(2, 3);
	if (show_dimension)
	{
		result += dimension_data[dimension].name;
	}

	return result;

}
