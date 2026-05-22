// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL replacement (K0.2).
//

#include <BALL/KERNEL/moleculeStore.h>
// K0.5.5: invalidate this store's cached CompiledExpressions on dtor so
// the cache doesn't hold dangling MoleculeStore* keys past our lifetime.
#include <BALL/KERNEL/compiledExpression.h>
// v2.1 P1.2 (D31b): private side-table state. Full type needed here for
// std::unique_ptr<MoleculeStoreSideTables>'s destructor instantiation.
#include <BALL/KERNEL/_moleculeStoreInternal.h>
#include <BALL/COMMON/exception.h>

#include <algorithm>
#include <cassert>
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
// v2.1 P1.2: side-table state is owned per-store. Eager-construct in
// the ctor body so accessors can return a valid reference without a
// null check. The pImpl object is small (vector heads only, no
// allocations yet) — composite_nodes_ allocates only when P1.3 wiring
// actually inserts a node.
MoleculeStore::MoleculeStore()
	: side_tables_(std::make_unique<MoleculeStoreSideTables>())
{
}
MoleculeStore::~MoleculeStore()
{
	CompiledExpressionCache::instance().invalidate_store(this);
	// side_tables_ unique_ptr destructs here; the full
	// MoleculeStoreSideTables type is visible because we included
	// _moleculeStoreInternal.h at the top of this TU.
}

// v2.1 P1.2: side-table accessor. Public on the header but only
// callable from TUs that include _moleculeStoreInternal.h (otherwise
// the return type is incomplete — caller can't bind the reference).
MoleculeStoreSideTables& MoleculeStore::sideTables_()
{
	return *side_tables_;
}
const MoleculeStoreSideTables& MoleculeStore::sideTables_() const
{
	return *side_tables_;
}

// v2.1 P1.3: allocate a fresh composite-node slot. Free-list pop or
// vector append. Returned handle's topology fields are all NONE/null
// — caller (P2 wiring code) sets parent/sibling/child as part of the
// mutation it's mirroring.
CompositeHandle MoleculeStoreSideTables::allocate_composite_node_(CompositeKind kind)
{
	CompositeHandle h;
	h.kind = kind;
	// P2.1.1 fix: idx=0 is the null sentinel. Reserve composite_nodes_[0]
	// as a dummy slot so any real allocation has idx >= 1. The check
	// triggers only on the very first allocate for this store.
	if (composite_nodes_.empty())
	{
		// Push the dummy at idx=0 (kind=NONE, all-null topology). Never
		// handed out; never recycled.
		composite_nodes_.push_back(CompositeNode{});
	}
	if (!composite_free_list_.empty())
	{
		h.idx = composite_free_list_.back();
		composite_free_list_.pop_back();
		// Reset the recycled node — old topology must not leak through.
		composite_nodes_[h.idx] = CompositeNode{};
		composite_nodes_[h.idx].kind = kind;
	}
	else
	{
		h.idx = static_cast<std::uint32_t>(composite_nodes_.size());
		CompositeNode n;
		n.kind = kind;
		composite_nodes_.push_back(n);
	}
	return h;
}

void MoleculeStoreSideTables::release_composite_node_(CompositeHandle h)
{
	if (h.isNull()) return;
	assert(h.idx < composite_nodes_.size());
	// Zero topology so a use-after-release that doesn't go through the
	// handle map at least reads NONE/null instead of stale links.
	composite_nodes_[h.idx] = CompositeNode{};
	composite_free_list_.push_back(h.idx);
}

CompositeNode& MoleculeStoreSideTables::node_(CompositeHandle h)
{
	assert(!h.isNull());
	assert(h.idx < composite_nodes_.size());
	return composite_nodes_[h.idx];
}
const CompositeNode& MoleculeStoreSideTables::node_(CompositeHandle h) const
{
	assert(!h.isNull());
	assert(h.idx < composite_nodes_.size());
	return composite_nodes_[h.idx];
}

