// $Id: chargeRuleProcessor.sip,v 1.1 2000/11/15 14:20:59 oliver Exp $


class ChargeRuleProcessor
  : RuleProcessor
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
%End
  public:
  // BALL_CREATE(ChargeRuleProcessor)
  ChargeRuleProcessor();
  ChargeRuleProcessor(INIFile& /file/, const String& /prefix/ = "ChargeRules");
  ChargeRuleProcessor(const ChargeRuleProcessor& /rule_processor/);
  ~ChargeRuleProcessor();
  virtual Processor::Result operator () (Atom& /atom/);
 };
