// $Id: numericalIntegrator.h,v 1.3 2001/04/04 14:11:42 anhi Exp $

#ifndef BALL_MATHS_NUMERICALINTEGRATOR_H
#define BALL_MATHS_NUMERICALINTEGRATOR_H

#include <BALL/MATHS/function.h>

namespace BALL
{

	/** Numerical integrator class.
			\\
			{\bf Definition:} \URL{BALL/MATHS/numericalIntegrator.h}
	*/
	template <typename Function, typename DataType>
	class NumericalIntegrator
	{

		public:

		BALL_CREATE(NumericalIntegrator)

		/// @name Constructors and destructor.
		//@{
		
		/// Default constructor
		NumericalIntegrator()
			throw();
		
		/// Copy constructor 
		NumericalIntegrator(const NumericalIntegrator& nint)
			throw();
		
		/// Destructor
		~NumericalIntegrator()
			throw();

		//@}


		/// @name Assignment
		//@{

		/// Assignment operator
		const NumericalIntegrator& operator = (const NumericalIntegrator& nint)
			throw();

		/// Clear method, might be unusable...
		virtual void clear()
			throw();

		//@}
		
		
		/// @name Predicates
		//@{

		/// Equality operator 
		bool operator == (const NumericalIntegrator& nint) const
			throw();

		//@}


		/// @name Accessors
		//@{

		/** set the function to be integrated
				@param the function to be assigned
		*/
		void setFunction(const Function& function)
			throw();

		/** get the function to be integrated
				@return a const reference to the actual function
		*/
		const Function& getFunction() const
			throw();

		/** get the function to be integrated
				@return a mutable reference to the actual function
		*/
		Function& getFunction()
			throw();

		/** Get the value of the function at position {\em x}
				@param x the position at which {\tt function\_} is to be evaluated
				@return the value of {\tt function\_} at {\em x}
		*/
		DataType getValue(DataType x) const
			throw();

		/** Integrate the function numerically
				@param from lower limit of the integration
				@param to upper limit of the integration
				@return the value of the integral
		*/
		DataType integrate(DataType from, DataType to)
			throw();

		//@}


		protected:

		//_ The function to be integrated
		Function function_;

	};


	template<typename Function, typename DataType>
	BALL_INLINE
	NumericalIntegrator<Function, DataType>::NumericalIntegrator()
		throw()
		: function_()
	{
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	NumericalIntegrator<Function, DataType>::NumericalIntegrator(const NumericalIntegrator<Function, DataType>& nint)
		throw()
		: function_(nint.function_)
	{
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	NumericalIntegrator<Function, DataType>::~NumericalIntegrator()
		throw()
	{
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	const NumericalIntegrator<Function, DataType>&
	NumericalIntegrator<Function, DataType>::operator =
	(const NumericalIntegrator<Function, DataType>& nint)
		throw()
	{
		function_ = nint.function_;
		return *this;
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	void NumericalIntegrator<Function, DataType>::clear()
		throw()
	{
		// BAUSTELLE: Je nach template gibt es clear() in function_ gar nicht.
		// That's why I commented it out... :-) function_.clear();
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	void NumericalIntegrator<Function, DataType>::setFunction
	(const Function& function)
		throw()
	{	
		function_ = function;
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	Function& NumericalIntegrator<Function, DataType>::getFunction()
		throw()
	{
		return function;
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	const Function& NumericalIntegrator<Function, DataType>::getFunction()
	const
		throw()
	{
		return function;
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	bool NumericalIntegrator<Function, DataType>::operator ==
	(const NumericalIntegrator<Function, DataType>& nint) const
		throw()
	{
		return (function_ == nint.function_);
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	DataType NumericalIntegrator<Function, DataType>::getValue(DataType x) const
		throw()
	{
		return function_(x);
	}


	template<typename Function, typename DataType>
	BALL_INLINE
	DataType NumericalIntegrator<Function, DataType>::integrate
	(DataType from, DataType to)
		throw()
	{
	}

}

#endif // BALL_MATHS_NUMERICALINTEGRATOR_H
