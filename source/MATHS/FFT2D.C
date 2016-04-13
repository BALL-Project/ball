// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT2D.C,v 1.6.10.1 2006/03/02 18:52:58 anhi Exp $

#include <BALL/MATHS/FFT2D.h>

namespace BALL
{
	
#ifdef BALL_HAS_FFTW_DOUBLE

	template <>
	TFFT2D<DoubleTraits>::~TFFT2D()
	{
		// AR: destroy FFTW plans if there are any
		if (planCalculated_)
		{
			dataAdress_ = 0;
    	fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
		}
	}

	template <>
	TFFT2D<DoubleTraits>::TFFT2D(const TFFT2D &data)
		: TRegularData2D<Complex>(data),
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
		 // AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}								
	}

	// AR: ldn is not any longer the binary logarithm but the absolute number of grid points
	template <>
	TFFT2D<DoubleTraits>::TFFT2D(Size ldnX, Size ldnY, double stepPhysX, double stepPhysY, Vector2 origin, bool inFourierSpace)
		: TRegularData2D<Complex>(TRegularData2D<Complex>::IndexType((ldnX), (ldnY)), -origin, Vector2((((ldnX)-1)*stepPhysX),(((ldnX)-1)*stepPhysY))),
		  lengthX_(ldnX),
			lengthY_(ldnY),
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
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}						

		// AR: why comment?
/*	 	if (inFourierSpace)
		{
			lower_ = minFourier_;
			upper_ = maxFourier_;
			spacing_ = stepFourier_;
		} */
	}
	
	template <>
	void TFFT2D<DoubleTraits>::clear()
	{
		TRegularData2D<Complex>::clear();
		
		// AR: new version
		dataAdress_ = 0;
		dataLength_ = 0;
				
		// AR: if there are calculated plans, destroy them
		if (planCalculated_)
		{
    	fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
			planCalculated_ = false;
		}
	}
	

	template <>
	const TFFT2D<DoubleTraits>& TFFT2D<DoubleTraits>::operator = (const TFFT2D<DoubleTraits>& fft2D)
	{
		clear();
		//lower_ = fft2D.lower_;
		//upper_ = fft2D.upper_;
		//spacing_ = fft2D.spacing_;
		lengthX_ = fft2D.lengthX_;
		lengthY_ = fft2D.lengthY_;
		inFourierSpace_ = fft2D.inFourierSpace_;
		numPhysToFourier_ = fft2D.numPhysToFourier_;
		numFourierToPhys_ = fft2D.numFourierToPhys_;
		origin_ = fft2D.origin_;
		stepPhysX_ = fft2D.stepPhysX_;
		stepPhysY_ = fft2D.stepPhysY_;
		stepFourierX_ = fft2D.stepFourierX_;
		stepFourierY_ = fft2D.stepFourierY_;
	  minPhys_ = Vector2(-origin_.x,-origin_.y);
    maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
    maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);

		data_ = fft2D.data_;
		size_ = fft2D.size_;
				
		// AR: destroy old FFTW plans if there are any
		if (planCalculated_)
		{
			fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
		}
		
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}
		
		return *this;
	}

	template <>
	void TFFT2D<DoubleTraits>::destroy()
	{		
		// TRegularData2D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//		TRegularData2D<Complex>::destroy();
		TRegularData2D<Complex>::clear();
		
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
    
    // AR: new version
		dataAdress_ = 0;
		dataLength_ = 0;
				
		// AR: if there are calculated plans, destroy them
		if (planCalculated_)
		{
    	fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
			planCalculated_ = false;
		}
	}

	
	template <>
	void TFFT2D<DoubleTraits>::doFFT()
	{
		if ((dataAdress_ != &data_[0]) || (dataLength_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
    		fftw_destroy_plan(planForward_);
				fftw_destroy_plan(planBackward_);
			}
			
			dataLength_ = data_.size();
			if (dataLength_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				
				planForward_  = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft2D in that case?
				return;
			}				
		}
		
		fftw_execute(planForward_);
		
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT2D<DoubleTraits>::doiFFT()

	{
		if ((dataAdress_ != &data_[0]) || (dataLength_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
    		fftw_destroy_plan(planForward_);
				fftw_destroy_plan(planBackward_);
			}
			
			dataLength_ = data_.size();
			if (dataLength_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftw_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft2D in that case?
				return;
			}				
		}
		
		fftw_execute(planBackward_);
		
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

#endif // BALL_HAS_FFTW_DOUBLE

#ifdef BALL_HAS_FFTW_FLOAT

	template <>
	TFFT2D<FloatTraits>::~TFFT2D()
	{
		// AR: destroy FFTW plans if there are any
		if (planCalculated_)
		{
			dataAdress_ = 0;
			fftwf_destroy_plan(planForward_);
			fftwf_destroy_plan(planBackward_);
		}
	}

	template <>
	TFFT2D<FloatTraits>::TFFT2D(const TFFT2D &data)
		: TRegularData2D<Complex>(data),
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
		 // AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}								
	}

	// AR: ldn is not any longer the binary logarithm but the absolute number of grid points
	template <>
	TFFT2D<FloatTraits>::TFFT2D(Size ldnX, Size ldnY, double stepPhysX, double stepPhysY, Vector2 origin, bool inFourierSpace)
		: TRegularData2D<Complex>(TRegularData2D<Complex>::IndexType((ldnX), (ldnY)), -origin, Vector2((((ldnX)-1)*stepPhysX),(((ldnX)-1)*stepPhysY))),
		  lengthX_(ldnX),
			lengthY_(ldnY),
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
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}						

		// AR: why comment?
