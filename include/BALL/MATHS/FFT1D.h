// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT1D.h,v 1.5 2003/02/25 15:53:20 sturm Exp $

#ifndef BALL_MATHS_FFT1D_H
#define BALL_MATHS_FFT1D_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MATHS_COMPLEX_H
# include <BALL/MATHS/complex.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif

#include <math.h>
#include <fftw.h>

namespace BALL
{
    /** \ingroup FFT
     *  @{
     */
	/** A class to perform Fast Fourier Transforms and inverse Fast Fourier Transforms
			on regularly spaced one dimensional data.  \par
			This class makes use of the freely available library <b>FFTW</b>, which can be
			found at http://www.fftw.org
			coordinate system can be handled automatically. The normaliztion is chosen
			symmetrically.
			 \par
			<b>Definition:</b> BALL/MATHS/FFT1D.h
	 */

	class FFT1D : public TRegularData1D<FFTW_COMPLEX>
	{
		public:

      BALL_CREATE(FFT1D)

      /**  @name Constructors and Destructors
       */
      //@{
 
      /// Default constructor
      FFT1D()
        throw();

			/// Copy constructor
			FFT1D(const FFT1D &data)
				throw();

			/** Detailed constructor.  \par
			 		@param ldn The binary logarithm of the number of grid points (we use the logarithm to
										 ensure that the number of points is a power of two, which is important for
										 the FFT)
					@param stepPhys The step width in physical space
					@param origin The origin of the coordinate system
					@param inFourierSpace Flag to decide whether the data is assumed to be in physical or fourier
																space
			 */
			FFT1D(Size ldn, double stepPhys=1., double origin=0., bool inFourierSpace=false)
				throw();
      
			/// Destructor
			virtual ~FFT1D()
				throw();
			
			//@}

			/** @name Assignment
			 */
			//@{

			/// Assignment operator
			const FFT1D& operator = (const FFT1D& fft1d)
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
			bool operator == (const FFT1D& fft1d) const
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

			/** Translate the origin in physical space about {\em transOrigin}.
				If the result is out of bounds, the function does nothing and
				returns {\em false}.
			 */
			bool translate(double trans_origin)
				throw();

			/** Set the step width in physical space to {\em newWidth}.
				The step width in fourier space is automatically adjusted
				accordingly. {\em newWidth} must be positive, otherwise
				the function does nothing and retuns {\em false}.
			 */
			bool setPhysStepWidth(double new_width)
				throw();

			/** Returns the step width in physical space.
			 */
			double getPhysStepWidth() const
				throw();

			/** Returns the step width in fourier space.
			 */
			double getFourierStepWidth() const
				throw();

			/** Returns the minimal position of the grid in physical space.
			 */
			double getPhysSpaceMin() const
				throw();

			/** Returns the maximal position of the grid in physical space.
			 */
			double getPhysSpaceMax() const
				throw();

			/** Returns the minimal position of the grid in fourier space.
			 */
			double getFourierSpaceMin() const
				throw();

			/** Returns the maximal position of the grid in fourier space.
			 */
			double getFourierSpaceMax() const
				throw();

			/** Returns the data at the grid position closest to {\em pos},
				and automatically includes
				the correct phase factor and normalization.
			 */
			Complex getData(const double pos) const
				throw(Exception::OutOfGrid);

			/** Returns the data at point <b>pos</b>. If <b>pos</b> is not a 
			 		point on the grid, the data is linearly interpolated.
					This method automatically includes the correct phase factor
					and normalization.
				*/
			Complex getInterpolatedValue(const double pos) const
				throw(Exception::OutOfGrid);

			/** Sets the data point at the grid position closest to {\em pos}
				to the value {\em val}, and -- if called in fourier space --
				automatically includes the correct phase factor and normalization.
			 */
			void setData(double pos, Complex val)
				throw(Exception::OutOfGrid);

			/** Access the data at the grid position closest to {\em pos}.
				This function returns the "raw" data at that position.
			 */
			FFTW_COMPLEX& operator[](const double pos)
				throw(Exception::OutOfGrid);

			/** Access the data at the grid position closest to {\em pos}.
			 		This function returns the "raw" data at that position.
				*/
			const FFTW_COMPLEX& operator[](const double pos) const
				throw(Exception::OutOfGrid);
			
			/** This computes the phase factor in fourier space that results
				if the origin of the coordinate system in physical space
				is not in the "lower left corner".
			 */
			Complex phase(const double pos) const
				throw();

		protected:
			Size length_;
			bool inFourierSpace_;
			Size numPhysToFourier_;
			Size numFourierToPhys_;
			double origin_;
			double stepPhys_;
			double stepFourier_;
      double minPhys_, maxPhys_;
      double minFourier_, maxFourier_;
			fftw_plan planForward_;
			fftw_plan planBackward_;
	};
  /** @} */
}
#endif // BALL_MATHS_FFT1D_H
