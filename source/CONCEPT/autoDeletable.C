// $Id: autoDeletable.C,v 1.2 1999/12/04 18:34:26 oliver Exp $

#include <BALL/CONCEPT/autoDeletable.h>

namespace BALL {


	AutoDeletable::AutoDeletable()
	{
		if ((void*)this == last_ptr_)
		{
			// if it has been created using it's own
			// new operator, it should be on the heap
			// and can be deleted automatically
			enabled_ = true;
		} else {
			// otherwise it is on the stack or part of an
			// array - we can't delete it automatically
			enabled_ = false;
		}
	}

	AutoDeletable::AutoDeletable(const AutoDeletable&, bool /* deep */)
	{
		// autodeletable object cannot be copied by default
		// it doesn't make sense!
		// To prevent any misuse, we behave as in the default constructor
		if ((void*)this == last_ptr_)
		{
			// if it has been created using it's own
			// new operator, it should be on the heap
			// and can be deleted automatically
			enabled_ = true;
		} else {
			// otherwise it is on the stack or part of an
			// array - we can't delete it automatically
			enabled_ = false;
		}
	}

	AutoDeletable::~AutoDeletable()
	{
	}

	void* AutoDeletable::operator new (size_t size)
	{
		last_ptr_ = ::operator new(size);
		return last_ptr_;
	}
	
	void AutoDeletable::operator delete (void* ptr)
	{
		::operator delete(ptr);
	}
	
	void* AutoDeletable::last_ptr_ = 0;

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/autoDeletable.iC>
#	endif

} // namespace BALL
