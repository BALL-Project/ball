class AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class AtomContainerProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (AtomContainer&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class BondProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Bond&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class ChainProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Chain&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Composite&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Fragment&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class ResidueProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Residue&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class ConstAtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class ConstCompositeProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
%End
	public:
	bool start();
	bool finish();
	Processor::Result operator () (Composite&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
