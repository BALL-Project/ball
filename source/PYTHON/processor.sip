// $Id: processor.sip,v 1.1 2000/07/03 11:11:07 oliver Exp $

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
