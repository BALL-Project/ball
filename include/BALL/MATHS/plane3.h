// $Id: plane3.h,v 1.21 2001/01/21 21:10:14 amoll Exp $

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

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

namespace BALL 
{

	/**	Threedimensional plane.
      {\bf Definition:} \URL{BALL/MATHS/plane3.h}
			A plane is defined by a point and its normal.
	*/

	template <typename T>
	class TPlane3;

	template <typename T>
	std::istream& operator >> (std::istream& s, TPlane3<T>& plane)
		throw();

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TPlane3<T>& plane)
		throw();

	template <typename T>
	class TPlane3
	{
		public:

		BALL_CREATE(TPlane3<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TPlane3 object. The two components
				are initialized to {\tt (T)0}.
		*/
		TPlane3()
			throw()
			:	p(),
				n()
		{
		}

		/**	Copy constructor.
				Create a new TPlane3 object from another.
				@param plane the TPlane3 object to be copied
		*/	
		TPlane3(const TPlane3& plane)
			throw()
			:	p(plane.p),
				n(plane.n)
		{
		}

		/**	Detailed constructor.
				Create a new instances of TPlane3 from a point and the normal.
				@param	point assigned to {\tt p}
				@param	normal assigned to {\tt n}
		*/
		TPlane3(const TVector3<T>& point, const TVector3<T>& normal)
			throw()
			:	p(point),
				n(normal)
		{
		}

		/**	Detailed constructor.
				Create a new instance of TPlane3 from three points.
				@param	a assigned to {\tt p}
				@param	a, b, c are used to calculate the normal {\tt n}
		*/
		TPlane3(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
			throw()
			:	p(a),
				n((a - b) % (b - c))
		{
		}

		/**	Constructor.
				Create a new TPlane3 object from four {\tt T} values.
				Form: COORDINATE (ax + by + cz + d = 0)
				@param	a, b, c are used to calculate the normal {\tt n} and the point {\tt p}
		*/
		TPlane3(const T& a, const T& b, const T& c, const T& d)
			throw(Exception::DivisionByZero)
		{
			n = TVector3<T>(a, b, c);
			if (a == 0 && b == 0 && c == 0)
			{
				throw Exception::DivisionByZero(__FILE__, __LINE__);			
			}
			if (!Maths::isZero(a))
			{
				p.set(-d / a, 0, 0);
			}
			else if (!Maths::isZero(b))
			{
				p.set(0, -d / b, 0);
			}
			else if (!Maths::isZero(c))
			{
				p.set(0, 0, -d / c);
			}
		}

		/**	Destructor.	
				Destructs the TPlane3 object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TPlane3()
			throw()
		{
		}

		/**	Clear method.
				The value are set to 0.
		*/
		virtual void clear() 
			throw()
		{
			n.clear();
			p.clear();
		}

		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void swap(TPlane3 &plane)
			throw()
		{
			TVector3<T> temp_point(p);
			p = plane.p;
			plane.p = temp_point;

			TVector3<T> temp_vector(n);
			n = plane.n;
			plane.n = temp_vector;
		}

		/**	Swap the contents of two instances of planes.
				@param vector the plane to swap contents with
				@param bool ignored - just for interface consistency
		*/
		void set(const TPlane3& plane)
			throw()
		{
			p = plane.p;
			n = plane.n;
		}

		/**	Assign from a point and a normal.
				@param	point the new point
				@param	normal the new normal
		*/
		void set(const TVector3<T>& point, const TVector3<T>& normal)
			throw()
		{
			p = point;
			n = normal;
		}

		/**	Assign from three points.
				@param a the first point
				@param b the second point
				@param c the third point
		*/
		void set(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
			throw()
		{
				p = a;
				n = (a - b) % (b - c);
		}

		/**	Assignment operator.
				Assign the components from another instance of plane.
				@param plane the plane to assign from
		**/
		const TPlane3& operator = (const TPlane3 &plane)
			throw()
		{
			p = plane.p;
			n = plane.n;

			return *this;
		}

		/**	Assign to another instance of Plane3.
				Assigns the components to another plane.
				@param plane the plane to be assigned to
		*/
		void get(TPlane3& plane) const
			throw()
		{
			plane.p = p;
			plane.n = n;
		}

		/**	Assign to a point and a normal.
				@param point the point to be assigned to
				@param normal the normal to be assigned to
		*/
		void get(TVector3<T>& point, TVector3<T>& normal) const
			throw()
		{
			point = p;
			normal = n;
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Normalize the the normal of the plane.
				The normal is scaled with its length:
				$\{x|y|z\} *= \sqrt{x^2 + y^2 + z^2}$.
				@exception DivisionByZero if the length of the normal is 0
		*/
		void normalize()
			throw(Exception::DivisionByZero)
		{
			T length = n.getLength();
			// throw an exception on zero length normal
			if (length == 0.0)
			{
				throw Exception::DivisionByZero(__FILE__, __LINE__);
			}

			n /= length;
		}

		/**	Hessify the plane.
				The normal is scaled with its length:
				$\{x|y|z\} *= \sqrt{x^2 + y^2 + z^2}$.
				If the dot product of the point with the normal
				is less then zero, the normal is negated.
		*/
		void hessify()
			throw()
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

		/**	Equality operator.
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool operator == (const TPlane3& plane) const
			throw()
		{
			return (p == plane.p && n == plane.n);
		}

		/**	Inequality operator.
				@return bool, {\bf false} if all components are equal, {\bf true} otherwise
		*/
		bool operator != (const TPlane3& plane) const
			throw()
		{
			return (p != plane.p || n != plane.n);
		}

		/**	Test whether a given point is a member of the plane.
				@param point the point to be tested
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TVector3<T>& point) const
			throw()
		{
			return Maths::isZero(n * (point - p));
		}

		/**	Test whether a given line is a member of the plane.
				@param line the line to be tested
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TLine3<T>& line) const
			throw()
		{
			return (Maths::isZero(n * line.d) && has(line.p));
		}
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				always retruns true
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

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}

		/**	@name	Attributes
		*/
		//@{
		/**	The point
		*/
		TVector3<T> p;

		/**	The normal
		*/
		TVector3<T> n;
		//@}
	};

	/**	Default plane class of type {\bf float}
	*/
	typedef TPlane3<float> Plane3;

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
			reads in two instances of TVector3: {\bf p, n}
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TPlane3<T>& plane)
		throw()
	{
		char c;
		s >> c >> plane.p >>  plane.n >> c;
		return s;
	}

	/**	Output- Operator
			Prints two instances of TVector3: {\bf p, n}
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TPlane3<T>& plane)
		throw()
	{
		return (s << '(' << plane.p << ' '  << plane.n << ')');
	}
	//@}

} // namespace BALL

#endif // BALL_MATHS_PLANE3_H
