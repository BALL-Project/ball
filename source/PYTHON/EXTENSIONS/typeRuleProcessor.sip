// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typeRuleProcessor.sip,v 1.2 2003/08/26 09:18:24 oliver Exp $
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

	virtual Processor::Result operator () (Atom& /atom/);
};
