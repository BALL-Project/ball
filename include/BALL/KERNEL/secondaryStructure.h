// $Id: secondaryStructure.h,v 1.5 2000/04/14 00:40:23 amoll Exp $

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#define BALL_KERNEL_SECONDARYSTRUCTURE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#	include <BALL/KERNEL/residueIterator.h>
#endif

namespace BALL 
{
	 
	class Protein;

	/**	Kernel Secondary Structure Class.
			This class is used to represent secondary structure elements
			of protein chains.\\
			{\bf Definition:}\URL{BALL/KERNEL/secondaryStructure.h}
	*/
	class SecondaryStructure
		: public BaseFragment
	{
		public:

		BALL_CREATE(SecondaryStructure)


		/**	@name	Enums
		*/
		//@{

		/**	
		*/
		enum Property
		{
			PROPERTY__HELIX       = Fragment::NUMBER_OF_PROPERTIES + 1,
			PROPERTY__RANDOM_COIL = Fragment::NUMBER_OF_PROPERTIES,
			PROPERTY__STRAND      = Fragment::NUMBER_OF_PROPERTIES,
			PROPERTY__TURN        = Fragment::NUMBER_OF_PROPERTIES,

			NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constructor
		SecondaryStructure();
	
		/// Copy constructor
		SecondaryStructure(const SecondaryStructure& secondary_structure,bool deep = true);
	
		/// Detailled constructor 
		SecondaryStructure(const String& name);

		///	Destructor
		virtual ~SecondaryStructure();

		/// Clears the contents of the secondary structure
		virtual void clear();
	
		/// Clears the contents of the secondary structure and removes it from all composite structures
		virtual void destroy();
	
		//@}

		/** @name Persistence */
		//@{

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Assignment */
		//@{

		/** Assignment with cloning facility.
				Assign the SecondaryStructure {\em secondary_structure} to {\em *this} SecondaryStructure.
				The assignment is either deep or shallow (default).
				@param  secondary_structure the SecondaryStructure to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em secondary_structure}
		*/
		void set(const SecondaryStructure& secondary_structure, bool deep = true);

		/** Assignment operator.
				Assign the SecondaryStructure {\em secondary_structure} to {\em *this} secondary_structure.
				The assignment is either deep or shallow (default).
				@param   secondary_structure the SecondaryStructure to be copied (cloned)
				@return  secondary_structure& - {\em *this} SecondaryStructure
				@see     secondary_structure::set
		*/
		SecondaryStructure &operator =(const SecondaryStructure& secondary_structure);

		/** Copying with cloning facility.
				Copy {\em *this} SecondaryStructure to the SecondaryStructure {\em secondary_structure}.
				The assignment is either deep or shallow (default).
				@param  secondary_structure the SecondaryStructure to be assigned to
				@see    secondary_structure::set
		*/
		void get(SecondaryStructure& secondary_structure, bool deep = true) const;

		/** Swapping of SecondaryStructure.
				Swap the states of {\em *this} SecondaryStructure with the SecondaryStructure {\em secondary_structure}.
				@param  secondary_structure the SecondaryStructure {\em *this} SecondaryStructure is being swapped with
		*/
		void swap(SecondaryStructure& secondary_structure);

		//@}

	
		/**	@name Accessors */	
		//@{

		/** Get a pointer to the superior protein.
				The reference is 0 if {\em *this} SecondaryStructure does not have a parent protein.
				@return  Protein* -
								 mutable reference to the parent protein of {\em *this} SecondaryStructure,
		*/
		Protein* getProtein();

		/** Get a pointer to the superior protein.
				The reference is 0 if {\em *this} SecondaryStructure does not have a parent protein.
				@return  Protein* -
								 constant reference to the parent protein of {\em *this} SecondaryStructure,
		*/
		const Protein* getProtein() const;

		/** Get a pointer to the superior chain.
				The reference is 0 if {\em *this} SecondaryStructure does not have a parent chain.
				@return  Protein* -
								 mutable reference to the parent chain of {\em *this} SecondaryStructure,
		*/
		Chain* getChain();

		/** Get a pointer to the superior chain.
				The reference is 0 if {\em *this} SecondaryStructure does not have a parent chain.
				@return  Protein* -
								 constant reference to the parent chain of {\em *this} SecondaryStructure,
		*/
		const Chain* getChain() const;

		/** Get a pointer to a subaltern Residue at a given position.
				The reference is 0 if {\em *this} residue does not have a Residue at the given position.
				@param   index the position of the subaltern Residue
				@exception IndexUnderflow if {\tt index < 0}
				@return  Residue* -
								 mutable reference to the subaltern Residue at positon {\em index} of {\em *this} residue,
		*/
		Residue* getResidue(Index index);
	
		/** Get a pointer to a subaltern Residue at a given position.
				The reference is 0 if {\em *this} residue does not have a Residue at the given position.
				@param   index the position of the subaltern Residue
				@exception IndexUnderflow if {\tt index < 0}
				@return  Residue* -
								 constant reference to the subaltern Residue at positon {\em index} of {\em *this} residue,
		*/
		const Residue* getResidue(Index index) const;

		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} residue does not have a Residue at the given position.

				@return  Residue* -
								 mutable reference to the N-terminal Residue
		*/
		Residue* getNTerminal();
	
		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} residue does not have a Residue at the given position.

