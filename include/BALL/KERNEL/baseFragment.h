// $Id: baseFragment.h,v 1.16 2000/04/26 13:05:41 amoll Exp $

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#define BALL_KERNEL_BASEFRAGMENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMITERATOR_H
#	include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENTITERATOR_H
#	include <BALL/KERNEL/baseFragmentIterator.h>
#endif


#define BALL_BASEFRAGMENT_DEFAULT_NAME   ""

namespace BALL 
{

	class Atom;
	class Molecule;

	/**	Atom Container Base Class.
			The {\tt BaseFragment} class is the base class
			of all kernel classes containing atoms.\\
			{\bf Definition:}\URL{BALL/KERNEL/baseFragment.h}
	*/
	class BaseFragment
		: public Composite,
			public PropertyManager
	{
		public:

		BALL_CREATE(BaseFragment)

		/**	@name	Enums 
		*/
		//@{

		///
		enum Property
		{	
			///
			NUMBER_OF_PROPERTIES = 0
		};
		//@}

		/**	@name Constructors and Destructors */
		//@{

		/** Default constructor */
		BaseFragment();
	
		/** Copy constructor */
		BaseFragment(const BaseFragment& base_fragment, bool deep = true);
	
		/** Constructor */
		BaseFragment(const String& name);

		/** Destructor */
		virtual ~BaseFragment();

		/** Clears the contents of the BaseFragment.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::clear}), but does not remove it from its parent composite structures.
		*/
		virtual void clear();
	
		/** Clears the contents of the BaseFragment and removes it from its parent composite structures.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::destroy}), and removes it from its parent composite structures.
		*/
		virtual void destroy();

		//@}

		/**	@name	Persistence */
		//@{

		/// Writes a persistent copy of the object
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Reads a persistent object
		virtual void persistentRead(PersistenceManager& pm);

		//@}			
	
		/**	@name	Assignment */
		//@{

		/** Assignment with cloning facility.
				Assign the BaseFragment {\em base_fragment} to {\em *this} BaseFragment.
				The assignment is either deep or shallow (default).
				@param  base_fragment the base_fragment to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em base_fragment}
		*/
		void set(const BaseFragment& base_fragment, bool deep = true);

		/** Assignment operator.
				Assign the BaseFragment {\em base_fragment} to {\em *this} base_fragment.
				The assignment is either deep or shallow (default).
				@param   base_fragment the BaseFragment to be copied (cloned)
				@return  BaseFragment& - {\em *this} BaseFragment
				@see     BaseFragment::set
		*/
		BaseFragment& operator = (const BaseFragment& base_fragment);

		/** Copying with cloning facility.
				Copy {\em *this} BaseFragment to the BaseFragment {\em base_fragment}.
				The assignment is either deep or shallow (default).
				@param  base_fragment the BaseFragment to be assigned to
				@see    BaseFragment::set
		*/
		void get(BaseFragment& base_fragment, bool deep = true) const;

		/** Swapping of BaseFragment.
				Swap the states of {\em *this} BaseFragment with the BaseFragment {\em base_fragment}.
				@param  base_fragment the BaseFragment {\em *this} BaseFragment is being swapped with
		*/
		void swap(BaseFragment& base_fragment);

		//@}
	
		/**	@name	Accessors */
		//@{

		/** Change of the BaseFragment's name.
				Change the name of {\em *this} BaseFragment to {\em name}.
				@param  name the new name of {\em *this} BaseFragment
				@see    BaseFragment::getName
		*/
		void setName(const String& name);

    /** Mutable inspection of the BaseFragment's name.
    Access a mutable reference to the name of {\em *this} BaseFragment.
	  @return  String& - mutable reference to the name of {\em *this} BaseFragment
    @see     BaseFragment::setName
		*/
		String& getName();

		/** Constant inspection of the BaseFragment's name.
				Access a constant reference to the name of {\em *this} BaseFragment.
				@return  String& - constant reference to the name of {\em *this} BaseFragment
				@see     BaseFragment::setName
		*/
		const String& getName() const;

		/** Mutable inspection of the BaseFragment's parent molecule.
				Access a mutable reference to the parent molecule of {\em *this} BaseFragment.
				The reference is 0 if {\em *this} BaseFragment does not have a parent molecule.\\
				\\
				{\bf Note:} No corresponding mutator BaseFragment::setMolecule exists to
				consider design of contract - a BaseFragment may not insert into a molecule,
				it must be inserted via the molecule.
				@return  Molecule* -
								 mutable reference to the parent molecule of {\em *this} BaseFragment,
								 0 if {\em *this} BaseFragment does not have a parent molecule
		*/
		Molecule* getMolecule();

		/** Mutable inspection of the BaseFragment's parent molecule.
				Access a constant reference to the parent molecule of {\em *this} BaseFragment.
				The reference is 0 if {\em *this} BaseFragment does not have a parent molecule.\\
				\\
				{\bf Note:} No corresponding mutator BaseFragment::setMolecule exists to
				consider design of contract - a BaseFragment may not insert into a molecule,
				it must be inserted via the molecule.
				@return  Molecule* -
								 constant reference to the parent molecule of {\em *this} BaseFragment,
								 0 if {\em *this} atom does not have a parent molecule
		*/
		const Molecule* getMolecule() const;

		/** Get a pointer to the superior BaseFragment.
				The reference is 0 if {\em *this} BaseFragment does not have a parent BaseFragment.
				@return  BaseFragment* -
								 mutable reference to the parent BaseFragment of {\em *this} BaseFragment,
		*/
		BaseFragment* getSuperBaseFragment();

		/** Get a constant pointer to the superior BaseFragment.
				The reference is 0 if {\em *this} BaseFragment does not have a parent BaseFragment.
				@return  BaseFragment* -
								 constant reference to the parent BaseFragment of {\em *this} BaseFragment,
		*/
		const BaseFragment* getSuperBaseFragment() const;

		/** Get a pointer to a subaltern BaseFragment at a given position.
				The reference is 0 if {\em *this} BaseFragment does not have a BaseFragment at the given position.
				@param   index the position of the subaltern BaseFragment
				@exception IndexUnderflow if {\tt index < 0}
				@return  BaseFragment* -
								 mutable reference to the subaltern BaseFragment at positon {\em index} of {\em *this} BaseFragment,
		*/
		BaseFragment* getBaseFragment(Index index);
	
		/** Get a constant pointer to a subaltern BaseFragment at a given position.
				The reference is 0 if {\em *this} BaseFragment does not have a BaseFragment at the given position.
				@param   index the position of the subaltern BaseFragment
				@exception IndexUnderflow if {\tt index < 0}
				@return  BaseFragment* -
								 constant reference to the subaltern BaseFragment at positon {\em index} of {\em *this} BaseFragment,
		*/
		const BaseFragment* getBaseFragment(Index index) const;
	
		/** Get a pointer to a subaltern atom at a given position.
				The reference is 0 if {\em *this} BaseFragment does not have an atom at the given position.
				@param   index the position of the subaltern atom
				@exception IndexUnderflow if {\tt index < 0}
				@return  Atom* -
								 mutable reference to the subaltern atom at positon {\em index} of {\em *this} BaseFragment,
		*/
		Atom* getAtom(Index index);
	
		/** Get a pointer to a subaltern atom at a given position.
				The reference is 0 if {\em *this} BaseFragment does not have an atom at the given position.
				@param   index the position of the subaltern atom
				@exception IndexUnderflow if {\tt index < 0}
				@return  Atom* -
								 constant reference to the subaltern atom at positon {\em index} of {\em *this} BaseFragment,
		*/
		const Atom* getAtom(Index index) const;
	
		/** Get a pointer to a subaltern atom with the name {\em name}.
				The reference is 0 if {\em *this} BaseFragment does not have an atom with this name.
				@param   name the name of the subaltern atom
				@return  Atom* -
								 mutable reference to the subaltern atom with the name {\em name} of {\em *this} BaseFragment,
		*/
		Atom* getAtom(const String& name);
	
		/** Get a pointer to a subaltern atom with the name {\em name}.
				The reference is 0 if {\em *this} BaseFragment does not have an atom with this name.
				@param   name the name of the subaltern atom
				@return  Atom* -
								 constant reference to the subaltern atom with the name {\em name} of {\em *this} BaseFragment,
		*/
		const Atom* getAtom(const String& name) const;
	
		/** Count the subaltern BaseFragments.
				@return Size the number of BaseFragments
		*/
		Size countBaseFragments() const;

		/** Count the subaltern atoms.
				@return Size the number of atoms
		*/
		Size countAtoms() const;

		/** Count the subaltern bonds.
				@return Size the number of bonds
		*/
		Size countBonds() const;

		/** Count the inter bonds.
				@return Size the number of inter bonds
		*/
		Size countInterBonds() const;

		/** Count the intra bonds.
				@return Size the number of intra bonds
		*/
		Size countIntraBonds() const;

		/** Prepend an atom at position 0.
				@param atom, the atom to prepend
		*/
		void prepend(Atom& atom);

		/** Append an atom to the last position.
				@param atom, the atom to append
		*/
		void append(Atom& atom);

		/** Insert an atom to the last position.
				@param atom, the atom to insert
		*/
		void insert(Atom& atom);

		/** Insert an atom before a given {\em Comosite} object.
				@param atom, the atom to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before);

		/** Insert an atom after a given {\em Comosite} object.
				@param atom, the atom to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after);

		/** Remove an atom
				@param atom the atom to remove
		*/
		bool remove(Atom& atom);

		/** Prepend a BaseFragment at position 0.
				@param base_fragment, the BaseFragment to prepend
		*/
		void prepend(BaseFragment& base_fragment);

		/** Append a BaseFragment at the last position.
				@param base_fragment, the BaseFragment to prepend
		*/
		void append(BaseFragment& base_fragment);

		/** Insert a BaseFragment at the last position.
				@param base_fragment, the BaseFragment to insert
		*/
		void insert(BaseFragment& base_fragment);

		/** Insert a BaseFragment before a given {\em Comosite} object.
				@param base_fragment, the BaseFragment to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(BaseFragment& base_fragment, Composite& before);

		/** Insert a BaseFragment after a given {\em Comosite} object.
				@param base_fragment, the BaseFragment to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(BaseFragment& base_fragment, Composite& after);

		/**	Cut all children of {\tt base_fragment} and prepend them before the children of this BaseFragment.
				@param base_fragment the BaseFragment to access
		*/
		void spliceBefore(BaseFragment& base_fragment);

		/**	Cut all children of {\tt base_fragment} and append them after the children of this BaseFragment.
				@param base_fragment the BaseFragment to access
		*/
		void spliceAfter(BaseFragment& base_fragment);

		/**	Move the children of {\em base_fragment} into this BaseFragment.
				The children of {\tt base_fragment} are inserted at the position of 
				{\tt base_fragment} if {\tt base_fragment} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(BaseFragment& base_fragment);

		/** Remove a BaseFragment
				@param base_fragment the BaseFragment to remove
				@return false if base_fragment could not be removed
		*/
		bool remove(BaseFragment& base_fragment);

		//@}

		/**	@name	Miscellaneous */
		//@{

		/** Destroy all bonds in {\tt this} BaseFragment
		*/
		void destroyBonds();

		//@}
	
		/**	@name	Predicates */
		//@{

		/** Test if a BaseFragment is a SubBaseFragment of {\tt this} BaseFragment
				@param base_fragment the BaseFragment to test
		*/
		bool isSubBaseFragmentOf(const BaseFragment& base_fragment) const;

		/** Test if a BaseFragment is a SuperBaseFragment of {\tt this} BaseFragment
				@param base_fragment the BaseFragment to test
		*/
		bool isSuperBaseFragmentOf(const BaseFragment& base_fragment) const;
		//@}

		/**	Debugging and Diagnostics */
		//@{

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this} BaseFragment.
				If the internal state of {\em *this} BaseFragment is correct (self-validated) and consistent {\tt true} is returned,
				{\tt false} otherwise. 
				@return			bool -
										{\tt true} if the internal state of {\em *this} BaseFragment is correct (self-validated) and consistent,
										{\tt false} otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} BaseFragment to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param   s - output stream where to output the internal state of {\em *this} BaseFragment
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/**	@name	Storers */
		//@{

		/* Persistent stream input and state restorage.
				Read persistent bond data from the input stream {\em s} and restore the state of {\em *this} BaseFragment.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this} BaseFragment
		*/
		virtual void read(std::istream& s);

		/* Persistent stream output and state storage.
				Write persistent bond data to the output stream {\em s} and store the state of {\em *this} BaseFragment.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this} BaseFragment
		*/
		virtual void write(std::ostream& s) const;

		//@}

		// --- INTERNAL ITERATION ---
		/**	@name	Internal Iteration
		*/
		//@{

		///	Apply to all bonds inside this BaseFragment
		bool applyIntraBond(UnaryProcessor<Bond>& processor);

		/// Apply to all bonds to atoms outside this BaseFragment
		bool applyInterBond(UnaryProcessor<Bond>& processor);
		//@}

		// --- EXTERNAL ITERATORS
		
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)


		private:

		void clear_();

		// --- ATTRIBUTES

		String  name_;
	};

} // namespace BALL

#endif // BALL_KERNEL_BASEFRAGMENT_H
