// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.C,v 1.19 2003/08/26 09:18:48 oliver Exp $

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
			: name_("unknown"),
				color_(),
				composite_(0)
		{
		}

		GeometricObject::GeometricObject(const GeometricObject& geometric_object, bool deep)
			throw()
			:	name_(geometric_object.name_),
				color_(geometric_object.color_),
				composite_(geometric_object.composite_)
		{
		}

		GeometricObject::~GeometricObject()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class " << RTTI::getName<GeometricObject>() << endl;
			#endif 
		}

		void GeometricObject::clear()
			throw()
		{
			name_ = "unknown";
			color_ = ColorRGBA();
			composite_ = 0;
		}

		void GeometricObject::set(const GeometricObject& geometric_object, bool deep)
			throw()
		{
			name_ = geometric_object.name_;
			color_ = geometric_object.color_;
			composite_ = geometric_object.composite_;
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
			name_.swap(geometric_object.name_);

			const Composite* composite = geometric_object.composite_;
			geometric_object.composite_ = composite_;
			composite_ = composite;
		}

		void GeometricObject::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "name           : " << name_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "type name      : " << getTypeName() << endl;

			BALL_DUMP_DEPTH(s,depth);
			s << "composite 		 : " << composite_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
				
	} // namespace VIEW
} // namespace BALL
