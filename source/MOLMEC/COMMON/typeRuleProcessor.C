// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typeRuleProcessor.C,v 1.2.2.1 2003/01/07 13:21:11 anker Exp $

#include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>

namespace BALL
{
	TypeRuleProcessor::TypeRuleProcessor()
	{
	}

	TypeRuleProcessor::TypeRuleProcessor
		(INIFile& file, const String& prefix)
	{
		initialize(file, prefix);
	}

	TypeRuleProcessor::TypeRuleProcessor
		(const TypeRuleProcessor& rule_processor)
		: RuleProcessor(rule_processor)
	{
	}

	TypeRuleProcessor::~TypeRuleProcessor()
	{
	}

	Processor::Result TypeRuleProcessor::operator () (Atom& atom)
	{
		atom.setType((Atom::Type)evaluate(atom).toInt());
		return Processor::CONTINUE;
	}
}
