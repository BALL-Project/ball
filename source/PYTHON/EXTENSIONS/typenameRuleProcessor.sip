// $Id: typenameRuleProcessor.sip,v 1.1 2001/09/03 16:53:18 oliver Exp $

class TypenameRuleProcessor
	: RuleProcessor
{
%HeaderCode
  #include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>
  using namespace BALL;
%End
	public:
	//BALL_CREATE(TypenameRuleProcessor)
	TypenameRuleProcessor();
	TypenameRuleProcessor(INIFile& /file/, const String& /prefix/);
	TypenameRuleProcessor(const TypenameRuleProcessor& /rule_processor/);
	~TypenameRuleProcessor();

	virtual Processor::Result operator () (Atom& /atom/);
};
