// $Id: chain.h,v 1.8 2000/04/14 13:09:54 amoll Exp $

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
		: public BaseFragment
	{
		public:

		BALL_CREATE(Chain)
		
		/**	@name	Enums
		*/
		//@{

		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = Fragment::NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constrcutor
		Chain();
	
		/// Copy constructor
		Chain(const Chain& chain, bool deep = true);
	
		/// Detailled constructor
		Chain(const String& name);

		/// Destructor
		virtual ~Chain();

		/// Clears the chains contents
		virtual void clear();
	
		/// Clears the chains contents and removes the cahin from all composite structures
		virtual void destroy();
			
		//@}

		/** @name Persistence */
		//@{

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}
		
		/**	@name Assignment */
		//@{

		/** Assignment with cloning facility.
				Assign the chain {\em base_fragment} to {\em *this} chain.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em chain}
		*/
		void set(const Chain& chain, bool deep = true);

		/** Assignment operator.
				Assign the chain {\em chain} to {\em *this} chain.
				The assignment is either deep or shallow (default).
				@param   chain the chain to be copied (cloned)
				@return  chain& - {\em *this} chain
				@see     chain::set
		*/
		Chain& operator = (const Chain& chain);

		/** Copying with cloning facility.
				Copy {\em *this} chain to the chain {\em chain}.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be assigned to
				@see    chain::set
		*/
		void get(Chain& chain, bool deep = true) const;

		/** Swapping of chain.
				Swap the states of {\em *this} chain with the chain {\em chain}.
				@param  chain the chain {\em *this} chain is being swapped with
		*/
		void swap(Chain& chain);

		//@}
	
		/**	@name	Accessors */
		//@{

		/** Get a pointer to the parent protein.
				The reference is 0 if {\em *this} chain does not have a parent protein.
				@return  Protein* -
								 mutable reference to the parent protein of {\em *this} chain,
		*/
		Protein* getProtein();

		/** Get a constant pointer to the parent protein.
				The reference is 0 if {\em *this} chain does not have a parent protein.
				@return  Protein* -
								 constant reference to the parent protein of {\em *this} chain,
		*/
		const Protein* getProtein() const;

		/** Get a pointer to a subaltern SecondaryStructure at a given position.
				The reference is 0 if {\em *this} chain does not have a SecondaryStructure at this position.
				@param   index the position of the subaltern SecondaryStructure
				@exception IndexUnderflow if {\tt index < 0}
				@return  SecondaryStructure* -
								 mutable reference to the subaltern SecondaryStructure at positon {\em index} of {\em *this} chain,
		*/
		SecondaryStructure* getSecondaryStructure(Index index);
	
		/** Get a constant pointer to a subaltern SecondaryStructure at a given position.
				The reference is 0 if {\em *this} chain does not have a SecondaryStructure at this position.
				@param   index the position of the subaltern SecondaryStructure
				@exception IndexUnderflow if {\tt index < 0}
				@return  SecondaryStructure* -
								 constant reference to the subaltern SecondaryStructure at positon {\em index} of {\em *this} chain,
		*/
		const SecondaryStructure* getSecondaryStructure(Index index) const;
	
		/** Get a pointer to a subaltern Residue at a given position.
				The reference is 0 if {\em *this} chain does not have a Residue at this position.
				@param   index the position of the subaltern Residue
				@exception IndexUnderflow if {\tt index < 0}
				@return  Residue* -
								 mutable reference to the subaltern Residue at positon {\em index} of {\em *this} chain,
		*/
		Residue* getResidue(Index index);
	
		/** Get a pointer to a subaltern Residue at a given position.
				The reference is 0 if {\em *this} chain does not have a Residue at this position.
				@param   index the position of the subaltern Residue
				@exception IndexUnderflow if {\tt index < 0}
				@return  Residue* -
								 constant reference to the subaltern Residue at positon {\em index} of {\em *this} chain,
		*/
		const Residue* getResidue(Index index) const;
	
		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} chain does not have a Residue at this position.

				@return  Residue* -
								 mutable reference to the N-terminal Residue
		*/
		Residue* getNTerminal();
	
		/** Get a constant pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} chain does not have a Residue at this position.

				@return  Residue* -
								 constant reference to the N-terminal Residue
		*/
		const Residue* getNTerminal() const;

		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} chain does not have a Residue at this position.

				@return  Residue* -
								 mutable reference to the C-terminal Residue
		*/
		Residue* getCTerminal();
	
		/** Get a constant pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} chain does not have a Residue at this position.

				@return  Residue* -
								 constant reference to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a subaltern PDBAtom at a given position.
				The reference is 0 if {\em *this} chain does not have a PDBAtom at this position.
				@param   index the position of the subaltern PDBAtom
				@exception IndexUnderflow if {\tt index < 0}
				@return  PDBAtom* -
								 mutable reference to the subaltern PDBAtom at positon {\em index} of {\em *this} chain,
		*/
		PDBAtom* getPDBAtom(Index index);
	
		/** Get a pointer to a subaltern PDBAtom at a given position.
				The reference is 0 if {\em *this} chain does not have a PDBAtom at this position.
				@param   index the position of the subaltern PDBAtom
				@exception IndexUnderflow if {\tt index < 0}
				@return  PDBAtom* -
								 constant reference to the subaltern PDBAtom at positon {\em index} of {\em *this} chain,
		*/
		const PDBAtom* getPDBAtom(Index index) const;
	
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
				@param secondary_structure, the SecondaryStructure to prepend
		*/
		void prepend(SecondaryStructure& secondary_structure);

		/** Append a SecondaryStructure after the last position.
				@param secondary_structure, the SecondaryStructure to append
		*/
		void append(SecondaryStructure& secondary_structure);

		/** Insert a SecondaryStructure after the last position.
				@param secondary_structure, the SecondaryStructure to insert
		*/
		void insert(SecondaryStructure& secondary_structure);

		/** Insert a SecondaryStructure before a given {\em Comosite} object.
				@param secondary_structure, the SecondaryStructure to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(SecondaryStructure& secondary_structure, Composite& before);

		/** Insert a SecondaryStructure after a given {\em Comosite} object.
				@param secondary_structure, the SecondaryStructure to insert
				@param after, the {\em Comosite} object to insert before
		*/
		void insertAfter(SecondaryStructure& secondary_structure, Composite& after);

		/** Remove a SecondaryStructure
				@param secondary_structure the SecondaryStructure to remove
		*/
		bool remove(SecondaryStructure& secondary_structure);

		/** Prepend a Residue at position 0.
				@param residue, the Residue to prepend
		*/
		void prepend(Residue& residue);

		/** Append a Residue after the last position.
				@param residue, the Residue to append
		*/
		void append(Residue& residue);

		/** Insert a Residue after the last position.
				@param residue, the Residue to insert
		*/
		void insert(Residue& residue);

		/** Insert a Residue before a given {\em Comosite} object.
				@param residue, the Residue to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Residue& residue,Composite& before);

		/** Insert a Residue after a given {\em Comosite} object.
				@param residue, the Residue to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(Residue& residue, Composite& after);

		/** Remove a Residue.
				@param residue the Residue to remove
		*/
		bool remove(Residue& residue);

		/**	Cut all children of {\tt chain} and prepend them before the children of this chain.
				@param chain the chain to access
		*/
		void spliceBefore(Chain& chain);

		/**	Cut all children of {\tt chain} and append them after the children of this chain.
				@param chain the chain to access
		*/
		void spliceAfter(Chain &chain);

		/**	Move the children of {\tt chain} into this chain.
				The children of {\tt chain} are inserted at the position of 
				{\tt chain} if {\tt chain} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(Chain &chain);
		
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this} chain.
				If the internal state of {\em *this} chain is correct (self-validated) and consistent {\tt true} is returned,
				{\tt false} otherwise. 
				@return			bool -
										{\tt true} if the internal state of {\em *this} chain is correct (self-validated) and consistent,
										{\tt false} otherwise
		*/
		virtual bool isValid() const;
		
		/** Internal state dump.
				Dump the current internal state of {\em *this} chain to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param   s - output stream where to output the internal state of {\em *this} chain
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		// --- STORERS

		/* Persistent stream input and state restorage.
				Read persistent chain data from the input stream {\em s} and restore the state of {\em *this} chain.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this} chain
		*/
		virtual void read(std::istream& s);

		/* Persistent stream output and state storage.
				Write persistent chain data to the output stream {\em s} and store the state of {\em *this} chain.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this} chain
		*/
		virtual void write(std::ostream& s) const;

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

	protected:

	private:

		BaseFragment* getBaseFragment(Index index);
	
		const BaseFragment* getBaseFragment(Index index) const;
	
		Atom* getAtom(Index index);
	
		const Atom* getAtom(Index index) const;

		void prepend(Atom& atom);

		void append(Atom& atom);

		void insert(Atom& atom);

		void insertBefore(Atom& atom, Composite& before);

		void insertAfter(Atom& atom, Composite& after);

		bool remove(Atom& atom);

		void prepend(BaseFragment& base_fragment);

		void append(BaseFragment& base_fragment);

		void insert(BaseFragment& base_fragment);

		void insertBefore(BaseFragment& base_fragment, Composite& before);

		void insertAfter(BaseFragment& base_fragment, Composite& after);

		void spliceBefore(BaseFragment& base_fragment);

		void spliceAfter(BaseFragment& base_fragment);

		void splice(BaseFragment& base_fragment);

		bool remove(BaseFragment& base_fragment);

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)

		void clear_();
	};

} // namespace BALL

#endif // BALL_KERNEL_CHAIN_H
