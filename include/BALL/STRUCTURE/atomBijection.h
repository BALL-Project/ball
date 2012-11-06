// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ATOMBIJECTION_H
#define BALL_STRUCTURE_ATOMBIJECTION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

namespace BALL 
{

	/**	Atom bijection.
			This class implements a mapping of two sets of atoms onto each other.
			It is used by the \link StructureMapper StructurMapper \endlink class
			and the \link RMSDMinimizer RMSDMinimizer \endlink classes to define
			which atoms are mapped onto each other.
			\p
			There are a few methods for general mappings (based on atom order, atom names, etc.)
			that should suffice for most applications. If you want to match proteins based on
			particular mappings (e.g. based on a pairwise alignment), you should create the mapping
			yourself. This is easily done by pushing an AtomPair into the vector:
			\code
				Atom* atom1 = ...;
				Atom* atom2 = ...;
	
				// Create an empty bijection
				AtomBijection bijection;

				// Map atom1 onto atom2.
				bijection.push_back(AtomBijection::AtomPair(atom1, atom2));
			\endcode
			\p
			The class behaves more or less like the vector of atom pointer pairs it
			truly is. In particular, the STL container interface has been fully 
			implemented.
	\ingroup StructureMapping
	*/
	class BALL_EXPORT AtomBijection
		: public std::vector<std::pair<Atom*, Atom*> >
	{
		public:

		/** @name Type definitions */
		//@{
		/** A struct for representing an atom pair of the mapping.
		*/
		typedef std::pair<Atom*, Atom*> AtomPair;
		typedef std::vector<std::pair<Atom*, Atom*> > PairVector;
		//@}
		
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		AtomBijection() {}

		/**	Construct a trivial bijection between to atom containers.
				Construct a simple bijection mapping the atoms of the two
				atom containers onto each other. The mapping iterates
				over the atoms and stops assigning pairs of atoms as soon
				as the smaller of the two atom sets is fully assigned. 
				The larger of the two atom container can thus contain
				unassigned atoms. No checking with respect to atom names,
				elements or the like are being made.
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				This corresponds to calling assignTrivial after default
				construction
		*/
		AtomBijection(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		///	Destructor
		virtual ~AtomBijection() {}

		//@}

		/**	@name Bijection construction */
		//@{
		/** Assign all atoms in the two atom containers in order.
				Construct a simple bijection mapping the atoms of the two
				atom containers onto each other. The mapping iterates
				over the atoms and stops assigning pairs of atoms as soon
				as the smaller of the two atom sets is fully assigned. 
				The larger of the two atom container can thus contain
				unassigned atoms. No checking with respect to atom names,
				elements or the like are being made.
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				This trivial bijection is useful, if the two atom containers
				correspond to exactly the same structure (i.e. they 
				just differ in their conformations). 
        Care must be taken that the order of atoms is correct.
        Beware of adding hydrogens, which might mess up atom 
        order in some cases.
				\p
				The number of atoms mapped is returned.
		*/
		Size assignTrivial(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		/**	Assign the atom pairs through a name matching.
				This method creates a mapping based on the atom names.
				If the atom is contained in a Residue/Protein, the name consists
				of the fully qualified name (<chain>:<residue name>:<residue id>:<atom name>).
				If no pair of atoms could by matched this way, it will try to match by
				atom names only (not considering residues, chains or the like).	
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				The method constructs a hash map for all atom names, so run time is linear 
				in the number of atoms.
				\p
				The number of atoms mapped is returned.
		*/
		Size assignByName(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		/** Assign the C-alpha atoms ordered by sequence.
				This method iterated over all residues and assigns
				the C-alpha atoms (i.e. all atoms named "CA" in a
				residue with the property AMINO_ACID) of the two proteins
				in the order they are traversed. The size of the mapping
				corresponds to the minimum of the number of C-alpha atoms
        of both atom containers.	
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				\return The number of atom pairs mapped
		*/
		Size assignCAlphaAtoms(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);


		/** Assign the backbone atoms ordered by sequence.
				This method iterated over all residues and assigns
				the backbone atoms (i.e. all atoms named "CA", "C",
				"N", "H", and "O" in every residue with the property AMINO_ACID)
				of the two proteins
				in the order they are traversed. The mapping terminates,
				if the traversal of the residues in one of the two atom containers
				terminates.	
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				\return The number of atom pairs mapped
		*/
		Size assignBackboneAtoms(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		/** Assign the atom pairs through a name matching and based on the 
		    property "ATOMBIJECTION_RMSD_SELECTION".
				@see assignTrivial()
				This is restriction is useful, if the focus of investigation
				is limited to e.g. a binding pocket.
		*/
		Size assignAtomsByProperty(AtomContainer& A, AtomContainer& B);
		//@}


		/**	@name Accessors */
		//@{
		///	Calculate the root mean squared deviation of the mapped atoms.
		double calculateRMSD() const;
		//@}

		/**	@name STL container compliance */
		//@{
		///
		using PairVector::size;
		///
		using PairVector::push_back;
		///
		using PairVector::begin;
		///
		using PairVector::end;
		///
		using PairVector::rbegin;
		///
		using PairVector::rend;
		//@}

	};

} // namespace BALL

#endif // BALL_STRUCTURE_ATOMBIJECTION_H
