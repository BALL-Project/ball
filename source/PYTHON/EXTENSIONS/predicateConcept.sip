// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicateConcept.sip,v 1.5 2003/08/26 09:18:19 oliver Exp $
//

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