				@return  Residue* -
								 constant reference to the N-terminal Residue
		*/
		const Residue* getNTerminal() const;

		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} residue does not have a Residue at the given position.

				@return  Residue* -
								 mutable reference to the C-terminal Residue
		*/
		Residue* getCTerminal();
	
		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} residue does not have a Residue at the given position.

				@return  Residue* -
								 constant reference to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a subaltern PDBAtom at a given position.
				The reference is 0 if {\em *this} residue does not have a PDBAtom at the given position.
				@param   index the position of the subaltern PDBAtom
				@exception IndexUnderflow if {\tt index < 0}
				@return  PDBAtom* -
								 constant reference to the subaltern PDBAtom at positon {\em index} of {\em *this} residue,
		*/
		PDBAtom* getPDBAtom(Index index);
	
		/** Get a pointer to a subaltern PDBAtom at a given position.
				The reference is 0 if {\em *this} residue does not have a PDBAtom at the given position.
				@param   index the position of the subaltern PDBAtom
				@exception IndexUnderflow if {\tt index < 0}
				@return  PDBAtom* -
								 mutable reference to the subaltern PDBAtom at positon {\em index} of {\em *this} residue,
		*/
		const PDBAtom* getPDBAtom(Index index) const;
	
		/** Count the Residues
				@return  Size the number of residues
		*/
		Size countResidues() const;

		/** Count the PDBAtoms
				@return  Size the number of PDBAtoms
		*/
		Size countPDBAtoms() const;

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

		/** Insert a Residue before an given {\em Comosite} object.
				@param residue, the Residue to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Residue& residue, Composite& before);

		/** Insert a Residue after an given {\em Comosite} object.
				@param residue, the Residue to insert
				@param after, the {\em Comosite} object to insert before
		*/
		void insertAfter(Residue& residue, Composite& after);

		/** Remove a Residue
				@param residue the Residue to remove
		*/
		bool remove(Residue& residue);

		/**	Cut all children of {\tt secondary_structure} and prepend them before the children of this SecondaryStructure.
				@param secondary_structure the SecondaryStructure to access
		*/
		void spliceBefore(SecondaryStructure& secondary_structure);

		/**	Cut all children of {\tt secondary_structure} and append them after the children of this SecondaryStructure.
				@param secondary_structure the SecondaryStructure to access
		*/
		void spliceAfter(SecondaryStructure& secondary_structure);

		/**	Move the children of {\tt secondary_structure} into this SecondaryStructure.
				The children of {\tt secondary_structure} are inserted at the position of 
				{\tt secondary_structure} if {\tt secondary_structure} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(SecondaryStructure& secondary_structure);
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
	
		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this} SecondaryStructure.
				If the internal state of {\em *this} chain is correct (self-validated) and consistent {\tt true} is returned,
				{\tt false} otherwise. 
				@return			bool -
										{\tt true} if the internal state of {\em *this} SecondaryStructure is correct (self-validated) and consistent,
										{\tt false} otherwise
		*/
		virtual bool isValid() const;

		///
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/** @name STORERS */

		/* Persistent stream input and state restorage.
				Read persistent bond data from the input stream {\em s} and restore the state of {\em *this} SecondaryStructure.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this} SecondaryStructure
		*/
		virtual void read(std::istream& s);

		/* Persistent stream output and state storage.
				Write persistent bond data to the output stream {\em s} and store the state of {\em *this} SecondaryStructure.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this} SecondaryStructure
		*/
		virtual void write(std::ostream& s) const;

			
		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

	protected:

	private:

		Fragment* getFragment(Index index);
	
		const Fragment* getFragment(Index index) const;
	
		Atom* getAtom(Index index);
	
		const Atom* getAtom(Index index) const;

		void prepend(Atom &atom);

		void append(Atom &atom);

		void insert(Atom &atom);

		void insertBefore(Atom &atom, Composite &before);

		void insertAfter(Atom &atom, Composite &after);

		bool remove(Atom &atom);

		void prepend(BaseFragment &base_fragment);

		void append(BaseFragment &base_fragment);

		void insert(BaseFragment &base_fragment);
			

		void insertBefore(BaseFragment &base_fragment, Composite &before);

		void insertAfter(BaseFragment &base_fragment, Composite &after);

		void spliceBefore(BaseFragment &base_fragment);

		void spliceAfter(BaseFragment &base_fragment);

		void splice(BaseFragment &base_fragment);

		bool remove(BaseFragment &base_fragment);

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)

		void clear_();
	};

} // namespace BALL

#endif // BALL_KERNEL_SECONDARYSTRUCTURE_H
