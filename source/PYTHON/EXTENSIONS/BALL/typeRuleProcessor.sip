// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
	TypeRuleProcessor(INIFile&, const String&);
	TypeRuleProcessor(const TypeRuleProcessor&);
	~TypeRuleProcessor();

	Processor::Result operator () (Atom&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
