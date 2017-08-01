class SecondaryStructureProcessor
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
%End
	public:
	SecondaryStructureProcessor();
	Processor::Result operator() (Composite&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
