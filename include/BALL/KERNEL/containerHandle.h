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

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#include <cstdint>

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
			if (store_ != nullptr && isValid()
				&& store_->container_kind_(idx_) == T::KIND)
			{
				return T(*store_, idx_);
			}
			return T();
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

// D64: a raw typed ctor on a wrong-kind row is debug-caught (the safe,
// release-checked path is ContainerHandleBase::as<T>()). Release C++
// elides the check (documented UB on misuse).
#ifdef BALL_CONTAINER_HANDLE_CHECKS
# define BALL_CONTAINER_HANDLE_ASSERT_KIND_(EXPECTED)                         \
	do {                                                                         \
		if (store_ != nullptr && store_->container_kind_(idx_) != (EXPECTED))      \
		{                                                                          \
			throw Exception::InvalidArgument(__FILE__, __LINE__,                     \
				"typed container handle constructed on a row of the wrong kind");      \
		}                                                                          \
	} while (0)
#else
# define BALL_CONTAINER_HANDLE_ASSERT_KIND_(EXPECTED) ((void)0)
#endif

// Typed handle boilerplate: a thin wrapper adding ONLY a KIND tag, the
// kind-checked ctor, and (optionally) kind-specific getters. No data
// members -> size identical to the base (pinned below).
#define BALL_DECLARE_CONTAINER_HANDLE(NAME, KIND_ENUM, EXTRA)                 \
	class BALL_EXPORT NAME : public ContainerHandleBase                          \
	{                                                                            \
		public:                                                                    \
		static constexpr ContainerKind KIND = ContainerKind::KIND_ENUM;            \
		NAME() = default;                                                          \
		NAME(MoleculeStore& store, std::uint32_t idx)                              \
			: ContainerHandleBase(store, idx) { assertKind_(); }                     \
		EXTRA                                                                      \
		private:                                                                   \
		void assertKind_() const                                                   \
		{                                                                          \
			BALL_CONTAINER_HANDLE_ASSERT_KIND_(KIND);                                \
		}                                                                          \
	};                                                                           \
	static_assert(sizeof(NAME) == sizeof(ContainerHandleBase),                   \
		#NAME " must add no data members (stays 24 B)");

	// Plain container kinds (no kind-specific scalar payload beyond name).
	BALL_DECLARE_CONTAINER_HANDLE(MoleculeHandle, MOLECULE, )
	BALL_DECLARE_CONTAINER_HANDLE(ChainHandle,    CHAIN,    )
	BALL_DECLARE_CONTAINER_HANDLE(FragmentHandle, FRAGMENT, )

	// Kinds with an `id` string payload.
	BALL_DECLARE_CONTAINER_HANDLE(ProteinHandle, PROTEIN,
		String getID() const { assertValid_(); return String(store_->container_id_(idx_)); }
	)
	BALL_DECLARE_CONTAINER_HANDLE(NucleicAcidHandle, NUCLEIC_ACID,
		String getID() const { assertValid_(); return String(store_->container_id_(idx_)); }
	)

	// Kinds with id + insertion code.
	BALL_DECLARE_CONTAINER_HANDLE(ResidueHandle, RESIDUE,
		String getID() const { assertValid_(); return String(store_->container_id_(idx_)); }
		char   getInsertionCode() const { assertValid_(); return store_->container_insertion_code_(idx_); }
	)
	BALL_DECLARE_CONTAINER_HANDLE(NucleotideHandle, NUCLEOTIDE,
		String getID() const { assertValid_(); return String(store_->container_id_(idx_)); }
		char   getInsertionCode() const { assertValid_(); return store_->container_insertion_code_(idx_); }
	)

	// Secondary structure: a numeric type code (maps to the v0
	// SecondaryStructure::Type enum; kept as a raw u8 in H1b).
	BALL_DECLARE_CONTAINER_HANDLE(SecondaryStructureHandle, SECONDARY_STRUCTURE,
		std::uint8_t getTypeCode() const { assertValid_(); return store_->container_ss_type_(idx_); }
	)

#undef BALL_DECLARE_CONTAINER_HANDLE
#undef BALL_CONTAINER_HANDLE_ASSERT_KIND_

} // namespace BALL

#endif // BALL_KERNEL_CONTAINERHANDLE_H
