// $Id: clearShiftProcessor.C,v 1.3 2000/09/18 16:26:27 oliver Exp $

#include<BALL/NMR/clearShiftProcessor.h>

using namespace std;

namespace BALL
{

	// default ctor
	ClearShiftProcessor::ClearShiftProcessor()
		throw()
	{
	}

		
	// destructor
	ClearShiftProcessor::~ClearShiftProcessor()
		throw()
	{
	}


	Processor::Result ClearShiftProcessor::operator () (Composite& composite)
		thow()
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

