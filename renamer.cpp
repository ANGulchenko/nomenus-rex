#include "renamer.h"

#include <set>
#include <iostream>
#include <libconfig.h++>

Renamer::Renamer(fs::path _source, fs::path _destination, fs::path _config)
	: source_dir(_source)
	, destination_dir(_destination)
{
	// Parsing the config, creating the Rules objects and putting them into the array.

	using namespace libconfig;

	Config cfg;

	try
	{
		cfg.readFile(_config.c_str());
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading file." << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
				  << " - " << pex.getError() << std::endl;
		exit(EXIT_FAILURE);
	}

	const Setting& root = cfg.getRoot();

	const Setting &rules_raw = root["rules"];

	int count = rules_raw.getLength();

	std::string rule_type;
	for(int i = 0; i < count; ++i)
	{
		const Setting &rule_raw = rules_raw[i];
		rule_raw.lookupValue("type", rule_type);


		if (rule_type == "date")
		{
			std::string date_format;
			bool var1_present = rule_raw.lookupValue("date_format", date_format);
			if (!var1_present)
			{
				std::cerr << "There is no 'date_format' variable in the 'date' rule." << std::endl;
				exit(EXIT_FAILURE);
			}
			rules.push_back(RuleDate(date_format));
		}else
		if (rule_type == "text")
		{
			std::string text;
			bool var1_present = rule_raw.lookupValue("text", text);
			if (!var1_present)
			{
				std::cerr << "There is no 'text' variable in the 'text' rule." << std::endl;
				exit(EXIT_FAILURE);
			}
			rules.push_back(RuleText(text));
		}else
		if (rule_type == "dir")
		{
			rules.push_back(RuleDir());
		}else
		if (rule_type == "integer")
		{
			rules.push_back(RuleInteger());
		}else
		if (rule_type == "extension")
		{
			rules.push_back(RuleExtension());
		}
	}

}

fs::path Renamer::applyRulesToOneRelativeFilename(fs::path relative_path)
{
	std::string new_filename;

	for (RuleBase& rule: rules)
	{
		switch (rule.getType())
		{
			//RuleType {Date, Text, Integer, Extension, Dir};
			case RuleType::Date:
			{
				new_filename += rule.getString();
			}break;
			case RuleType::Text:
			{
				new_filename += rule.getString();
			}break;
			case RuleType::Integer:
			{
				static_cast<RuleInteger&>(rule).process();
				new_filename += rule.getString();
			}break;
			case RuleType::Dir:
			{
				static_cast<RuleDir&>(rule).process(relative_path);
				new_filename += rule.getString();
			}break;
			case RuleType::Extension:
			{
				static_cast<RuleExtension&>(rule).process(relative_path);
				new_filename += rule.getString();
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
