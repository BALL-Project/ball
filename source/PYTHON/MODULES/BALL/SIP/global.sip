// $Id: global.sip,v 1.1 2000/07/18 21:27:40 oliver Exp $

class Index
{
	public:
	operator const char* ();
%MemberCode
	Index* ptr;
  if ((ptr = (Index*)sipGetCppPtr(sipThis,sipClass_Index)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
%End
%CanConvertToClassCode
	// automatic conversion of Py integers to Position 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_Index));
%End
%ConvertToClassCode
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Index(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (Index*)sipConvertToCpp(sipPy, sipClass_Index, sipIsErr);

	return 0;
%End
};

class Position
{
	public:
	operator const char* ();
%MemberCode
	Position* ptr;
  if ((ptr = (Position*)sipGetCppPtr(sipThis,sipClass_Position)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
%End
%CanConvertToClassCode
	// automatic conversion of Py integers to Position 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_Position));
%End
%ConvertToClassCode
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Position(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (Position*)sipConvertToCpp(sipPy, sipClass_Position, sipIsErr);

	return 0;
%End
};


class Size
{
	public:
	operator const char* ();
%MemberCode
	Size* ptr;
  if ((ptr = (Size*)sipGetCppPtr(sipThis,sipClass_Size)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
%End
%CanConvertToClassCode
	// automatic conversion of Py integers to Size 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_Size));
%End
%ConvertToClassCode
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new Size(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (Size*)sipConvertToCpp(sipPy, sipClass_Size, sipIsErr);

	return 0;
%End
};
