// $Id: objectProcessor.C,v 1.1 2000/09/23 13:28:32 hekl Exp $

#include <BALL/VIEW/GUI/FUNCTOR/objectProcessor.h>

using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
  
	  ObjectProcessor::ObjectProcessor()
			:
			view_center_vector_(0,0,0),
			view_direction_(2),
			view_distance_(25),
			address_array_((int)RANGE__ADDRESS, (int)VALUE__INVALID)
		{
		}

		ObjectProcessor::ObjectProcessor(const ObjectProcessor& o, bool /* deep */)
			:
			view_center_vector_(o.view_center_vector_),
			view_direction_(o.view_direction_),
			view_distance_(o.view_distance_),
			address_array_(o.address_array_)
		{
		}

		ObjectProcessor::~ObjectProcessor()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ObjectProcessor>() << endl;
			#endif 

			destroy();
		}

		void ObjectProcessor::clear()
		{
			view_center_vector_.set(0,0,0);
			view_direction_ = 2;
			view_distance_ = 25;
		}

		void ObjectProcessor::destroy()
		{
			clear();
		}

		void ObjectProcessor::setValue(int address, int value)
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
		
		int ObjectProcessor::getValue(int address)
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

		bool ObjectProcessor::hasValue(int address, int value)
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

   	void ObjectProcessor::applyOn(Composite & /* c */)
    {	
    }

   	void ObjectProcessor::calculateCenter(Composite & /* c */)
    {	
    }

		bool ObjectProcessor::isValid() const
		{
			return true;
		}

		void ObjectProcessor::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ObjectProcessor::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ObjectProcessor::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/FUNCTOR/objectProcessor.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
