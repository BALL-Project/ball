// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#define BALL_KERNEL_ATOMCONTAINER_H

#ifndef BALL_KERNEL_ATOMITERATOR_H
#	include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINERITERATOR_H
#	include <BALL/KERNEL/atomContainerIterator.h>
#endif

#define BALL_ATOMCONTAINER_DEFAULT_NAME   ""

namespace BALL
{
	class Molecule;

	/**	Atom Container Base Class.
			The <tt>AtomContainer</tt> class is the base class
			of all kernel classes containing atoms.
			 \par
			
    	\ingroup KernelContainers 
	*/
	class BALL_EXPORT AtomContainer
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
			///
			NUMBER_OF_PROPERTIES = 0
		};

		//@}
		/**	@name Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		AtomContainer();

		/** Copy constructor 
				@param atom_container the AtomContainer to construct from
				@param deep a flag indicating cloning
		*/
		AtomContainer(const AtomContainer& atom_container, bool deep = true);

		/** Detailed Constructor 
				@param name the name of the AtomContainer
		*/
		AtomContainer(const String& name);

		/** Destructor 
		*/
		virtual ~AtomContainer();

		/** Clear the contents of this instance of AtomContainer.
				This methods clears the base fragment's name, destroys all its children
				(as in  \link Composite::clear Composite::clear \endlink ), but does not remove it from its parent composite structures.
		*/
		virtual void clear();

		/** Clear the contents of this instance of AtomContainer and removes it from parent composite structures.
				This methods clears the base fragment's name, destroys all its children
				(as in  \link Composite::destroy Composite::destroy \endlink ), and removes it from its parent composite structures.
		*/
		virtual void destroy();

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assign the contents of an AtomContainer
				The assignment is either deep or shallow (default is deep).
				@param  atom_container the atom_container to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
				@see Composite::set
		*/
		void set(const AtomContainer& atom_container, bool deep = true);

		/** Assignment operator.
				The assignment is deep.
				@param   atom_container the AtomContainer to be copied (cloned)
				@return  AtomContainer& - this instance AtomContainer
				@see     AtomContainer::set
		*/
		AtomContainer& operator = (const AtomContainer& atom_container);

		/** Copy to another instance of AtomContainer.
				The assignment is either deep or shallow (default is deep).
				@param  atom_container the AtomContainer to be assigned to
				@see    Composite::get
		*/
		void get(AtomContainer& atom_container, bool deep = true) const;

		/** Swap the contents of two AtomContainers.
				@param  atom_container the AtomContainer this instance is being swapped with.
		*/
		void swap(AtomContainer& atom_container);

		//@}
		/** @name Predicates
		*/
		//@{

		/**	Equality operator.
				Two instances of AtomContainer are equal if they have the same handle.
		*/
		bool operator == (const AtomContainer& atom_container) const;

		/**	Inequality operator.
				@see operator ==
		*/
		bool operator != (const AtomContainer& atom_container) const;

		//@}
		/**	@name	Accessors 
		*/
		//@{

		/** Set the name.
				@param  name the new name
		*/
		void setName(const String& name);

		/** Return the name.
				@return  String& - constant reference to the name of this instance
		*/
		const String& getName() const;

		// v2.2 HCP-2c.1 (D-2c.3): the v0 IDENTITY bits (Molecule::IS_SOLVENT,
		// Residue::PROPERTY__AMINO_ACID/__WATER/__NON_STANDARD) feed the stored
		// container role columns (molecule_role / residue_kind). Override the BIT
		// property mutators to refine the mirrored role after the base op
		// (forward-only; a no-op when unbound or being destroyed). The named /
		// string property overloads carry no identity bit, so they are inherited
		// unchanged via the using-declarations -- preserving the full
		// PropertyManager overload surface (Codex HCP-2c-R1 caveat).
		// NB: the parameter type MUST be the global BALL::Property (= BALL_SIZE_TYPE)
		// to match PropertyManager::setProperty -- an unqualified `Property` here
		// would bind to AtomContainer::Property (the local enum above) and define a
		// DIFFERENT, non-overriding overload that the bit-write calls never reach.
		using PropertyManager::setProperty;
		using PropertyManager::clearProperty;
		using PropertyManager::toggleProperty;
		void setProperty(BALL::Property property);
		void clearProperty(BALL::Property property);
		void toggleProperty(BALL::Property property);

		/** Get a mutable pointer to the parent AtomContainer.
				The pointer is 0 if this instance does not have a parent AtomContainer.
				@return  AtomContainer* - mutable pointer to the parent AtomContainer
		*/
		AtomContainer* getSuperAtomContainer();

		/** Get a constant pointer to the parent AtomContainer.
				The pointer is 0 if this instance does not have a parent AtomContainer.
				@return  AtomContainer* - constant pointer to the parent AtomContainer
		*/
		const AtomContainer* getSuperAtomContainer() const;

		/** Get a pointer to a child AtomContainer at a given position.
				The pointer is 0 if this instance does not have an AtomContainer at
				the given position.
				@param   position of the child AtomContainer
				@return  AtomContainer* - mutable pointer to the child
		*/
		AtomContainer* getAtomContainer(Position position);

		/** Get a constant pointer to a child AtomContainer at a given position.
				The pointer is 0 if this instance does not have an AtomContainer at
				the given position.
				@param   position of the child AtomContainer
				@return  AtomContainer* - constant pointer to the child
		*/
		const AtomContainer* getAtomContainer(Position position) const;

		/** Get a pointer to a child atom at a given position.
				The pointer is 0 if this instance does not have an atom at the
				given position.
				@param   position the position of the child atom
				@return  Atom* - mutable pointer to the child
		*/
		Atom* getAtom(Position position);

		/** Get a pointer to a child atom at a given position.
				The pointer is 0 if this instance does not have an atom at the
				given position.
				@param   position the position of the child atom
				@return  Atom* - constant pointer to the child
		*/
		const Atom* getAtom(Position position) const;

		/** Get a pointer to a child atom with the name <b>  name </b>.
				The pointer is 0 if this instance does not have an atom with this
				name.
				@param   name the name of the child atom
				@return  Atom* - mutable pointer to the child
		*/
		Atom* getAtom(const String& name);

		/** Get a pointer to a child atom with the name <b>  name </b>.
				The pointer is 0 if this instance AtomContainer does not have an
				atom with this name.
				@param   name the name of the child atom
				@return  Atom* - constant pointer to the child
		*/
		const Atom* getAtom(const String& name) const;

		/** Count the child AtomContainers.
				@return Size the number of AtomContainers
		*/
		Size countAtomContainers() const;

		/** Count the child atoms.
				@return Size the number of atoms
		*/
		Size countAtoms() const;

		/** Count the child bonds.
				@return Size the number of bonds
		*/
		Size countBonds() const;

		/** Count all bonds which connect atoms in this instance with atoms outside.
				@return Size the number of inter bonds
		*/
		Size countInterBonds() const;

		/** Count all bonds which connect 2 atoms in this instance.
				@return Size the number of intra bonds
		*/
		Size countIntraBonds() const;

		/** Prepend an atom at position 0.
				@param atom the atom to prepend
		*/
		void prepend(Atom& atom);

		/** Append an atom to the last position.
				@param atom the atom to append
		*/
		void append(Atom& atom);

		/** Insert an atom to the last position.
				@param atom the atom to insert
		*/
		void insert(Atom& atom);

		/** Insert an atom before a given Composite object.
				@param atom the atom to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before);

		/** Insert an atom after a given Composite object.
				@param atom the atom to insert
				@param after the Composite object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after);

		/** Remove an atom
				@param atom the atom to remove
		*/
		bool remove(Atom& atom);

		Size removeHavingProperty(BALL::Property p);
		Size removeNotHavingProperty(BALL::Property p);
		Size removeHavingProperty(const string& name);
		Size removeNotHavingProperty(const string& name);

		/** Prepend an AtomContainer at position 0.
				@param atom_container the AtomContainer to prepend
		*/
		void prepend(AtomContainer& atom_container);

		/** Append an AtomContainer at the last position.
				@param atom_container the AtomContainer to prepend
		*/
		void append(AtomContainer& atom_container);

		/** Insert an AtomContainer at the last position.
				@param atom_container the AtomContainer to insert
		*/
		void insert(AtomContainer& atom_container);

		/** Insert an AtomContainer before a given Composite object.
				@param atom_container the AtomContainer to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(AtomContainer& atom_container, Composite& before);

		/** Insert an AtomContainer after a given Composite object.
				@param atom_container the AtomContainer to insert
				@param after the Composite object to insert after
		*/
		void insertAfter(AtomContainer& atom_container, Composite& after);

		/**	Cut all children of <tt>atom_container</tt> and prepend them before
				the children of this instance.
				@param atom_container the AtomContainer to access
		*/
		void spliceBefore(AtomContainer& atom_container);

		/**	Cut all children of <tt>atom_container</tt> and append them after the 
				children of this instance.
				@param atom_container the AtomContainer to access
		*/
		void spliceAfter(AtomContainer& atom_container);

		/**	Move the children of {\em atom_container} into this instance.
				The children of <tt>atom_container</tt> are inserted at its position
				if it is is a child of <tt>this</tt>.
				Otherwise the children are inserted using  \link spliceBefore spliceBefore \endlink .
		*/
		void splice(AtomContainer& atom_container);

		/** Remove an AtomContainer
				@param atom_container the AtomContainer to remove
				@return false if {\em atom_container} could not be removed
		*/
		bool remove(AtomContainer& atom_container);

		//@}
		/**	@name	Miscellaneous 
		*/
		//@{

		/** Destroy all bonds in this instance.
		*/
		void destroyBonds();

		//@}
		/**	@name	Predicates 
		*/
		//@{

		/** Test if an AtomContainer is a child.
				@param atom_container the AtomContainer to test
		*/
		bool isSubAtomContainerOf(const AtomContainer& atom_container) const;

		/** Test if an AtomContainer is the parent.
				@param atom_container the AtomContainer to test
		*/
		bool isSuperAtomContainerOf(const AtomContainer& atom_container) const;

		//@}
		/**	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state and consistency self-validation.
				If the internal state of this instance is correct (self-validated) and 
				consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
				@return	 bool
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of this instance to the 
				output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state of this instance
				@param   depth - the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
		/**	@name	Internal Iteration
		*/
		//@{

		///	Apply to all bonds connecting two atoms inside this AtomContainer
		bool applyIntraBond(UnaryProcessor<Bond>& processor);

		/// Apply to all bonds connected to atoms outside this AtomContainer
		bool applyInterBond(UnaryProcessor<Bond>& processor);

		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)

		BALL_DECLARE_STD_ITERATOR_WRAPPER(AtomContainer, Atom, atoms)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(AtomContainer, AtomContainer, atomContainers)

		// v2.2 H2a (D68/D74): override the Composite container-row binding
		// hooks. Stores {owning store, ContainerTable row index} of this
		// container's mirror row so the composite.C mirror can address it
		// in O(1) without naming AtomContainer (layering). 0/nullptr =
		// unbound; the mirror self-heals (re-materialises) when the bound
		// store != the current store (D74).
		MoleculeStore* getContainerRowStore_() const override { return container_row_store_; }
		std::uint32_t  getContainerRow_() const override      { return container_row_idx_; }
		void setContainerRowBinding_(MoleculeStore* store, std::uint32_t row) override
		{
			// v2.2 H4 commit 6.a (D-H4.6 R2) + Codex H4-DR R8 F1 fix
			// (D-H4.16 BRIDGE-LIFECYCLE): populate the container-row → v0
			// Composite bridge so ContainerHandleBase's eachProperty /
			// propertyNames can reach this AtomContainer's PropertyManager
			// bag through the dual-existence window.
			//
			// CRITICAL: on rebind / unbind, the OLD store's bridge slot for
			// the OLD row must be cleared FIRST -- otherwise it dangles and
			// a row-recycle on the old store hands a fresh handle the stale
			// pointer to this destroyed/moved AtomContainer.
			//
			// Preconditions for "clear old":
			//   (a) we had a non-null old store binding, AND
			//   (b) the old slot currently still points to *this* (defensive:
			//       another binding could have overwritten the slot via a
			//       move; never clobber that).
			MoleculeStore* const old_store = container_row_store_;
			const std::uint32_t  old_row   = container_row_idx_;
			const bool rebind_or_unbind = (old_store != 0)
			                              && (old_store != store || old_row != row);
			if (rebind_or_unbind && old_store->container_back_ptr(old_row) == this)
			{
				old_store->set_container_back_ptr(old_row, nullptr);
			}
			container_row_store_ = store;
			container_row_idx_   = row;
			if (store != 0) store->set_container_back_ptr(row, this);
		}

		// v2.2 HCP-1b: re-push this container's scalar identity (name + the
		// subtype id/insertion-code/SS-type/ResidueKind) to its row. Defined
		// in atomContainer.C; dispatches via detail::writeContainerScalars_.
		void mirrorResyncScalars_() override;

		// v2.2 HCP-2c.2 (D-2c.5 refined): materialise-the-new-member -- route to
		// detail::adoptSubtreeInto_ (atom migration + container materialisation).
		// No-op when being destroyed or dst is null. Defined in atomContainer.C.
		void mirrorAdoptSubtreeInto_(MoleculeStore* dst) override;

		private:

		/*_ The name of this container
		*/
		String  name_;

		// v2.2 H2a (D74): container-table row binding. Transitional
		// (removed at H4). 12 B on each container (O(thousands); memory
		// irrelevant). Atoms do NOT carry this (they are leaves).
		MoleculeStore* container_row_store_ = 0;
		std::uint32_t  container_row_idx_   = 0;

	};

	namespace detail
	{
		// v2.2 HCP-1b: write a container's SCALAR identity fields (name + the
		// per-subtype id/insertion-code/SS-type/ResidueKind) to its
		// ContainerTable row. The single source of the materialise-time
		// scalar dispatch -- shared by System::materialiseContainer_ (initial
		// population) and AtomContainer::mirrorResyncScalars_ (re-sync after
		// clear/swap/setters). Defined in atomContainer.C (which has the
		// subtype headers). No-op if `row` is 0.
		BALL_EXPORT void writeContainerScalars_(const AtomContainer& c,
		                                        MoleculeStore& store,
		                                        std::uint32_t row);

		// v2.2 HCP-2c.2 (D-2c.5 refined, Codex HCP-2c2-RA): the store-level
		// adoption body extracted from System::adoptSubtree -- atom migration
		// (snapshot/allocate/migrate + bond migration + src release) followed by
		// container-row materialisation -- operating purely on the destination
		// store `dst`. Shared by System::adoptSubtree AND
		// AtomContainer::mirrorAdoptSubtreeInto_ (materialise-the-new-member),
		// so insertParent (no new atoms) and replace-with-orphan (atoms migrated)
		// take ONE correct path. Defined in system.C (has the migration machinery).
		BALL_EXPORT void adoptSubtreeInto_(AtomContainer& root, MoleculeStore* dst);
	} // namespace detail

} // namespace BALL

#endif // BALL_KERNEL_ATOMCONTAINER_H
