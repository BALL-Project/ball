// $Id: FFT1D.C,v 1.1.2.1 2002/08/19 10:24:57 anhi Exp $

#include <BALL/MATHS/FFT1D.h>

namespace BALL
{
	FFT1D::FFT1D()
		throw()
		: TRegularData1D<FFTW_COMPLEX>(0,0,1.) // This is necessary because FFTW_COMPLEX has no default constructor
	{
	}

	FFT1D::~FFT1D()
		throw()
	{
	}

	FFT1D::FFT1D(const FFT1D &data)
		throw()
		: TRegularData1D<FFTW_COMPLEX>(data),
			length_(data.length_),
			inFourierSpace_(data.inFourierSpace_),
			numPhysToFourier_(data.numPhysToFourier_),
			numFourierToPhys_(data.numFourierToPhys_),
			origin_(data.origin_),
			stepPhys_(data.stepPhys_),
			stepFourier_(data.stepFourier_),
      minPhys_((-1.)*origin_),
      maxPhys_(((length_-1)*stepPhys_)-origin_),
      minFourier_((-1.)*(length_/2.-1)*stepFourier_),
      maxFourier_((length_/2.)*stepFourier_)
	{
		 planForward_  = fftw_create_plan(length_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
		 planBackward_ = fftw_create_plan(length_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								
	}

	FFT1D::FFT1D(Size ldn, double stepPhys, double origin, bool inFourierSpace)
		throw()
		: TRegularData1D<FFTW_COMPLEX>(-origin, (((1<<ldn)-1)*stepPhys)-origin, stepPhys),
		  length_(1<<ldn),
			inFourierSpace_(inFourierSpace),
			numPhysToFourier_(0),
			numFourierToPhys_(0),
			origin_(origin),
			stepPhys_(stepPhys),
			stepFourier_(2.*M_PI/(stepPhys_*length_)),
      minPhys_((-1.)*origin_),
      maxPhys_(((length_-1)*stepPhys_)-origin_),
      minFourier_((-1.)*(length_/2.-1)*stepFourier_),
      maxFourier_((length_/2.)*stepFourier_)
	{
		planForward_  = fftw_create_plan(length_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
	  planBackward_ = fftw_create_plan(length_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								

	 	if (inFourierSpace)
		{
			lower_ = minFourier_;
			upper_ = maxFourier_;
			spacing_ = stepFourier_;
		}
	}

	BALL_INLINE
	const FFT1D& FFT1D::operator = (const FFT1D& fft1d)
		throw()
	{
		clear();
		lower_ = fft1d.lower_;
		upper_ = fft1d.upper_;
		spacing_ = fft1d.spacing_;
		data_ = fft1d.data_;
		length_ = fft1d.length_;
		origin_ = fft1d.origin_;
		stepPhys_ = fft1d.stepPhys_;
		stepFourier_ = fft1d.stepFourier_;
    minPhys_ = ((-1.)*origin_);
    maxPhys_ = (((length_-1)*stepPhys_)-origin_);
    minFourier_ = ((-1.)*(length_/2.-1)*stepFourier_);
    maxFourier_ = ((length_/2.)*stepFourier_);
    numPhysToFourier_ = fft1d.numPhysToFourier_;
		numFourierToPhys_ = fft1d.numFourierToPhys_;
		planForward_ = fft1d.planForward_;
		planBackward_ = fft1d.planBackward_;
		
		return *this;
	}

	void FFT1D::clear()
		throw()
	{
		TRegularData1D<FFTW_COMPLEX>::clear();
	}

	void FFT1D::destroy()
		throw()
	{		
		TRegularData1D<FFTW_COMPLEX>::destroy();
		
		length_ = 0;
		origin_ = 
		stepPhys_ = 
		stepFourier_ = 0.;
		numPhysToFourier_ = 
		numFourierToPhys_ = 0;
    minPhys_ =
    maxPhys_ =
    minFourier_ =
    maxFourier_ = 0.;
	}

	bool FFT1D::operator == (const FFT1D& fft1d) const
		throw()
	{
		if (length_ == fft1d.length_ &&
				origin_ == fft1d.origin_ &&
				stepPhys_ == fft1d.stepPhys_ &&
				stepFourier_ == fft1d.stepFourier_ &&
				minPhys_ == fft1d.minPhys_ &&
				maxPhys_ == fft1d.maxPhys_ &&
				minFourier_ == fft1d.minFourier_ &&
				maxFourier_ == fft1d.maxFourier_ &&
				numPhysToFourier_ == fft1d.numPhysToFourier_ &&
				numFourierToPhys_ == fft1d.numFourierToPhys_)
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

	void FFT1D::doFFT()
		throw()
	{
		fftw_one(planForward_, &(*data_.begin()), 0);
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	void FFT1D::doiFFT()
		throw()
	{
		// Is this cast vector -> array portable?
		fftw_one(planBackward_, &(*data_.begin()), 0);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

	bool FFT1D::translate(double trans_origin)
		throw()
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

	bool FFT1D::setPhysStepWidth(double new_width)
		throw()
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
	
	double FFT1D::getPhysStepWidth() const
		throw()
	{
		return stepPhys_;
	}

	double FFT1D::getFourierStepWidth() const
		throw()
	{
		return stepFourier_;
	}

	double FFT1D::getPhysSpaceMin() const
		throw()
	{
    return minPhys_;
  }

	double FFT1D::getPhysSpaceMax() const
		throw()
	{
    return maxPhys_;
 	}

	double FFT1D::getFourierSpaceMin() const
		throw()
	{
		return minFourier_;
	}

	double FFT1D::getFourierSpaceMax() const
		throw()
	{
		return maxFourier_;
	}

	Complex FFT1D::getData(const double pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		double normalization=1.;

		if (!inFourierSpace_)
		{
			result = Complex((*this)[pos].re, (*this)[pos].im);
			normalization=1./pow(length_,numFourierToPhys_);
		}
		else
		{
			result = Complex((*this)[pos].re,(*this)[pos].im) * phase(pos);
			normalization=1./(sqrt(2.*M_PI))*stepPhys_/pow(length_,numFourierToPhys_);
		}

		result *= normalization;
		
		return result;
	}

	Complex FFT1D::getInterpolatedValue(const double pos) const
		throw(Exception::OutOfGrid)
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

		result  =  getData(before)*(BALL_COMPLEX_PRECISION)(1.-t);
		result +=  getData(after)*(BALL_COMPLEX_PRECISION)t; 

		return result;
	}

	void FFT1D::setData(double pos, Complex val)
		throw(Exception::OutOfGrid)
	{
		FFTW_COMPLEX dummy;
	
		if (!inFourierSpace_)
		{
			dummy.re = val.real()*((float)pow((float)(length_),(int)numFourierToPhys_));
			dummy.im = val.imag()*((float)pow((float)(length_),(int)numFourierToPhys_));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(BALL_COMPLEX_PRECISION)((sqrt(2*M_PI)/stepPhys_))
										 *(float)pow((float)length_,(int)numFourierToPhys_);
			
			dummy.re = val.real();
			dummy.im = val.imag();
			
			(*this)[pos]=dummy;
		}
	}

	FFTW_COMPLEX& FFT1D::operator[](const double pos)
		throw(Exception::OutOfGrid)
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
		
		return data_[internalPos];
	}

	const FFTW_COMPLEX& FFT1D::operator[](const double pos) const
		throw(Exception::OutOfGrid)
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
		
		return data_[internalPos];
	}

	Complex FFT1D::phase(const double pos) const
		throw()
	{
	  double phase = 2.*M_PI*(rint(pos/stepFourier_))
		                     *(rint(origin_/stepPhys_))
		                     /length_;

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
	}
}	