// v2.1 P1.5 (D23b): promote-once. Materialise the named column if it
// doesn't already exist, then transfer matching-type sparse entries
// into it. Mismatched-type entries stay sparse. The name keeps a
// dense column for the lifetime of the store after this call —
// promote-once / never-demote prevents oscillation flapping per
// D23b's R17b P17b-3 closure.
//
// Returns the number of sparse entries promoted (moved out of the
// sparse bag and into the column).
//
// Called from P2 wiring code's compact() path; P1 just ships the
// primitive.
std::size_t MoleculeStoreSideTables::promote_sparse_(const std::string& name,
                                                    PropertyColumnType t)
{
	PropertyColumnBase* col = property_columns_.findColumn(name);
	if (!col)
	{
		col = property_columns_.registerColumn(name, t);
		if (!col) return 0;  // cap reached; sparse stays.
	}
	if (col->type() != t) return 0;  // can't promote into mismatched-type column.

	std::size_t moved = 0;
	for (auto& kv : sparse_bag_)
	{
		std::uint32_t atom_idx = kv.first;
		auto& vec = kv.second;
		for (auto it = vec.begin(); it != vec.end(); )
		{
			if (it->name != name || it->type != t) { ++it; continue; }
			// Copy matching-type sparse value into the dense column.
			switch (t)
			{
				case PropertyColumnType::BOOL:
					static_cast<PropertyColumn<bool>*>(col)->set(atom_idx, it->v_bool);
					break;
				case PropertyColumnType::INT:
					static_cast<PropertyColumn<std::int32_t>*>(col)->set(atom_idx, it->v_int);
					break;
				case PropertyColumnType::UNSIGNED_INT:
					static_cast<PropertyColumn<std::uint32_t>*>(col)->set(atom_idx, it->v_uint);
					break;
				case PropertyColumnType::FLOAT:
					static_cast<PropertyColumn<float>*>(col)->set(atom_idx, it->v_float);
					break;
				case PropertyColumnType::DOUBLE:
					static_cast<PropertyColumn<double>*>(col)->set(atom_idx, it->v_double);
					break;
				case PropertyColumnType::STRING:
					static_cast<StringPropertyColumn*>(col)->set(atom_idx, it->v_string);
					break;
				default: break;
			}
			it = vec.erase(it);
			++moved;
		}
	}
	// Remove empty atom buckets.
	for (auto it = sparse_bag_.begin(); it != sparse_bag_.end(); )
	{
		if (it->second.empty()) it = sparse_bag_.erase(it);
		else ++it;
	}
	return moved;
}

// v2.1 P1.7 (D24b): owned atomic-word array, rebuilt under exclusive
// access. We use unique_ptr<atomic<u64>[]> rather than
// vector<atomic<u64>> because std::atomic is not Cpp17MoveInsertable
// and libc++ rejects vector::resize/reserve/shrink_to_fit. Copy uses
// relaxed atomic load/store, which is sound when the caller holds
// exclusive write — there are no concurrent readers during resize
// per D24b.
void MoleculeStoreSideTables::resize_selected_bits_(std::size_t new_atom_capacity)
{
	std::size_t new_words = (new_atom_capacity + 63) / 64;
	if (new_words == selected_bits_word_capacity_) return;

	auto new_bits = std::make_unique<std::atomic<std::uint64_t>[]>(new_words);
	std::size_t copy_words = std::min(selected_bits_word_capacity_, new_words);
	for (std::size_t w = 0; w < copy_words; ++w)
	{
		new_bits[w].store(
			selected_bits_[w].load(std::memory_order_relaxed),
			std::memory_order_relaxed);
	}
	for (std::size_t w = copy_words; w < new_words; ++w)
	{
		new_bits[w].store(0, std::memory_order_relaxed);
	}
	selected_bits_                = std::move(new_bits);
	selected_bits_word_capacity_  = new_words;
}

void MoleculeStoreSideTables::set_selected_(std::uint32_t i, bool v)
{
	std::size_t w = i >> 6;
	std::uint64_t mask = std::uint64_t(1) << (i & 63);
	assert(w < selected_bits_word_capacity_);
	if (v)
		selected_bits_[w].fetch_or(mask, std::memory_order_relaxed);
	else
		selected_bits_[w].fetch_and(~mask, std::memory_order_relaxed);
}

bool MoleculeStoreSideTables::is_selected_(std::uint32_t i) const
{
	std::size_t w = i >> 6;
	if (w >= selected_bits_word_capacity_) return false;
	std::uint64_t word = selected_bits_[w].load(std::memory_order_relaxed);
	return (word & (std::uint64_t(1) << (i & 63))) != 0;
}

// ============================================================
// v2.2 H1a (A2 flat SoA topology): ContainerTable implementation.
// Store-side only -- the v0 object tree is the source of truth (D60);
// this table is a verified mirror exercised by HierarchyParity_test.
// ============================================================

// D58: allocate a fresh container row. Reuse a freed slot if available
// (recycled rows are reset so no stale children/links leak), else
// append. Slot 0 is the reserved sentinel (pushed in the ctor), so the
// first real allocation has idx == 1.
std::uint32_t ContainerTable::allocate(ContainerKind kind)
{
	std::uint32_t idx;
	if (!free_list_.empty())
	{
		idx = free_list_.back();
		free_list_.pop_back();
		// D65: PRESERVE the slot's generation across reuse (release already
		// bumped it). A handle holding the pre-release generation must stay
		// invalid even after the slot is recycled.
		std::uint64_t gen = rows_[idx].generation;
		rows_[idx] = ContainerRow{};
		rows_[idx].generation = gen;
	}
	else
	{
		idx = static_cast<std::uint32_t>(rows_.size());
		rows_.emplace_back();  // fresh slot starts at generation 0
	}
	rows_[idx].kind = kind;
	return idx;
}

