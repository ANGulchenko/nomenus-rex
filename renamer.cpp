#include "renamer.h"
#include "Progressator/progressator.h"

#include <set>
#include <iostream>
#include <unicode/unistr.h>
#include <algorithm>

Renamer::Renamer()
	: keep_dir_structure {false}
	, copy_or_rename {CopyOrRename::copy}
	, sort_mode {SortMode::sic}
{

}

Renamer::~Renamer()
{

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

void	Renamer::setSortMode(SortMode mode)
{
	sort_mode = mode;
}


///////////////////

void	Renamer::addDateRule(const std::string& format)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleDate(format)));
}

void	Renamer::addTextRule(const std::string& text)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleText(text)));
}

void	Renamer::addDirRule(RuleDir::Mode mode, const std::string& separator)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleDir(mode, separator)));
}

void	Renamer::addIntegerRule(RuleInteger::Mode mode, int start, int step, int padding)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleInteger(mode, start, step, padding)));
}

void	Renamer::addExtensionRule(RuleExtension::Mode mode, const std::string& ext)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleExtension(mode, ext)));
}

void	Renamer::addFilenameRule(RuleFilename::Mode mode)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleFilename(mode)));
}

void	Renamer::addFilesizeRule(RuleFilesize::Dimension dimention, bool show_dimention, const std::string& decimal_separator)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleFilesize(dimention, decimal_separator, show_dimention)));
}

void	Renamer::addReplaceRule(const std::string& what, const std::string& to)
{
	rules.push_back(unique_ptr<RuleBase>(new RuleReplace(what, to)));
}



fs::path Renamer::applyRulesToOneFilename(const fs::path& _relative_path)
{
	std::string new_filename;

	for (unique_ptr<RuleBase>& rule: rules)
	{
		RuleBase* tempBasePtr = rule.get();

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
				static_cast<RuleInteger*>(tempBasePtr)->process(_relative_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Dir:
			{
				static_cast<RuleDir*>(tempBasePtr)->process(_relative_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Extension:
			{
				static_cast<RuleExtension*>(tempBasePtr)->process(_relative_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Filename:
			{
				static_cast<RuleFilename*>(tempBasePtr)->process(_relative_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Filesize:
			{
				fs::path absolute_path(source_dir / _relative_path);
				static_cast<RuleFilesize*>(tempBasePtr)->process(absolute_path);
				new_filename += rule->getString();
			}break;
			case RuleType::Replace:
			{
				static_cast<RuleReplace*>(tempBasePtr)->process(new_filename);
				//new_filename += rule->getString();
			}break;
		}

	}
	return fs::path(new_filename);
}

void Renamer::createRenameBijection()
{
	// Get our directories in the array
	getSourceFilenames(rename_vector, source_dir);
	sortSourceFilenames(rename_vector, sort_mode);


	for (auto& pair : rename_vector)
	{
		const std::filesystem::path& dir_entry = pair.first;
		fs::path absolute_file_path_source = dir_entry;
		fs::path relative_file_path_source = fs::relative(absolute_file_path_source, source_dir);

		fs::path new_name = applyRulesToOneFilename(relative_file_path_source);

		fs::path relative_file_path_without_filename = relative_file_path_source;
		relative_file_path_without_filename.remove_filename();
		fs::path absolute_file_path_destination = destination_dir;
		if (keep_dir_structure)
		{
			absolute_file_path_destination /= relative_file_path_without_filename;
		}
		absolute_file_path_destination /= new_name;

		pair.second = absolute_file_path_destination;
	}
}

void Renamer::testRenameBijection() const
{
	std::map<fs::path, fs::path> result;
	auto starting_point = rename_vector.begin();
	auto runner = starting_point;
	while (starting_point != rename_vector.end())
	{
		runner = starting_point;
		runner++;
		while (runner != rename_vector.end())
		{
			if ((*runner).second == (*starting_point).second)
			{
				result[(*runner).first] = (*runner).second;
			}
			runner++;
		}

		starting_point++;
	}

	if (result.size() > 0)
	{
		std::cerr << "ERROR: Filenames collision." << std::endl;
		for (auto& element: result)
		{
			std::cout << "┌╼" << element.first  << "\n" <<
						 "└╳" << element.second << "\n" << std::endl;
		}
		exit(EXIT_FAILURE);
	}

}

void Renamer::executeRenameBijection()
{
	Progressator progress(rename_vector.size());

	switch (copy_or_rename)
	{
		case CopyOrRename::copy:
		{
			for (auto& element: rename_vector)
			{
				fs::path dest_dir = element.second;
				dest_dir.remove_filename();
				fs::create_directories(dest_dir);
				fs::copy(element.first, element.second, std::filesystem::copy_options::overwrite_existing);
				progress.inc();progress.print();
			}
		}break;
		case CopyOrRename::rename:
		{
			for (auto& element: rename_vector)
			{
				fs::path dest_dir = element.second;
				dest_dir.remove_filename();
				fs::create_directories(dest_dir);
				fs::rename(element.first, element.second);
				progress.inc();progress.print();
			}
		}break;
	}

}

void  Renamer::printRenameBijection() const
{
	for (auto& element: rename_vector)
	{
		std::cout << "┌╼" << element.first  << "\n" <<
					 "└►" << element.second << "\n" << std::endl;
	}
}

void Renamer::getSourceFilenames(std::vector<std::pair<fs::path, fs::path>>& rename_vector, const fs::path&	dir)
{
	for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(dir))
	{
		if (!std::filesystem::is_directory(dir_entry))
		{
			rename_vector.push_back(std::pair(dir_entry, ""));
		}
	}
}

void Renamer::sortSourceFilenames(std::vector<std::pair<fs::path, fs::path>>& rename_vector, SortMode sort_mode)
{
	switch (sort_mode)
	{
		case SortMode::sic:
		{
			// Well, sorting isn't needed.
		}
		break;
		case SortMode::az:
		case SortMode::za:
		{
			std::vector<icu::UnicodeString> icu_str_vector;
			for (auto& path: rename_vector)
			{
				icu_str_vector.push_back(icu::UnicodeString(path.first.c_str()));
			}

			if (sort_mode == SortMode::az)
			{
				std::sort(icu_str_vector.begin(), icu_str_vector.end(), [](icu::UnicodeString a, icu::UnicodeString b) {return a < b;});
			}else
			if (sort_mode == SortMode::za)
			{
				std::sort(icu_str_vector.begin(), icu_str_vector.end(), [](icu::UnicodeString a, icu::UnicodeString b) {return a > b;});
			}

			size_t vector_size =  rename_vector.size();
			for (size_t counter = 0; counter < vector_size; counter++)
			{
				std::string path;
				icu_str_vector[counter].toUTF8String(path);
				rename_vector[counter].first = path;
			}
		}
		break;
	}
}

