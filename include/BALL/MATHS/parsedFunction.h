// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunction.h,v 1.7 2003/03/03 14:17:53 anhi Exp $

#ifndef BALL_MATHS_PARSEDFUNCTION_H
#define BALL_MATHS_PARSEDFUNCTION_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#include <numeric>

using std::unary_function;

namespace BALL
{
    /**  \addtogroup  FunctionClasses
     *  @{
     */  
	extern HashMap<String, double*> *parsedFunctionConstants;
	extern HashMap<String, double (*)(double)> *parsedFunctionFunctions;
	
	/** ParsedFunction.
	 		This function type contains a double - valued function string like
			\f$var1=atan(.5); sin(cos(atan(asin(exp(X+var1)))))\f$.
			When the operator () is called, \f$X\f$ is replaced with
			the argument of the operator call and the function string
			is parsed and evaluated.
	*/
	template <typename arg> 
	class ParsedFunction 
		: public unary_function<arg, double> 
	{
		public:
			
		/** @name Constructors and Destructors
		 */
		//@{

		/// Default constructor
		ParsedFunction()
			throw();

		/// Detailed constructor.
		ParsedFunction(String expression)
			throw();
		
		/// Copy constructor
		ParsedFunction(const ParsedFunction& func)
			throw();
		
		/// Destructor
		~ParsedFunction()
			throw();
		
		//@}
		
		/** @name Accessors
		 */
		//@{
		/** Evaluate the function at point \f$p\f$.
		 */
		double operator () (arg p)
			throw(Exception::ParseError);
	
		//@}
		/** @name Parsing
		 */
		//@{
		/** This function initializes the function table and the constant table
		    of our parser.
			*/
		void initTable()
			throw();
		//@}

		/** The table of constants
		 */
		HashMap<String, double*> constants_;

		/** The table of functions
		 */
		HashMap<String, double (*)(double)> functions_;
		
		protected:
			String expression_;
	};
  /** @} */
}

#endif // BALL_MATHS_PARSEDFUNCTION_H
