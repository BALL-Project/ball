// $Id: numericalSAS.sip,v 1.1 2000/06/30 05:58:52 oliver Exp $

%HeaderCode
	#include <BALL/STRUCTURE/numericalSAS.h>
	using namespace BALL;
%End

float calculateSASArea
     (const BaseFragment& /fragment/, float /probe_radius/ = 1.5,
      Size /number_of_dots/ = 400); 
float calculateSASVolume
     (const BaseFragment& /fragment/, float /probe_radius/ = 1.5,
      Size /number_of_dots/ = 400); 
float calculateSASAtomAreas
     (const BaseFragment& /fragment/, AtomDict& /atom_areas/,
      float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
float calculateSASPoints
     (const BaseFragment& /fragment/, Surface& /surface_points/,
      float /probe_radius/ = 1.5,  Size /number_of_dots/ = 400);
