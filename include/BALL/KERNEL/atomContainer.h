// $Id: atomContainer.h,v 1.5 2001/01/21 20:40:27 amoll Exp $

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#define BALL_KERNEL_ATOMCONTAINER_H

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

#ifndef BALL_KERNEL_ATOMCONTAINERITERATOR_H
#	include <BALL/KERNEL/atomContainerIterator.h>
#endif


#define BALL_ATOMCONTAINER_DEFAULT_NAME   ""

namespace BALL 
{

	class Atom;
	class Molecule;

	/**	Atom Container Base Class.
			The {\tt AtomContainer} class is the base class
			of all kernel classes containing atoms.\\
			{\bf Definition:}\URL{BALL/KERNEL/atomContainer.h}
	*/
	class AtomContainer
		: public Composite,
			public PropertyManager
	{
		public:

		BALL_CREATE_DEEP(AtomContainer)

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
		AtomContainer()
			throw();
	
		/** Copy constructor */
		AtomContainer(const AtomContainer& atom_container, bool deep = true)
			throw();
	
		/** Constructor */
		AtomContainer(const String& name)
			throw();

		/** Destructor */
		virtual ~AtomContainer()
			throw();

		/** Clears the contents of this instance of AtomContainer.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::clear}), but does not remove it from its parent composite structures.
		*/
		virtual void clear()
			throw();
	
		/** Clears the contents of this instance of AtomContainer and removes it from parent composite structures.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::destroy}), and removes it from its parent composite structures.
		*/
		virtual void destroy()
			throw();

		//@}

		/**	@name	Persistence */
		//@{

		/**	Persistent writing.
				Writes a AtomContainer object to a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a AtomContainer object from a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentRead(PersistenceManager& pm)
			throw();

		//@}			
	
		/**	@name	Assignment */
		//@{

		/** Assignment with cloning facility.
				Assign {\em atom_container} to {\em *this} instance.
				The assignment is either deep or shallow (default).
				@param  atom_container the atom_container to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em atom_container}
		*/
		void set(const AtomContainer& atom_container, bool deep = true)
			throw();

		/** Assignment operator.
				Assign {\em atom_container} to {\em *this} instance.
				The assignment is deep.
				@param   atom_container the AtomContainer to be copied (cloned)
				@return  AtomContainer& - {\em *this} AtomContainer
				@see     AtomContainer::set
		*/
		const AtomContainer& operator = (const AtomContainer& atom_container)
			throw();

		/** Copying with cloning facility.
				Copy {\em *this} to {\em atom_container}.
				The assignment is either deep or shallow (default).
				@param  atom_container the AtomContainer to be assigned to
				@see    AtomContainer::set
		*/
		void get(AtomContainer& atom_container, bool deep = true) const
			throw();

		/** Swapping of AtomContainers.
				Swap the states of {\em *this} with {\em atom_container}.
				@param  atom_container the AtomContainer {\em *this} is being swapped with
		*/
		void swap(AtomContainer& atom_container)
			throw();

		//@}
	
		/**	Equality operator.
				@see Object::operator ==
		*/
		bool operator == (const AtomContainer& atom_container) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const AtomContainer& atom_container) const
			throw();

		/**	@name	Accessors */
		//@{

		/** Change of the AtomContainer's name.
				Change the name of {\em *this} to {\em name}.
				@param  name the new name
				@see    AtomContainer::getName
		*/
		void setName(const String& name)
			throw();

    /** Mutable inspection of the name.
    Access a mutable reference to the name of {\em *this}.
	  @return  String& - mutable reference to the name
    @see     AtomContainer::setName
		*/
		String& getName()
			throw();

		/** Constant inspection of the name.
				Access a constant reference to the name of {\em *this}.
				@return  String& - constant reference to the name of {\em *this}
				@see     AtomContainer::setName
		*/
		const String& getName() const
			throw();

