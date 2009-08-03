// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%MappedType Index
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
	using namespace BALL;
%End

%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyInt_Check(sipPy);
	}
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Index;
		**sipCppPtr = PyInt_AsLong(sipPy);
	
		return 1;
	}
	
	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyInt_FromLong(0);
	}
	return PyInt_FromLong(*sipCpp);
%End
};

%MappedType Position
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
	using namespace BALL;
%End
%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyInt_Check(sipPy);
	}
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Position;
		**sipCppPtr = PyInt_AsLong(sipPy);
	
		return 1;
	}
	
	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyInt_FromLong(0);
	}
	return PyInt_FromLong(*sipCpp);
%End
};

%MappedType LongIndex
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
	using namespace BALL;
%End

%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyInt_Check(sipPy);
	}
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new LongIndex;
		**sipCppPtr = PyInt_AsLong(sipPy);
	
		return 1;
	}
	
	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyInt_FromLong(0);
	}
	return PyInt_FromLong(*sipCpp);
%End
};

%MappedType Time
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
	using namespace BALL;
%End

%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyInt_Check(sipPy);
	}
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Time;
		**sipCppPtr = PyInt_AsLong(sipPy);
	
		return 1;
	}
	
	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyInt_FromLong(0);
	}
	return PyInt_FromLong(*sipCpp);
%End
};


typedef int Size;
typedef double DoubleReal;
