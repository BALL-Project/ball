// $Id: geometricCollector.C,v 1.2 2001/05/13 14:28:35 hekl Exp $

#include <BALL/VIEW/FUNCTOR/geometricCollector.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GeometricCollector::GeometricCollector()
			throw()
			:	UnaryProcessor<Composite>(),
				only_selected_objects_(false),
				collection_()
		{
			clear();
		}

		GeometricCollector::~GeometricCollector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GeometricCollector>() << endl;
			#endif 

			destroy();
		}

		void GeometricCollector::clear()
			throw()
		{
			collection_.clear();
			only_selected_objects_ = false;
		}

		void GeometricCollector::destroy()
			throw()
		{
		}

		bool GeometricCollector::start()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "start collect process ..." << endl;
			#endif

				//			clear();

			return true;
		}
				
		bool GeometricCollector::finish()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "finished collect process ..." << endl;
			#endif

			return true;
		}
				
		Processor::Result GeometricCollector::operator()
			(Composite& composite)
			throw()
		{
			if (RTTI::isKindOf<GeometricObject>(composite) == false)
			{
				return Processor::CONTINUE;
			}

			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "collecting ..." << endl;
			#endif

			GeometricObject *geometric_object = RTTI::castTo<GeometricObject>(composite);

			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				geometric_object->dump(cout);
			#endif

			if (only_selected_objects_)
			{
				if (geometric_object->isSelected())
				{
					collection_.push_back(geometric_object);
				}
			}
			else
			{
				collection_.push_back(geometric_object);
			}

			return Processor::CONTINUE;
		}

		void GeometricCollector::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "number of composites in collection: " << collection_.size() << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "collect only selected geometric objects: " 
				<< ((only_selected_objects_ == true) ? "yes" : "no") << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/FUNCTOR/geometricCollector.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
