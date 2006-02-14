// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBondProcessor.sip,v 1.2.6.1 2006/02/14 15:02:44 amoll Exp $
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

