// $Id: analyticalSES.h,v 1.2 2000/05/30 10:43:26 oliver Exp $

#ifndef BALL_STRUCTURE_ANALYTICALSES_H
#define BALL_STRUCTURE_ANALYTICALSES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	
	class BaseFragment;
	
	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			BAUSTELLE
		
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
	*/
	float calculateSESArea
		(const BaseFragment& fragment, float probe_radius = 1.5);
	
} // namespace BALL

#endif // BALL_STRUCTURE_ANALYTICALSES_H
