// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sphere.h,v 1.2 2003/08/29 10:53:55 amoll Exp $
//

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#define BALL_VIEW_PRIMITIV_SPHERE_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Sphere class.			
				An instance of Sphere represents an instance of the geometric representation "sphere".
				The class Sphere is derived from the classes GeometricObject
				and Sphere3. See these classes for further information concerning
				interface and additional methods. \par
		*/
		class Sphere
			: public GeometricObject,
				protected Sphere3
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			Sphere()
				throw();

			/** Copy constructor with cloning facility.

			*/
			Sphere(const Sphere& sphere)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this sphere.
			*/
			virtual ~Sphere()
				throw();

			/** Explicit default initialization.
					\see  GeometricObject::clear
					\see  Sphere3::clear
			*/
			virtual void clear()
				throw();

			//@}	
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const Sphere& sphere)
				throw();

			/** Assignment operator.
			*/
			const Sphere& operator = (const Sphere& sphere)
				throw();

			/** Copying with cloning facility.
			*/
			void get(Sphere& sphere) const
				throw();

			/** Swapping of sphere's.
					Swap the value of this sphere with the sphere <b> sphere</b>.
			*/
			void swap(Sphere& sphere)
				throw();

			///
			void setRadius(float new_radius)
				throw() { radius = new_radius;}

			///
			float getRadius() const
				throw() { return radius;}

			///
			void setPosition(const Vector3& v3)
				throw() { point_ptr_->set(v3);}

			///
			const Vector3& getPosition() const
				throw() { return *point_ptr_;}

			///
			void setPositionAddress(const Vector3& v3)
				throw() { point_ptr_ = (Vector3 *)&v3;}

			///
			Vector3* getPositionAddress()
				throw() { return point_ptr_;}

			///
			bool isDefaultPositionAddress()
				throw() { return point_ptr_ == &p;}

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this sphere.
					If the internal state of this sphere is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls GeometricObject::isValid.
					\return			bool <tt> true</tt> if the internal state of this sphere is correct (self-validated) and consistent, <tt> false</tt> otherwise
					\see        GeometricObject::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of this sphere to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls Sphere3::dump.
					\param   s output stream where to output the value of this sphere
					\param   depth the dumping depth
					\see     GeometricObject::dump
					\see     Sphere3::dump					
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
			
			protected:
			
			Vector3* point_ptr_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SPHERE_H
