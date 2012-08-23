// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class ChainProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include <BALL/KERNEL/chain.h>
	using namespace BALL;
	typedef UnaryProcessor<Chain> ChainProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Chain&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};


