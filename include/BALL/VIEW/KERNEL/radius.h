// $Id: radius.h,v 1.6 2001/07/09 17:57:02 sturm Exp $

#ifndef BALL_VIEW_KERNEL_RADIUS_H
#define BALL_VIEW_KERNEL_RADIUS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/** Radius class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/radius.h}\\ \\
				The class Radius is used as a base class for all geometric objects that
				needs a radius. It provides the derived class with methods for accessing
				that radius value.
				@memo    Radius class (BALL VIEW kernel framework)
				@author  $Author: sturm $
				@version $Revision: 1.6 $
				@date    $Date: 2001/07/09 17:57:02 $
		*/
		class Radius
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new radius.
					The value of {\em *this} radius is set to 1.0.
					@return      Radius new constructed radius
					@see         Real
			*/
			Radius()
				throw();

			/** Copy constructor.
					Construct new radius by copying the radius {\em r}.
					@param       r the radius to be copied
					@return      Radius new constructed radius copied from {\em r}
					@see         Real
			*/
			Radius(const Radius& r)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} radius.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Radius()
				throw();

			/** Explicit default initialization.
					Set the value of {\em *this} radius to 1.0.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}
		
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the radius {\em r} to {\em *this} radius.
					The value of {\em *this} radius is initialized to the value of 
					the radius {\em r}.\\
					@param       r the radius to be copied
			*/
			void set(const Radius& r)
				throw();

			/** Assignment operator.
					Assign the radius {\em r} to {\em *this} radius.
					Calls \Ref{set}.
					The value of {\em *this} radius is initialized to the value 
					of the radius {\em r}.\\
					@param       r the radius to be copied
					@return      Radius& constant reference of {\em *this} radius
					@see         set
			*/
			const Radius& operator = (const Radius& r)
				throw();

			/** Copying.
					Copy {\em *this} radius to the radius {\em r}.
					Calls \Ref{set}.
					The value of the radius {\em r} is initialized to the
					value of {\em *this} radius.\\
					@param       r the radius to be assigned to
					@see         set
			*/
			void get(Radius& r) const
				throw();

			/** Swapping of radius's.
					Swap the value of {\em *this} radius with the radius {\em r}.
					@param       r the radius being swapped with {\em *this} radius 
					@see         Radius
			*/
			void swap(Radius& r)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the value of {\em *this} radius.
					Change the value of {\em *this} radius to the value
					represented by the parameter {\em r}.
					@param       r the new radius of {\em *this} radius
					@see         getRadius
					@see         Real
			*/
			void setRadius(const Real r)
				throw();

			/** Mutable inspection of the value of {\em *this} radius.
					Access the mutual reference of the value of {\em *this} radius.
					@return      Real& mutable reference to the value of {\em *this} radius
					@see         setRadius
					@see         Real
			*/
			Real& getRadius()
				throw();

			/** Non-mutable inspection of the value of {\em *this} radius.
					For further information see \Ref{getRadius}.
			*/
			const Real& getRadius() const
				throw();

			/** Inspection of the value of {\em *this} radius.
					Access the value of {\em *this} radius by using \Ref{Real}.
					@param       r the real receiving the value of {\em *this} radius
					@see         setRadius
					@see         Real
			*/
			void getRadius(Real& r) const
				throw();
			//@}
			
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current value of {\em *this} radius to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the value of {\em *this} radius
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent radius data from the input stream {\em s} and 
				  restore the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal value of {\em *this} radius
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent radius data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal value of {\em *this} radius
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}


			private:

			/* radius */
			Real radius_;

		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/radius.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_RADIUS_H
