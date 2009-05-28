%MappedType RepresentationList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Representation)
%End

%ConvertToTypeCode
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Representation)
%End
};

%MappedType GeometricObjectList
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(GeometricObject)
%End

%ConvertToTypeCode
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(GeometricObject)
%End
};

