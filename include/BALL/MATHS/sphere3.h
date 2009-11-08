// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_SPHERE3_H
#define BALL_MATHS_SPHERE3_H

#ifdef BALL_HAS_IEEEFP_H
#	include <ieeefp.h>
#endif

#include <iostream>

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	/** \defgroup Sphere Sphere
    \ingroup GeometricObjects
	 */
	//@{

	/** Generic Sphere Class.
	*/
	template <typename T>
	class TSphere3
	{
		public:

		BALL_CREATE(TSphere3)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TSphere3 object. The three components
				are initialized to <tt>(T)0</tt>.
		*/
		TSphere3()
		 : p(),
		   radius(0)
		{
		}

		/**	Copy constructor.
				Create a new Sphere3 object from another.
				@param sphere the Sphere3 object to be copied
		*/	
		TSphere3(const TSphere3& sphere)
		 : p(sphere.p),
		   radius(sphere.radius)
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
				As there are no dynamic data structures, nothing happens.
		*/
		virtual ~TSphere3()
		{
		}

		/** Default initialisation
		 */
		virtual void clear()
		{
			p.clear();
			radius = (T) 0;
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		/**	Swap the contents of two instances of Sphere3.
				@param	sphere the Sphere3 to swap contents with
		*/
		void swap(TSphere3& sphere)
		{
			TVector3<T> temp_point(p);
			p = sphere.p;
			sphere.p = temp_point;

			T temp = radius;
			radius = sphere.radius;
			sphere.radius = temp;
		}

		/**	Assign from another instance of Sphere3.
				@param sphere	the Sphere3 object to assign from
		*/
		void set(const TSphere3& sphere)
		{
			p = sphere.p;
			radius = sphere.radius;
		}

		/**	Assign from a point and a radius.
				@param point the point to assign from
				@param r the radius to assign from

		*/
		void set(const TVector3<T>& point, const T& r)
		{
			p = point;
			radius = r;
		}

		/**	Assignment operator.
				Assign the components from another instance of Sphere.
				@param sphere the sphere to assign from
		**/
		TSphere3& operator = (const TSphere3& sphere)
		{
			p = sphere.p;
			radius = sphere.radius;
			return *this;
		}

		/**	Assign to another instance of Sphere.
				Assigns the components to another Sphere.
				@param sphere	the sphere to be assigned to
		*/
		void get(TSphere3& sphere) const
		{
			sphere.p = p;
			sphere.radius = radius;
		}

		/**	Assign to an instance of Vector3 and a variable of type <tt>T</tt>.
				@param	point the point component
				@param	r the radius component
		*/
		void get(TVector3<T>& point, T& r) const
		{
			point = p;
			r = radius;
		}

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, <b>true</b> if all components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TSphere3& sphere) const
		{
			return (p == sphere.p && Maths::isEqual(radius, sphere.radius));
		}

		/**	Inequality operator.
				@return bool, <b>true</b> if the two Sphere3 differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TSphere3& sphere) const
		{
			return (p != sphere.p || Maths::isNotEqual(radius, sphere.radius));
		}

		/**	Test whether a given point is a member of the Sphere.
				Optional it can be tested if the point lies on the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, <b>true</b> or <b>false</b>
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const
		{
			if (on_surface)
			{
				return Maths::isEqual(p.getDistance(point), radius);
			} 
			else 
			{
				return Maths::isLessOrEqual(p.getDistance(point), radius);
			}
		}

		/**	Test whether the Sphere is empty (radius = 0).
				@return bool, <b>true</b> or <b>false</b>
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
	//@}

	/**	@name	Storers
	 		Stream operators for the class Sphere

			\ingroup Sphere
	*/
	//@{

	/**	Input operator.
			Reads in an instance of TVector3 and a <b>T</b> value : p, radius.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TSphere3<T>& sphere)
		
	{
		char c;
		s >> c >> sphere.p >> sphere.radius >> c;
		return s;
	}

	/**	Output Operator.
			Write the contents of <tt>p</tt> and <tt>radius</tt> to an output stream.
			The values are enclose in brackets. \par
			<b>Example:</b> \par
			<tt>((0 1 1.5) 0.4)</tt>
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSphere3<T>& sphere)
		
	{
		s << '(' << sphere.p << ' ' << sphere.radius << ')';
		return s;
	}
	//@}
	
	/**	Default sphere type.
			If double precision is not needed, <tt>TSphere3<float></tt> should
			be used. It is predefined as <tt>Sphere3</tt> for convenience.
	 		\ingroup Sphere
	*/
	typedef TSphere3<float> Sphere3;

} // namespace BALL

#endif // BALL_MATHS_SPHERE3_H
