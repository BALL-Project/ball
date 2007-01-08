// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentProcessor.sip,v 1.6 2004/02/25 11:22:40 oliver Exp $
//

class FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include "sipBALLFragment.h"
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

