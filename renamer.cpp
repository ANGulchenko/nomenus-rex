#include "renamer.h"

#include <set>
#include <iostream>
#include <libconfig.h++>

Renamer::Renamer()
	: keep_dir_structure {false}
	, copy_or_rename {CopyOrRename::copy}
{

}

Renamer::~Renamer()
{
	for (RuleBase* rule: rules)
	{
		delete rule;
	}

	rules.clear();
}

///////////////////
void	Renamer::setPaths(const std::string& source, const std::string& destination)
{
	source_dir = source;
	destination_dir = destination;
}

void	Renamer::setKeepDirStructure(bool keep)
{
	keep_dir_structure = keep;
}

void	Renamer::setCopyOrRename(CopyOrRename mode)
{
	copy_or_rename = mode;
}


///////////////////

void	Renamer::addDateRule(const std::string& format)
{
	rules.push_back(new RuleDate(format));
}

void	Renamer::addTextRule(const std::string& text)
{
	rules.push_back(new RuleText(text));
}

void	Renamer::addDirRule(RuleDir::Mode mode, const std::string& separator)
{
	rules.push_back(new RuleDir(mode, separator));
}

void	Renamer::addIntegerRule(RuleInteger::Mode mode, int start, int step, int padding)
{
	rules.push_back(new RuleInteger(mode, start, step, padding));
}

void	Renamer::addExtensionRule(RuleExtension::Mode mode, const std::string& ext)
{
	rules.push_back(new RuleExtension(mode, ext));
}


fs::path Renamer::applyRulesToOneRelativeFilename(fs::path relative_path)
{
	std::string new_filename;

	for (RuleBase* rule: rules)
	{
		switch (rule->getType())
		{
			//RuleType {Date, Text, Integer, Extension, Dir};
			case RuleType::Date:
			{
				new_filename += rule->getString();
			}break;
			case RuleType::Text:
			{
				new_filename += rule->getString();
			}break;
			case RuleType::Integer:
			{
				static_cast<RuleInteger*>(rule)->process(relative_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Dir:
			{
				static_cast<RuleDir*>(rule)->process(relative_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Extension:
			{
				static_cast<RuleExtension*>(rule)->process(relative_path);
				new_filename += rule->getString();
			}break;
		}

	}
	return fs::path(new_filename);
}

void Renamer::createRenameBijectionMap()
{
	for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(source_dir))
	{
		if (dir_entry.is_regular_file())
		{
			fs::path absolute_file_path_source = dir_entry.path();
			fs::path relative_file_path_source = fs::relative(absolute_file_path_source, source_dir);

			fs::path new_name = applyRulesToOneRelativeFilename(relative_file_path_source);

			fs::path relative_file_path_without_filename = relative_file_path_source;
			relative_file_path_without_filename.remove_filename();
			fs::path absolute_file_path_destination = destination_dir;
			if (keep_dir_structure)
			{
				absolute_file_path_destination /= relative_file_path_without_filename;
			}
			absolute_file_path_destination /= new_name;

			rename_map[absolute_file_path_source] = absolute_file_path_destination;
		}
	}
}

std::map<fs::path /*source*/, fs::path /*destination*/>	Renamer::testRenameBijectionMap()
{
	std::set<fs::path> unique_check;
	for (auto& element: rename_map)
	{
		unique_check.insert(element.second);
	}
	if (rename_map.size() != unique_check.size())
	{
		exit(-1);
	}
	std::map<fs::path /*source*/, fs::path /*destination*/> result;
	return result;
}

void Renamer::executeRenameBijectionMap()
{
	switch (copy_or_rename)
	{
		case CopyOrRename::copy:
		{
			for (auto& element: rename_map)
			{
				fs::path dest_dir = element.second;
				dest_dir.remove_filename();
				fs::create_directories(dest_dir);
				fs::copy(element.first, element.second, std::filesystem::copy_options::overwrite_existing);
			}
		}break;
		case CopyOrRename::rename:
		{
			for (auto& element: rename_map)
			{
				fs::path dest_dir = element.second;
				dest_dir.remove_filename();
				fs::create_directories(dest_dir);
				fs::rename(element.first, element.second);
			}
		}break;
	}

}

void  Renamer::printRenameBijectionMap()
{
	for (auto& element: rename_map)
	{
		std::cout << "╒════════════\n│" << element.first << "\n│" << element.second << "\n╘════════════\n";
	}
}
