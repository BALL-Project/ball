// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: radiusRuleProcessor.C,v 1.3.2.1 2003/01/07 13:21:09 anker Exp $

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
