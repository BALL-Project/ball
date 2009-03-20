// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: twoColoredTube.C,v 1.5.28.1 2007-03-25 22:02:36 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		TwoColoredTube::TwoColoredTube()
			:	GeometricObject(),			
			  ColorExtension2(),
	  		Vertex2(),
				radius_(1)
		{
		}

		TwoColoredTube::TwoColoredTube(const TwoColoredTube& two_colored_tube)
			:	GeometricObject(two_colored_tube),
				ColorExtension2(two_colored_tube),
				Vertex2(two_colored_tube),
				radius_(two_colored_tube.radius_)
		{
		}

		TwoColoredTube::~TwoColoredTube()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<TwoColoredTube>() << std::endl;
			#endif 
		}

		void TwoColoredTube::clear()
		{
			GeometricObject::clear();
			ColorExtension2::clear();
			Vertex2::clear();
			radius_ = 1;
		}

		void TwoColoredTube::set(const TwoColoredTube& two_colored_tube)
		{
			GeometricObject::set(two_colored_tube);
			ColorExtension2::set(two_colored_tube);
			Vertex2::set(two_colored_tube);
			radius_ = two_colored_tube.radius_;
		}

		const TwoColoredTube& TwoColoredTube::operator = (const TwoColoredTube& two_colored_tube)
		{
			set(two_colored_tube);
			return *this;
		}

		void TwoColoredTube::swap(TwoColoredTube& two_colored_tube)
		{
			GeometricObject::swap(two_colored_tube);
			ColorExtension2::swap(two_colored_tube);
			Vertex2::swap(two_colored_tube);

			float temp = two_colored_tube.radius_;
			two_colored_tube.radius_ = radius_;
			radius_ = temp;
		}

		bool TwoColoredTube::isValid() const
		{
			return (GeometricObject::isValid());
		}

		void TwoColoredTube::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension2::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);
			s << "radius : " << radius_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void TwoColoredTube::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(getVertex1());
			vertices.push_back(getVertex2());
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIVES/twoColoredTube.iC>
#		endif

	} // namespace VIEW
} // namespace BALL
