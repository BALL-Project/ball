// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT1D.C,v 1.5 2003/05/03 17:29:32 oliver Exp $

#include <BALL/MATHS/FFT1D.h>

namespace BALL
{
	FFT1D::FFT1D()
		throw()
		: TRegularData1D<FFTW_COMPLEX>(0, 0, 1.)  // This is necessary because FFTW_COMPLEX has no default constructor
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
			in_fourier_space_(data.in_fourier_space_),
			num_phys_to_fourier_(data.num_phys_to_fourier_),
			num_fourier_to_phys_(data.num_fourier_to_phys_),
			origin_(data.origin_),
			step_phys_(data.step_phys_),
			step_fourier_(data.step_fourier_),
      min_phys_((-1.)*origin_),
      max_phys_(((length_-1)*step_phys_)-origin_),
      min_fourier_((-1.)*(length_/2.-1)*step_fourier_),
      max_fourier_((length_/2.)*step_fourier_)
	{
		 plan_forward_  = fftw_create_plan(length_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
		 plan_backward_ = fftw_create_plan(length_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								
	}

	FFT1D::FFT1D(Size ldn, double step_phys, double origin, bool in_fourier_space)
		throw()
		: TRegularData1D<FFTW_COMPLEX>(-origin, (((1<<ldn)-1)*step_phys)-origin, step_phys),
		  length_(1<<ldn),
			in_fourier_space_(in_fourier_space),
			num_phys_to_fourier_(0),
			num_fourier_to_phys_(0),
			origin_(origin),
			step_phys_(step_phys),
			step_fourier_(2.*M_PI/(step_phys_*length_)),
      min_phys_((-1.)*origin_),
      max_phys_(((length_-1)*step_phys_)-origin_),
      min_fourier_((-1.)*(length_/2.-1)*step_fourier_),
      max_fourier_((length_/2.)*step_fourier_)
	{
		plan_forward_  = fftw_create_plan(length_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
	  plan_backward_ = fftw_create_plan(length_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								

	 	if (in_fourier_space)
		{
			origin_ = min_fourier_;
			dimension_ = max_fourier_ - min_fourier_;
			spacing_ = step_fourier_;
		}
	}

	BALL_INLINE
	const FFT1D& FFT1D::operator = (const FFT1D& fft1d)
		throw()
	{
		clear();
		origin_ = fft1d.origin_;
		dimension_ = fft1d.dimension_;
		spacing_ = fft1d.spacing_;
		data_ = fft1d.data_;
		length_ = fft1d.length_;
		origin_ = fft1d.origin_;
		step_phys_ = fft1d.step_phys_;
		step_fourier_ = fft1d.step_fourier_;
    min_phys_ = ((-1.)*origin_);
    max_phys_ = (((length_-1)*step_phys_)-origin_);
    min_fourier_ = ((-1.)*(length_/2.-1)*step_fourier_);
    max_fourier_ = ((length_/2.)*step_fourier_);
    num_phys_to_fourier_ = fft1d.num_phys_to_fourier_;
		num_fourier_to_phys_ = fft1d.num_fourier_to_phys_;
		plan_forward_ = fft1d.plan_forward_;
		plan_backward_ = fft1d.plan_backward_;
		
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
		TRegularData1D<FFTW_COMPLEX>::clear();
		
		length_ = 0;
		origin_ = 
		step_phys_ = 
		step_fourier_ = 0.;
		num_phys_to_fourier_ = 
		num_fourier_to_phys_ = 0;
    min_phys_ =
    max_phys_ =
    min_fourier_ =
    max_fourier_ = 0.;
	}

	bool FFT1D::operator == (const FFT1D& fft1d) const
		throw()
	{
		if (length_ == fft1d.length_ &&
				origin_ == fft1d.origin_ &&
				step_phys_ == fft1d.step_phys_ &&
				step_fourier_ == fft1d.step_fourier_ &&
				min_phys_ == fft1d.min_phys_ &&
				max_phys_ == fft1d.max_phys_ &&
				min_fourier_ == fft1d.min_fourier_ &&
				max_fourier_ == fft1d.max_fourier_ &&
				num_phys_to_fourier_ == fft1d.num_phys_to_fourier_ &&
				num_fourier_to_phys_ == fft1d.num_fourier_to_phys_)
		{
			double min  = in_fourier_space_ ?  min_fourier_ :  min_phys_;
			double max  = in_fourier_space_ ?  max_fourier_ :  max_phys_;
			double step = in_fourier_space_ ? step_fourier_ : step_phys_;
				
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
		fftw_one(plan_forward_, &(*data_.begin()), 0);
		in_fourier_space_ = true;
		num_phys_to_fourier_++;
	}

	void FFT1D::doiFFT()
		throw()
	{
		// Is this cast vector -> array portable?
		fftw_one(plan_backward_, &(*data_.begin()), 0);
		in_fourier_space_ = false;
		num_fourier_to_phys_++;
	}

	bool FFT1D::translate(double trans_origin)
		throw()
	{
		Position internalOrigin = (int) rint(trans_origin/step_phys_);
		if (internalOrigin <= length_)
		{
			origin_ = trans_origin;

      min_phys_ = ((-1.)*origin_);
      max_phys_ = (((length_-1)*step_phys_)-origin_);
      min_fourier_ = ((-1.)*(length_/2.-1)*step_fourier_);
      max_fourier_ = ((length_/2.)*step_fourier_);
      
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
			step_phys_ = new_width;
			step_fourier_ = 2.*M_PI/(step_phys_*length_);

      min_phys_ = ((-1.)*origin_);
      max_phys_ = (((length_-1)*step_phys_)-origin_);
      min_fourier_ = ((-1.)*(length_/2.-1)*step_fourier_);
      max_fourier_ = ((length_/2.)*step_fourier_);

      return true;
		}
	}
	
	double FFT1D::getPhysStepWidth() const
		throw()
	{
		return step_phys_;
	}

	double FFT1D::getFourierStepWidth() const
		throw()
	{
		return step_fourier_;
	}

	double FFT1D::getPhysSpaceMin() const
		throw()
	{
    return min_phys_;
  }

	double FFT1D::getPhysSpaceMax() const
		throw()
	{
    return max_phys_;
 	}

	double FFT1D::getFourierSpaceMin() const
		throw()
	{
		return min_fourier_;
	}

	double FFT1D::getFourierSpaceMax() const
		throw()
	{
		return max_fourier_;
	}

	Complex FFT1D::getData(const double pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		double normalization=1.;

		if (!in_fourier_space_)
		{
			result = Complex((*this)[pos].re, (*this)[pos].im);
			normalization=1./pow(length_,num_fourier_to_phys_);
		}
		else
		{
			result = Complex((*this)[pos].re,(*this)[pos].im) * phase(pos);
			normalization=1./(sqrt(2.*M_PI))*step_phys_/pow(length_,num_fourier_to_phys_);
		}

		result *= normalization;
		
		return result;
	}

	Complex FFT1D::getInterpolatedValue(const double pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		
		double min  = in_fourier_space_ ? min_fourier_  :  min_phys_;
		double max  = in_fourier_space_ ? max_fourier_  :  max_phys_;
		double step = in_fourier_space_ ? step_fourier_ : step_phys_;
		
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
	
		if (!in_fourier_space_)
		{
			dummy.re = val.re*((float)pow((float)(length_),(int)num_fourier_to_phys_));
			dummy.im = val.im*((float)pow((float)(length_),(int)num_fourier_to_phys_));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(BALL_COMPLEX_PRECISION)((sqrt(2*M_PI)/step_phys_))
										 *(float)pow((float)length_,(int)num_fourier_to_phys_);
			
			dummy.re = val.re;
			dummy.im = val.im;
			
			(*this)[pos]=dummy;
		}
	}

	FFTW_COMPLEX& FFT1D::operator[](const double pos)
		throw(Exception::OutOfGrid)
	{
		Index internalPos;

		if (!in_fourier_space_)
		{
			internalPos = (Index) rint((pos+origin_)/step_phys_);
		}
		else
		{
			internalPos =  (Index) rint(pos/step_fourier_);

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

		if (!in_fourier_space_)
		{
			internalPos = (Index) rint((pos+origin_)/step_phys_);
		}
		else
		{
			internalPos =  (Index) rint(pos/step_fourier_);

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
	  double phase = 2.*M_PI*(rint(pos/step_fourier_))
		                     *(rint(origin_/step_phys_))
		                     /length_;

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
	}
}	
