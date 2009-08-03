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
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(GeometricObject)
%End
};

