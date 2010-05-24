// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunction.C,v 1.8 2003/12/19 14:53:55 anne Exp $

#include <BALL/MATHS/parsedFunction.h>

namespace BALL
{
	StringHashMap<double*> *ParsedFunctionConstants;
	StringHashMap<double(*)(double)> *ParsedFunctionFunctions;

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

	template <>
	double ParsedFunction<double>::operator () (double argument)
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

}
