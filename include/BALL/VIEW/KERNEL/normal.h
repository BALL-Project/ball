// $Id: normal.h,v 1.4 2001/02/04 15:58:21 hekl Exp $

#ifndef BALL_VIEW_KERNEL_NORMAL_H
#define BALL_VIEW_KERNEL_NORMAL_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	namespace VIEW
	{

		/** NormalVector class.
				
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/normal.h}
				\\
				The class NormalVector is used as a base class for all geometric objects that
				needs a normal vector. It provides the derived class with methods for accessing
				that normal vector.
				
				@memo    NormalVector class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/02/04 15:58:21 $
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

					@return      NormalVector - new constructed normalVector
					@see         Vector3::Vector3
			*/
			NormalVector();

			/** Copy constructor.
					Construct new normalVector by copying the normalVector {\em n}.

					@param       n the normalVector to be copied
					@return      NormalVector - new constructed normalVector copied from {\em n}
					@see         Vector3::Vector3
			*/
			NormalVector(const NormalVector& n);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} normalVector.
					Calls \Ref{NormalVector::destroy}.
					@see         NormalVector::destroy
			*/
			virtual ~NormalVector();

			/** Explicit default initialization.
					Set the normal vector of {\em *this} normalVector to the vector (1.0, 0.0, 0.0).
			*/
			virtual void clear();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy();
			//@}
		
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the normalVector {\em n} to {\em *this} normalVector.
					The vector of {\em *this} normalVector is initialized to the vector of 
					the normalVector {\em n}.\\

					@param       n the normalVector to be copied
					@see         Vector3::Vector3
			*/
			void set(const NormalVector& n);

			/** Assignment operator.
					Assign the normalVector {\em n} to {\em *this} normalVector.
					Calls \Ref{NormalVector::set}.
					The vector of {\em *this} normalVector is initialized to the vector 
					of the normalVector {\em n}.\\

					@param       n the normalVector to be copied
					@return      NormalVector& - {\em *this} normalVector
					@see         NormalVector::set
			*/
			NormalVector& operator = (const NormalVector& n);

			/** Copying.
					Copy {\em *this} normalVector to the normalVector {\em n}.
					Calls \Ref{NormalVector::set}.
					The vector of the normalVector {\em n} is initialized to the
					vector of {\em *this} normalVector.\\

					@param       n the normalVector to be assigned to
					@see         NormalVector::set
			*/
			void get(NormalVector& n) const;

			/** Swapping of normalVectors.
					Swap the vector of {\em *this} normalVector with the normalVector
					{\em n}.

					@param       n the normalVector being swapped with {\em *this} normalVector 
					@see         NormalVector::NormalVector
			*/
			void swap(NormalVector& n);
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the vector of {\em *this} normalVector.
					Change the vector of {\em *this} normalVector to the vector
					represented by the parameter {\em n}.

					@param       n the new vector of {\em *this} normalVector
					@see         NormalVector::getNormalVector
					@see         Vector3::Vector3
			*/
			void setNormalVector(const Vector3& n);

			/** Change the vector of {\em *this} normalVector.
					Change the vector of {\em *this} normalVector to the vector represented by the
					parameters {\em x}, {\em y} and {\em z}.

					@param       x the x component of the new vector of {\em *this} normalVector
					@param       y the y component of the new vector of {\em *this} normalVector
					@param       z the z component of the new vector of {\em *this} normalVector
					@see         NormalVector::getNormalVector
					@see         Vector3::Vector3
			*/
			void setNormalVector(const Real x, const Real y, const Real z);

			/** Mutable inspection of the vector of {\em *this} normalVector.
					Access the mutual reference of the vector of {\em *this} normalVector.
					
					@return      Vector3& - mutable reference to the vector of {\em *this} normalVector
					@see         NormalVector::setNormalVector
					@see         Vector3::Vector3
			*/
			Vector3& getNormalVector();

			/** Non-mutable inspection of the vector of {\em *this} normalVector.
					Access the constant reference of the vector of {\em *this} normalVector.
					
					@return      Vector3& - constant reference to the vector of {\em *this} normalVector
					@see         NormalVector::setNormalVector
					@see         Vector3::Vector3
			*/
			const Vector3& getNormalVector() const;

			/** Inspection of the vector of {\em *this} normalVector.
					Access the vector of {\em *this} normalVector by using \Ref{Vector3}.
					
					@param       n the vector receiving the vector of {\em *this} normalVector
					@see         NormalVector::setNormalVector
					@see         Vector3::Vector3
			*/
			void getNormalVector(Vector3& n) const;

			/** Inspection of the components of the vector of {\em *this} normalVector.
					Access the components of the vector of {\em *this} normalVector by using \Ref{Real}.
					
					@param       x the x component of the vector of {\em *this} normalVector
					@param       y the y component of the vector of {\em *this} normalVector
					@param       z the z component of the vector of {\em *this} normalVector
					@see         NormalVector::setNormalVector
					@see         Real::Real
			*/
			void getNormalVector(Real& x, Real& y, Real& z) const;	
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

					@return			bool -
											{\tt true} if the internal state of {\em *this} normalVector is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        Vector3::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state of {\em *this} normalVector to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the state of {\em *this} normalVector
					@param   depth the dumping depth
					@see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			 Read persistent normalVector data from the input stream {\em s} and 
				 restore the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s input stream from where to restore the internal state of {\em *this} normalVector
				 @exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and state storage.
  			 Write persistent normalVector data to the output stream {\em s} and 
				 store the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s output stream to where to store the internal state of {\em *this} normalVector
				 @exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;
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
