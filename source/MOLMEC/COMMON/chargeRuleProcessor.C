// $Id: chargeRuleProcessor.C,v 1.1 2000/05/24 12:12:53 anker Exp $

#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>

namespace BALL
{
	ChargeRuleProcessor::ChargeRuleProcessor()
	{
	}

	ChargeRuleProcessor::ChargeRuleProcessor(INIFile& file,
			const String& prefix = "ChargeRules")
	{
		initialize(file, prefix);
	}

	ChargeRuleProcessor::ChargeRuleProcessor(
			const ChargeRuleProcessor& rule_processor)
		: RuleProcessor(rule_processor)
	{
	}

	ChargeRuleProcessor::~ChargeRuleProcessor()
	{
	}

	Processor::Result ChargeRuleProcessor::operator () (Atom& atom)
	{
		atom.setCharge(evaluate(atom).toFloat());
		return Processor::CONTINUE;
	}
}
