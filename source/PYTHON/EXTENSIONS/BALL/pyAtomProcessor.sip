// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class AtomProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Atom&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};

