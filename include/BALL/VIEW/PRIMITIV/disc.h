// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: disc.h,v 1.2 2003/08/26 08:05:10 oliver Exp $

#ifndef BALL_VIEW_PRIMITIV_DISC_H
#define BALL_VIEW_PRIMITIV_DISC_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Disc class.			
				An instance of Disc represents an instance of the geometric representation "Disc".
				The class Disc is derived from the classes GeometricObject
				and Disc3. See these classes for further information concerning
				interface and additional methods. \par
		*/
		class Disc
			: public GeometricObject,
				protected Circle3
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			Disc()
				throw();

			/** Copy constructor with cloning facility.
			*/
			Disc(const Disc& disc, bool deep = true)
				throw();

			Disc(const Circle3& circle)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this Disc.
			*/
			virtual ~Disc()
				throw();

			/** Explicit default initialization.
					\see  GeometricObject::clear
					\see  Disc3::clear
			*/
			virtual void clear()
				throw();

			//@}	
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const Disc& Disc, bool deep = true)
				throw();

			/** Assignment operator.
			*/
			const Disc& operator = (const Disc& Disc)
				throw();

			/** Copying with cloning facility.
			*/
			void get(Disc& Disc, bool deep = true) const
				throw();

			/** Swapping of Disc's.
					Swap the value of this Disc with the Disc <b> Disc</b>.
			*/
			void swap(Disc& Disc)
				throw();

			///
			void setCircle(const Circle3& circle)
				throw();

			const Circle3& getCircle() const
				throw() { return *this;}

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this Disc.
					If the internal state of this Disc is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls GeometricObject::isValid.
					\return			bool <tt> true</tt> if the internal state of this Disc is correct (self-validated) and consistent, <tt> false</tt> otherwise
					\see        GeometricObject::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of this Disc to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls Disc3::dump.
					\param   s output stream where to output the value of this Disc
					\param   depth the dumping depth
					\see     GeometricObject::dump
					\see     Disc3::dump					
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_Disc_H
