// $Id: clearShiftProcessor.C,v 1.5 2000/09/20 13:51:17 amoll Exp $

#include<BALL/NMR/clearShiftProcessor.h>

namespace BALL
{

	ClearShiftProcessor::ClearShiftProcessor()
		throw()
	{
	}

	ClearShiftProcessor::~ClearShiftProcessor()
		throw()
	{
	}

	Processor::Result ClearShiftProcessor::operator () (Composite& composite)
		throw()
	{
		// clear the "chemical_shift" property of the atom
		// (if defined)
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (atom != 0)
		{
			if (atom->hasProperty("chemical_shift"))
			{
				atom->clearProperty("chemical_shift");
			}
		}

		return Processor::CONTINUE;
	}

} //namespace BALL