// Return a row to the free list after clearing it. Idempotent: a row
// whose kind is already NONE is either the slot-0 sentinel or already
// freed, so re-release is a no-op (no double-push). Caller must have
// already detached the row from its parent's child list.
void ContainerTable::release(std::uint32_t idx)
{
	if (idx == NONE || idx == 0 || idx >= rows_.size()) return;
	if (rows_[idx].kind == ContainerKind::NONE) return;  // sentinel / already freed
	// D65: BUMP the per-slot generation on release so any handle still
	// holding the pre-release generation fails its validity check (stale),
	// rather than reading a freed/recycled row.
	std::uint64_t next_gen = rows_[idx].generation + 1;
	rows_[idx] = ContainerRow{};   // clears children + kind=NONE
	rows_[idx].generation = next_gen;
	free_list_.push_back(idx);
}

bool ContainerTable::is_freed(std::uint32_t idx) const
{
	if (idx == 0 || idx >= rows_.size()) return false;  // slot 0 = sentinel, not "freed"
	return rows_[idx].kind == ContainerKind::NONE;
}

std::uint32_t ContainerTable::intern(const std::string& s)
{
	if (s.empty()) return 0u;
	auto it = string_intern_.find(s);
	if (it != string_intern_.end()) return it->second;
	std::uint32_t off = static_cast<std::uint32_t>(string_pool_.size());
	string_pool_.append(s);
	string_pool_.push_back('\0');
	string_intern_.emplace(s, off);
	return off;
}

std::size_t ContainerTable::child_ordinal_(std::uint32_t parent_idx, ChildRef c) const
{
	const auto& ch = rows_[parent_idx].children;
	for (std::size_t i = 0; i < ch.size(); ++i)
		if (ch[i] == c) return i;
	return static_cast<std::size_t>(-1);
}

void ContainerTable::set_reverse_edge_(std::uint32_t parent_idx, ChildRef c)
{
	if (c.kind == ChildRef::CONTAINER)
		rows_[c.idx].parent_container_idx = parent_idx;
	else
		atom_parent_[c.idx] = parent_idx;
}

void ContainerTable::clear_reverse_edge_(ChildRef c)
{
	if (c.kind == ChildRef::CONTAINER)
		rows_[c.idx].parent_container_idx = NONE;
	else
		atom_parent_.erase(c.idx);
}

void ContainerTable::append_child(std::uint32_t parent_idx, ChildRef c)
{
	// R36c idempotency: appending the child that is ALREADY the last child
	// is a no-op. This mirrors v0 Composite::appendChild(), which no-ops
	// when the argument is already last_child_ -- the insert mirror still
	// calls through, and without this guard it would push a duplicate
	// ChildRef. (A child legitimately appears at most once in a parent, so
	// "ref == current last child" always means a re-append.)
	std::vector<ChildRef>& kids = rows_[parent_idx].children;
	if (!kids.empty() && kids.back() == c) return;
	kids.push_back(c);
	set_reverse_edge_(parent_idx, c);
}

void ContainerTable::prepend_child(std::uint32_t parent_idx, ChildRef c)
{
	auto& ch = rows_[parent_idx].children;
	ch.insert(ch.begin(), c);
	set_reverse_edge_(parent_idx, c);
}

void ContainerTable::insert_child_before(std::uint32_t parent_idx, ChildRef c, ChildRef pivot)
{
	std::size_t ord = child_ordinal_(parent_idx, pivot);
	if (ord == static_cast<std::size_t>(-1)) { append_child(parent_idx, c); return; }
	auto& ch = rows_[parent_idx].children;
	ch.insert(ch.begin() + static_cast<std::ptrdiff_t>(ord), c);
	set_reverse_edge_(parent_idx, c);
}

void ContainerTable::insert_child_after(std::uint32_t parent_idx, ChildRef c, ChildRef pivot)
{
	std::size_t ord = child_ordinal_(parent_idx, pivot);
	if (ord == static_cast<std::size_t>(-1)) { append_child(parent_idx, c); return; }
	auto& ch = rows_[parent_idx].children;
	ch.insert(ch.begin() + static_cast<std::ptrdiff_t>(ord + 1), c);
	set_reverse_edge_(parent_idx, c);
}

