// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicate.sip,v 1.3 2003/10/29 22:54:42 oliver Exp $
//

class KernelPredicate
 : CompositePrdicate
{
%TypeHeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL
%End
  public:
  virtual bool operator () (const Composite& /composite/) const throw();
};
