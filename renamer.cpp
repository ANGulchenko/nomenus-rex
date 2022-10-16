#include "renamer.h"
#include "Progressator/progressator.h"
#include "cfgvarsingleton.h"

#include <set>
#include <iostream>
#include <unicode/unistr.h>
#include <algorithm>
#include <chrono>

Renamer::Renamer()
	: keep_dir_structure {false}
	, copy_or_rename {CopyOrRename::copy}
	, sort_mode {SortMode::sic}
{

}

///////////////////

void Renamer::createRenameBijection()
{
	cfg::print("Start createRenameBijection()... ");
	auto start = std::chrono::high_resolution_clock::now();

	// Get our directories in the array
	getSourceFilenames(rename_vector, source_dir);
	sortSourceFilenames(rename_vector, sort_mode);


	for (auto& pair : rename_vector)
	{
		const std::filesystem::path& dir_entry = pair.first;
		fs::path absolute_file_path_source = dir_entry;
		fs::path relative_file_path_source = fs::relative(absolute_file_path_source, source_dir);
		std::string name_in_process;

		RuleParams params = {.absolute_path = absolute_file_path_source,
							 .relative_path = relative_file_path_source,
							 .name_in_process = name_in_process};

		for (std::unique_ptr<RuleBase>& rule: rules)
		{
			params.name_in_process += rule->process(params);
		}

		fs::path relative_file_path_without_filename = relative_file_path_source;
		relative_file_path_without_filename.remove_filename();
		fs::path absolute_file_path_destination = destination_dir;
		if (keep_dir_structure)
		{
			absolute_file_path_destination /= relative_file_path_without_filename;
		}
		absolute_file_path_destination /= name_in_process;

		pair.second = absolute_file_path_destination;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	auto time_delta = duration.count();

	cfg::print("Finished in " + std::to_string(time_delta) + " microseconds. "
			   "(" + std::to_string(rename_vector.size()) + " filename pairs)\n");

}

void Renamer::testRenameBijection() const
{
	cfg::print("Start testRenameBijection()... ");
	auto start = std::chrono::high_resolution_clock::now();

	std::map<fs::path, fs::path> result;
	std::set<std::string> new_names;

	for (auto& pair: rename_vector)
	{
		if (auto ins_res = new_names.insert(pair.second); ins_res.second == false)
		{
			// Isertion has failed, so we have a name collision here
			result[pair.first] = pair.second;
		}
	}

	if (result.size() > 0)
	{
		std::cerr << "ERROR: Filenames collision." << std::endl;
		for (auto& element: result)
		{
			std::cerr << "┌╼" << element.first  << "\n" <<
						 "└╳" << element.second << "\n" << std::endl;
		}
		exit(EXIT_FAILURE);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	auto time_delta = duration.count();

	cfg::print("Finished in " + std::to_string(time_delta) + " microseconds.\n");
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
				try
				{
					fs::copy(element.first, element.second, std::filesystem::copy_options::overwrite_existing);
				}
				catch(std::filesystem::filesystem_error const& ex)
				{
					// In this case we should just notify the user and that's it: the user can
					// delete the previously copied files by his own.
					std::cerr << "\n what():  " << ex.what() << '\n';
					exit(EXIT_FAILURE);
				}

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
				try
				{
					fs::rename(element.first, element.second);
				}
				catch(std::filesystem::filesystem_error const& ex)
				{
					// We need to rename the processed files back
					std::cerr << "\n what():  " << ex.what() << '\n';
					std::cerr << "Renaming files back to their original names...\n";

					std::pair<fs::path, fs::path> problematic_pair = {ex.path1(), ex.path2()};
					auto iter_of_problematic_pair = std::find(rename_vector.begin(),
															  rename_vector.end(),
															  problematic_pair);

					for (auto i = rename_vector.begin(); i != iter_of_problematic_pair; ++i)
					{
						fs::rename((*i).second, (*i).first);
					}

					exit(EXIT_FAILURE);
				}

				progress.inc();progress.print();
			}
		}break;
	}

}


void  Renamer::printRenameBijection() const
{
	if (!CfgVarSingleton::Instance().verbose)
	{
		return;
	}

	int amount_to_print = rename_vector.size()-1;
	if (CfgVarSingleton::Instance().amount_of_bijections_printed >= 0
		&& CfgVarSingleton::Instance().amount_of_bijections_printed < amount_to_print)
	{
		amount_to_print = CfgVarSingleton::Instance().amount_of_bijections_printed;
	}

	for (size_t i = rename_vector.size() - amount_to_print; i < rename_vector.size(); ++i)
	{
		std::cout << "┌╼" << rename_vector[i].first  << "\n" <<
					 "└►" << rename_vector[i].second << "\n";
	}

	std::cout << std::endl;
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

