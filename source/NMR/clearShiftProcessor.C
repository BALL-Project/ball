// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/NMR/clearShiftProcessor.h>

namespace BALL
{

	ClearShiftProcessor::ClearShiftProcessor()	
		:	ShiftModule()
	{
		valid_ = true;
	}

	ClearShiftProcessor::~ClearShiftProcessor()
	{
	}

	ClearShiftProcessor::ClearShiftProcessor(const ClearShiftProcessor& processor)
		:	ShiftModule(processor)
	{
	}

	Processor::Result ClearShiftProcessor::operator () (Composite& composite)
	{
		// Clear the chemical shift property (ShiftModule::PROPERTY__SHIFT)
		// of the atom (if defined).
		// v2.1 P3.3 (D41.1): centralised detail::compositeAsAtom_.
		Atom* atom = detail::compositeAsAtom_(&composite);
		if (atom != 0)
		{
			if (atom->hasProperty(ShiftModule::PROPERTY__SHIFT))
			{
				atom->clearProperty(ShiftModule::PROPERTY__SHIFT);
			}
		}

		return Processor::CONTINUE;
	}

} //namespace BALL
