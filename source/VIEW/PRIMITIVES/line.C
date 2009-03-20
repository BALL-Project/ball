// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: line.C,v 1.4.28.1 2007-03-25 22:02:34 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/line.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Line::Line()
			:	GeometricObject(),
				Vertex2()
		{
		}

		Line::Line(const Line& line)
			:	GeometricObject(line),
				Vertex2(line)
		{
		}

		Line::~Line()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<Line>() << std::endl;
			#endif 
		}

		void Line::clear()
		{
			GeometricObject::clear();
			Vertex2::clear();
		}

		void Line::set(const Line& line)
		{
			GeometricObject::set(line);
			Vertex2::set(line);
		}

		const Line& Line::operator = (const Line& line)
		{
			set(line);
			return *this;
		}

		void Line::swap(Line& line)
		{
			GeometricObject::swap(line);
			Vertex2::swap(line);
		}

		bool Line::isValid() const
		{
			return (GeometricObject::isValid() &&
										  Vertex2::isValid());
		}

		void Line::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Line::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(getVertex1());
			vertices.push_back(getVertex2());
		}

	} // namespace VIEW
} // namespace BALL
