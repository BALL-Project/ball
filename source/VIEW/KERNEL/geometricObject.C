// $Id: geometricObject.C,v 1.4 1999/12/30 18:05:46 oliver Exp $

#include <BALL/VIEW/KERNEL/geometricObject.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GeometricObject::GeometricObject()
			:	Composite(),
				PropertyManager(),
				selected_color_(255, 255, 0, 255),
				name_("unkown")
		{
			clear_();
		}

		GeometricObject::GeometricObject
			(const GeometricObject& geometric_object, bool deep)
			:	Composite(geometric_object, deep),
				PropertyManager(geometric_object, deep),
				selected_color_(geometric_object.selected_color_),
				name_(geometric_object.name_)
		{
		}

		GeometricObject::~GeometricObject()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GeometricObject>() << endl;
			#endif 

			destroy();
		}

		void GeometricObject::clear()
		{
			Composite::clear();
			PropertyManager::clear();

			clear_();
		}

		void GeometricObject::destroy()
		{
			Composite::destroy();
			PropertyManager::destroy();

			clear_();
		}

		void GeometricObject::set
			(const GeometricObject& geometric_object, bool deep)
		{
			Composite::set(geometric_object, deep);
			PropertyManager::set(geometric_object, deep);
			
			selected_color_ = geometric_object.selected_color_;
			name_ = geometric_object.name_;
		}

		GeometricObject& GeometricObject::operator =
			(const GeometricObject& geometric_object)
		{
			set(geometric_object);

			return *this;
		}

		void GeometricObject::get
			(GeometricObject& geometric_object, bool deep) const
		{
			geometric_object.set(*this, deep);
		}

		void GeometricObject::swap(GeometricObject& geometric_object)
		{
			Composite::swap(geometric_object);
			PropertyManager::swap(geometric_object);

			selected_color_.swap(geometric_object.selected_color_);
			name_.swap(geometric_object.name_);
		}

		void GeometricObject::setProperty
			(Property property)
		{
			if (property == GeometricObject::PROPERTY__OBJECT_STATIC 
					|| property == GeometricObject::PROPERTY__OBJECT_DYNAMIC)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
				clearProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_TRANSPARENT 
							 || property == GeometricObject::PROPERTY__OBJECT_OPAQUE)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT);
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPAQUE);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_VISIBLE 
							 || property == GeometricObject::PROPERTY__OBJECT_HIDDEN)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_VISIBLE);
				clearProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_OPENED 
							 || property == GeometricObject::PROPERTY__OBJECT_CLOSED)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPENED);
				clearProperty(GeometricObject::PROPERTY__OBJECT_CLOSED);
			}
			else if (property >= GeometricObject::PROPERTY__DRAWING_PRECISION_LOW
							 && property <= GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED);
			}
			else if (property >= GeometricObject::PROPERTY__DRAWING_MODE_DOTS 
							 && property <= GeometricObject::PROPERTY__DRAWING_MODE_SOLID)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
			}

			PropertyManager::setProperty(property);
		}

		bool GeometricObject::isValid() const
		{
			return (bool)(Composite::isValid() == true
										&& PropertyManager::isValid() == true);
		}

		void GeometricObject::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			Composite::dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "name           : " << name_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "selected color : " << selected_color_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "property       : " << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "     static object: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__OBJECT_STATIC) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "    dynamic object: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__OBJECT_DYNAMIC) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "transparent object: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__OBJECT_TRANSPARENT) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "           visible: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__OBJECT_VISIBLE) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "            opened: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__OBJECT_OPENED) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "        dmode dots: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_MODE_DOTS) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "   dmode wireframe: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "       dmode solid: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "    dprecision low: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << " dprecision medium: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "   dprecision high: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  dprecision ultra: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "   dprecision user: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED) 
					 << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "          selected: " 
					 << (isSelected() ? "yes" : "no") << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
				
		void GeometricObject::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GeometricObject::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}
		/*
		bool GeometricObject::draw()
		{
			return true;
		}
		*/
		bool GeometricObject::extract()
		{
			return true;
		}

		void 
		GeometricObject::getDrawingModeAndPrecision
			(unsigned int& mode, unsigned int& precision) const
		{
			if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW) == true)
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_LOW;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM) == true)
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH) == true)
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA) == true)
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED) == true)
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED;
			}
			else // default
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH;
			}

			if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_DOTS) == true)
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_DOTS;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME) == true)
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) == true)
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_SOLID;
			}
			else // default
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_SOLID;
			}

			precision -= (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_LOW;
			mode -= (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_DOTS;
		}

		void GeometricObject::clear_()
		{
			setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			setProperty(GeometricObject::PROPERTY__OBJECT_OPAQUE);
			setProperty(GeometricObject::PROPERTY__OBJECT_VISIBLE);
			setProperty(GeometricObject::PROPERTY__OBJECT_CLOSED);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
			setProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);

			deselect();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/geometricObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
