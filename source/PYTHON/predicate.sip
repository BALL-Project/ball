// $Id: predicate.sip,v 1.2 2000/07/06 14:08:49 oliver Exp $

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