bool ContainerTable::remove_child(std::uint32_t parent_idx, ChildRef c)
{
	auto& ch = rows_[parent_idx].children;
	for (auto it = ch.begin(); it != ch.end(); ++it)
	{
		if (*it == c)
		{
			ch.erase(it);
			clear_reverse_edge_(c);
			return true;
		}
	}
	return false;
}

// H2a/H2b: clear a row's child VECTOR only -- does NOT touch the children's
// reverse edges. This makes mirrorRederiveOwnRow_ order-INDEPENDENT for
// multi-parent topology moves (swap/splice/insertParent): each affected
// parent's re-append re-establishes the reverse edge of its CURRENT
// children to itself, so a child moved A->B ends up with reverse==B
// regardless of whether A or B is re-derived first. (If clear() cleared
// reverse edges, re-deriving the source after the destination would clobber
// the destination's freshly-set reverse edge.) Children genuinely removed
// from the tree get their reverse cleared by remove_child (the removeChild
// mirror); a child orphaned by the live-clear() direct-detach branch becomes
// unreachable, so its now-stale reverse edge does not affect reachable
// parity.
void ContainerTable::clear_children(std::uint32_t parent_idx)
{
	if (parent_idx == 0 || parent_idx >= rows_.size()) return;
	rows_[parent_idx].children.clear();
}

// R32 LOW: checked reparent. Detach `c` from its current parent (read
// from the reverse edge) before re-attaching, so no stale old-parent
// edge survives. Used by H2's move/splice mirror wiring.
void ContainerTable::reparent_child(std::uint32_t new_parent, ChildRef c)
{
	std::uint32_t old_parent = (c.kind == ChildRef::CONTAINER)
		? rows_[c.idx].parent_container_idx
		: atom_parent(c.idx);
	if (old_parent != NONE && old_parent < rows_.size())
		remove_child(old_parent, c);
	append_child(new_parent, c);
}

// D59/D46: walk parent_container_idx up from `start` (inclusive),
// adjusting each row's selection_count by delta (+1 select, -1 deselect).
// Underflow on an unbalanced deselect clamps at 0.
void ContainerTable::bump_selection_up(std::uint32_t start_container_idx, int delta)
{
	std::uint32_t cur = start_container_idx;
	while (cur != NONE && cur != 0 && cur < rows_.size())
	{
		std::uint32_t& sc = rows_[cur].selection_count;
		if (delta >= 0)
			sc += static_cast<std::uint32_t>(delta);
		else
		{
			std::uint32_t dec = static_cast<std::uint32_t>(-delta);
			sc = (sc < dec) ? 0u : (sc - dec);
		}
		cur = rows_[cur].parent_container_idx;
	}
}

// D56: recursive worker -- copy one src row into *this, recursing for
// CONTAINER children (which builds the old->new remap implicitly via the
// returned new index) and remapping ATOM children through atom_remap.
// NB: allocate() / append_child() may reallocate rows_, so we never hold
// a ContainerRow& across the child loop -- all writes index rows_ fresh.
std::uint32_t ContainerTable::migrate_one_(
	const ContainerTable& src, std::uint32_t src_idx,
	const std::unordered_map<std::uint32_t, std::uint32_t>& atom_remap)
{
	const ContainerRow& srow = src.rows_[src_idx];
	const ContainerKind kind = srow.kind;
	// Snapshot scalar state before any allocation (srow is in `src`, not
	// `*this`, so it stays valid -- but read it up front for clarity).
	const std::string  name_str = src.str(srow.name_offset);
	const std::string  id_str   = src.str(srow.payload.id_offset);
	const char         ins_code = srow.payload.insertion_code;
	const std::uint8_t ss_type  = srow.payload.ss_type;
	const std::uint32_t sel_cnt = srow.selection_count;
	// Copy the child list out before recursion (recursion mutates rows_).
	const std::vector<ChildRef> src_children = srow.children;

	std::uint32_t new_idx = allocate(kind);
	{
		ContainerRow& nrow = rows_[new_idx];
		nrow.name_offset           = intern(name_str);
		nrow.payload.id_offset     = intern(id_str);
		nrow.payload.insertion_code = ins_code;
		nrow.payload.ss_type       = ss_type;
		nrow.selection_count       = sel_cnt;
		// D65: the destination row keeps the generation that allocate()
		// assigned to its slot -- the source's generation is NOT copied
		// (each slot owns its own monotonic counter).
	}
	// NB: container-property migration (D59) is deferred to H2 alongside
	// the full mutation-mirror wiring; H1a migrates topology + payload +
	// names + selection counts only.
	for (const ChildRef& c : src_children)
	{
		if (c.kind == ChildRef::CONTAINER)
		{
			std::uint32_t child_new = migrate_one_(src, c.idx, atom_remap);
			append_child(new_idx, ChildRef{ChildRef::CONTAINER, child_new});
		}
		else
		{
			// R32 HIGH: a missing remap entry is a hard contract violation
			// (cross-store migration requires a complete atom-slot map; an
			// identity map is the explicit same-index/test mode). Silently
			// falling back to the source index would mis-attach the edge to
			// an unrelated destination atom slot.
			auto it = atom_remap.find(c.idx);
			if (it == atom_remap.end())
			{
				throw Exception::InvalidArgument(__FILE__, __LINE__,
					"ContainerTable::migrate_subtree_from: atom_remap is "
					"missing an entry for a migrated atom child; a complete "
					"atom-slot remap is required (identity map for same-index "
					"moves).");
			}
			append_child(new_idx, ChildRef{ChildRef::ATOM, it->second});
		}
	}
	return new_idx;
}

