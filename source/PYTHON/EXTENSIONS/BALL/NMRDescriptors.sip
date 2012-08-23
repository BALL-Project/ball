%ModuleHeaderCode
	#include <BALL/QSAR/NMRDescriptors.h>
	using namespace BALL;
%End

PyResidueDict& calculateResidueChiAngles(const Protein&);
%MethodCode
	const Protein *a0;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J1|fi",sipClass_Protein,&a0))
	{
		PyResidueDict *res;
 
		res = new PyResidueDict;
		calculateResidueChiAngles(*a0, *res);
		PyObject *resobj = sipConvertFromMappedType(res,sipMappedType_PyResidueDict,NULL);

		delete res;

		return resobj;
	}
%End

PyResidueDict& calculateResidueChiAngles(const Chain&);
%MethodCode
	const Chain *a0;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J1|fi",sipClass_Chain,&a0))
	{
		PyResidueDict *res;
 
		res = new PyResidueDict;
		calculateResidueChiAngles(*a0, *res);
		PyObject *resobj = sipConvertFromMappedType(res,sipMappedType_PyResidueDict,NULL);

		delete res;

		return resobj;
	}
%End


float calculateResidueChiAngles(const Residue&) const;


PyResidueDict& calculateResidueChi2Angles(const Protein&);
%MethodCode
	const Protein *a0;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J1|fi",sipClass_Protein,&a0))
	{
		PyResidueDict *res;
 
		res = new PyResidueDict;
		calculateResidueChiAngles(*a0, *res);
		PyObject *resobj = sipConvertFromMappedType(res,sipMappedType_PyResidueDict,NULL);

		delete res;

		return resobj;
	}
%End

PyResidueDict& calculateResidueChi2Angles(const Chain&);
%MethodCode
	const Chain *a0;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"J1|fi",sipClass_Chain,&a0))
	{
		PyResidueDict *res;
 
		res = new PyResidueDict;
		calculateResidueChiAngles(*a0, *res);
		PyObject *resobj = sipConvertFromMappedType(res,sipMappedType_PyResidueDict,NULL);

		delete res;

		return resobj;
	}
%End

float calculateResidueChi2Angles(const Residue&) const;

