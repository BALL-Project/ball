// $Id: makeSpectrumProcessor.C,v 1.5 2000/09/21 22:10:24 amoll Exp $

#include<BALL/NMR/makeSpectrumProcessor.h>

namespace BALL
{

	MakeSpectrumProcessor::MakeSpectrumProcessor()
	{
	}

	MakeSpectrumProcessor::~MakeSpectrumProcessor()
	{
	}

	bool MakeSpectrumProcessor::finish()
	{
		spectrum_.calculateShifts();
		return true;
	}

	Processor::Result MakeSpectrumProcessor::operator () (const Atom& atom)
	{
		// identify the system 
		const Composite& root = atom.getRoot();
		if (RTTI::isKindOf<System>(root))
		{
			System* system = RTTI::castTo<System>(root);
			spectrum_.setSystem(system);
			
			return Processor::BREAK;
		}

		return Processor::CONTINUE;
	}

}	// namespace BALL  
