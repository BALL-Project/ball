// $Id: radiusRuleProcessor.C,v 1.3 2000/06/02 08:32:52 oliver Exp $

#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>

namespace BALL
{
	RadiusRuleProcessor::RadiusRuleProcessor()
	{
	}

	RadiusRuleProcessor::RadiusRuleProcessor
		(INIFile& file, const String& prefix)
	{
		initialize(file, prefix);
	}

	RadiusRuleProcessor::RadiusRuleProcessor
		(const RadiusRuleProcessor& rule_processor)
		: RuleProcessor(rule_processor)
	{
	}

	RadiusRuleProcessor::~RadiusRuleProcessor()
	{
	}

	Processor::Result RadiusRuleProcessor::operator () (Atom& atom)
	{
		atom.setRadius(evaluate(atom).toFloat());
		return Processor::CONTINUE;
	}
}
