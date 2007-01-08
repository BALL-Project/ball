// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: radiusRuleProcessor.sip,v 1.5 2003/11/01 09:27:39 oliver Exp $
//

class RadiusRuleProcessor
  : RuleProcessor
{
%TypeHeaderCode	
	#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
%End
  public:
  // BALL_CREATE(RadiusRuleProcessor)
  RadiusRuleProcessor();
  RadiusRuleProcessor(INIFile& /file/, const String& /prefix/ = "RadiusRules");
  RadiusRuleProcessor(const RadiusRuleProcessor& /rule_processor/);
  ~RadiusRuleProcessor();
  Processor::Result operator () (Atom& /atom/);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
