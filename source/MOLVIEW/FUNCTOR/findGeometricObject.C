// $Id: findGeometricObject.C,v 1.3 1999/12/28 18:00:46 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{


		FindGeometricObject::FindGeometricObject()
			:	UnaryProcessor<Composite>(),
				ExtendedPropertyManager(),
				geometric_object_(0)
		{
		}

		FindGeometricObject::FindGeometricObject
			(const FindGeometricObject &find_geometric_object,
			 bool deep)
				:
				UnaryProcessor<Composite>(find_geometric_object, deep),
				ExtendedPropertyManager(find_geometric_object, deep),
				geometric_object_(find_geometric_object.geometric_object_)
		{
		}

		FindGeometricObject::~FindGeometricObject
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<FindGeometricObject>::getName() << endl;
			#endif 

			destroy();
		}

		void 
		FindGeometricObject::clear
			()
		{
			ExtendedPropertyManager::clear();
			geometric_object_ = 0;
		}

		void 
		FindGeometricObject::destroy
			()
		{
			clear();
		}

		void 
		FindGeometricObject::set
			(const FindGeometricObject &find_geometric_object,
			 bool deep)
		{
			clear();

			ExtendedPropertyManager::set(find_geometric_object, deep);
		}

		FindGeometricObject &
		FindGeometricObject::operator =
			(const FindGeometricObject &find_geometric_object)
		{
			set(find_geometric_object);

			return *this;
		}

		void 
		FindGeometricObject::get
			(FindGeometricObject &find_geometric_object,
			 bool deep) const
		{
			find_geometric_object.set(*this, deep);
		}

		void 
		FindGeometricObject::swap
			(FindGeometricObject &find_geometric_object)
		{
			ExtendedPropertyManager::swap(find_geometric_object);
		}

		bool 
		FindGeometricObject::start
			()
		{
			Log.info() << "starting FindGeometricObject::start()" << endl;
			geometric_object_ = 0;

			return true;
		}
				
		bool 
		FindGeometricObject::finish
			()
		{
			Log.info() << "starting FindGeometricObject::finish()" << endl;
			return true;
		}
				
		Processor::Result 
		FindGeometricObject::operator ()
			(Composite &composite)
		{
			Log.info() << "FindGeometricObject::operator(" << (void*)&composite << ")" << endl;
			// skip composites that are not instances of geometricObject
			if (RTTI<VIEW::GeometricObject>::isKindOf(composite) == false)
			{
				return Processor::CONTINUE;
			}

			Log.info() << "FindGeometricObject::operator(" << (void*)&composite << "): isKindOf(Composite)" << endl;
			VIEW::GeometricObject *__pGeometricObject 
				= RTTI<VIEW::GeometricObject>::castTo(composite);

			BitVector and_bitvector = getBitVector().operator&(__pGeometricObject->getBitVector());
			BitVector help_bitvector(and_bitvector.getSize());

			help_bitvector.bitwiseOr(getBitVector());
			
			Log.info() << "own: " << help_bitvector << " object:  " << __pGeometricObject->getBitVector()
					 << "  &: " << and_bitvector << " size own: " << help_bitvector.getSize()
					 << "  temp size: " << and_bitvector.getSize() << endl;
			
			if (and_bitvector == help_bitvector)
			{
				geometric_object_ = __pGeometricObject;

				return Processor::BREAK;
			}
			
			return Processor::CONTINUE;
		}

		bool 
		FindGeometricObject::isValid
			() const
		{
			return true;
		}

		void 
		FindGeometricObject::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "searching for: " << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "static object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__OBJECT_STATIC) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "dynamic object: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__OBJECT_DYNAMIC) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "transparent object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__OBJECT_TRANSPARENT) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "opaque object: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__OBJECT_OPAQUE) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "visible object: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__OBJECT_VISIBLE) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "hidden object: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__OBJECT_HIDDEN) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode dots: " 
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_DOTS) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode wireframe: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing mode solid: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision low: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_LOW) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision medium: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM) 
					 << endl;
			
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision high: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision ultra: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA) 
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "drawing precision user defined: "
					 << BALL_MOLVIEW_PRINT_PROPERTY
											(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "object: --------------------------------------" << endl;

			if (geometric_object_ != 0)
			{
				geometric_object_->dump(s, depth + 1);
			}
			else
			{
				BALL_DUMP_DEPTH(s, depth);
				s << "no object found" << endl;;
			}

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/findGeometricObject.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
