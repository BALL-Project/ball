// $Id: chargeRuleProcessor.C,v 1.2 2000/06/02 08:32:52 oliver Exp $

#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>

namespace BALL
{
	ChargeRuleProcessor::ChargeRuleProcessor()
	{
	}

	ChargeRuleProcessor::ChargeRuleProcessor
		(INIFile& file, const String& prefix)
	{
		initialize(file, prefix);
	}

	ChargeRuleProcessor::ChargeRuleProcessor
		(const ChargeRuleProcessor& rule_processor)
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
