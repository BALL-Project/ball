// $Id: pyCompositeProcessor.sip,v 1.1 2000/07/18 21:28:09 oliver Exp $

class CompositeProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
	public:
	bool start();
	bool finish();
	virtual Processor::Result operator () (Composite& /atom/);
};

