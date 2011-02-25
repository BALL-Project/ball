// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class PeptideCapProcessor
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/peptideCapProcessor.h>
	#include <BALL/STRUCTURE/peptides.h>
	using namespace BALL;
	using namespace BALL::Peptides;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
public:
	PeptideCapProcessor();
	virtual Processor::Result operator() (Composite& /composite/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
