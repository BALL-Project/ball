// $Id: typenameRuleProcessor.C,v 1.2 2000/06/02 08:32:52 oliver Exp $

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
