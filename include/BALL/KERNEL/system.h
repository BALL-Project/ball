// $Id: system.h,v 1.18 2001/01/14 21:57:15 amoll Exp $

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

		/**	@name	Constructors and Destructors*/
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
	
		/** @name Persistence */
		//@{

		/**	Persistent writing.
				Writes a System object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a System object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw();

		//@}

	
		/**	@name	Assignemnt */
		//@{

		/** Assignment with cloning facility.
				Assign {\em system} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param  system the System to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em system}
		*/
		void set(const System& system, bool deep = true)
			throw();

		/** Assignment operator.
				Assign {\em system} to {\em *this}.
				@param   system the System to be copied (cloned)
				@return  System& - {\em *this}
				@see     System::set
		*/
		const System& operator = (const System& system)
			throw();

		/** Copying with cloning facility.
				Copy {\em *this} to {\em system}.
				The assignment is either deep or shallow (default).
				@param  System the System to be assigned to
				@see    System::set
		*/
		void get(System& system, bool deep = true) const
			throw();
	
		//@}

		/**	Equality operator.
				Two Systems are equal if they have the same attributes and properties and are homomorph.
				@see AtomContainer::operator ==
		*/
		bool operator == (const System& system) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const System& system) const
			throw();


		/**	@name	Accessors */
		//@{

		/** Get a pointer to a child Molecule at a given position.
				The reference is 0 if {\em *this} does not have a Molecule at this position.
				@param   position the position of the child molecule
				@return  Molecule* -
								 mutable reference to the child molecule at {\em position} of {\em *this}
		*/
		Molecule* getMolecule(Position position)
			throw();

		/** Get a pointer to a child Molecule at a given position.
				The reference is 0 if {\em *this} does not have a Molecule at this position.
				@param   position the position of the child molecule
				@return  Molecule* -
								 constant reference to the child molecule at {\em position} of {\em *this}
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

		/** Insert a molecule before a given {\em Comosite} object.
				@param molecule, the molecule to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Molecule& molecule, Composite& before)
			throw();

		/** Insert a molecule after a given {\em Comosite} object.
				@param molecule, the molecule to insert
				@param after, the {\em Comosite} object to insert before
		*/
		void insertAfter(Molecule& molecule, Composite& after)
			throw();

		/** Remove a molecule.
				@param molecule the molecule to remove
		*/
		bool remove(Molecule& molecule)
			throw();

		/**	Move the children of {\tt system} into {\em *this}.
				Cut all children of {\tt system} and prepend them before the children of {\em *this}.
				@param system the system to access
		*/
		void spliceBefore(System& system)
			throw();

		/**	Move the children of {\tt system} into {\em *this}.
				Cut all children of {\tt system} and append them after the children of {\em *this}.
				@param system the system to access
		*/
		void spliceAfter(System& system)
			throw();

		/**	Move the children of {\tt system} into {\em *this}.
				The children are inserted using \Ref{spliceBefore}.
		*/
		void splice(System& system)
			throw();
		//@}

		/**	@name	Storers */
		//@{

		/* Persistent stream input and state restorage.
				Read persistent data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void read(std::istream& s)
			throw();

		/* Persistent stream output and state storage.
				Write persistent data to the output stream {\em s} and store the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void write(std::ostream& s) const
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
