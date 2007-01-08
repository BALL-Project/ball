// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: processor.sip,v 1.3 2003/10/29 22:54:42 oliver Exp $
//

class Processor
{
%TypeHeaderCode	
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
