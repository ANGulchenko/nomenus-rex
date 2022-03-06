#include "renamer.h"

#include <set>
#include <iostream>
#include <libconfig.h++>

Renamer::Renamer()
	: keep_dir_structure(false)
{

}

Renamer::~Renamer()
{
	rules.clear();
}

///////////////////
void	Renamer::setPaths(const std::string& source, const std::string& destination)
{
	source_dir = {source};
	destination_dir = {destination};
}

void	Renamer::setKeepDirStructure(bool keep)
{
	keep_dir_structure = keep;
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

void	Renamer::addDirRule()
{
	rules.push_back(new RuleDir());
}

void	Renamer::addIntegerRule(RuleInteger::Mode mode, int start, int step)
{
	rules.push_back(new RuleInteger(mode, start, step));
}

void	Renamer::addExtensionRule()
{
	rules.push_back(new RuleExtension());
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

			fs::path absolute_file_path_destination = destination_dir / new_name;
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
	for (auto& element: rename_map)
	{
		fs::copy(element.first, element.second);
	}
}

void  Renamer::printRenameBijectionMap()
{
	for (auto& element: rename_map)
	{
		std::cout << element.first << "\n" << element.second << "\n =================\n";
	}
}
