// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: analyticalSES.sip,v 1.2 2003/08/26 09:18:12 oliver Exp $
//

%HeaderCode 
	#include <BALL/STRUCTURE/analyticalSES.h>
	using namespace BALL;
%End

float calculateSESAtomAreas
  (const AtomContainer& /atom_container/, AtomDict& /atom_areas/, float /probe_radius/ = 1.5);
float calculateSESArea
  (const AtomContainer& /atom_container/, float /probe_radius/ = 1.5);
float calculateSESVolume
  (const AtomContainer& /atom_container/, float /probe_radius/ = 1.5);
