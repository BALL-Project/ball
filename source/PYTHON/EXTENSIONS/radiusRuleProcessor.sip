// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: radiusRuleProcessor.sip,v 1.2 2003/08/26 09:18:22 oliver Exp $
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
  virtual Processor::Result operator () (Atom& /atom/);
};
