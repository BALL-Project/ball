// $Id: createSpectrumProcessor.C,v 1.5 2000/09/20 14:04:33 amoll Exp $

#include<BALL/NMR/createSpectrumProcessor.h>
#include<BALL/KERNEL/PTE.h>
#include<BALL/KERNEL/atom.h>

namespace BALL
{

	CreateSpectrumProcessor::CreateSpectrumProcessor()
	{
	}

	bool CreateSpectrumProcessor::start()
	{
		// clear the contents of the old peak list
		peaklist_.clear();
		return true;
	}

	CreateSpectrumProcessor::~CreateSpectrumProcessor ()
	{
	}

	Processor::Result CreateSpectrumProcessor::operator () (Atom& atom)
	{
		// Collect all H atoms
		if (atom.getElement() == PTE[Element::H])
		{
			Peak1D peak;
			peak.setAtom(&atom);
			peaklist_.push_back(peak);
		}

		return Processor::CONTINUE;
	}

	const list<Peak1D>& CreateSpectrumProcessor::getPeakList() const
	{
		return peaklist_;
	}

}	// namespace Ball
