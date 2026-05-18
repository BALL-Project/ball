// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — JSON writer for MoleculeStore (K0.6.1).
// Reader: K0.6.2.
//

#include <BALL/KERNEL/moleculeStoreJson.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/COMMON/exception.h>
#include <BALL/EXTERNAL/nlohmann_json.hpp>

#include <istream>
#include <ostream>
#include <string>
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

// ============================================================
// K0.6.2 — JSON reader
// ============================================================
namespace
{
	// Reset `store` to a fresh-construction state via clear(). MUST
	// fully empty (not just release) so loadStoreJSON's allocate_atom
	// loop returns sequential indices 0..n-1 that match the document's
	// slot layout — release_atom alone leaves freed slots on the
	// free-list, which then get reused out-of-order.
	void reset_store_(BALL::MoleculeStore& s)
	{
		s.clear();
	}

	// One-shot REQUIRE: throw a clear ParseError on missing/wrong-type
	// keys instead of an opaque nlohmann::json::exception.
	void require_(bool cond, const char* msg)
	{
		if (!cond)
			throw BALL::Exception::ParseError(__FILE__, __LINE__, msg,
				"K0.6.2: malformed MoleculeStore JSON");
	}
} // namespace

void loadStoreJSON(MoleculeStore& store, std::istream& is)
{
	using nlohmann::json;

	json doc;
	try
	{
		is >> doc;
	}
	catch (const json::parse_error& e)
	{
		throw Exception::ParseError(__FILE__, __LINE__, e.what(),
			"K0.6.2: JSON parse failure");
	}

	// Header invariants.
	require_(doc.is_object(),                       "top-level must be object");
	require_(doc.contains("format_version"),        "missing format_version");
	require_(doc.contains("size"),                  "missing size");
	require_(doc.contains("atoms"),                 "missing atoms");
	require_(doc.contains("bonds"),                 "missing bonds");

	const int  version  = doc["format_version"].get<int>();
	require_(version <= MOLECULE_STORE_JSON_VERSION,
		"format_version higher than this build supports");
	require_(version >= 1, "format_version must be >= 1");

	const std::size_t n = doc["size"].get<std::size_t>();

	const json& a = doc["atoms"];
	require_(a.is_object(), "atoms must be object");
	// Every column must be present and length n. This catches truncated
	// files and column/size mismatches cheaply before any allocation.
	static const char* kColumns[] = {
		"positions", "velocities", "forces", "charges", "radii",
		"atom_types", "formal_charges", "element_indices", "selection",
		"names", "type_names", "stable_ids", "is_freed"
	};
	for (const char* col : kColumns)
	{
		require_(a.contains(col),       col);
		require_(a[col].is_array(),     col);
		require_(a[col].size() == n,    col);
	}

	// Wipe, reserve, populate. reserve avoids per-slot reallocation so
	// the bond table's slot indices stay consistent during load.
	reset_store_(store);
	store.reserve(n + 8);

	for (std::size_t i = 0; i < n; ++i)
	{
		const auto idx = store.allocate_atom();   // sequential -> i
		(void)idx;
		const json& p = a["positions"][i];
		const json& v = a["velocities"][i];
		const json& f = a["forces"][i];
		require_(p.is_array() && p.size() == 3, "positions[i] not [x,y,z]");
		require_(v.is_array() && v.size() == 3, "velocities[i] not [x,y,z]");
		require_(f.is_array() && f.size() == 3, "forces[i] not [x,y,z]");
		store.position(i)      = Vector3(p[0].get<float>(), p[1].get<float>(), p[2].get<float>());
		store.velocity(i)      = Vector3(v[0].get<float>(), v[1].get<float>(), v[2].get<float>());
		store.force(i)         = Vector3(f[0].get<float>(), f[1].get<float>(), f[2].get<float>());
		store.charge(i)        = a["charges"][i].get<float>();
		store.radius(i)        = a["radii"][i].get<float>();
		store.atom_type(i)     = static_cast<short>(a["atom_types"][i].get<int>());
		store.formal_charge(i) = static_cast<short>(a["formal_charges"][i].get<int>());
		store.element_index(i) = static_cast<std::uint8_t>(a["element_indices"][i].get<int>());
		store.set_selected(i, a["selection"][i].get<int>() != 0);
		store.set_name(i,      a["names"][i].get<std::string>());
		store.set_type_name(i, a["type_names"][i].get<std::string>());
		// stable_ids are READ but not back-patched: MoleculeStore owns
		// the next_stable_id counter and assigned a fresh id when we
		// allocate_atom'd. Cross-session identity isn't preserved by
		// K0.6.2; that's a K0.6.3 follow-on once we expose a stable-id
		// override path on the store.
	}

	// Bond table — single pass; tombstoned bonds were stripped at write.
	const json& bonds = doc["bonds"];
	for (const auto& br : bonds)
	{
		require_(br.is_object() && br.contains("a") && br.contains("b")
			&& br.contains("order") && br.contains("type") && br.contains("flags"),
			"bonds[i] missing field");
		const auto bond_a    = br["a"].get<std::uint32_t>();
		const auto bond_b    = br["b"].get<std::uint32_t>();
		const auto order_v   = static_cast<std::uint8_t>(br["order"].get<int>());
		const auto type_v    = static_cast<std::uint8_t>(br["type"].get<int>());
		store.add_bond(bond_a, bond_b, order_v, type_v);
		// flags field is read for forward-compat but not yet honored;
		// FLAG_BOND_DEAD wouldn't make sense for a freshly-added bond.
	}

	// Last: replay release_atom on is_freed[i]==1 so the bond table's
	// indices stayed valid through population. CSR rebuild on next bond
	// query will skip the freed slots' bonds.
	const json& freed = a["is_freed"];
	for (std::size_t i = 0; i < n; ++i)
		if (freed[i].get<int>() != 0)
			store.release_atom(static_cast<MoleculeStore::Index>(i));
}

} // namespace BALL
