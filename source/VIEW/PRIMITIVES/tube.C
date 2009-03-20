// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tube.C,v 1.7.16.1 2007-03-25 22:02:36 oliver Exp $
//

#include <BALL/VIEW/PRIMITIVES/tube.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Tube::Tube()
			: GeometricObject(),
   			Vertex2(),
				radius_(1)
		{
		}

		Tube::Tube(const Tube& tube)
			: GeometricObject(tube),
   			Vertex2(tube),
				radius_(tube.radius_)
		{
		}

		Tube::~Tube()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info()  << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Tube>() << std::endl;
			#endif 
		}

		void Tube::clear()
		{
			GeometricObject::clear();
			Vertex2::clear();
			radius_ = 1;
		}

		void Tube::set(const Tube& tube)
		{
			GeometricObject::set(tube);
			Vertex2::set(tube);
			radius_ = tube.radius_;
		}

		const Tube& Tube::operator = (const Tube& tube)
		{
			set(tube);
			return *this;
		}

		void Tube::swap(Tube& tube)
		{
			GeometricObject::swap(tube);
			Vertex2::swap(tube);

			float temp = tube.radius_;
			tube.radius_ = radius_;
			radius_ = temp;
		}

		bool Tube::isValid() const
		{
			return (GeometricObject::isValid()	
							&& Vertex2::isValid());
		}

		void Tube::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);
			s << "radius : " << radius_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Tube::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(getVertex1());
			vertices.push_back(getVertex2());
		}

	} // namespace VIEW
} // namespace BALL
