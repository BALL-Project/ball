// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: numericalSAS.sip,v 1.7 2003/10/29 22:54:42 oliver Exp $
//

%ModuleHeaderCode
	#include <BALL/STRUCTURE/numericalSAS.h>
	using namespace BALL;
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

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J1|fi",sipClass_AtomContainer,&a0,&a1,&a2))
	{
		PyAtomDict *res;
 
		res = new PyAtomDict;
		calculateSASAtomAreas(*a0, *res, a1, a2);
		PyObject *resobj = sipConvertFrom_PyAtomDict(res);

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

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J1|fi",sipClass_AtomContainer, &a0, &a1, &a2))
	{
		Surface* res = new Surface;
		calculateSASPoints(*a0, *res, a1, a2);

		return sipMapCppToSelf(res,sipClass_Surface);
	}
%End
