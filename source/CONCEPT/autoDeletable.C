// $Id: autoDeletable.C,v 1.7 2000/12/09 20:54:12 amoll Exp $

#include <BALL/CONCEPT/autoDeletable.h>
#include <new>

namespace BALL 
{	
	void* AutoDeletable::last_ptr_ = 0;

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL
