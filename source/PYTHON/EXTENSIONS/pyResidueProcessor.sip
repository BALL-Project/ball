// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyResidueProcessor.sip,v 1.6 2004/02/25 11:22:41 oliver Exp $
//

class ResidueProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include "sipBALLResidue.h"
	typedef UnaryProcessor<Residue> ResidueProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Residue& /atom/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

