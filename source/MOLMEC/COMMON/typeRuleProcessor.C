// $Id: typeRuleProcessor.C,v 1.1 2000/05/29 11:19:58 oliver Exp $

#include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>

namespace BALL
{
	TypeRuleProcessor::TypeRuleProcessor()
	{
	}

	TypeRuleProcessor::TypeRuleProcessor(INIFile& file,
			const String& prefix = "TypeRules")
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
