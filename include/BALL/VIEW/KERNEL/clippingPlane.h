// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.h,v 1.1.2.1 2005/05/09 15:37:20 amoll Exp $
//

#ifndef  BALL_VIEW_KERNEL_CLIPPING_PLANE_H
#define  BALL_VIEW_KERNEL_CLIPPING_PLANE_H

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef  BALL_VIEW_KERNEL_REPRESENTATION_H
# include <BALL/VIEW/KERNEL/representation.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Representation
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_EXPORT ClippingPlane
			: public Representation
		{
			public:

			BALL_CREATE(ClippingPlane)

			/** @name Enums
			 */
			//@{
			
			ClippingPlane()
				throw();

			///
			ClippingPlane(const ClippingPlane& plane)
				throw();

			/** Destructor
			*/
			virtual ~ClippingPlane()
				throw();

			//@}
			/**	@name	Predicats and Accessors
			*/	
			//@{
			
			///
			const ClippingPlane& operator = (const ClippingPlane& plane)
				throw();
			
			///
			void clear()
				throw();

			///
			HashSet<Representation*>& getRepresentations() { return reps_;}

			///
			float getX() const { return x_;}

			///
			float getY() const { return y_;}

			///
			float getZ() const { return z_;}

			///
			float getD() const { return d_;}

			///
			void setX(float x) { x_ = x;}

			///
			void setY(float y) { y_ = y;}

			///
			void setZ(float z) { z_ = z;}

			///
			void setD(float d) { d_ = d;}


			//@}

			protected:

			HashSet<Representation*> reps_;
			float x_, y_, z_, d_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIPPING_PLANE_H
