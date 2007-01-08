// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: analyticalSES.sip,v 1.3 2003/10/29 22:54:36 oliver Exp $
//

%TypeHeaderCode 
	#include <BALL/STRUCTURE/analyticalSES.h>
	using namespace BALL;
%End

float calculateSESAtomAreas
  (const AtomContainer& /atom_container/, AtomDict& /atom_areas/, float /probe_radius/ = 1.5);
float calculateSESArea
  (const AtomContainer& /atom_container/, float /probe_radius/ = 1.5);
float calculateSESVolume
  (const AtomContainer& /atom_container/, float /probe_radius/ = 1.5);
