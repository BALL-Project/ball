// $Id: pyAtomProcessor.sip,v 1.1 2001/09/03 16:53:12 oliver Exp $

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

