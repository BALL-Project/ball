// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: point.C,v 1.3 2003/08/29 10:53:25 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/point.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Point::Point()
			throw()
			:	GeometricObject(),
				Vertex()
		{
		}

		Point::Point(const Point& point)
			throw()
			:	GeometricObject(point),
				Vertex(point)
		{
		}

		Point::~Point()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info () << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Point>() << std::endl;
			#endif 
		}

		void Point::clear()
			throw()
		{
			GeometricObject::clear();
			Vertex::clear();
		}

		void Point::set(const Point& point)
			throw()
		{
			GeometricObject::set(point);
			Vertex::set(point);
		}

		const Point& Point::operator =(const Point& point)
			throw()
		{
			set(point);
			return *this;
		}

		void Point::get(Point& point) const
			throw()
		{
			point.set(*this);
		}

		void Point::swap(Point& point)
			throw()
		{
			GeometricObject::swap(point);
			Vertex::swap(point);
		}

		bool Point::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
											 Vertex::isValid() );
		}

		void Point::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
