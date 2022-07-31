#include "tests.h"

void testRule(WhatToInit whatInit,
			  WhatToTest whatTest,
			  RuleBase& rule,
			  const std::string& init_string,
			  const std::string& correct_answer);


void tests()
{
	/// RuleExec //////////////////////////////////////////////////////////////////
	{
		RuleExec rule("echo '<Placeholder>' | grep -Eo '[0-9]+'", "<Placeholder>");
		testRule(WhatToInit::absolute, WhatToTest::getString, rule, "/test/ololo0001.txt", "0001");
	}
	/// RuleText //////////////////////////////////////////////////////////////////
	{
		RuleText rule("Test");
		testRule(WhatToInit::relative, WhatToTest::getString, rule, "", "Test");
	}
	{
		RuleText rule("");
		testRule(WhatToInit::relative, WhatToTest::getString, rule, "", "");
	}

	/// RuleDir //////////////////////////////////////////////////////////////////

	{
		RuleDir rule(RuleDir::Mode::whole, "_");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.dat",
				 "dir_subdir1_subdir2");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.dat",
				 "dirsubdir1subdir2");
	}

	{
		RuleDir rule(RuleDir::Mode::whole, "");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "file.dat",
				 "");
	}
	{
		RuleDir rule(RuleDir::Mode::parent_only, "_");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.dat",
				 "subdir2");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "_");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/поддиректория/الدليل/file.dat",
				 "dir_поддиректория_الدليل");
	}
	{
		RuleDir rule(RuleDir::Mode::whole, "[separator]");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.dat",
				 "dir[separator]subdir1[separator]subdir2");
	}

	/// RuleExtension //////////////////////////////////////////////////////////////////
	{
		RuleExtension rule(RuleExtension::Mode::lowercase, "");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.dAt",
				 ".dat");
	}

	{
		RuleExtension rule(RuleExtension::Mode::lowercase, "");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.РаСширение",
				 ".расширение");
	}

	{
		RuleExtension rule(RuleExtension::Mode::lowercase, "");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.",
				 ".");
	}

	{
		RuleExtension rule(RuleExtension::Mode::lowercase, ".ext");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/file.dat",
				 ".ext");
	}

	/// RuleFilename //////////////////////////////////////////////////////////////////

	{
		RuleFilename rule(RuleFilename::Mode::lowercase);
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/fIlE.dAt",
				 "file");
	}

	{
		RuleFilename rule(RuleFilename::Mode::uppercase);
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "dir/subdir1/subdir2/fileфайл.dat",
				 "FILEФАЙЛ");
	}

	/// RuleInteger //////////////////////////////////////////////////////////////////

	{
		RuleInteger rule(RuleInteger::Mode::global, 0, 1, 4);

		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "",
				 "0000");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "",
				 "0001");
		testRule(WhatToInit::relative, WhatToTest::getString, rule,
				 "",
				 "0002");
	}

	/// RuleInteger //////////////////////////////////////////////////////////////////

	{
		RuleReplace rule("Bla", "Fuh");
		testRule(WhatToInit::in_process, WhatToTest::in_process, rule,
				 "123Bla123Bla",
				 "123Fuh123Fuh");
	}

	{
		RuleReplace rule("Bla", "");
		testRule(WhatToInit::in_process, WhatToTest::in_process, rule,
				 "123Bla123Bla",
				 "123123");
	}

	{
		RuleReplace rule("", "Fuh");
		testRule(WhatToInit::in_process, WhatToTest::in_process, rule,
				 "123Bla123Bla",
				 "123Bla123Bla");
	}

	std::cerr << "All tests are done.\n";
	//exit(0);
}


void testRule(WhatToInit whatInit,
			  WhatToTest whatTest,
			  RuleBase& rule,
			  const std::string& init_string,
			  const std::string& correct_answer)
{

	std::string in_process("");
	std::filesystem::path abs("");
	std::filesystem::path rel("");

	switch (whatInit)
	{
		case WhatToInit::relative:
		{
			rel = std::filesystem::path(init_string);
		}break;
		case WhatToInit::absolute:
		{
			abs = std::filesystem::path(init_string);
		}break;
		case WhatToInit::in_process:
		{
			in_process = init_string;
		}break;
	}


	RuleParams params = {.absolute_path = abs,
						 .relative_path = rel,
						 .name_in_process = in_process};

	rule.process(params);

	switch (whatTest)
	{
		case WhatToTest::getString:
		{
			if (std::string res = rule.getString(); res != correct_answer)
			{
				std::cerr << "(" << typeid(rule).name() <<"): \n"
						  <<" getString() == \"" << res << "\" \n"
						  << " but should be \""<< correct_answer <<"\"" << std::endl;
				exit(EXIT_FAILURE);
			}
		}break;
		case WhatToTest::in_process:
		{
			if (std::string res = params.name_in_process; res != correct_answer)
			{
				std::cerr << "(" << typeid(rule).name() <<"): \n"
						  <<" name_in_process == \"" << res << "\" \n"
						  << " but should be \""<< correct_answer <<"\"" << std::endl;
				exit(EXIT_FAILURE);
			}
		}break;

	default:
		break;
	}

}

