// $Id: pyCompositeProcessor.sip,v 1.1 2000/07/06 14:08:50 oliver Exp $

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

