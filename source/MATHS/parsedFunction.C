// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunction.C,v 1.8 2003/12/19 14:53:55 anne Exp $

#include <BALL/MATHS/parsedFunction.h>

extern int ParsedFunctionparse();
extern double ParsedFunctionResult;
extern void ParsedFunction_initBuffer(const char*);
extern void ParsedFunction_delBuffer();

namespace BALL
{
	StringHashMap<double*> *ParsedFunctionConstants;
	StringHashMap<double(*)(double)> *ParsedFunctionFunctions;

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

	/* Strange... gcc-3.0.1 needs this constructor as a non-template version... */
	template <>
	ParsedFunction<float>::ParsedFunction(const String& expression)
		
		: constants_(),
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

	/** Strange... gcc-3.0.1 needs this destructor in a non-template version... **/
	template <>
	ParsedFunction<float>::~ParsedFunction()
	{
	}

	template <typename arg>
	double ParsedFunction<arg>::operator () (arg argument)
		throw(Exception::ParseError)
	{
		constants_["X"] = (double) &argument;
		ParsedFunctionConstants = &constants_;
		ParsedFunctionFunctions = &functions_;
		ParsedFunction_initBuffer(expression_.c_str());
		ParsedFunctionparse();
		ParsedFunction_delBuffer();
		
		return ParsedFunctionResult;
	}

	template <>
	double ParsedFunction<float>::operator () (float argument)
		throw(Exception::ParseError)
	{
		double arg = argument;
		constants_["X"] = &arg;
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
