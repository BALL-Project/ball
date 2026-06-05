// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 H1b: container value-handle TYPES (D63/D64/D65/D66).
//
// The molecular-hierarchy containers (Molecule/Chain/Residue/Protein/
// SecondaryStructure/Nucleotide/NucleicAcid/Fragment) become value
// handles {MoleculeStore*, container_idx, generation} reading the H1a
// container metadata table via MoleculeStore's scalar accessors. Through
// dual existence (H1b-H3) they coexist with the identically-named v0 C++
// classes, so they carry a transitional `Handle` suffix (D63); at H4 the
// v0 classes are deleted and the canonical names are restored (a per-kind
// migration audit, NOT a one-line alias -- see V22-H1b-DESIGN.md D63).
//
// ENCAPSULATION (D66a): this PUBLIC header includes ONLY moleculeStore.h
// + containerKind.h and never <BALL/KERNEL/_moleculeStoreInternal.h>. All
// reads forward to MoleculeStore::container_*_() scalar accessors that
// return public types only. The private ContainerRow/ChildRef/
// ContainerTable types never appear here (D31b boundary).
//
// READ-ONLY (D66): H1b handles expose scalar getters + parent/child
// navigation + validity. Handle-yielding iterators / apply / mutators are
// H2. Through dual existence the v0 object tree is the source of truth
// (D60); these handles read the verified mirror.
//

#ifndef BALL_KERNEL_CONTAINERHANDLE_H
#define BALL_KERNEL_CONTAINERHANDLE_H

#ifndef BALL_KERNEL_MOLECULESTORE_H
# include <BALL/KERNEL/moleculeStore.h>
#endif

#ifndef BALL_KERNEL_CONTAINERKIND_H
# include <BALL/KERNEL/containerKind.h>
#endif

#ifndef BALL_KERNEL_CONTAINERROLE_H
# include <BALL/KERNEL/containerRole.h>   // HCP-2a: MoleculeRole/FragmentRole/ResidueKind/SSKind
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#include <cstdint>
#include <vector>

// v2.2 H4 commit 6.a (D-H4.6 R2): for eachProperty + propertyNames inline
// implementations that bridge to the v0 PropertyManager bag.
#include <BALL/DATATYPE/string.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/KERNEL/atomContainer.h>      // for the container_back_ptr bridge

// D54/D65: handle-validity checks compile in BALL_DEBUG builds AND the
// Python wrapper layer; zero cost in release C++ (documented UB on misuse,
// same contract class as the existing MoleculeStore reference getters).
#if defined(BALL_DEBUG) || defined(BALL_PYTHON_WRAPPER)
# define BALL_CONTAINER_HANDLE_CHECKS 1
#endif

namespace BALL
{
	/** Kind-agnostic base for the v2.2 container value handles (D64).

			Holds the 24 B value triple `{MoleculeStore*, u32 idx, u64
			generation}` and the kind-agnostic read-only getters. NON-virtual /
			non-polymorphic (no vtable): the typed handles inherit it purely
			for getter reuse and add NO data members, so each stays 24 B
			(pinned by static_assert). A default-constructed base is the NULL
			handle (`store_ == nullptr`, `operator bool` false).

			Container handles are O(thousands) so 24 B is fine; the 16 B
			handle target applies only to Atom/Bond (the O(100k+) D13 case).

			\ingroup KernelContainers
	*/
	class BALL_EXPORT ContainerHandleBase
	{
		public:

		/// Null handle.
		ContainerHandleBase() = default;

		/** Bind to container row `idx` in `store`, capturing the row's
				current per-slot generation (D65). Kind-agnostic — any container
				row is valid here; the typed subclasses add a kind assert.
		*/
		ContainerHandleBase(MoleculeStore& store, std::uint32_t idx)
			: store_(&store), idx_(idx),
			  generation_(store.container_generation_(idx))
		{}

		/// True if this is the null handle (store == nullptr).
		bool isNull() const { return store_ == nullptr; }

