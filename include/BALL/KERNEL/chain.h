// $Id: chain.h,v 1.20 2001/01/14 21:57:14 amoll Exp $

#ifndef BALL_KERNEL_CHAIN_H
#define BALL_KERNEL_CHAIN_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H
#	include <BALL/KERNEL/secondaryStructureIterator.h>
#endif


#define BALL_CHAIN_DEFAULT_NAME   ' '

namespace BALL 
{
	
	class Protein;

	/** Protein Chain Class.
			{\bf Definition:}\URL{BALL/KERNEL/chain.h}
	*/
	class Chain
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(Chain)
		
		/**	@name	Enums
		*/
		//@{

		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = AtomContainer::NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constrcutor
		Chain()
			throw();
	
		/// Copy constructor
		Chain(const Chain& chain, bool deep = true)
			throw();
	
		/// Detailled constructor
		Chain(const String& name)
			throw();

		/// Destructor
		virtual ~Chain()
			throw();
			
		//@}

		/** @name Persistence */
		//@{

		/**	Persistent writing.
				Writes a Chain object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a Chain object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw();

		//@}
		
		/**	@name Assignment */
		//@{

		/** Assignment with cloning facility.
				Assign the chain {\em chain} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em chain}
		*/
		void set(const Chain& chain, bool deep = true)
			throw();

		/** Assignment operator.
				Assign the chain {\em chain} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param   chain the chain to be copied (cloned)
				@return  chain& - {\em *this}
				@see     chain::set
		*/
		const Chain& operator = (const Chain& chain)
			throw();

		/** Copying with cloning facility.
				Copy {\em *this} to the chain {\em chain}.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be assigned to
				@see    chain::set
		*/
		void get(Chain& chain, bool deep = true) const
			throw();

		/** Swapping of chains.
				Swap the states of {\em *this} with the chain {\em chain}.
				@param  chain the chain {\em *this} is being swapped with
		*/
		void swap(Chain& chain)
			throw();

		//@}
	
