// $Id: typenameRuleProcessor.C,v 1.1 2000/05/24 16:33:16 oliver Exp $

#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>

namespace BALL
{
	TypenameRuleProcessor::TypenameRuleProcessor()
	{
	}

	TypenameRuleProcessor::TypenameRuleProcessor(INIFile& file,
			const String& prefix = "TypenameRules")
	{
		initialize(file, prefix);
	}

	TypenameRuleProcessor::TypenameRuleProcessor(
			const TypenameRuleProcessor& rule_processor)
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
