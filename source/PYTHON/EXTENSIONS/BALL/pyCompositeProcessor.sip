// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include <BALL/CONCEPT/composite.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Composite&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

