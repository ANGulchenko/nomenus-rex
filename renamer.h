#ifndef RENAMER_H
#define RENAMER_H

#include <vector>
#include <filesystem>
#include <map>

#include "rule.h"

namespace fs = std::filesystem;

class Renamer
{
public:
	Renamer(fs::path source, fs::path destination, fs::path config);
	fs::path applyRulesToOneRelativeFilename(fs::path relative_path);
	void createRenameBijectionMap();
	std::map<fs::path /*source*/, fs::path /*destination*/>	testRenameBijectionMap();
	void executeRenameBijectionMap();
	void printRenameBijectionMap();


private:
	fs::path source_dir;
	fs::path destination_dir;
	std::vector<RuleBase> rules;
	std::map<fs::path /*source*/, fs::path /*destination*/> rename_map;
};

#endif // RENAMER_H
