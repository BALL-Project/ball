// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class RuleProcessor
  : AtomProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/ruleProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  // BALL_CREATE(RuleProcessor)
  RuleProcessor();
  RuleProcessor(INIFile&, const String&);
  RuleProcessor(const RuleProcessor&);
  ~RuleProcessor();
  void clear();
  void destroy();   
  bool initialize(INIFile&, const String&);
  // const RuleProcessor& operator = (const RuleProcessor&);
  void set(const RuleProcessor&);
  virtual bool start();
  virtual bool finish();
  Processor::Result operator () (Atom&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End

  String evaluate(const Atom&);
  bool isValid() const;
  // void dump(std::ostream& s = std::cout) const;
};
