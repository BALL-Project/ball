// $Id: pyResidueProcessor.sip,v 1.1 2000/08/07 17:46:19 oliver Exp $

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

