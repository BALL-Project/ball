// $Id: support.h,v 1.6 2001/01/26 16:07:04 anker Exp $

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

#ifndef BALL_MOLMEC_COMMON_ATOMVECTOR_H
#	include <BALL/MOLMEC/COMMON/atomVector.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#	include <BALL/DATATYPE/hashGrid.h>
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

		/**	@name	Enums
		*/
		//@{
		/**	The type of algorithm used to calculate the pair list.
		*/
		enum PairListAlgorithmType
		{
			/**	use a hash grid 
			*/
			HASH_GRID,
			/**	brute force all agains all
			*/
			BRUTE_FORCE
		};
		//@}
			
		/**	Create a pair vector for non-bonded interactions.
				Calculates a vector of atom pairs whose distance is smaller than
				distance.  The {\tt type} determines if a brute force algorithm
				({\tt type == BRUTE_FORCE}) or a more sophisticated grid method
				({\tt type == HASH_GRID}) is used.
				@param	pair_vector the vector containing pairs of interacting atoms
				@param	atom_vector the atoms to be considered for pairs
				@param	box	the periodic boundary used (if {\tt
				periodic_boundary_enabled == true})
				@param	distance the maximum distance (cut-off distance) for two atoms
				@param	periodic_boundary_enabled flag indicating the use of
				periodic boundary conditions
				@param	type	the type of algorithm used to calculate the pair vector
				@return	the number of pairs generated ({\tt pair_vector.size()})
		*/
		BALL::Size calculateNonBondedAtomPairs
			(ForceField::PairVector& pair_vector, 
			 const AtomVector& atom_vector, const Box3& box, 
			 double distance,	bool periodic_boundary_enabled, 
			 PairListAlgorithmType type);

		/**	Merge the non-overlapping molecules of a system into another
				system.
				Fills {\tt system} with copies of the solvent molecules stored in
				{\tt solvent}. Solvent molecules are added to {\tt system} if
				the molecules lie in the {\tt box} and if they do not overlap with
				the molecules in {\tt solute_grid}. A molecule has an overlap with
				another molecule if any atom of the first molecule is within {\tt
				distance} of any atom of the other molecule.
				@param	system the system to be filled
				@param	solute_grid a hash grid containing the solute molecules
				@param	solvent the system containing the solvent
				@param	box the periodic boundary of {\tt system_A}
				@param	distance used to determine overlaps between two atoms
				@return	the number of molecules added to {\tt system_A}
		*/
		BALL::Size addNonOverlappingMolecules
			(System& system, const HashGrid3<Atom*>& solute_grid,
			 const System& solvent, const Box3& box, double distance);

		/** Adapt periodic water (or general solvent) boxes to our periodic
				boundary definition. 
				Periodic water boxes created by different programs might contain
				molecules which have their center of gravity outside of the box
				because of a different defintion of the periodic boundary. This
				function adapts such boxes to our definition by simply translating
				such molecules to the opposite boundary wall such that their center
				of gravity lies in the box afterwards.
				@param system the system containing the water box which is to be
				adapted
				@param box the box definition for the periodic boundary
		*/
		void adaptWaterBox(System& system, const Box3& box);

		//@}

	} // namespace MolmecSupport

} // namespace BALL

#endif // BALL_MOLMEC_COMMON_SUPPORT_H
