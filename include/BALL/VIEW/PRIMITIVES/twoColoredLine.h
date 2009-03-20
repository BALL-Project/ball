// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: twoColoredLine.h,v 1.10.16.1 2007-03-25 21:26:12 oliver Exp $
//

#ifndef BALL_VIEW_VIEW_PRIMITIVES_TWOCOLOREDLINE_H
#define BALL_VIEW_VIEW_PRIMITIVES_TWOCOLOREDLINE_H

#ifndef BALL_VIEW_DATATYPE_COLOREXTENSIONS_H
#	include <BALL/VIEW/DATATYPE/colorExtensions.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATPYE_VERTEX2_H
#	include <BALL/VIEW/DATATYPE/vertex2.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** TwoColoredLine class.
				An instance of TwoColoredLine represents an instance of the geometric
				representation of a two colored "line".
				A twoColoredLine has the following properties. 
				  - color1 - the color for the first vertex of the line
				  - color2 - the color for the second vertex of the line
					- vertex1 - the first vertex of the line
					- vertex2 - the second vertex of the line
				\par
				The class TwoColoredLine is a line that is half colored in the color 1 and
				in color 2. The line is exactly divided in the middle. One half (starting
				from vertex1 til middle vertex) is colored by the color1 and the other half 
				(starting from middle vertex to vertex2) is colored by color2.
				The class TwoColoredLine is derived from the classes GeometricObject,
				ColorExtension2	and Vertex2.
				See these classes for further information concerning
				interface and additional methods. \par
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT TwoColoredLine
			: public GeometricObject,
				public ColorExtension2,
				public Vertex2
		{
			public:

			BALL_CREATE(TwoColoredLine)

			/**	@name	Constructors */	
			//@{

			/** Default Constructor.
					Construct new twoColoredLine.
					The properties of this twoColoredLine are set to:
  				  - color1 - set to the color black
						- color2 - set to the color black
		  			- vertex1 - set to the vector (0,0,0)
		  			- vertex2 - set to the vector (0,0,0)
					\par
					\return      TwoColoredLine new constructed twoColoredLine
					\see         GeometricObject
					\see         ColorExtension2
					\see         Vertex2
			*/
			TwoColoredLine();

			/** Copy constructor with cloning facility.
					\see         GeometricObject
					\see         ColorExtension2
					\see         Vertex2
			*/
			TwoColoredLine(const TwoColoredLine& two_colored_line);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this twoColoredLine.
			*/
			virtual ~TwoColoredLine();

			/** Explicit default initialization.
					Calls GeometricObject::clear.
					Calls ColorExtension2::clear.
					Calls Vertex2::clear.
					\see  GeometricObject::clear
					\see  ColorExtension2::clear
					\see  Vertex2::clear
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods */
			//@{

			/** Assignment.
					Calls GeometricObject::set.
					Calls ColorExtension2::set.
					Calls Vertex2::set.
					\see         GeometricObject::set
					\see         ColorExtension2::set
					\see         Vertex2::set
			*/
			void set(const TwoColoredLine& two_colored_line);

			/** Assignment operator.
					Calls set.
			*/
			const TwoColoredLine& operator = (const TwoColoredLine& two_colored_line);

			/** Swapping of two_colored_line's.
			*/
			void swap(TwoColoredLine& two_colored_line);

			//@}
			/**	@name	Accessors: inspectors and mutators */
			//@{
			
			/** Inspection of the middle vector.
					Access the geometric middle vertex between vertex1 and vertex2 of
					this twoColoredLine.
					\return. Vector3 the middle vertex between vertex1 and vertex 2.
					\see     Vector3
			*/
			Vector3 getMiddleVertex() const;

			//@}
			/**	@name	debuggers and diagnostics */
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure
					consistencies	of this twoColoredLine.
					If the internal state of this twoColoredLine is correct 
					(self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls {GeometricObject::isValid}.
					\return			bool <tt> true</tt> if the internal state of this twoColoredLine is correct 
					\see        GeometricObject::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current value of this twoColoredLine to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls ColorExtension2::dump.
					Calls Vertex2::dump.
					\param   s output stream where to output the value of this twoColoredLine
					\param   depth the dumping depth
					\see        GeometricObject::dump
					\see        ColorExtension2::dump
					\see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			
			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;

			//@}
		};

	#ifndef BALL_NO_INLINE_FUNCTIONS
	# include <BALL/VIEW/PRIMITIVES/twoColoredLine.iC>
	#endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_VIEW_PRIMITIVES_TWOCOLOREDLINE_H
