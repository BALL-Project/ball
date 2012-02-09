// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_TFFT1D_H
#define BALL_MATHS_TFFT1D_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif

#include <cmath>
#include <complex>
#include <fftw3.h>

#include <BALL/MATHS/fftwCommon.h>

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
	 */
	template <typename ComplexTraits>
	class TFFT1D 
		: public TRegularData1D<std::complex<typename ComplexTraits::ComplexPrecision> >
	{
		public:

		typedef std::complex<typename ComplexTraits::ComplexPrecision> Complex;
		typedef TRegularData1D<std::complex<typename ComplexTraits::ComplexPrecision> >	ComplexVector;

		BALL_CREATE(TFFT1D)

		/**  @name Constructors and Destructors
		 */
		//@{

		/// Default constructor
		TFFT1D();

		/// Copy constructor
		TFFT1D(const TFFT1D &data);

		/** Detailed constructor.  \par
				@param ldn The binary logarithm of the number of grid points (we use the logarithm to
									 ensure that the number of points is a power of two, which is important for
									 the FFT)
				@param stepPhys The step width in physical space
				@param origin The origin of the coordinate system
				@param inFourierSpace Flag to decide whether the data is assumed to be in physical or fourier
															space
		 */
		 // AR: ldn is not any longer the binary logarithm but the absolute number of grid points
		TFFT1D(Size ldn, double stepPhys = 1., double origin = 0., bool inFourierSpace = false);
		
		/// Destructor
		virtual ~TFFT1D();
		
		//@}

		/** @name Assignment
		 */
		//@{

		/// Assignment operator
		const TFFT1D& operator = (const TFFT1D& fft1d);
		
		/** Clear the contents.
		 */
		virtual void clear();
		
		/** Clear the contents and reset all attributes.
		 */
		virtual void destroy();

		//@}

		/** @name Predicates
		 */
		//@{

		/** Equality operator.
		 */
		bool operator == (const TFFT1D& fft1d) const;
		//@}
		
		// @name Accessors
		
		/** Perform a single fast fourier transform on the data.
		 */
		void doFFT();

		/** Perform a single inverse fourier transform on the data.
		 */
		void doiFFT();

		/** Translate the origin in physical space about <b>  transOrigin </b>.
			If the result is out of bounds, the function does nothing and
			returns <b>  false </b>.
		 */
		bool translate(double trans_origin);

		/** Set the step width in physical space to <b>  newWidth </b>.
			The step width in fourier space is automatically adjusted
			accordingly. <b>  newWidth </b> must be positive, otherwise
			the function does nothing and retuns <b>  false </b>.
		 */
		bool setPhysStepWidth(double new_width);

		/** Returns the step width in physical space.
		 */
		double getPhysStepWidth() const;

		/** Returns the step width in fourier space.
		 */
		double getFourierStepWidth() const;

		/** Returns the minimal position of the grid in physical space.
		 */
		double getPhysSpaceMin() const;

		/** Returns the maximal position of the grid in physical space.
		 */
		double getPhysSpaceMax() const;

		/** Returns the minimal position of the grid in fourier space.
		 */
		double getFourierSpaceMin() const;

		/** Returns the maximal position of the grid in fourier space.
		 */
		double getFourierSpaceMax() const;
			
		/** AR: Return the largest grid position direction. 
		 		This method returns the maximum position allowed in the grid. As the point 
				in the origin has the indices 0, this method returns the number of 
				points minus one.
		 */
		Size getMaxIndex() const;

		/** AR: Return the number of inverse transforms that have been carried out using this class.
			 	This is an important factor for the normalization of the data.
			 */
		Size getNumberOfInverseTransforms() const;
	
		/** AR: Returns the grid coordinate corresponding to the position.
		 */
		double getGridCoordinates(Position position) const;

		/** Returns the data at the grid position closest to <b>  pos </b>,
		 *	and automatically includes
		 * 	the correct phase factor and normalization.
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		Complex getData(const double pos) const;

		/** Returns the data at point <b>pos</b>. If <b>pos</b> is not a 
		 *	point on the grid, the data is linearly interpolated.
		 *	This method automatically includes the correct phase factor
		 *	and normalization.
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		Complex getInterpolatedValue(const double pos) const;

		/** Sets the data point at the grid position closest to <b>  pos </b>
		 *	to the value <b>  val </b>, and -- if called in fourier space --
		 *	automatically includes the correct phase factor and normalization.
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		void setData(double pos, Complex val);

		/** Access the data at the grid position closest to <b>  pos </b>.
		 *  This function returns the "raw" data at that position.
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		Complex& operator [] (const double pos);

		/** Access the data at the grid position closest to <b>  pos </b>.
		 *  This function returns the "raw" data at that position.
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		const Complex& operator [] (const double pos) const;
		
		/** Access the (raw) data at Position pos
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		Complex& operator[](const Position& pos)
		{
			return TRegularData1D<Complex>::operator[](pos);
		}
			
		/** Access the (raw) data at Position pos. Const method.
		 *
		 *  @throw Exception::OutOfGrid if pos is outside the grid boundaries
		 */
		const Complex& operator[](const Position& pos) const
		{
			return TRegularData1D<Complex>::operator[](pos);
		} 
		
		void setNumberOfFFTTransforms(Size num)
		{
			numPhysToFourier_ = num;
		}
			
		void setNumberOfiFFTTransforms(Size num)
		{
			numFourierToPhys_ = num;
		}
		
		/** AR: Returns <b>true</b> if the data is considered to be in Fourier space,
		    <b>false</b> otherwise.
		 */
		bool isInFourierSpace() const;
		
		/** Compute the phase factor.
			This computes the phase factor in fourier space that results
			if the origin of the coordinate system in physical space
			is not in the "lower left corner".
		 */
		Complex phase(const double pos) const;

		protected:

		Size length_;
		bool inFourierSpace_;
		Size numPhysToFourier_;
		Size numFourierToPhys_;
		double origin_;
		double stepPhys_;
		double stepFourier_;
		double minPhys_;
		double maxPhys_;
		double minFourier_;
		double maxFourier_;

		typename ComplexTraits::FftwPlan planForward_;
		typename ComplexTraits::FftwPlan planBackward_;
		
		// AR: to control plan calculation with new fftw3
		Complex *dataAdress_;
		bool planCalculated_;
	};
  //@}
  
  /**	Default type
	*/
	typedef TFFT1D<BALL_FFTW_DEFAULT_TRAITS> FFT1D;
  
  // AR:
	/** Global assignment operator from FFT2D to TRegularData2D<Complex>
	 */