// R32 HIGH: post-order release of the migrated source subtree. Reads each
// row's children before freeing it; clears source atom reverse edges for
// migrated atoms; frees source container rows.
void ContainerTable::release_source_subtree_(ContainerTable& src, std::uint32_t src_idx)
{
	const std::vector<ChildRef> children = src.rows_[src_idx].children;
	for (const ChildRef& c : children)
	{
		if (c.kind == ChildRef::CONTAINER)
			release_source_subtree_(src, c.idx);
		else
			src.atom_parent_.erase(c.idx);  // migrated atom no longer parented here
	}
	src.release(src_idx);
}

std::uint32_t ContainerTable::migrate_subtree_from(
	ContainerTable& src, std::uint32_t src_root,
	const std::unordered_map<std::uint32_t, std::uint32_t>& atom_remap)
{
	// D56 migration is orphan-store -> System-store; a self-move is
	// nonsensical and would free rows we just copied from.
	assert(&src != this);
	std::uint32_t new_root = migrate_one_(src, src_root, atom_remap);
	rows_[new_root].parent_container_idx = NONE;  // caller re-parents under dest tree
	// R32 HIGH: this is a true MOVE -- release the detached source subtree
	// so the orphan table doesn't leak rows and stale source aliases fail.
	release_source_subtree_(src, src_root);
	return new_root;
}

// ============================================================
// v2.2 H1b (D66a): MoleculeStore scalar container accessors.
// Read-only, public-typed bridge over the ContainerTable so the public
// container-handle layer never names the internal row/edge/table types.
// All bounds-guarded: an out-of-range / sentinel idx returns a benign
// default rather than UB (handles also gate via isValid in debug).
// ============================================================

ContainerKind MoleculeStore::container_kind_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return ContainerKind::NONE;
	return t.row(idx).kind;
}

std::string MoleculeStore::container_name_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return {};
	return t.str(t.row(idx).name_offset);
}

std::string MoleculeStore::container_id_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return {};
	return t.str(t.row(idx).payload.id_offset);
}

char MoleculeStore::container_insertion_code_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return ' ';
	return t.row(idx).payload.insertion_code;
}

std::uint8_t MoleculeStore::container_ss_type_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return 0;
	return t.row(idx).payload.ss_type;
}

// v2.2 HCP-1a (D-HC1): collapse role accessors. During dual existence the v0
// ContainerKind still encodes the Molecule/Fragment role + SS sub-kind, so
// these DERIVE from kind (+ ss_type). ResidueKind is the one identity that is
// NOT encoded by kind (it is the former Residue::Property bits), so it is
// stored in the payload. At HCP-2 the kind shrinks to {MOLECULE,FRAGMENT} and
// the role becomes the stored identity these read.
MoleculeRole MoleculeStore::container_molecule_role_(std::uint32_t idx) const
{
	switch (container_kind_(idx))
	{
		case ContainerKind::PROTEIN:      return MoleculeRole::PROTEIN;
		case ContainerKind::NUCLEIC_ACID: return MoleculeRole::NUCLEIC_ACID;
		// HCP-1R MEDIUM: a plain Molecule is NOT necessarily a small molecule
		// -- it may be solvent/water/ion (the v0 Molecule::IS_SOLVENT bit).
		// That bit is a container property whose mirror is deferred, so the
		// row cannot distinguish them yet. Return UNKNOWN rather than assert
		// SMALL_MOLECULE (which would contradict the v0 source of truth for
		// solvated systems). The precise SMALL_MOLECULE/SOLVENT/WATER/LIGAND/
		// ION role lands with the container-property mirror (HCP-2).
		case ContainerKind::MOLECULE:     return MoleculeRole::UNKNOWN;
		default:                          return MoleculeRole::UNKNOWN;
	}
}

