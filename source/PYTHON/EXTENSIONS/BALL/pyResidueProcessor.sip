// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ResidueProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include <BALL/KERNEL/residue.h>
	using namespace BALL;
	typedef UnaryProcessor<Residue> ResidueProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Residue&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

