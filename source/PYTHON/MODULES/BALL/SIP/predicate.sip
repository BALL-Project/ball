// $Id: predicate.sip,v 1.1 2000/07/18 21:27:56 oliver Exp $

class KernelPredicate
 : CompositePrdicate
{
%HeaderCode
	#include <BALL/CONCEPT/predicate.h>
	using namespace BALL
%End
  public:
  virtual bool operator () (const Composite& /composite/) const;
};
