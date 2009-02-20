// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: circle3.h,v 1.42 2004/07/05 20:57:28 oliver Exp $
//

#ifndef BALL_MATHS_CIRCLE3_H
#define BALL_MATHS_CIRCLE3_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif


namespace BALL 
{
	/** \defgroup Circle Generic three-dimensional circle.

			\ingroup GeometricObjects
	*/
	//@{
	template <typename T>
	class TCircle3;

	/** @name Storers
	 		\ingroup Circle
	*/
	//@{
	///
	template <typename T>
	std::istream& operator >> (std::istream& s, TCircle3<T>& circle);

	///
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TCircle3<T>& circle);
	//@}
	
	/**	Generic Circle in Three-Dimensional Space.
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
				are initialized to <tt>0</tt>.
		*/
		TCircle3()
			:	p(),
				n(),
				radius(0)
		{
		}

		/**	Copy constructor.
				Create a new TCircle3 object from another.
				@param circle the TCircle3 object to be copied
		*/	
		TCircle3(const TCircle3& circle)
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

		/**	Clear method.
				The values are set to 0.
		*/
		virtual void clear() 
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
				@param circle	the circle to be assigned to
		*/
		void get(TCircle3& circle) const
		{
			circle.p = p;
			circle.n = n;
			circle.radius = radius;
		}

		/**	Assign to two variables of type TVector3 and one <tt>T</tt> value.
				@param	point the center point
				@param	normal the circle normal
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
				@return bool, <b>true</b> if all components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TCircle3& circle) const
		{
			return (p == circle.p && n == circle.n && Maths::isEqual(radius, circle.radius));
		}

		/**	Inequality operator.
				@return bool, <b>false</b> if all components are equal, <b>true</b> otherwise
		*/
		bool operator != (const TCircle3& circle) const
		{
			return (p != circle.p || n != circle.n || Maths::isNotEqual(radius, circle.radius));
		}

		/**	Test if a given point is a member of the circle.
				Optional it can be testet, if the point lies on the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, <b>true</b> or <b>false</b>
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const
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
				@return bool <b>true</b>
		*/
		bool isValid() const
		{
			return true;
		}

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const
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
	//@}

	/** needed for windows dlls **/
#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT TCircle3<float>;
#endif

	/**	Default three-dimensional circle class of type <b>float</b>
	 	\ingroup Circle
	*/
	typedef TCircle3<float> Circle3;

	/**	Input operator.
			Reads in two TVector3 and a <b>T</b> value: p, n, radius
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TCircle3<T>& circle)
	{
		  char c;
			s >> c;
			s >> circle.p >> circle.n >> circle.radius;
			s >> c;
			return s;
	}

	/**	Output  Operator.
			Writes the values of <tt>p</tt>, <tt>n</tt>, and <tt>radius</tt> to an 
			output stream. The three values are separated by spaces and enclosed in brackets. \par
			<b>Example:</b> \par
			<tt>((0 0 0) (1 2 1) 3.5)</tt>
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TCircle3<T>& circle)
	{
			return s << '(' << circle.p 
							 << ' ' << circle.n
							 << ' ' << circle.radius 
							 << ')';
	}

} // namespace BALL

#endif // BALL_MATHS_CIRCLE3_H
