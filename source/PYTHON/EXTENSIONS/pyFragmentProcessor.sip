// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentProcessor.sip,v 1.4 2003/10/29 22:54:44 oliver Exp $
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
};

