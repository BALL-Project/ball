// $Id: pyResidueProcessor.sip,v 1.1 2001/09/03 16:53:14 oliver Exp $

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

