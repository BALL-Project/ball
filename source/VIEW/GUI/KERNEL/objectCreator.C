// $Id: objectCreator.C,v 1.1 2000/09/23 13:28:35 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/objectCreator.h>

using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
  
		ObjectCreator::ObjectCreator()
			:
			init_(false),
			pm_(),
			address_array_((int)RANGE__ADDRESS, -1)
		{
		}

		ObjectCreator::ObjectCreator(const ObjectCreator &objectCreator, bool /* deep */)
			:
			init_(false),
			pm_(objectCreator.pm_),
			address_array_(objectCreator.address_array_)
		{
		}

		ObjectCreator::~ObjectCreator()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ObjectCreator>() << endl;
			#endif 

			destroy();
		}

		void ObjectCreator::clear()
		{
		}

		void ObjectCreator::destroy()
		{
			clear();
		}

		void ObjectCreator::setValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (address < 0 
					|| address > RANGE__ADDRESS)
			{
				throw InvalidRange(__FILE__, __LINE__);
			}
				
			#endif

			address_array_[address] = value;
		}
		
		int ObjectCreator::getValue(int address)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (address < 0 
					|| address > RANGE__ADDRESS)
			{
				throw InvalidRange(__FILE__, __LINE__);
			}
				
			#endif

			return address_array_[address];
		}

		bool ObjectCreator::hasValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (address < 0 
					|| address > RANGE__ADDRESS)
			{
				throw InvalidRange(__FILE__, __LINE__);
			}
				
			#endif

			return (bool)(address_array_[address] == value);
		}

 	  void ObjectCreator::initPersistenceManager(TextPersistenceManager & /* pm */)
    {
    }

	  Composite *ObjectCreator::convertObject(PersistentObject & /* po */)
    {
			return (Composite *)0;
    }

	  Composite *ObjectCreator::operator() (IOStreamSocket &iostream_socket)
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

		bool ObjectCreator::isValid() const
		{
			return true;
		}

		void ObjectCreator::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ObjectCreator::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ObjectCreator::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/objectCreator.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
