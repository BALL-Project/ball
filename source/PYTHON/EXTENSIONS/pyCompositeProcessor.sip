// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeProcessor.sip,v 1.3 2003/09/09 18:57:07 oliver Exp $
//

class CompositeProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Composite& /atom/);
};

