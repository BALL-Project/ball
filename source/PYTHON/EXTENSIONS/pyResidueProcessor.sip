// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ResidueProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include "sipAPIBALL.h"
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

