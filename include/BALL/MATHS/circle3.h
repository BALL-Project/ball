// $Id: circle3.h,v 1.11 2000/03/21 23:08:06 amoll Exp $

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

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif


namespace BALL 
{
	/**	@name	Three-dimensional circle.
	*/
	//@{

	template <typename T>
	class TCircle3;

	template <typename T>
	std::istream& operator >> (std::istream& s, TCircle3<T>& circle);

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TCircle3<T>& circle);


	/**	Generic Circle in Three-Dimensional Space.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
	*/
	template <typename T>
	class TCircle3
	{
		public:

		BALL_CREATE(TCircle3<T>)


		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TCircle3 object. The three components
				are initialized to {\tt 0}.
		*/
		TCircle3()
			: p(),
				n(),
				radius(0)
		{
		}

		/**	Copy constructor.
				Create a new TCircle3 object from another.
				@param vector the TCircle3 object to be copied
				@param bool ignored - just for interface consistency
		*/	
		TCircle3(const TCircle3& circle, bool /* deep */ = true)
			:	p(circle.p),
				n(circle.n),
				radius(circle.radius)
		{
		}

		/**	Detailled constructor.
				Create a new TCircle3 object from two points and a {\tt T} value.
				@param	point assigned to the point 
				@param	normal assigned to the normal
				@param	radius assigned tp the radius
		*/
		TCircle3(const TVector3<T>& point, const TVector3<T>& normal, const T& radius)
			:	p(point),
				n(normal),
				radius(radius)
		{
		}

		/**	Destructor.	
				Destructs the TCircle3 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TCircle3()
		{
		}
		//@}


		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two circles.
				@param	circle the circle to swap contents with
		*/
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

		/**	Assign from another TCircle3.
				@param vector	the TCirce3 object to assign from
				@param deep ignored
		*/
		void set(const TCircle3& circle, bool /* deep */ = true)
		{
			p = circle.p;
			n = circle.n;
			radius = circle.radius;
		}

		/**	Assign the circle components.
				@param	point assigned to the point 
				@param	normal assigned to the normal
				@param	rad assigned tp the radius
		*/
		void set(const TVector3<T>& point, const TVector3<T>& normal, const T& rad)
		{
			p = point;
			n = normal;
			radius = rad;
		}

		/**	Assignment operator.
				Assign the components from another circle.
				@param circle the circle to assign from
		**/
		TCircle3& operator = (const TCircle3& circle)
		{
			p = circle.p;
			n = circle.n;
			radius = circle.radius;

			return *this;
		}

		/**	Assign to another TCircle3.
				Assigns the components to another circle.
				@param circle	the circle to be asigned to
				@param deep ignored
		*/
		void get(TCircle3& circle, bool /* deep */ = true) const
		{
			circle.p = p;
			circle.n = n;
			circle.radius = radius;
		}

		/**	Assign to two variables of type TVector3 and one {\tt T} value.
				@param	point
				@param	normal
				@param	rhs the radius
		*/
		void get(TVector3<T>& point, TVector3<T>& normal, T& rhs) const
		{
			point = p;
			normal = n;
			rhs = radius;
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool operator == (const TCircle3& circle) const
		{
			return (bool)(p == circle.p && n == circle.n && Maths::isEqual(radius, circle.radius));
		}

		/**	Inequality operator.
				@return bool, {\bf false} if all components are equal, {\bf true} otherwise
		*/
		bool operator != (const TCircle3& circle) const
		{
			return (bool)(p != circle.p || n != circle.n || Maths::isNotEqual(radius, circle.radius));
		}


		/**	Test if a given point is a member of the circle.
				Optional it can be testet, if it is a member of the surface.
				@param point the point to be tested
				@param on_surface = true, to test the surface (default = false)
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const
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

		/**	Test if instance is valid.
				always retruns true
				@return bool {\bf true}
		*/
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

		/**	@name	Attributes
		*/
		//@{

		/**	Circle Center.
				This point describes the center of the circle.
		*/
		TVector3<T> p;

		/**	Normal vector.
				This vector is orthogonal to the circle's plane.
		*/
		TVector3<T> n;

		/**	Radius.
				The radius of the circle.
		*/
		T radius;
		//@}
	};

	/**	Default three-dimensional circle class of type {\bf float}
	*/
	typedef TCircle3<float> Circle3;

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
			reads in two TVector3 and a {\bf T} value: p, n, radius
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TCircle3<T>& circle)
	{
		  char c;
			for(int i=0; i<7; i++)
			{
				s >> c;
			}
			s >> circle.p >> circle.n >> circle.radius;
			s >> c;
			return s;
	}

	/**	Output- Operator
			prints out two TVector3 and a {\bf T} value: p, n. radius
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TCircle3<T>& circle)
	{
			return (s << "CIRCLE(" << circle.p 
								<< " " << circle.n
								<< " " << circle.radius << ")");
	}
	//@}

} // namespace BALL

#endif // BALL_MATHS_CIRCLE3_H
