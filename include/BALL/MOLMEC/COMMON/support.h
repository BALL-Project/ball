// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: support.h,v 1.28 2005/12/23 17:01:52 amoll Exp $
//


#ifndef BALL_MOLMEC_COMMON_SUPPORT_H
#define BALL_MOLMEC_COMMON_SUPPORT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_SIMPLEBOX3_H
#	include <BALL/MATHS/simpleBox3.h>
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

#ifndef BALL_KERNEL_BOND_H
#     include <BALL/KERNEL/bond.h>
#endif

#include <vector>

namespace BALL 
{
	class Atom;
	class System;

	/** MolmecSupport
      \ingroup  MolmecCommon
	*/
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

			/**	brute force all against all
			*/
			BRUTE_FORCE
		};
		//@}
			
		/**	Create a pair vector for non-bonded interactions.
				Calculates a vector of atom pairs whose distance is smaller than
				<tt>distance</tt>.  The <tt>type</tt> determines if a brute force algorithm
				(<tt>type == BRUTE_FORCE</tt>) or a more sophisticated grid method
				(<tt>type == HASH_GRID</tt>) is used.
				@param	pair_vector the vector containing pairs of interacting atoms
				@param	atom_vector the atoms to be considered for pairs
				@param	box	the periodic boundary used (if <tt>	periodic_boundary_enabled == true</tt>)
				@param	distance the maximum distance (cut-off distance) for two atoms
				@param	periodic_boundary_enabled flag indicating the use of periodic boundary conditions
				@param	type	the type of algorithm used to calculate the pair vector
				@return	the number of pairs generated (<tt>pair_vector.size()</tt>)
		*/
		BALL_EXPORT BALL::Size calculateNonBondedAtomPairs
			(ForceField::PairVector& pair_vector, 
			 const AtomVector& atom_vector, const SimpleBox3& box, 
			 double distance,	bool periodic_boundary_enabled, 
			 PairListAlgorithmType type)
			throw(Exception::OutOfMemory);

		/**	Sort the pair list.
				The atom pairs in the list ar sorted in such a way, that those atom pairs
				where at least one of the atoms is selected are at the beginning of the list.
				The number of such pairs is returned. Running time is linear in the length of the list.
				@return number of atom pairs in the list, where at least one atom is selected
				@param pair_vector the unsorted pair_list (modified during the sorting)
		*/
		BALL_EXPORT Size sortNonBondedAtomPairsAfterSelection(ForceField::PairVector& pair_vector);

		/**	Merge the non-overlapping molecules of a system into another system.
				Fills <tt>system</tt> with copies of the solvent molecules stored in	<tt>solvent</tt>. 
				Solvent molecules are added to <tt>system</tt> if the molecules lie in the 
				<tt>box</tt> and if they do not overlap with	the molecules in <tt>solute_grid</tt>. 
				A molecule has an overlap with another molecule if any atom of the first 
				molecule is within <tt>	distance</tt> of any atom of the other molecule.
				@param	system the system to be filled
				@param	solute_grid a hash grid containing the solute molecules
				@param	solvent the system containing the solvent
				@param	box the periodic boundary of <tt>system</tt>
				@param	distance used to determine overlaps between two atoms
				@return	the number of molecules added to <tt>system</tt>
		*/
		BALL_EXPORT BALL::Size addNonOverlappingMolecules
			(System& system, const HashGrid3<const Atom*>& solute_grid,
			 const System& solvent, const SimpleBox3& box, double distance);

		/** Adapt periodic water (or general solvent) boxes to our periodic
				boundary definition. 
				Periodic water boxes created by different programs might contain
				molecules which have their center of gravity outside of the box
				because of a different defintion of the periodic boundary. This
				function adapts such boxes to our definition by simply translating
				such molecules to the opposite boundary wall such that their center
				of gravity lies in the box afterwards. \par
				This might be obsoleted by PeriodicBoundarry::updateMolecules()
				@param system the system containing the water box which is to be adapted
				@param box the box definition for the periodic boundary
		*/
		BALL_EXPORT void adaptWaterBox(System& system, const SimpleBox3& box);

		/** Compute the minimum image
		*/
		// ?????
		BALL_EXPORT void calculateMinimumImage
			(Vector3& distance, const Vector3& period);

		/**	Compute all torsions in a given set of molecules.
				@return the number of torsions added to <tt>torsions</tt>
				@param start an iterator pointing to the start of the atoms
				@param end a past-the-end iterator for the atoms 
				@param torsions the vector of torsions
				@param use_selection if set to <b>true</b>, a torsion will be added only if all four atoms are selected
		*/
    template <typename TorsionType, typename AtomIteratorType>
    Size computeTorsions
			(const AtomIteratorType& start, const AtomIteratorType& end,
			 std::vector<TorsionType>& torsions, bool use_selection = false);


		// 
		template <typename TorsionType, typename AtomIteratorType>
		Size computeTorsions
			(const AtomIteratorType& start, const AtomIteratorType& end, 
			 std::vector<TorsionType>& torsions, bool use_selection)
		{
			// pointers to the four atoms
			Atom*	a1;
			Atom*	a2;
			Atom*	a3;
			Atom*	a4;

			Size number_of_added_torsions = 0;

			// Iterate over all atoms...
			// 
			AtomIteratorType atom_it = start;
			for (; atom_it != end; ++atom_it) 
			{
				// ...and check each bond whether it is part of 
				// a torsion.
				Atom::BondIterator it1 = (*atom_it)->beginBond();
				for (; +it1 ; ++ it1) 
				{
					// Consider each bond just once by making sure that
					// our start atom is the *first* atom of the bond.
					if (*atom_it == it1->getFirstAtom()) 
					{
						// We know have the two central atoms of a potential
						// torsion and store them in a2 and a3.
						a2 = *atom_it;
						a3 = const_cast<Atom*>(it1->getSecondAtom());

						// Now, find all other atoms (atoms 1 and 4)
						Atom::BondIterator it2;
						Atom::BondIterator it3;
						for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
						{
							if (it2->getSecondAtom() != it1->getSecondAtom()) 
							{
								// determine the first atom
								if (it2->getFirstAtom() == *atom_it) 
								{
									a1 = const_cast<Atom*>(it2->getSecondAtom());
								} 
								else 
								{
									a1 = const_cast<Atom*>(it2->getFirstAtom());
								}
	 
								for (it3 = const_cast<Atom*>(it1->getSecondAtom())->beginBond(); +it3 ; ++it3) 
								{
									if (it3->getFirstAtom() != a2 ) 
									{
										// determine the fourth atom a4
										if (it3->getFirstAtom() == a3)
										{
											a4 = const_cast<Atom*>(it3->getSecondAtom());
										} 
										else 
										{
											a4 = const_cast<Atom*>(it3->getFirstAtom());
										}

										if (use_selection == false 
												|| (use_selection == true 
														&& (a1->isSelected() || a2->isSelected() || a3->isSelected() || a4->isSelected())))
										{
											// Push the torsion onto the torsion vector.
											TorsionType tmp;
											tmp.atom1 = a1;
											tmp.atom2 = a2;
											tmp.atom3 = a3;
											tmp.atom4 = a4;

											torsions.push_back(tmp);
											number_of_added_torsions++;
										}
									} 
								}
							}
						}
					}
				}
			}

			// return the number of torsions computed
			return number_of_added_torsions;
		}

	} // namespace MolmecSupport
} // namespace BALL

#endif // BALL_MOLMEC_COMMON_SUPPORT_H
