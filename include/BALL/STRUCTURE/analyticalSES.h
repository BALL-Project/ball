// $Id: analyticalSES.h,v 1.5 2000/06/27 14:35:09 oliver Exp $

#ifndef BALL_STRUCTURE_ANALYTICALSES_H
#define BALL_STRUCTURE_ANALYTICALSES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	// forward declarations
	class BaseFragment;
	class Atom;
	template <typename Key, typename Value>
	class HashMap;
	
	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			BAUSTELLE
		
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
			@return the SES area in $\AA^2$
	*/
	float calculateSESAtomAreas
		(const BaseFragment& fragment, HashMap<Atom*,float>& atom_areas, float probe_radius = 1.5);
	
	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			BAUSTELLE
		
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
			@return the SES area in $\AA^2$
	*/
	float calculateSESArea
		(const BaseFragment& fragment, float probe_radius = 1.5);
	
	/**	Calculate the solvent excluded volume analytically.
			This method uses the algorithm by Michael L. Connolly.
			BAUSTELLE
		
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
			@return the volume in $\AA^3$
	*/
	float calculateSESVolume
		(const BaseFragment& fragment, float probe_radius = 1.5);
	
} // namespace BALL

#endif // BALL_STRUCTURE_ANALYTICALSES_H
