%MappedType Index
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyLong_Check(sipPy);
	}
	if (PyLong_Check(sipPy))
	{
		*sipCppPtr = new Index;
		**sipCppPtr = PyLong_AsLong(sipPy);

		return 1;
	}

	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyLong_FromLong(0);
	}
	return PyLong_FromLong(*sipCpp);
%End
};

%MappedType Position
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
%End
%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyLong_Check(sipPy);
	}
	if (PyLong_Check(sipPy))
	{
		*sipCppPtr = new Position;
		**sipCppPtr = PyLong_AsLong(sipPy);

		return 1;
	}

	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyLong_FromLong(0);
	}
	return PyLong_FromLong(*sipCpp);
%End
};

%MappedType LongIndex
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyLong_Check(sipPy);
	}
	if (PyLong_Check(sipPy))
	{
		*sipCppPtr = new LongIndex;
		**sipCppPtr = PyLong_AsLong(sipPy);

		return 1;
	}

	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyLong_FromLong(0);
	}
	return PyLong_FromLong(*sipCpp);
%End
};

%MappedType Time
{
%TypeHeaderCode
	#include <BALL/COMMON/global.h>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL)
	{
		return PyLong_Check(sipPy);
	}
	if (PyLong_Check(sipPy))
	{
		*sipCppPtr = new Time;
		**sipCppPtr = PyLong_AsLong(sipPy);

		return 1;
	}

	return 0;
%End
%ConvertFromTypeCode
	if (sipCpp == NULL)
	{
		return PyLong_FromLong(0);
	}
	return PyLong_FromLong(*sipCpp);
%End
};

typedef int Size;
typedef double DoubleReal;
