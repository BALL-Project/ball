// $Id: radiusRuleProcessor.C,v 1.2 2000/05/24 12:12:53 anker Exp $

#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>

namespace BALL
{
	RadiusRuleProcessor::RadiusRuleProcessor()
	{
	}

	RadiusRuleProcessor::RadiusRuleProcessor(INIFile& file,
			const String& prefix = "RadiusRules")
	{
		initialize(file, prefix);
	}

	RadiusRuleProcessor::RadiusRuleProcessor(
			const RadiusRuleProcessor& rule_processor)
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
