// $Id: findGeometricObject.C,v 1.11 2001/05/13 15:02:39 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{


		FindGeometricObjects::FindGeometricObjects()
			throw()
			:	UnaryProcessor<Composite>(),
				ExtendedPropertyManager(),
				geometric_objects_()
		{
		}

		FindGeometricObjects::FindGeometricObjects
			(const FindGeometricObjects &find_geometric_objects, bool deep)
			throw()
			:	UnaryProcessor<Composite>(find_geometric_objects),
				ExtendedPropertyManager(find_geometric_objects, deep),
				geometric_objects_(find_geometric_objects.geometric_objects_)
		{
		}

		FindGeometricObjects::~FindGeometricObjects()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<FindGeometricObjects>() << endl;
			#endif 

			destroy();
		}

		void FindGeometricObjects::clear()
			throw()
		{
			ExtendedPropertyManager::clear();
			geometric_objects_.clear();
		}

		void FindGeometricObjects::destroy()
			throw()
		{
		}

		void FindGeometricObjects::set
			(const FindGeometricObjects &find_geometric_objects, bool deep)
			throw()
		{
			clear();

			ExtendedPropertyManager::set(find_geometric_objects, deep);
		}

		const FindGeometricObjects& FindGeometricObjects::operator =
			(const FindGeometricObjects &find_geometric_objects)
			throw()
		{
			set(find_geometric_objects);

			return *this;
		}

		void FindGeometricObjects::get
			(FindGeometricObjects& find_geometric_objects, bool deep) const
			throw()
		{
			find_geometric_objects.set(*this, deep);
		}

		void FindGeometricObjects::swap(FindGeometricObjects& find_geometric_objects)
			throw()
		{
			ExtendedPropertyManager::swap(find_geometric_objects);
		}

		bool FindGeometricObjects::start()
			throw()
		{
			geometric_objects_.clear();

			return true;
		}
				
		bool FindGeometricObjects::finish()
			throw()
		{
			return true;
		}
				
		Processor::Result FindGeometricObjects::operator () (Composite &composite)
			throw()
		{
			// skip composites that are not instances of geometricObject
			if (RTTI::isKindOf<GeometricObject>(composite) == false)
			{
				return Processor::CONTINUE;
			}

			GeometricObject* pGeometricObject = RTTI::castTo<GeometricObject>(composite);

			BitVector and_bitvector = getBitVector().operator&(pGeometricObject->getBitVector());
			BitVector help_bitvector(and_bitvector.getSize());

			help_bitvector.bitwiseOr(getBitVector());

			if (and_bitvector == help_bitvector)
			{
				geometric_objects_.push_back(pGeometricObject);
			}
			
			return Processor::CONTINUE;
		}

		bool FindGeometricObjects::isValid() const
			throw()
		{
			return ExtendedPropertyManager::isValid();
		}

		void FindGeometricObjects::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "searching for: " << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "static object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_STATIC) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "dynamic object: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_DYNAMIC) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "transparent object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_TRANSPARENT) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "opaque object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_OPAQUE) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "visible object: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_VISIBLE) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "hidden object: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__OBJECT_HIDDEN) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode dots: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_MODE_DOTS) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode wireframe: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode solid: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision low: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision medium: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision high: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision ultra: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "number of objects: " << geometric_objects_.size() << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
