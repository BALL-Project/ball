// $Id: numericalSAS.sip,v 1.3 2001/12/23 22:03:59 oliver Exp $

%HeaderCode
	#include <BALL/STRUCTURE/numericalSAS.h>
	using namespace BALL;
%End

float calculateSASArea
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400); 
float calculateSASVolume
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400); 
PyAtomDict& calculateSASAtomAreas
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
%MemberCode
	const AtomContainer *a0;
	PyObject *a0obj;
	float a1 = 1.5;
	Size a2 = 0;
	// if (sipParseArgs(&sipArgsParsed,sipArgs,"mJ0",sipThisObj,sipClass_QWindowsStyle,&ptr,sipClass_QScrollBar,&a0)
	if (sipParseArgs(&sipArgsParsed,sipArgs,"J0fi",sipClass_AtomContainer,&a0obj,&a1,&a2))
	{
		PyAtomDict *res;

		int iserr = 0;

		a0 = (AtomContainer*)sipConvertToCpp(a0obj,sipClass_AtomContainer,&iserr);	
		if (iserr)
			return NULL;

		res = new PyAtomDict;
		calculateSASAtomAreas(* a0, *res, a1, a2);
		PyObject *resobj = sipConvertFrom_PyAtomDict(res);

		delete res;

		return resobj;
	}
%End

Surface& calculateSASPoints
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
%MemberCode
	const AtomContainer *a0;
	PyObject *a0obj;
	float a1 = 1.5;
	Size a2 = 0;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J0fi",sipClass_AtomContainer,&a0obj,&a1,&a2))
	{
		Surface *res;
		int iserr = 0;

		a0 = (AtomContainer*)sipConvertToCpp(a0obj,sipClass_AtomContainer,&iserr);	
		if (iserr)
			return NULL;

		res = new Surface;
		calculateSASPoints(*a0, *res, a1,a2);

		return sipMapCppToSelf(res,sipClass_Surface);
	}
%End
