// $Id: predicateConcept.sip,v 1.4 2003/06/19 13:27:21 oliver Exp $

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
  bool operator () (const Composite&) const throw();
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
  bool operator() (const Atom&) const throw();
};
