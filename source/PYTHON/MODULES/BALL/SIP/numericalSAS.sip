// $Id: numericalSAS.sip,v 1.1 2000/07/18 21:27:50 oliver Exp $

%HeaderCode
	#include <BALL/STRUCTURE/numericalSAS.h>
	using namespace BALL;
%End

float calculateSASArea
     (const BaseFragment& /fragment/, float /probe_radius/ = 1.5, Size& /number_of_dots/ = Size(400)); 
float calculateSASVolume
     (const BaseFragment& /fragment/, float /probe_radius/ = 1.5, Size& /number_of_dots/ = Size(400)); 
PyAtomDict& calculateSASAtomAreas
     (const BaseFragment& /fragment/, float /probe_radius/ = 1.5, Size& /number_of_dots/ = 400);
%MemberCode
	const BaseFragment *a0;
	PyObject *a0obj;
	float a1 = 1.5;
	Size *a2 = NULL;
	PyObject *a2obj = NULL;

	if (sipParseArgs(sipArgs,"-I|fI",sipCanConvertTo_BaseFragment,&a0obj,&a1,sipCanConvertTo_Size,&a2obj))
	{
		PyAtomDict *res;

		int iserr = 0;

		sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);
		int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);
		if (a2 == NULL)
		{
			a2 = new Size;
			*a2 = 400;
		}

		if (iserr)
			return NULL;

		res = new PyAtomDict;
		calculateSASAtomAreas(* a0, *res, a1,* a2);
		PyObject *resobj = sipConvertFrom_PyAtomDict(res);

		delete res;
		delete a2;

		return resobj;
	}
%End

Surface& calculateSASPoints
     (const BaseFragment& /fragment/, float /probe_radius/ = 1.5, Size& /number_of_dots/ = 400);
%MemberCode
	const BaseFragment *a0;
	PyObject *a0obj;
	float a1 = 1.5;
	Size *a2 = NULL;
	PyObject *a2obj = NULL;

	if (sipParseArgs(sipArgs,"-I|fI",sipCanConvertTo_BaseFragment,&a0obj,&a1,sipCanConvertTo_Size,&a2obj))
	{
		Surface *res;

		int iserr = 0;

		sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);
		int istemp2 = sipConvertTo_Size(a2obj,&a2,1,&iserr);

		if (iserr)
			return NULL;

		res = new Surface;
		calculateSASPoints(* a0, *res, a1,* a2);

		if (istemp2)
			delete a2;

		return sipMapCppToSelf(res,sipClass_Surface);
	}
%End
