// $Id: ruleProcessor.sip,v 1.1 2000/11/15 14:21:00 oliver Exp $

class RuleProcessor
  : AtomProcessor
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/ruleProcessor.h>
%End
  public:
  // BALL_CREATE(RuleProcessor)
  RuleProcessor();
  RuleProcessor(INIFile& /file/, const String& /prefix/);
  RuleProcessor(const RuleProcessor& /rule_processor/);
  ~RuleProcessor();
  void clear();
  void destroy();   
  bool initialize(INIFile& /file/, const String& /prefix/);
  // const RuleProcessor& operator = (const RuleProcessor& /rule_processor/);
  void set(const RuleProcessor& /rule_processor/);
  virtual bool start();
  virtual bool finish();
  virtual Processor::Result operator () (Atom& /atom/);
  String evaluate(const Atom& /atom/);
  bool isValid() const;
  // void dump(std::ostream& s = std::cout) const;
};
