// $Id: system.h,v 1.23 2001/07/15 16:14:23 oliver Exp $

#ifndef BALL_KERNEL_SYSTEM_H
#define BALL_KERNEL_SYSTEM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_MOLECULEITERATOR_H
#	include <BALL/KERNEL/moleculeIterator.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_PROTEINITERATOR_H
#	include <BALL/KERNEL/proteinIterator.h>
#endif

#ifndef BALL_KERNEL_NUCLEICACID_H
#	include <BALL/KERNEL/nucleicAcid.h>
#endif

#ifndef BALL_KERNEL_NUCLEICACIDITERATOR_H
#	include <BALL/KERNEL/nucleicAcidIterator.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDEIDITERATOR_H
#	include <BALL/KERNEL/nucleotideIterator.h>
#endif

#define BALL_SYSTEM_DEFAULT_NAME   ""

namespace BALL 
{

	class Molecule;

	/** Kernel System Class.
			This class is used to represent a system, i.e., a collection
			of molecules.\\
			{\bf Definition:}\URL{BALL/KERNEL/system.h}
	*/
	class System
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(System)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/// Default constructor
		System()
			throw();
	
		/// Copy constructor
		System(const System& system, bool deep = true)
			throw();
	
		/// Detailled constructor
		System(const String& name)
			throw();

		///	Destructor
		virtual ~System()
			throw();

		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a System object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw(Exception::GeneralException);

		/**	Reads a System object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw(Exception::GeneralException);

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  system the System to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy
		*/
		void set(const System& system, bool deep = true)
			throw();

		/** Assignment operator.
				@param   system the System to be copied (cloned)
				@return  System& - this instance
		*/
		const System& operator = (const System& system)
			throw();

		/** Copying with cloning facility.
				The assignment is either deep or shallow (default).
				@param  System the System to be assigned to
		*/
		void get(System& system, bool deep = true) const
			throw();
	
		//@}

		/**	Equality operator.
				Two instance of System are equal if they have the same handle.
				@see Object::operator ==
		*/
		bool operator == (const System& system) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const System& system) const
			throw();

		/**	@name	Accessors 
		*/
		//@{

		/** Get a pointer to a child Molecule at a given position.
				The pointer is 0 if this instance does not have a Molecule at this position.
				@param   position the position of the child molecule
				@return  Molecule* - mutable pointer to the child molecule at {\em position}
		*/
		Molecule* getMolecule(Position position)
			throw();

		/** Get a pointer to a child Molecule at a given position.
				The pointer is 0 if this instance does not have a Molecule at this position.
				@param   position the position of the child molecule
				@return  Molecule* - constant pointer to the child molecule at {\em position}
		*/
		const Molecule* getMolecule(Position position) const
			throw();

		/** Count the molecules in this system.
				@return Size the number of molecules
		*/
		Size countMolecules() const
			throw();

		/** Count the fragments in this system.
				@return Size the number of fragments
		*/
		Size countFragments() const
			throw();

		/** Count the atoms in this system.
				@return Size the number of atoms
		*/
		Size countAtoms() const
			throw();

		/** Count the proteins in this system.
				@return Size the number of proteins
		*/
		Size countProteins() const
			throw();

		/** Count the chains in this system.
				@return Size the number of chains
		*/
		Size countChains() const
			throw();

		/** Count the secondary structures in this system.
				@return Size the number of secondary structures
		*/
		Size countSecondaryStructures() const
			throw();

		/** Count the residues in this system.
				@return Size the number of residues
		*/
		Size countResidues() const
			throw();

		/** Count the nucleic acids in this system.
				@return Size the number of nucleic acids
		*/
		Size countNucleicAcids() const
			throw();

		/** Count the nucleotides in this system.
				@return Size the number of nucleotides
		*/
		Size countNucleotides() const
			throw();

		/** Prepend a molecule at position 0.
				@param molecule, the molecule to prepend
		*/
		void prepend(Molecule& molecule)
			throw();

		/** Append a molecule after the last position.
				@param molecule, the molecule to append
		*/
		void append(Molecule& molecule)
			throw();

		/** Insert a molecule after the last position.
				@param molecule, the molecule to insert
		*/
		void insert(Molecule& molecule)
			throw();

		/** Insert a molecule before a given Composite object.
				@param molecule, the molecule to insert
				@param before, the Composite object to insert before
		*/
		void insertBefore(Molecule& molecule, Composite& before)
			throw();

		/** Insert a molecule after a given Composite object.
				@param molecule, the molecule to insert
				@param after, the Composite object to insert before
		*/
		void insertAfter(Molecule& molecule, Composite& after)
			throw();

		/** Remove a molecule.
				@param molecule the molecule to remove
		*/
		bool remove(Molecule& molecule)
			throw();

		/**	Move the children of {\tt system} into this instance.
				Cut all children of {\tt system} and prepend them before the children of this instance.
				@param system the system to access
		*/
		void spliceBefore(System& system)
			throw();

		/**	Move the children of {\tt system} into this instance.
				Cut all children of {\tt system} and append them after the children of this instance.
				@param system the system to access
		*/
		void spliceAfter(System& system)
			throw();

		/**	Move the children of {\tt system} into this instance.
				The children are inserted using \Ref{spliceBefore}.
		*/
		void splice(System& system)
			throw();

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

	};

} // namespace BALL

#endif // BALL_KERNEL_SYSTEM_H
