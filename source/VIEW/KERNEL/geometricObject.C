// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.C,v 1.12.2.1 2003/01/07 13:23:35 anker Exp $

#include <BALL/VIEW/KERNEL/geometricObject.h>

using std::endl;
using std::ostream;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

		GeometricObject::GeometricObject()
			throw()
			:	Composite(),
				PropertyManager(),
				name_("unknown")
		{
			clear_();
		}

		GeometricObject::GeometricObject(const GeometricObject& geometric_object, bool deep)
			throw()
			:	Composite(geometric_object, deep),
				PropertyManager(geometric_object),
				name_(geometric_object.name_)
		{
		}

		GeometricObject::~GeometricObject()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class " << RTTI::getName<GeometricObject>() << endl;
			#endif 

			destroy();
		}

		void GeometricObject::clear()
			throw()
		{
			Composite::clear();
			PropertyManager::clear();
			name_ = "unknown";
			clear_();
		}

		void GeometricObject::destroy()
			throw()
		{
			Composite::destroy();
			PropertyManager::destroy();
		}

		void GeometricObject::set(const GeometricObject& geometric_object, bool deep)
			throw()
		{
			Composite::set(geometric_object, deep);
			PropertyManager::set(geometric_object);
			
			name_ = geometric_object.name_;
		}

		GeometricObject& GeometricObject::operator = (const GeometricObject& geometric_object)
			throw()
		{
			set(geometric_object);
			return *this;
		}

		void GeometricObject::get(GeometricObject& geometric_object, bool deep) const
			throw()
		{
			geometric_object.set(*this, deep);
		}

		void GeometricObject::swap(GeometricObject& geometric_object)
			throw()
		{
			Composite::swap(geometric_object);
			PropertyManager::swap(geometric_object);

			name_.swap(geometric_object.name_);
		}

		void GeometricObject::setProperty(Property property)
			throw()
		{
			if (property == GeometricObject::PROPERTY__OBJECT_TRANSPARENT || 
					property == GeometricObject::PROPERTY__OBJECT_OPAQUE)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT);
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPAQUE);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_VISIBLE || 
							 property == GeometricObject::PROPERTY__OBJECT_HIDDEN)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_VISIBLE);
				clearProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_OPENED || 
							 property == GeometricObject::PROPERTY__OBJECT_CLOSED)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPENED);
				clearProperty(GeometricObject::PROPERTY__OBJECT_CLOSED);
			}
			else if (property >= GeometricObject::PROPERTY__DRAWING_PRECISION_LOW && 
							 property <= GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
			}
			else if (property >= GeometricObject::PROPERTY__DRAWING_MODE_DOTS && 
							 property <= GeometricObject::PROPERTY__DRAWING_MODE_SOLID)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_STATIC || 
							 property == GeometricObject::PROPERTY__OBJECT_DYNAMIC)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
				clearProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			}

			PropertyManager::setProperty(property);
		}

		String GeometricObject::getTypeName() const
			throw()
    {
			return String("GeometricObject");
    }

		bool GeometricObject::isValid() const
			throw()
		{
			return (Composite::isValid() && PropertyManager::isValid());
		}

		void GeometricObject::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			Composite::dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "name           : " << name_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "type name      : " << getTypeName() << endl;

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
			s << "    always front object: " 
					 << BALL_VIEW_PRINT_PROPERTY(GeometricObject::PROPERTY__OBJECT_ALWAYS_FRONT) 
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
			s << "          selected: " 
					 << (isSelected() ? "yes" : "no") << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
				
		bool GeometricObject::extract()
			throw()
		{
			return true;
		}

		void GeometricObject::getDrawingModeAndPrecision(unsigned int& mode, unsigned int& precision) const
			throw()
		{
			if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW))
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_LOW;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM))
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH))
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH;
			} 
			else // default
			{
				precision = (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH;
			}

			if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_DOTS))
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_DOTS;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME))
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME;
			} 
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID))
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_SOLID;
			}
			else // default
			{
				mode = (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_SOLID;
			}

			// removed precision ultra, shifted all precisions 1 up
			precision = precision - (unsigned int)GeometricObject::PROPERTY__DRAWING_PRECISION_LOW + 1;
			mode -= (unsigned int)GeometricObject::PROPERTY__DRAWING_MODE_DOTS;
		}

		void GeometricObject::clear_()
			throw()
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
