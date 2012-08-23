// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class RuleEvaluator
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/ruleEvaluator.h>
	using namespace BALL;
%End
  public:

  // BALL_CREATE(RuleEvaluator)
  // typedef std::list<std::pair<Expression, String> > RuleList;
  // typedef StringHashMap<RuleList> RuleMap;
  RuleEvaluator() throw();
  RuleEvaluator(INIFile&, const String&) throw();
  RuleEvaluator(const RuleEvaluator&) throw();
  ~RuleEvaluator() throw();
  bool initialize(INIFile&, const String&) throw();
  const String& getPrefix() const throw();
  void setPrefix(const String&) throw();
  // const RuleEvaluator& operator = (const RuleEvaluator&);
  virtual void clear() throw();
  String operator () (const Atom&) const;
%MethodCode
  sipRes = new String(sipCpp->operator () (*a0));
%End

  bool operator == (const RuleEvaluator&) const;
  bool isValid() const throw();
  // void dump(ostream& s = std::cout, Size indent_depth = 0) const throw();
};