		/// Non-null test (the D61 null-handle idiom: `if (h) ...`).
		explicit operator bool() const { return store_ != nullptr; }

		/** Full validity (D65): non-null, in-range, not freed, and the row's
				per-slot generation still matches the one captured at binding.
				A recycled/migrated slot bumps its generation, so a stale alias
				returns false here. Always compiled (it is a query).
		*/
		bool isValid() const
		{
			return store_ != nullptr
				&& idx_ != 0
				&& idx_ < store_->container_table_size_()
				&& !store_->container_is_freed_(idx_)
				&& store_->container_generation_(idx_) == generation_;
		}

		ContainerKind getKind() const
		{ assertValid_(); return store_->container_kind_(idx_); }

		String getName() const
		{ assertValid_(); return String(store_->container_name_(idx_)); }

		/// Immediate-parent row index, or MoleculeStore::CONTAINER_NONE.
		std::uint32_t getParentIndex() const
		{ assertValid_(); return store_->container_parent_(idx_); }

		/// Immediate parent as a base handle; null handle at the root.
		ContainerHandleBase getParent() const
		{
			assertValid_();
			std::uint32_t p = store_->container_parent_(idx_);
			if (p == MoleculeStore::CONTAINER_NONE) return ContainerHandleBase();
			return ContainerHandleBase(*store_, p);
		}

		/// Number of ordered children (atoms + child containers).
		std::size_t countChildren() const
		{ assertValid_(); return store_->container_child_count_(idx_); }

		/// The i-th ordered child as a public {is_atom, idx} reference.
		ContainerChildRef getChild(std::size_t i) const
		{ assertValid_(); return store_->container_child_(idx_, i); }

		/** The i-th child as a (base) container handle; null handle if that
				child is an atom (use getChild(i).idx as the atom store index).
		*/
		ContainerHandleBase getChildContainer(std::size_t i) const
		{
			assertValid_();
			ContainerChildRef c = store_->container_child_(idx_, i);
			// Null handle if the child is an atom OR the index is out of
			// range (container_child_ returns {is_atom=false, idx=0} for an
			// out-of-range i; idx 0 is the sentinel, never a real container).
			if (c.is_atom || c.idx == 0) return ContainerHandleBase();
			return ContainerHandleBase(*store_, c.idx);
		}

		/// Number of selected atoms in this container's subtree. Computed on
		/// read by walking the subtree edges + reading the v0 atom selection
		/// (back_ptr->isSelected(), the Selectable::selected_ truth) -- HCP-1R.
		/// Correct for every mutation path (select/deselect AND topology moves)
		/// regardless of how the selection was set. O(subtree).
		std::uint32_t getSelectionCount() const
		{ assertValid_(); return store_->container_selection_count_(idx_); }

		MoleculeStore* getStore() const      { return store_; }
		std::uint32_t  getStoreIndex() const { return idx_; }
		std::uint64_t  getGeneration() const { return generation_; }

		bool operator==(const ContainerHandleBase& o) const
		{ return store_ == o.store_ && idx_ == o.idx_ && generation_ == o.generation_; }
		bool operator!=(const ContainerHandleBase& o) const { return !(*this == o); }

		/** Release-checked narrow to a typed handle (D64). Returns a typed
				handle when this row's kind matches `T::KIND` and the handle is
				valid; otherwise the NULL typed handle. No RTTI — a `kind` tag
				compare. This is how `getParent()`/`getChildContainer()` results
				are downcast to a concrete kind.
		*/
		template <typename T>
		T as() const
		{
			// HCP-2a: the role-aware handles accept a SET of kinds, so narrow via
			// the static T::acceptsKind() predicate (the legacy single-kind wrappers
			// define it as a one-kind check, preserving their old narrow). Still a
			// tag compare -- no RTTI.
			if (store_ != nullptr && isValid()
				&& T::acceptsKind(store_->container_kind_(idx_)))
			{
				return T(*store_, idx_);
			}
			return T();
		}

