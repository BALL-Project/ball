// $Id: predicateConcept.sip,v 1.1 2001/09/03 16:53:11 oliver Exp $

class UnaryCompositePredicate 
//  : public unary_function<T, bool> 
{
%HeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL;
	typedef UnaryPredicate<Composite> UnaryCompositePredicate;
%End
  public:
	UnaryCompositePredicate() throw();
	~UnaryCompositePredicate() throw();
  virtual bool operator() (const Composite&) const throw();
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
	UnaryAtomPredicate() throw();
	~UnaryAtomPredicate() throw();
  virtual bool operator() (const Atom&) const throw();
};
