// $Id: pyCompositeProcessor.sip,v 1.1 2000/07/18 17:02:45 oliver Exp $

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

