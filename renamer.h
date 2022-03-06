#ifndef RENAMER_H
#define RENAMER_H

#include <vector>
#include <filesystem>
#include <map>

#include "Rules/rules.h"

namespace fs = std::filesystem;

class Renamer
{
public:
	Renamer();
	~Renamer();
	////////////////////
	void	setPaths(const std::string& source, const std::string& destination);
	void	setKeepDirStructure(bool keep);
	////////////////////
	void	addDateRule(const std::string& format);
	void	addTextRule(const std::string& text);
	void	addDirRule();
	void	addIntegerRule(RuleInteger::Mode mode, int start, int step);
	void	addExtensionRule();
	////////////////////

	fs::path applyRulesToOneRelativeFilename(fs::path relative_path);
	void createRenameBijectionMap();
	std::map<fs::path /*source*/, fs::path /*destination*/>	testRenameBijectionMap();
	void executeRenameBijectionMap();
	void printRenameBijectionMap();


private:
	fs::path	source_dir;
	fs::path	destination_dir;
	bool		keep_dir_structure;
	std::vector<RuleBase*> rules;
	std::map<fs::path /*source*/, fs::path /*destination*/> rename_map;
};

#endif // RENAMER_H
