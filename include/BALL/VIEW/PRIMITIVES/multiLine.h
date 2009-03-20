// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: multiLine.h,v 1.1.4.1 2007-03-25 21:26:09 oliver Exp $
//

#ifndef BALL_VIEW_PRIMITIVES_MULTILINE_H
#define BALL_VIEW_PRIMITIVES_MULTILINE_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATYPE_VERTEX2_H
#	include <BALL/VIEW/DATATYPE/vertex2.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOREXTENSIONS_H
# include <BALL/VIEW/DATATYPE/colorExtensions.h>
#endif

namespace BALL
{
	namespace VIEW
	{
         
		/** A line with multiple vertices and colors.
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT MultiLine
			: public GeometricObject,
				public MultiColorExtension
		{
			public:

			BALL_CREATE(MultiLine)

			/// Default Constructor.
			MultiLine();

			/// Copy constructor with cloning facility.
			MultiLine(const MultiLine& line);

			/** Destructor
			*/
			virtual ~MultiLine();

			/** Explicit default initialization.
			*/
			virtual void clear();

			///
			void set(const MultiLine& line);

			/** Assignment operator.
			*/
			const MultiLine& operator = (const MultiLine& line);

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


			vector<Vector3> tangents;
			vector<Vector3> vertices;
		//@}
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIVES_MULTILINE_H
