// $Id: FFT3D.C,v 1.3.2.1 2003/02/05 15:32:51 anker Exp $

#include <BALL/MATHS/FFT3D.h>

namespace BALL
{
	FFT3D::FFT3D()
		throw()
		: TRegularData3D<FFTW_COMPLEX>()
	{
	}

	FFT3D::~FFT3D()
		throw()
	{
	}

	FFT3D::FFT3D(const FFT3D &data)
		throw()
		: TRegularData3D<FFTW_COMPLEX>(data),
			lengthX_(data.lengthX_),
			lengthY_(data.lengthY_),
			lengthZ_(data.lengthZ_),
			inFourierSpace_(data.inFourierSpace_),
			numPhysToFourier_(data.numPhysToFourier_),
			numFourierToPhys_(data.numFourierToPhys_),
			origin_(data.origin_),
			stepPhysX_(data.stepPhysX_),
			stepPhysY_(data.stepPhysY_),
			stepPhysZ_(data.stepPhysZ_),
			stepFourierX_(data.stepFourierX_),
			stepFourierY_(data.stepFourierY_),
			stepFourierZ_(data.stepFourierZ_),
      minPhys_(Vector3(-origin_.x,-origin_.y,-origin_.z)),
      maxPhys_(Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z)),
//      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
      minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
