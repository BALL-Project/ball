// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
