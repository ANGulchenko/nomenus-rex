#include "tests.h"
#include <iostream>

void testsCmp(int testIndex, RuleBase& rule, const std::string& correct_answer)
{
	if (std::string res = rule.getString(); res != correct_answer)
	{
		std::cerr << "Test №" << testIndex << ": "
				  << rule.getTypeStr()<<" getString() == \"" << res << "\""
				  << " but should be \""<< correct_answer <<"\"" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void tests()
{
	{RuleText rule("Test"); testsCmp(1, rule, "Test");}
	{RuleText rule(""); 	testsCmp(2, rule, "");}

	{
		RuleInteger rule(RuleInteger::Mode::global, 0, 1, 4);
		std::filesystem::path path("");
		rule.process(path);
		testsCmp(3, rule, "0000");
		rule.process(path);
		testsCmp(4, rule, "0001");
		rule.process(path);
		testsCmp(5, rule, "0002");
	}

	{
		std::string data("123Bla123Bla");
		RuleReplace rule("Bla", "Fuh");
		rule.process(data);
		testsCmp(6, rule, "123Fuh123Fuh");
	}

	{
		std::string data("123Bla123Bla");
		RuleReplace rule("Bla", "");
		rule.process(data);
		testsCmp(7, rule, "123123");
	}

	{
		std::string data("123Bla123Bla");
		RuleReplace rule("", "Fuh");
		rule.process(data);
		testsCmp(8, rule, "123Bla123Bla");
	}



}
