// $Id: object.C,v 1.9 2001/07/05 17:49:50 oliver Exp $

#include <BALL/CONCEPT/object.h>


using std::endl;
using std::cout;
using std::istream;
using std::ostream;

namespace BALL 
{

	Handle Object::global_handle_ = (Handle)0;

	Object::Object()
		throw()
		:	AutoDeletable(),
			handle_(Object::global_handle_++)
	{
	}

	Object::Object(const Object& /* object */, bool)
		throw()
		:	AutoDeletable(),
			handle_(Object::global_handle_++)
	{
	}

	Object::~Object()
		throw()
	{
	}

	void Object::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_HEADER(s, this, this)

		BALL_DUMP_DEPTH(s, depth);
		s << "  handle: " << handle_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  auto-deletable: " << AutoDeletable::isAutoDeletable() << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	int Object::compare(const Object & object) const
		throw()
	{
		if (*this == object)
		{
			return 0;
		} 
		if (*this < object)
		{
			return -1;
		} 
		else 
		{
			return 1;
		}
	}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/object.iC>
#	endif

} // namespace BALL


