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
	enum class CopyOrRename{copy, rename};
	Renamer();
	~Renamer();
	////////////////////
	void	setPaths(const std::string& source, const std::string& destination);
	void	setKeepDirStructure(bool keep);
	void	setCopyOrRename(CopyOrRename mode);
	////////////////////
	void	addDateRule(const std::string& format);
	void	addTextRule(const std::string& text);
	void	addDirRule(RuleDir::Mode mode, const std::string& separator);
	void	addIntegerRule(RuleInteger::Mode mode, int start, int step, int padding);
	void	addExtensionRule(RuleExtension::Mode mode, const std::string& ext);
	////////////////////

	fs::path applyRulesToOneRelativeFilename(fs::path relative_path);
	void createRenameBijectionMap();
	std::map<fs::path /*source*/, fs::path /*destination*/>	testRenameBijectionMap();
	void executeRenameBijectionMap();
	void printRenameBijectionMap();


private:
	fs::path		source_dir;
	fs::path		destination_dir;
	bool			keep_dir_structure;
	CopyOrRename	copy_or_rename;
	std::vector<RuleBase*> rules;
	std::map<fs::path /*source*/, fs::path /*destination*/> rename_map;
};

#endif // RENAMER_H
