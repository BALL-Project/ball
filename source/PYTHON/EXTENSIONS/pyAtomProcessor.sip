// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyAtomProcessor.sip,v 1.4 2003/10/29 22:54:43 oliver Exp $
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
};

