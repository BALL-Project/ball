// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: chargeRuleProcessor.sip,v 1.3 2003/09/09 18:57:05 oliver Exp $
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
  Processor::Result operator () (Atom& /atom/);
 };
