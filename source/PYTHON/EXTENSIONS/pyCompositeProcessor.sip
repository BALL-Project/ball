// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyCompositeProcessor.sip,v 1.5 2003/11/01 09:27:38 oliver Exp $
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
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

