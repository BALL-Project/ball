// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: autoDeletable.C,v 1.8.2.1 2003/01/07 13:20:30 anker Exp $

#include <BALL/CONCEPT/autoDeletable.h>

namespace BALL 
{	
	void* AutoDeletable::last_ptr_ = 0;

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL
