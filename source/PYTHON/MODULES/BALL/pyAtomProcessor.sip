// $Id: pyAtomProcessor.sip,v 1.1 2000/07/18 17:02:43 oliver Exp $

class AtomProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
	public:
	bool start();
	bool finish();
	virtual Processor::Result operator () (Atom& /atom/);
};

