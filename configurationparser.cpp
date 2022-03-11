#include "AnyOption/anyoption.h"
#include "configurationparser.h"

ConfigurationParser::ConfigurationParser(int argc, char *argv[], Renamer& renamer)
{
	std::string source;
	std::string destination;
	std::string config;

	AnyOption *opt = new AnyOption();
	opt->addUsage("nomenus-rex usage: ");
	opt->addUsage("");
	opt->addUsage(" -h  --help          Prints this help ");
	opt->addUsage(" -s  --source        Source dir");
	opt->addUsage(" -d  --destination   Destination dir");
	opt->addUsage(" -c  --config        Configuration file");
	opt->addUsage("");
	opt->addUsage("Dirs can be also set up in the config file. CLI parameters have higher priority.");

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

	delete opt;

	if (config == "")
	{
		std::cerr << "No config file specified!" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Parsing the config, creating the Rules objects and putting them into the array.



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
			std::cerr << "There is no 'Source_dir' neither in parameters nor in config file" << std::endl;
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
			std::cerr << "There is no 'Destination_dir' neither in parameters nor in config file" << std::endl;
			exit(EXIT_FAILURE);
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

			renamer.addIntegerRule(mode, start, step);
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
