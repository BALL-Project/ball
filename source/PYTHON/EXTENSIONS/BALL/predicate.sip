class KernelPredicate
	: CompositePrdicate
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
%End
	public:
	virtual bool operator () (const Composite&) const throw();
};
