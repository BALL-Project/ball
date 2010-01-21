// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%ModuleCode
	#include <BALL/STRUCTURE/numericalSAS.h>
	using namespace BALL;
	#if SIP_VERSION >= 0x040a00
		#define PARSE_ARG sipParseErr
	#else
		#define PARSE_ARG sipArgsParsed
	#endif
%End

float calculateSASArea
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400); 
float calculateSASVolume
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400); 
PyAtomDict& calculateSASAtomAreas
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
%MethodCode
	const AtomContainer *a0;
	float a1 = 1.5;
	Size a2 = 400;

	if (sipParseArgs(&PARSE_ARG,sipArgs,"J1|fi",BALL_SIP_TYPE(AtomContainer),&a0,&a1,&a2))
	{
		PyAtomDict *res;
 
		res = new PyAtomDict;
		calculateSASAtomAreas(*a0, *res, a1, a2);
		PyObject *resobj = BALL_CONVERT_FROM_MAPPED_TYPE(res,PyAtomDict,NULL);

		delete res;

		return resobj;
	}
%End

Surface& calculateSASPoints
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
%MethodCode
	const AtomContainer *a0;
	float a1 = 1.5;
	Size a2 = 400;

	if (sipParseArgs(&PARSE_ARG,sipArgs,"J1|fi",BALL_SIP_TYPE(AtomContainer), &a0, &a1, &a2))
	{
		Surface* res = new Surface;
		calculateSASPoints(*a0, *res, a1, a2);

		return BALL_CONVERT_FROM_INSTANCE(res,Surface, 0);
	}
%End
