// $Id: predicateConcept.sip,v 1.2 2003/04/20 12:46:12 oliver Exp $

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
	virtual ~UnaryCompositePredicate() throw();
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
	virtual ~UnaryAtomPredicate() throw();
  virtual bool operator() (const Atom&) const throw();
};
