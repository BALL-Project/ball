// $Id: rombergIntegrator.h,v 1.5 2001/07/14 12:56:14 amoll Exp $

#ifndef BALL_MATHS_ROMBERGINTEGRATOR_H
#define BALL_MATHS_ROMBERGINTEGRATOR_H

#ifndef BALL_MATHS_NUMERICALINTERGRATOR_H
# include <BALL/MATHS/numericalIntegrator.h>
#endif

namespace BALL
{
	/** A numerical integration of a BALL-Function using a Romberg integration scheme. 
			{\bf Definition:} \URL{BALL/MATHS/rombergIntegrator.h}
	*/
	template <typename Function, typename DataType>
	class RombergIntegrator
				: public NumericalIntegrator<Function, DataType>
  {
		public:

		BALL_CREATE(RombergIntegrator)

		/// @name Constructors and destructor.
		//@{

		/// Default constructor
		RombergIntegrator(float epsilon=1E-5, Size nsteps=1000)
			throw();
	
		/// Copy constructor
    RombergIntegrator(const RombergIntegrator& romint)
			throw();

		/// Destructor
		~RombergIntegrator()
			throw();

		//@}
		/// @name Assignment
		//@{

		/// Assignment operator
		const RombergIntegrator& operator = (const RombergIntegrator& romint)
			throw();
		
		/// Clear method
		virtual void clear()
			throw();

		/// Set the upper bound for the error we want to allow
		void setEpsilon(float eps)
			throw();

		/// Set the maximum number of steps we want to use in computation
		void setMaxNumSteps(Size mns)
			throw();

		//@}
		/// @name Predicates
		//@{
		
		/// Equality operator
		bool operator == (const RombergIntegrator& romint) const
			throw();

		//@}
		/// @name Accessors
		//@{
		
		/** Integrate the function numerically.
		 		@param from lower limit of the integration
				@param to upper limit of the integration
			  @return the value of the integral
		*/
		DataType integrate(DataType from, DataType to)
			throw();
		

		/** Integrate the function numerically using a simple trapezoid integration.
		 		This function is intended as a helper function for the computation of
				the romberg integration, but it can be used as a regular integrator as
				well, if speed is more important than reliability.
				@param h gives the width of each step
				@param from lower limit of the integration
				@param to upper limit of the integration
				@return the value of the integral
		*/
		DataType trapezoid(DataType h, DataType from, DataType to)
			throw();
		
		//@}
		
		protected:

		float epsilon_;
		Size maxNumSteps_;
		vector<DataType> result_;
	};

	template<typename Function, typename DataType>
	BALL_INLINE
	RombergIntegrator<Function, DataType>::RombergIntegrator(float eps, Size nsteps)
		throw() : NumericalIntegrator<Function, DataType>(), epsilon_(eps), maxNumSteps_(nsteps)
	{
		result_.reserve(maxNumSteps_ / 10);
	}

	template<typename Function, typename DataType>
	BALL_INLINE
  RombergIntegrator<Function, DataType>::RombergIntegrator(const 
												RombergIntegrator<Function, DataType>& romint)
		throw()
		:NumericalIntegrator<Function, DataType>(romint)
	{
	}

	template<typename Function, typename DataType>
	BALL_INLINE
	RombergIntegrator<Function, DataType>::~RombergIntegrator()
		throw()
	{
	}

	template<typename Function, typename DataType>
	BALL_INLINE
	const RombergIntegrator<Function, DataType>&
	RombergIntegrator<Function, DataType>::operator =
	(const RombergIntegrator<Function, DataType>& romint)
		throw()
	{
		function_ = romint.function_;
		maxNumSteps_ = romint.maxNumSteps_;
		epsilon_ = romint.epsilon_;
		result_ = romint.result_;
		return *this;
	}

	template<typename Function, typename DataType>
	BALL_INLINE
	void RombergIntegrator<Function, DataType>::clear()
		throw()
	{
		// Problem: function_.clear() might not exist... function_.clear();
	}

	template<typename Function, typename DataType>
	BALL_INLINE
	void RombergIntegrator<Function, DataType>::setEpsilon(float eps)
		throw()
	{
		epsilon_ = eps;
	}

	template<typename Function, typename DataType>
	BALL_INLINE
	void RombergIntegrator<Function, DataType>::setMaxNumSteps(Size nsteps)
		throw()
	{
		maxNumSteps_ = nsteps;
		result_.reserve(maxNumSteps_ / 10); // we hope that we do not
																				// need more than 1/10 - th of
                                        // the allowed operations
	}
	
	template<typename Function, typename DataType>
	BALL_INLINE
	bool RombergIntegrator<Function, DataType>::operator ==
	(const RombergIntegrator<Function, DataType> &romint) const
		throw()
	{
		return (    (function_ == romint.function_)
					 	 && (epsilon_  == romint.epsilon_ )
						 && (maxNumSteps_ == romint.maxNumSteps_)
						 && (result_      == romint.result_     ));
	}

	template<typename Function, typename DataType>
	BALL_INLINE
	DataType RombergIntegrator<Function, DataType>::trapezoid(DataType h, DataType from, DataType to)
		throw()
	{
		DataType sum=0;
		DataType helper = (to - from);
		int count;
		
		Size nsteps = (Size) (sqrt((helper*helper)/(h*h)));
		for (count=1; count<nsteps-1; count++)
		{
			sum +=function_(from+(count*h));

		}

		sum+=function_(from)+function_(to);
		sum*=h;

		return sum;
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	DataType RombergIntegrator<Function, DataType>::integrate
	(DataType from, DataType to)
		throw()
	{
		float h = 1.;
  	result_.push_back(trapezoid(h, from, to)); // this is the zeroth approximation
		
		int i=1;
		int j=0;
		int count = 0;
		DataType dev;

		do {
			result_.push_back(trapezoid(h/((float) i+1), from, to));
		
			for (j=1; j <= i; j++) {
				result_.push_back(result_[(i*(i+1))/2 + (j-1)] + 1. / (pow(4, j) - 1) * (result_[(i*(i+1))/2 + j-1]
			  	              - result_[((i-1)*i)/2+j-1]));
				count++;
			};
			i++;
			dev = result_[((i-2)*(i-1))/2+(i-2)] - result_[((i-1)*(i))/2+(i-1)];
		} while ( (sqrt(dev*dev) > epsilon_) 
	  	       && (count < maxNumSteps_));

		return (result_[((i-1)*(i))/2 + (i-1)]);
	}
	
} // namespace BALL
	
#endif // BALL_MATHS_ROMBERGINTEGRATOR_H
