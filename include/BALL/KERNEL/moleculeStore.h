// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL replacement (K0.2).
// See .planning/v2.x/KERNEL-V2-DECISIONS.md for the design contract.
//

#ifndef BALL_KERNEL_MOLECULESTORE_H
#define BALL_KERNEL_MOLECULESTORE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace BALL
{
	class Atom;
	class Bond;

	/**	Bond record (struct-of-arrays bond table row).
			12 bytes; first/second atom indices (uint32) + bond order (uint8) +
			bond type (uint8) + flags (uint16). Compact replacement for the
			heavyweight v1.x Bond object.
	*/
	struct BALL_EXPORT BondRecord
	{
		std::uint32_t a;
		std::uint32_t b;
		std::uint8_t  order;
		std::uint8_t  type;
		std::uint16_t flags;
	};

	/**	MoleculeStore — ground truth for per-atom and per-bond data.

			SoA columns (positions, velocities, forces, charges, element,
			selection bitmap, name offsets into a string pool, residue index,
			stable id, and a back-pointer to the owning Atom handle).

			Owned per System (D5 in KERNEL-V2-DECISIONS.md). Atom and Bond are
			handle objects holding (store*, idx, generation); their getters
			dispatch through the store. Reference-return getters
			(Vector3& Atom::getPosition()) bind to a column row and stay valid
			until the next store reallocation, which is detected via the
			generation counter (D7).

			@see Atom
			@see Bond
			@see System

			\ingroup  KernelContainers
	*/
	class BALL_EXPORT MoleculeStore
	{
		public:

		typedef std::uint32_t Index;
		typedef std::uint64_t Generation;
		typedef std::uint64_t StableId;

		/**	@name Construction
		*/
		//@{

		MoleculeStore();
		~MoleculeStore();

		// MoleculeStore is non-copyable for now; cloning copies are a v2.0.x
		// follow-on (D12 persistence-format work).
		MoleculeStore(const MoleculeStore&)            = delete;
		MoleculeStore& operator=(const MoleculeStore&) = delete;

		//@}
		/**	@name Atom column accessors (used by Atom handle)
		*/
		//@{

		Vector3&       position(Index i)             { return positions_[i]; }
		const Vector3& position(Index i) const       { return positions_[i]; }

		Vector3&       velocity(Index i)             { return velocities_[i]; }
		const Vector3& velocity(Index i) const       { return velocities_[i]; }

		Vector3&       force(Index i)                { return forces_[i]; }
		const Vector3& force(Index i) const          { return forces_[i]; }

		float&         charge(Index i)               { return charges_[i]; }
		float          charge(Index i) const         { return charges_[i]; }

		float&         radius(Index i)               { return radii_[i]; }
		float          radius(Index i) const         { return radii_[i]; }

		short&         atom_type(Index i)            { return atom_types_[i]; }
		short          atom_type(Index i) const      { return atom_types_[i]; }

		short&         formal_charge(Index i)        { return formal_charges_[i]; }
		short          formal_charge(Index i) const  { return formal_charges_[i]; }

		std::uint8_t&  element_index(Index i)        { return element_indices_[i]; }
		std::uint8_t   element_index(Index i) const  { return element_indices_[i]; }

		bool           selected(Index i) const       { return selection_[i] != 0; }
		void           set_selected(Index i, bool v) { selection_[i] = v ? 1 : 0; }

		StableId       stable_id(Index i) const      { return stable_ids_[i]; }

		Atom*          back_ptr(Index i) const       { return back_ptr_[i]; }
		void           set_back_ptr(Index i, Atom* p){ back_ptr_[i] = p; }

		// String pool for atom & type names. Each atom has two name_offset
		// values (name, type_name) into the pool.
		const std::string& string_pool() const       { return string_pool_; }
		std::uint32_t      name_offset(Index i) const{ return name_offsets_[i]; }
		std::uint32_t      type_name_offset(Index i) const { return type_name_offsets_[i]; }

		void               set_name(Index i, const std::string& s);
		void               set_type_name(Index i, const std::string& s);
		std::string        get_name(Index i) const;
		std::string        get_type_name(Index i) const;

		//@}
		/**	@name Lifecycle
		*/
		//@{

		// Allocate a new atom slot. Returns its index. Generation advances if
		// the column vectors had to reallocate.
		Index allocate_atom();

		// Reserve N slots up front so subsequent allocate_atom() calls don't
		// reallocate (the "reserve discipline" of the D7 reference contract).
		void  reserve(std::size_t n);

		// Shrink columns to fit. Always advances generation (drops references).
		void  compact();

		// Current logical size + capacity.
		std::size_t size() const     { return positions_.size(); }
		std::size_t capacity() const { return positions_.capacity(); }

		//@}
		/**	@name Bond table
		*/
		//@{

		std::uint32_t add_bond(Index a, Index b,
		                       std::uint8_t order = 1,
		                       std::uint8_t type = 0);

		std::size_t bond_count() const               { return bonds_.size(); }
		BondRecord& bond(std::uint32_t i)            { return bonds_[i]; }
		const BondRecord& bond(std::uint32_t i) const{ return bonds_[i]; }

		// Bond handle back-pointers (parallel to bonds_).
		Bond*       bond_back_ptr(std::uint32_t i) const   { return bond_back_ptr_[i]; }
		void        set_bond_back_ptr(std::uint32_t i, Bond* p) { bond_back_ptr_[i] = p; }

		// Number of bonds incident to atom i. O(1) after CSR rebuild.
		std::size_t bond_degree(Index i) const;

		// Returns the indices of all bonds incident to atom i. O(degree(i))
		// after CSR rebuild. Rebuild happens lazily on first call after a
		// bond mutation. CSR replaces the K0.2 linear scan per D15.
		std::vector<std::uint32_t> bonds_of(Index i) const;

		// Iterate bonds incident to atom i without materialising a vector.
		// `fn(bond_idx)` is invoked once per incident bond.
		template <typename F>
		void for_each_bond_of(Index i, F&& fn) const
		{
			ensure_csr_();
			const std::uint32_t lo = bond_csr_off_[i];
			const std::uint32_t hi = bond_csr_off_[i + 1];
			for (std::uint32_t k = lo; k < hi; ++k)
			{
				fn(bond_csr_idx_[k]);
			}
		}

		//@}
		/**	@name Generation + dirty-tracking
		*/
		//@{

		Generation generation() const                { return generation_; }
		Generation selection_generation() const      { return selection_generation_; }

		// Mark the selection bitmap dirty (any selection_-mutating call should
		// invoke this). Independent of structural generation.
		void mark_selection_dirty()                  { ++selection_generation_; }

		//@}
		/**	@name Live-reference enforcement (D7 amendment, K0.2c)

				D7 contract: reference-return getters (Vector3& getPosition()
				etc.) are valid only while no store-resizing mutation is in
				flight. Generation check on the handle catches stale handles
				but cannot catch already-escaped Vector3& references.

				Enforcement: callers that take a reference wrap the borrow in
				a `BorrowedColumnRef` RAII helper which increments
				`borrowed_ref_count_` on construction and decrements on
				destruction. Mutating operations (allocate_atom that
				reallocates, reserve growing capacity, compact) assert that
				`borrowed_ref_count_ == 0` in debug builds.

				Release builds elide the count and the assertion; documented
				as undefined-behavior contract.
		*/
		//@{

#ifndef NDEBUG
		void borrow_ref_inc_() const                 { ++borrowed_ref_count_; }
		void borrow_ref_dec_() const                 { assert(borrowed_ref_count_ > 0); --borrowed_ref_count_; }
		std::size_t borrowed_ref_count() const       { return borrowed_ref_count_; }
		void assert_no_borrowed_refs_(const char* op) const
		{
			if (borrowed_ref_count_ != 0)
			{
				std::fprintf(stderr,
					"FATAL: MoleculeStore::%s called while %zu reference(s) "
					"into store columns are still live. This would invalidate "
					"those references. Caller must release Vector3& / float& "
					"borrows (via BorrowedColumnRef RAII) before any "
					"resize/compact/allocate beyond capacity.\n",
					op, borrowed_ref_count_);
				std::abort();
			}
		}
#else
		void borrow_ref_inc_() const                 {}
		void borrow_ref_dec_() const                 {}
		std::size_t borrowed_ref_count() const       { return 0; }
		void assert_no_borrowed_refs_(const char*) const {}
#endif

		//@}

		private:

		void bump_generation_if_reallocated_(std::size_t old_cap);

		// Atom columns. All vectors are sized to size().
		std::vector<Vector3>      positions_;
		std::vector<Vector3>      velocities_;
		std::vector<Vector3>      forces_;
		std::vector<float>        charges_;
		std::vector<float>        radii_;
		std::vector<short>        atom_types_;
		std::vector<short>        formal_charges_;
		std::vector<std::uint8_t> element_indices_;
		std::vector<std::uint8_t> selection_;
		std::vector<std::uint32_t> name_offsets_;
		std::vector<std::uint32_t> type_name_offsets_;
		std::vector<StableId>     stable_ids_;
		std::vector<Atom*>        back_ptr_;

		std::string               string_pool_;

		// Bond table.
		std::vector<BondRecord>   bonds_;
		std::vector<Bond*>        bond_back_ptr_;

		// CSR adjacency: bond_csr_off_[i] = first bond-list index for atom i;
		// bond_csr_off_[i+1] - bond_csr_off_[i] = degree of atom i.
		// bond_csr_idx_ stores bond indices (into bonds_) sorted by source
		// atom. Rebuilt lazily; mutated by add_bond invalidates csr_dirty_.
		mutable std::vector<std::uint32_t> bond_csr_off_;
		mutable std::vector<std::uint32_t> bond_csr_idx_;
		mutable bool                       csr_dirty_ = true;
		void ensure_csr_() const;

		Generation generation_           = 0;
		Generation selection_generation_ = 0;
		StableId   next_stable_id_       = 1;

#ifndef NDEBUG
		mutable std::size_t borrowed_ref_count_ = 0;
#endif
	};

	/**	RAII helper that borrows a typed reference into a MoleculeStore
			column while incrementing the store's borrowed_ref_count_.
			Use this whenever code holds a Vector3& / float& / short& into
			the store across non-trivial work. Destructor releases the
			borrow.

			In release builds, the helper is a thin wrapper with no overhead
			(borrow inc/dec are no-ops).
	*/
	template <typename T>
	class BALL_EXPORT BorrowedColumnRef
	{
		public:
		BorrowedColumnRef(const MoleculeStore& s, T& ref) noexcept
			: store_(&s), ref_(&ref) { store_->borrow_ref_inc_(); }
		~BorrowedColumnRef() noexcept { if (store_) store_->borrow_ref_dec_(); }

		BorrowedColumnRef(const BorrowedColumnRef&)            = delete;
		BorrowedColumnRef& operator=(const BorrowedColumnRef&) = delete;
		BorrowedColumnRef(BorrowedColumnRef&& o) noexcept
			: store_(o.store_), ref_(o.ref_) { o.store_ = nullptr; }
		BorrowedColumnRef& operator=(BorrowedColumnRef&&) = delete;

		T&       get()        { return *ref_; }
		const T& get() const  { return *ref_; }
		T*       operator->() { return ref_; }
		T&       operator*()  { return *ref_; }

		private:
		const MoleculeStore* store_;
		T*                   ref_;
	};

} // namespace BALL

#endif // BALL_KERNEL_MOLECULESTORE_H