/*	 	if (inFourierSpace)
		{
			lower_ = minFourier_;
			upper_ = maxFourier_;
			spacing_ = stepFourier_;
		} */
	}
	
	template <>
	void TFFT2D<FloatTraits>::clear()
	{
		TRegularData2D<Complex>::clear();
		
		// AR: new version
		dataAdress_ = 0;
		dataLength_ = 0;
				
		// AR: if there are calculated plans, destroy them
		if (planCalculated_)
		{
			fftwf_destroy_plan(planForward_);
			fftwf_destroy_plan(planBackward_);
			planCalculated_ = false;
		}
	}
	

	template <>
	const TFFT2D<FloatTraits>& TFFT2D<FloatTraits>::operator = (const TFFT2D& fft2D)
	{
		clear();
		//lower_ = fft2D.lower_;
		//upper_ = fft2D.upper_;
		//spacing_ = fft2D.spacing_;
		lengthX_ = fft2D.lengthX_;
		lengthY_ = fft2D.lengthY_;
		inFourierSpace_ = fft2D.inFourierSpace_;
		numPhysToFourier_ = fft2D.numPhysToFourier_;
		numFourierToPhys_ = fft2D.numFourierToPhys_;
		origin_ = fft2D.origin_;
		stepPhysX_ = fft2D.stepPhysX_;
		stepPhysY_ = fft2D.stepPhysY_;
		stepFourierX_ = fft2D.stepFourierX_;
		stepFourierY_ = fft2D.stepFourierY_;
	  minPhys_ = Vector2(-origin_.x,-origin_.y);
    maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
    maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);

		data_ = fft2D.data_;
		size_ = fft2D.size_;
				
		// AR: destroy old FFTW plans if there are any
		if (planCalculated_)
		{
			fftwf_destroy_plan(planForward_);
			fftwf_destroy_plan(planBackward_);
		}
		
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}
		
		return *this;
	}

	template <>
	void TFFT2D<FloatTraits>::destroy()
	{		
		// TRegularData2D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//		TRegularData2D<Complex>::destroy();
		TRegularData2D<Complex>::clear();
		
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
    
    // AR: new version
		dataAdress_ = 0;
		dataLength_ = 0;
				
		// AR: if there are calculated plans, destroy them
		if (planCalculated_)
		{
			fftwf_destroy_plan(planForward_);
			fftwf_destroy_plan(planBackward_);
			planCalculated_ = false;
		}
	}

	
	template <>
	void TFFT2D<FloatTraits>::doFFT()
	{
		if ((dataAdress_ != &data_[0]) || (dataLength_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwf_destroy_plan(planForward_);
				fftwf_destroy_plan(planBackward_);
			}
			
			dataLength_ = data_.size();
			if (dataLength_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				
				planForward_  = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft2D in that case?
				return;
			}				
		}
		
		fftwf_execute(planForward_);	
		
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT2D<FloatTraits>::doiFFT()

	{
		if ((dataAdress_ != &data_[0]) || (dataLength_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwf_destroy_plan(planForward_);
				fftwf_destroy_plan(planBackward_);
			}
			
			dataLength_ = data_.size();
			if (dataLength_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwf_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft2D in that case?
				return;
			}				
		}
		
		fftwf_execute(planBackward_);
		
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}


#endif	// BALL_HAS_FFTW_FLOAT

#ifdef BALL_HAS_FFTW_LONG_DOUBLE

template <>
	TFFT2D<LongDoubleTraits>::~TFFT2D()

	{
		// AR: destroy FFTW plans if there are any
		if (planCalculated_)
		{
			dataAdress_ = 0;
			fftwl_destroy_plan(planForward_);
			fftwl_destroy_plan(planBackward_);
		}
	}

	template <>
	TFFT2D<LongDoubleTraits>::TFFT2D(const TFFT2D &data)
		: TRegularData2D<Complex>(data),
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
		 // AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}								
	}

	// AR: ldn is not any longer the binary logarithm but the absolute number of grid points
	template <>
	TFFT2D<LongDoubleTraits>::TFFT2D(Size ldnX, Size ldnY, double stepPhysX, double stepPhysY, Vector2 origin, bool inFourierSpace)
		: TRegularData2D<Complex>(TRegularData2D<Complex>::IndexType((ldnX), (ldnY)), -origin, Vector2((((ldnX)-1)*stepPhysX_),(((ldnX)-1)*stepPhysY_))),
		  lengthX_(ldnX),
			lengthY_(ldnY),
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
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}						

		// AR: why comment?
