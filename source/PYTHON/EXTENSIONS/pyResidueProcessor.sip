// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyResidueProcessor.sip,v 1.4 2003/10/29 22:54:45 oliver Exp $
//

class ResidueProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Residue> ResidueProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Residue& /atom/);
};

