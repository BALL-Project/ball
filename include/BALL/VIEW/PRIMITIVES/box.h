// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: box.h,v 1.9.16.1 2007-03-25 21:26:07 oliver Exp $
//

#ifndef BALL_VIEW_PRIMITIV_BOX_H
#define BALL_VIEW_PRIMITIV_BOX_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MATHS_BOX3_H
#	include <BALL/MATHS/box3.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Box class.	
				An instance of this class represents an instance of the geometric representation of a threedimensional box.
				The class Box is derived from the classes GeometricObject
				and Box3. See these classes for further information concerning
				interface and additional methods. \par
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT Box
			: public GeometricObject,
				public Box3
		{
			public:

			BALL_CREATE(Box)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new Box.
					The properties of this Box are set to:
  				  - color - to the color black
						- width, depth, height - to one
						- right_vector to 0, 1, 0
						- heigth_vector to 1, 0, 0
						- depth_vector to 0, 0, 1
					\par
					\return      Box new constructed Box
			*/
			Box();

			/** Copy constructor with cloning facility.
			*/
			Box(const Box& box);

			Box(const Vector3& point, 
					const Vector3& right_vector,
					const Vector3& height_vector,
					float depth);

			Box(const Vector3& point, 
					const Vector3& right_vector  = Vector3(0, 1, 0),
					const Vector3& height_vector = Vector3(1, 0, 0),
					const Vector3& depth_vector = Vector3(0, 0, 1));
			
			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			virtual ~Box();

			/** Explicit default initialization.
					Calls GeometricObject::clear
					Calls Box3::clear
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const Box& box);

			/** Assignment operator.
			*/
			const Box& operator = (const Box& box);

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this Box.
					If the internal state of this Box is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls GeometricObject::isValid.
					Calls Box3::isValid.
					\see        GeometricObject::isValid
					\see        Box3::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current value of this Box to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls Box3::dump.
					\param   s output stream where to output the value of this Box
					\param   depth the dumping depth
					\see     GeometricObject::dump
					\see     Box::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const;
			
			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;
			//@}
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_BOX_H
