// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ruleEvaluator.sip,v 1.4 2003/11/01 09:27:40 oliver Exp $
//

class RuleEvaluator
{
%TypeHeaderCode
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
%MethodCode
  sipRes = new String(sipCpp->operator () (*a0));
%End

  bool operator == (const RuleEvaluator& /evaluator/) const;
  bool isValid() const throw();
  // void dump(ostream& s = std::cout, Size indent_depth = 0) const throw();
};
