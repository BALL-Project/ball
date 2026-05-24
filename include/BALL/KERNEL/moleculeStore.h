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

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

// v2.2 H1b (D66a): public container-handle API types (ContainerKind +
// ContainerChildRef). Pulled in here so the scalar container accessors
// below — and the container-handle layer that calls them — never need
// the private _moleculeStoreInternal.h (D31b boundary).
#ifndef BALL_KERNEL_CONTAINERKIND_H
# include <BALL/KERNEL/containerKind.h>
#endif
// v2.2 HCP-1a: collapse role taxonomy (D-HC1) for the role accessors below.
#ifndef BALL_KERNEL_CONTAINERROLE_H
# include <BALL/KERNEL/containerRole.h>
#endif

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iosfwd>      // K0.6.5c: friend decls for loadStoreJSON/loadSystemJSON
#include <memory>      // v2.1 P1.2: unique_ptr<MoleculeStoreSideTables>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace BALL
{
	class Atom;
	class Bond;
	class MoleculeStore;
	class System;
	// v2.1 P1.2 (D31b): forward-decl of the side-table PImpl. Full
	// type lives in include/BALL/KERNEL/_moleculeStoreInternal.h and
	// is consumed only by moleculeStore.C, composite.C, property.C,
	// selectable.C, and SideTableParity_test.C. The unique_ptr<>
	// destructor materialises in moleculeStore.C where the full type
	// is visible.
	class MoleculeStoreSideTables;
	// K0.6.5: forward-declare the JSON loader helper so MoleculeStore
	// can friend it without pulling moleculeStoreJson.h into this header.
	namespace detail { void json_obj_to_store(MoleculeStore& s, const void* json_in); }
	// K0.6.5c: forward-declare System loader for the same reason.
	void loadSystemJSON(System& sys, std::istream& is);

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

		// K0.4.6 (Codex Round 4 HIGH-4): process-global orphan store +
		// its serialising mutex. The orphan store hosts every default-
		// constructed Atom until adopted by a System, so it sees writes
		// from every thread that constructs/destroys atoms. Per-System
		// stores remain single-threaded by the D5 contract and do NOT
		// take this lock. Callers that mutate the orphan (Atom::bindToStore_,
		// ~Atom releasing back to orphan, System::adopt releasing the
		// source slot when src == orphan) must hold orphanMutex() across
		// the entire mutation.
		static MoleculeStore& orphanStore();
		// V21-ORPHAN-MUTATOR-LOCK: switched from std::mutex to
		// std::recursive_mutex. The Atom() ctor (R11 fix A) holds this
		// across its initial-writes block AND those initial writes go
		// through the writeStoreXxx_ helpers which (V21-ORPHAN-MUTATOR-
		// LOCK) also try to acquire the lock. Non-recursive mutex would
		// deadlock. Recursive lets the ctor own the lock and the helpers
		// re-enter freely. Per-process singleton overhead is negligible.
		static std::recursive_mutex& orphanMutex();

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

		// K0.6.3 / K0.6.3b (Codex R7 OPEN-2): single checked bulk-restore
		// path for loadStoreJSON. Replaces the prior two underscore-public
		// helpers, which any caller could invoke and break the
		// "unique stable_id per live atom" invariant.
		// - ids.size() must equal store size()
		// - each id must be unique (verified by hashset)
		// - next_stable_id_ is reseeded past max(ids) inside the method
		// - throws Exception::InvalidArgument on size mismatch or duplicate
		// Friended to the JSON loaders (both store-only and System) so
		// they can call restore_stable_ids_for_load_ — kept private to
		// preserve the "unique stable_id per live atom" invariant
		// against accidental external callers. K0.6.3b made it private;
		// K0.6.5 refactor moved store load via detail::json_obj_to_store;
		// K0.6.5c extends the friend to loadSystemJSON which builds its
		// own fresh-index-keyed StableId vector.
		private:
		friend void loadStoreJSON(MoleculeStore&, std::istream&);
		friend void loadSystemJSON(System&, std::istream&);
		void restore_stable_ids_for_load_(const std::vector<StableId>& ids);
		public:
		// detail::json_obj_to_store needs the same access.
		friend void detail::json_obj_to_store(MoleculeStore&, const void*);

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

		// K0.5.1: intern a string into the store's pool and return its
		// stable offset. Used by the compiled-selection compiler to turn
		// AtomNamePred("CA") into a precomputed name_offset that can be
		// 32-bit-compared against name_offsets_[i] in the inner loop —
		// no string compare, no hash, no allocation.
		std::uint32_t      intern_name(const std::string& s);
		std::uint32_t      intern_type_name(const std::string& s);

		// K0.5.1: raw-pointer column accessors for the bitmap eval loop.
		// Valid for the lifetime of the underlying vector storage; same
		// D7 reference-stability contract — invalidated by reserve/compact/
		// allocate-beyond-capacity. Eval loops re-fetch the pointer per
		// evaluate() call.
		const std::uint8_t*  element_indices_data()    const { return element_indices_.data(); }
		const float*         charges_data()            const { return charges_.data(); }
		const std::uint32_t* name_offsets_data()       const { return name_offsets_.data(); }
		const std::uint32_t* type_name_offsets_data()  const { return type_name_offsets_.data(); }
		const std::uint8_t*  selection_data()          const { return selection_.data(); }
		const std::uint8_t*  is_freed_data()           const { return is_freed_.data(); }

		// K0.3b.LATER.5+6: live String columns are the authority for
		// Atom::getName() / getTypeName(). The string_pool_ + offset path
		// remains the persistence-side representation (StoreFormat). set_name
		// keeps both in sync; release_atom / clear / reserve handle both.
		const String&      name(Index i) const       { return name_strings_[i]; }
		const String&      type_name(Index i) const  { return type_name_strings_[i]; }

		//@}
		/**	@name Lifecycle
		*/
		//@{

		// Allocate a new atom slot. Returns its index. If a freed slot
		// exists on the free-list (from a previous release_atom call) it
		// is reused; otherwise a new slot is appended. Generation advances
		// only when the column vectors actually reallocate.
		//
		// K0.3c.8: the back_ptr-aware overload binds the back pointer in
		// the same call to close the window where is_freed(idx) is false
		// but back_ptr(idx) is null. Callers that immediately bind a
		// handle (e.g. Atom::bindToStore_) should always use this form.
		Index allocate_atom();
		Index allocate_atom(Atom* back_ptr);

		// Release a slot back to the free-list. Called from ~Atom (K0.3c.1).
		// Clears back_ptr_[i] = nullptr (the "freed" sentinel) and pushes
		// i onto free_list_ for reuse. Does NOT compact the columns.
		// Does NOT remove incident bonds — that's K0.3c.2's responsibility
		// (bond removal in store + CSR-rebuild skip).
		void release_atom(Index i);

		// True if slot i has been released by release_atom().
		// Distinct from "back_ptr == nullptr" because back_ptr is null
		// transiently for freshly-allocated-but-not-yet-bound slots too.
		// Used by CSR rebuild to skip dead bonds touching freed slots.
		bool is_freed(Index i) const { return is_freed_[i] != 0; }

		// Reserve N slots up front so subsequent allocate_atom() calls don't
		// reallocate (the "reserve discipline" of the D7 reference contract).
		void  reserve(std::size_t n);

		// Shrink columns to fit. Always advances generation (drops
		// references).
		//
		// 2026-05-18 (Codex R13.2): COMPACT IS NOT SAFE TO RUN
		// CONCURRENTLY WITH CompiledExpression::evaluate() on the same
		// store. compact() rebuilds the string_pool_ + intern table
		// (V21-STRING-POOL-COMPACT, 25bfcd58e). The cache invalidate
		// call inside compact() drops cached compiled expressions, so
		// future get_or_compile re-compiles against the fresh offsets.
		// But any CompiledExpression instance currently being executed
		// (held via shared_ptr in another thread) still references the
		// OLD intern offsets — calling evaluate() concurrently with
		// compact() is a data race on name_offsets_/type_name_offsets_
		// AND a logical bug (cached offset constants point into the
		// new pool at a different string).
		//
		// Caller contract: serialise compact() against all evaluate()
		// calls on the same store. Equivalent to "compact() requires
		// exclusive access to the store". The bond/orphan mutexes
		// don't help here because evaluate() doesn't take them.
		//
		// v2.1 backlog (V21-COMPILED-EXPR-GENERATION-CHECK): add a
		// per-evaluate generation check inside CompiledExpression so
		// stale offsets trigger a recompile-and-retry instead of UB.
		// Until then, single-thread-per-store-or-external-mutex.
		void  compact();

		// K0.6.2: full reset to a fresh-construction state. Drops all
		// atoms, bonds, name interns, free-list entries, CSR caches.
		// Advances generation. Used by loadStoreJSON to wipe a non-empty
		// destination before populating from the document — release_atom
		// alone leaves slots on the free-list, which then collide with
		// the document's slot-index assumptions during reload.
		void  clear();

		// Current logical size + capacity. NB: size() includes freed slots
		// (they remain in the column to preserve back_ptr indices for other
		// live atoms). live_atom_count() returns size() - free_list_size.
		std::size_t size() const             { return positions_.size(); }
		std::size_t capacity() const         { return positions_.capacity(); }
		std::size_t live_atom_count() const  { return positions_.size() - free_list_.size(); }
		std::size_t freed_slot_count() const { return free_list_.size(); }

		//@}
		/**	@name Bond table
		*/
		//@{

		// 2026-05-18 (Codex R12 K9): THREAD-SAFETY CONTRACT for bond
		// mutation. add_bond / remove_bond / remove_bonds_between AND
		// the CSR-rebuild path (ensure_csr_) all touch `bonds_`,
		// `bond_free_list_`, `bond_csr_off_`, `bond_csr_idx_`, and
		// `csr_dirty_` without synchronisation. Callers must serialise
		// access:
		//   - Orphan store: bonds are never created on the orphan store
		//     in the current codebase (default Atom() doesn't bond;
		//     only Atom::createBond / Bond ctor allocates a bond record
		//     after the atom has been adopted into a per-System store).
		//     So orphan + bond is not a contention point.
		//   - Per-System store: standard usage is single-threaded per
		//     System. Multi-thread bond mutation on the same per-System
		//     store is UNDEFINED BEHAVIOUR — caller must wrap with an
		//     external mutex.
		// v2.1 backlog: V21-BOND-MUTEX would add a per-store mutex if
		// concurrent bond mutation becomes a real use case (e.g.,
		// parallel force-field setup that builds bonds from atom pairs).
		std::uint32_t add_bond(Index a, Index b,
		                       std::uint8_t order = 1,
		                       std::uint8_t type = 0);

		// K0.3c.2: remove a bond from the store. Tombstones the record
		// (BondRecord::flags |= FLAG_DEAD) and pushes the index onto
		// bond_free_list_ for reuse by future add_bond. Idempotent.
		// CSR rebuild skips dead bonds.
		void remove_bond(std::uint32_t bond_idx);

		// Convenience: remove every bond connecting atoms (a, b) in either
		// direction. Returns the number of bonds removed (typically 0 or 1).
		std::size_t remove_bonds_between(Index a, Index b);

		// True if bond record at index i is tombstoned.
		bool is_bond_dead(std::uint32_t i) const { return (bonds_[i].flags & FLAG_BOND_DEAD) != 0; }

		std::size_t bond_count() const               { return bonds_.size(); }   // includes dead
		std::size_t live_bond_count() const          { return bonds_.size() - bond_free_list_.size(); }
		std::size_t dead_bond_count() const          { return bond_free_list_.size(); }
		BondRecord& bond(std::uint32_t i)            { return bonds_[i]; }
		const BondRecord& bond(std::uint32_t i) const{ return bonds_[i]; }

		// Bond handle back-pointers (parallel to bonds_).
		Bond*       bond_back_ptr(std::uint32_t i) const   { return bond_back_ptr_[i]; }
		void        set_bond_back_ptr(std::uint32_t i, Bond* p) { bond_back_ptr_[i] = p; }

		// K0.3c.4: rewrite every live BondRecord so .a/.b reference
		// indices are swapped (i↔j). Matches v1.x Atom::swap semantics
		// of "atom1 now has atom2's bond connectivity, in-place." Self-
		// bond between i and j is symmetric and survives unchanged.
		void swap_atom_connectivity(Index i, Index j);

		// Bit on BondRecord::flags marking a tombstoned record.
		static constexpr std::uint16_t FLAG_BOND_DEAD = 0x0001;

		// Number of bonds incident to atom i. O(1) after CSR rebuild.
		std::size_t bond_degree(Index i) const;

		// Returns the indices of all bonds incident to atom i. O(degree(i))
		// after CSR rebuild. Rebuild happens lazily on first call after a
		// bond mutation. CSR replaces the K0.2 linear scan per D15.
		std::vector<std::uint32_t> bonds_of(Index i) const;

		// Iterate bonds incident to atom i. `fn(bond_idx)` is invoked
		// once per incident bond.
		//
		// K0.3c.10 mutation-safe: snapshot the CSR slice into a local
		// before invoking the callback so callers may safely call
		// store-mutating ops (remove_bond, allocate_atom, etc.) from
		// within the callback without corrupting the outer iteration.
		// Cost: one vector allocation per call; the slice is typically
		// degree(i) which is small (<= ~6 for organic chemistry).
		template <typename F>
		void for_each_bond_of(Index i, F&& fn) const
		{
			// v2.1 P4.1 (V21-LOAD-BATCH, Codex R24): empty-bond-table
			// fast path. When the store has zero bond records there are
			// no incident bonds for any atom, so skip ensure_csr_()
			// entirely. This is the load-time O(n^2) fix: the orphan
			// store holds ~100k bondless atoms during a JSON load, and
			// every per-atom adopt() re-dirties the CSR (via
			// allocate_atom/release_atom) — without this guard each
			// adopt's for_each_bond_of triggered a full O(n_atoms) CSR
			// rebuild (the bond_csr_off_.assign), giving O(n^2). The
			// guard makes the bondless case O(1) per call → O(n) total.
			// The ensure_csr_() internal empty fast path alone is
			// insufficient because the assign(n_atoms+1, 0) still runs
			// per call when csr_dirty_ keeps being re-set.
			if (bonds_.empty()) return;
			ensure_csr_();
			const std::uint32_t lo = bond_csr_off_[i];
			const std::uint32_t hi = bond_csr_off_[i + 1];
			std::vector<std::uint32_t> snapshot(
				bond_csr_idx_.begin() + lo,
				bond_csr_idx_.begin() + hi);
			for (std::uint32_t bi : snapshot) fn(bi);
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
		/**	@name Container metadata accessors (v2.2 H1b, D66a)

				Scalar read accessors over the container metadata table
				(ContainerTable, in the side-table PImpl). They return ONLY
				public types (ContainerKind, ContainerChildRef, std::string,
				scalars) so the public container-handle layer
				(<BALL/KERNEL/containerHandle.h>) can forward to them without
				ever naming the private ContainerRow/ChildRef/ContainerTable
				types — keeping the D31b encapsulation boundary intact. Defined
				out-of-line in moleculeStore.C where the internal header is
				visible. `idx` is a container-row index (1-based; 0 is the
				sentinel). Mirrors the existing atom-column accessor pattern.

				Through H1b–H3 the container table is a verified mirror; the v0
				object tree is the source of truth (D60).
		*/
		//@{

		ContainerKind      container_kind_(std::uint32_t idx) const;
		std::string        container_name_(std::uint32_t idx) const;
		std::string        container_id_(std::uint32_t idx) const;
		char               container_insertion_code_(std::uint32_t idx) const;
		std::uint8_t       container_ss_type_(std::uint32_t idx) const;
		// v2.2 HCP-1a (D-HC1): collapse role accessors. MoleculeRole /
		// FragmentRole / SSKind are DERIVED from container_kind_ (+ ss_type)
		// during dual existence (the v0 kind still encodes them); ResidueKind
		// is STORED in the payload (absorbs the Residue::Property identity
		// bits). At HCP-2 the kind shrinks to {MOLECULE,FRAGMENT} and the
		// role becomes the stored identity.
		MoleculeRole       container_molecule_role_(std::uint32_t idx) const;
		FragmentRole       container_fragment_role_(std::uint32_t idx) const;
		ResidueKind        container_residue_kind_(std::uint32_t idx) const;
		SSKind             container_ss_kind_(std::uint32_t idx) const;
		std::uint32_t      container_parent_(std::uint32_t idx) const;
		std::size_t        container_child_count_(std::uint32_t idx) const;
		ContainerChildRef  container_child_(std::uint32_t idx, std::size_t i) const;
		std::uint32_t      container_selection_count_(std::uint32_t idx) const;
		std::uint64_t      container_generation_(std::uint32_t idx) const;
		bool               container_is_freed_(std::uint32_t idx) const;
		std::size_t        container_table_size_() const;

		// Sentinel returned by container_parent_ for a root / detached
		// container (mirrors ContainerRow::NONE; kept public so handles can
		// test for "no parent" without the internal type).
		static constexpr std::uint32_t CONTAINER_NONE = 0xFFFFFFFFu;

		// v2.2 H2a (D67/D70/D73): write-side container accessors — the
		// mutation-mirror surface. Take/return ONLY public types so the
		// CONCEPT-layer mirror (composite.C) and System::adoptSubtree
		// (system.C) drive the table without naming the internal
		// ContainerRow/ChildRef/ContainerTable types (D31b boundary).
		// Forward-only mirror: callers never invoke these from a
		// destructor cascade (the removeChild hook gates on
		// being_destroyed_, D69). All idx-bounds-guarded.
		std::uint32_t container_create_(ContainerKind kind);
		void container_set_name_(std::uint32_t idx, const String& s);
		void container_set_id_(std::uint32_t idx, const String& s);
		void container_set_insertion_code_(std::uint32_t idx, char c);
		void container_set_ss_type_(std::uint32_t idx, std::uint8_t t);
		// v2.2 HCP-1a: set the stored ResidueKind (role=RESIDUE fragments).
		void container_set_residue_kind_(std::uint32_t idx, ResidueKind rk);
		// v2.2 HCP-2c.1 (D-2c.1): set the stored MoleculeRole (molecule rows).
		void container_set_molecule_role_(std::uint32_t idx, MoleculeRole mr);
		void container_append_atom_(std::uint32_t row, std::uint32_t atom_idx);
		void container_append_container_(std::uint32_t parent_row, std::uint32_t child_row);
		void container_remove_atom_(std::uint32_t row, std::uint32_t atom_idx);
		void container_remove_container_(std::uint32_t parent_row, std::uint32_t child_row);
		// Clear a row's child list (+ reverse edges) so the insert mirror can
		// re-derive it from the v0 child order in one idempotent pass.
		void container_clear_children_(std::uint32_t row);

		//@}
		/**	@name Live-reference enforcement (D7 amendment, K0.2c, audited K0.4.7)

				D7 contract: reference-return getters (Vector3& getPosition()
				etc.) are valid only while no store-resizing mutation is in
				flight. Generation check on the handle catches stale handles
				but cannot catch already-escaped Vector3& references.

				**K0.4.7 enforcement audit (Codex Round 4 HIGH-5):** the raw
				reference-returning getters (`Atom::getPosition()`,
				`Atom::getVelocity()`, `Atom::getForce()`) do NOT auto-wrap
				their result in a `BorrowedColumnRef`, so they do NOT bump
				`borrowed_ref_count_` and the `assert_no_borrowed_refs_`
				debug guard will not fire on a subsequent reallocating
				operation. This is a **documented contract**: callers that
				intend to hold a column reference across mutating store
				operations MUST explicitly construct a `BorrowedColumnRef`
				(see helper below) to opt in to the debug-build guard.
				Without opt-in, dangling references are undefined behaviour
				in both debug and release.

				Wrapping every getter result implicitly would change the
				return type, breaking the v1.x-compatible signature surface
				the rest of the codebase relies on. The trade-off chosen for
				v2.0: keep the natural reference API, document the lifetime
				rules, and offer opt-in machine enforcement for code that
				wants it. K0.5 / K0.6 will progressively migrate hot
				kernel-internal call sites that DO want enforcement.

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

		// K0.4.6: unified allocation path with optional back_ptr binding;
		// the public allocate_atom() / allocate_atom(Atom*) overloads
		// forward to this helper. Writes back_ptr_ BEFORE marking the slot
		// live so a concurrent reader never sees the (is_freed==false,
		// back_ptr==nullptr) tear.
		Index allocate_atom_with_back_ptr_(Atom* back_ptr);

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
		// K0.3b.LATER.5+6: live String columns. Sole authority for
		// Atom::getName/getTypeName reads. Pool + offsets remain for the
		// persistence/binary format.
		std::vector<String>        name_strings_;
		std::vector<String>        type_name_strings_;
		std::vector<StableId>     stable_ids_;
		std::vector<Atom*>        back_ptr_;
		std::vector<std::uint8_t> is_freed_;       // K0.3c.1 freed-slot bitset

		std::string               string_pool_;

		// K0.3c.9: intern table mapping string content -> offset in
		// string_pool_. set_name / set_type_name consult this before
		// appending so duplicate writes (e.g. every default Atom() ctor
		// dual-writing the type-name "?") reuse a single pool entry.
		// Long-running processes no longer leak string_pool_ space
		// monotonically. Empty string keeps the offset-0 fast path and
		// is not stored in the map.
		std::unordered_map<std::string, std::uint32_t> string_intern_;

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

		// V21-BOND-MUTEX (Codex R12 K9): per-store mutex protecting
		// every mutation of bonds_, bond_back_ptr_, bond_free_list_,
		// bond_csr_off_, bond_csr_idx_, and csr_dirty_. add_bond,
		// remove_bond, remove_bonds_between, swap_atom_connectivity,
		// and ensure_csr_ all take this. Reads via bond(i),
		// bond_back_ptr(i), bonds_of(i), bond_degree(i),
		// for_each_bond_of(i, fn) call ensure_csr_ which is mutex-
		// protected; the subsequent reads of the now-stable CSR /
		// bonds_ vectors are NOT mutex-protected — caller must keep
		// the returned references/iterators valid by not invoking
		// any bond mutator concurrently. This matches the
		// D7-reference-stability contract pattern.
		mutable std::mutex bond_mutex_;

		// V21-BOND-MUTEX: lock-free helper called from remove_bond and
		// remove_bonds_between. Caller must hold bond_mutex_.
		void remove_bond_unsafe_(std::uint32_t bond_idx);

		// V21-STRING-POOL-COMPACT (Codex R12 K4): rebuild string_pool_
		// + string_intern_ from the live name/type_name columns,
		// reclaiming space from freed slots. Called from compact().
		void rebuild_string_pool_();

		// V21-STABLE-ID-OVERFLOW (Codex R12 K7): allocate next stable_id
		// with UINT64_MAX wraparound check. Throws Exception::OutOfMemory
		// rather than silently re-issuing 0. Called from allocate_atom paths.
		StableId next_stable_id_alloc_();

		Generation generation_           = 0;
		Generation selection_generation_ = 0;
		StableId   next_stable_id_       = 1;

		// K0.3c.1: free-list of slot indices released by release_atom().
		// allocate_atom() pops from here before extending the columns.
		std::vector<Index> free_list_;

		// K0.3c.2: free-list of bond-record indices tombstoned by
		// remove_bond. add_bond pops from here before appending.
		std::vector<std::uint32_t> bond_free_list_;

		// v2.1 P1.2 (D22b + D31b): PImpl side-table state. Full type
		// in include/BALL/KERNEL/_moleculeStoreInternal.h. P1.2 populates
		// composite_nodes_ + free list; P1.4-P1.8 extend with property
		// columns + selection bits. Atom/Composite/PropertyManager/
		// Selectable continue to inherit + use inline state in v2.0
		// shape through P1; P2 flips reads to the side tables.
		std::unique_ptr<MoleculeStoreSideTables> side_tables_;

		// v2.1 P1.2: accessor for the 4 wiring TUs (composite.C,
		// property.C, selectable.C, moleculeStoreJson.C). Public so
		// the wiring TUs can call without friend declarations; gated
		// by the fact that only those TUs include
		// _moleculeStoreInternal.h, so only they can name the return
		// type. Anyone else gets a forward-decl-only error.
		public:
		MoleculeStoreSideTables&       sideTables_();
		const MoleculeStoreSideTables& sideTables_() const;
		private:

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
