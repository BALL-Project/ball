// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%ModuleCode
	#include <BALL/STRUCTURE/peptideCapProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Chain> ChainProcessor;
%End

class PeptideCapProcessor
	: ChainProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/peptideCapProcessor.h>
	#include <BALL/STRUCTURE/peptides.h>
	using namespace BALL;
	using namespace BALL::Peptides;
	typedef UnaryProcessor<Chain> ChainProcessor;
%End
public:
	PeptideCapProcessor();
	virtual Processor::Result operator() (Chain&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