		/** Mutable inspection of the parent molecule.
				Access a mutable reference to the parent molecule of {\em *this}.
				The reference is 0 if {\em *this} does not have a parent molecule.\\
				\\
				{\bf Note:} No corresponding mutator AtomContainer::setMolecule exists to
				consider design of contract - a AtomContainer may not insert into a molecule,
				it must be inserted via the molecule.
				@return  Molecule* -
								 mutable reference to the parent molecule of {\em *this},
								 0 if {\em *this} AtomContainer does not have a parent molecule
		*/
		Molecule* getMolecule()
			throw();

		/** Mutable inspection of the parent molecule.
				Access a constant reference to the parent molecule of {\em *this}.
				The reference is 0 if {\em *this} AtomContainer does not have a parent molecule.\\
				\\
				{\bf Note:} No corresponding mutator AtomContainer::setMolecule exists to
				consider design of contract - a AtomContainer may not insert into a molecule,
				it must be inserted via the molecule.
				@return  Molecule* -
								 constant reference to the parent molecule of {\em *this},
								 0 if {\em *this} does not have a parent molecule
		*/
		const Molecule* getMolecule() const
			throw();

		/** Get a pointer to the parent AtomContainer.
				The reference is 0 if {\em *this} instance does not have a parent AtomContainer.
				@return  AtomContainer* -
								 mutable reference to the parent AtomContainer of {\em *this},
		*/
		AtomContainer* getSuperAtomContainer()
			throw();

		/** Get a constant pointer to the parent AtomContainer.
				The reference is 0 if {\em *this} instance does not have a parent AtomContainer.
				@return  AtomContainer* -
								 constant reference to the parent AtomContainer of {\em *this},
		*/
		const AtomContainer* getSuperAtomContainer() const
			throw();

		/** Get a pointer to a child AtomContainer at a given position.
				The reference is 0 if {\em *this} instance does not have a AtomContainer at the given position.
				@param   position of the child AtomContainer
				@return  AtomContainer* -
								 mutable reference to the child AtomContainer at {\em positon} of {\em *this},
		*/
		AtomContainer* getAtomContainer(Position position)
			throw();
	
		/** Get a constant pointer to a child AtomContainer at a given position.
				The reference is 0 if {\em *this} instance does not have a AtomContainer at the given position.
				@param   position of the child AtomContainer
				@return  AtomContainer* -
								 constant reference to the child AtomContainer at {\em positon} of {\em *this},
		*/
		const AtomContainer* getAtomContainer(Position position) const
			throw();
	
		/** Get a pointer to a child atom at a given position.
				The reference is 0 if {\em *this} instance does not have an atom at the given position.
				@param   position the position of the child atom
				@return  Atom* -
								 mutable reference to the child atom at {\em positon} of {\em *this},
		*/
		Atom* getAtom(Position position)
			throw();
	
		/** Get a pointer to a child atom at a given position.
				The reference is 0 if {\em *this} instance does not have an atom at the given position.
				@param   position the position of the child atom
				@return  Atom* -
								 constant reference to the child atom at {\em position} of {\em *this},
		*/
		const Atom* getAtom(Position position) const
			throw();
	
		/** Get a pointer to a child atom with the name {\em name}.
				The reference is 0 if {\em *this} instance does not have an atom with this name.
				@param   name the name of the child atom
				@return  Atom* -
								 mutable reference to the child atom with the name {\em name} of {\em *this},
		*/
		Atom* getAtom(const String& name)
			throw();
	
		/** Get a pointer to a child atom with the name {\em name}.
				The reference is 0 if {\em *this} AtomContainer does not have an atom with this name.
				@param   name the name of the child atom
				@return  Atom* -
								 constant reference to the child atom with the name {\em name} of {\em *this},
		*/
		const Atom* getAtom(const String& name) const
			throw();
	
		/** Count the child AtomContainers.
				@return Size the number of AtomContainers
		*/
		Size countAtomContainers() const
			throw();

		/** Count the child atoms.
				@return Size the number of atoms
		*/
		Size countAtoms() const
			throw();

		/** Count the child bonds.
				@return Size the number of bonds
		*/
		Size countBonds() const
			throw();

		/** Count the inter bonds.
				@return Size the number of inter bonds
		*/
		Size countInterBonds() const
			throw();

		/** Count the intra bonds.
				@return Size the number of intra bonds
		*/
		Size countIntraBonds() const
			throw();

