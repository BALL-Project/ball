// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typeRuleProcessor.sip,v 1.4 2003/10/29 22:54:48 oliver Exp $
//

class TypeRuleProcessor
	: RuleProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>
	using namespace BALL;
%End
	public:
	// BALL_CREATE(TypeRuleProcessor)
	TypeRuleProcessor();
	TypeRuleProcessor(INIFile& /file/, const String& /prefix/);
	TypeRuleProcessor(const TypeRuleProcessor& /rule_processor/);
	~TypeRuleProcessor();

	Processor::Result operator () (Atom& /atom/);
};
