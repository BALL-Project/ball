// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_CHAIN_H
#define BALL_KERNEL_CHAIN_H

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H
#	include <BALL/KERNEL/secondaryStructureIterator.h>
#endif


#define BALL_CHAIN_DEFAULT_NAME   ' '

namespace BALL
{
	class Protein;

	/** Protein chain class.
			This class represents a polypeptide chain within a  \link Protein Protein \endlink .
			Chains can contain  \link SecondaryStructure SecondaryStructure \endlink s or  \link Residue Residue \endlink s.
			 \par
			
	 		\ingroup KernelContainers 
	*/
	class BALL_EXPORT Chain
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
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Default constrcutor
		Chain();

		/// Copy constructor
		Chain(const Chain& chain, bool deep = true);

		/// Detailled constructor
		Chain(const String& name);

		/// Destructor
		virtual ~Chain();

		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a Chain object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a Chain object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name Assignment 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const Chain& chain, bool deep = true);

		/** Assignment operator.
				The assignment is deep.
				@param   chain the chain to be copied (cloned)
				@return  chain& - this instance.
				@see     chain::set
		*/
		Chain& operator = (const Chain& chain);

		/** Copying with cloning facility.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be assigned to
				@see    chain::set
		*/
		void get(Chain& chain, bool deep = true) const;

		/** Swapping of chains.
				@param  chain the chain this instance is being swapped with
		*/
		void swap(Chain& chain);

		//@}

		/**	Equality operator.
				Two chains are equal if they have the same handle.
				@see Object::operator ==
		*/
		bool operator == (const Chain& chain) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Chain& chain) const;


		/**	@name	Accessors 
		*/
		//@{

		/** Get a pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - mutable pointer to the parent protein
		*/
		Protein* getProtein();

		/** Get a constant pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - constant pointer to the parent protein
		*/
		const Protein* getProtein() const;

		/** Get a pointer to a child SecondaryStructure at a given position.
				The pointer is 0 if this instance does not have a SecondaryStructure at this position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* -
								 mutable pointer to the child SecondaryStructure at <b>  position </b>
		*/
		SecondaryStructure* getSecondaryStructure(Position position);

		/** Get a constant pointer to a child SecondaryStructure at a given position.
				The pointer is 0 if this instance does not have a SecondaryStructure at this position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* -
								 constant pointer to the child SecondaryStructure at <b>  position </b>
		*/
		const SecondaryStructure* getSecondaryStructure(Position position) const;

		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at this position.
				@param   position the position of the child Residue
				@return  Residue* - mutable pointer to the child Residue at <b>  position </b>
		*/
		Residue* getResidue(Position position);

		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at this position.
				@param   position the position of the child Residue
				@return  Residue* - constant pointer to the child Residue at <b>  position </b>
		*/
		const Residue* getResidue(Position position) const;

		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the N-terminal Residue
		*/
		Residue* getNTerminal();

		/** Get a constant pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the N-terminal Residue
		*/
		const Residue* getNTerminal() const;

		/** Get a pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the C-terminal Residue
		*/
		Residue* getCTerminal();

		/** Get a constant pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - mutable pointer to the child PDBAtom at <b>  position </b>
		*/
		PDBAtom* getPDBAtom(Position position);

		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - constant pointer to the child PDBAtom at <b>  position </b>
		*/
		const PDBAtom* getPDBAtom(Position position) const;

		/** Count the SecondaryStructures
				@return  Size the number of secondary structures
		*/
		Size countSecondaryStructures() const;

		/** Count the Residues
				@return  Size the number of residues
		*/
		Size countResidues() const;

		/** Count the PDBAtoms
				@return  Size the number of PDBAtoms
		*/
		Size countPDBAtoms() const;

		/** Prepend a SecondaryStructure at position 0.
				@param secondary_structure the SecondaryStructure to prepend
		*/
		void prepend(SecondaryStructure& secondary_structure);

		/** Append a SecondaryStructure after the last position.
				@param secondary_structure the SecondaryStructure to append
		*/
		void append(SecondaryStructure& secondary_structure);

		/** Insert a SecondaryStructure after the last position.
				@param secondary_structure the SecondaryStructure to insert
		*/
		void insert(SecondaryStructure& secondary_structure);

		/** Insert a SecondaryStructure before a given Composite object.
				@param secondary_structure the SecondaryStructure to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(SecondaryStructure& secondary_structure, Composite& before);

		/** Insert a SecondaryStructure after a given Composite object.
				@param secondary_structure the SecondaryStructure to insert
				@param after the Composite object to insert after
		*/
		void insertAfter(SecondaryStructure& secondary_structure, Composite& after);

		/** Remove a SecondaryStructure
				@param secondary_structure the SecondaryStructure to remove
		*/
		bool remove(SecondaryStructure& secondary_structure);

		/** Prepend a Residue at position 0.
				@param residue the Residue to prepend
		*/
		void prepend(Residue& residue);

		/** Append a Residue after the last position.
				@param residue the Residue to append
		*/
		void append(Residue& residue);

		/** Insert a Residue after the last position.
				@param residue the Residue to insert
		*/
		void insert(Residue& residue);

		/** Insert a Residue before a given Composite object.
				@param residue the Residue to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(Residue& residue, Composite& before);

		/** Insert a Residue after a given Composite object.
				@param residue the Residue to insert
				@param after the Composite object to insert after
		*/
		void insertAfter(Residue& residue, Composite& after);

		/** Remove a Residue.
				@param residue the Residue to remove
		*/
		bool remove(Residue& residue);

		/**	Cut all children of <tt>chain</tt> and prepend them before the children of this chain.
				@param chain the chain to access
		*/
		void spliceBefore(Chain& chain);

		/**	Cut all children of <tt>chain</tt> and append them after the children of this chain.
				@param chain the chain to access
		*/
		void spliceAfter(Chain &chain);

		/**	Move the children of <tt>chain</tt> into this chain.
				The children of <tt>chain</tt> are inserted using  \link spliceBefore spliceBefore \endlink .
		*/
		void splice(Chain &chain);

		//@}

		/** @name Type predicates */
		//@{
		/// @copydoc Composite::isChain
		virtual bool isChain() const { return true; }
		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state to the output 
				ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state.
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
;

		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

		BALL_DECLARE_STD_ITERATOR_WRAPPER(Chain, SecondaryStructure, secondaryStructures)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Chain, Residue           , residues)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Chain, PDBAtom           , pdbAtoms)

	protected:

	private:
		AtomContainer* getAtomContainer(Position position);

		const AtomContainer* getAtomContainer(Position position) const;

		Atom* getAtom(Position position);

		const Atom* getAtom(Position position) const;

		void prepend(Atom& atom);

		void append(Atom& atom);

		void insert(Atom& atom);

		void insertBefore(Atom& atom, Composite& before);

		void insertAfter(Atom& atom, Composite& after);

		bool remove(Atom& atom);

		void prepend(AtomContainer& atom_container);

		void append(AtomContainer& atom_container);

		void insert(AtomContainer& atom_container);

		void insertBefore(AtomContainer& atom_container, Composite& before);

		void insertAfter(AtomContainer& atom_container, Composite& after);

		void spliceBefore(AtomContainer& atom_container);

		void spliceAfter(AtomContainer& atom_container);

		void splice(AtomContainer& atom_container);

		bool remove(AtomContainer& atom_container);

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Chain, AtomContainer, atomContainers)
	};
} // namespace BALL

#endif // BALL_KERNEL_CHAIN_H