		/**	Equality operator.
				Two chains are equal if they have the same attributes and properties and are homomorph.
				@see AtomContainer::operator ==
		*/
		bool operator == (const Chain& chain) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Chain& chain) const
			throw();


		/**	@name	Accessors */
		//@{

		/** Get a pointer to the parent protein.
				The reference is 0 if {\em *this} does not have a parent protein.
				@return  Protein* -
								 mutable reference to the parent protein of {\em *this},
		*/
		Protein* getProtein()
			throw();

		/** Get a constant pointer to the parent protein.
				The reference is 0 if {\em *this} does not have a parent protein.
				@return  Protein* -
								 constant reference to the parent protein of {\em *this},
		*/
		const Protein* getProtein() const
			throw();

		/** Get a pointer to a child SecondaryStructure at a given position.
				The reference is 0 if {\em *this} does not have a SecondaryStructure at this position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* -
								 mutable reference to the child SecondaryStructure at {\em position} of {\em *this},
		*/
		SecondaryStructure* getSecondaryStructure(Position position)
			throw();
	
		/** Get a constant pointer to a child SecondaryStructure at a given position.
				The reference is 0 if {\em *this} does not have a SecondaryStructure at this position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* -
								 constant reference to the child SecondaryStructure at {\em position} of {\em *this},
		*/
		const SecondaryStructure* getSecondaryStructure(Position position) const
			throw();
	
		/** Get a pointer to a child Residue at a given position.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@param   position the position of the child Residue
				@return  Residue* -
								 mutable reference to the child Residue at {\em position} of {\em *this},
		*/
		Residue* getResidue(Position position)
			throw();
	
		/** Get a pointer to a child Residue at a given position.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@param   position the position of the child Residue
				@return  Residue* -
								 constant reference to the child Residue at {\em position} of {\em *this},
		*/
		const Residue* getResidue(Position position) const
			throw();
	
		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@return  Residue* -
								 mutable reference to the N-terminal Residue
		*/
		Residue* getNTerminal()
			throw();
	
		/** Get a constant pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@return  Residue* -
								 constant reference to the N-terminal Residue
		*/
		const Residue* getNTerminal() const
			throw();

		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@return  Residue* -
								 mutable reference to the C-terminal Residue
		*/
		Residue* getCTerminal()
			throw();
	
		/** Get a constant pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@return  Residue* -
								 constant reference to the C-terminal Residue
		*/
		const Residue* getCTerminal() const
			throw();

		/** Get a pointer to a child PDBAtom at a given position.
				The reference is 0 if {\em *this} does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* -
								 mutable reference to the child PDBAtom at {\em position} of {\em *this},
		*/
		PDBAtom* getPDBAtom(Position position)
			throw();
	
		/** Get a pointer to a child PDBAtom at a given position.
				The reference is 0 if {\em *this} does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* -
								 constant reference to the child PDBAtom at {\em position} of {\em *this},
		*/
		const PDBAtom* getPDBAtom(Position position) const
			throw();
	
		/** Count the SecondaryStructures
				@return  Size the number of secondary structures
		*/
		Size countSecondaryStructures() const
			throw();

		/** Count the Residues
				@return  Size the number of residues
		*/
		Size countResidues() const
			throw();

		/** Count the PDBAtoms
				@return  Size the number of PDBAtoms
		*/
		Size countPDBAtoms() const
			throw();

		/** Prepend a SecondaryStructure at position 0.
				@param secondary_structure, the SecondaryStructure to prepend
		*/
		void prepend(SecondaryStructure& secondary_structure)
			throw();

		/** Append a SecondaryStructure after the last position.
				@param secondary_structure, the SecondaryStructure to append
		*/
		void append(SecondaryStructure& secondary_structure)
			throw();

		/** Insert a SecondaryStructure after the last position.
				@param secondary_structure, the SecondaryStructure to insert
		*/
		void insert(SecondaryStructure& secondary_structure)
			throw();

		/** Insert a SecondaryStructure before a given {\em Comosite} object.
				@param secondary_structure, the SecondaryStructure to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(SecondaryStructure& secondary_structure, Composite& before)
			throw();

		/** Insert a SecondaryStructure after a given {\em Comosite} object.
				@param secondary_structure, the SecondaryStructure to insert
				@param after, the {\em Comosite} object to insert before
		*/
		void insertAfter(SecondaryStructure& secondary_structure, Composite& after)
			throw();

		/** Remove a SecondaryStructure
				@param secondary_structure the SecondaryStructure to remove
		*/
		bool remove(SecondaryStructure& secondary_structure)
			throw();

		/** Prepend a Residue at position 0.
				@param residue, the Residue to prepend
		*/
		void prepend(Residue& residue)
			throw();

		/** Append a Residue after the last position.
				@param residue, the Residue to append
		*/
		void append(Residue& residue)
			throw();

		/** Insert a Residue after the last position.
				@param residue, the Residue to insert
		*/
		void insert(Residue& residue)
			throw();

		/** Insert a Residue before a given {\em Comosite} object.
				@param residue, the Residue to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Residue& residue,Composite& before)
			throw();

		/** Insert a Residue after a given {\em Comosite} object.
				@param residue, the Residue to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(Residue& residue, Composite& after)
			throw();

		/** Remove a Residue.
				@param residue the Residue to remove
		*/
		bool remove(Residue& residue)
			throw();

		/**	Cut all children of {\tt chain} and prepend them before the children of this chain.
				@param chain the chain to access
		*/
		void spliceBefore(Chain& chain)
			throw();

		/**	Cut all children of {\tt chain} and append them after the children of this chain.
				@param chain the chain to access
		*/
		void spliceAfter(Chain &chain)
			throw();

		/**	Move the children of {\tt chain} into this chain.
				The children of {\tt chain} are inserted using \Ref{spliceBefore}.
		*/
		void splice(Chain &chain)
			throw();
		
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
		
		/** Internal state dump.
				Dump the current internal state of {\em *this} to the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}

		// --- STORERS

		/* Persistent stream input and state restorage.
				Read persistent chain data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void read(std::istream& s)
			throw();

		/* Persistent stream output and state storage.
				Write persistent chain data to the output stream {\em s} and store the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void write(std::ostream& s) const
			throw();

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

	protected:

	private:

		AtomContainer* getAtomContainer(Position position)
			throw();
	
		const AtomContainer* getAtomContainer(Position position) const
			throw();
	
		Atom* getAtom(Position position)
			throw();
	
		const Atom* getAtom(Position position) const
			throw();

		void prepend(Atom& atom)
			throw();

		void append(Atom& atom)
			throw();

		void insert(Atom& atom)
			throw();

		void insertBefore(Atom& atom, Composite& before)
			throw();

		void insertAfter(Atom& atom, Composite& after)
			throw();

		bool remove(Atom& atom)
			throw();

		void prepend(AtomContainer& atom_container)
			throw();

		void append(AtomContainer& atom_container)
			throw();

		void insert(AtomContainer& atom_container)
			throw();

		void insertBefore(AtomContainer& atom_container, Composite& before)
			throw();

		void insertAfter(AtomContainer& atom_container, Composite& after)
			throw();

		void spliceBefore(AtomContainer& atom_container)
			throw();

		void spliceAfter(AtomContainer& atom_container)
			throw();

		void splice(AtomContainer& atom_container)
			throw();

		bool remove(AtomContainer& atom_container)
			throw();

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)
	};

} // namespace BALL

#endif // BALL_KERNEL_CHAIN_H
