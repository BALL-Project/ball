// $Id: chain.h,v 1.13 2000/05/09 16:12:12 amoll Exp $

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
			NUMBER_OF_PROPERTIES = BaseFragment::NUMBER_OF_PROPERTIES
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
				Assign the chain {\em chain} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em chain}
		*/
		void set(const Chain& chain, bool deep = true);

		/** Assignment operator.
				Assign the chain {\em chain} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param   chain the chain to be copied (cloned)
				@return  chain& - {\em *this}
				@see     chain::set
		*/
		Chain& operator = (const Chain& chain);

		/** Copying with cloning facility.
				Copy {\em *this} to the chain {\em chain}.
				The assignment is either deep or shallow (default).
				@param  chain the chain to be assigned to
				@see    chain::set
		*/
		void get(Chain& chain, bool deep = true) const;

		/** Swapping of chain.
				Swap the states of {\em *this} with the chain {\em chain}.
				@param  chain the chain {\em *this} is being swapped with
		*/
		void swap(Chain& chain);

		//@}
	
		/**	@name	Accessors */
		//@{

		/** Get a pointer to the parent protein.
				The reference is 0 if {\em *this} does not have a parent protein.
				@return  Protein* -
								 mutable reference to the parent protein of {\em *this},
		*/
		Protein* getProtein();

		/** Get a constant pointer to the parent protein.
				The reference is 0 if {\em *this} does not have a parent protein.
				@return  Protein* -
								 constant reference to the parent protein of {\em *this},
		*/
		const Protein* getProtein() const;

		/** Get a pointer to a subaltern SecondaryStructure at a given position.
				The reference is 0 if {\em *this} does not have a SecondaryStructure at this position.
				@param   position the position of the subaltern SecondaryStructure
				@return  SecondaryStructure* -
								 mutable reference to the subaltern SecondaryStructure at {\em position} of {\em *this},
		*/
		SecondaryStructure* getSecondaryStructure(Position position);
	
		/** Get a constant pointer to a subaltern SecondaryStructure at a given position.
				The reference is 0 if {\em *this} does not have a SecondaryStructure at this position.
				@param   position the position of the subaltern SecondaryStructure
				@return  SecondaryStructure* -
								 constant reference to the subaltern SecondaryStructure at {\em position} of {\em *this},
		*/
		const SecondaryStructure* getSecondaryStructure(Position position) const;
	
		/** Get a pointer to a subaltern Residue at a given position.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@param   position the position of the subaltern Residue
				@return  Residue* -
								 mutable reference to the subaltern Residue at {\em position} of {\em *this},
		*/
		Residue* getResidue(Position position);
	
		/** Get a pointer to a subaltern Residue at a given position.
				The reference is 0 if {\em *this} does not have a Residue at this position.
				@param   position the position of the subaltern Residue
				@return  Residue* -
								 constant reference to the subaltern Residue at {\em position} of {\em *this},
		*/
		const Residue* getResidue(Position position) const;
	
		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.

				@return  Residue* -
								 mutable reference to the N-terminal Residue
		*/
		Residue* getNTerminal();
	
		/** Get a constant pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.

				@return  Residue* -
								 constant reference to the N-terminal Residue
		*/
		const Residue* getNTerminal() const;

		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.

				@return  Residue* -
								 mutable reference to the C-terminal Residue
		*/
		Residue* getCTerminal();
	
		/** Get a constant pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} does not have a Residue at this position.

				@return  Residue* -
								 constant reference to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a subaltern PDBAtom at a given position.
				The reference is 0 if {\em *this} does not have a PDBAtom at this position.
				@param   position the position of the subaltern PDBAtom
				@return  PDBAtom* -
								 mutable reference to the subaltern PDBAtom at {\em position} of {\em *this},
		*/
		PDBAtom* getPDBAtom(Position position);
	
		/** Get a pointer to a subaltern PDBAtom at a given position.
				The reference is 0 if {\em *this} does not have a PDBAtom at this position.
				@param   position the position of the subaltern PDBAtom
				@return  PDBAtom* -
								 constant reference to the subaltern PDBAtom at {\em position} of {\em *this},
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
				The children of {\tt chain} are inserted using \Ref{spliceBefore}.
		*/
		void splice(Chain &chain);
		
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
		
		/** Internal state dump.
				Dump the current internal state of {\em *this} to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		// --- STORERS

		/* Persistent stream input and state restorage.
				Read persistent chain data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void read(std::istream& s);

		/* Persistent stream output and state storage.
				Write persistent chain data to the output stream {\em s} and store the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void write(std::ostream& s) const;

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

	protected:

	private:

		BaseFragment* getBaseFragment(Position position);
	
		const BaseFragment* getBaseFragment(Position position) const;
	
		Atom* getAtom(Position position);
	
		const Atom* getAtom(Position position) const;

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
