// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunction.C,v 1.6 2002/12/21 11:10:30 oliver Exp $

#include <BALL/MATHS/parsedFunction.h>

extern int parsedFunctionparse();
extern double parsedFunctionResult;
extern void parsedFunction_initBuffer(const char*);
extern void parsedFunction_delBuffer();

namespace BALL
{
	HashMap<String, double*> *parsedFunctionConstants;
	HashMap<String, double(*)(double)> *parsedFunctionFunctions;
}
