// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT2D.h,v 1.10 2004/05/18 11:21:46 amoll Exp $
//

#ifndef BALL_MATHS_FFT2D_H
#define BALL_MATHS_FFT2D_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MATHS_COMPLEX_H
# include <BALL/MATHS/complex.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

#include <math.h>
#include <fftw.h>

namespace BALL
{
	/** A class to perform Fast Fourier Transforms and inverse Fast Fourier Transforms
			on regularly spaced two dimensional data.  \par
			This class makes use of the freely available library <b>FFTW</b>, which can be
			found at http://www.fftw.org
			coordinate system can be handled automatically. The normaliztion is chosen
			symmetrically.
			 \par
			S/FFT2D.h
	 \ingroup FFT
	 */

	class FFT2D 
		: public TRegularData2D<FFTW_COMPLEX>
	{
		public:

      BALL_CREATE(FFT2D)

      /**  @name Constructors and Destructors
       */
      //@{
 
      /// Default constructor
      FFT2D()
        throw();

			/// Copy constructor
			FFT2D(const FFT2D &data)
				throw();

			/** Detailed constructor.  \par
			 		@param ldnX The binary logarithm of the number of grid points in X direction (we use the logarithm to
										 ensure that the number of points is a power of two, which is important for
										 the FFT)
					@param ldnY The binary logarithm of the number of grid points in Y direction
					@param stepPhysX The step width in X direction in physical space
					@param stepPhysY The step width in Y direction in physical space
					@param origin The origin of the coordinate system
					@param inFourierSpace Flag to decide whether the data is assumed to be in physical or fourier
																space
			 */
			FFT2D(Size ldnX, Size ldnY, double stepPhysX=1., double stepPhysY=1., Vector2 origin=Vector2(0.,0.), bool inFourierSpace=false)
				throw();
      
			/// Destructor
			virtual ~FFT2D()
				throw();
			
			//@}

			/** @name Assignment
			 */
			//@{

			/// Assignment operator
			const FFT2D& operator = (const FFT2D& FFT2D)
				throw();
			
			/** Clear the contents.
			 */
			virtual void clear()
				throw();
			
			/** Clear the contents and reset all attributes.
			 */
			virtual void destroy()
				throw();

			//@}

			/** @name Predicates
			 */
			//@{

			/** Equality operator.
			 */
			bool operator == (const FFT2D& FFT2D) const
				throw();
			//@}
			
			// @name Accessors
			
			/** Perform a single fast fourier transform on the data.
			 */
			void doFFT()
				throw();

			/** Perform a single inverse fourier transform on the data.
			 */
			void doiFFT()
				throw();

			/** Translate the origin in physical space about {\em trans_origin},
					i.e. the new origin will be located at the former position {\em trans_origin}.
					If the result is out of bounds, the function does nothing and
					returns <b>  false </b>.
			 */
			bool translate(const Vector2& trans_origin)
				throw();

			/** Set the step width in physical space to {\em new_width_x, new_width_y}.
				The step width in fourier space is automatically adjusted
				accordingly. {\em new_width_x and new_width_y} must be positive, otherwise
				the function does nothing and retuns <b>  false </b>.
			 */
			bool setPhysStepWidth(double new_width_x, double new_width_y)
				throw();

			/** Returns the step width in physical space in X direction.
			 */
			double getPhysStepWidthX() const
				throw();

			/** Returns the step width in physical space in Y direction.
			 */
			double getPhysStepWidthY() const
				throw();

			/** Returns the step width in fourier space in X direction.
			 */
			double getFourierStepWidthX() const
				throw();

			/** Returns the step width in fourier space in Y direction.
			 */
			double getFourierStepWidthY() const
				throw();

			/** Returns the minimal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMinX() const
				throw();

			/** Returns the minimal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMinY() const
				throw();

			/** Returns the maximal position of the grid in physical space in X direction.
			 */
			double getPhysSpaceMaxX() const
				throw();

			/** Returns the maximal position of the grid in physical space in Y direction.
			 */
			double getPhysSpaceMaxY() const
				throw();

			/** Returns the minimal position of the grid in fourier space in X direction.
			 */
			double getFourierSpaceMinX() const
				throw();

			/** Returns the minimal position of the grid in fourier space in Y direction.
			 */
			double getFourierSpaceMinY() const
				throw();

			/** Returns the maximal position of the grid in fourier space in X direction.
			 */
			double getFourierSpaceMaxX() const
				throw();

			/** Returns the maximal position of the grid in fourier space in Y direction.
			 */
			double getFourierSpaceMaxY() const
				throw();

			/** Returns the data at the grid position closest to <b>  pos </b>,
				and automatically includes
				the correct phase factor and (symmetric) normalization.
			 */
			Complex getData(const Vector2& pos) const
				throw(Exception::OutOfGrid);

			/** Returns the data at point <b>pos</b>. If <b>pos</b> is not a 
			 		point on the grid, the data is linearly interpolated.
					This method automatically includes the correct phase factor
					and (symmetric) normalization.
				*/
			Complex getInterpolatedValue(const Vector2& pos) const
				throw(Exception::OutOfGrid);

			/** Sets the data point at the grid position closest to <b>  pos </b>
				to the value <b>  val </b>, and -- if called in fourier space --
				automatically includes the correct phase factor and 
				(symmetric) normalization.
			 */
			void setData(const Vector2& pos, Complex val)
				throw(Exception::OutOfGrid);

			/** Access the data at the grid position closest to <b>  pos </b>.
				This function returns the "raw" data at that position.
			 */
			FFTW_COMPLEX& operator[](const Vector2& pos)
				throw(Exception::OutOfGrid);

			/** Access the data at the grid position closest to <b>  pos </b>.
			 		This function returns the "raw" data at that position.
				*/
			const FFTW_COMPLEX& operator[](const Vector2& pos) const
				throw(Exception::OutOfGrid);
			
			/** This computes the phase factor in fourier space that results
				if the origin of the coordinate system in physical space
				is not in the "lower left corner".
			 */
			Complex phase(const Vector2& pos) const
				throw();

		protected:
			Size lengthX_, lengthY_;
			bool inFourierSpace_;
			Size numPhysToFourier_;
			Size numFourierToPhys_;
			Vector2 origin_;
			double stepPhysX_, stepPhysY_;
			double stepFourierX_, stepFourierY_;
      Vector2 minPhys_, maxPhys_;
      Vector2 minFourier_, maxFourier_;
			fftwnd_plan planForward_;
			fftwnd_plan planBackward_;
	};
}
			 

#endif // BALL_MATHS_FFT2D_H
