// $Id: sphere3.h,v 1.5 2000/01/16 17:28:43 oliver Exp $

#ifndef BALL_MATHS_SPHERE3_H
#define BALL_MATHS_SPHERE3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_INCLUDE_IEEEFP
#	include <ieeefp.h>
#endif

#include <math.h>
#include <iostream>

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_POINT3_H
#	include <BALL/MATHS/point3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/** Generic Sphere Class.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
	*/
	template <class T>
	class TSphere3
	{
		public:

		BALL_CREATE(TSphere3)

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TSphere3()
			: p(),
				radius(0)
		{
		}

		///
		TSphere3(const TSphere3& sphere3,bool /* deep */ = true)
			: p(sphere3.p),
				radius(sphere3.radius)
		{
		}

		///
		TSphere3(const TPoint3<T>& point, const T& radius)
			: p(point),
				radius(radius)
		{
		}

		///
		virtual ~TSphere3()
		{
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		///
		void swap(TSphere3& sphere3)
		{
			TPoint3<T> temp_point(p);
			p = sphere3.p;
			sphere3.p = temp_point;

			T temp = radius;
			radius = sphere3.radius;
			sphere3.radius = temp;
		}

		///
		void set(const TSphere3& sphere3, bool /* deep */ = true)
		{
			p = sphere3.p;
			radius = sphere3.radius;
		}

		///
		void set(const TPoint3<T>& point, const T& radius)
		{
			p = point;
			radius = radius;
		}

		///
		TSphere3 &operator =
			(const TSphere3& sphere3)
		{
			p = sphere3.p;
			radius = sphere3.radius;

			return *this;
		}

		///
		void get(TSphere3& sphere3, bool /* deep */ = true) const
		{
			sphere3.p = p;
			sphere3.radius = radius;
		}

		///
		void get(TPoint3<T>& point, T& radius) const
		{
			point = p;
			radius = radius;
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TSphere3& sphere3) const
		{
			return (bool)(p == sphere3.p && Maths::isEqual(radius, sphere3.radius));
		}

		///
		bool operator != (const TSphere3& sphere3) const
		{
			return (bool)(p != sphere3.p || Maths::isNotEqual(radius, sphere3.radius));
		}

		///
		bool has(const TPoint3<T>& point, bool on_surface = false) const
		{
			if (on_surface == true)
			{
				return Maths::isEqual(p.getDistance(point), radius);
			} else {
				return Maths::isLessOrEqual(p.getDistance(point), radius);
			}
		}

		///
		bool isEmpty() const
		{
			return Maths::isZero(radius);
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
			s << "  radius: " << radius << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}

		/**	@name	Storers
		*/
		//@{

		///
		friend std::istream& operator >> (std::istream& s, TSphere3& sphere3)
		{
			return (s >> sphere3.p >> sphere3.radius);
		}

		///
		friend std::ostream& operator << (std::ostream& s, const TSphere3& sphere3)
		{
			return (s << "SPHERE(" << sphere3.p << ", " << sphere3.radius << ")");
		}
		//@}

		/**	@name	Attributes
		*/
		//@{

		///
		TPoint3<T> p;

		///
		T radius;
		//@}
	};


	///
	typedef TSphere3<float> Sphere3;

} // namespace BALL

#endif // BALL_MATHS_SPHERE3_H
