// $Id: makeSpectrumProcessor.C,v 1.3 2000/09/07 19:38:30 oliver Exp $

#include<BALL/NMR/makeSpectrumProcessor.h>

using namespace std;

namespace BALL
{

	MakeSpectrumProcessor::MakeSpectrumProcessor()
	{
		spectrum_.insertShiftModule("JB");
		spectrum_.insertShiftModule("LEF");
		spectrum_.insertShiftModule("ANISO");
	}

	MakeSpectrumProcessor::~MakeSpectrumProcessor()
	{
	}

	bool MakeSpectrumProcessor::finish()
	{
		// starte die Berechnungen

		spectrum_.calculateShifts();

		return true;
	}

	Processor::Result MakeSpectrumProcessor::operator () (Atom& atom)
	{
		// identify the system 
		Composite& root = atom.getRoot();
		if (RTTI::isKindOf<System>(root))
		{
			system_ = RTTI::castTo<System>(root);
			spectrum_.setSystem(system_);
			
			return Processor::BREAK;
		}

		return Processor::CONTINUE;
	}

}	// namespace BALL  
