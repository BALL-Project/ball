// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: line.h,v 1.9.16.1 2007-03-25 21:26:09 oliver Exp $
//

#ifndef BALL_VIEW_PRIMITIVES_LINE_H
#define BALL_VIEW_PRIMITIVES_LINE_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATYPE_VERTEX2_H
#	include <BALL/VIEW/DATATYPE/vertex2.h>
#endif

namespace BALL
{
	namespace VIEW
	{
         
		/** Line class.
				An instance of Line represents an instance of the geometric representation "line".
				A line has the following properties. 
				  - color - the color of the line
					- vertex1 - the first vertex of the line
					- vertex2 - the second vertex of the line
				\par
				The class Line is derived from the classes GeometricObject 
				and Vertex2. See these classes for further information concerning
				interface and additional methods. \par
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT Line
			: public GeometricObject,
				public Vertex2
		{
			public:

			BALL_CREATE(Line)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new line.
					The properties of this line are set to:
  				  - color - to the color black
		  			- vertex1 - to the vector (0,0,0)
		  			- vertex2 - to the vector (0,0,0)
					\par
					\return      Line new constructed line
					\see         GeometricObject
					\see         Vertex2
					*/
			Line();

			/** Copy constructor with cloning facility.
					\see         GeometricObject
					\see         Vertex2
			*/
			Line(const Line& line);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor
			*/
			virtual ~Line();

			/** Explicit default initialization.
					Calls GeometricObject::clear.
					Calls Vertex2::clear.
					\see  GeometricObject::clear
					\see  Vertex2::clear
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods */
			//@{

			/** Assign the line <b> line</b> to this line.
					the line <b> line</b>.\par
			*/
			void set(const Line& line);

			/** Assignment operator.
					Calls set.
			*/
			const Line& operator = (const Line& line);

			/** Swapping of line's.
					Swap the value of this line with the line <b> line</b>.
					\param       line the line being swapped with this line 
					\see         Line
			*/
			void swap(Line& line);

			//@}
			/**	@name	debuggers and diagnostics */
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this line.
					If the internal state of this line is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls GeometricObject::isValid.
					Calls Vertex2::isValid.
					\return			bool <tt> true</tt> if the internal state of this line is correct 
											(self-validated) and consistent, <tt> false</tt> otherwise
					\see        GeometricObject::isValid
					\see        Vertex2::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current value of this line to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls Vertex2::dump.
					\param   s output stream where to output the value of this line
					\param   depth the dumping depth
					\see        GeometricObject::dump
					\see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			
			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;

		//@}
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIVES_LINE_H
