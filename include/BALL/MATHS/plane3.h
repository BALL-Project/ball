// $Id: plane3.h,v 1.6 2000/02/16 17:07:48 oliver Exp $

#ifndef BALL_MATHS_PLANE3_H
#define BALL_MATHS_PLANE3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_INCLUDE_IEEEFP
#	include <ieeefp.h>
#endif

#include <math.h>
#include <iostream>

#ifndef BALL_MATHS_LINE3_H
#	include <BALL/MATHS/line3.h>
#endif

#ifndef BALL_MATHS_POINT3_H
#	include <BALL/MATHS/point3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

namespace BALL 
{

	/**	Threedimensional plane.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
 	*/
	template <class T>
	class TPlane3
	{
		public:

		BALL_CREATE(TPlane3<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TPlane3()
			:	p(),
				n()
		{
		}

		///
		TPlane3(const TPlane3& plane, bool /* deep */ = true)
			:	p(plane.p),
				n(plane.n)
		{
		}

		///
		TPlane3(const TVector3<T>& point, const TVector3<T>& normal)
			:	p(point),
				n(normal)
		{
		}

		///
		TPlane3(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
			:	p(a),
				n((a - b) % (b - c))
		{
		}

		///
		virtual ~TPlane3()
		{
		}
		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void swap(TPlane3 &plane)
		{
			TVector3<T> temp_point(p);
			p = plane.p;
			plane.p = temp_point;

			TVector3<T> temp_vector(n);
			n = plane.n;
			plane.n = temp_vector;
		}

		///
		void set(const TPlane3& plane, bool /* deep */ = true)
		{
			p = plane.p;
			n = plane.n;
		}

		///
		void set(const TVector3<T>& point, const TVector3<T>& normal)
		{
			p = point;
			n = normal;
		}

		///
		TPlane3& operator = (const TPlane3 &plane)
		{
			p = plane.p;
			n = plane.n;

			return *this;
		}

		///
		void get(TPlane3 &plane, bool /* deep */ = true) const
		{
			plane.p = p;
			plane.n = n;
		}

		///
		void get(TVector3<T>& point, TVector3<T>& normal) const
		{
			point = p;
			normal = n;
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

		///
		void normalize()
		{
			T length = n.getLength();
			// throw an exception on zero length normal
			if (length == 0.0)
			{
				throw Exception::DivisionByZero(__FILE__, __LINE__);
			}

			n /= length;
		}

		/**
		*/
		void hessify()
		{
			normalize();
      if (Maths::isLess(n * p, 0))
      {
        n.negate();
			}
		}

		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TPlane3& plane) const
		{
			return (bool)(p == plane.p && n == plane.n);
		}

		///
		bool operator != (const TPlane3& plane) const
		{
			return (bool)(p != plane.p || n != plane.n);
		}

		///
		bool has(const TVector3<T>& point) const
		{
			return Maths::isZero(n * (point - p));
		}

		///
		bool has(const TLine3<T>& line) const
		{
			return (bool)(Maths::isZero(n * line.d) && has(line.p));
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

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}

		/**	@name	Storers
		*/
		//@{

		///
		friend std::istream& operator >> (std::istream& s, TPlane3& plane)
		{
			return (s >> "PLANE(" >> plane.p >> ", " >> plane.n >> ")");
		}

		///
		friend std::ostream& operator << (std::ostream& s, const TPlane3& plane)
		{
			return (s << plane.p  << plane.n);
		}
		//@}

		/**	@name	Attributes
		*/
		//@{
		/// point
		TVector3<T> p;

		/// normal
		TVector3<T> n;
		//@}
	};

	///
	typedef TPlane3<float> Plane3;

} // namespace BALL

#endif // BALL_MATHS_PLANE3_H
