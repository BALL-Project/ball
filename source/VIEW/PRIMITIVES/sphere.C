// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sphere.C,v 1.5 2003/12/10 15:30:53 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/sphere.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Sphere::Sphere()
			throw()
			:	GeometricObject(),
				Sphere3(),
				point_ptr_(&p)
		{
		}

		Sphere::Sphere(const Sphere& sphere)
			throw()
			:	GeometricObject(sphere),
				Sphere3(sphere),
				point_ptr_(sphere.point_ptr_)
		{
		}

		Sphere::~Sphere()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
									  << " of class " << RTTI::getName<Sphere>() << endl;
			#endif 
		}

		void Sphere::clear()
			throw()
		{
			GeometricObject::clear();
			Sphere3::clear();
			point_ptr_ = &p;
		}

		void Sphere::set(const Sphere& sphere)
			throw()
		{
			GeometricObject::set(sphere);
			Sphere3::set(sphere);
			point_ptr_ = sphere.point_ptr_;
		}

		const Sphere& Sphere::operator = (const Sphere& sphere)
			throw()
		{
			set(sphere);
			return *this;
		}

		void Sphere::swap(Sphere& sphere)
			throw()
		{
			GeometricObject::swap(sphere);
			Sphere3::swap(sphere);


			Vector3 *tmp_vector = point_ptr_;

			if (sphere.point_ptr_ != &sphere.p)
			{
				point_ptr_ = sphere.point_ptr_;
				
				if (tmp_vector != &p)
				{
					sphere.point_ptr_ = tmp_vector;
				}
				else
				{
					sphere.point_ptr_ = &sphere.p;
				}
			}
			else if (point_ptr_ != &p)
			{
				sphere.point_ptr_ = tmp_vector;
				
				point_ptr_ = &sphere.p;
			}  

			p.swap(sphere.p);
		}

		bool Sphere::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
											Sphere3::isValid() &&
														point_ptr_ != 0);
		}

		void Sphere::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "point : " << (*point_ptr_) << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "pointer : " << (point_ptr_) << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "radius: " << (radius) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