		/** v2.2 H4 commit 6.a (D-H4.6 R2): names of all NamedProperties on
		    this container (by value, safe across mutation). Mirrors the
		    AtomHandle::propertyNames() API at atomHandle.h:329 so consumer
		    code that uses the visitor pattern works uniformly across atoms
		    AND containers. Reaches the v0 PropertyManager bag through the
		    container_back_ptr dual-existence bridge; H4 commit 12 removes
		    the bridge and this method backs onto the store's
		    property_columns_ + sparse bag directly. */
		std::vector<String> propertyNames() const
		{
			std::vector<String> out;
			if (!isValid()) return out;
			AtomContainer* c = store_->container_back_ptr(idx_);
			if (c == nullptr) return out;
			Size n = c->countNamedProperties();
			out.reserve(n);
			for (Size i = 0; i < n; ++i)
				out.push_back(String(c->getNamedProperty(i).getName()));
			return out;
		}

		/** v2.2 H4 commit 6.a (D-H4.6 R2): visit each NamedProperty on
		    this container as `const NamedProperty&`. Mirrors
		    AtomHandle::eachProperty at atomHandle.h:346. */
		template <typename Visitor>
		void eachProperty(Visitor&& visit) const
		{
			if (!isValid()) return;
			AtomContainer* c = store_->container_back_ptr(idx_);
			if (c == nullptr) return;
			Size n = c->countNamedProperties();
			for (Size i = 0; i < n; ++i)
				visit(static_cast<const NamedProperty&>(c->getNamedProperty(i)));
		}

		/** v2.2 H4 commit 7b.3 (D-H4.15 R3 counts cluster):
		    `countAtoms()` mirrors AtomContainer::countAtoms but reads
		    through the container_back_ptr dual-existence bridge. The
		    post-H4 implementation walks the store's atom-parent
		    reverse-index directly; the v0-side dispatch is here for
		    dual existence.

		    Returns 0 for null/stale handles and for unbound containers
		    (orphan rows or freed slots). */
		Size countAtoms() const
		{
			if (!isValid()) return 0;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->countAtoms() : 0;
		}

		/** v2.2 H4 commit 7b.3 (D-H4.15 R3 counts cluster):
		    `countAtomContainers()` mirrors AtomContainer::countAtomContainers
		    (recursive child AtomContainer count). */
		Size countAtomContainers() const
		{
			if (!isValid()) return 0;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->countAtomContainers() : 0;
		}

		/** v2.2 H4 commit 7b.3 (D-H4.15 R3 counts cluster):
		    `countBonds()` mirrors AtomContainer::countBonds (recursive
		    bond count rooted at this container). */
		Size countBonds() const
		{
			if (!isValid()) return 0;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->countBonds() : 0;
		}

		/** v2.2 H4 commit 7b.6 (D-H4.15 R3 predicates cluster):
		    `isSubAtomContainerOf` mirrors AtomContainer::isSubAtomContainerOf.
		    Compares container_idx + ancestry through the table-side parent
		    walk; bridges to v0 only if the other handle is itself a
		    container row in the same store. */
		bool isSubAtomContainerOf(const ContainerHandleBase& other) const
		{
			if (!isValid() || !other.isValid()) return false;
			if (store_ != other.store_) return false;
			// Walk up the parent chain from this row; if we hit other's
			// row, this is a sub-container of other.
			std::uint32_t cur = idx_;
			while (cur != MoleculeStore::CONTAINER_NONE)
			{
				if (cur == other.idx_) return true;
				cur = store_->container_parent_(cur);
			}
			return false;
		}

		/** v2.2 H4 commit 7b.6 (D-H4.15 R3 predicates cluster):
		    `isSuperAtomContainerOf` is the dual of isSubAtomContainerOf. */
		bool isSuperAtomContainerOf(const ContainerHandleBase& other) const
		{
			return other.isSubAtomContainerOf(*this);
		}

