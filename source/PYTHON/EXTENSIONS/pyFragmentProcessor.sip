// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentProcessor.sip,v 1.5 2003/11/01 09:27:39 oliver Exp $
//

class FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
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