/*	 	if (inFourierSpace)
		{
			lower_ = minFourier_;
			upper_ = maxFourier_;
			spacing_ = stepFourier_;
		} */
	}
	
	template <>
	void TFFT2D<LongDoubleTraits>::clear()
	{
		TRegularData2D<Complex>::clear();
		
		// AR: new version
		dataAdress_ = 0;
		dataLength_ = 0;
				
		// AR: if there are calculated plans, destroy them
		if (planCalculated_)
		{
			fftwl_destroy_plan(planForward_);
			fftwl_destroy_plan(planBackward_);
			planCalculated_ = false;
		}
	}
	

	template <>
	const TFFT2D<LongDoubleTraits>& TFFT2D<LongDoubleTraits>::operator = (const TFFT2D& fft2D)
	{
		clear();
		//lower_ = fft2D.lower_;
		//upper_ = fft2D.upper_;
		//spacing_ = fft2D.spacing_;
		lengthX_ = fft2D.lengthX_;
		lengthY_ = fft2D.lengthY_;
		inFourierSpace_ = fft2D.inFourierSpace_;
		numPhysToFourier_ = fft2D.numPhysToFourier_;
		numFourierToPhys_ = fft2D.numFourierToPhys_;
		origin_ = fft2D.origin_;
		stepPhysX_ = fft2D.stepPhysX_;
		stepPhysY_ = fft2D.stepPhysY_;
		stepFourierX_ = fft2D.stepFourierX_;
		stepFourierY_ = fft2D.stepFourierY_;
	  minPhys_ = Vector2(-origin_.x,-origin_.y);
    maxPhys_ = Vector2(((lengthX_-1)*stepPhysX_)-origin_.x,((lengthY_-1)*stepPhysY_)-origin_.y);
	  minFourier_ = Vector2(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_);
    maxFourier_ = Vector2((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_);

		data_ = fft2D.data_;
		size_ = fft2D.size_;
				
		// AR: destroy old FFTW plans if there are any
		if (planCalculated_)
		{
			fftwl_destroy_plan(planForward_);
			fftwl_destroy_plan(planBackward_);
		}
		
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);	
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}
		
		return *this;
	}

	template <>
	void TFFT2D<LongDoubleTraits>::destroy()
	{		
		// TRegularData2D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//		TRegularData2D<Complex>::destroy();
		TRegularData2D<Complex>::clear();
		
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
    
    // AR: new version
		dataAdress_ = 0;
		dataLength_ = 0;
				
		// AR: if there are calculated plans, destroy them
		if (planCalculated_)
		{
			fftwl_destroy_plan(planForward_);
			fftwl_destroy_plan(planBackward_);
			planCalculated_ = false;
		}
	}

	
	template <>
	void TFFT2D<LongDoubleTraits>::doFFT()
	{
		if ((dataAdress_ != &data_[0]) || (dataLength_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwl_destroy_plan(planForward_);
				fftwl_destroy_plan(planBackward_);
			}
			
			dataLength_ = data_.size();
			if (dataLength_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				
				planForward_  = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
				
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft2D in that case?
				return;
			}				
		}
		
		fftwl_execute(planForward_);
		
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT2D<LongDoubleTraits>::doiFFT()
	{
		if ((dataAdress_ != &data_[0]) || (dataLength_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwl_destroy_plan(planForward_);
				fftwl_destroy_plan(planBackward_);
			}
			
			dataLength_ = data_.size();
			if (dataLength_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwl_plan_dft_2d(lengthX_, lengthY_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft2D in that case?
				return;
			}				
		}
		
		fftwl_execute(planBackward_);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

#endif // BALL_HAS_FFTW_LONG_DOUBLE
	
}

