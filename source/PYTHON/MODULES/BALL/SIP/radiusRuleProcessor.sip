// $Id: radiusRuleProcessor.sip,v 1.1 2000/11/15 14:20:59 oliver Exp $

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
