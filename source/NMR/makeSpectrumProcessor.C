// $Id: makeSpectrumProcessor.C,v 1.2 2000/08/28 16:08:53 oliver Exp $

#include<BALL/NMR/makeSpectrumProcessor.h>

using namespace std;

namespace BALL
{

	MakeSpectrumProcessor::MakeSpectrumProcessor ()
	{
		spectrum_ = new NMRSpectrum;
		spectrum_->insert_shift_module ("JB");
		spectrum_->insert_shift_module ("LEF");
		spectrum_->insert_shift_module ("ANISO");
	}

	MakeSpectrumProcessor::~MakeSpectrumProcessor ()
	{
	}

	bool MakeSpectrumProcessor::start ()
	{
		// hier passiert nichts
		return 1;
	}

	bool MakeSpectrumProcessor::finish ()
	{
		// starte die Berechnungen

		spectrum_->calculate_shifts ();

		return 1;
	}

	Processor::Result MakeSpectrumProcessor::operator () (Object & object)
	{
		// identifieziere das System und gebe es an spectrum_
		if (RTTI::isKindOf < System > (object))
		{
			system_ = RTTI::castTo < System > (object);
			spectrum_->set_system (system_);
		}

		return Processor::CONTINUE;
	}

}	// namespace BALL  
