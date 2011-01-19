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
		// clear the checmical shift property (ShiftModule::PROPERTY__SHIFT)
		// of the atom(if defined)
		Atom* atom = dynamic_cast<Atom*>(&composite);
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
