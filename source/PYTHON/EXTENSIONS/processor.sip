// $Id: processor.sip,v 1.1 2001/09/03 16:53:11 oliver Exp $

class Processor
{
%HeaderCode	
	#include <BALL/CONCEPT/processor.h>
	using namespace BALL;
%End
	public:
	enum Result
	{
		ABORT    = 0,
		BREAK    = 1,
	 	CONTINUE = 2
	};
};
