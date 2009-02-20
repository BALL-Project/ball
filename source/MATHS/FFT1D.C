// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FFT1D.C,v 1.7 2005/02/02 10:50:48 anhi Exp $
//

#include <BALL/MATHS/FFT1D.h>

namespace BALL
{
	// rest see FFT1D.h
	
	
#ifdef BALL_HAS_FFTW_DOUBLE

	template <>
	TFFT1D<DoubleTraits>::~TFFT1D()
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
	TFFT1D<DoubleTraits>::TFFT1D(const TFFT1D<DoubleTraits> &data)
		: TRegularData1D<Complex>(data),
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
		// AR: new version for FFTW3
		if (length_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_),  reinterpret_cast<fftw_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_),reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			planCalculated_ = false;
			dataAdress_ = 0;
		}
	}
	
	
	// AR: ldn is not any longer the binary logarithm but the absolute number of grid points
	template <>
	TFFT1D<DoubleTraits>::TFFT1D(Size ldn, double stepPhys, double origin, bool inFourierSpace)
		: TRegularData1D<Complex>(-origin, ((ldn)-1)*stepPhys, stepPhys),
		  length_(ldn),
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
		// AR: new version for FFTW3
		dataAdress_ = &data_[0];
		planCalculated_ = true;
		planForward_  = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																		reinterpret_cast<fftw_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
		planBackward_ = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																		reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
	 	if (inFourierSpace)
		{
			origin_ = minFourier_;
			dimension_ = maxFourier_ - minFourier_;
			spacing_ = stepFourier_;
		}
	}
	
	
	
	template <>
	void TFFT1D<DoubleTraits>::clear()
	{
		TRegularData1D<Complex>::clear();
		
		// AR: new version for FFTW3
    dataAdress_ = 0;
    length_ = 0;
    
    if (planCalculated_)
    {
			fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
			planCalculated_ = false;
    }
	}
	
	
		
	template <>
	const TFFT1D<DoubleTraits>& TFFT1D<DoubleTraits>::operator = (const TFFT1D<DoubleTraits>& fft1d)
	{
		clear();
		origin_ = fft1d.origin_;
		dimension_ = fft1d.dimension_;
		spacing_ = fft1d.spacing_;
		origin_ = fft1d.origin_;
		stepPhys_ = fft1d.stepPhys_;
		stepFourier_ = fft1d.stepFourier_;

		data_ = fft1d.data_;
		length_ = fft1d.data_.size();
		
		inFourierSpace_ = fft1d.inFourierSpace_;

    minPhys_ = ((-1.)*origin_);
    maxPhys_ = (((length_-1)*stepPhys_)-origin_);
    minFourier_ = ((-1.)*(length_/2.-1)*stepFourier_);
    maxFourier_ = ((length_/2.)*stepFourier_);

    numPhysToFourier_ = fft1d.numPhysToFourier_;
		numFourierToPhys_ = fft1d.numFourierToPhys_;
		
		// AR: new code for FFTW3
		
		// if there are old plans, destroy them first...
		if (planCalculated_)
		{
			fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
		}
		
		if (length_ != 0)
		{
			planCalculated_ = true;
			dataAdress_ = &data_[0];
		
			// ...and calculate new ones
			planForward_  = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																				reinterpret_cast<fftw_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
			planBackward_ = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																				reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			planCalculated_ = false;
			dataAdress_ = 0;
		}
		
		return *this;
	}


	template <>
	void TFFT1D<DoubleTraits>::destroy()
	{		
		TRegularData1D<Complex >::clear();
		
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
    
    // AR: new version for FFTW3
    dataAdress_ = 0;
    
    if (planCalculated_)
    {
			fftw_destroy_plan(planForward_);
			fftw_destroy_plan(planBackward_);
			planCalculated_ = false;
    }
	}
	
	template <>
	void TFFT1D<DoubleTraits>::doFFT()
	{
		// AR: new version, if start adress or vector size has changed, recalculate the plans
		if ((dataAdress_ != &data_[0]) || (length_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftw_destroy_plan(planForward_);
				fftw_destroy_plan(planBackward_);
			}
			
			length_ = data_.size();
			if (length_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																				reinterpret_cast<fftw_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
				planBackward_ = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																				reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft1d in that case?
				return;
			}				
		}
		
		fftw_execute(planForward_);
		
		// AR: old version
		//fftw_one(planForward_, reinterpret_cast<fftw_complex*>(&(*data_.begin())), 0);
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}
	
	
	
	template <>
	void TFFT1D<DoubleTraits>::doiFFT()
	{
		// AR: new version, if vector size or start adress has changed, recalculate the plans
		if ((dataAdress_ != &data_[0]) || (length_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftw_destroy_plan(planForward_);
				fftw_destroy_plan(planBackward_);
			}
			
			length_ = data_.size();
			if (length_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																				reinterpret_cast<fftw_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
				planBackward_ = fftw_plan_dft_1d(length_, reinterpret_cast<fftw_complex*>(dataAdress_), 
																				reinterpret_cast<fftw_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft1d in that case?
				return;
			}				
		}
		
		fftw_execute(planBackward_);
		
		// AR: old version
		// Is this cast vector -> array portable?
		//fftw_one(planBackward_, reinterpret_cast<fftw_complex*>(&(*data_.begin())), 0);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}
	
#endif // BALL_HAS_FFTW_DOUBLE	

#ifdef BALL_HAS_FFTW_FLOAT	

	template <>
	TFFT1D<FloatTraits>::~TFFT1D()
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
	TFFT1D<FloatTraits>::TFFT1D(const TFFT1D<FloatTraits> &data)
		: TRegularData1D<Complex>(data),
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
		// AR: new version for FFTW3
		if (length_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), 
																			reinterpret_cast<fftwf_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), 
																			reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			planCalculated_ = false;
			dataAdress_ = 0;
		}
	}
	
		
	// AR: ldn is not any longer the binary logarithm but the absolute number of grid points
	template <>
	TFFT1D<FloatTraits>::TFFT1D(Size ldn, double stepPhys, double origin, bool inFourierSpace)
		: TRegularData1D<Complex >(-origin, ((ldn)-1)*stepPhys, stepPhys),
		  length_(ldn),
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
		// AR: new version for FFTW3
		dataAdress_ = &data_[0];
		planCalculated_ = true;
		planForward_  = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), 
																		reinterpret_cast<fftwf_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
		planBackward_ = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), 
																		reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);

	 	if (inFourierSpace)
		{
			origin_ = minFourier_;
			dimension_ = maxFourier_ - minFourier_;
			spacing_ = stepFourier_;
		}
	}
	
	
		
	template <>
	void TFFT1D<FloatTraits>::clear()
	{
		TRegularData1D<Complex>::clear();
		
		// AR: new version for FFTW3
    dataAdress_ = 0;
    length_ = 0; 
    if (planCalculated_)
    {
			fftwf_destroy_plan(planForward_);
			fftwf_destroy_plan(planBackward_);
			planCalculated_ = false;
    }
	}
	
	template <>
	const TFFT1D<FloatTraits>& TFFT1D<FloatTraits>::operator = (const TFFT1D<FloatTraits>& fft1d)
	{
		clear();
		origin_ = fft1d.origin_;
		dimension_ = fft1d.dimension_;
		spacing_ = fft1d.spacing_;
		origin_ = fft1d.origin_;
		stepPhys_ = fft1d.stepPhys_;
		stepFourier_ = fft1d.stepFourier_;
    minPhys_ = ((-1.)*origin_);
    maxPhys_ = (((length_-1)*stepPhys_)-origin_);
    minFourier_ = ((-1.)*(length_/2.-1)*stepFourier_);
    maxFourier_ = ((length_/2.)*stepFourier_);
    numPhysToFourier_ = fft1d.numPhysToFourier_;
		numFourierToPhys_ = fft1d.numFourierToPhys_;
		
		// AR: new code for FFTW3
		
		// if there are old plans, destroy them first...
		if (planCalculated_)
		{
			fftwf_destroy_plan(planForward_);
			fftwf_destroy_plan(planBackward_);
		}
		
		data_ = fft1d.data_;
		length_ = fft1d.data_.size();
		if (length_ != 0)
		{
			planCalculated_ = true;
			dataAdress_ = &data_[0];
		
			// ...and calculate new ones
			planForward_  = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), 
																				reinterpret_cast<fftwf_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
			planBackward_ = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), 
																				reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			planCalculated_ = false;
			dataAdress_ = 0;
		}
		
		return *this;
	}
	
	
	
	template <>
	void TFFT1D<FloatTraits>::destroy()
	{		
		TRegularData1D<Complex >::clear();
		
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
		// AR: new version for FFTW3
		dataAdress_ = 0;
		if (planCalculated_)
		{
				fftwf_destroy_plan(planForward_);
				fftwf_destroy_plan(planBackward_);
				planCalculated_ = false;
		}
	}
	
	
	template <>
	void TFFT1D<FloatTraits>::doFFT()
	{
		// AR: new version, if start adress or vector size has changed, recalculate the plans
		if ((dataAdress_ != &data_[0]) || (length_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwf_destroy_plan(planForward_);
				fftwf_destroy_plan(planBackward_);
			}
			
			length_ = data_.size();
			if (length_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), reinterpret_cast<fftwf_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
				planBackward_ = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_),  reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft1d in that case?
				return;
			}				
		}
		
		fftwf_execute(planForward_);	

		// AR: old version
		//fftw_one(planForward_, reinterpret_cast<fftw_complex*>(&(*data_.begin())), 0);
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}

	template <>
	void TFFT1D<FloatTraits>::doiFFT()
	{
		// AR: new version, if vector size or start adress has changed, recalculate the plans
		if ((dataAdress_ != &data_[0]) || (length_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwf_destroy_plan(planForward_);
				fftwf_destroy_plan(planBackward_);
			}
			
			length_ = data_.size();
			if (length_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), reinterpret_cast<fftwf_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
				planBackward_ = fftwf_plan_dft_1d(length_, reinterpret_cast<fftwf_complex*>(dataAdress_), reinterpret_cast<fftwf_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft1d in that case?
				return;
			}				
		}
		
		fftwf_execute(planBackward_);
		
		// AR: old version
		// Is this cast vector -> array portable?
		//fftw_one(planBackward_, reinterpret_cast<fftw_complex*>(&(*data_.begin())), 0);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}

#endif // BALL_HAS_FFTW_FLOAT	

#ifdef BALL_HAS_FFTW_LONG_DOUBLE

	template <>
	TFFT1D<LongDoubleTraits>::~TFFT1D()
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
	TFFT1D<LongDoubleTraits>::TFFT1D(const TFFT1D<LongDoubleTraits> &data)
		: TRegularData1D<Complex>(data),
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
		// AR: new version for FFTW3
		if (length_ != 0)
		{
			dataAdress_ = &data_[0];
			planCalculated_ = true;
			planForward_  = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_),  reinterpret_cast<fftwl_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			planCalculated_ = false;
			dataAdress_ = 0;
		}
	}
	
	// AR: ldn is not any longer the binary logarithm but the absolute number of grid points
	template <>
	TFFT1D<LongDoubleTraits>::TFFT1D(Size ldn, double stepPhys, double origin, bool inFourierSpace)
		: TRegularData1D<Complex>(-origin, ((ldn)-1)*stepPhys, stepPhys),
		  length_(ldn),
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
		// AR: new version for FFTW3
		dataAdress_ = &data_[0];
		planCalculated_ = true;
		planForward_  = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), reinterpret_cast<fftwl_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
		planBackward_ = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);

	 	if (inFourierSpace)
		{
			origin_ = minFourier_;
			dimension_ = maxFourier_ - minFourier_;
			spacing_ = stepFourier_;
		}
	}
	
	template <>
	void TFFT1D<LongDoubleTraits>::clear()
	{
		TRegularData1D<Complex>::clear();
		
		// AR: new version for FFTW3
    dataAdress_ = 0;
    length_ = 0;
    if (planCalculated_)
    {
			fftwl_destroy_plan(planForward_);
			fftwl_destroy_plan(planBackward_);
			planCalculated_ = false;
    }
	}
	
	template <>
	const TFFT1D<LongDoubleTraits>& TFFT1D<LongDoubleTraits>::operator = (const TFFT1D<LongDoubleTraits>& fft1d)
	{
		clear();
		origin_ = fft1d.origin_;
		dimension_ = fft1d.dimension_;
		spacing_ = fft1d.spacing_;
		origin_ = fft1d.origin_;
		stepPhys_ = fft1d.stepPhys_;
		stepFourier_ = fft1d.stepFourier_;
		minPhys_ = ((-1.)*origin_);
		maxPhys_ = (((length_-1)*stepPhys_)-origin_);
		minFourier_ = ((-1.)*(length_/2.-1)*stepFourier_);
		maxFourier_ = ((length_/2.)*stepFourier_);
		numPhysToFourier_ = fft1d.numPhysToFourier_;
		numFourierToPhys_ = fft1d.numFourierToPhys_;
		
		// AR: new code for FFTW3
		
		// if there are old plans, destroy them first...
		if (planCalculated_)
		{
			fftwl_destroy_plan(planForward_);
			fftwl_destroy_plan(planBackward_);
		}
		
		data_ = fft1d.data_;
		length_ = fft1d.data_.size();
		if (length_ != 0)
		{
			planCalculated_ = true;
			dataAdress_ = &data_[0];
		
			// ...and calculate new ones
			planForward_  = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_),  reinterpret_cast<fftwl_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
			planBackward_ = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			planCalculated_ = false;
			dataAdress_ = 0;
		}
		
		return *this;
	}
	
	template <>
	void TFFT1D<LongDoubleTraits>::destroy()
	{		
		TRegularData1D<Complex >::clear();
		
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
    // AR: new version for FFTW3
    dataAdress_ = 0;
    if (planCalculated_)
    {
		fftwl_destroy_plan(planForward_);
		fftwl_destroy_plan(planBackward_);
		planCalculated_ = false;
    }
	}
	
	template <>
	void TFFT1D<LongDoubleTraits>::doFFT()
	{
		// AR: new version, if start adress or vector size has changed, recalculate the plans
		if ((dataAdress_ != &data_[0]) || (length_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwl_destroy_plan(planForward_);
				fftwl_destroy_plan(planBackward_);
			}
			
			length_ = data_.size();
			if (length_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_),  reinterpret_cast<fftwl_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
				planBackward_ = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), 
				reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft1d in that case?
				return;
			}				
		}
		
		fftwl_execute(planForward_);
		
		// AR: old version
		//fftw_one(planForward_, reinterpret_cast<fftw_complex*>(&(*data_.begin())), 0);
		inFourierSpace_ = true;
		numPhysToFourier_++;
	}
	
	template <>
	void TFFT1D<LongDoubleTraits>::doiFFT()
	{
		// AR: new version, if vector size or start adress has changed, recalculate the plans
		if ((dataAdress_ != &data_[0]) || (length_ != data_.size()))
		{
			// AR: if there are any old plans, destroy them
			if (planCalculated_)
			{
				fftwl_destroy_plan(planForward_);
				fftwl_destroy_plan(planBackward_);
			}
			
			length_ = data_.size();
			if (length_ != 0)
			{
				dataAdress_ = &data_[0];
				planCalculated_ = true;
				planForward_  = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), reinterpret_cast<fftwl_complex*>(dataAdress_),  FFTW_FORWARD,  FFTW_ESTIMATE);
				planBackward_ = fftwl_plan_dft_1d(length_, reinterpret_cast<fftwl_complex*>(dataAdress_), reinterpret_cast<fftwl_complex*>(dataAdress_), FFTW_BACKWARD, FFTW_ESTIMATE);
			}
			else
			{
				dataAdress_ = 0;
				planCalculated_ = false;
				
				// AR: if there are no data return without doing anything but update 'dataAdress_' and the flag
				//     Clear fft1d in that case?
				return;
			}
		}
		
		fftwl_execute(planBackward_);
		
		// AR: old version
		// Is this cast vector -> array portable?
		//fftw_one(planBackward_, reinterpret_cast<fftw_complex*>(&(*data_.begin())), 0);
		inFourierSpace_ = false;
		numFourierToPhys_++;
	}
	
#endif // BALL_HAS_FFTW_LONG_DOUBLE
	
}
