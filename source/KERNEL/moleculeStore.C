// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL replacement (K0.2).
//

#include <BALL/KERNEL/moleculeStore.h>
// K0.5.5: invalidate this store's cached CompiledExpressions on dtor so
// the cache doesn't hold dangling MoleculeStore* keys past our lifetime.
#include <BALL/KERNEL/compiledExpression.h>
#include <BALL/COMMON/exception.h>

#include <limits>
#include <mutex>
#include <unordered_set>

namespace BALL
{

// 2026-05-18 (V21-STABLE-ID-OVERFLOW, Codex R12 K7):
// Allocate the next stable_id and check for UINT64_MAX wraparound.
// On a 64-bit counter at 1 ns per allocation that's ~584 years
// until exhaustion — practically unreachable in any real workload —
// but a careless test that resets next_stable_id_ to a high value
// via the loader's max_id+1 path or a deliberately adversarial JSON
// could brush against it. Throwing OutOfMemory (closest existing
// exception type for "resource exhausted") makes the failure
// explicit rather than silently wrapping to 0 and re-issuing live
// ids — which would break the "unique stable_id per live atom"
// invariant catastrophically.
MoleculeStore::StableId MoleculeStore::next_stable_id_alloc_()
{
	if (next_stable_id_ == std::numeric_limits<StableId>::max())
		throw Exception::OutOfMemory(__FILE__, __LINE__, 0);
	return next_stable_id_++;
}

// K0.6.3b: checked bulk restore. Called only by loadStoreJSON via the
// friend declaration in moleculeStore.h. Throws if the caller violates
// the stable_id uniqueness invariant.
void MoleculeStore::restore_stable_ids_for_load_(const std::vector<StableId>& ids)
{
	if (ids.size() != positions_.size())
		throw Exception::InvalidArgument(__FILE__, __LINE__,
			"restore_stable_ids_for_load_: ids.size() != store size()");

	std::unordered_set<StableId> seen;
	seen.reserve(ids.size());
	StableId max_id = 0;
	for (StableId id : ids)
	{
		if (!seen.insert(id).second)
			throw Exception::InvalidArgument(__FILE__, __LINE__,
				"restore_stable_ids_for_load_: duplicate stable_id");
		if (id > max_id) max_id = id;
	}

	for (std::size_t i = 0; i < ids.size(); ++i)
		stable_ids_[i] = ids[i];
	// V21-STABLE-ID-OVERFLOW: bump only if needed AND not wrapping.
	// If max_id == UINT64_MAX, future allocations will throw on the
	// first next_stable_id_alloc_() call rather than silently
	// re-issuing 0.
	if (max_id < std::numeric_limits<StableId>::max()
	    && max_id + 1 > next_stable_id_)
	{
		next_stable_id_ = max_id + 1;
	}
	else if (max_id == std::numeric_limits<StableId>::max())
	{
		next_stable_id_ = std::numeric_limits<StableId>::max();
	}
}

// Destruction-order is no longer load-bearing on this ctor.
// History:
//   B1.2 (commit 14f10fdd1): added `(void) CompiledExpressionCache::instance()`
//     here to pin the cache singleton to construct before any MoleculeStore,
//     so the cache would (per C++17 [basic.start.term]) destruct after them.
//     Fixed the original heap-System SIGSEGV at process exit.
//   DTOR-HARDEN (commit f79910675): switched CompiledExpressionCache to a
//     leaky-immortal heap singleton whose destructor never runs. The
//     destruction-order concern became structurally impossible.
//   V21-DROP-CACHE-CTOR-PIN (this commit): pin removed. The dtor at line
//     below can call CompiledExpressionCache::instance() at any point in
//     process teardown and always get a valid cache, with or without prior
//     ctor-side touch.
MoleculeStore::MoleculeStore() = default;
MoleculeStore::~MoleculeStore()
{
	CompiledExpressionCache::instance().invalidate_store(this);
}

// K0.4.6: orphan-store singleton + mutex. Function-local statics give
// thread-safe lazy init (C++17 [stmt.dcl] p4).
//
// 2026-05-18 (Codex R11 fix A complement): pre-reserve a generous
// capacity on first use. The K0.4.6 stress test (4 threads × 500 atoms)
// was flaking ~2-3% even with the ctor-lock fix because non-ctor
// mutators (setCharge/setName/setPosition called from worker code)
// still don't take the orphan mutex, and any concurrent ctor's
// allocate_atom triggers a column reallocation that races with those
// mutators. Pre-reserving 64k avoids reallocation for any realistic
// orphan-store workload (default Atom() ctors from temporary stack
// objects, ASIO / test fixtures). Proper per-mutator orphan-locking
// remains v2.1 work; this band-aid kills the test flake in practice.
MoleculeStore& MoleculeStore::orphanStore()
{
	static MoleculeStore orphan;
	static std::once_flag reserve_once;
	// Bind directly to `orphan`, not via recursive orphanStore() —
	// otherwise call_once + recursion through the same function would
	// deadlock on the once_flag.
	std::call_once(reserve_once, [] {
		orphan.reserve(65536);
	});
	return orphan;
}
std::recursive_mutex& MoleculeStore::orphanMutex()
{
	static std::recursive_mutex m;
	return m;
}

// K0.3c.8 / K0.4.6: atomic back-ptr binding. Single function with an
// optional caller-supplied back_ptr; the bare-pointer no-arg overload
// forwards with back_ptr=nullptr. The non-null path writes back_ptr
// BEFORE clearing is_freed_ so a concurrent reader of an in-flight slot
// never observes (is_freed==false, back_ptr==nullptr). Combined with
// the orphan-store mutex held in Atom::bindToStore_, this closes the
// Round 4 MEDIUM-8 window.
MoleculeStore::Index MoleculeStore::allocate_atom(Atom* back_ptr)
{
	return allocate_atom_with_back_ptr_(back_ptr);
}

MoleculeStore::Index MoleculeStore::allocate_atom()
{
	return allocate_atom_with_back_ptr_(nullptr);
}

MoleculeStore::Index MoleculeStore::allocate_atom_with_back_ptr_(Atom* back_ptr)
{
	// K0.3c.1: try to reuse a freed slot first. Free-list reuse never
	// triggers a column reallocation (slot is still in-place), so D7
	// reference-stability is preserved trivially.
	if (!free_list_.empty())
	{
		const Index idx = free_list_.back();
		free_list_.pop_back();
		back_ptr_[idx] = back_ptr;   // K0.4.6: write back_ptr BEFORE clearing is_freed_
		is_freed_[idx] = 0;
		// Reset the slot's columns to defaults (reused slot was zeroed at
		// release_atom but defensive).
		positions_[idx]       = Vector3(0.f, 0.f, 0.f);
		velocities_[idx]      = Vector3(0.f, 0.f, 0.f);
		forces_[idx]          = Vector3(0.f, 0.f, 0.f);
		charges_[idx]         = 0.f;
		radii_[idx]           = 0.f;
		atom_types_[idx]      = 0;
		formal_charges_[idx]  = 0;
		element_indices_[idx] = 0;
		selection_[idx]       = 0;
		name_offsets_[idx]    = 0;
		type_name_offsets_[idx] = 0;
		name_strings_[idx].clear();
		type_name_strings_[idx].clear();
		stable_ids_[idx]      = next_stable_id_alloc_();
		// back_ptr already written above (K0.4.6); for the nullptr-caller
		// case the slot is live-but-unbound, which is acceptable because
		// the caller is by contract about to bind it.
		csr_dirty_ = true;
		return idx;
	}

	const std::size_t old_cap = positions_.capacity();
	// If this insert would cause a reallocation, no live refs into the
	// store columns may exist. D7 amendment enforcement.
	if (positions_.size() == positions_.capacity())
	{
		assert_no_borrowed_refs_("allocate_atom (would reallocate)");
	}
	const Index idx = static_cast<Index>(positions_.size());

	positions_.emplace_back(Vector3(0.f, 0.f, 0.f));
	velocities_.emplace_back(Vector3(0.f, 0.f, 0.f));
	forces_.emplace_back(Vector3(0.f, 0.f, 0.f));
	charges_.emplace_back(0.f);
	radii_.emplace_back(0.f);
	atom_types_.emplace_back(0);
	formal_charges_.emplace_back(0);
	element_indices_.emplace_back(0);
	selection_.emplace_back(0);
	name_offsets_.emplace_back(0);          // 0 = empty (string_pool_[0] = '\0')
	type_name_offsets_.emplace_back(0);
	name_strings_.emplace_back();           // K0.3b.LATER.5: default empty String
	type_name_strings_.emplace_back();      // K0.3b.LATER.6
	stable_ids_.emplace_back(next_stable_id_alloc_());
	// K0.4.6: write back_ptr BEFORE marking the slot live (is_freed_=0).
	// A concurrent reader sequenced after the is_freed_ store will see a
	// valid back_ptr; a reader sequenced before sees a freed slot. The
	// (is_freed=false, back_ptr=nullptr) tearing window is closed.
	back_ptr_.emplace_back(back_ptr);
	is_freed_.emplace_back(0);              // K0.3c.1: fresh slot is live

	// Make sure offset 0 in the string pool is always an empty C-string so
	// get_name(i) on a fresh atom returns "".
	if (string_pool_.empty()) string_pool_.push_back('\0');

	// CSR offsets array depends on n_atoms; invalidate so next query rebuilds.
	csr_dirty_ = true;

	bump_generation_if_reallocated_(old_cap);
	return idx;
}

// K0.3c.1: release a slot back to the free-list. Clears back_ptr_ as the
// "freed" sentinel; subsequent CSR rebuilds (ensure_csr_) skip bonds that
// touch freed slots. Bond removal proper is K0.3c.2 — for now, dangling
// bond records pointing at freed atoms are tolerated and filtered out at
// query time.
void MoleculeStore::release_atom(Index i)
{
	if (i >= back_ptr_.size())   return;   // defensive
	if (is_freed_[i] != 0)       return;   // already freed (idempotent)

	is_freed_[i] = 1;
	back_ptr_[i] = nullptr;
	// Zero the columns at the freed slot. Not required for correctness
	// (back_ptr_==nullptr is the sentinel), but keeps debugging cleaner
	// and helps subsequent allocate_atom reuse skip the defensive reset.
	positions_[i]       = Vector3(0.f, 0.f, 0.f);
	velocities_[i]      = Vector3(0.f, 0.f, 0.f);
	forces_[i]          = Vector3(0.f, 0.f, 0.f);
	charges_[i]         = 0.f;
	radii_[i]           = 0.f;
	atom_types_[i]      = 0;
	formal_charges_[i]  = 0;
	element_indices_[i] = 0;
	selection_[i]       = 0;
	name_offsets_[i]    = 0;
	type_name_offsets_[i] = 0;
	name_strings_[i].clear();
	type_name_strings_[i].clear();
	// stable_ids_[i] is not reset; the freed-then-reallocated slot gets
	// a fresh stable id from next_stable_id_alloc_() on reuse. Old stable ids
	// don't collide.

	free_list_.push_back(i);
	csr_dirty_ = true;
}

void MoleculeStore::reserve(std::size_t n)
{
	if (n <= positions_.capacity()) return;
	assert_no_borrowed_refs_("reserve (growing capacity)");
	const std::size_t old_cap = positions_.capacity();

	positions_.reserve(n);
	velocities_.reserve(n);
	forces_.reserve(n);
	charges_.reserve(n);
	radii_.reserve(n);
	atom_types_.reserve(n);
	formal_charges_.reserve(n);
	element_indices_.reserve(n);
	selection_.reserve(n);
	name_offsets_.reserve(n);
	type_name_offsets_.reserve(n);
	name_strings_.reserve(n);
	type_name_strings_.reserve(n);
	stable_ids_.reserve(n);
	back_ptr_.reserve(n);
	is_freed_.reserve(n);

	bump_generation_if_reallocated_(old_cap);
}

void MoleculeStore::clear()
{
	// K0.6.2: full reset. Drops every atom, bond, name intern, free-list
	// entry, CSR cache. Used by loadStoreJSON to wipe a non-empty
	// destination before populating from the document; the slot indices
	// in the document assume sequential allocation from 0, which the
	// free-list-reuse path of allocate_atom would break.
	assert_no_borrowed_refs_("clear");

	positions_.clear();
	velocities_.clear();
	forces_.clear();
	charges_.clear();
	radii_.clear();
	atom_types_.clear();
	formal_charges_.clear();
	element_indices_.clear();
	selection_.clear();
	name_offsets_.clear();
	type_name_offsets_.clear();
	name_strings_.clear();
	type_name_strings_.clear();
	stable_ids_.clear();
	back_ptr_.clear();
	is_freed_.clear();

	string_pool_.clear();
	string_intern_.clear();

	bonds_.clear();
	bond_back_ptr_.clear();
	bond_free_list_.clear();
	bond_csr_off_.clear();
	bond_csr_idx_.clear();
	csr_dirty_ = true;

	free_list_.clear();

	// Generation advances unconditionally — any held column reference is
	// now invalid.
	++generation_;
	++selection_generation_;
	next_stable_id_ = 1;
}

void MoleculeStore::compact()
{
	assert_no_borrowed_refs_("compact");
	const std::size_t old_cap = positions_.capacity();

	positions_.shrink_to_fit();
	velocities_.shrink_to_fit();
	forces_.shrink_to_fit();
	charges_.shrink_to_fit();
	radii_.shrink_to_fit();
	atom_types_.shrink_to_fit();
	formal_charges_.shrink_to_fit();
	element_indices_.shrink_to_fit();
	selection_.shrink_to_fit();
	name_offsets_.shrink_to_fit();
	type_name_offsets_.shrink_to_fit();
	name_strings_.shrink_to_fit();
	type_name_strings_.shrink_to_fit();
	stable_ids_.shrink_to_fit();
	back_ptr_.shrink_to_fit();
	is_freed_.shrink_to_fit();

	// 2026-05-18 (Codex R12 fix K5): bump generation unconditionally.
	// Pre-fix only bumped on capacity change, but the documented
	// contract is "compact() invalidates all borrowed refs". Even if
	// shrink_to_fit() chose not to reallocate (free space below
	// implementation threshold), the contract said callers couldn't
	// hold refs across compact() — so bumping unconditionally matches
	// the contract and only costs a 64-bit increment.
	(void) old_cap;  // K5 fix: was used by the now-removed conditional bump
	++generation_;
}

void MoleculeStore::bump_generation_if_reallocated_(std::size_t old_cap)
{
	if (positions_.capacity() != old_cap)
	{
		++generation_;
	}
}

// String pool: append-only, but K0.3c.9 deduplicates writes via an
// intern table (string_intern_). Repeated set_name(i, "X") calls reuse
// the same string_pool_ offset instead of appending. Without this,
// every Atom() ctor's K0.3b.6 dual-write of the default type-name "?"
// would leak 2 bytes per construction (Codex Round 3 HIGH-7).
// release_atom does not reclaim pool space; the intern table keeps
// pool size bounded by the unique-string count.

void MoleculeStore::set_name(Index i, const std::string& s)
{
	// K0.3b.LATER.5: mirror into live String column (sole authority for
	// Atom::getName reads). Pool path stays for persistence.
	name_strings_[i] = String(s.c_str());
	if (string_pool_.empty()) string_pool_.push_back('\0');
	if (s.empty()) { name_offsets_[i] = 0; return; }
	auto it = string_intern_.find(s);
	if (it != string_intern_.end())
	{
		name_offsets_[i] = it->second;
		return;
	}
	const std::uint32_t off = static_cast<std::uint32_t>(string_pool_.size());
	string_pool_.append(s);
	string_pool_.push_back('\0');
	string_intern_.emplace(s, off);
	name_offsets_[i] = off;
}

void MoleculeStore::set_type_name(Index i, const std::string& s)
{
	// K0.3b.LATER.6: mirror into live String column.
	type_name_strings_[i] = String(s.c_str());
	if (string_pool_.empty()) string_pool_.push_back('\0');
	if (s.empty()) { type_name_offsets_[i] = 0; return; }
	auto it = string_intern_.find(s);
	if (it != string_intern_.end())
	{
		type_name_offsets_[i] = it->second;
		return;
	}
	const std::uint32_t off = static_cast<std::uint32_t>(string_pool_.size());
	string_pool_.append(s);
	string_pool_.push_back('\0');
	string_intern_.emplace(s, off);
	type_name_offsets_[i] = off;
}

// K0.5.1: intern a string into string_pool_ + string_intern_ without
// binding it to any specific atom slot. Returns the offset for use as
// a precompiled constant inside a CompiledExpression leaf. Empty string
// maps to offset 0 (the pool's reserved empty-string slot).
std::uint32_t MoleculeStore::intern_name(const std::string& s)
{
	if (string_pool_.empty()) string_pool_.push_back('\0');
	if (s.empty()) return 0;
	auto it = string_intern_.find(s);
	if (it != string_intern_.end()) return it->second;
	const std::uint32_t off = static_cast<std::uint32_t>(string_pool_.size());
	string_pool_.append(s);
	string_pool_.push_back('\0');
	string_intern_.emplace(s, off);
	return off;
}
std::uint32_t MoleculeStore::intern_type_name(const std::string& s)
{
	// type names share the same pool + intern table as names, since they
	// are both string-pool offsets and the same string would dedup.
	return intern_name(s);
}

std::string MoleculeStore::get_name(Index i) const
{
	if (string_pool_.empty()) return std::string();
	const char* p = string_pool_.data() + name_offsets_[i];
	return std::string(p);
}

std::string MoleculeStore::get_type_name(Index i) const
{
	if (string_pool_.empty()) return std::string();
	const char* p = string_pool_.data() + type_name_offsets_[i];
	return std::string(p);
}

std::uint32_t MoleculeStore::add_bond(Index a, Index b,
                                      std::uint8_t order,
                                      std::uint8_t type)
{
	std::lock_guard<std::mutex> lk(bond_mutex_);  // V21-BOND-MUTEX
	// K0.3c.2: prefer reusing a tombstoned slot before extending.
	if (!bond_free_list_.empty())
	{
		const std::uint32_t idx = bond_free_list_.back();
		bond_free_list_.pop_back();
		BondRecord& r = bonds_[idx];
		r.a = a;
		r.b = b;
		r.order = order;
		r.type = type;
		r.flags = 0;        // clear FLAG_BOND_DEAD on reuse
		bond_back_ptr_[idx] = nullptr;
		csr_dirty_ = true;
		return idx;
	}

	BondRecord r;
	r.a = a;
	r.b = b;
	r.order = order;
	r.type = type;
	r.flags = 0;
	bonds_.push_back(r);
	bond_back_ptr_.push_back(nullptr);
	csr_dirty_ = true;
	return static_cast<std::uint32_t>(bonds_.size() - 1);
}

// K0.3c.2: tombstone a bond record and push onto bond_free_list_.
// Idempotent (returns immediately if already dead).
// V21-BOND-MUTEX: internal helper that does NOT take bond_mutex_.
// Caller must already hold it. Used by remove_bond (which locks)
// and remove_bonds_between (which locks once then calls this in loop).
void MoleculeStore::remove_bond_unsafe_(std::uint32_t bond_idx)
{
	if (bond_idx >= bonds_.size()) return;             // defensive
	if (is_bond_dead(bond_idx))    return;             // already dead

	bonds_[bond_idx].flags |= FLAG_BOND_DEAD;
	bond_back_ptr_[bond_idx] = nullptr;
	bond_free_list_.push_back(bond_idx);
	csr_dirty_ = true;
}

void MoleculeStore::remove_bond(std::uint32_t bond_idx)
{
	std::lock_guard<std::mutex> lk(bond_mutex_);  // V21-BOND-MUTEX
	remove_bond_unsafe_(bond_idx);
}

// K0.3c.4: swap atom-connectivity in every live BondRecord so a bond
// previously incident to i is now incident to j (and vice versa). Used
// by Atom::swap to keep the store's bond graph consistent with the
// v1.x bond_[] arrays that get swapped at the Atom layer.
void MoleculeStore::swap_atom_connectivity(Index i, Index j)
{
	std::lock_guard<std::mutex> lk(bond_mutex_);  // V21-BOND-MUTEX
	if (i == j) return;
	for (auto& b : bonds_)
	{
		if (b.flags & FLAG_BOND_DEAD) continue;
		// Symmetric self-bond (b.a == i && b.b == j) ends up swapped twice
		// and is invariant, which is the correct semantics.
		if (b.a == i)      b.a = j;
		else if (b.a == j) b.a = i;
		if (b.b == i)      b.b = j;
		else if (b.b == j) b.b = i;
	}
	csr_dirty_ = true;
}

// Remove every bond between atoms a and b (in either direction).
// Returns count removed.
std::size_t MoleculeStore::remove_bonds_between(Index a, Index b)
{
	std::lock_guard<std::mutex> lk(bond_mutex_);  // V21-BOND-MUTEX
	std::size_t removed = 0;
	for (std::uint32_t k = 0; k < bonds_.size(); ++k)
	{
		if (is_bond_dead(k)) continue;
		const auto& br = bonds_[k];
		if ((br.a == a && br.b == b) || (br.a == b && br.b == a))
		{
			remove_bond_unsafe_(k);                // already locked
			++removed;
		}
	}
	return removed;
}

// CSR rebuild — single pass O(N_atoms + N_bonds). Called lazily on the
// first bonds_of() / bond_degree() / for_each_bond_of() after any bond
// mutation that toggled csr_dirty_.
void MoleculeStore::ensure_csr_() const
{
	// V21-BOND-MUTEX: lock + double-check pattern. If csr_dirty_ is
	// false on the unlocked initial check we can skip the lock entirely
	// (fast path). On dirty, acquire mutex, re-check (another thread
	// may have rebuilt while we waited), then rebuild.
	if (!csr_dirty_) return;
	std::lock_guard<std::mutex> lk(bond_mutex_);
	if (!csr_dirty_) return;

	const std::size_t n_atoms = positions_.size();
	bond_csr_off_.assign(n_atoms + 1, 0);

	// Count degree per atom (each bond contributes to two atoms).
	// K0.3c.1: skip bonds touching a freed slot.
	// K0.3c.2: skip tombstoned bonds.
	auto is_live_bond = [this](const BondRecord& b) -> bool
	{
		return ((b.flags & FLAG_BOND_DEAD) == 0)
		    && !is_freed(b.a) && !is_freed(b.b);
	};
	for (const auto& b : bonds_)
	{
		if (!is_live_bond(b)) continue;
		++bond_csr_off_[b.a + 1];
		if (b.a != b.b) ++bond_csr_off_[b.b + 1];
	}
	// Prefix-sum: bond_csr_off_[i] is now the start index for atom i.
	for (std::size_t i = 1; i <= n_atoms; ++i)
	{
		bond_csr_off_[i] += bond_csr_off_[i - 1];
	}
	// Fill bond_csr_idx_ using a cursor copy of the offsets.
	std::vector<std::uint32_t> cursor(bond_csr_off_.begin(), bond_csr_off_.begin() + n_atoms);
	bond_csr_idx_.assign(bond_csr_off_[n_atoms], 0u);
	for (std::size_t k = 0; k < bonds_.size(); ++k)
	{
		const auto& b = bonds_[k];
		if (!is_live_bond(b)) continue;
		bond_csr_idx_[cursor[b.a]++] = static_cast<std::uint32_t>(k);
		if (b.a != b.b)
		{
			bond_csr_idx_[cursor[b.b]++] = static_cast<std::uint32_t>(k);
		}
	}

	csr_dirty_ = false;
}

std::size_t MoleculeStore::bond_degree(Index i) const
{
	ensure_csr_();
	return bond_csr_off_[i + 1] - bond_csr_off_[i];
}

std::vector<std::uint32_t> MoleculeStore::bonds_of(Index i) const
{
	ensure_csr_();
	const std::uint32_t lo = bond_csr_off_[i];
	const std::uint32_t hi = bond_csr_off_[i + 1];
	return std::vector<std::uint32_t>(
		bond_csr_idx_.begin() + lo,
		bond_csr_idx_.begin() + hi);
}

} // namespace BALL
