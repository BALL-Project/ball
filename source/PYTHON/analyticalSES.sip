// $Id: analyticalSES.sip,v 1.1 2000/06/30 05:58:46 oliver Exp $

%HeaderCode 
	#include <BALL/STRUCTURE/analyticalSES.h>
	using namespace BALL;
%End

float calculateSESAtomAreas
  (const BaseFragment& /fragment/, AtomDict& /atom_areas/, float /probe_radius/ = 1.5);
float calculateSESArea
  (const BaseFragment& /fragment/, float /probe_radius/ = 1.5);
float calculateSESVolume
  (const BaseFragment& /fragment/, float /probe_radius/ = 1.5);
