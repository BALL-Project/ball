// $Id: predicate.sip,v 1.2 2001/07/25 11:32:34 oliver Exp $

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
