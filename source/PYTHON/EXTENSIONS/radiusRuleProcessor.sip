// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: radiusRuleProcessor.sip,v 1.4 2003/10/29 22:54:45 oliver Exp $
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
};
