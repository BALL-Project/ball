// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: twoColoredLine.C,v 1.4.28.1 2007-03-25 22:02:36 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
		
		TwoColoredLine::TwoColoredLine()
			:	GeometricObject(),
				ColorExtension2(),
				Vertex2()
		{
		}

		TwoColoredLine::TwoColoredLine(const TwoColoredLine& two_colored_line)
			:	GeometricObject(two_colored_line),
				ColorExtension2(two_colored_line),
				Vertex2(two_colored_line)
		{
		}

		TwoColoredLine::~TwoColoredLine()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<TwoColoredLine>() << std::endl;
			#endif 
		}

		void TwoColoredLine::clear()
		{
			GeometricObject::clear();
			ColorExtension2::clear();
			Vertex2::clear();
		}

		void TwoColoredLine::set(const TwoColoredLine& two_colored_line)
		{
			GeometricObject::set(two_colored_line);
			ColorExtension2::set(two_colored_line);
			Vertex2::set(two_colored_line);
		}

		const TwoColoredLine& TwoColoredLine::operator = (const TwoColoredLine &two_colored_line)
		{
			set(two_colored_line);
			return *this;
		}

		void TwoColoredLine::swap(TwoColoredLine& two_colored_line)
		{
			GeometricObject::swap(two_colored_line);
			ColorExtension2::swap(two_colored_line);
			Vertex2::swap(two_colored_line);
		}

		bool TwoColoredLine::isValid() const
		{
			return (GeometricObject::isValid());
		}

		void TwoColoredLine::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension2::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void TwoColoredLine::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(getVertex1());
			vertices.push_back(getVertex2());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIVES/twoColoredLine.iC>
#		endif

	} // namespace VIEW
} // namespace BALL
