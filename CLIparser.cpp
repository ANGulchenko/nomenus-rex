#include "CLIparser.h"
#include "AnyOption/anyoption.h"

void CLIparser(int argc, char *argv[], std::string& source, std::string& destination, std::string& config)
{
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
}
