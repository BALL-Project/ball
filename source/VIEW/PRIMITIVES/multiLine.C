// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: multiLine.C,v 1.1.4.1 2007-03-25 22:02:34 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/multiLine.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		MultiLine::MultiLine()
			:	GeometricObject(),
				MultiColorExtension()
		{
		}

		MultiLine::MultiLine(const MultiLine& line)
			:	GeometricObject(line),
				MultiColorExtension(line)
		{
		}

		MultiLine::~MultiLine()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<MultiLine>() << std::endl;
			#endif 
		}

		void MultiLine::clear()
		{
			GeometricObject::clear();
			colors.clear();
		}

		void MultiLine::set(const MultiLine& line)
		{
			GeometricObject::set(line);
		}

		const MultiLine& MultiLine::operator = (const MultiLine& line)
		{
			set(line);
			return *this;
		}

		bool MultiLine::isValid() const
		{
			return (GeometricObject::isValid());
		}

		void MultiLine::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			MultiColorExtension::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void MultiLine::getVertices(vector<Vector3>& vert) const
		{
			vert.reserve(vertices.size() + vert.size());
			for (Position p = 0; p < vertices.size(); p++)
			{
				vert.push_back(vertices[p]);
			}
		}

	} // namespace VIEW
} // namespace BALL
