// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeProcessor.sip,v 1.2 2003/08/26 09:18:20 oliver Exp $
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
	virtual Processor::Result operator () (Composite& /atom/);
};

