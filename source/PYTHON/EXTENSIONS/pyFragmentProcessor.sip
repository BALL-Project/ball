// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyFragmentProcessor.sip,v 1.3 2003/09/09 18:57:07 oliver Exp $
//

class FragmentProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Fragment> FragmentProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Fragment& /atom/);
};

