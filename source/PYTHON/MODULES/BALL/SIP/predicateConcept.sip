// $Id: predicateConcept.sip,v 1.1 2000/07/18 21:27:58 oliver Exp $

class UnaryCompositePredicate 
//  : public unary_function<T, bool> 
{
%HeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL;
	typedef UnaryPredicate<Composite> UnaryCompositePredicate;
%End
  public:
  virtual bool operator() (const Composite&) const;
};


class UnaryAtomPredicate 
//  : public unary_function<T, bool> 
{
%HeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL;
	typedef UnaryPredicate<Atom> UnaryAtomPredicate;
%End
  public:
  virtual bool operator() (const Atom&) const;
};


