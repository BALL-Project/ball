// $Id: autoDeletable.C,v 1.8 2000/12/10 19:46:05 oliver Exp $

#include <BALL/CONCEPT/autoDeletable.h>

namespace BALL 
{	
	void* AutoDeletable::last_ptr_ = 0;

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL
