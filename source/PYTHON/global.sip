class Position
{
	public:
%CanConvertToClassCode
	// automatic conversion of Py integers to Position 
	return (sipIsSubClassInstance(sipPy, sipClass_Int);
%End
%ConvertToClassCode
	if (PyInt_Check(sipPy))
	{
		sipCppPtr = new Position(PyInt_
		*sipCppPtr = new Position(Py
	
		return 1;
	}
	
	*sipCppPtr = (*String)sipConvertToCpp(sipPy, sipClass_String, sipIsErr);

	return 0;
%End
};
