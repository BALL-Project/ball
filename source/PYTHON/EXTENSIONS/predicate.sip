// $Id: predicate.sip,v 1.1 2001/09/03 16:53:11 oliver Exp $

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
