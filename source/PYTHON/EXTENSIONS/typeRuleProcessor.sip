// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typeRuleProcessor.sip,v 1.5 2003/11/01 09:27:40 oliver Exp $
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
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
