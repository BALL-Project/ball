// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT1D.h,v 1.12 2004/05/18 11:21:45 amoll Exp $
//

#ifndef BALL_MATHS_FFT1D_H
#define BALL_MATHS_FFT1D_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MATHS_COMPLEX_H
# include <BALL/MATHS/complex.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif

#include <math.h>
#include <fftw3.h>

namespace BALL
{
	/**  \addtogroup  FFT */
	//@{

	/** A class to perform Fast Fourier Transforms and inverse Fast Fourier Transforms
			on regularly spaced one dimensional data.  \par
			This class makes use of the freely available library <b>FFTW</b>, which can be
			found at http://www.fftw.org
			coordinate system can be handled automatically. The normaliztion is chosen
			symmetrically.
			\URL{FFT1D.h}
	 */
	class FFT1D 
		: public TRegularData1D<Complex>
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
				@param step_phys The step width in physical space
				@param origin The origin of the coordinate system
				@param in_fourier_space Flag to decide whether the data is assumed to be in physical or fourier
															space
		 */
		FFT1D(Size ldn, double step_phys = 1., double origin = 0., bool in_fourier_space_ = false)
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

		/** Translate the origin in physical space about <b>  transOrigin </b>.
			If the result is out of bounds, the function does nothing and
			returns <b>  false </b>.
		 */
		bool translate(double trans_origin)
			throw();

		/** Set the step width in physical space to <b>  newWidth </b>.
			The step width in fourier space is automatically adjusted
			accordingly. <b>  newWidth </b> must be positive, otherwise
			the function does nothing and retuns <b>  false </b>.
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

		/** Returns the data at the grid position closest to <b>  pos </b>,
			and automatically includes
			the correct phase factor and normalization.
		 */
		Complex getData(double pos) const
			throw(Exception::OutOfGrid);

		/** Returns the data at point <b>pos</b>. If <b>pos</b> is not a 
				point on the grid, the data is linearly interpolated.
				This method automatically includes the correct phase factor
				and normalization.
			*/
		Complex getInterpolatedValue(double pos) const
			throw(Exception::OutOfGrid);

		/** Sets the data point at the grid position closest to <b>  pos </b>
			to the value <b>  val </b>, and -- if called in fourier space --
			automatically includes the correct phase factor and normalization.
		 */
		void setData(double pos, Complex val)
			throw(Exception::OutOfGrid);

		/** Access the data at the grid position closest to <b>  pos </b>.
			This function returns the "raw" data at that position.
		 */
		Complex& operator [] (const double pos)
			throw(Exception::OutOfGrid);

		/** Access the data at the grid position closest to <b>  pos </b>.
				This function returns the "raw" data at that position.
			*/
		const Complex& operator [] (const double pos) const
			throw(Exception::OutOfGrid);
		
		/** Compute the phase factor.
			This computes the phase factor in fourier space that results
			if the origin of the coordinate system in physical space
			is not in the "lower left corner".
		 */
		Complex phase(double pos) const
			throw();

		protected:

		Size length_;
		bool in_fourier_space_;
		Size num_phys_to_fourier_;
		Size num_fourier_to_phys_;
		double origin_;
		double step_phys_;
		double step_fourier_;
		double min_phys_;
		double max_phys_;
		double min_fourier_;
		double max_fourier_;
		fftw_plan plan_forward_;
		fftw_plan plan_backward_;
	};
  //@}
}
#endif // BALL_MATHS_FFT1D_H
