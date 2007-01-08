// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomicContactEnergy.h,v 1.15 2005/12/23 17:01:43 amoll Exp $
//

#ifndef BALL_ENERGY_ATOMICCONTACTENERGY_H
#define BALL_ENERGY_ATOMICCONTACTENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class AtomContainer;

	/** Calculate the atomic contact energy.
			Calculates the atomic contact energy (ACE) after
			Zhang et al. (J. Mol. Biol., 267, 707-726 (1997)).
			The energy is returned in units of kJ/mol.
			Unparametrized Atoms are simply ignored, this may lead to unreasonable values.  
			However, a warning is being printed in this case.
			\par
			<b>Files:</b>  \par
			The two files given as default parameters contain the original parameters from
			the JMB publication (ACE_parameters.dat) as a 18x18 matrix. ACE_types.dat contains
			the mapping of residue/atom names to theses types for standard amino acid residues.

    	\ingroup EnergyMiscellaneous 
	*/ 
  BALL_EXPORT double calculateACE
    (const AtomContainer& atom_container,
     const string& type_filename = "energy/ACE_types.dat", 
		 const string& parameter_filename = "energy/ACE_parameters.dat");

} // namespace BALL

#endif
