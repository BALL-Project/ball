// $Id: circle3.h,v 1.5 2000/01/10 15:51:02 oliver Exp $

#ifndef BALL_MATHS_CIRCLE3_H
#define BALL_MATHS_CIRCLE3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_INCLUDE_IEEEFP
#	include <ieeefp.h>
#endif 

#include <math.h>
#include <iostream>

#ifndef BALL_MATHS_POINT3_H
#	include <BALL/MATHS/point3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif


namespace BALL 
{

	/**	Generic Circle in Three-Dimensional Space.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
	*/
	template <class T>
	class TCircle3
	{
		public:

		BALL_CREATE(TCircle3<T>)


		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TCircle3()
			: p(),
				n(),
				radius(0)
		{
		}

		///
		TCircle3(const TCircle3& circle, bool /* deep */ = true)
			:	p(circle.p),
				n(circle.n),
				radius(circle.radius)
		{
		}

		///
		TCircle3(const TPoint3<T>& point, const TVector3<T>& normal, const T& radius)
			:	p(point),
				n(normal),
				radius(radiuse)
		{
		}

		///
		virtual ~TCircle3()
		{
		}
		//@}


		/**	@name	Assignment
		*/
		//@{

		///
		void swap(TCircle3& circle)
		{
			TVector3<T> temp_vector(p);
			p = circle.p;
			circle.p = temp_vector;

			temp_vector = n;
			n = circle.n;
			circle.n = temp_vector;

			T temp = radius;
			radius = circle.radius;
			circle.radius = temp;
		}

		///
		void set(const TCircle3& circle, bool /* deep */ = true)
		{
			p = circle.p;
			n = circle.n;
			radius = circle.radius;
		}

		///
		void set(const TPoint3<T>& point, const TVector3<T>& normal, const T& radius)
		{
			p = point;
			n = normal;
			radius = radius;
		}

		///
		TCircle3& operator = (const TCircle3 &)
		{
			p = circle.p;
			n = circle.n;
			radius = circle.radius;

			return *this;
		}

		///
		void get(TCircle3& circle, bool /* deep */ = true) const
		{
			circle.p = p;
			circle.n = n;
			circle.radius = radius;
		}

		///
		void get(TPoint3<T>& point, TVector3<T>& normal, T& radius) const
		{
			point = p;
			normal = n;
			radius = radius;
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TCircle3& circle) const
		{
			return (bool)(p == circle.p && n == circle.n && Maths::isEqual(radius, circle.radius));
		}

		///
		bool operator != (const TCircle3& circle) const
		{
			return (bool)(p != circle.p || n != circle.n || Maths::isNotEqual(radius, circle.radius));
		}


		///
		bool has(const TPoint3<T>& point, bool on_surface = false) const
		{
			if (on_surface == true)
			{
				return (bool)(Maths::isZero(n * (point - p))
											&& Maths::isEqual(p.getDistance(point), radius));
			} else {
				return (bool)(Maths::isZero(n * (point - p)) 
											&& Maths::isLessOrEqual(p.getDistance(point), radius));
			}
		}
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		///
		bool isValid() const
		{
			return true;
		}

		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
	
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "  position: " << p << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  normal: " << n << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  radius: " << radius << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}

		/**	@name	Storers
		*/
		//@{

		///
		friend std::istream& operator >> (std::istream& s, TCircle3 &)
		{
			return (s >> circle.p >> circle.n  >> circle.radius);
		}

		///
		friend std::ostream& operator << (std::ostream& s,const TCircle3& circle)
		{
			return (s << "CIRCLE(" << circle.p 
								<< ", " << circle.n
								<< ", " << circle.radius << ")");
		}
		//@}

		/**	@name	Attributes
		*/
		//@{

		/**	Circle Center.
				This point describes the center of the circle.
		*/
		TPoint3<T> 	p;

		/**	Normal vector.
				This vector is orthogonal to the circle's plane.
		*/
		TVector3<T> n;

		/**	Radius.
				The radius of the circle.
		*/
		T 					radius;
		//@}
	};

	///
	typedef TCircle3<Real> Circle3;

} // namespace BALL

#endif // BALL_MATHS_CIRCLE3_H
