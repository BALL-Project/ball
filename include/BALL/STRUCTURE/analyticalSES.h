// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: analyticalSES.h,v 1.14 2003/08/26 08:04:51 oliver Exp $
//

#ifndef BALL_STRUCTURE_ANALYTICALSES_H
#define BALL_STRUCTURE_ANALYTICALSES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	// forward declarations
	class Atom;
	class AtomContainer;
	template <typename Key, typename Value>
	class HashMap;
	
	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			?????		
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@return the SES area in $\AA^2$
    	
			\ingroup  Surface
	*/
	float calculateSESAtomAreas
		(const AtomContainer& fragment, HashMap<const Atom*,float>& atom_areas, float probe_radius = 1.5);
	
	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			?????
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@return the SES area in $\AA^2$

			\ingroup  Surface
	*/
	float calculateSESArea
		(const AtomContainer& fragment, float probe_radius = 1.5);
	
	/**	Calculate the solvent excluded volume analytically.
			This method uses the algorithm by Michael L. Connolly.
			?????
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@return the volume in $\AA^3$

			\ingroup  Surface
	*/
	float calculateSESVolume
		(const AtomContainer& fragment, float probe_radius = 1.5);
} // namespace BALL

#endif // BALL_STRUCTURE_ANALYTICALSES_H
