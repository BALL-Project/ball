// $Id: objectCreator.C,v 1.3 2001/05/13 14:28:36 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/objectCreator.h>

using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
  
		ObjectCreator::ObjectCreator()
				throw()
			:
			init_(false),
			pm_()
		{
		}

		ObjectCreator::~ObjectCreator()
				throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ObjectCreator>() << endl;
			#endif 

			destroy();
		}

		void ObjectCreator::clear()
				throw()
		{
		}

		void ObjectCreator::destroy()
				throw()
		{
		}

 	  void ObjectCreator::initPersistenceManager(TextPersistenceManager & /* pm */)
				throw()
    {
    }

	  Composite *ObjectCreator::convertObject(PersistentObject & /* po */)
				throw()
    {
			return (Composite *)0;
    }

	  Composite *ObjectCreator::operator() (IOStreamSocket &iostream_socket)
				throw()
    {
			// initialize the PersistenceManager only one times
			if (init_ == false)
			{
				init_ = true;

				initPersistenceManager(pm_);
			}

			// read persistent object from stream
			pm_.setIstream(iostream_socket);
			PersistentObject*	po = pm_.readObject();
			
			// convert the object
			return convertObject(*po);
    }

		void ObjectCreator::read(istream & /* s */)
				throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ObjectCreator::write(ostream & /* s */) const
				throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/objectCreator.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
