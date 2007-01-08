// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: chargeRuleProcessor.sip,v 1.5 2003/11/01 09:27:37 oliver Exp $
//

class ChargeRuleProcessor
  : RuleProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
%End
  public:
  // BALL_CREATE(ChargeRuleProcessor)
  ChargeRuleProcessor();
  ChargeRuleProcessor(INIFile& /file/, const String& /prefix/ = "ChargeRules");
  ChargeRuleProcessor(const ChargeRuleProcessor& /rule_processor/);
  ~ChargeRuleProcessor();
  Processor::Result operator () (Atom& /atom/);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
 };
