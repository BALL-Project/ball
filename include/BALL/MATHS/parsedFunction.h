// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunction.h,v 1.9 2003/12/19 14:54:08 anne Exp $
//

#ifndef BALL_MATHS_PARSEDFUNCTION_H
#define BALL_MATHS_PARSEDFUNCTION_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <numeric>

using std::unary_function;

namespace BALL
{
    /**  \addtogroup  FunctionClasses
     *  @{
     */  
	extern StringHashMap<double*> *ParsedFunctionConstants;
	extern StringHashMap<double (*)(double)> *ParsedFunctionFunctions;
	
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
			ParsedFunction();
	
			/// Detailed constructor.
			ParsedFunction(const String& expression);
			
			/// Copy constructor
			ParsedFunction(const ParsedFunction& func);
			
			/// Destructor
			~ParsedFunction();
			
			//@}
			
			/** @name Accessors
			*/
			//@{
			/** Evaluate the function at point \f$p\f$.
			*/
			double operator () (arg p) throw(Exception::ParseError);
		
			//@}
			/** @name Parsing
			*/
			//@{
			/** This function initializes the function table and the constant table
				of our parser.
				*/
			void initTable();
			//@}
	
			/** The table of constants
			*/
			StringHashMap<double*> constants_;
	
			/** The table of functions
			*/
			StringHashMap<double (*)(double)> functions_;
		
		protected:
			String expression_;
	};
  /** @} */
}

#endif // BALL_MATHS_PARSEDFUNCTION_H
