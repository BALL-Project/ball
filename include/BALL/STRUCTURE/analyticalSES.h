// $Id: analyticalSES.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_STRUCTURE_ANALYTICALSES_H
#define BALL_STRUCTURE_ANALYTICALSES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

namespace BALL 
{

	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			BAUSTELLE
		
			@param	composite the composite containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
	*/
	float calculateSESArea
		(const Composite& composite, float probe_radius = 1.5);
	
} // namespace BALL

#endif // BALL_STRUCTURE_ANALYTICALSES_H
