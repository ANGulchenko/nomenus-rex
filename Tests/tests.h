#ifndef TESTS_H
#define TESTS_H

#include "../Rules/rules.h"
#include "../Progressator/progressator.h"

enum class WhatToInit {relative, absolute, in_process};
enum class WhatToTest {getString, in_process};
void testForRelativeName(WhatToInit whatInit, WhatToTest whatTest, RuleBase& rule, const std::string& relative_name, const std::string& correct_answer);

void testsCmp(int testIndex, RuleBase& rule, const RuleParams& params, const std::string& correct_answer);
void tests();

#endif
