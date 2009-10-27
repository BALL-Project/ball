%MappedType RepresentationList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Representation)
%End

%ConvertToTypeCode
	//The following line is to work around an oddity (bug) in the preprocessor.
	sipIsErr == 0 || sipPy == 0;
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Representation)
%End
};

%MappedType GeometricObjectList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(GeometricObject)
%End

%ConvertToTypeCode
	//The following line is to work around an oddity (bug) in the preprocessor.
	sipIsErr == 0 || sipPy == 0;
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(GeometricObject)
%End
};

