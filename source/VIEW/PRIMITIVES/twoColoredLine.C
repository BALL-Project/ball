// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: twoColoredLine.C,v 1.2 2003/08/26 15:32:49 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
		
		TwoColoredLine::TwoColoredLine()
			throw()
			:	GeometricObject(),
				ColorExtension2(),
				Vertex2()
		{
		}

		TwoColoredLine::TwoColoredLine(const TwoColoredLine& two_colored_line, bool deep)
			throw()
			:	GeometricObject(two_colored_line, deep),
				ColorExtension2(two_colored_line),
				Vertex2(two_colored_line)
		{
		}

		TwoColoredLine::~TwoColoredLine()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<TwoColoredLine>() << std::endl;
			#endif 
		}

		void TwoColoredLine::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension2::clear();
			Vertex2::clear();
		}

		void TwoColoredLine::set(const TwoColoredLine& two_colored_line, bool deep)
			throw()
		{
			GeometricObject::set(two_colored_line, deep);
			ColorExtension2::set(two_colored_line);
			Vertex2::set(two_colored_line);
		}

		const TwoColoredLine& TwoColoredLine::operator = (const TwoColoredLine &two_colored_line)
			throw()
		{
			set(two_colored_line);
			return *this;
		}

		void TwoColoredLine::get(TwoColoredLine& two_colored_line, bool deep) const
			throw()
		{
			two_colored_line.set(*this, deep);
		}

		void TwoColoredLine::swap(TwoColoredLine& two_colored_line)
			throw()
		{
			GeometricObject::swap(two_colored_line);
			ColorExtension2::swap(two_colored_line);
			Vertex2::swap(two_colored_line);
		}

		bool TwoColoredLine::isValid() const
			throw()
		{
			return (GeometricObject::isValid());
		}

		void TwoColoredLine::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension2::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIVES/twoColoredLine.iC>
#		endif

	} // namespace VIEW
} // namespace BALL
