// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: disc.C,v 1.2 2003/08/26 15:32:47 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/disc.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Disc::Disc()
			throw()
			:	GeometricObject(),
				Circle3()
		{
		}

		Disc::Disc(const Disc& disc, bool deep)
			throw()
			:	GeometricObject(disc, deep),
				Circle3(disc)
		{
		}

		Disc::~Disc()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
									  << " of class " << RTTI::getName<Disc>() << endl;
			#endif 
		}

		void Disc::clear()
			throw()
		{
			GeometricObject::clear();
			Circle3::clear();
		}

		void Disc::set(const Disc& disc, bool deep)
			throw()
		{
			GeometricObject::set(disc, deep);
			Circle3::set(disc);
		}

		const Disc& Disc::operator = (const Disc& disc)
			throw()
		{
			set(disc);
			return *this;
		}

		void Disc::get(Disc& disc, bool deep) const
			throw()
		{
			disc.set(*this, deep);
		}

		void Disc::swap(Disc& disc)
			throw()
		{
			GeometricObject::swap(disc);
			Circle3::swap(disc);
		}

		bool Disc::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
											Circle3::isValid());
		}

		void Disc::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Circle3::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Disc::setCircle(const Circle3& circle)
			throw()
		{
			this->p = circle.p;
			this->n = circle.n;
			this->radius = circle.radius;
		}

		Disc::Disc(const Circle3& circle)
			throw()
			: GeometricObject(),
				Circle3(circle)
		{}

	} // namespace VIEW
} // namespace BALL
