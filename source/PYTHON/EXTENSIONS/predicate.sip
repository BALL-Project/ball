// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicate.sip,v 1.2 2003/08/26 09:18:19 oliver Exp $
//

class KernelPredicate
 : CompositePrdicate
{
%HeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL
%End
  public:
  virtual bool operator () (const Composite& /composite/) const throw();
};
