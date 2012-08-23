// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class KernelPredicate
 : CompositePrdicate
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL
%End
  public:
  virtual bool operator () (const Composite&) const throw();
};
