// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunction.C,v 1.4 2002/12/18 16:00:34 sturm Exp $

#include <BALL/MATHS/parsedFunction.h>

extern int parsedFunctionparse();
extern double parsedFunctionResult;
extern void parsedFunction_initBuffer(const char*);
extern void parsedFunction_delBuffer();

namespace BALL
{
	HashMap<String, double*> *parsedFunctionConstants;
	HashMap<String, double(*)(double)> *parsedFunctionFunctions;

	template <typename arg>
	ParsedFunction<arg>::ParsedFunction()
		throw()
		:	constants_(),
			functions_(),
			expression_("")
	{
		initTable();
	}

	template <typename arg>
	ParsedFunction<arg>::ParsedFunction(String expression)
		throw()
		:	constants_(),
			functions_(),
			expression_(expression)
	{
		initTable();
	}

	/** Strange... gcc-3.0.1 needs this constructor as a non-template version... **/
	template <>
	ParsedFunction<float>::ParsedFunction(String expression)
		throw()
		: constants_(),
			functions_(),
			expression_(expression)
			
	{
		initTable();
	}

	template <typename arg>
	ParsedFunction<arg>::ParsedFunction(const ParsedFunction& func)
		throw()
	{
		constants_ = func.constants_;
		functions_ = func.functions_;
		expression_ = func.expression_;
		initTable();
	}

	template <typename arg>
	ParsedFunction<arg>::~ParsedFunction()
		throw()
	{
	}

	/** Strange... gcc-3.0.1 needs this destructor in a non-template version... **/
	template <>
	ParsedFunction<float>::~ParsedFunction()
		throw()
	{
	}

	template <typename arg>
	double ParsedFunction<arg>::operator () (arg argument)
		throw(Exception::ParseError)
	{
		constants_["X"] = (double) &argument;
		parsedFunctionConstants = &constants_;
		parsedFunctionFunctions = &functions_;
		parsedFunction_initBuffer(expression_.c_str());
		parsedFunctionparse();
		parsedFunction_delBuffer();
		
		return parsedFunctionResult;
	}

	template <>
	double ParsedFunction<float>::operator () (float argument)
		throw(Exception::ParseError)
	{
		double arg = argument;
		constants_["X"] = &arg;
		parsedFunctionConstants = &constants_;
		parsedFunctionFunctions = &functions_;
		parsedFunction_initBuffer(expression_.c_str());
		parsedFunctionparse();
		parsedFunction_delBuffer();
		return parsedFunctionResult;
	}


	template <typename arg>
	void ParsedFunction<arg>::initTable()
		throw()
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
