// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: chargeRuleProcessor.C,v 1.3 2002/02/27 12:21:34 sturm Exp $

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
