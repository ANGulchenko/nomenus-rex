#ifndef RULEFILENAME_H
#define RULEFILENAME_H

#include "ruleBase.h"
#include <filesystem>

class RuleFilename: public RuleBase
{
public:
	enum class Mode {lowercase, uppercase, sic};
	explicit RuleFilename(Mode _mode);
	void	process(const RuleParams& params) override;
private:
	Mode		mode;
};

#endif
