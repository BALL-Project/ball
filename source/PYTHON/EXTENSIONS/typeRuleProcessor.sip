// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typeRuleProcessor.sip,v 1.3 2003/09/09 18:57:08 oliver Exp $
//

class TypeRuleProcessor
	: RuleProcessor
{
%HeaderCode
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
