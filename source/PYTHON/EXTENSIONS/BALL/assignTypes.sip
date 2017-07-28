class AssignBaseProcessor
	: public AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
%End

	public:
	AssignBaseProcessor();

	void setMaximumUnassignedAtoms(Size);
	Size getMaximumUnassignedAtoms() const;
	Size getNumberOfUnassignedAtoms() const;

	virtual Processor::Result operator () (Atom& atom);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	virtual ~AssignBaseProcessor();
};

class AssignTypeProcessor
	: public AssignBaseProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
%End

	public:
	AssignTypeProcessor(const AtomTypes&);
	virtual Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class AssignTypeNameProcessor
	: public AssignBaseProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
%End

	public:
	AssignTypeNameProcessor(const String&, bool overwrite);
	virtual Processor::Result operator () (Atom& atom);
%MethodCode
		sipRes = sipCpp->operator () (*a0);
%End
};
