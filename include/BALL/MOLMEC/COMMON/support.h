// $Id: support.h,v 1.2 2000/01/10 15:51:04 oliver Exp $

#ifndef BALL_MOLMEC_COMMON_SUPPORT_H
#define BALL_MOLMEC_COMMON_SUPPORT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_BOX3_H
#	include <BALL/MATHS/box3.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#include <vector>

namespace BALL 
{

	class Atom;
	class System;

	namespace MolmecSupport 
	{
		
		using std::pair;
			
		/**	@name	Support Functions for the Implementation of Force Fields.
		*/
		//@{

		/**	Create a pair vector for non-bonded interactions.
				Calculates a vector of atom pairs whose distance is smaller than distance.
				The {\tt type} determines if a brute force algorithm ({\tt type == 0})
				or a more sophisticated grid method ({\tt type == 1}) is used.
				@param	pair_vector the vector containing pairs of interacting atoms
				@param	atom_vector the atoms to be considered for pairs
				@param	box	the periodic boundary used (if {\tt periodic\_boundary\_enabled == true})
				@param	distance the maximum distance (cut-off distance) for two atoms
				@param	periodic_boundary_enabled flag indicating the use of periodic boundary conditions
				@param	type	the type of algorithm used to calculate the pair vector
				@return	the number of pairs generated ({\tt pair\_vector.size()})
		*/
		BALL::Size calculateNonBondedAtomPairs
			(ForceField::PairVector& pair_vector, 
			 const vector<Atom*>& atom_vector, const TBox3<Real>& box, 
			 float distance,	bool periodic_boundary_enabled, int type);

		/**	Merge the non-overlapping molecules of a system into another system.
				Fills {\tt system\_A} with copies of the solvent molecules stored in {\tt system\_B}.
				Solvent molecules are added to {\tt system\_A} if the molecules lie in the {\tt box}
				and if they do not overlap with the molecules in {\tt system\_A}. A molecule has
				an overlap with another molecule if any atom of the first molecule is within {\tt distance}
				of any atom of the other molecule.
				@param	system_A the system to be filled
				@param	system_B the system containing the solvent
				@param	box the periodic boundary of {\tt system\_A}
				@param	distance used to determine overlaps between two atoms
				@return	the number of molecules added to {\tt system\_A}
		*/
		BALL::Size addNonOverlappingMolecules
			(System&  system_A, const System& system_B,
			 const TBox3<Real>& box, float distance);

		//@}

	} // namespace MolmecSupport

} // namespace BALL

#endif // BALL_MOLMEC_COMMON_SUPPORT_H
