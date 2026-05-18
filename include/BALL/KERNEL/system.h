// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_SYSTEM_H
#define BALL_KERNEL_SYSTEM_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_MOLECULEITERATOR_H
#	include <BALL/KERNEL/moleculeIterator.h>
#endif

#ifndef BALL_KERNEL_PROTEINITERATOR_H
#	include <BALL/KERNEL/proteinIterator.h>
#endif

#ifndef BALL_KERNEL_NUCLEICACIDITERATOR_H
#	include <BALL/KERNEL/nucleicAcidIterator.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDEIDITERATOR_H
#	include <BALL/KERNEL/nucleotideIterator.h>
#endif

#include <memory>  // K0.4.1: std::unique_ptr<MoleculeStore>

#define BALL_SYSTEM_DEFAULT_NAME   ""

namespace BALL
{
	class MoleculeStore;  // K0.4.1 forward decl

	/** System class.
			This class is used to represent a system, i.e., a collection
			of molecules. \par
			
    	\ingroup KernelContainers 
	*/
	class BALL_EXPORT System
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(System)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/// Default constructor
		System();
	
		/// Copy constructor
		System(const System& system, bool deep = true);
	
		/// Detailled constructor
		System(const String& name);

		///	Destructor
		virtual ~System();

		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a System object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a System object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  system the System to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const System& system, bool deep = true);

		/** Assignment operator.
				@param   system the System to be copied (cloned)
				@return  System& - this instance
		*/
		System& operator = (const System& system);

		/** Copying with cloning facility.
				The assignment is either deep or shallow (default).
				@param  System the System to be assigned to
		*/
		void get(System& system, bool deep = true) const;
	
		//@}

		/**	Equality operator.
				Two instance of System are equal if they have the same handle.
				@see Object::operator ==
		*/
		bool operator == (const System& system) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const System& system) const;

		/**	@name	Accessors 
		*/
		//@{

		/** Get a pointer to a child Molecule at a given position.
				The pointer is 0 if this instance does not have a Molecule at this position.

				@note The current implementation of this function has linear complexity

				@param   position the position of the child molecule
				@return  Molecule* - mutable pointer to the child Molecule at <b>  position </b>
		*/
		Molecule* getMolecule(Position position);

		/** Get a pointer to a child Molecule at a given position.
				The pointer is 0 if this instance does not have a Molecule at this position.

				@note The current implementation of this function has linear complexity

				@param   position the position of the child molecule
				@return  Molecule* - constant pointer to the child Molecule at <b>  position </b>
		*/
		const Molecule* getMolecule(Position position) const;

		/** Get a pointer to a child Protein a given position.
		    The pointer is 0 if this instance does not have a Protein at this position.

		    @note The current implementation of this function has linear complexity

		    @param   position the position of the child Protein
		    @return  Protien* - mutable pointer to the child Protein at <b>  position </b>
		*/
		Protein* getProtein(Position position);

		/** Get a pointer to a child Protein a given position.
		    The pointer is 0 if this instance does not have a Protein at this position.

		    @note The current implementation of this function has linear complexity

		    @param   position the position of the child Protein
		    @return  Protien* - mutable pointer to the child Protein at <b>  position </b>
		*/
		const Protein* getProtein(Position position) const;

		/** Count the molecules in this system.
				@return Size the number of molecules
		*/
		Size countMolecules() const;

		/** Count the fragments in this system.
				@return Size the number of fragments
		*/
		Size countFragments() const;

		/** Count the atoms in this system.
				@return Size the number of atoms
		*/
		Size countAtoms() const;

		/** Count the proteins in this system.
				@return Size the number of proteins
		*/
		Size countProteins() const;

		/** Count the chains in this system.
				@return Size the number of chains
		*/
		Size countChains() const;

		/** Count the secondary structures in this system.
				@return Size the number of secondary structures
		*/
		Size countSecondaryStructures() const;

		/** Count the residues in this system.
				@return Size the number of residues
		*/
		Size countResidues() const;

		/** Count the nucleic acids in this system.
				@return Size the number of nucleic acids
		*/
		Size countNucleicAcids() const;

		/** Count the nucleotides in this system.
				@return Size the number of nucleotides
		*/
		Size countNucleotides() const;

		/** Prepend a molecule at position 0.
				@param molecule the molecule to prepend
		*/
		void prepend(Molecule& molecule);

		/** Append a molecule after the last position.
				@param molecule the molecule to append
		*/
		void append(Molecule& molecule);

		/** Insert a molecule after the last position.
				@param molecule the molecule to insert
		*/
		void insert(Molecule& molecule);

		/** Insert a molecule before a given Composite object.
				@param molecule the molecule to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(Molecule& molecule, Composite& before);

		/** Insert a molecule after a given Composite object.
				@param molecule the molecule to insert
				@param after the Composite object to insert before
		*/
		void insertAfter(Molecule& molecule, Composite& after);

		/** Remove a molecule.
				@param molecule the molecule to remove
		*/
		bool remove(Molecule& molecule);

		/**	Move the children of <tt>system</tt> into this instance.
				Cut all children of <tt>system</tt> and prepend them before the children of this instance.
				@param system the system to access
		*/
		void spliceBefore(System& system);

		/**	Move the children of <tt>system</tt> into this instance.
				Cut all children of <tt>system</tt> and append them after the children of this instance.
				@param system the system to access
		*/
		void spliceAfter(System& system);

		/**	Move the children of <tt>system</tt> into this instance.
				The children are inserted using  \link spliceBefore spliceBefore \endlink .
		*/
		void splice(System& system);

		//@}
		
		// --- EXTERNAL ITERATORS ---

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Molecule)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Protein)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Nucleotide)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(NucleicAcid)

		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Atom, atoms)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, AtomContainer, atomContainers)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Fragment, fragments)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Molecule, molecules)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Protein, proteins)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Residue, residues)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Chain, chains)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, SecondaryStructure, secondaryStructures)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, Nucleotide, nucleotides)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(System, NucleicAcid, nucleicAcids)

		//@}
		/** @name v2.0 MoleculeStore (K0.4.1)
		*/
		//@{

		/** Get the per-System MoleculeStore. Every System owns one;
				atoms inserted into this System are adopted into this store
				(via Atom-side bind / migration from the orphan store).
				See KERNEL-V2-DECISIONS.md D5 and V2.0-ROADMAP.md K0.4.
		*/
		MoleculeStore&       getStore();
		const MoleculeStore& getStore() const;

		/** Adopt an atom into this System's MoleculeStore.

				Migrates atom's payload from its current store (typically
				the global orphan store) into this System's store. Updates
				atom.store_ / store_idx_ to point at the new slot. The
				original slot is released back to its store's free-list.

				For each bond incident to this atom in the original store:
				if the bond's partner is already in this System's store,
				re-add the bond here and tombstone the original. If the
				partner is still in the original store, the bond stays
				there until the partner is adopted (at which point that
				adopt call migrates the bond).

				Idempotent: if atom is already in this->getStore(),
				returns immediately.

				K0.4.2 deliverable. Wired into AtomContainer::insert in
				K0.4.3.
		*/
		void adopt(Atom& atom);

		//@}

		private:

		// K0.4.1: per-System MoleculeStore. Owned via unique_ptr so the
		// System destructor cleans it up. Forward-declared to keep this
		// header light; defined in system.C where moleculeStore.h is
		// included.
		std::unique_ptr<MoleculeStore> store_;
	};
} // namespace BALL

#endif // BALL_KERNEL_SYSTEM_H
