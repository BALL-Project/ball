// $Id: makeSpectrumProcessor.C,v 1.6 2000/09/22 17:18:13 oliver Exp $

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

	Processor::Result MakeSpectrumProcessor::operator () (Atom& atom)
	{
		// identify the system 
		Composite& root = atom.getRoot();
		if (RTTI::isKindOf<System>(root))
		{
			System* system = RTTI::castTo<System>(root);
			spectrum_.setSystem(system);
			
			return Processor::BREAK;
		}

		return Processor::CONTINUE;
	}

}	// namespace BALL  
