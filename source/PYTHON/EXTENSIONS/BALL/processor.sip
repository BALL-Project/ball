class Processor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
%End
	public:

	enum Result
	{
		ABORT    = 0,
		BREAK    = 1,
	 	CONTINUE = 2
	};
};
