// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clearShiftProcessor.C,v 1.7 2002/02/27 12:23:53 sturm Exp $

#include<BALL/NMR/clearShiftProcessor.h>

namespace BALL
{

	ClearShiftProcessor::ClearShiftProcessor()
		throw()
		:	ShiftModule()
	{
		valid_ = true;
	}

	ClearShiftProcessor::~ClearShiftProcessor()
		throw()
	{
	}

	ClearShiftProcessor::ClearShiftProcessor(const ClearShiftProcessor& processor)
		throw()
		:	ShiftModule(processor)
	{
	}

	Processor::Result ClearShiftProcessor::operator () (Composite& composite)
		throw()
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
