// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: chargeRuleProcessor.sip,v 1.2 2003/08/26 09:18:13 oliver Exp $
//

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
