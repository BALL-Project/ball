// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.h,v 1.8.16.1 2007-03-25 21:26:11 oliver Exp $
//

#ifndef BALL_VIEW_PRIMITIV_SIMPLEBOX_H
#define BALL_VIEW_PRIMITIV_SIMPLEBOX_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MATHS_SIMPLEBOX3_H
#	include <BALL/MATHS/simpleBox3.h>
#endif

namespace BALL
{
	namespace VIEW
	{

      
		/** SimpleBox class.	
				An instance of this class represents an instance of the geometric representation of a simple box.
				A simpleBox is a box whose sides are parallel to the coordinate axes.
				The class SimpleBox is derived from the classes GeometricObject
				and SimpleBox3. See these classes for further information concerning
				interface and additional methods. \par
				\ingroup ViewPrimitives
		*/
// Export doesn't work for some reason...
		class BALL_VIEW_EXPORT SimpleBox
// 		class SimpleBox
			: public SimpleBox3,
			 	public GeometricObject
		{
			public:

			BALL_CREATE(SimpleBox)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The properties of this SimpleBox are set to:
  				  - color - to the color black
					\par
			*/
			SimpleBox();

			/** Copy constructor with cloning facility.
			*/
			SimpleBox(const SimpleBox& box);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			virtual ~SimpleBox();

			/** Explicit default initialization.
					Calls GeometricObject::clear
					Calls SimpleBox3::clear
			*/
			virtual void clear();

			//@}

			/**	@name	Assignment methods */
			//@{

			/** Assignment.
			*/
			void set(const SimpleBox& box);

			/** Assignment operator.
			*/
			const SimpleBox& operator = (const SimpleBox& box);

			//@}
			/**	@name	debuggers and diagnostics */
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this SimpleBox.
					If the internal state of this SimpleBox is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls GeometricObject::isValid.
					Calls SimpleBox3::isValid.
					\see        GeometricObject::isValid
					\see        SimpleBox3::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current value of this SimpleBox to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls SimpleBox3::dump.
					\param   s output stream where to output the value of this SimpleBox
					\param   depth the dumping depth
					\see     GeometricObject::dump
					\see     SimpleBox::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const;
			
			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;

			//@}
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SIMPLEBOX_H
