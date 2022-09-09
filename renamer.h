#ifndef RENAMER_H
#define RENAMER_H

#include <vector>
#include <filesystem>
#include <map>
#include <memory>

#include "Rules/rules.h"

namespace fs = std::filesystem;

class Renamer
{
public:
	enum class CopyOrRename{copy, rename};
	enum class SortMode{sic, az, za};
	Renamer();
	////////////////////

	fs::path applyRulesToOneFilename(const RuleParams& params);
	void createRenameBijection();
	void testRenameBijection() const;
	void executeRenameBijection();
	void printRenameBijection() const;

	fs::path		source_dir;
	fs::path		destination_dir;
	bool			keep_dir_structure;
	CopyOrRename	copy_or_rename;
	SortMode		sort_mode;
	std::vector<std::unique_ptr<RuleBase>> rules;


private:

	std::vector<std::pair<fs::path, fs::path>> rename_vector;

	void getSourceFilenames(std::vector<std::pair<fs::path, fs::path>>& rename_vector, const fs::path&	source_dir);
	void sortSourceFilenames(std::vector<std::pair<fs::path, fs::path>>& rename_vector, SortMode sort_mode);
};

#endif // RENAMER_H
