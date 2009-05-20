// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include "sipAPIBALL.h"
	typedef UnaryProcessor<Fragment> FragmentProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Fragment& /atom/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

