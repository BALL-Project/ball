class Index
{
	public:
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
