#include <stdlib.h>

#include "AnyOption/anyoption.h"
#include "configurationparser.h"
#include "version.h"

ConfigurationParser::ConfigurationParser(int argc, char *argv[], bool& askConfirmationBeforeFileProcessing, Renamer& renamer)
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
	opt->addUsage(" -e  --example       Prints out the example configuration");
	opt->addUsage(" -y  --yes           Process files without confirmation");
	opt->addUsage("");
	opt->addUsage("Directories can be also set up in the config file. CLI parameters have higher priority.");

	opt->setFlag("help", 'h');
	opt->setCommandOption("source", 's');
	opt->setCommandOption("destination", 'd');
	opt->setCommandOption("config", 'c');
	opt->setFlag("yes", 'y');
	opt->setFlag("example", 'e');

	opt->processCommandArgs(argc, argv);

	if (opt->hasOptions())
	{
		if (opt->getFlag("help") || opt->getFlag('h'))
		{
			opt->printUsage();
			exit(EXIT_SUCCESS);
		}

		if (opt->getFlag("yes") || opt->getFlag('y'))
		{
			askConfirmationBeforeFileProcessing = false;
		}else
		{
			askConfirmationBeforeFileProcessing = true;
		}

		if (opt->getFlag("example") || opt->getFlag('e'))
		{
			printTypicalConfig();
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

	// We should check if "source" exists. Destination doesn't matter -- it'll
	// be created while processing anyway.

	if (!filesystem::exists(fs::path(source)))
	{
		std::cerr << "\nERROR: Source dir("<< source <<") doesn't exist." << std::endl;
		exit(EXIT_FAILURE);
	}

	renamer.source_dir = source;
	renamer.destination_dir = destination;

	//////////////////////////////////////////////////////////////////////////////

	bool keep_dir_structure = false;
	getVar(root, "keep_dir_structure", keep_dir_structure);
	renamer.keep_dir_structure = keep_dir_structure;

	Renamer::CopyOrRename copy_or_rename_mode =
			enumParser<Renamer::CopyOrRename>(root, "Config root", "copy_or_rename",
					   {
						 {"copy", Renamer::CopyOrRename::copy},
						 {"rename", Renamer::CopyOrRename::rename}
					   });
	renamer.copy_or_rename = copy_or_rename_mode;

	Renamer::SortMode sort_mode =
			enumParser<Renamer::SortMode>(root, "Config root", "sort_mode",
					   {
						 {"sic", Renamer::SortMode::sic},
						 {"az", Renamer::SortMode::az},
						 {"za", Renamer::SortMode::za}
					   });
	renamer.sort_mode = sort_mode;

	//////////////////////////////////////////////////////////////////////////////

	const Setting &rules_raw = root["rules"];

	int count = rules_raw.getLength();

	std::string rule_type;
	for(int i = 0; i < count; ++i)
	{
		const Setting &rule_raw = rules_raw[i];
		getVar(rule_raw, "type", rule_type);

		////////////////////////////////////////////////////////////////////////
		if (rule_type == "date")
		{
			std::string date_format;
			getRuleVar(rule_raw, "date_format", rule_type, date_format);
			//renamer.rules.push_back(std::unique_ptr<RuleBase>(new RuleDate(date_format)));
			renamer.rules.push_back(std::make_unique<RuleDate>(date_format));
		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "text")
		{
			std::string text;
			getRuleVar(rule_raw, "text", rule_type, text);
			renamer.rules.push_back(std::make_unique<RuleText>(text));
		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "dir")
		{
			RuleDir::Mode mode =
				enumParser<RuleDir::Mode>(rule_raw, rule_type, "mode",
				   {
					 {"whole path", RuleDir::Mode::whole},
					 {"parent dir only", RuleDir::Mode::parent_only}
				   });

			std::string separator;
			getRuleVar(rule_raw, "separator", rule_type, separator);
			renamer.rules.push_back(std::make_unique<RuleDir>(mode, separator));
		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "integer")
		{
			RuleInteger::Mode mode =
				enumParser<RuleInteger::Mode>(rule_raw, rule_type, "mode",
				   {
					 {"global", RuleInteger::Mode::global},
					 {"local at every dir", RuleInteger::Mode::local_at_every_dir}
				   });

			int  start;
			int  step;
			int	 padding;
			getRuleVar(rule_raw, "start", rule_type, start);
			getRuleVar(rule_raw, "step", rule_type, step);
			getRuleVar(rule_raw, "padding", rule_type, padding);

			renamer.rules.push_back(std::make_unique<RuleInteger>(mode, start, step, padding));

		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "extension")
		{
			RuleExtension::Mode mode =
				enumParser<RuleExtension::Mode>(rule_raw, rule_type, "mode",
				   {
					 {"sic", RuleExtension::Mode::sic},
					 {"lowercase", RuleExtension::Mode::lowercase},
					 {"uppercase", RuleExtension::Mode::uppercase}
				   });

			std::string ext;
			getRuleVar(rule_raw, "ext", rule_type, ext);

			renamer.rules.push_back(std::make_unique<RuleExtension>(mode, ext));

		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "filename")
		{
			RuleFilename::Mode mode =
				enumParser<RuleFilename::Mode>(rule_raw, rule_type, "mode",
				   {
					 {"sic", RuleFilename::Mode::sic},
					 {"lowercase", RuleFilename::Mode::lowercase},
					 {"uppercase", RuleFilename::Mode::uppercase}
				   });

			renamer.rules.push_back(std::make_unique<RuleFilename>(mode));

		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "filesize")
		{
			RuleFilesize::Dimension dimension =
				enumParser<RuleFilesize::Dimension>(rule_raw, rule_type, "dimension",
				   {
					 {"B", RuleFilesize::Dimension::B},
					 {"KiB", RuleFilesize::Dimension::KiB},
					 {"MiB", RuleFilesize::Dimension::MiB},
					 {"GiB", RuleFilesize::Dimension::GiB}
				   });


			std::string decimal_separator;
			bool show_dimension;
			getRuleVar(rule_raw, "decimal_separator", rule_type, decimal_separator);
			getRuleVar(rule_raw, "show_dimension", rule_type, show_dimension);

			renamer.rules.push_back(std::make_unique<RuleFilesize>(dimension, decimal_separator, show_dimension));

		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "filedate")
		{
			std::string date_format;
			getRuleVar(rule_raw, "date_format", rule_type, date_format);
			renamer.rules.push_back(std::make_unique<RuleFileCreationDate>(date_format));
		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "replace")
		{
			std::string  what;
			std::string  to;
			getRuleVar(rule_raw, "what", rule_type, what);
			getRuleVar(rule_raw, "to", rule_type, to);

			renamer.rules.push_back(std::make_unique<RuleReplace>(what, to));
		}
		/////////////////////////////////////////////////////////////////////////
		else
		if (rule_type == "exec")
		{
			std::string  command;
			std::string  placeholder;
			getRuleVar(rule_raw, "command", rule_type, command);
			getRuleVar(rule_raw, "placeholder", rule_type, placeholder);

			renamer.rules.push_back(std::make_unique<RuleExec>(command, placeholder));
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

	return raw_parameter;
}

void	ConfigurationParser::printTypicalConfig()
{

	fs::path current_path = std::filesystem::current_path();
	cout << "source_dir = " << current_path << ";" << std::endl;
	cout << "destination_dir = " << current_path << ";" << std::endl;

const char* default_config = R"CONFIG(
keep_dir_structure = false;
copy_or_rename = "copy";
//sort_mode = "az"|"za"|"sic"
sort_mode = "az";

rules = (
	{
		type        = "text";
		text        = "PREFIX_";
	},
	{
		type        = "dir";
		// mode     = "whole path"|"parent dir only"
		mode        = "whole path";
		separator   = "-";

	},
	{
		type        = "text";
		text        = "_";
	},
	{
		type        = "integer";
		// mode     = "global"|"local at every dir"
		mode        = "local at every dir";
		start       = 0;
		step        = 1;
		padding     = 5;
	},
	{
		type        = "extension";
		// leave the "ext" variable empty to use an original extension
		ext         = "";
		// mode     = "lowercase"|"uppercase"|"sic";
		mode        = "lowercase";
	}
	/*{
		type        = "date";
		date_format = "%Y-%m-%d";
	},*/
	/*{
		type        = "filedate";
		date_format = "%Y-%m-%d";
	},*/
	/*{
	   type        = "filename";
	   // mode     = "lowercase"|"uppercase"|"sic";
	   mode        = "lowercase";
	},*/
	/*{
	   type              = "filesize";
	   // dimension      = "B"|"KiB"|"MiB"|"GiB"
	   dimension         = "KiB";
	   show_dimension    = true;
	   decimal_separator = ",";
	},*/
	/*{
	   type        = "replace";
	   what        = "BlaBla";
	   to          = "Ololo";
	},*/
	/*{
	   type        = "exec";
	   command     = "echo '<Placeholder>' | grep -Eo '[0-9]+'";
	   placeholder = "<Placeholder>";
	},*/
);
)CONFIG";

std::cout << default_config << std::endl;
}
