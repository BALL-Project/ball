// $Id: geometricCollector.C,v 1.1 2000/06/04 17:52:50 hekl Exp $

#include <BALL/VIEW/FUNCTOR/geometricCollector.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GeometricCollector::GeometricCollector()
			:	UnaryProcessor<Composite>(),
				only_selected_objects_(false),
				collection_()
		{
		}

		GeometricCollector::GeometricCollector
		  (const GeometricCollector& collector, bool /* deep */)
			:	UnaryProcessor<Composite>(collector),
				only_selected_objects_(collector.only_selected_objects_),
				collection_()
		{
		}

		GeometricCollector::~GeometricCollector()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GeometricCollector>() << endl;
			#endif 

			destroy();
		}

		void GeometricCollector::clear()
		{
			collection_.clear();
		}

		void GeometricCollector::destroy()
		{
			only_selected_objects_ = false;
			collection_.clear(); 
		}

		void GeometricCollector::set
			(const GeometricCollector&  /* collector */, bool /* deep */)
		{
			clear();
		}

		GeometricCollector& GeometricCollector::operator =
			(const GeometricCollector& collector)
		{
			set(collector);

			return *this;
		}

		void GeometricCollector::get
			(GeometricCollector& collector, bool deep) const
		{
			collector.set(*this, deep);
		}

		void GeometricCollector::swap(GeometricCollector&  /* collector */)
		{
		}

		bool GeometricCollector::start()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "start collect process ..." << endl;
			#endif

				//			clear();

			return true;
		}
				
		bool GeometricCollector::finish()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "finished collect process ..." << endl;
			#endif

			return true;
		}
				
		Processor::Result GeometricCollector::operator()
			(Composite& composite)
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

		bool GeometricCollector::isValid() const
		{
			return true;
		}

		void GeometricCollector::dump
			(ostream& s, Size depth) const
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

		void GeometricCollector::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GeometricCollector::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/FUNCTOR/geometricCollector.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
