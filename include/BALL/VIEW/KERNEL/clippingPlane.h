// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.h,v 1.1.2.8 2005/10/11 14:39:18 amoll Exp $
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
		class BALL_VIEW_EXPORT ClippingPlane
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
			HashSet<const Representation*>& getRepresentations() { return reps_;}

			///
			const Vector3& getNormal() const { return normal_;}

			///
			void setNormal(const Vector3& normal) { normal_ = normal;}

			///
			bool isActive() const { return active_;}

			///
			void setActive(bool state) { active_ = state;}

			///
			bool isHidden() const { return hidden_;}

			///
			void setHidden(bool hidden) { hidden_ = hidden;}

			///
			float getDistance() const;

			///
			const Vector3& getPoint() const { return point_;}

			///
			void setPoint(const Vector3& v) { point_ = v;}

			///
			void flip() { normal_ = -normal_;}

			//@}

			protected:

			HashSet<const Representation*> reps_;
			Vector3 normal_;
			Vector3 point_;
			bool active_;
			bool hidden_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIPPING_PLANE_H
