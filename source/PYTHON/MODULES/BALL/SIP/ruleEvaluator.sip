// $Id: ruleEvaluator.sip,v 1.2 2001/07/25 11:32:36 oliver Exp $

class RuleEvaluator
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/ruleEvaluator.h>
%End
  public:

  // BALL_CREATE(RuleEvaluator)
  // typedef std::list<std::pair<Expression, String> > RuleList;
  // typedef StringHashMap<RuleList> RuleMap;
  RuleEvaluator() throw();
  RuleEvaluator(INIFile& /file/, const String& /prefix/) throw();
  RuleEvaluator(const RuleEvaluator& /evaluator/) throw();
  ~RuleEvaluator() throw();
  bool initialize(INIFile& /file/, const String& /prefix/) throw();
  const String& getPrefix() const throw();
  void setPrefix(const String& /prefix/) throw();
  // const RuleEvaluator& operator = (const RuleEvaluator& /evaluator/);
  virtual void clear() throw();
  String operator () (const Atom& /atom/) const;
  bool operator == (const RuleEvaluator& /evaluator/) const;
  bool isValid() const throw();
  // void dump(ostream& s = std::cout, Size indent_depth = 0) const throw();
};
