// $Id: circle3.h,v 1.25 2001/02/21 14:27:22 anker Exp $

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
	std::istream& operator >> (std::istream& s, TCircle3<T>& circle)
		throw();

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TCircle3<T>& circle)
		throw();

	/**	Generic Circle in Three-Dimensional Space.\\
			{\bf Definition:} \URL{BALL/MATHS/.h}\\
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
			throw()
			: p(),
				n(),
				radius(0)
		{
		}

		/**	Copy constructor.
				Create a new TCircle3 object from another.
				@param circle the TCircle3 object to be copied
		*/	
		TCircle3(const TCircle3& circle)
			throw()
			:	p(circle.p),
				n(circle.n),
				radius(circle.radius)
		{
		}

		/**	Detailed constructor.
				Create a new TCircle3 object from the central point, the normal and a radius.
				@param	point assigned to the point 
				@param	normal assigned to the normal
				@param	radius assigned tp the radius
		*/
		TCircle3(const TVector3<T>& point, const TVector3<T>& normal, const T& radius)
			throw()
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
			throw()
		{
		}

		/**	Clear method.
				The values are set to 0.
		*/
		virtual void clear() 
			throw()
		{
			p.clear();
			n.clear();
			radius = (T)0;
		}

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two circles.
				@param	circle the circle to swap contents with
		*/
		void swap(TCircle3& circle)
			throw()
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
				@param circle	the TCirce3 object to assign from
		*/
		void set(const TCircle3& circle)
			throw()
		{
			p = circle.p;
			n = circle.n;
			radius = circle.radius;
		}

		/**	Assign the circle components.
				@param	point assigned to the point 
				@param	normal assigned to the normal
				@param	rad assigned to the radius
		*/
		void set(const TVector3<T>& point, const TVector3<T>& normal, const T& rad)
			throw()
		{
			p = point;
			n = normal;
			radius = rad;
		}

		/**	Assignment operator.
				Assign the components from another circle.
				@param circle the circle to assign from
		**/
		const TCircle3& operator = (const TCircle3& circle)
			throw()
		{
			p = circle.p;
			n = circle.n;
			radius = circle.radius;

			return *this;
		}

		/**	Assign to another TCircle3.
				Assigns the components to another circle.
				@param circle	the circle to be assigned to
		*/
		void get(TCircle3& circle) const
			throw()
		{
			circle.p = p;
			circle.n = n;
			circle.radius = radius;
		}

		/**	Assign to two variables of type TVector3 and one {\tt T} value.
				@param	point the center point
				@param	normal the circle normal
				@param	rhs the radius
		*/
		void get(TVector3<T>& point, TVector3<T>& normal, T& rhs) const
			throw()
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
			throw()
		{
			return (p == circle.p && n == circle.n && Maths::isEqual(radius, circle.radius));
		}

		/**	Inequality operator.
				@return bool, {\bf false} if all components are equal, {\bf true} otherwise
		*/
		bool operator != (const TCircle3& circle) const
			throw()
		{
			return (p != circle.p || n != circle.n || Maths::isNotEqual(radius, circle.radius));
		}

		/**	Test if a given point is a member of the circle.
				Optional it can be testet, if the point lies on the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const
			throw()
		{
			if (on_surface)
			{
				return (Maths::isZero(n * (point - p))
											&& Maths::isEqual(p.getDistance(point), radius));
			} 
			else 
			{
				return (Maths::isZero(n * (point - p)) 
											&& Maths::isLessOrEqual(p.getDistance(point), radius));
			}
		}

		//@}
		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test if instance is valid.
				Always returns true
				@return bool {\bf true}
		*/
		bool isValid() const
			throw()
		{
			return true;
		}

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
	
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "  position: " << p << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  normal: " << n << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  radius: " << radius << std::endl;

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

	/**	Input- Operator.
			Reads in two TVector3 and a {\bf T} value: p, n, radius
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TCircle3<T>& circle)
		throw()
	{
		  char c;
			s >> c;
			s >> circle.p >> circle.n >> circle.radius;
			s >> c;
			return s;
	}

	/**	Output  Operator.
			Writes the values of {\tt p}, {\tt n}, and {\tt radius} to an 
			output stream. The three values are separated by spaces and enclosed in brackets.\\
			{\bf Example:}\\
			{\tt ((0 0 0) (1 2 1) 3.5)}
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TCircle3<T>& circle)
		throw()
	{
			return s << '(' << circle.p 
							 << ' ' << circle.n
							 << ' ' << circle.radius 
							 << ')';
	}

	//@}
	//@}

} // namespace BALL

#endif // BALL_MATHS_CIRCLE3_H
