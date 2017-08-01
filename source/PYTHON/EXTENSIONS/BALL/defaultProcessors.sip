class ClearChargeProcessor
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/defaultProcessors.h>
%End
	public:
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class ClearRadiusProcessor
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/defaultProcessors.h>
%End
	public:
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class AssignRadiusProcessor
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/defaultProcessors.h>
%End
	public:
	AssignRadiusProcessor();
	AssignRadiusProcessor(const String&);
	virtual bool start();
	virtual bool finish();
	Processor::Result operator() (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
	void setFilename(const String&);
	String& getFilename();
	Size getNumberOfAssignments();
	Size getNumberOfErrors();
};

class AssignChargeProcessor
	: AssignRadiusProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/defaultProcessors.h>
%End
	public:
	AssignChargeProcessor();
	AssignChargeProcessor(const String&);
	virtual bool start();
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
	float getTotalCharge();
};
