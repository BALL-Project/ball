// $Id: createSpectrumProcessor.C,v 1.4 2000/09/07 21:03:40 oliver Exp $

#include<BALL/NMR/createSpectrumProcessor.h>

using namespace std;

namespace BALL
{

	CreateSpectrumProcessor::CreateSpectrumProcessor()
	{
		raster_ = 0.01e-6;
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
