// $Id: sphere3.h,v 1.19 2000/05/05 17:39:06 amoll Exp $

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

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/** Generic Sphere Class.
      {\bf Definition:} \URL{BALL/MATHS/sphere3.h}
      \\
	*/
	template <typename T>
	class TSphere3
	{
		public:

		BALL_CREATE_NODEEP(TSphere3)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TSphere3 object. The three components
				are initialized to {\tt (T)0}.
		*/
		TSphere3()
			: p(),
				radius(0)
		{
		}

		/**	Copy constructor.
				Create a new Sphere3 object from another.
				@param sphere3 the Sphere3 object to be copied
		*/	
		TSphere3(const TSphere3& sphere3)
			: p(sphere3.p),
				radius(sphere3.radius)
		{
		}

		/**	Detailed constructor.
				Create a new Sphere3 object from a point and a radius.
				@param	point assigned to the point
				@param	radius assigned to the radius
		*/
		TSphere3(const TVector3<T>& point, const T& radius)
			: p(point),
				radius(radius)
		{
		}

		/**	Destructor.	
				Destructs the Sphere3 object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TSphere3()
		{
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		/**	Swap the contents of two instances of Sphere3.
				@param	sphere3 the sphere3 to swap contents with
		*/
		void swap(TSphere3& sphere3)
		{
			TVector3<T> temp_point(p);
			p = sphere3.p;
			sphere3.p = temp_point;

			T temp = radius;
			radius = sphere3.radius;
			sphere3.radius = temp;
		}

		/**	Assign from another instance of Sphere3.
				@param sphere3	the Sphere3 object to assign from
		*/
		void set(const TSphere3& sphere)
		{
			p = sphere.p;
			radius = sphere.radius;
		}

		/**	Assign to an instance of TVector3 and a variable of type {\tt T}.
				@param point the point to assign from
				@param rhs the radius to assign from

		*/
		void set(const TVector3<T>& point, const T& rhs)
		{
			p = point;
			radius = rhs;
		}

		/**	Assignment operator.
				Assign the components from another instance of Sphere.
				@param sphere3 the sphere to assign from
		**/
		TSphere3& operator =
			(const TSphere3& sphere3)
		{
			p = sphere3.p;
			radius = sphere3.radius;

			return *this;
		}

		/**	Assign to another instance of Sphere.
				Assigns the components to another Sphere.
				@param sphere	the vector to be assigned to
		*/
		void get(TSphere3& sphere3) const
		{
			sphere3.p = p;
			sphere3.radius = radius;
		}

		/**	Assign to an instance of Vector3 and a variable of type {\tt T}.
				@param	point the point component
				@param	rhs the radius component
		*/
		void get(TVector3<T>& point, T& rhs) const
		{
			point = p;
			rhs = radius;
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool operator == (const TSphere3& sphere3) const
		{
			return (bool)(p == sphere3.p && Maths::isEqual(radius, sphere3.radius));
		}

		/**	Inequality operator.
				@return bool, {\bf true} if the two Sphere3 differ in at least one component, {\bf false} otherwise
		*/
		bool operator != (const TSphere3& sphere3) const
		{
			return (bool)(p != sphere3.p || Maths::isNotEqual(radius, sphere3.radius));
		}

		/**	Test whether a given point is a member of the Sphere.
				Optional it can be testet, if it is a member of the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const
		{
			if (on_surface == true)
			{
				return Maths::isEqual(p.getDistance(point), radius);
			} else {
				return Maths::isLessOrEqual(p.getDistance(point), radius);
			}
		}

		/**	Test whether the Sphere is empty (radius = 0).
				@return bool, {\bf true} or {\bf false}
		*/
		bool isEmpty() const
		{
			return Maths::isZero(radius);
		}
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true
				@return bool {\bf true}
		*/
		bool isValid() const
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
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "  position: " << p << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  radius: " << radius << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}



		/**	@name	Attributes
		*/
		//@{

		/**	The midpoint of the sphere.
		*/
		TVector3<T> p;

		/** The radius of the sphere.
		*/
		T radius;
		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator.
			Reads in an instance of TVector3 and a {\bf T} value : p, radius
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TSphere3<T>& sphere3)
	{
		char c;
		s >> c >> sphere3.p >> sphere3.radius >> c;
		return s;
	}

	/**	Output Operator.
			Write the contents of {\tt p} and {\tt radius} to an output stream.
			The values are enclose in brackets.\\
			{\bf Example:}\\
			{\tt ((0 1 1.5) 0.4)}
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSphere3<T>& sphere3)
	{
		s << '(' << sphere3.p << ' ' << sphere3.radius << ')';
		return s;
	}
	//@}


	/**	The Default TSphere3 Type.
			If double precision is not needed, {\tt TSphere3<float>} should
			be used. It is predefined as {\tt Sphere3} for convenience.
	*/
	typedef TSphere3<float> Sphere3;

} // namespace BALL

#endif // BALL_MATHS_SPHERE3_H
