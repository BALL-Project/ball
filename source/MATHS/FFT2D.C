// $Id: FFT2D.C,v 1.2 2002/12/12 10:22:09 oliver Exp $

#include <BALL/MATHS/FFT2D.h>

namespace BALL
{
	FFT2D::FFT2D()
		throw()
		: TRegularData2D<FFTW_COMPLEX>()
	{
	}

	FFT2D::~FFT2D()
		throw()
	{
	}

	FFT2D::FFT2D(const FFT2D &data)
		throw()
		: TRegularData2D<FFTW_COMPLEX>(data),
			lengthX_(data.lengthX_),
			lengthY_(data.lengthY_),
			inFourierSpace_(data.inFourierSpace_),
			numPhysToFourier_(data.numPhysToFourier_),
			numFourierToPhys_(data.numFourierToPhys_),
			origin_(data.origin_),
			stepPhysX_(data.stepPhysX_),
			stepPhysY_(data.stepPhysY_),
			stepFourierX_(data.stepFourierX_),
			stepFourierY_(data.stepFourierY_),
      minPhys_(Vector2(-origin_.x,-origin_.y)),
      maxPhys_(Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y)),
      minFourier_(Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_)),
      maxFourier_(Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_))
	{
		 planForward_  = fftw2d_create_plan(lengthX_, lengthY_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
		 planBackward_ = fftw2d_create_plan(lengthX_, lengthY_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								
	}

	FFT2D::FFT2D(Size ldnX, Size ldnY, double stepPhysX, double stepPhysY, Vector2 origin, bool inFourierSpace)
		throw()
		: TRegularData2D<FFTW_COMPLEX>(Vector2(-origin.x,-origin.y), Vector2((((1<<ldnX)-1)*stepPhysX_)-origin.x,(((1<<ldnX)-1)*stepPhysY_)-origin.y), (1<<ldnX), (1<<ldnY)),
		  lengthX_(1<<ldnX),
			lengthY_(1<<ldnY),
			inFourierSpace_(inFourierSpace),
			numPhysToFourier_(0),
			numFourierToPhys_(0),
			origin_(origin),
			stepPhysX_(stepPhysX),
			stepPhysY_(stepPhysY),
			stepFourierX_(2.*M_PI/(stepPhysX_*lengthX_)),
			stepFourierY_(2.*M_PI/(stepPhysY_*lengthY_)),
      minPhys_(Vector2(-origin_.x,-origin_.y)),
      maxPhys_(Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y)),
	    minFourier_(Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_)),
      maxFourier_(Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_))
{
		planForward_  = fftw2d_create_plan(lengthX_, lengthY_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
	  planBackward_ = fftw2d_create_plan(lengthX_, lengthY_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								

/*	 	if (inFourierSpace)
		{
			lower_ = minFourier_;
			upper_ = maxFourier_;
			spacing_ = stepFourier_;
		} */
	}

	BALL_INLINE
	const FFT2D& FFT2D::operator = (const FFT2D& fft2D)
		throw()
	{
		clear();
		//lower_ = fft2D.lower_;
		//upper_ = fft2D.upper_;
		//spacing_ = fft2D.spacing_;
		data = fft2D.data;
		lengthX_ = fft2D.lengthX_;
		lengthY_ = fft2D.lengthY_;
		origin_ = fft2D.origin_;
		stepPhysX_ = fft2D.stepPhysX_;
		stepPhysY_ = fft2D.stepPhysY_;
		stepFourierX_ = fft2D.stepFourierX_;
		stepFourierY_ = fft2D.stepFourierY_;
	  minPhys_ = Vector2(-origin_.x,-origin_.y);
    maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
    maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);
		numPhysToFourier_ = fft2D.numPhysToFourier_;
		numFourierToPhys_ = fft2D.numFourierToPhys_;
		planForward_ = fft2D.planForward_;
		planBackward_ = fft2D.planBackward_;
		
		return *this;
	}

	void FFT2D::clear()
		throw()
	{
		TRegularData2D<FFTW_COMPLEX>::clear();
	}

	void FFT2D::destroy()
		throw()
	{		
		// TRegularData2D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//		TRegularData2D<FFTW_COMPLEX>::destroy();
		TRegularData2D<FFTW_COMPLEX>::clear();
		
		lengthX_ = 
		lengthY_ = 0;
		origin_  = Vector2(0.,0.);  
		stepPhysX_ =
		stepPhysY_ =
		stepFourierX_ = 
		stepFourierY_ = 0.;
		numPhysToFourier_ = 
		numFourierToPhys_ = 0;
    minPhys_ =
    maxPhys_ =
    minFourier_ =
    maxFourier_ = Vector2(0.,0.);
	}

	bool FFT2D::operator == (const FFT2D& fft2D) const
		throw()
	{
		if (lengthX_ == fft2D.lengthX_ &&
				lengthY_ == fft2D.lengthY_ &&
				origin_ == fft2D.origin_ &&
				stepPhysX_ == fft2D.stepPhysX_ &&
				stepPhysY_ == fft2D.stepPhysY_ &&
				stepFourierX_ == fft2D.stepFourierX_ &&
				stepFourierY_ == fft2D.stepFourierY_ &&
				minPhys_ == fft2D.minPhys_ &&
				maxPhys_ == fft2D.maxPhys_ &&
				minFourier_ == fft2D.minFourier_ &&
				maxFourier_ == fft2D.maxFourier_ &&
				numPhysToFourier_ == fft2D.numPhysToFourier_ &&
				numFourierToPhys_ == fft2D.numFourierToPhys_)
		{
			Vector2 min  = inFourierSpace_ ?  minFourier_  :   minPhys_;
			Vector2 max  = inFourierSpace_ ?  maxFourier_  :   maxPhys_;
			double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
			double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;	
			
			for (double posX=min.x; posX<=max.x; posX+=stepX)
			{
				for (double posY=min.y; posY<=max.y; posY+=stepY)
				{
					if (getData(Vector2(posX,posY)) != fft2D.getData(Vector2(posX,posY)))
					{
						return false;
					}
				}
			}
			
			return true;
		}
	
		return false;
	}

	void FFT2D::doFFT()
		throw()
	{
//		fftwnd_one(planForward_, &(*data.begin()), 0);
		fftwnd_one(planForward_,data, 0); //TRegularData2D is *not* based on vector but on standard-arrays...
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	void FFT2D::doiFFT()
		throw()
	{
		//fftwnd_one(planBackward_, &(*data.begin()), 0);
		fftwnd_one(planBackward_,data,0);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

	bool FFT2D::translate(const Vector2& trans_origin)
		throw()
	{
		Position internalOriginX = (Position) rint(trans_origin.x*stepPhysX_);
		Position internalOriginY = (Position) rint(trans_origin.y*stepPhysY_);
		
		if ((internalOriginX <= lengthX_) && (internalOriginY <= lengthY_))
		{
			origin_.x = trans_origin.x;
			origin_.y = trans_origin.y;
			
      minPhys_ = Vector2(-origin_.x,-origin_.y);
  	  maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	 	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
			maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);
		 
			return true;
		}
		else
		{
			return false;
		}
	}

	bool FFT2D::setPhysStepWidth(double new_width_x, double new_width_y)
		throw()
	{
		if ((new_width_x <= 0) || (new_width_y <= 0))
		{
			return false;
		}
		else
		{
			stepPhysX_ = new_width_x;
			stepPhysY_ = new_width_y;
			stepFourierX_ = 2.*M_PI/(stepPhysX_*lengthX_);
			stepFourierY_ = 2.*M_PI/(stepPhysY_*lengthY_);

			minPhys_ = Vector2(-origin_.x,-origin_.y);
  	  maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	 	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
			maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);
	
      return true;
		}
	}
	
	double FFT2D::getPhysStepWidthX() const
		throw()
	{
		return stepPhysX_;
	}

	double FFT2D::getPhysStepWidthY() const
		throw()
	{
		return stepPhysY_;
	}

	double FFT2D::getFourierStepWidthX() const
		throw()
	{
		return stepFourierX_;
	}

	double FFT2D::getFourierStepWidthY() const
		throw()
	{
		return stepFourierY_;
	}

	double FFT2D::getPhysSpaceMinX() const
		throw()
	{
    return minPhys_.x;
  }

	double FFT2D::getPhysSpaceMinY() const
		throw()
	{
    return minPhys_.y;
  }

	double FFT2D::getPhysSpaceMaxX() const
		throw()
	{
    return maxPhys_.x;
 	}

	double FFT2D::getPhysSpaceMaxY() const
		throw()
	{
    return maxPhys_.y;
 	}

	double FFT2D::getFourierSpaceMinX() const
		throw()
	{
		return minFourier_.x;
	}

	double FFT2D::getFourierSpaceMinY() const
		throw()
	{
		return minFourier_.y;
	}

	double FFT2D::getFourierSpaceMaxX() const
		throw()
	{
		return maxFourier_.x;
	}

	double FFT2D::getFourierSpaceMaxY() const
		throw()
	{
		return maxFourier_.y;
	}

	Complex FFT2D::getData(const Vector2& pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		double normalization=1.;

		if (!inFourierSpace_)
		{
			result = Complex((*this)[pos].re, (*this)[pos].im);
			normalization=1./((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_));
		}
		else
		{
			result = Complex((*this)[pos].re,(*this)[pos].im) * phase(pos);
			normalization=1./(sqrt(2.*M_PI))*(stepPhysX_*stepPhysY_)/((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_));
		}

		result *= normalization;
		
		return result;
	}

	Complex FFT2D::getInterpolatedValue(const Vector2& pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		
		Vector2 min  = inFourierSpace_ ? minFourier_   :   minPhys_;
		Vector2 max  = inFourierSpace_ ? maxFourier_   :   maxPhys_;
		double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
		double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;
		
		if (    (pos.x < min.x) || (pos.y < min.y)
				 || (pos.x > max.x) || (pos.y > max.y)  )
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		Vector2 h(pos.x - min.x, pos.y - min.y);
		double modX = fmod((double)h.x,stepX);
		double modY = fmod((double)h.y,stepY);

		if (modX==0 && modY ==0) // we are on the grid
		{
			return getData(pos);
		}

		double beforeX = floor(h.x/stepX)*stepX+ min.x;
		double beforeY = floor(h.y/stepY)*stepY+ min.y;
		double afterX  =  ceil(h.x/stepX)*stepX+ min.x;
		double afterY  =  ceil(h.y/stepY)*stepY+ min.y;
			
		double tx = (pos.x - beforeX)/stepX;
		double ty = (pos.y - beforeY)/stepY;

		result  = getData(Vector2(beforeX,beforeY))*(BALL_COMPLEX_PRECISION)((1.-tx)*(1.-ty));
		result += getData(Vector2(afterX, beforeY))*(BALL_COMPLEX_PRECISION)(    tx *(1.-ty));
		result += getData(Vector2(beforeX,afterY ))*(BALL_COMPLEX_PRECISION)((1.-tx)*    ty );
		result += getData(Vector2(afterX, afterY ))*(BALL_COMPLEX_PRECISION)(    tx *    ty );

		return result;
	}

	void FFT2D::setData(const Vector2& pos, Complex val)
		throw(Exception::OutOfGrid)
	{
		FFTW_COMPLEX dummy;
	
		if (!inFourierSpace_)
		{
			dummy.re = val.re*((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_));
			dummy.im = val.im*((float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(BALL_COMPLEX_PRECISION)((sqrt(2*M_PI)/(stepPhysX_*stepPhysY_)))
										 *(float)pow((float)(lengthX_*lengthY_),(int)numFourierToPhys_);
			
			dummy.re = val.re;
			dummy.im = val.im;
			
			(*this)[pos]=dummy;
		}
	}

	FFTW_COMPLEX& FFT2D::operator[](const Vector2& pos)
		throw(Exception::OutOfGrid)
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j;
			
			i = (Index) rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) rint((pos.y+origin_.y)/stepPhysY_);

			internalPos = j + i*lengthY_;
		}
		else
		{
			Index i, j;

			i = (Index) rint(pos.x/stepFourierX_);
			j = (Index) rint(pos.y/stepFourierY_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			internalPos = (j + i*lengthY_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return data[internalPos];
	}

	const FFTW_COMPLEX& FFT2D::operator[](const Vector2& pos) const
		throw(Exception::OutOfGrid)
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j;
			
			i = (Index) rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) rint((pos.y+origin_.y)/stepPhysY_);

			internalPos = j + i*lengthY_;
		}
		else
		{
			Index i, j;

			i = (Index) rint(pos.x/stepFourierX_);
			j = (Index) rint(pos.y/stepFourierY_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			internalPos = (j + i*lengthY_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return data[internalPos];
	}
	
	Complex FFT2D::phase(const Vector2& pos) const
		throw()
	{
	  double phase = 2.*M_PI*(  rint(pos.x/stepFourierX_)*rint(origin_.x/stepPhysX_)
															/lengthX_
														+ rint(pos.y/stepFourierY_)*rint(origin_.y/stepPhysY_)
															/lengthY_ );

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
	}
}

