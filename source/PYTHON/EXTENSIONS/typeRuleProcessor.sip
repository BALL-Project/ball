// $Id: typeRuleProcessor.sip,v 1.1 2001/09/03 16:53:18 oliver Exp $

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
