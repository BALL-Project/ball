// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: line.C,v 1.1 2003/08/26 12:38:43 oliver Exp $

#include <BALL/VIEW/PRIMITIV/line.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Line::Line()
			throw()
			:	GeometricObject(),
				Vertex2()
		{
		}

		Line::Line(const Line& line, bool deep)
			throw()
			:	GeometricObject(line, deep),
				Vertex2(line)
		{
		}

		Line::~Line()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<Line>() << std::endl;
			#endif 
		}

		void Line::clear()
			throw()
		{
			GeometricObject::clear();
			Vertex2::clear();
		}

		void Line::set(const Line& line, bool deep)
			throw()
		{
			GeometricObject::set(line, deep);
			Vertex2::set(line);
		}

		const Line& Line::operator = (const Line& line)
			throw()
		{
			set(line);
			return *this;
		}

		void Line::get(Line& line, bool deep) const
			throw()
		{
			line.set(*this, deep);
		}

		void Line::swap(Line& line)
			throw()
		{
			GeometricObject::swap(line);
			Vertex2::swap(line);
		}

		bool Line::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
										  Vertex2::isValid());
		}

		void Line::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
