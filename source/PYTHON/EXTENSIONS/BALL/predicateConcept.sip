class UnaryCompositePredicate
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	#include <BALL/CONCEPT/composite.h>
	using UnaryCompositePredicate = UnaryPredicate<Composite>;
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
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	#include <BALL/KERNEL/atom.h>
	using UnaryAtomPredicate = UnaryPredicate<Atom>;
%End
	public:
	UnaryAtomPredicate() throw();
	~UnaryAtomPredicate() throw();
	bool operator() (const Atom&) const throw();
%MethodCode
	sipRes = sipCpp->operator()(*a0);
%End
};
