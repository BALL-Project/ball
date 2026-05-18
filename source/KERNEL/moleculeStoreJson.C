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

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace
{
	// K0.6.3 bit-exact float helpers. Round-trip is byte-identical for
	// normals, subnormals, +/-0, +/-inf, signaling/quiet NaNs.
	std::string float_to_hex_(float f)
	{
		std::uint32_t bits;
		std::memcpy(&bits, &f, sizeof(bits));
		char buf[16];
		std::snprintf(buf, sizeof(buf), "0x%08x", bits);
		return std::string(buf);
	}
	float hex_to_float_(const std::string& s)
	{
		std::uint32_t bits = static_cast<std::uint32_t>(std::stoul(s, nullptr, 16));
		float f;
		std::memcpy(&f, &bits, sizeof(f));
		return f;
	}

	// K0.6.3 reader-side float decode: auto-detect string vs number.
	// String -> hex IEEE-754. Number -> nlohmann decimal parse.
	float decode_float_(const nlohmann::json& v)
	{
		if (v.is_string()) return hex_to_float_(v.get<std::string>());
		return v.get<float>();
	}

	// Emit a single float using the selected format.
	nlohmann::json encode_float_(float f, BALL::JsonFloatFormat fmt)
	{
		if (fmt == BALL::JsonFloatFormat::BIT_EXACT_HEX)
			return float_to_hex_(f);
		return f;
	}
}

namespace BALL
{

void saveStoreJSON(const MoleculeStore& store, std::ostream& os, int indent,
                   JsonFloatFormat float_fmt)
{
	using nlohmann::json;
	const std::size_t n = store.size();

	json doc;
	doc["format_version"]   = MOLECULE_STORE_JSON_VERSION;        // major
	doc["format_minor"]     = MOLECULE_STORE_JSON_VERSION_MINOR;  // K0.6.3
	doc["float_format"]     = (float_fmt == JsonFloatFormat::BIT_EXACT_HEX
		? "bit_exact_hex" : "decimal");
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
		positions.push_back({encode_float_(p.x, float_fmt),
		                     encode_float_(p.y, float_fmt),
		                     encode_float_(p.z, float_fmt)});
		velocities.push_back({encode_float_(v.x, float_fmt),
		                      encode_float_(v.y, float_fmt),
		                      encode_float_(v.z, float_fmt)});
		forces.push_back({encode_float_(f.x, float_fmt),
		                  encode_float_(f.y, float_fmt),
		                  encode_float_(f.z, float_fmt)});
		charges.push_back(encode_float_(store.charge(i), float_fmt));
		radii.push_back(encode_float_(store.radius(i), float_fmt));
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
	// K0.6.3: format_minor is OPTIONAL — K0.6.1/.2 docs predate the
	// minor field. Higher-minor docs are loadable (we ignore unknown
	// top-level keys); lower-minor is fine too.

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

	MoleculeStore::StableId max_stable_id = 0;
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
		// K0.6.3: floats auto-decode via decode_float_ (string=hex,
		// number=decimal) — handles both DECIMAL and BIT_EXACT_HEX
		// writer output transparently.
		store.position(i)      = Vector3(decode_float_(p[0]), decode_float_(p[1]), decode_float_(p[2]));
		store.velocity(i)      = Vector3(decode_float_(v[0]), decode_float_(v[1]), decode_float_(v[2]));
		store.force(i)         = Vector3(decode_float_(f[0]), decode_float_(f[1]), decode_float_(f[2]));
		store.charge(i)        = decode_float_(a["charges"][i]);
		store.radius(i)        = decode_float_(a["radii"][i]);
		store.atom_type(i)     = static_cast<short>(a["atom_types"][i].get<int>());
		store.formal_charge(i) = static_cast<short>(a["formal_charges"][i].get<int>());

		// K0.6.3 range invariants. element_index is u8 (0..255); we
		// don't tighten to the PTE upper bound here because Element::
		// UNKNOWN at 0 + future PTE additions both need to round-trip.
		const int eli = a["element_indices"][i].get<int>();
		require_(eli >= 0 && eli <= 255, "element_index out of [0,255]");
		store.element_index(i) = static_cast<std::uint8_t>(eli);

		const int sel = a["selection"][i].get<int>();
		require_(sel == 0 || sel == 1, "selection must be 0 or 1");
		store.set_selected(i, sel != 0);

		store.set_name(i,      a["names"][i].get<std::string>());
		store.set_type_name(i, a["type_names"][i].get<std::string>());

		// K0.6.3: restore stable_id from the document. Earlier K0.6.2
		// note: stable_ids were read but not patched, which broke
		// cross-session identity. Now we write through the loader hook
		// and reseed next_stable_id_ after the loop so future allocs
		// don't collide with restored ids.
		const auto sid = a["stable_ids"][i].get<std::uint64_t>();
		store.set_stable_id_for_load_(static_cast<MoleculeStore::Index>(i), sid);
		if (sid > max_stable_id) max_stable_id = sid;
	}
	store.reseed_next_stable_id_(max_stable_id + 1);

	// Bond table — single pass; tombstoned bonds were stripped at write.
	const json& bonds = doc["bonds"];
	for (const auto& br : bonds)
	{
		require_(br.is_object() && br.contains("a") && br.contains("b")
			&& br.contains("order") && br.contains("type") && br.contains("flags"),
			"bonds[i] missing field");
		const auto bond_a    = br["a"].get<std::uint32_t>();
		const auto bond_b    = br["b"].get<std::uint32_t>();
		// K0.6.3 range invariants. Bond endpoints must be live slots
		// (the writer skips dead bonds; a dead-bond endpoint here is
		// either corruption or a hand-edit).
		require_(bond_a < n, "bond.a out of [0, size)");
		require_(bond_b < n, "bond.b out of [0, size)");
		const int order_i = br["order"].get<int>();
		const int type_i  = br["type"].get<int>();
		require_(order_i >= 0 && order_i <= 255, "bond.order out of [0,255]");
		require_(type_i  >= 0 && type_i  <= 255, "bond.type out of [0,255]");
		const auto order_v   = static_cast<std::uint8_t>(order_i);
		const auto type_v    = static_cast<std::uint8_t>(type_i);
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
