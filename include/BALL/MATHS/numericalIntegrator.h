// $Id: numericalIntegrator.h,v 1.1 2000/12/06 19:05:07 anker Exp $

#ifndef BALL_MATHS_NUMERICALINTEGRATOR_H
#define BALL_MATHS_NUMERICALINTEGRATOR_H

#include <BALL/MATHS/function.h>

namespace BALL
{

	/** Numerical integrator class
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

		/// Clear method
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

		/** Get the value of the function at position {\em x}
				@param x the position at which {\tt function\_} is to be evaluated
				@return the value of {\tt function\_} at {\em x}
		*/
		DataType getValue(DataType x)
		{
			return function_(x);
		}

		/** Integrate the function numerically
				@param from lower limit of the integration
				@param to upper limit of the integration
				@return the value of the integral
		*/
		DataType integrate(DataType from, DataType to)
		{
		}

		//@}


		protected:

		//_ The function to be integrated
		Function function_;

	};

}

#endif // BALL_MATHS_NUMERICALINTEGRATOR_H