//	const TRegularData1D<Complex>& operator << (TRegularData1D<Complex>& to, const TFFT1D& from)
	//; ?????
	
	/** Global assignment operator from FFT3D to TRegularData3D<float>.
	 		This operator assigns the <b>real</b> part of the complex FFT2D-data to the
			TRegularData2D<float> to.
	 */
//	const RegularData1D& operator << (RegularData1D& to, const TFFT1D& from)
//; ???????



  template <typename ComplexTraits>
	TFFT1D<ComplexTraits>::TFFT1D()
		: TRegularData1D<Complex>(0, 0, 1.),  // AR: old case: This is necessary because FFTW_COMPLEX has no default constructor
			length_(0),
			inFourierSpace_(false),
			dataAdress_(0),
			planCalculated_(false)
	{
	}
		

	template <typename ComplexTraits>
	bool TFFT1D<ComplexTraits>::operator == (const TFFT1D<ComplexTraits>& fft1d) const
	{
		if (ComplexVector::size() == fft1d.size() &&
				origin_ == fft1d.origin_ &&
				stepPhys_ == fft1d.stepPhys_ &&
				stepFourier_ == fft1d.stepFourier_ &&
				minPhys_ == fft1d.minPhys_ &&
				maxPhys_ == fft1d.maxPhys_ &&
				minFourier_ == fft1d.minFourier_ &&
				maxFourier_ == fft1d.maxFourier_ &&
				inFourierSpace_ == fft1d.inFourierSpace_ &&
				numPhysToFourier_ == fft1d.numPhysToFourier_ &&
				numFourierToPhys_ == fft1d.numFourierToPhys_ &&
				planCalculated_ == fft1d.planCalculated_)
		{
			double min  = inFourierSpace_ ?  minFourier_ :  minPhys_;
			double max  = inFourierSpace_ ?  maxFourier_ :  maxPhys_;
			double step = inFourierSpace_ ? stepFourier_ : stepPhys_;
				
			for (double pos=min; pos<=max; pos+=step)
			{
				if (getData(pos) != fft1d.getData(pos))
				{
					return false;
				}
			}
			
			return true;
		}
	
		return false;
	}	

	template <typename ComplexTraits>
	bool TFFT1D<ComplexTraits>::translate(double trans_origin)
	{
		Position internalOrigin = (int) rint(trans_origin/stepPhys_);
		if (internalOrigin <= length_)
		{
			origin_ = trans_origin;

      minPhys_ = ((-1.)*origin_);
      maxPhys_ = (((length_-1)*stepPhys_)-origin_);
      minFourier_ = ((-1.)*(length_/2.-1)*stepFourier_);
      maxFourier_ = ((length_/2.)*stepFourier_);
      
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename ComplexTraits>
	bool TFFT1D<ComplexTraits>::setPhysStepWidth(double new_width)
	{
		if (new_width < 0)
		{
			return false;
		}
		else
		{
			stepPhys_ = new_width;
			stepFourier_ = 2.*M_PI/(stepPhys_*length_);

      minPhys_ = ((-1.)*origin_);
      maxPhys_ = (((length_-1)*stepPhys_)-origin_);
      minFourier_ = ((-1.)*(length_/2.-1)*stepFourier_);
      maxFourier_ = ((length_/2.)*stepFourier_);

      return true;
		}
	}

	template <typename ComplexTraits>	
	double TFFT1D<ComplexTraits>::getPhysStepWidth() const
	{
		return stepPhys_;
	}

	template <typename ComplexTraits>
	double TFFT1D<ComplexTraits>::getFourierStepWidth() const
	{
		return stepFourier_;
	}

	template <typename ComplexTraits>
	double TFFT1D<ComplexTraits>::getPhysSpaceMin() const
	{
    return minPhys_;
  }

	template <typename ComplexTraits>
	double TFFT1D<ComplexTraits>::getPhysSpaceMax() const
	{
    return maxPhys_;
 	}

	template <typename ComplexTraits>
	double TFFT1D<ComplexTraits>::getFourierSpaceMin() const
	{
		return minFourier_;
	}

	template <typename ComplexTraits>
	double TFFT1D<ComplexTraits>::getFourierSpaceMax() const
	{
		return maxFourier_;
	}

	template <typename ComplexTraits>	
	Size TFFT1D<ComplexTraits>::getMaxIndex() const
	{
		return (length_ - 1);
	}

	template <typename ComplexTraits>	
	Size TFFT1D<ComplexTraits>::getNumberOfInverseTransforms() const
	{
		return numFourierToPhys_;
	}
	
	// AR: new 
	template <typename ComplexTraits>
	double TFFT1D<ComplexTraits>::getGridCoordinates(Position position) const
	{
		if (!inFourierSpace_)
		{
			if (position >= ComplexVector::size())
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			double r;
			
			r = -origin_ + (float)position * stepPhys_;

			return r;
		}
		else
		{
			if (position >= ComplexVector::size())
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			double r;
			Index x;
	
			x = position;

			if (x>=length_/2.)
			{
				x-=length_;
			}
			
			r = (float)x * stepFourier_;

			return r;
		}
	}

	template <typename ComplexTraits>
	typename TFFT1D<ComplexTraits>::Complex TFFT1D<ComplexTraits>::getData(const double pos) const
	{
		Complex result;
		double normalization=1.;

		if (!inFourierSpace_)
		{
			result = (*this)[pos];//Complex((*this)[pos].real(), (*this)[pos].imag());
			normalization=1./pow((double)length_,(int)numFourierToPhys_);
		}
		else
		{
			result = (*this)[pos]*phase(pos);//Complex((*this)[pos].real(),(*this)[pos].imag()) * phase(pos);
			//Log.error() << pos <<  " " << phase(pos).real() <<  std::endl;
			normalization=1./(sqrt(2.*M_PI))/pow((double)length_,(int)numFourierToPhys_);
		}

		result *= normalization;
		
		return result;
	}

	template <typename ComplexTraits>
	typename TFFT1D<ComplexTraits>::Complex TFFT1D<ComplexTraits>::getInterpolatedValue(const double pos) const
	{
		Complex result;
		
		double min  = inFourierSpace_ ? minFourier_  :  minPhys_;
		double max  = inFourierSpace_ ? maxFourier_  :  maxPhys_;
		double step = inFourierSpace_ ? stepFourier_ : stepPhys_;
		
		if ((pos < min) || (pos > max))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		double h = pos - min;
		double mod = fmod(h,step);

		if (mod ==0) // we are on the grid
		{
			return getData(pos);
		}

		double before = floor(h/step)*step+ min;
		double after  = ceil(h/step)*step+ min;
			
		double t = (pos - before)/step;

		result  =  getData(before)*(typename ComplexTraits::ComplexPrecision)(1.-t);
		result +=  getData(after)*(typename ComplexTraits::ComplexPrecision)t; 

		return result;
	}

	template <typename ComplexTraits>
	void TFFT1D<ComplexTraits>::setData(double pos, Complex val)
	{
		Complex dummy;
		if (!inFourierSpace_)
		{
			dummy = Complex(val.real()*((typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(length_),(int)numFourierToPhys_)),
											 val.imag()*((typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)(length_),(int)numFourierToPhys_)));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(typename ComplexTraits::ComplexPrecision)((sqrt(2*M_PI)/stepPhys_))
										 *(typename ComplexTraits::ComplexPrecision)pow((typename ComplexTraits::ComplexPrecision)length_,(int)numFourierToPhys_);
			
			dummy = val;
			
			(*this)[pos]=dummy;
		}
	}

	template <typename ComplexTraits>
	typename TFFT1D<ComplexTraits>::Complex& TFFT1D<ComplexTraits>::operator[](const double pos) 
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			internalPos = (Index) rint((pos+origin_)/stepPhys_);
		}
		else
		{
			internalPos =  (Index) rint(pos/stepFourier_);

			if (internalPos < 0)
			{
				internalPos+=length_;
			}
		}

		if ((internalPos < 0) || (internalPos>=(Index) length_))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return operator [] ((Position)internalPos);
	}

	template <typename ComplexTraits>
	const typename TFFT1D<ComplexTraits>::Complex& TFFT1D<ComplexTraits>::operator[](const double pos) const
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			internalPos = (Index) rint((pos+origin_)/stepPhys_);
		}
		else
		{
			internalPos =  (Index) rint(pos/stepFourier_);

			if (internalPos < 0)
			{
				internalPos+=length_;
			}
		}

		if ((internalPos < 0) || (internalPos>=(Index) length_))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return operator [] ((Position)internalPos);
	}

	template <typename ComplexTraits>
	typename TFFT1D<ComplexTraits>::Complex TFFT1D<ComplexTraits>::phase(const double pos) const
	{
	  double phase = 2.*M_PI*(rint(pos/stepFourier_))
		                     *(rint(origin_/stepPhys_))
		                     /length_;
		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
	}

	template <typename ComplexTraits>
	bool TFFT1D<ComplexTraits>::isInFourierSpace() const
	{
		return inFourierSpace_;
	}
