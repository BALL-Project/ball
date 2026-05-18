// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — JSON writer for MoleculeStore (K0.6.1).
// Reader: K0.6.2.
//

#include <BALL/KERNEL/moleculeStoreJson.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/EXTERNAL/nlohmann_json.hpp>

#include <ostream>
#include <vector>

namespace BALL
{

void saveStoreJSON(const MoleculeStore& store, std::ostream& os, int indent)
{
	using nlohmann::json;
	const std::size_t n = store.size();

	json doc;
	doc["format_version"]   = MOLECULE_STORE_JSON_VERSION;
	doc["size"]             = n;
	doc["live_atom_count"]  = store.live_atom_count();
	doc["generation"]       = static_cast<std::uint64_t>(store.generation());

	// --- Atom columns ----------------------------------------------------
	// Per-column SoA arrays of length n. Freed slots are kept in place
	// (parallel is_freed[] flag) so bond indices stay valid; the reader
	// reclaims freed slots after population.
	json& atoms = doc["atoms"];

	json positions    = json::array();
	json velocities   = json::array();
	json forces       = json::array();
	json charges      = json::array();
	json radii        = json::array();
	json atom_types   = json::array();
	json formal_ch    = json::array();
	json element_idx  = json::array();
	json selection    = json::array();
	json names        = json::array();
	json type_names   = json::array();
	json stable_ids   = json::array();
	json is_freed     = json::array();

	positions.get_ptr<json::array_t*>()->reserve(n);
	velocities.get_ptr<json::array_t*>()->reserve(n);
	forces.get_ptr<json::array_t*>()->reserve(n);
	charges.get_ptr<json::array_t*>()->reserve(n);
	radii.get_ptr<json::array_t*>()->reserve(n);
	atom_types.get_ptr<json::array_t*>()->reserve(n);
	formal_ch.get_ptr<json::array_t*>()->reserve(n);
	element_idx.get_ptr<json::array_t*>()->reserve(n);
	selection.get_ptr<json::array_t*>()->reserve(n);
	names.get_ptr<json::array_t*>()->reserve(n);
	type_names.get_ptr<json::array_t*>()->reserve(n);
	stable_ids.get_ptr<json::array_t*>()->reserve(n);
	is_freed.get_ptr<json::array_t*>()->reserve(n);

	for (std::size_t i = 0; i < n; ++i)
	{
		const Vector3& p = store.position(i);
		const Vector3& v = store.velocity(i);
		const Vector3& f = store.force(i);
		positions.push_back({p.x, p.y, p.z});
		velocities.push_back({v.x, v.y, v.z});
		forces.push_back({f.x, f.y, f.z});
		charges.push_back(store.charge(i));
		radii.push_back(store.radius(i));
		atom_types.push_back(store.atom_type(i));
		formal_ch.push_back(store.formal_charge(i));
		element_idx.push_back(static_cast<int>(store.element_index(i)));
		selection.push_back(store.selected(i) ? 1 : 0);
		names.push_back(store.get_name(i));
		type_names.push_back(store.get_type_name(i));
		stable_ids.push_back(static_cast<std::uint64_t>(store.stable_id(i)));
		is_freed.push_back(store.is_freed(i) ? 1 : 0);
	}

	atoms["positions"]      = std::move(positions);
	atoms["velocities"]     = std::move(velocities);
	atoms["forces"]         = std::move(forces);
	atoms["charges"]        = std::move(charges);
	atoms["radii"]          = std::move(radii);
	atoms["atom_types"]     = std::move(atom_types);
	atoms["formal_charges"] = std::move(formal_ch);
	atoms["element_indices"]= std::move(element_idx);
	atoms["selection"]      = std::move(selection);
	atoms["names"]          = std::move(names);
	atoms["type_names"]     = std::move(type_names);
	atoms["stable_ids"]     = std::move(stable_ids);
	atoms["is_freed"]       = std::move(is_freed);

	// --- Bond table ------------------------------------------------------
	json bonds = json::array();
	const std::size_t nb = store.bond_count();
	bonds.get_ptr<json::array_t*>()->reserve(store.live_bond_count());
	for (std::size_t i = 0; i < nb; ++i)
	{
		if (store.is_bond_dead(static_cast<std::uint32_t>(i))) continue;
		const BondRecord& br = store.bond(static_cast<std::uint32_t>(i));
		bonds.push_back({
			{"a", br.a},
			{"b", br.b},
			{"order", br.order},
			{"type",  br.type},
			{"flags", br.flags}
		});
	}
	doc["bonds"] = std::move(bonds);

	os << doc.dump(indent);
}

} // namespace BALL
