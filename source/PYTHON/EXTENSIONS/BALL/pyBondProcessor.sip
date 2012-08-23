// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class BondProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include <BALL/KERNEL/bond.h>
	using namespace BALL;
	typedef UnaryProcessor<Bond> BondProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Bond&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

