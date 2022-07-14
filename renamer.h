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
	~Renamer();
	////////////////////

	void	setPaths(const std::string& source, const std::string& destination);
	void	setKeepDirStructure(bool keep);
	void	setCopyOrRename(CopyOrRename mode);
	void	setSortMode(SortMode mode);
	////////////////////

	void	addDateRule(const std::string& format);
	void	addTextRule(const std::string& text);
	void	addDirRule(RuleDir::Mode mode, const std::string& separator);
	void	addIntegerRule(RuleInteger::Mode mode, int start, int step, int padding);
	void	addExtensionRule(RuleExtension::Mode mode, const std::string& ext);
	void	addFilenameRule(RuleFilename::Mode mode);
	void	addFilesizeRule(RuleFilesize::Dimension dimention, bool show_dimention, const std::string& decimal_separator);
	void	addReplaceRule(const std::string& what, const std::string& to);
	////////////////////

	fs::path applyRulesToOneFilename(const fs::path& relative_path);
	void createRenameBijection();
	void testRenameBijection() const;
	void executeRenameBijection();
	void printRenameBijection() const;


private:
	fs::path		source_dir;
	fs::path		destination_dir;
	bool			keep_dir_structure;
	CopyOrRename	copy_or_rename;
	SortMode		sort_mode;
	std::vector<std::unique_ptr<RuleBase>> rules;
	std::vector<std::pair<fs::path, fs::path>> rename_vector;

	void getSourceFilenames(std::vector<std::pair<fs::path, fs::path>>& rename_vector, const fs::path&	source_dir);
	void sortSourceFilenames(std::vector<std::pair<fs::path, fs::path>>& rename_vector, SortMode sort_mode);
};

#endif // RENAMER_H
