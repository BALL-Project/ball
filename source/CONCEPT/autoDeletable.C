// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: autoDeletable.C,v 1.10 2004/11/07 08:25:35 oliver Exp $
//

#include <BALL/CONCEPT/autoDeletable.h>

namespace BALL 
{	
	void* AutoDeletable::last_ptr_ = 0;

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL
