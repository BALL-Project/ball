// $Id: pyFragmentProcessor.sip,v 1.1 2001/09/03 16:53:13 oliver Exp $

class FragmentProcessor
{
%HeaderCode
	#include <BALL/CONCEPT/processor.h>
	typedef UnaryProcessor<Fragment> FragmentProcessor;
%End
	public:
	bool start();
	bool finish();
	virtual Processor::Result operator () (Fragment& /atom/);
};

