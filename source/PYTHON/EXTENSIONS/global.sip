// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.sip,v 1.5 2003/10/29 22:54:40 oliver Exp $
//

%MappedType Index
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
	#include "sipBALLIndex.h"
%End

%ConvertToTypeCode
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Index(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	return 0;
%End
%ConvertFromTypeCode
	return NULL;
%End
};

%MappedType Position
{
%ConvertToTypeCode
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Position(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	return 0;
%End
%ConvertFromTypeCode
	return NULL;
%End
};

typedef int Size;
typedef double DoubleReal;

// class Size
// {
// 	public:
// 	operator const char* ();
// %MethodCode
// 	Size* ptr;
//   if ((ptr = (Size*)sipGetCppPtr((sipThisType*)a0,sipClass_Size)) == NULL)
//     return NULL;
// 
//   return PyString_FromString(String(*ptr).c_str());
// %End
// %ConvertToClassCode
// 	if (PyInt_Check(sipPy))
// 	{
// 		*sipCppPtr = new Size(PyInt_AS_LONG(sipPy));
// 	
// 		return 1;
// 	}
// 	
// 	*sipCppPtr = (Size*)sipConvertToCpp(sipPy, sipClass_Size, sipIsErr);
// 
// 	return 0;
// %End
// };
// 
