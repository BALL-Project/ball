// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeProcessor.sip,v 1.4 2003/10/29 22:54:44 oliver Exp $
//

class CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Composite& /atom/);
};