		/** v2.2 H4 commit 7b.2 (D-H4.15 R3 atom-lookup cluster):
		    `getAtom(name)` mirrors AtomContainer::getAtom(const String&).
		    Returns v0 Atom* during dual existence; at H4 commit 8 Atom
		    is renamed to AtomHandle and the return value becomes the
		    handle by value (D63). No cycle introduced because the
		    return type is the v0 pointer, not AtomHandle. */
		Atom* getAtom(const String& name) const
		{
			if (!isValid()) return nullptr;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->getAtom(name) : nullptr;
		}

		/** v2.2 H4 commit 7b.2: `getAtom(position)` index-based lookup. */
		Atom* getAtom(Position position) const
		{
			if (!isValid()) return nullptr;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->getAtom(position) : nullptr;
		}

		/** v2.2 H4 commit 7b.7 (D-H4.15 R3 bond-application cluster):
		    `applyIntraBond(processor)` mirrors AtomContainer::applyIntraBond
		    -- apply a UnaryProcessor<Bond> to each bond whose endpoints
		    are both within this container's atom set. */
		bool applyIntraBond(UnaryProcessor<Bond>& processor) const
		{
			if (!isValid()) return false;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->applyIntraBond(processor) : false;
		}

		/** v2.2 H4 commit 7b.7 (D-H4.15 R3 bond-application cluster):
		    `applyInterBond(processor)` mirrors AtomContainer::applyInterBond
		    -- apply a UnaryProcessor<Bond> to each bond whose endpoints
		    straddle this container's boundary. */
		bool applyInterBond(UnaryProcessor<Bond>& processor) const
		{
			if (!isValid()) return false;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->applyInterBond(processor) : false;
		}

		/** v2.2 H4 commit 6.a (D-H4.6 R2): property setter cluster.
		    Mirrors AtomHandle's set/has/clearProperty surface (atomHandle.h
		    lines 310-322) for containers. Dispatches through the
		    container_back_ptr bridge during dual existence; post-H4 the
		    setProperty writes to property_columns_ + sparse bag
		    directly. */
		bool hasProperty(const String& n) const
		{
			if (!isValid()) return false;
			AtomContainer* c = store_->container_back_ptr(idx_);
			return c ? c->hasProperty(n) : false;
		}
		void setProperty(const String& n)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->setProperty(n);
		}
		void setProperty(const String& n, bool v)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->setProperty(n, v);
		}
		void setProperty(const String& n, int v)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->setProperty(n, v);
		}
		void setProperty(const String& n, float v)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->setProperty(n, v);
		}
		void setProperty(const String& n, double v)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->setProperty(n, v);
		}
		void setProperty(const String& n, const String& v)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->setProperty(n, v);
		}
		void clearProperty(const String& n)
		{
			if (!isValid()) return;
			if (AtomContainer* c = store_->container_back_ptr(idx_)) c->clearProperty(n);
		}

		protected:

		void assertValid_() const
		{
#ifdef BALL_CONTAINER_HANDLE_CHECKS
			if (!isValid())
			{
				throw Exception::InvalidArgument(__FILE__, __LINE__,
					"container handle is null or stale (invalidated by a store "
					"release/migration since it was created)");
			}
#endif
		}

		MoleculeStore* store_      = nullptr;
		std::uint32_t  idx_        = 0;
		std::uint64_t  generation_ = 0;
	};

	static_assert(sizeof(ContainerHandleBase) == 24,
		"ContainerHandleBase must be 24 B: {store* 8, u32 idx (+4 pad), u64 gen 8}");

// HCP-2a: debug/Python-checked role/kind assertion (BALL_CONTAINER_HANDLE_CHECKS
// = BALL_DEBUG || BALL_PYTHON_WRAPPER). Release C++ elides it -- documented UB on
// misuse, the same contract class as the H1b kind-checked ctor.
#ifdef BALL_CONTAINER_HANDLE_CHECKS
# define BALL_CONTAINER_HANDLE_REQUIRE_(cond, msg)                            \
	do { if (!(cond)) { throw Exception::InvalidArgument(__FILE__, __LINE__, msg); } } while (0)
