// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyAtomProcessor.sip,v 1.2 2003/08/26 09:18:20 oliver Exp $
//

class AtomProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
	public:
	bool start();
	bool finish();
	virtual Processor::Result operator () (Atom& /atom/);
};

