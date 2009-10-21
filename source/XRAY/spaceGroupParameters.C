// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/XRAY/spaceGroupParameters.h>

using namespace std;

namespace BALL 
{
	SpaceGroupParameters::SpaceGroupParameters()
		: ParameterSection()
	{
	}
	
	SpaceGroupParameters::~SpaceGroupParameters()
	{
		clear();
	}
	
	void SpaceGroupParameters::clear()
	{
		symops_.clear();	
		ParameterSection::clear();
	}
	
	bool SpaceGroupParameters::extractSpaceGroup(Parameters& parameters, const String& space_group)
	{
		return true;
	}
	
	bool SpaceGroupParameters::extractSpaceGroup(Parameters& parameters, const Position iucr_no)
	{
		return true;
	}
} // namespace BALL
