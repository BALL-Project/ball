// $Id: clearShiftProcessor.C,v 1.2 2000/09/16 07:44:12 oliver Exp $

#include<BALL/NMR/clearShiftProcessor.h>

using namespace std;

namespace BALL
{

	// default ctor
	ClearShiftProcessor::ClearShiftProcessor()
	{
	}

		
	// destructor
	ClearShiftProcessor::~ClearShiftProcessor()
	{
	}


	Processor::Result ClearShiftProcessor::operator () (Composite& composite)
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

