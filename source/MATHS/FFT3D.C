// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT3D.C,v 1.7.10.2 2006/03/01 19:04:24 anhi Exp $
//

#include <BALL/MATHS/FFT3D.h>

namespace BALL
{
	
#ifdef BALL_HAS_FFTW_DOUBLE

	template <>
	TFFT3D<DoubleTraits>::~TFFT3D()
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
	TFFT3D<DoubleTraits>::TFFT3D(const TFFT3D &data)
		: TRegularData3D<Complex>(data),
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
      
      // AR: rows changed
      
      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
//      minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
//      maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
	{
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																						reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
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
	TFFT3D<DoubleTraits>::TFFT3D(Size ldnX, Size ldnY, Size ldnZ, double stepPhysX, double stepPhysY, double stepPhysZ, Vector3 origin, bool inFourierSpace)
		: TRegularData3D<Complex>(TRegularData3D<Complex>::IndexType((ldnX), (ldnY), (ldnZ)), -origin, Vector3((((ldnX)-1)*stepPhysX),(((ldnY)-1)*stepPhysY),(((ldnZ)-1)*stepPhysZ))),
		  lengthX_(ldnX),
			lengthY_(ldnY),
			lengthZ_(ldnZ),
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
      
      // AR: rows changed
      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
      //minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      //maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
	{
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																						reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																						reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}
	}
	
	template <>
	void TFFT3D<DoubleTraits>::clear()
	{
		TRegularData3D<Complex>::clear();
		
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
	const TFFT3D<DoubleTraits>& TFFT3D<DoubleTraits>::operator = (const TFFT3D& fft3D)
	{
		clear();
		TRegularData3D<Complex>::operator = (fft3D);
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
			planForward_  = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																						reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
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
	void TFFT3D<DoubleTraits>::destroy()
	{		
		// TRegularData3D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//TRegularData3D<Complex>::destroy();
		TRegularData3D<Complex>::clear();
		
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
	void TFFT3D<DoubleTraits>::doFFT()
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
				planForward_  = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																						reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																						reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft3D in that case?
				return;
			}				
		}
		
		fftw_execute(planForward_);
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT3D<DoubleTraits>::doiFFT()
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
				planForward_  = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftw_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftw_complex*>(dataAdress_),
																							reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft3D in that case?
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
	TFFT3D<FloatTraits>::~TFFT3D()
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
	TFFT3D<FloatTraits>::TFFT3D(const TFFT3D &data)
		: TRegularData3D<Complex>(data),
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
      
      // AR: rows changed
      
      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
//      minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
//      maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
	{
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																						reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
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
	TFFT3D<FloatTraits>::TFFT3D(Size ldnX, Size ldnY, Size ldnZ, double stepPhysX, double stepPhysY, double stepPhysZ, Vector3 origin, bool inFourierSpace)
		: TRegularData3D<Complex>(TRegularData3D<Complex>::IndexType((ldnX), (ldnY), (ldnZ)), -origin, Vector3((((ldnX)-1)*stepPhysX),(((ldnY)-1)*stepPhysY),(((ldnZ)-1)*stepPhysZ))),
		  lengthX_(ldnX),
			lengthY_(ldnY),
			lengthZ_(ldnZ),
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
      
      // AR: rows changed
      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
      //minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      //maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
	{
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																						reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																						reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}
	}
	
	template <>
	void TFFT3D<FloatTraits>::clear()
	{
		TRegularData3D<Complex>::clear();
		
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
	const TFFT3D<FloatTraits>& TFFT3D<FloatTraits>::operator = (const TFFT3D& fft3D)
	{
		clear();
		TRegularData3D<Complex>::operator = (fft3D);
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
			planForward_  = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																						reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
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
	void TFFT3D<FloatTraits>::destroy()
	{		
		// TRegularData3D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//TRegularData3D<Complex>::destroy();
		TRegularData3D<Complex>::clear();
		
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
	void TFFT3D<FloatTraits>::doFFT()

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
				planForward_  = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																						reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																						reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft3D in that case?
				return;
			}				
		}
		
		fftwf_execute(planForward_);	
		
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT3D<FloatTraits>::doiFFT()
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
				
				planForward_  = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwf_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwf_complex*>(dataAdress_),
																							reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft3D in that case?
				return;
			}				
		}
		
