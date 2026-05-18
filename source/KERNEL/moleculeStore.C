// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL replacement (K0.2).
//

#include <BALL/KERNEL/moleculeStore.h>

namespace BALL
{

MoleculeStore::MoleculeStore() = default;
MoleculeStore::~MoleculeStore() = default;

MoleculeStore::Index MoleculeStore::allocate_atom()
{
	// K0.3c.1: try to reuse a freed slot first. Free-list reuse never
	// triggers a column reallocation (slot is still in-place), so D7
	// reference-stability is preserved trivially.
	if (!free_list_.empty())
	{
		const Index idx = free_list_.back();
		free_list_.pop_back();
		is_freed_[idx] = 0;   // clear freed flag on reuse
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
		stable_ids_[idx]      = next_stable_id_++;
		// back_ptr_[idx] cleared on release_atom; will be set by caller's
		// set_back_ptr(idx, this). For now stays nullptr (= "freed"); the
		// Atom ctor's bindToStore_ immediately overwrites it. There's a
		// brief window between allocate_atom returning and bindToStore_
		// setting back_ptr where is_freed(idx) is still true. K0.4
		// transactional adopt() must handle this.
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
	stable_ids_.emplace_back(next_stable_id_++);
	back_ptr_.emplace_back(nullptr);
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
	// stable_ids_[i] is not reset; the freed-then-reallocated slot gets
	// a fresh stable id from next_stable_id_++ on reuse. Old stable ids
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
	stable_ids_.reserve(n);
	back_ptr_.reserve(n);
	is_freed_.reserve(n);

	bump_generation_if_reallocated_(old_cap);
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
	stable_ids_.shrink_to_fit();
	back_ptr_.shrink_to_fit();
	is_freed_.shrink_to_fit();

	bump_generation_if_reallocated_(old_cap);
}

void MoleculeStore::bump_generation_if_reallocated_(std::size_t old_cap)
{
	if (positions_.capacity() != old_cap)
	{
		++generation_;
	}
}

// String pool: append-only. Each set_name stores a fresh copy, even if a
// duplicate exists; this trades pool size for write speed. K0.6 will add a
// dedup pass.

void MoleculeStore::set_name(Index i, const std::string& s)
{
	if (string_pool_.empty()) string_pool_.push_back('\0');
	if (s.empty()) { name_offsets_[i] = 0; return; }
	name_offsets_[i] = static_cast<std::uint32_t>(string_pool_.size());
	string_pool_.append(s);
	string_pool_.push_back('\0');
}

void MoleculeStore::set_type_name(Index i, const std::string& s)
{
	if (string_pool_.empty()) string_pool_.push_back('\0');
	if (s.empty()) { type_name_offsets_[i] = 0; return; }
	type_name_offsets_[i] = static_cast<std::uint32_t>(string_pool_.size());
	string_pool_.append(s);
	string_pool_.push_back('\0');
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
void MoleculeStore::remove_bond(std::uint32_t bond_idx)
{
	if (bond_idx >= bonds_.size()) return;             // defensive
	if (is_bond_dead(bond_idx))    return;             // already dead

	bonds_[bond_idx].flags |= FLAG_BOND_DEAD;
	bond_back_ptr_[bond_idx] = nullptr;
	bond_free_list_.push_back(bond_idx);
	csr_dirty_ = true;
}

// Remove every bond between atoms a and b (in either direction).
// Returns count removed.
std::size_t MoleculeStore::remove_bonds_between(Index a, Index b)
{
	std::size_t removed = 0;
	for (std::uint32_t k = 0; k < bonds_.size(); ++k)
	{
		if (is_bond_dead(k)) continue;
		const auto& br = bonds_[k];
		if ((br.a == a && br.b == b) || (br.a == b && br.b == a))
		{
			remove_bond(k);
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
