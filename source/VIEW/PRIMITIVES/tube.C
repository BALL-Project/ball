// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tube.C,v 1.1 2003/08/26 12:38:44 oliver Exp $

#include <BALL/VIEW/PRIMITIV/tube.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Tube::Tube()
			throw()
			: GeometricObject(),
   			Vertex2(),
				radius_(1)
		{
		}

		Tube::Tube(const Tube& tube, bool deep)
			throw()
			: GeometricObject(tube, deep),
   			Vertex2(tube),
				radius_(1)
		{
		}

		Tube::~Tube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info()  << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Tube>() << std::endl;
			#endif 
		}

		void Tube::clear()
			throw()
		{
			GeometricObject::clear();
			Vertex2::clear();
			radius_ = 1;
		}

		void Tube::set(const Tube& tube, bool deep)
			throw()
		{
			GeometricObject::set(tube, deep);
			Vertex2::set(tube);
			radius_ = tube.radius_;
		}

		const Tube& Tube::operator = (const Tube& tube)
			throw()
		{
			set(tube);
			return *this;
		}

		void Tube::get(Tube& tube, bool deep) const
			throw()
		{
			tube.set(*this, deep);
		}

		void Tube::swap(Tube& tube)
			throw()
		{
			GeometricObject::swap(tube);
			Vertex2::swap(tube);

			Real temp = tube.radius_;
			tube.radius_ = radius_;
			radius_ = temp;
		}

		bool Tube::isValid() const
			throw()
		{
			return (GeometricObject::isValid()	
							&& Vertex2::isValid());
		}

		void Tube::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);
			s << "radius : " << radius_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