FragmentRole MoleculeStore::container_fragment_role_(std::uint32_t idx) const
{
	switch (container_kind_(idx))
	{
		case ContainerKind::CHAIN:               return FragmentRole::CHAIN;
		case ContainerKind::RESIDUE:             return FragmentRole::RESIDUE;
		case ContainerKind::NUCLEOTIDE:          return FragmentRole::RESIDUE;
		case ContainerKind::SECONDARY_STRUCTURE: return FragmentRole::SECONDARY_STRUCTURE;
		case ContainerKind::FRAGMENT:            return FragmentRole::ARBITRARY;
		default:                                 return FragmentRole::UNKNOWN;
	}
}

ResidueKind MoleculeStore::container_residue_kind_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return ResidueKind::UNKNOWN;
	return t.row(idx).payload.residue_kind;
}

SSKind MoleculeStore::container_ss_kind_(std::uint32_t idx) const
{
	// Map the legacy SecondaryStructure::Type (HELIX=0,COIL=1,STRAND=2,
	// TURN=3,UNKNOWN=4) to the collapse SSKind enum.
	switch (container_ss_type_(idx))
	{
		case 0:  return SSKind::HELIX;
		case 1:  return SSKind::COIL;
		case 2:  return SSKind::STRAND;
		case 3:  return SSKind::TURN;
		default: return SSKind::UNKNOWN;
	}
}

std::uint32_t MoleculeStore::container_parent_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return CONTAINER_NONE;
	return t.row(idx).parent_container_idx;  // == CONTAINER_NONE when root/detached
}

std::size_t MoleculeStore::container_child_count_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return 0;
	return t.row(idx).children.size();
}

ContainerChildRef MoleculeStore::container_child_(std::uint32_t idx, std::size_t i) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return {};
	const std::vector<ChildRef>& ch = t.row(idx).children;
	if (i >= ch.size()) return {};
	const ChildRef& c = ch[i];
	return ContainerChildRef(c.kind == ChildRef::ATOM, c.idx);
}

std::uint32_t MoleculeStore::container_selection_count_(std::uint32_t idx) const
{
	// v2.2 HCP-1R (CRITICAL fix): DERIVE the subtree selected-atom count on
	// read by walking the row's subtree edges and querying the store's atom
	// selection bitmap. Previously this returned an incrementally-maintained
	// counter that drifted when selected atoms were moved/removed/cleared
	// (the bumps only fired on select/deselect transitions, not on topology
	// mutation) and that was bypassed by Selectable::setSelected(). Computing
	// from the authoritative selection_ column + the edge structure is
	// correct-by-construction for every mutation path (selection OR topology),
	// at O(subtree) read cost -- acceptable: no production consumer reads it
	// yet (dual existence), and the collapse reworks selection at HCP-2.
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size() || t.is_freed(idx)) return 0;
	std::uint32_t count = 0;
	t.preorder(idx,
		[](std::uint32_t /*cidx*/) {},
		[&](std::uint32_t atom_idx)
		{
			// Read the v0 selection truth (Selectable::selected_, what
			// atom.select()/isSelected() use) via the store's atom back
			// pointer -- this is the dual-existence source of truth. (The SoA
			// selection_ column is a separate v2.0 bitmap not synced by the
			// Composite select() path.) Post-flip this reads the SoA directly.
			Atom* a = (atom_idx < back_ptr_.size()) ? back_ptr_[atom_idx] : 0;
			if (a != 0 && a->isSelected()) ++count;
		});
	return count;
}

std::uint64_t MoleculeStore::container_generation_(std::uint32_t idx) const
{
	const ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return 0;
	return t.row(idx).generation;
}

bool MoleculeStore::container_is_freed_(std::uint32_t idx) const
{
	return side_tables_->container_table_.is_freed(idx);
}

std::size_t MoleculeStore::container_table_size_() const
{
	return side_tables_->container_table_.size();
}

// ============================================================
// v2.2 H2a (D67/D70/D73): write-side container accessors — the
// mutation-mirror surface. Bounds-guarded; public-typed.
// ============================================================

std::uint32_t MoleculeStore::container_create_(ContainerKind kind)
{
	return side_tables_->container_table_.allocate(kind);
}

void MoleculeStore::container_set_name_(std::uint32_t idx, const String& s)
{
	ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return;
	t.row(idx).name_offset = t.intern(std::string(s));
}

void MoleculeStore::container_set_id_(std::uint32_t idx, const String& s)
{
	ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return;
	t.row(idx).payload.id_offset = t.intern(std::string(s));
}

void MoleculeStore::container_set_insertion_code_(std::uint32_t idx, char c)
{
	ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return;
	t.row(idx).payload.insertion_code = c;
}

void MoleculeStore::container_set_ss_type_(std::uint32_t idx, std::uint8_t ty)
{
	ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return;
	t.row(idx).payload.ss_type = ty;
}

