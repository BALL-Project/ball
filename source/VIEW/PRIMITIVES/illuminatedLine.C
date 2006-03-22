// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: illuminatedLine.C,v 1.1.2.1 2006/03/22 16:06:40 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		IlluminatedLine::IlluminatedLine()
			throw()
			:	GeometricObject()
		{
		}

		IlluminatedLine::IlluminatedLine(const IlluminatedLine& line)
			throw()
			:	GeometricObject(line)
		{
		}

		IlluminatedLine::~IlluminatedLine()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<IlluminatedLine>() << std::endl;
			#endif 
		}

		void IlluminatedLine::clear()
			throw()
		{
			GeometricObject::clear();
		}

		void IlluminatedLine::set(const IlluminatedLine& line)
			throw()
		{
			GeometricObject::set(line);
		}

		const IlluminatedLine& IlluminatedLine::operator = (const IlluminatedLine& line)
			throw()
		{
			set(line);
			return *this;
		}

		bool IlluminatedLine::isValid() const
			throw()
		{
			return (GeometricObject::isValid());
		}

		void IlluminatedLine::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