#else
# define BALL_CONTAINER_HANDLE_REQUIRE_(cond, msg) ((void)0)
#endif

	/** Role-aware molecule-level value handle (HCP-2a). Binds any molecule row
			-- v0 kinds MOLECULE/PROTEIN/NUCLEIC_ACID during dual existence -- and
			exposes the derived `MoleculeRole`. Replaces the H1b single-kind
			`MoleculeHandle`/`ProteinHandle`/`NucleicAcidHandle`; at H4 it is renamed
			to the canonical `Molecule` (D63). NON-polymorphic, 24 B (no data members).
			\ingroup KernelContainers
	*/
	class BALL_EXPORT MoleculeHandle : public ContainerHandleBase
	{
		public:
		/// The molecule-level kinds this handle binds (HCP-2a multi-kind narrow).
		static bool acceptsKind(ContainerKind k)
		{
			return k == ContainerKind::MOLECULE
				|| k == ContainerKind::PROTEIN
				|| k == ContainerKind::NUCLEIC_ACID;
		}

		MoleculeHandle() = default;
		MoleculeHandle(MoleculeStore& store, std::uint32_t idx)
			: ContainerHandleBase(store, idx)
		{
			BALL_CONTAINER_HANDLE_REQUIRE_(store_ == nullptr || acceptsKind(store_->container_kind_(idx_)),
				"MoleculeHandle constructed on a non-molecule-level row");
		}

		/// Derived molecule role (D-HC1: PROTEIN / NUCLEIC_ACID / SMALL_MOLECULE / ...).
		MoleculeRole getMoleculeRole() const
		{ assertValid_(); return store_->container_molecule_role_(idx_); }

		/// Identifier -- role-asserting: only PROTEIN/NUCLEIC_ACID molecules carry one.
		String getID() const
		{
			assertValid_();
			BALL_CONTAINER_HANDLE_REQUIRE_(
				getMoleculeRole() == MoleculeRole::PROTEIN || getMoleculeRole() == MoleculeRole::NUCLEIC_ACID,
				"MoleculeHandle::getID() on a molecule whose role carries no id");
			return String(store_->container_id_(idx_));
		}
	};
	static_assert(sizeof(MoleculeHandle) == sizeof(ContainerHandleBase),
		"MoleculeHandle must add no data members (stays 24 B)");

	/** Role-aware fragment-level value handle (HCP-2a). Binds any fragment row
			-- v0 kinds CHAIN/FRAGMENT/RESIDUE/NUCLEOTIDE/SECONDARY_STRUCTURE -- and
			exposes the derived `FragmentRole`/`ResidueKind`/`SSKind` plus role-asserting
			id / insertion-code / SS-type. Replaces the H1b single-kind `FragmentHandle`/
			`ChainHandle`/`ResidueHandle`/`NucleotideHandle`/`SecondaryStructureHandle`;
			at H4 -> canonical `Fragment` (D63). 24 B.
			\ingroup KernelContainers
	*/
	class BALL_EXPORT FragmentHandle : public ContainerHandleBase
	{
		public:
		static bool acceptsKind(ContainerKind k)
		{
			return k == ContainerKind::CHAIN
				|| k == ContainerKind::FRAGMENT
				|| k == ContainerKind::RESIDUE
				|| k == ContainerKind::NUCLEOTIDE
				|| k == ContainerKind::SECONDARY_STRUCTURE;
		}

		FragmentHandle() = default;
		FragmentHandle(MoleculeStore& store, std::uint32_t idx)
			: ContainerHandleBase(store, idx)
		{
			BALL_CONTAINER_HANDLE_REQUIRE_(store_ == nullptr || acceptsKind(store_->container_kind_(idx_)),
				"FragmentHandle constructed on a non-fragment-level row");
		}

		/// Derived fragment role (CHAIN / RESIDUE / SECONDARY_STRUCTURE / ...).
		FragmentRole getFragmentRole() const
		{ assertValid_(); return store_->container_fragment_role_(idx_); }
		/// Derived residue kind (AMINO_ACID / NUCLEOTIDE / WATER / ...) for RESIDUE-role.
		ResidueKind getResidueKind() const
		{ assertValid_(); return store_->container_residue_kind_(idx_); }
		/// Derived secondary-structure kind (HELIX / STRAND / ...) for SS-role.
		SSKind getSSKind() const
		{ assertValid_(); return store_->container_ss_kind_(idx_); }

		/// Identifier -- role-asserting: RESIDUE-role fragments (former Residue/
		/// Nucleotide) carry an id.
		String getID() const
		{
			assertValid_();
			BALL_CONTAINER_HANDLE_REQUIRE_(getFragmentRole() == FragmentRole::RESIDUE,
				"FragmentHandle::getID() on a non-RESIDUE fragment");
			return String(store_->container_id_(idx_));
		}
		/// Insertion code -- role-asserting (RESIDUE-role fragments).
		char getInsertionCode() const
		{
			assertValid_();
			BALL_CONTAINER_HANDLE_REQUIRE_(getFragmentRole() == FragmentRole::RESIDUE,
				"FragmentHandle::getInsertionCode() on a non-RESIDUE fragment");
			return store_->container_insertion_code_(idx_);
		}
		/// Raw SS type code (legacy spelling) -- role-asserting (SECONDARY_STRUCTURE).
		std::uint8_t getTypeCode() const
		{
			assertValid_();
			BALL_CONTAINER_HANDLE_REQUIRE_(getFragmentRole() == FragmentRole::SECONDARY_STRUCTURE,
				"FragmentHandle::getTypeCode() on a non-secondary-structure fragment");
			return store_->container_ss_type_(idx_);
		}
	};
	static_assert(sizeof(FragmentHandle) == sizeof(ContainerHandleBase),
		"FragmentHandle must add no data members (stays 24 B)");