		fftwf_execute(planBackward_);
		
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}
#endif // BALL_HAS_FFTW_FLOAT


#ifdef BALL_HAS_FFTW_LONG_DOUBLE

	template <>
	TFFT3D<LongDoubleTraits>::~TFFT3D()
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
	TFFT3D<LongDoubleTraits>::TFFT3D(const TFFT3D &data)
		: TRegularData3D<Complex>(data),
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
      
      // AR: rows changed
      
      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
//      minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
//      maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
	{
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																						reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
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
	TFFT3D<LongDoubleTraits>::TFFT3D(Size ldnX, Size ldnY, Size ldnZ, double stepPhysX, double stepPhysY, double stepPhysZ, Vector3 origin, bool inFourierSpace)
		: TRegularData3D<Complex>(TRegularData3D<Complex>::IndexType((ldnX), (ldnY), (ldnZ)), -origin, Vector3((((ldnX)-1)*stepPhysX),(((ldnY)-1)*stepPhysY),(((ldnZ)-1)*stepPhysZ))),
		  lengthX_(ldnX),
			lengthY_(ldnY),
			lengthZ_(ldnZ),
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
      
      // AR: rows changed
      minFourier_(Vector3(-(lengthX_/2.-1)*stepFourierX_,-(lengthY_/2.-1)*stepFourierY_,-(lengthZ_/2.-1)*stepFourierZ_)),
      //minFourier_(Vector3(-(lengthX_/2.)*stepFourierX_,-(lengthY_/2.)*stepFourierY_,-(lengthZ_/2.)*stepFourierZ_)),
      //maxFourier_(Vector3((lengthX_/2.-1)*stepFourierX_,(lengthY_/2.-1)*stepFourierY_,(lengthZ_/2.-1)*stepFourierZ_))
      maxFourier_(Vector3((lengthX_/2.)*stepFourierX_,(lengthY_/2.)*stepFourierY_,(lengthZ_/2.)*stepFourierZ_))
	{
		// AR: new code for FFTW3
		dataLength_ = data_.size();
		
		if (dataLength_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																						reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																						reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			dataAdress_ = 0;
			planCalculated_ = false;
		}
	}
	
	template <>
	void TFFT3D<LongDoubleTraits>::clear()
	{
		TRegularData3D<Complex>::clear();
		
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
	const TFFT3D<LongDoubleTraits>& TFFT3D<LongDoubleTraits>::operator = (const TFFT3D& fft3D)
	{
		clear();
		TRegularData3D<Complex>::operator = (fft3D);
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
			planForward_  = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																						reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
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
	void TFFT3D<LongDoubleTraits>::destroy()
	{		
		// TRegularData3D seems to be missing a destroy routine...
		// until this is fixed, we will use clear() instead
		//TRegularData3D<Complex>::destroy();
		TRegularData3D<Complex>::clear();
		
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
	void TFFT3D<LongDoubleTraits>::doFFT()
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
				planForward_  = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																						reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																						reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft3D in that case?
				return;
			}				
		}
		
		fftwl_execute(planForward_);
		
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT3D<LongDoubleTraits>::doiFFT()
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
				planForward_  = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_FORWARD, FFTW_ESTIMATE);
				planBackward_ = fftwl_plan_dft_3d(lengthX_, lengthY_, lengthZ_, reinterpret_cast<fftwl_complex*>(dataAdress_),
																							reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft3D in that case?
				return;
			}				
		}
		
		fftwl_execute(planBackward_);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

#endif // BALL_HAS_FFTW_LONG_DOUBLE

}
