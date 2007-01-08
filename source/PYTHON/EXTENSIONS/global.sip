// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.sip,v 1.7 2005/12/23 17:02:51 amoll Exp $
//

%MappedType Index
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
	#include "sipBALLIndex.h"
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
	#include "sipBALLLongIndex.h"
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
	#include "sipBALLTime.h"
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
