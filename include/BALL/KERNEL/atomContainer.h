// $Id: atomContainer.h,v 1.12 2001/07/04 09:43:25 amoll Exp $

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
			of all kernel classes containing atoms.
			\\
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

		/// The number of predefined properties for AtomContainer
		enum Property
		{	
			/// BAUSTELLE
			NUMBER_OF_PROPERTIES = 0
		};

		//@}


		/**	@name Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		AtomContainer()
			throw();
	
		/** Copy constructor 
				@param atom_container the AtomContainer to construct from
				@param deep a flag indicating cloning
		*/
		AtomContainer(const AtomContainer& atom_container, bool deep = true)
			throw();
	
		/** Detailed Constructor 
				@param name the name of the AtomContainer
		*/
		AtomContainer(const String& name)
			throw();

		/** Destructor 
		*/
		virtual ~AtomContainer()
			throw();

		/** Clears the contents of this instance of AtomContainer and removes
				it from parent composite structures.  This methods clears the base
				fragment's name, destroys all its children (as in
				\Ref{Composite::destroy}), and removes it from its parent composite
				structures.
		*/
		virtual void destroy()
			throw();

		//@}

		/**	@name	Persistence 
		*/
		//@{

		/**	Write an AtomContainer object to a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Read an AtomContainer object from a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentRead(PersistenceManager& pm)
			throw();
			
		//@}			
		/**	@name	Assignment 
		*/
		//@{

		/** Assign the contents of an AtomContainer
				The assignment is either deep or shallow (default is deep).
				@param  atom_container the atom_container to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy
				@see Composite::set
		*/
		void set(const AtomContainer& atom_container, bool deep = true)
			throw();

		/** Assignment operator.
				The assignment is deep.
				@param   atom_container the AtomContainer to be copied (cloned)
				@return  AtomContainer& - this instance AtomContainer
				@see     AtomContainer::set
		*/
		const AtomContainer& operator = (const AtomContainer& atom_container)
			throw();

		/** Copy to another instance of AtomContainer.
				The assignment is either deep or shallow (default is deep).
				@param  atom_container the AtomContainer to be assigned to
				@see    Composite::get
		*/
		void get(AtomContainer& atom_container, bool deep = true) const
			throw();

		/** Swap the contents of two AtomContainers.
				@param  atom_container the AtomContainer this instance is being swapped with.
		*/
		void swap(AtomContainer& atom_container)
			throw();

		/** Clears the contents of this instance of AtomContainer.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::clear}), but does not remove it from its
				parent composite structures.
		*/
		virtual void clear()
			throw();
	
		//@}
		/** @name Predicates
		*/
		//@{

		/**	Equality operator.
				Two instances of AtomContainer are equal if they have the same handle.
		*/
		bool operator == (const AtomContainer& atom_container) const
			throw();

		/**	Inequality operator.
				@see operator ==
		*/
		bool operator != (const AtomContainer& atom_container) const
			throw();

		//@}
		/**	@name	Accessors 
		*/
		//@{

		/** Set the name.
				@param  name the new name
		*/
		void setName(const String& name)
			throw();

		/** Return the name.
				@return  String& - constant reference to the name of this instance
		*/
		const String& getName() const
			throw();

		/** Get a mutable pointer to the parent AtomContainer.
				The pointer is 0 if this instance does not have a parent AtomContainer.
				@return  AtomContainer* - mutable pointer to the parent AtomContainer
		*/
		AtomContainer* getSuperAtomContainer()
			throw();

		/** Get a constant pointer to the parent AtomContainer.
				The pointer is 0 if this instance does not have a parent AtomContainer.
				@return  AtomContainer* - constant pointer to the parent AtomContainer
		*/
		const AtomContainer* getSuperAtomContainer() const
			throw();

		/** Get a pointer to a child AtomContainer at a given position.
				The pointer is 0 if this instance does not have an AtomContainer at
				the given position.
				@param   position of the child AtomContainer
				@return  AtomContainer* - mutable pointer to the child
		*/
		AtomContainer* getAtomContainer(Position position)
			throw();
	
		/** Get a constant pointer to a child AtomContainer at a given position.
				The pointer is 0 if this instance does not have an AtomContainer at
				the given position.
				@param   position of the child AtomContainer
				@return  AtomContainer* - constant pointer to the child
		*/
		const AtomContainer* getAtomContainer(Position position) const
			throw();
	
		/** Get a pointer to a child atom at a given position.
				The pointer is 0 if this instance does not have an atom at the
				given position.
				@param   position the position of the child atom
				@return  Atom* - mutable pointer to the child
		*/
		Atom* getAtom(Position position)
			throw();
	
		/** Get a pointer to a child atom at a given position.
				The pointer is 0 if this instance does not have an atom at the
				given position.
				@param   position the position of the child atom
				@return  Atom* - constant pointer to the child
		*/
		const Atom* getAtom(Position position) const
			throw();
	
		/** Get a pointer to a child atom with the name {\em name}.
				The pointer is 0 if this instance does not have an atom with this
				name.
				@param   name the name of the child atom
				@return  Atom* - mutable pointer to the child
		*/
		Atom* getAtom(const String& name)
			throw();
	
		/** Get a pointer to a child atom with the name {\em name}.
				The pointer is 0 if this instance AtomContainer does not have an
				atom with this name.
				@param   name the name of the child atom
				@return  Atom* - constant pointer to the child
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

		/** Count all bonds which connect 2 atoms in this instance.
				@return Size the number of inter bonds
		*/
		Size countInterBonds() const
			throw();

		/** Count all bonds which connect atoms in this instance with atoms
				outside.
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

		/** Insert an atom before a given Composite object.
				@param atom, the atom to insert
				@param before, the Composite object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before)
			throw();

		/** Insert an atom after a given Composite object.
				@param atom, the atom to insert
				@param after, the Composite object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after)
			throw();

		/** Remove an atom
				@param atom the atom to remove
		*/
		bool remove(Atom& atom)
			throw();

		/** Prepend an AtomContainer at position 0.
				@param atom_container, the AtomContainer to prepend
		*/
		void prepend(AtomContainer& atom_container)
			throw();

		/** Append an AtomContainer at the last position.
				@param atom_container, the AtomContainer to prepend
		*/
		void append(AtomContainer& atom_container)
			throw();

		/** Insert an AtomContainer at the last position.
				@param atom_container, the AtomContainer to insert
		*/
		void insert(AtomContainer& atom_container)
			throw();

		/** Insert an AtomContainer before a given Composite object.
				@param atom_container, the AtomContainer to insert
				@param before, the Composite object to insert before
		*/
		void insertBefore(AtomContainer& atom_container, Composite& before)
			throw();

		/** Insert an AtomContainer after a given Composite object.
				@param atom_container, the AtomContainer to insert
				@param after, the Composite object to insert after
		*/
		void insertAfter(AtomContainer& atom_container, Composite& after)
			throw();

		/**	Cut all children of {\tt atom_container} and prepend them before
				the children of this instance.
				@param atom_container the AtomContainer to access
		*/
		void spliceBefore(AtomContainer& atom_container)
			throw();

		/**	Cut all children of {\tt atom_container} and append them after the 
				children of this instance.
				@param atom_container the AtomContainer to access
		*/
		void spliceAfter(AtomContainer& atom_container)
			throw();

		/**	Move the children of {\em atom_container} into this instance.
				The children of {\tt atom_container} are inserted at its position
				if it is is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(AtomContainer& atom_container)
			throw();

		/** Remove an AtomContainer
				@param atom_container the AtomContainer to remove
				@return false if {\em atom_container} could not be removed
		*/
		bool remove(AtomContainer& atom_container)
			throw();

		//@}
		/**	@name	Miscellaneous 
		*/
		//@{

		/** Destroy all bonds in this instance.
		*/
		void destroyBonds()
			throw();

		//@}
		/**	@name	Predicates 
		*/
		//@{

		/** Test if an AtomContainer is a child.
				@param atom_container the AtomContainer to test
		*/
		bool isSubAtomContainerOf(const AtomContainer& atom_container) const
			throw();

		/** Test if an AtomContainer is the parent.
				@param atom_container the AtomContainer to test
		*/
		bool isSuperAtomContainerOf(const AtomContainer& atom_container) const
			throw();

		//@}
		/**	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state and consistency self-validation.
				If the internal state of this instance is correct (self-validated) and 
				consistent {\tt true} is returned, {\tt false} otherwise. 
				@return	 bool
		*/
		virtual bool isValid() const
			throw();

		/** Internal state dump.
				Dump the current internal state of this instance to the 
				output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of this instance
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}
		/**	@name	Internal Iteration
		*/
		//@{

		///	Apply to all bonds connecting two atoms inside this AtomContainer
		bool applyIntraBond(UnaryProcessor<Bond>& processor)
			throw();

		/// Apply to all bonds connected to atoms outside this AtomContainer
		bool applyInterBond(UnaryProcessor<Bond>& processor)
			throw();

		//@}

		// --- EXTERNAL ITERATORS
		
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)


		private:

		/*_ The name of this container
		*/
		String  name_;

	};

} // namespace BALL

#endif // BALL_KERNEL_ATOMCONTAINER_H
