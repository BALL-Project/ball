// $Id: object.C,v 1.7 2000/12/19 12:51:06 amoll Exp $

#include <BALL/CONCEPT/object.h>


using std::endl;
using std::cout;
using std::istream;
using std::ostream;

namespace BALL 
{

	Handle Object::global_handle_ = (Handle)0;

#	ifdef BALL_SUPPORT_OBJECT_MANAGER

	Object *ObjectManager::first_ = 0;
	Object *ObjectManager::last_ = 0;
	Size ObjectManager::number_of_insertions_ = 0;
	Size ObjectManager::number_of_removals_ = 0;
	Size ObjectManager::number_of_objects_ = 0;

#	endif // BALL_SUPPORT_OBJECT_MANAGER

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
#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		previous_ = next_= 0;
		ObjectManager::insert(*this);

#		endif // BALL_SUPPORT_OBJECT_MANAGER
	}

	Object::Object(const Object&, bool)
		throw()
		:	AutoDeletable(),
			handle_(Object::global_handle_++)
	{
#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		previous_ = next_ = 0;
		ObjectManager::insert(*this);

#		endif // BALL_SUPPORT_OBJECT_MANAGER
	}

	Object::~Object()
		throw()
	{
#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		ObjectManager::remove(*this);

#		endif // BALL_SUPPORT_OBJECT_MANAGER
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



#	ifdef BALL_SUPPORT_OBJECT_MANAGER

	void ObjectManager::insert(Object &object)
		throw()
	{
		if (first_ == 0)
		{
			first_ = last_ = &object;
			object.previous_ = object.next_ = 0;

		} 
		else 
		{
			object.previous_ = last_;
			object.next_ = 0;

			last_ = last_->next_ = &object;
		}

		++number_of_objects_;
		++number_of_insertions_;
	}
		
	void ObjectManager::remove(Object &object)
		throw()
	{
		if (first_ == &object)
		{
			if (last_ == &object)
			{
				first_ = last_ = 0;
			}
			else 
			{
				first_ = object.next_;
				first_->previous_ = 0;
			}
		} 
		else 
		{
			if (last_ == &object)
			{
				last_ = object.previous_;
				last_->next_ = 0;
			} 
			else 
			{
				object.previous_->next_ = object.next_;
				object.next_->previous_ = object.previous_;
			}
		}

		--number_of_objects_;
		++number_of_removals_;
	}

	bool ObjectManager::isValid()
		throw()
	{
		Size size = 0;
		ObjectIterator object_it;
		
		for (object_it = begin(); !object_it.isEnd(); ++object_it)
		{
			++size;
		}

		if (size != number_of_objects_)
		{
			return false;
		}

		for (object_it = rbegin(), size = 0; !object_it.isREnd(); --object_it)
		{
			++size;
		}

		return (size == number_of_objects_);
	}

	void ObjectManager::dump (ostream& s, Size depth)
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_CLASS_HEADER(s, "ObjectManager", 0);

		BALL_DUMP_DEPTH(s, depth);
		cout << "  updates: " << countUpdates() << endl;

		BALL_DUMP_DEPTH(s, depth);
		cout << "    insertions: " << countInsertions() << endl;

		BALL_DUMP_DEPTH(s, depth);
		cout << "    removals: " << countRemovals() << endl;

		BALL_DUMP_DEPTH(s, depth);
		cout << "  active objects: " << countObjects() << endl;

		for (ObjectIterator object_it = begin(); object_it.isValid(); ++object_it)
		{
			BALL_DUMP_DEPTH(s, depth);
			cout << "    "
					 << typeid(*object_it).name() << ' '
					 << (*object_it).getHandle()
					 << endl;
		}
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/object.iC>
#	endif

} // namespace BALL

#endif // BALL_SUPPORT_OBJECT_MANAGER