		/** Prepend an atom at position 0.
				@param atom, the atom to prepend
		*/
		void prepend(Atom& atom)
			throw();

		/** Append an atom to the last position.
				@param atom, the atom to append
		*/
		void append(Atom& atom)
			throw();

		/** Insert an atom to the last position.
				@param atom, the atom to insert
		*/
		void insert(Atom& atom)
			throw();

		/** Insert an atom before a given {\em Comosite} object.
				@param atom, the atom to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before)
			throw();

		/** Insert an atom after a given {\em Comosite} object.
				@param atom, the atom to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after)
			throw();

		/** Remove an atom
				@param atom the atom to remove
		*/
		bool remove(Atom& atom)
			throw();

		/** Prepend a AtomContainer at position 0.
				@param atom_container, the AtomContainer to prepend
		*/
		void prepend(AtomContainer& atom_container)
			throw();

		/** Append a AtomContainer at the last position.
				@param atom_container, the AtomContainer to prepend
		*/
		void append(AtomContainer& atom_container)
			throw();

		/** Insert a AtomContainer at the last position.
				@param atom_container, the AtomContainer to insert
		*/
		void insert(AtomContainer& atom_container)
			throw();

		/** Insert a AtomContainer before a given {\em Comosite} object.
				@param atom_container, the AtomContainer to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(AtomContainer& atom_container, Composite& before)
			throw();

		/** Insert a AtomContainer after a given {\em Comosite} object.
				@param atom_container, the AtomContainer to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(AtomContainer& atom_container, Composite& after)
			throw();

		/**	Cut all children of {\tt atom_container} and prepend them before the children of {\em *this}.
				@param atom_container the AtomContainer to access
		*/
		void spliceBefore(AtomContainer& atom_container)
			throw();

		/**	Cut all children of {\tt atom_container} and append them after the children of {\em *this}.
				@param atom_container the AtomContainer to access
		*/
		void spliceAfter(AtomContainer& atom_container)
			throw();

		/**	Move the children of {\em atom_container} into {\em *this} instance.
				The children of {\tt atom_container} are inserted at its position
				if it is is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(AtomContainer& atom_container)
			throw();

		/** Remove a AtomContainer
				@param atom_container the AtomContainer to remove
				@return false if {\em atom_container} could not be removed
		*/
		bool remove(AtomContainer& atom_container)
			throw();

		//@}

		/**	@name	Miscellaneous */
		//@{

		/** Destroy all bonds in {\tt this} AtomContainer
		*/
		void destroyBonds()
			throw();

		//@}
	
		/**	@name	Predicates */
		//@{

		/** Test if a AtomContainer is a child of {\tt this}.
				@param atom_container the AtomContainer to test
		*/
		bool isSubAtomContainerOf(const AtomContainer& atom_container) const
			throw();

		/** Test if a AtomContainer is a parent of {\tt this}.
				@param atom_container the AtomContainer to test
		*/
		bool isSuperAtomContainerOf(const AtomContainer& atom_container) const
			throw();
		//@}

		/**	Debugging and Diagnostics */
		//@{

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this}.
				If the internal state of {\em *this} is correct (self-validated) and consistent {\tt true} is returned,
				{\tt false} otherwise. 
				@return			bool -
										{\tt true} if the internal state of {\em *this} is correct (self-validated) and consistent,
										{\tt false} otherwise
		*/
		virtual bool isValid() const
			throw();

		/** Internal state dump.
				Dump the current internal state of {\em *this} to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
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

		// --- INTERNAL ITERATION ---
		/**	@name	Internal Iteration
		*/
		//@{

		///	Apply to all bonds inside this AtomContainer
		bool applyIntraBond(UnaryProcessor<Bond>& processor)
			throw();

		/// Apply to all bonds to atoms outside this AtomContainer
		bool applyInterBond(UnaryProcessor<Bond>& processor)
			throw();
		//@}

		// --- EXTERNAL ITERATORS
		
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)


		private:

		// --- ATTRIBUTES
		String  name_;
	};

} // namespace BALL

#endif // BALL_KERNEL_ATOMCONTAINER_H
