// $Id: chargeRuleProcessor.sip,v 1.1 2001/09/03 16:53:05 oliver Exp $


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
