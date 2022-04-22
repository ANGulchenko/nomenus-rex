#include "tests.h"
#include <iostream>

void tests()
{
	RuleDir::test();
	RuleExtension::test();
	RuleFilename::test();
	RuleText::test();
	RuleInteger::test();
	RuleReplace::test();


	Progressator::test();


	std::cerr << "All tests are done.\n";
	exit(0);
}
