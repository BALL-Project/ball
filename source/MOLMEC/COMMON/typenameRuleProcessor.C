// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typenameRuleProcessor.C,v 1.2.2.1 2003/01/07 13:21:11 anker Exp $

#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>

namespace BALL
{
	TypenameRuleProcessor::TypenameRuleProcessor()
	{
	}

	TypenameRuleProcessor::TypenameRuleProcessor
		(INIFile& file, const String& prefix)
	{
		initialize(file, prefix);
	}

	TypenameRuleProcessor::TypenameRuleProcessor
		(const TypenameRuleProcessor& rule_processor)
		: RuleProcessor(rule_processor)
	{
	}

	TypenameRuleProcessor::~TypenameRuleProcessor()
	{
	}

	Processor::Result TypenameRuleProcessor::operator () (Atom& atom)
	{
		atom.setTypeName(evaluate(atom));
		return Processor::CONTINUE;
	}
}