void MoleculeStore::container_set_residue_kind_(std::uint32_t idx, ResidueKind rk)
{
	ContainerTable& t = side_tables_->container_table_;
	if (idx == 0 || idx >= t.size()) return;
	t.row(idx).payload.residue_kind = rk;
}

void MoleculeStore::container_append_atom_(std::uint32_t row, std::uint32_t atom_idx)
{
	ContainerTable& t = side_tables_->container_table_;
	if (row == 0 || row >= t.size()) return;
	t.append_child(row, ChildRef{ChildRef::ATOM, atom_idx});
}

void MoleculeStore::container_append_container_(std::uint32_t parent_row, std::uint32_t child_row)
{
	ContainerTable& t = side_tables_->container_table_;
	if (parent_row == 0 || parent_row >= t.size()) return;
	if (child_row == 0 || child_row >= t.size())   return;
	t.append_child(parent_row, ChildRef{ChildRef::CONTAINER, child_row});
}

void MoleculeStore::container_remove_atom_(std::uint32_t row, std::uint32_t atom_idx)
{
	ContainerTable& t = side_tables_->container_table_;
	if (row == 0 || row >= t.size()) return;
	t.remove_child(row, ChildRef{ChildRef::ATOM, atom_idx});
}

void MoleculeStore::container_remove_container_(std::uint32_t parent_row, std::uint32_t child_row)
{
	ContainerTable& t = side_tables_->container_table_;
	if (parent_row == 0 || parent_row >= t.size()) return;
	t.remove_child(parent_row, ChildRef{ChildRef::CONTAINER, child_row});
}

void MoleculeStore::container_clear_children_(std::uint32_t row)
{
	side_tables_->container_table_.clear_children(row);
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
	// 2026-05-18 (R13.4 fix): allocate the stable_id FIRST. If
	// next_stable_id_alloc_() throws on UINT64_MAX overflow, no
	// column or free-list state has been mutated — the store stays
	// at its pre-call shape.
	const StableId new_sid = next_stable_id_alloc_();

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
		atom_types_[idx]      = -1;  // HCP-1P.A: Atom::UNKNOWN_TYPE (born default)
		formal_charges_[idx]  = 0;
		element_indices_[idx] = 0;
		selection_[idx]       = 0;
		name_offsets_[idx]    = 0;
		type_name_offsets_[idx] = 0;  // HCP-1P.A: born default type-name "" (offset 0)
		name_strings_[idx].clear();
		type_name_strings_[idx].clear();
		stable_ids_[idx]      = new_sid;
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
	atom_types_.emplace_back(static_cast<short>(-1));  // HCP-1P.A: Atom::UNKNOWN_TYPE
	formal_charges_.emplace_back(0);
	element_indices_.emplace_back(0);
	selection_.emplace_back(0);
	name_offsets_.emplace_back(0);          // 0 = empty (string_pool_[0] = '\0')
	type_name_offsets_.emplace_back(0);     // HCP-1P.A: born default type-name "" (offset 0)
	name_strings_.emplace_back();           // K0.3b.LATER.5: default empty String
	type_name_strings_.emplace_back();      // HCP-1P.A: born default "" (empty)
	stable_ids_.emplace_back(new_sid);      // R13.4: pre-allocated above
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

	// v2.2 H2b (KR1 HIGH-2): a full store reset must also drop the container
	// mirror. Previously side_tables_ (container_table_ rows + ChildRef edges
	// + atom_parent_ map + property columns + composite nodes) survived
	// clear(), leaving the container table describing the WIPED molecule -- a
	// topology desync, and container handles stayed isValid() across the
	// reset. Replace with a fresh instance (matches the ctor: a fresh
	// ContainerTable re-seeds sentinel slot 0); the old rows are gone, so any
	// held container handle now fails its freed/generation check.
	side_tables_ = std::make_unique<MoleculeStoreSideTables>();

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

	// 2026-05-18 (V21-STRING-POOL-COMPACT, Codex R12 K4):
	// Rebuild string_pool_ + string_intern_ from the live
	// name_strings_/type_name_strings_ columns. The pool was append-
	// only and grew monotonically over a long-running process; freed
	// slots never released their pool space. Now compact() reclaims
	// it. Cost: O(N_live_atoms) string copies + hash inserts.
	rebuild_string_pool_();

	// 2026-05-18 (Codex R12 fix K5): bump generation unconditionally.
	// Pre-fix only bumped on capacity change, but the documented
	// contract is "compact() invalidates all borrowed refs". Even if
	// shrink_to_fit() chose not to reallocate (free space below
	// implementation threshold), the contract said callers couldn't
	// hold refs across compact() — so bumping unconditionally matches
	// the contract and only costs a 64-bit increment.
	(void) old_cap;  // K5 fix: was used by the now-removed conditional bump
	++generation_;

	// V21-STRING-POOL-COMPACT: cached CompiledExpression instances
	// may have baked-in intern_name offsets that just became stale.
	// Invalidate every cached expression for this store.
	CompiledExpressionCache::instance().invalidate_store(this);
}

