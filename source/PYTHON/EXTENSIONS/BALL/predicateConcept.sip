// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class UnaryCompositePredicate 
//  : public unary_function<T, bool> 
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	#include <BALL/CONCEPT/composite.h>
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
	#include <BALL/KERNEL/atom.h>
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
