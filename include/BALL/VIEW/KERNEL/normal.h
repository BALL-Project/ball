// $Id: normal.h,v 1.8 2001/07/16 14:49:32 amoll Exp $

#ifndef BALL_VIEW_KERNEL_NORMAL_H
#define BALL_VIEW_KERNEL_NORMAL_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	namespace VIEW
	{

		/** NormalVector class.
				The class NormalVector is used as a base class for all geometric objects that
				needs a normal vector. It provides the derived class with methods for accessing
				that normal vector. \\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/normal.h}
		*/
		class NormalVector
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new normalVector.
					The normal vector of {\em *this} normalVector is set to (1.0, 0.0, 0.0).
					@return      NormalVector new constructed normalVector
					@see         Vector3
			*/
			NormalVector()
				throw();

			/** Copy constructor.
					Construct new normalVector by copying the normalVector {\em n}.
					@param       n the normalVector to be copied
					@return      NormalVector new constructed normalVector copied from {\em n}
					@see         Vector3
			*/
			NormalVector(const NormalVector& n)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} normalVector.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~NormalVector()
				throw();

			/** Explicit default initialization.
					Set the normal vector of {\em *this} normalVector to the vector (1.0, 0.0, 0.0).
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
					Assign the normalVector {\em n} to {\em *this} normalVector.
					The vector of {\em *this} normalVector is initialized to the vector of 
					the normalVector {\em n}.\\
					@param       n the normalVector to be copied
					@see         Vector3
			*/
			void set(const NormalVector& n)
				throw();

			/** Assignment operator.
					Assign the normalVector {\em n} to {\em *this} normalVector.
					Calls \Ref{set}.
					The vector of {\em *this} normalVector is initialized to the vector 
					of the normalVector {\em n}.\\
					@param       n the normalVector to be copied
					@return      NormalVector& constant reference of {\em *this} normalVector
					@see         set
			*/
			const NormalVector& operator = (const NormalVector& n)
				throw();

			/** Copying.
					Copy {\em *this} normalVector to the normalVector {\em n}.
					Calls \Ref{set}.
					The vector of the normalVector {\em n} is initialized to the
					vector of {\em *this} normalVector.\\
					@param       n the normalVector to be assigned to
					@see         set
			*/
			void get(NormalVector& n) const
				throw();

			/** Swapping of normalVectors.
					Swap the vector of {\em *this} normalVector with the normalVector
					{\em n}.
					@param       n the normalVector being swapped with {\em *this} normalVector 
					@see         NormalVector
			*/
			void swap(NormalVector& n)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the vector of {\em *this} normalVector.
					Change the vector of {\em *this} normalVector to the vector
					represented by the parameter {\em n}.
					@param       n the new vector of {\em *this} normalVector
					@see         getNormalVector
					@see         Vector3
			*/
			void setNormalVector(const Vector3& n)
				throw();

			/** Change the vector of {\em *this} normalVector.
					Change the vector of {\em *this} normalVector to the vector represented by the
					parameters {\em x}, {\em y} and {\em z}.
					@param       x the x component of the new vector of {\em *this} normalVector
					@param       y the y component of the new vector of {\em *this} normalVector
					@param       z the z component of the new vector of {\em *this} normalVector
					@see         getNormalVector
					@see         Vector3
			*/
			void setNormalVector(const Real x, const Real y, const Real z)
				throw();

			/** Mutable inspection of the vector of {\em *this} normalVector.
					Access the mutual reference of the vector of {\em *this} normalVector.
					@return      Vector3& mutable reference to the vector of {\em *this} normalVector
					@see         setNormalVector
					@see         Vector3
			*/
			Vector3& getNormalVector()
				throw();

			/** Non-mutable inspection of the vector of {\em *this} normalVector.
			For further information see \Ref{getNormalVector}.
			*/
			const Vector3& getNormalVector() const
				throw();

			/** Inspection of the vector of {\em *this} normalVector.
					Access the vector of {\em *this} normalVector by using \Ref{Vector3}.
					@param       n the vector receiving the vector of {\em *this} normalVector
					@see         setNormalVector
					@see         Vector3
			*/
			void getNormalVector(Vector3& n) const
				throw();

			/** Inspection of the components of the vector of {\em *this} normalVector.
					Access the components of the vector of {\em *this} normalVector by using \Ref{Real}.
					@param       x the x component of the vector of {\em *this} normalVector
					@param       y the y component of the vector of {\em *this} normalVector
					@param       z the z component of the vector of {\em *this} normalVector
					@see         setNormalVector
					@see         Real
			*/
			void getNormalVector(Real& x, Real& y, Real& z) const
				throw();	
			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} normalVector.
					If the internal state of {\em *this} normalVector is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {Vector3::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} normalVector is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        Vector3::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} normalVector to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} normalVector
					@param   depth the dumping depth
					@see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent normalVector data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} normalVector
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent normalVector data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} normalVector
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}
			
			
			private:

			/* normal vector */
			Vector3 normal_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/normal.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_NORMAL_H
