// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ANALYTICALSES_H
#define BALL_STRUCTURE_ANALYTICALSES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_MOLECULESTORE_H
#	include <BALL/KERNEL/moleculeStore.h>     // v2.2 H3d.B
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
			@param	atom_areas
			@return the SES area in \f$\AA^2\f$
    	
			\ingroup  Surface
	*/
	BALL_EXPORT float calculateSESAtomAreas
		(const AtomContainer& fragment, HashMap<const Atom*,float>& atom_areas, float probe_radius = 1.5);

	/** v2.2 H3d.B (D-H3.8): StableId-keyed overload. The output map keys
	    the resulting SES atom areas by StableId. Single-store discipline:
	    the fragment's store is captured at the first qualifying atom;
	    foreign-store atoms are skipped. The v0 HashMap<const Atom*, float>
	    overload above stays for back-compat with QSAR/Solvation/PB
	    consumers which migrate piecewise in follow-up H3d.B commits. */
	BALL_EXPORT float calculateSESAtomAreas
		(const AtomContainer& fragment,
		 HashMap<MoleculeStore::StableId, float>& atom_areas_sid,
		 float probe_radius = 1.5f);
	
	/**	Calculate the solvent excluded surface area analytically.
			This method uses the algorithm by Michael L. Connolly.
			?????
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@return the SES area in \f$\AA^2\f$

			\ingroup  Surface
	*/
	BALL_EXPORT float calculateSESArea
		(const AtomContainer& fragment, float probe_radius = 1.5);
	
	/**	Calculate the solvent excluded volume analytically.
			This method uses the algorithm by Michael L. Connolly.
			?????
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@return the volume in \f$\AA^3\f$

			\ingroup  Surface
	*/
	BALL_EXPORT float calculateSESVolume
		(const AtomContainer& fragment, float probe_radius = 1.5);
} // namespace BALL

#endif // BALL_STRUCTURE_ANALYTICALSES_H