// V21-STRING-POOL-COMPACT: rebuild the string pool + intern table
// from scratch using only the strings referenced by LIVE atom slots.
// Internal helper called from compact(). Updates name_offsets_ and
// type_name_offsets_ to point into the fresh pool.
void MoleculeStore::rebuild_string_pool_()
{
	std::string                                    new_pool;
	std::unordered_map<std::string, std::uint32_t> new_intern;
	new_pool.push_back('\0');                       // reserved offset 0 = empty string

	auto intern = [&](const std::string& s) -> std::uint32_t
	{
		if (s.empty()) return 0;
		auto it = new_intern.find(s);
		if (it != new_intern.end()) return it->second;
		const std::uint32_t off = static_cast<std::uint32_t>(new_pool.size());
		new_pool.append(s);
		new_pool.push_back('\0');
		new_intern.emplace(s, off);
		return off;
	};

	for (std::size_t i = 0; i < positions_.size(); ++i)
	{
		if (is_freed_[i])
		{
			// Freed slots: drop their pool refs. Use offset 0 (empty).
			name_offsets_[i] = 0;
			type_name_offsets_[i] = 0;
			continue;
		}
		name_offsets_[i]      = intern(std::string(name_strings_[i].c_str()));
		type_name_offsets_[i] = intern(std::string(type_name_strings_[i].c_str()));
	}

	string_pool_   = std::move(new_pool);
	string_intern_ = std::move(new_intern);
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
	// HCP-1P.B: assign the encapsulated std::string directly (no String(c_str)
	// strlen+realloc round-trip); BALL::String wraps a std::string.
	static_cast<std::string&>(name_strings_[i]) = s;
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
	// HCP-1P.B: assign the encapsulated std::string directly (no round-trip).
	static_cast<std::string&>(type_name_strings_[i]) = s;
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

	// v2.1 P4.1 (V21-LOAD-BATCH, Codex R24 P24-4): empty-bond-table
	// fast path. When the store has zero bond records, the CSR
	// adjacency is trivially empty — there is nothing to count or
	// fill. Build the empty CSR (all-zero offsets, empty idx) and
	// clear the dirty flag WITHOUT the O(n_atoms) assign + bonds_
	// scan that the general path below does.
	//
	// This closes the loadSystemJSON O(n^2) regression: the orphan
	// store accumulates ~100k freshly-created bondless atoms during
	// a load; each per-atom adopt() calls for_each_bond_of(orphan)
	// -> ensure_csr_(), and every allocate_atom/release_atom re-sets
	// csr_dirty_, so without this guard each adopt re-ran a full
	// O(n_atoms) rebuild producing an empty adjacency anyway. With
	// the guard the rebuild is O(n_atoms) for the offsets-zero only
	// (assign of n_atoms+1 zeros) — actually still O(n_atoms) for the
	// assign, BUT it skips the bonds_ scan AND, more importantly,
	// the result is cached: subsequent for_each_bond_of calls hit the
	// `if (!csr_dirty_) return;` fast path until the next atom
	// mutation. The net adopt loop drops from O(n^2) to O(n).
	//
	// (add_bond re-dirties on push + tombstone-slot reuse, so the
	// empty CSR is correctly invalidated when bonds appear.)
	if (bonds_.empty())
	{
		bond_csr_off_.assign(n_atoms + 1, 0);
		bond_csr_idx_.clear();
		csr_dirty_ = false;
		return;
	}

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
	// v2.1 P4.1 (V21-LOAD-BATCH, Codex R24): empty-bond fast path —
	// no incident bonds when bonds_ is empty; skip ensure_csr_.
	if (bonds_.empty()) return 0;
	ensure_csr_();
	return bond_csr_off_[i + 1] - bond_csr_off_[i];
}

std::vector<std::uint32_t> MoleculeStore::bonds_of(Index i) const
{
	// v2.1 P4.1 (V21-LOAD-BATCH, Codex R24): empty-bond fast path.
	if (bonds_.empty()) return std::vector<std::uint32_t>();
	ensure_csr_();
	const std::uint32_t lo = bond_csr_off_[i];
	const std::uint32_t hi = bond_csr_off_[i + 1];
	return std::vector<std::uint32_t>(
		bond_csr_idx_.begin() + lo,
		bond_csr_idx_.begin() + hi);
}

} // namespace BALL
