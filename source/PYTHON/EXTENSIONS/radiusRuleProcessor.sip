// $Id: radiusRuleProcessor.sip,v 1.1 2001/09/03 16:53:15 oliver Exp $

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
