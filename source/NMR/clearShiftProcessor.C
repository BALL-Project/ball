// $Id: clearShiftProcessor.C,v 1.1 2000/09/15 08:58:44 oliver Exp $

#include<BALL/NMR/ClearShiftProcessor.h>

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
		Atom* atom = dynamic_cast<Atom*>(composite);
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

