#include <stdlib.h>

#include "AnyOption/anyoption.h"
#include "configurationparser.h"
#include "version.h"

ConfigurationParser::ConfigurationParser(int argc, char *argv[], Renamer& renamer)
{
	std::string source;
	std::string destination;
	std::string config;

	AnyOption *opt = new AnyOption();
	// report version
	std::string VERSION_TOTAL = std::to_string(VERSION_MAJOR)+"."+std::to_string(VERSION_MINOR)+"."+std::to_string(VERSION_PATCH);
	std::string intro_line = "Nomenus-rex("+VERSION_TOTAL+") is a file mass-renaming utility.";
	opt->addUsage(intro_line.c_str());
	opt->addUsage("Parameters: ");
	opt->addUsage("");
	opt->addUsage(" -h  --help          Prints this help ");
	opt->addUsage(" -s  --source        Source dir");
	opt->addUsage(" -d  --destination   Destination dir");
	opt->addUsage(" -c  --config        Configuration file");
	opt->addUsage("");
	opt->addUsage("Directories can be also set up in the config file. CLI parameters have higher priority.");

	opt->setFlag("help", 'h');
	opt->setCommandOption("source", 's');
	opt->setCommandOption("destination", 'd');
	opt->setCommandOption("config", 'c');

	opt->processCommandArgs(argc, argv);

	if (opt->hasOptions())
	{
		if (opt->getFlag("help") || opt->getFlag('h'))
		{
			opt->printUsage();
			exit(EXIT_SUCCESS);
		}

		if (opt->getValue('s') != NULL || opt->getValue("source") != NULL)
		{
			source = string(opt->getValue("source"));
		}
		if (opt->getValue('d') != NULL || opt->getValue("destination") != NULL)
		{
			destination = string(opt->getValue("destination"));
		}
		if (opt->getValue('c') != NULL || opt->getValue("config") != NULL)
		{
			config = string(opt->getValue("config"));
		}
	}



	if (config == "")
	{
		std::cerr << "\nERROR: No config file is specified!" << std::endl;
		opt->printUsage();
		exit(EXIT_FAILURE);
	}

	config = getConfigPathString(config);

	delete opt;

	// Parsing the config, creating the Rules objects and putting them into the array.
	makeSureThatConfigDirIsExist();
	Config cfg;

	try
	{
		cfg.readFile(config.c_str());
	}
	catch(const FileIOException &fioex)
	{
		std::cerr << "I/O error while reading configuration file." << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(const ParseException &pex)
	{
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
				  << " - " << pex.getError() << std::endl;
		exit(EXIT_FAILURE);
	}

	const Setting& root = cfg.getRoot();

	//////////////////////////////////////////////////////////////////////////////
	// Check source/destination dirs. But we must remember that those dirs
	// from CLI have bigger priority.

	if (source == "")
	{
		std::string source_dir_cfg;
		if (root.lookupValue("source_dir", source_dir_cfg))
		{
			source = {source_dir_cfg};
		}else
		{
			std::cerr << "\nERROR: There is no 'Source_dir' neither in parameters nor in config file" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	if (destination == "")
	{
		std::string destination_dir_cfg;
		if (root.lookupValue("destination_dir", destination_dir_cfg))
		{
			destination = {destination_dir_cfg};
		}else
		{
			std::cerr << "\nERROR: There is no 'Destination_dir' neither in parameters nor in config file" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// replace ~ with a home dir path if needed

	const char *home = getenv("HOME");

	if (home)
	{
		if (size_t tylda = source.find("~"); tylda != std::string::npos)
		{
			source.replace(tylda, 1, home);
		}

		if (size_t tylda = source.find("~"); tylda != std::string::npos)
		{
			destination.replace(tylda, 1, home);
		}
	}

	renamer.setPaths(source, destination);

	//////////////////////////////////////////////////////////////////////////////

	bool keep_dir_structure = false;
	getVar(root, "keep_dir_structure", keep_dir_structure);
	renamer.setKeepDirStructure(keep_dir_structure);

	std::string copy_or_rename_mode_str;
	Renamer::CopyOrRename copy_or_rename_mode = Renamer::CopyOrRename::copy;

	getVar(root, "copy_or_rename", copy_or_rename_mode_str);

	if (copy_or_rename_mode_str == "copy")
	{
		copy_or_rename_mode = Renamer::CopyOrRename::copy;
	}else
	if (copy_or_rename_mode_str == "rename")
	{
		copy_or_rename_mode = Renamer::CopyOrRename::rename;
	}

	renamer.setCopyOrRename(copy_or_rename_mode);

	//////////////////////////////////////////////////////////////////////////////

	const Setting &rules_raw = root["rules"];

	int count = rules_raw.getLength();

	std::string rule_type;
	for(int i = 0; i < count; ++i)
	{
		const Setting &rule_raw = rules_raw[i];
		getVar(rule_raw, "type", rule_type);

		if (rule_type == "date")
		{
			std::string date_format;
			getRuleVar(rule_raw, "date_format", rule_type, date_format);
			renamer.addDateRule(date_format);
		}else
		if (rule_type == "text")
		{
			std::string text;
			getRuleVar(rule_raw, "text", rule_type, text);
			renamer.addTextRule(text);
		}else
		if (rule_type == "dir")
		{
			std::string str_mode;
			RuleDir::Mode mode = RuleDir::Mode::parent_only;
			std::string separator;

			getRuleVar(rule_raw, "mode", rule_type, str_mode);

			if (str_mode == "whole path")
			{
				mode = RuleDir::Mode::whole;
			}else
			if (str_mode == "parent dir only")
			{
				mode = RuleDir::Mode::parent_only;
			}

			getRuleVar(rule_raw, "separator", rule_type, separator);

			renamer.addDirRule(mode, separator);
		}else
		if (rule_type == "integer")
		{
			std::string str_mode;
			RuleInteger::Mode mode = RuleInteger::Mode::global;
			int  start;
			int  step;
			int	 padding;

			getRuleVar(rule_raw, "mode", rule_type, str_mode);

			if (str_mode == "global")
			{
				mode = RuleInteger::Mode::global;
			}else
			if (str_mode == "local at every dir")
			{
				mode = RuleInteger::Mode::local_at_every_dir;
			}


			getRuleVar(rule_raw, "start", rule_type, start);
			getRuleVar(rule_raw, "step", rule_type, step);
			getRuleVar(rule_raw, "padding", rule_type, padding);

			renamer.addIntegerRule(mode, start, step, padding);
		}else
		if (rule_type == "extension")
		{
			std::string str_mode;
			RuleExtension::Mode mode = RuleExtension::Mode::sic;
			std::string ext;

			getRuleVar(rule_raw, "mode", rule_type, str_mode);
			if (str_mode == "sic")
			{
				mode = RuleExtension::Mode::sic;
			}else
			if (str_mode == "lowercase")
			{
				mode = RuleExtension::Mode::lowercase;
			}else
			if (str_mode == "uppercase")
			{
				mode = RuleExtension::Mode::uppercase;
			}

			getRuleVar(rule_raw, "ext", rule_type, ext);

			renamer.addExtensionRule(mode, ext);
		}
	}
}

void		ConfigurationParser::makeSureThatConfigDirIsExist()
{
	if (const char *xdg_config_path = getenv("XDG_CONFIG_HOME"); xdg_config_path)
	{
		std::string config_dir = {xdg_config_path};
		config_dir += "/nomenus-rex/";
		fs::create_directories(config_dir);
	}else
	if (const char *home_path = getenv("HOME"); home_path)
	{
		std::string config_dir = {home_path};
		config_dir += "/.config/nomenus-rex/";
		fs::create_directories(config_dir);
	}
}

std::string	ConfigurationParser::getConfigPathString(std::string raw_parameter)
{
	// User might give the path to config with a ~ sign.
	const char *home = getenv("HOME");
	if (home)
	{
		if (size_t tylda = raw_parameter.find("~"); tylda != std::string::npos)
		{
			raw_parameter.replace(tylda, 1, home);
		}
	}

	// We should check if the user's data is a valid path to the config.
	// If not, we should try to assemble something with XDG_CONFIG_HOME/HOME
	if (std::filesystem::exists(std::filesystem::path(raw_parameter)))
	{
		std::cerr << "NOTIFICATION: Using " << raw_parameter << " config file" << std::endl;
		return raw_parameter;
	}else
	{
		std::cerr << "ERROR: Config file " << raw_parameter << " doesn't exist" << std::endl;
	}


	//Nah, there isn't such file.
	//Let's try with XDG_CONFIG_HOME


	const char *xdg_home = getenv("XDG_CONFIG_HOME");
	if (xdg_home)
	{
		const std::string xdg_name = std::string(xdg_home) + std::string("/nomenus-rex/") + raw_parameter;
		if (std::filesystem::exists(std::filesystem::path(xdg_name)))
		{
			std::cerr << "NOTIFICATION: Using " << xdg_name << " config file" << std::endl;
			return xdg_name;
		}else
		{
			std::cerr << "ERROR: Config file " << xdg_name << " doesn't exist" << std::endl;
		}
	}

	// No luck with XDG_CONFIG_HOME. Let's try HOME
	if (home)
	{
		std::string home_name = std::string(home) + std::string("/.config/nomenus-rex/") + raw_parameter;
		if (std::filesystem::exists(std::filesystem::path(home_name)))
		{
			std::cerr << "NOTIFICATION: Using " << home_name << " config file" << std::endl;
			return home_name;
		}else
		{
			// No. Just no. We have nowehere elese to look for that file.
			std::cerr << "ERROR: Config file " << home_name << " doesn't exist" << std::endl;
			std::cerr << "Nowhere else to search for config. Halting." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}
