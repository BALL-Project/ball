// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: illuminatedLine.h,v 1.1.2.2 2006/03/30 14:26:57 amoll Exp $
//

#ifndef BALL_VIEW_PRIMITIVES_ILLUMINATEDLINE_H
#define BALL_VIEW_PRIMITIVES_ILLUMINATEDLINE_H

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
         
		/** Illuminated Line class.
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT IlluminatedLine
			: public GeometricObject
		{
			public:

			BALL_CREATE(IlluminatedLine)

			/// Default Constructor.
			IlluminatedLine()
				throw();

			/// Copy constructor with cloning facility.
			IlluminatedLine(const IlluminatedLine& line)
				throw();

			/** Destructor
			*/
			virtual ~IlluminatedLine()
				throw();

			/** Explicit default initialization.
			*/
			virtual void clear()
				throw();

			///
			void set(const IlluminatedLine& line)
				throw();

			/** Assignment operator.
			*/
			const IlluminatedLine& operator = (const IlluminatedLine& line)
				throw();

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
			virtual bool isValid() const
				throw();

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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			
			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;


			vector<Vector3> tangents;
			vector<Vector3> vertices;
			vector<ColorRGBA> colors;

		//@}
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIVES_LINE_H
