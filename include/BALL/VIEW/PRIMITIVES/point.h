// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: point.h,v 1.6 2003/11/29 15:57:44 amoll Exp $
//

#ifndef BALL_VIEW_PRIMITIVES_POINT_H
#define BALL_VIEW_PRIMITIVES_POINT_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATYPE_VERTEX1_H
#	include <BALL/VIEW/DATATYPE/vertex1.h>
#endif

namespace BALL
{
	namespace VIEW
	{
    
		/** Point class.
				An instance of Point represents an instance of the geometric representation "point".
				A point has the following properties. 
				  - color - the color of the point
					- vertex - the position of the point
				\par
				The class Point is derived from the classes GeometricObject
				and Vertex. See these classes for further information concerning
				interface and additional methods. \par
				\ingroup ViewPrimitives
		*/
		class Point
			: public GeometricObject,
				public Vertex
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The properties of this point are set to:
  				  - color - to the color black
		  			- vertex - to the vector (0,0,0)
					\par
			*/
			Point()
				throw();

			/** Copy constructor 
			*/
			Point(const Point& point)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			virtual ~Point()
				throw();

			/** Explicit default initialization.
					Calls GeometricObject::clear
					Calls Vertex::clear
			*/
			virtual void clear()
				throw();

			//@}

			/**	@name	Assignment methods */
			//@{

			/** Assignment.
			*/
			void set(const Point& point)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const Point& operator = (const Point& point)
				throw();

			/** Swapping of point's.
					Swap the value of this point with the point <b> point</b>.
					\param       point the point being swapped with this point 
			*/
			void swap(Point& point)
				throw();

			//@}
			/**	@name	debuggers and diagnostics */
			//@{

			/** Internal state and consistency self-validation.
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of this point to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls Vertex::dump.
					\param   s output stream where to output the value of this point
					\param   depth the dumping depth
					\see     GeometricObject::dump
					\see     Vertex::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}

		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIVES_POINT_H
