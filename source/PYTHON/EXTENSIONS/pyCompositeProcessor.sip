// $Id: pyCompositeProcessor.sip,v 1.1 2001/09/03 16:53:12 oliver Exp $

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

