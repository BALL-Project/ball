// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicateConcept.sip,v 1.7 2003/11/01 09:27:38 oliver Exp $
//

class UnaryCompositePredicate 
//  : public unary_function<T, bool> 
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL;
	typedef UnaryPredicate<Composite> UnaryCompositePredicate;
%End
  public:
	UnaryCompositePredicate() throw();
	~UnaryCompositePredicate() throw();
  bool operator () (const Composite&) const throw();
%MethodCode
	sipRes = sipCpp->operator()(*a0);
%End
};


class UnaryAtomPredicate 
//  : public unary_function<T, bool> 
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL;
	typedef UnaryPredicate<Atom> UnaryAtomPredicate;
%End
  public:
	UnaryAtomPredicate() throw();
	~UnaryAtomPredicate() throw();
  bool operator() (const Atom&) const throw();
%MethodCode
	sipRes = sipCpp->operator()(*a0);
%End
};
