// $Id: pyFragmentProcessor.sip,v 1.1 2000/07/18 17:02:46 oliver Exp $

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

