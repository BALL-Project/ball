// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clippingPlane.h,v 1.3.18.1 2007/03/25 21:25:58 oliver Exp $
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
		/** ClippingPlane 
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_VIEW_EXPORT ClippingPlane
		{
			public:

			BALL_CREATE(ClippingPlane)

			ClippingPlane();

			///
			ClippingPlane(const ClippingPlane& plane);

			/** Destructor
			*/
			virtual ~ClippingPlane();

			///
			const ClippingPlane& operator = (const ClippingPlane& plane);
			
			///
			void clear();

			///
			bool operator == (const ClippingPlane& plane) const;

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

			///
			static ColorRGBA& getCappingColor() { return capping_color_;}

			///
			bool cappingEnabled() const { return cap_;}

			///
			void setCappingEnabled(bool state) { cap_ = state;}

			protected:

			HashSet<const Representation*> reps_;
			Vector3 normal_;
			Vector3 point_;
			bool active_;
			bool hidden_;
			bool cap_;
			static ColorRGBA capping_color_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIPPING_PLANE_H
