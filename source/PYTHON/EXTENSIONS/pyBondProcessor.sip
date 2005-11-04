// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBondProcessor.sip,v 1.1.2.1 2005/11/04 08:07:07 oliver Exp $
//

class BondProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include "sipBALLBond.h"
	using namespace BALL;
	typedef UnaryProcessor<Bond> BondProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Bond& /atom/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

