// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: disc.C,v 1.4.28.1 2007-03-25 22:02:33 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/disc.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Disc::Disc()
			:	GeometricObject(),
				Circle3()
		{
		}

		Disc::Disc(const Disc& disc)
			:	GeometricObject(disc),
				Circle3(disc)
		{
		}

		Disc::~Disc()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
									  << " of class " << RTTI::getName<Disc>() << endl;
			#endif 
		}

		void Disc::clear()
		{
			GeometricObject::clear();
			Circle3::clear();
		}

		void Disc::set(const Disc& disc)
		{
			GeometricObject::set(disc);
			Circle3::set(disc);
		}

		const Disc& Disc::operator = (const Disc& disc)
		{
			set(disc);
			return *this;
		}

		void Disc::swap(Disc& disc)
		{
			GeometricObject::swap(disc);
			Circle3::swap(disc);
		}

		bool Disc::isValid() const
		{
			return (GeometricObject::isValid() && 
											Circle3::isValid());
		}

		void Disc::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Circle3::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Disc::setCircle(const Circle3& circle)
		{
			this->p = circle.p;
			this->n = circle.n;
			this->radius = circle.radius;
		}

		Disc::Disc(const Circle3& circle)
			: GeometricObject(),
				Circle3(circle)
		{}

		void Disc::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(n);
		}

	} // namespace VIEW
} // namespace BALL
