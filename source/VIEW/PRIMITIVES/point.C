// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: point.C,v 1.4.28.1 2007-03-25 22:02:35 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/point.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Point::Point()
			:	GeometricObject(),
				Vertex()
		{
		}

		Point::Point(const Point& point)
			:	GeometricObject(point),
				Vertex(point)
		{
		}

		Point::~Point()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info () << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Point>() << std::endl;
			#endif 
		}

		void Point::clear()
		{
			GeometricObject::clear();
			Vertex::clear();
		}

		void Point::set(const Point& point)
		{
			GeometricObject::set(point);
			Vertex::set(point);
		}

		const Point& Point::operator =(const Point& point)
		{
			set(point);
			return *this;
		}

		void Point::swap(Point& point)
		{
			GeometricObject::swap(point);
			Vertex::swap(point);
		}

		bool Point::isValid() const
		{
			return (GeometricObject::isValid() &&
											 Vertex::isValid() );
		}

		void Point::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Point::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(getVertex());
		}

	} // namespace VIEW
} // namespace BALL
