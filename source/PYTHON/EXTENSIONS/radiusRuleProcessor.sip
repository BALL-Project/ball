// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: radiusRuleProcessor.sip,v 1.3 2003/09/09 18:57:07 oliver Exp $
//

class RadiusRuleProcessor
  : RuleProcessor
{
%HeaderCode	
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
