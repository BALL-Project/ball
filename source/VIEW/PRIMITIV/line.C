// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: line.C,v 1.11 2002/12/16 12:23:14 sturm Exp $

#include <BALL/VIEW/PRIMITIV/line.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Line::Line()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Vertex2()
		{
		}

		Line::Line(const Line& line, bool deep)
			throw()
			:	GeometricObject(line, deep),
				ColorExtension(line),
				Vertex2(line)
		{
		}

		Line::Line(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex2()
		{
		}

		Line::~Line()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Line>() << endl;
			#endif 

			destroy();
		}

		void Line::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex2::clear();
		}

		void Line::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void Line::set(const Line& line, bool deep)
			throw()
		{
			GeometricObject::set(line, deep);
			ColorExtension::set(line);
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
			ColorExtension::swap(line);
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
			ColorExtension::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Line::extract()
			throw()
		{
			return true;  
		}

	} // namespace VIEW

} // namespace BALL
