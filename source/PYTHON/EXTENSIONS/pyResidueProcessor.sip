// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyResidueProcessor.sip,v 1.2 2003/08/26 09:18:21 oliver Exp $
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
	virtual Processor::Result operator () (Residue& /atom/);
};

