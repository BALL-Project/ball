// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: processor.sip,v 1.2 2003/08/26 09:18:19 oliver Exp $
//

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