/*	
	const TRegularData1D<Complex >& operator << (TRegularData1D<Complex >& to, const TFFT1D<ComplexTraits>& from)
	{
		// first decide if the TFFT1D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxIndex()+1;
			
			TRegularData1D<Complex > newGrid(TRegularData1D<Complex >::IndexType(lengthX), from.getPhysSpaceMin(), from.getPhysSpaceMax());

			// and fill it
			double normalization=1./(pow((float)(lengthX),from.getNumberOfInverseTransforms()));
			
			for (Position i = 0; i < from.size(); i++)
			{
				newGrid[i] = from[i]*(ComplexTraits::ComplexPrecision)normalization;
			}

			to = newGrid;

			return to;
		}
		else
		{
			// we are in Fourier space
			
			// create a new grid
			Size lengthX = from.getMaxIndex()+1;
			//float stepPhysX = from.getPhysStepWidthX();
			float stepFourierX = from.getFourierStepWidth();

			
			TRegularData1D<Complex > newGrid(TRegularData1D<Complex >::IndexType(lengthX),
																			from.getFourierSpaceMin(), 
																			from.getFourierSpaceMax());

			// and fill it
			// AR: old double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			double normalization=1./sqrt(2.*M_PI)/(pow((float)(lengthX),from.getNumberOfInverseTransforms()));
			
			
			Index x;
			float r;
	
			for (Position i = 0; i < from.size(); i++)
			{
				x = i;

				if (x>lengthX/2.)
				{
					x-=lengthX;
				}

				r = (float)x * stepFourierX;

				newGrid[i] = from[i]*(ComplexTraits::ComplexPrecision)normalization*from.phase(r);
			}

			to = newGrid;

			return to;
		}
	}
	*/
	
	template <typename ComplexTraits>
	const RegularData1D& operator << (RegularData1D& to, const TFFT1D<ComplexTraits>& from)
	{
		// first decide if the FFT1D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxIndex()+1;
			
			RegularData1D newGrid(lengthX);
			newGrid.setOrigin(from.getPhysSpaceMin());
			newGrid.setDimension(from.getPhysSpaceMax()-from.getPhysSpaceMin());

			// and fill it
			double normalization = 1./(pow((float)(lengthX),from.getNumberOfInverseTransforms()));
			
			for (Position i = 0; i < from.size(); i++)
			{
				newGrid[i] = from[i].real()*normalization;
			}

			to = newGrid;

			return to;
		}
		else
		{
			// we are in Fourier space
			
			// create a new grid
			Size lengthX = from.getMaxIndex()+1;
			//float stepPhysX = from.getPhysStepWidth();
			float stepFourierX = from.getFourierStepWidth();

			RegularData1D newGrid(lengthX);
			newGrid.setOrigin(from.getFourierSpaceMin());
			newGrid.setDimension(from.getFourierSpaceMax()-from.getFourierSpaceMin());

			// and fill it
			// AR: old version double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			double normalization=1./sqrt(2.*M_PI)/(pow((float)(lengthX),from.getNumberOfInverseTransforms()));
			
			Index x;
			signed int xp;
			float r;
	
			for (Position i = 0; i < from.size(); i++)
			{
				x =  i;
				
				xp = x;

				if (xp>=lengthX/2.)
				{
					xp-=(int)lengthX;
				}

				if (x>=lengthX/2.)
				{
					x-=(int)(lengthX/2.);
				}
				else
				{
					x+=(int)(lengthX/2.);
				}


				r = ((float)xp * stepFourierX);

				newGrid[i] = (from[i]*(typename ComplexTraits::ComplexPrecision)normalization*from.phase(r)).real();
			}

			to = newGrid;

			return to;
		}
	}
}
#endif // BALL_MATHS_TFFT1D_H