//      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
      maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
	{
		 planForward_  = fftw3d_create_plan(lengthX_, lengthY_, lengthZ_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
		 planBackward_ = fftw3d_create_plan(lengthX_, lengthY_, lengthZ_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								
	}

	FFT3D::FFT3D(Size ldnX, Size ldnY, Size ldnZ, double stepPhysX, double stepPhysY, double stepPhysZ, Vector3 origin, bool inFourierSpace)
		throw()
		: TRegularData3D<FFTW_COMPLEX>(Vector3(-origin.x,-origin.y,-origin.z), Vector3((((1<<ldnX)-1)*stepPhysX_)-origin.x,(((1<<ldnX)-1)*stepPhysY_)-origin.y,(((1<<ldnZ)-1)*stepPhysZ_)-origin.z), (1<<ldnX), (1<<ldnY), (1<<ldnZ)),
		  lengthX_(1<<ldnX),
			lengthY_(1<<ldnY),
			lengthZ_(1<<ldnZ),
			inFourierSpace_(inFourierSpace),
			numPhysToFourier_(0),
			numFourierToPhys_(0),
			origin_(origin),
			stepPhysX_(stepPhysX),
			stepPhysY_(stepPhysY),
			stepPhysZ_(stepPhysZ),
			stepFourierX_(2.*M_PI/(stepPhysX_*lengthX_)),
			stepFourierY_(2.*M_PI/(stepPhysY_*lengthY_)),
			stepFourierZ_(2.*M_PI/(stepPhysZ_*lengthZ_)),
      minPhys_(Vector3(-origin_.x,-origin_.y,-origin_.z)),
      maxPhys_(Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z)),
      //minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
      minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
      //maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
	{
		planForward_  = fftw3d_create_plan(lengthX_, lengthY_, lengthZ_, FFTW_FORWARD,  FFTW_ESTIMATE | FFTW_IN_PLACE);
	  planBackward_ = fftw3d_create_plan(lengthX_, lengthY_, lengthZ_, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);								
	}

	BALL_INLINE
	const FFT3D& FFT3D::operator = (const FFT3D& fft3D)
		throw()
	{
		clear();
		data = fft3D.data;
		lengthX_ = fft3D.lengthX_;
		lengthY_ = fft3D.lengthY_;
		lengthZ_ = fft3D.lengthZ_;
		origin_ = fft3D.origin_;
		stepPhysX_ = fft3D.stepPhysX_;
		stepPhysY_ = fft3D.stepPhysY_;
		stepPhysZ_ = fft3D.stepPhysZ_;
		stepFourierX_ = fft3D.stepFourierX_;
		stepFourierY_ = fft3D.stepFourierY_;
		stepFourierZ_ = fft3D.stepFourierZ_;
    minPhys_ = Vector3(-origin_.x,-origin_.y,-origin_.z);
    maxPhys_ = Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z);
    minFourier_ = Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_);
    maxFourier_ = Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_);
		numPhysToFourier_ = fft3D.numPhysToFourier_;
		numFourierToPhys_ = fft3D.numFourierToPhys_;
		planForward_ = fft3D.planForward_;
		planBackward_ = fft3D.planBackward_;
		
		return *this;
	}

	void FFT3D::clear()
		throw()
	{
		TRegularData3D<FFTW_COMPLEX>::clear();
	}

	void FFT3D::destroy()
		throw()
	{		
		// TRegularData3D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//TRegularData3D<FFTW_COMPLEX>::destroy();
		TRegularData3D<FFTW_COMPLEX>::clear();
		
		lengthX_ = 
		lengthY_ = 
		lengthZ_ = 0;
		origin_  = Vector3(0.,0.,0.);  
		stepPhysX_ =
		stepPhysY_ =
		stepPhysZ_ =
		stepFourierX_ = 
		stepFourierY_ = 
		stepFourierZ_ = 0.;
		numPhysToFourier_ = 
		numFourierToPhys_ = 0;
    minPhys_ =
    maxPhys_ =
    minFourier_ =
    maxFourier_ = Vector3(0.,0.,0.);
	}

	bool FFT3D::operator == (const FFT3D& fft3D) const
		throw()
	{
		if (lengthX_ == fft3D.lengthX_ &&
				lengthY_ == fft3D.lengthY_ &&
				lengthZ_ == fft3D.lengthZ_ &&
				origin_ == fft3D.origin_ &&
				stepPhysX_ == fft3D.stepPhysX_ &&
				stepPhysY_ == fft3D.stepPhysY_ &&
				stepPhysZ_ == fft3D.stepPhysZ_ &&
				stepFourierX_ == fft3D.stepFourierX_ &&
				stepFourierY_ == fft3D.stepFourierY_ &&
				stepFourierZ_ == fft3D.stepFourierZ_ &&
				minPhys_ == fft3D.minPhys_ &&
				maxPhys_ == fft3D.maxPhys_ &&
				minFourier_ == fft3D.minFourier_ &&
				maxFourier_ == fft3D.maxFourier_ &&
				numPhysToFourier_ == fft3D.numPhysToFourier_ &&
				numFourierToPhys_ == fft3D.numFourierToPhys_)
		{
			Vector3 min  = inFourierSpace_ ?  minFourier_  :   minPhys_;
			Vector3 max  = inFourierSpace_ ?  maxFourier_  :   maxPhys_;
			double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
			double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;	
			double stepZ = inFourierSpace_ ? stepFourierZ_ : stepPhysZ_;	
			
			for (double posX=min.x; posX<=max.x; posX+=stepX)
			{
				for (double posY=min.y; posY<=max.y; posY+=stepY)
				{
					for (double posZ=min.z; posZ<=max.z; posZ+=stepZ)
					{
						if (getData(Vector3(posX,posY,posZ)) != fft3D.getData(Vector3(posX,posY,posZ)))
						{
							return false;
						}
					}
				}
			}
			
			return true;
		}
	
		return false;
	}

	void FFT3D::doFFT()
		throw()
	{
//		fftwnd_one(planForward_, &(*data.begin()), 0);
		fftwnd_one(planForward_,data, 0); //TRegularData3D is *not* based on vector but on standard-arrays...
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	void FFT3D::doiFFT()
		throw()
	{
		//fftwnd_one(planBackward_, &(*data.begin()), 0);
		fftwnd_one(planBackward_,data,0);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

	bool FFT3D::translate(const Vector3& trans_origin)
		throw()
	{
		Position internalOriginX = (Position) rint(trans_origin.x*stepPhysX_);
		Position internalOriginY = (Position) rint(trans_origin.y*stepPhysY_);
		Position internalOriginZ = (Position) rint(trans_origin.z*stepPhysZ_);
		
		if ((internalOriginX <= lengthX_) && (internalOriginY <= lengthY_) && (internalOriginZ <= lengthZ_))
		{
			origin_.x = trans_origin.x;
			origin_.y = trans_origin.y;
			origin_.z = trans_origin.z;
			
	    minPhys_ = Vector3(-origin_.x,-origin_.y,-origin_.z);
			maxPhys_ = Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z);
			minFourier_ = Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_);
			maxFourier_ = Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_);
	 
			return true;
		}
		else
		{
			return false;
		}
	}

	bool FFT3D::setPhysStepWidth(double new_width_x, double new_width_y, double new_width_z)
		throw()
	{
		if ((new_width_x <= 0) || (new_width_y <= 0) || (new_width_z <= 0))
		{
			return false;
		}
		else
		{
			stepPhysX_ = new_width_x;
			stepPhysY_ = new_width_y;
			stepPhysZ_ = new_width_z;
			stepFourierX_ = 2.*M_PI/(stepPhysX_*lengthX_);
			stepFourierY_ = 2.*M_PI/(stepPhysY_*lengthY_);
			stepFourierZ_ = 2.*M_PI/(stepPhysZ_*lengthZ_);

			minPhys_ = Vector3(-origin_.x,-origin_.y,-origin_.z);
			maxPhys_ = Vector3(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y,((lengthZ_-1)*stepPhysZ_)-origin_.z);
			minFourier_ = Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_);
			maxFourier_ = Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_);
	
      return true;
		}
	}
	
	double FFT3D::getPhysStepWidthX() const
		throw()
	{
		return stepPhysX_;
	}

	double FFT3D::getPhysStepWidthY() const
		throw()
	{
		return stepPhysY_;
	}

	double FFT3D::getPhysStepWidthZ() const
		throw()
	{
		return stepPhysZ_;
	}
	
	double FFT3D::getFourierStepWidthX() const
		throw()
	{
		return stepFourierX_;
	}

	double FFT3D::getFourierStepWidthY() const
		throw()
	{
		return stepFourierY_;
	}

	double FFT3D::getFourierStepWidthZ() const
		throw()
	{
		return stepFourierZ_;
	}

	double FFT3D::getPhysSpaceMinX() const
		throw()
	{
    return minPhys_.x;
  }

	double FFT3D::getPhysSpaceMinY() const
		throw()
	{
    return minPhys_.y;
  }

	double FFT3D::getPhysSpaceMinZ() const
		throw()
	{
    return minPhys_.z;
  }

	double FFT3D::getPhysSpaceMaxX() const
		throw()
	{
    return maxPhys_.x;
 	}

	double FFT3D::getPhysSpaceMaxY() const
		throw()
	{
    return maxPhys_.y;
 	}

	double FFT3D::getPhysSpaceMaxZ() const
		throw()
	{
    return maxPhys_.z;
 	}
	
	double FFT3D::getFourierSpaceMinX() const
		throw()
	{
		return minFourier_.x;
	}

	double FFT3D::getFourierSpaceMinY() const
		throw()
	{
		return minFourier_.y;
	}

	double FFT3D::getFourierSpaceMinZ() const
		throw()
	{
		return minFourier_.z;
	}

	double FFT3D::getFourierSpaceMaxX() const
		throw()
	{
		return maxFourier_.x;
	}

	double FFT3D::getFourierSpaceMaxY() const
		throw()
	{
		return maxFourier_.y;
	}

	double FFT3D::getFourierSpaceMaxZ() const
		throw()
	{
		return maxFourier_.z;
	}

	Size FFT3D::getMaxXIndex() const
		throw()
	{
		return (lengthX_ - 1);
	}
	
	Size FFT3D::getMaxYIndex() const
		throw()
	{
		return (lengthY_ - 1);
	}
	
	Size FFT3D::getMaxZIndex() const
		throw()
	{
		return (lengthZ_ - 1);
	}
	
	Size FFT3D::getNumberOfInverseTransforms() const
		throw()
	{
		return numFourierToPhys_;
	}

	Vector3 FFT3D::getGridCoordinates(Position position) const
		throw()
	{
		if (!inFourierSpace_)
		{
			if (position > number_of_grid_points_)
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			Vector3 r;
			Position  x, y, z;

			z = position % lengthZ_;
			y = (position % (lengthY_ * lengthZ_)) / lengthZ_;
			x =  position / (lengthY_ * lengthZ_);

			r.set(-origin_.x + (float)x * stepPhysX_,
						-origin_.y + (float)y * stepPhysY_,
						-origin_.z + (float)z * stepPhysZ_);

			return r;
		}
		else
		{
			if (position > number_of_grid_points_)
			{
				throw Exception::OutOfGrid(__FILE__, __LINE__);
			}
		
			Vector3 r;
			Index x, y, z;
	
			z = position % lengthZ_;
			y = (position % (lengthY_ * lengthZ_)) / lengthZ_;
			x =  position / (lengthY_ * lengthZ_);

			if (x>=lengthX_/2.)
			{
				x-=lengthX_;
			}
			
			if (y>=lengthY_/2.)
			{
				y-=lengthY_;
			}

			if (z>=lengthZ_/2.)
			{
				z-=lengthZ_;
			}

			r.set((float)x * stepFourierX_,
						(float)y * stepFourierY_,
						(float)z * stepFourierZ_);

			return r;
		}
	}
	
	Complex FFT3D::getData(const Vector3& pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		double normalization=1.;

		if (!inFourierSpace_)
		{
			result = Complex((*this)[pos].re, (*this)[pos].im);
			normalization=1./((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
		}
		else
		{
			result = Complex((*this)[pos].re,(*this)[pos].im) * phase(pos);
			normalization=1./(sqrt(2.*M_PI))*(stepPhysX_*stepPhysY_*stepPhysZ_)/((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
		}

		result *= normalization;
		
		return result;
	}

	Complex FFT3D::getInterpolatedValue(const Vector3& pos) const
		throw(Exception::OutOfGrid)
	{
		Complex result;
		
		Vector3 min  = inFourierSpace_ ? minFourier_   :   minPhys_;
		Vector3 max  = inFourierSpace_ ? maxFourier_   :   maxPhys_;
		double stepX = inFourierSpace_ ? stepFourierX_ : stepPhysX_;
		double stepY = inFourierSpace_ ? stepFourierY_ : stepPhysY_;
		double stepZ = inFourierSpace_ ? stepFourierZ_ : stepPhysZ_;
		
		if (    (pos.x < min.x) || (pos.y < min.y) || (pos.z < min.z)
				 || (pos.x > max.x) || (pos.y > max.y) || (pos.z > max.z) )
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}

		Vector3 h(pos.x - min.x, pos.y - min.y, pos.z - min.z);
		double modX = fmod((double)h.x,stepX);
		double modY = fmod((double)h.y,stepY);
		double modZ = fmod((double)h.z,stepZ);

		if (modX==0 && modY==0 && modZ==0) // we are on the grid
		{
			return getData(pos);
		}

		double beforeX = floor(h.x/stepX)*stepX+min.x;
		double beforeY = floor(h.y/stepY)*stepY+min.y;
		double beforeZ = floor(h.z/stepZ)*stepZ+min.z;
		double afterX  =  ceil(h.x/stepX)*stepX+min.x;
		double afterY  =  ceil(h.y/stepY)*stepY+min.y;
		double afterZ  =  ceil(h.z/stepZ)*stepZ+min.z;
			
		double tx = (pos.x - beforeX)/stepX;
		double ty = (pos.y - beforeY)/stepY;
		double tz = (pos.z - beforeZ)/stepZ;

		result  = getData(Vector3(beforeX,beforeY,beforeZ))*(BALL_COMPLEX_PRECISION)((1.-tx)*(1.-ty)*(1.-tz));
		result += getData(Vector3(afterX, beforeY,beforeZ))*(BALL_COMPLEX_PRECISION)(    tx *(1.-ty)*(1.-tz));
		result += getData(Vector3(beforeX,afterY, beforeZ))*(BALL_COMPLEX_PRECISION)((1.-tx)*    ty *(1.-tz));
		result += getData(Vector3(beforeX,beforeY,afterZ ))*(BALL_COMPLEX_PRECISION)((1.-tx)*(1.-ty)*    tz );
		result += getData(Vector3(afterX, afterY, beforeZ))*(BALL_COMPLEX_PRECISION)(    tx *    ty *(1.-tz));
		result += getData(Vector3(afterX, beforeY,afterZ ))*(BALL_COMPLEX_PRECISION)(    tx *(1.-ty)*    tz );
		result += getData(Vector3(beforeX,afterY, afterZ ))*(BALL_COMPLEX_PRECISION)((1.-tx)*    ty *    tz );
		result += getData(Vector3(afterX, afterY, afterZ ))*(BALL_COMPLEX_PRECISION)(    tx *    ty *    tz );

		return result;
	}

	void FFT3D::setData(const Vector3& pos, Complex val)
		throw(Exception::OutOfGrid)
	{
		FFTW_COMPLEX dummy;
	
		if (!inFourierSpace_)
		{
			dummy.re = val.re*((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
			dummy.im = val.im*((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
	
			(*this)[pos]=dummy;
		}
		else
		{
			val*=phase(pos)*(BALL_COMPLEX_PRECISION)((sqrt(2*M_PI)/(stepPhysX_*stepPhysY_*stepPhysZ_)))
				             *((float)pow((float)(lengthX_*lengthY_*lengthZ_),(int)numFourierToPhys_));
			
			dummy.re = val.re;
			dummy.im = val.im;
			
			(*this)[pos]=dummy;
		}
	}

	FFTW_COMPLEX& FFT3D::operator[](const Vector3& pos)
		throw(Exception::OutOfGrid)
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j, k;

			i = (Index) rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) rint((pos.y+origin_.y)/stepPhysY_);
			k = (Index) rint((pos.z+origin_.z)/stepPhysZ_);
			
			internalPos = (k + (j + i*lengthY_)*lengthZ_);
			
			/*(Index) rint(       (pos.z+origin_.z)/stepPhysZ_
																  +	(	  (pos.y+origin_.y)/stepPhysY_
																	    + (pos.x+origin_.x)/stepPhysX_*lengthY_ 
																		) * lengthZ_
																); */
		}
		else
		{
			Index i, j, k;

			i = (Index) rint(pos.x/stepFourierX_);
			j = (Index) rint(pos.y/stepFourierY_);
			k = (Index) rint(pos.z/stepFourierZ_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			if (k<0)
			{
				k+=lengthZ_;
			}

			internalPos = (k + (j + i*lengthY_)*lengthZ_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_*lengthZ_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return data[internalPos];
	}

	const FFTW_COMPLEX& FFT3D::operator[](const Vector3& pos) const
		throw(Exception::OutOfGrid)
	{
		Index internalPos;

		if (!inFourierSpace_)
		{
			Index i, j, k;

			i = (Index) rint((pos.x+origin_.x)/stepPhysX_);
			j = (Index) rint((pos.y+origin_.y)/stepPhysY_);
			k = (Index) rint((pos.z+origin_.z)/stepPhysZ_);
			
			internalPos = (k + (j + i*lengthY_)*lengthZ_);
			
			/*(Index) rint(       (pos.z+origin_.z)/stepPhysZ_
																  +	(	  (pos.y+origin_.y)/stepPhysY_
																	    + (pos.x+origin_.x)/stepPhysX_*lengthY_ 
																		) * lengthZ_
																); */
		}
		else
		{
			Index i, j, k;

			i = (Index) rint(pos.x/stepFourierX_);
			j = (Index) rint(pos.y/stepFourierY_);
			k = (Index) rint(pos.z/stepFourierZ_);

			if (i<0)
			{
				i+=lengthX_;
			}

			if (j<0)
			{
				j+=lengthY_;
			}

			if (k<0)
			{
				k+=lengthZ_;
			}

			internalPos = (k + (j + i*lengthY_)*lengthZ_);
		}

		if ((internalPos < 0) || (internalPos>=(Index) (lengthX_*lengthY_*lengthZ_)))
		{
			throw Exception::OutOfGrid(__FILE__, __LINE__);
		}
		
		return data[internalPos];
	}

	FFTW_COMPLEX& FFT3D::operator[](const Position& pos)
		throw(Exception::OutOfGrid)
	{
		return data[pos];
	}

	const FFTW_COMPLEX& FFT3D::operator[](const Position& pos) const
		throw(Exception::OutOfGrid)
	{
		return data[pos];
	}

	Complex FFT3D::phase(const Vector3& pos) const
		throw()
	{
	  double phase = -2.*M_PI*(  (rint(pos.x/stepFourierX_))*(rint(origin_.x/stepPhysX_))
															/lengthX_
														+ (rint(pos.y/stepFourierY_))*(rint(origin_.y/stepPhysY_))
															/lengthY_
														+ (rint(pos.z/stepFourierZ_))*(rint(origin_.z/stepPhysZ_))
															/lengthZ_ );
	

		Complex result = Complex(cos(phase), sin(phase));
						
		return result;
	}

	bool FFT3D::isInFourierSpace() const
		throw()
	{
		return inFourierSpace_;
	}
	
	const TRegularData3D<Complex>& operator << (TRegularData3D<Complex>& to, const FFT3D& from)
		throw()
	{
		// first decide if the FFT3D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			Size lengthZ = from.getMaxZIndex()+1;
			
			TRegularData3D<Complex> newGrid(from.getPhysSpaceMinX(), 
																			from.getPhysSpaceMinY(),
																			from.getPhysSpaceMinZ(),
																			from.getPhysSpaceMaxX(),
																			from.getPhysSpaceMaxY(),
																			from.getPhysSpaceMaxZ(),
																			lengthX, lengthY, lengthZ);

			// and fill it
			double normalization=1./(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			FFTW_COMPLEX dataIn;
			Complex      dataOut;
			
			for (Position i=0; i<from.getSize(); i++)
			{
				Position x, y, z;

				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);

				dataIn     = from[i];
				dataOut.re = dataIn.re;
				dataOut.im = dataIn.im;
				
				newGrid[x + (y + z*lengthY)*lengthZ] = dataOut*normalization;
			}

			to = newGrid;

			return to;
		}
		else
		{
			// we are in Fourier space
			
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			Size lengthZ = from.getMaxZIndex()+1;
			float stepPhysX = from.getPhysStepWidthX();
			float stepPhysY = from.getPhysStepWidthY();
			float stepPhysZ = from.getPhysStepWidthZ();
			float stepFourierX = from.getFourierStepWidthX();
			float stepFourierY = from.getFourierStepWidthY();
			float stepFourierZ = from.getFourierStepWidthZ();


			
			TRegularData3D<Complex> newGrid(from.getFourierSpaceMinX(), 
																			from.getFourierSpaceMinY(),
																			from.getFourierSpaceMinZ(),
																			from.getFourierSpaceMaxX(),
																			from.getFourierSpaceMaxY(),
																			from.getFourierSpaceMaxZ(),
																			lengthX, lengthY, lengthZ);

			// and fill it
			double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));

			Index x, y, z;
			Vector3 r;
			FFTW_COMPLEX dataIn;
			Complex      dataOut;
	
			for (Position i=0; i<from.getSize(); i++)
			{
				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);

				if (x>lengthX/2.)
				{
					x-=lengthX;
				}

				if (y>lengthY/2.)
				{
					y-=lengthY;
				}

				if (z>lengthZ/2.)
				{
					z-=lengthZ;
				}

				r.set((float)x * stepFourierX,
							(float)y * stepFourierY,
							(float)z * stepFourierZ);

				dataIn = from[i];
				dataOut.re = dataIn.re;
				dataOut.im = dataIn.im;
				
				newGrid[x + (y + z*lengthY)*lengthZ] = dataOut*normalization*from.phase(r);
			}

			to = newGrid;

			return to;
		}
	}
	
	const RegularData3D& operator << (RegularData3D& to, const FFT3D& from)
		throw()
	{
		// first decide if the FFT3D data is in Fourier space.
		if (!from.isInFourierSpace())
		{
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			Size lengthZ = from.getMaxZIndex()+1;
			
			RegularData3D newGrid(from.getPhysSpaceMinX(), 
														from.getPhysSpaceMinY(),
														from.getPhysSpaceMinZ(),
														from.getPhysSpaceMaxX(),
														from.getPhysSpaceMaxY(),
														from.getPhysSpaceMaxZ(),
														lengthX, lengthY, lengthZ);

			// and fill it
			double normalization=1./(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));
			FFTW_COMPLEX dataIn;
			Complex      dataOut;
			
			for (Position i=0; i<from.getSize(); i++)
			{
				Position x, y, z;

				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);

				dataIn     = from[i];
				dataOut.re = dataIn.re;
				dataOut.im = dataIn.im;
				
				newGrid[x + (y + z*lengthY)*lengthZ] = dataOut.re*normalization;
			}

			to = newGrid;

			return to;
		}
		else
		{
			// we are in Fourier space
			
			// create a new grid
			Size lengthX = from.getMaxXIndex()+1;
			Size lengthY = from.getMaxYIndex()+1;
			Size lengthZ = from.getMaxZIndex()+1;
			float stepPhysX = from.getPhysStepWidthX();
			float stepPhysY = from.getPhysStepWidthY();
			float stepPhysZ = from.getPhysStepWidthZ();
			float stepFourierX = from.getFourierStepWidthX();
			float stepFourierY = from.getFourierStepWidthY();
			float stepFourierZ = from.getFourierStepWidthZ();


			
			RegularData3D newGrid(from.getFourierSpaceMinX(), 
														from.getFourierSpaceMinY(),
														from.getFourierSpaceMinZ(),
														from.getFourierSpaceMaxX(),
														from.getFourierSpaceMaxY(),
														from.getFourierSpaceMaxZ(),
														lengthX, lengthY, lengthZ);

			// and fill it
			double normalization=1./(sqrt(2.*M_PI))*(stepPhysX*stepPhysY*stepPhysZ)/(pow((float)(lengthX*lengthY*lengthZ),from.getNumberOfInverseTransforms()));

			Index x, y, z;
			signed int xp, yp, zp;
			Vector3 r;
			FFTW_COMPLEX dataIn;
			Complex      dataOut;
	
			for (Position i=0; i<from.getSize(); i++)
			{
				z =  i % lengthZ;
				y = (i % (lengthY * lengthZ)) / lengthZ;
				x =  i / (lengthY * lengthZ);
				
				xp = x;
				yp = y;
				zp = z;

				if (xp>=lengthX/2.)
				{
					xp-=(int)lengthX;
				}
				if (yp>=lengthY/2.)
				{
					yp-=(int)lengthY;
				}
				if (zp>=lengthZ/2.)
				{
					zp-=(int)lengthZ;
				}

				if (x>=lengthX/2.)
				{
					x-=(int)(lengthX/2.);
				}
				else
				{
					x+=(int)(lengthX/2.);
				}

				if (y>=lengthY/2.)
				{
					y-=(int)(lengthY/2.);
				}
				else
				{
					y+=(int)(lengthY/2.);
				}

				if (z>=lengthZ/2.)
				{
					z-=(int)(lengthZ/2.);
				}
				else
				{
					z+=(int)(lengthZ/2.);
				}

				r.set((float)xp * stepFourierX,
							(float)yp * stepFourierY,
							(float)zp * stepFourierZ);

				dataIn = from[i];
	
				dataOut.re = dataIn.re;
				dataOut.im = dataIn.im;

				newGrid[x + (y + z*lengthY)*lengthZ] = (dataOut*normalization*from.phase(r)).re;
			}

			to = newGrid;

			return to;
		}
	}

}
