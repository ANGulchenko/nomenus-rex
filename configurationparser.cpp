#include <libconfig.h++>

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

	using namespace libconfig;

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
			renamer.addDateRule(date_format);
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
			renamer.addTextRule(text);
		}else
		if (rule_type == "dir")
		{
			renamer.addDirRule();
		}else
		if (rule_type == "integer")
		{
			renamer.addIntegerRule();
		}else
		if (rule_type == "extension")
		{
			renamer.addExtensionRule();
		}
	}
}
