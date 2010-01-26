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

extern int ParsedFunctionparse();
extern double ParsedFunctionResult;
extern void ParsedFunction_initBuffer(const char*);
extern void ParsedFunction_delBuffer();

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

	template <>	ParsedFunction<float>::~ParsedFunction();
	template <> double ParsedFunction<float>::operator() (float);
	template <> double ParsedFunction<double>::operator() (double);

	template class BALL_EXPORT ParsedFunction<float>;
	template class BALL_EXPORT ParsedFunction<double>;

	template <typename arg>
	ParsedFunction<arg>::ParsedFunction()
		:	constants_(),
			functions_(),
			expression_("")
	{
		initTable();
	}

	template <typename arg>
	ParsedFunction<arg>::ParsedFunction(const String& expression)
		:	constants_(),
			functions_(),
			expression_(expression)
	{
		initTable();
	}

	template <typename arg>
	ParsedFunction<arg>::ParsedFunction(const ParsedFunction& func)	
	{
		constants_ = func.constants_;
		functions_ = func.functions_;
		expression_ = func.expression_;
		initTable();
	}

	template <typename arg>
	ParsedFunction<arg>::~ParsedFunction()
	{
	}

	template <typename arg>
	double ParsedFunction<arg>::operator () (arg argument)
		throw(Exception::ParseError)
	{
		double d_arg = (double)argument;
		constants_["X"] = &d_arg;
		ParsedFunctionConstants = &constants_;
		ParsedFunctionFunctions = &functions_;
		ParsedFunction_initBuffer(expression_.c_str());
		ParsedFunctionparse();
		ParsedFunction_delBuffer();
		
		return ParsedFunctionResult;
	}

	template <typename arg>
	void ParsedFunction<arg>::initTable()
	{
		// initialize the functions table
		functions_["sin"] = (double(*)(double))&sin;
		functions_["cos"] = (double(*)(double))&cos;
		functions_["asin"] = (double(*)(double))&asin;
		functions_["acos"] = (double(*)(double))&acos;
		functions_["tan"] = (double(*)(double))&tan;
		functions_["atan"] = (double(*)(double))&atan;
		functions_["ln"] = (double(*)(double))&log;
		functions_["exp"] = (double(*)(double))&exp;
		functions_[""] = 0;
	}

}

#endif // BALL_MATHS_PARSEDFUNCTION_H
