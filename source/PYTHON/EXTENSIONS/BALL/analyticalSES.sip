%TypeHeaderCode
	#include <BALL/STRUCTURE/analyticalSES.h>
%End

float calculateSESAtomAreas
	(const AtomContainer&, AtomDict&, float probe_radius = 1.5);
float calculateSESArea
	(const AtomContainer&, float probe_radius = 1.5);
float calculateSESVolume
	(const AtomContainer&, float probe_radius = 1.5);
