// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyResidueProcessor.sip,v 1.3 2003/09/09 18:57:07 oliver Exp $
//

class ResidueProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Residue> ResidueProcessor;
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Residue& /atom/);
};

