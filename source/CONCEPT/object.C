// $Id: object.C,v 1.8 2001/01/22 20:50:32 amoll Exp $

#include <BALL/CONCEPT/object.h>


using std::endl;
using std::cout;
using std::istream;
using std::ostream;

namespace BALL 
{

	Handle Object::global_handle_ = (Handle)0;

	// the following four methods
	// may not be inlined - they are called
	// in object.h

	Object* Object::getPrevious()
		throw()
	{
		return previous_;
	}
			
	const Object* Object::getPrevious() const
		throw()
	{
		return previous_;
	}
			
	Object* Object::getNext()
		throw()
	{
		return next_;
	}
			
	const Object* Object::getNext() const
		throw()
	{
		return next_;
	}

	Object::Object()
		throw()
		:	AutoDeletable(),
			handle_(Object::global_handle_++)
	{
	}

	Object::Object(const Object&, bool)
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

	istream& operator >> (istream &s, Object &object)
		throw()
	{
		object.read(s);
		return s;
	}

	ostream& operator << (ostream &s, const Object &object)
		throw()
	{
		object.write(s);
		return s;
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


