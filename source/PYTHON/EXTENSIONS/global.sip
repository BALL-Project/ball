// $Id: global.sip,v 1.1 2001/09/03 16:53:09 oliver Exp $

%MappedType Index
{
%HeaderCode
	#include <BALL/COMMON/global.h>
	#include "sipBALLIndex.h"
%End

%CanConvertToTypeCode
	// automatic conversion of Py integers to Position 
	return PyInt_Check(sipPy);
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
%CanConvertToTypeCode
	// automatic conversion of Py integers to Position 
	return PyInt_Check(sipPy);
%End
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
// %MemberCode
// 	Size* ptr;
//   if ((ptr = (Size*)sipGetCppPtr((sipThisType*)a0,sipClass_Size)) == NULL)
//     return NULL;
// 
//   return PyString_FromString(String(*ptr).c_str());
// %End
// %CanConvertToClassCode
// 	// automatic conversion of Py integers to Size 
// 	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_Size));
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
