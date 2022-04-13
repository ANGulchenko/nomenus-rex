#ifndef RULEFILESIZE_H
#define RULEFILESIZE_H

#include "ruleBase.h"
#include <filesystem>
#include <map>

constexpr std::size_t operator""_KiB(unsigned long long int x) {return 1024ULL * x;}
constexpr std::size_t operator""_MiB(unsigned long long int x) {return 1024_KiB * x;}
constexpr std::size_t operator""_GiB(unsigned long long int x) {return 1024_MiB * x;}

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
	struct iB2size{std::string name; uintmax_t size;};
	std::map<Dimension, iB2size> dimension_data;
};

#endif
