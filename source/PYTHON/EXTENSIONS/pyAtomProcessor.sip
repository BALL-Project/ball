// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyAtomProcessor.sip,v 1.5 2003/11/01 09:27:38 oliver Exp $
//

class AtomProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Atom& /atom/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

