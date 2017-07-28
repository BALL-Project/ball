class PeptideCapProcessor
	: ChainProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/peptideCapProcessor.h>
	#include <BALL/STRUCTURE/peptides.h>
	using namespace BALL::Peptides;
%End
public:
	PeptideCapProcessor();
	virtual Processor::Result operator() (Chain&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