// HCP-2a (D-2a.3): the 6 former typed handles are `[[deprecated]]` single-kind
// no-data subclass wrappers. They PRESERVE the old single-kind narrow
// (`as<OldHandle>()` + `OldHandle::KIND`) + the inherited kind-specific getters,
// while warning callers to migrate to the role-aware handle + getRole(). Deleted at
// H4 (canonical rename, D63). Defining them does not warn (only USE does); they add
// no data members -> 24 B (structurally guaranteed by the empty body + 24 B base).
#define BALL_DECLARE_LEGACY_HANDLE(NAME, BASE, KIND_ENUM)                     \
	class BALL_DEPRECATED BALL_EXPORT NAME : public BASE                         \
	{                                                                            \
		public:                                                                    \
		static constexpr ContainerKind KIND = ContainerKind::KIND_ENUM;            \
		static bool acceptsKind(ContainerKind k) { return k == ContainerKind::KIND_ENUM; } \
		NAME() = default;                                                          \
		NAME(MoleculeStore& store, std::uint32_t idx) : BASE(store, idx)           \
		{                                                                          \
			BALL_CONTAINER_HANDLE_REQUIRE_(store_ == nullptr || acceptsKind(store_->container_kind_(idx_)), \
				#NAME " constructed on a row of the wrong kind");                      \
		}                                                                          \
	};

	BALL_DECLARE_LEGACY_HANDLE(ProteinHandle,            MoleculeHandle, PROTEIN)
	BALL_DECLARE_LEGACY_HANDLE(NucleicAcidHandle,        MoleculeHandle, NUCLEIC_ACID)
	BALL_DECLARE_LEGACY_HANDLE(ChainHandle,              FragmentHandle, CHAIN)
	BALL_DECLARE_LEGACY_HANDLE(ResidueHandle,            FragmentHandle, RESIDUE)
	BALL_DECLARE_LEGACY_HANDLE(NucleotideHandle,         FragmentHandle, NUCLEOTIDE)
	BALL_DECLARE_LEGACY_HANDLE(SecondaryStructureHandle, FragmentHandle, SECONDARY_STRUCTURE)

#undef BALL_DECLARE_LEGACY_HANDLE
#undef BALL_CONTAINER_HANDLE_REQUIRE_

} // namespace BALL

#endif // BALL_KERNEL_CONTAINERHANDLE_H
