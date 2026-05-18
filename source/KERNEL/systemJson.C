// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — System + Molecule JSON persistence (K0.6.5).
//

#include <BALL/KERNEL/systemJson.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/moleculeStoreJson.h>
#include <BALL/KERNEL/propertyJson.h>
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
	void require_(bool cond, const char* msg)
	{
		if (!cond)
			throw BALL::Exception::ParseError(__FILE__, __LINE__, msg,
				"K0.6.5: malformed System JSON");
	}

	// K0.6.3 bit-exact decode (mirror of helpers in moleculeStoreJson.C
	// — kept TU-local rather than exposed to avoid bloating the public
	// header surface).
	float hex_to_float_(const std::string& s)
	{
		std::uint32_t bits = static_cast<std::uint32_t>(std::stoul(s, nullptr, 16));
		float f;
		std::memcpy(&f, &bits, sizeof(f));
		return f;
	}
	float decode_float_(const nlohmann::json& v)
	{
		if (v.is_string()) return hex_to_float_(v.get<std::string>());
		return v.get<float>();
	}
}

namespace BALL
{

void saveSystemJSON(const System& sys, std::ostream& os, int indent,
                    JsonFloatFormat float_fmt)
{
	using nlohmann::json;
	json doc;
	doc["document_type"]  = "System";
	doc["format_version"] = SYSTEM_JSON_VERSION;
	doc["format_minor"]   = SYSTEM_JSON_VERSION_MINOR;
	doc["name"]           = std::string(sys.getName().c_str());

	// K0.6.5b: System's own PropertyManager bag (System inherits via
	// AtomContainer -> Composite -> PropertyManager).
	json sys_props = json::object();
	detail::properties_to_json(sys, &sys_props);
	if (!sys_props.empty()) doc["properties"] = std::move(sys_props);

	// Embed the MoleculeStore body under "store" via the detail helper
	// (no string round-trip).
	json store_doc;
	detail::store_to_json_obj(
	    const_cast<System&>(sys).getStore(),
	    &store_doc, float_fmt);
	doc["store"] = std::move(store_doc);

	// Walk molecules; for each, list its atoms' store indices AND its
	// own PropertyManager bag (K0.6.5b).
	// Also track which store slots are claimed by ANY molecule so we
	// can emit the unclaimed-but-live slots as System-level orphan
	// atoms separately.
	System& mut_sys = const_cast<System&>(sys);
	const std::size_t store_n = mut_sys.getStore().size();
	std::vector<bool> in_a_molecule(store_n, false);

	json molecules = json::array();
	const Size nmol = mut_sys.countMolecules();
	molecules.get_ptr<json::array_t*>()->reserve(nmol);
	for (Position mi = 0; mi < nmol; ++mi)
	{
		Molecule* m = mut_sys.getMolecule(mi);
		if (m == nullptr) continue;
		json mol_obj;
		mol_obj["name"] = std::string(m->getName().c_str());

		// K0.6.5b: per-molecule properties.
		json mol_props = json::object();
		detail::properties_to_json(*m, &mol_props);
		if (!mol_props.empty()) mol_obj["properties"] = std::move(mol_props);

		json atom_indices = json::array();
		atom_indices.get_ptr<json::array_t*>()->reserve(m->countAtoms());
		for (AtomIterator it = m->beginAtom(); +it; ++it)
		{
			const std::uint32_t idx = static_cast<std::uint32_t>(it->getStoreIndex());
			atom_indices.push_back(idx);
			if (idx < store_n) in_a_molecule[idx] = true;
		}
		mol_obj["atom_indices"] = std::move(atom_indices);
		molecules.push_back(std::move(mol_obj));
	}
	doc["molecules"] = std::move(molecules);

	// K0.6.5b: System-level orphan atoms — live store slots NOT
	// claimed by any Molecule. Emitted as a sibling array so the
	// reader can re-attach them as direct System children.
	json orphan = json::array();
	MoleculeStore& store = mut_sys.getStore();
	for (std::size_t i = 0; i < store_n; ++i)
	{
		if (store.is_freed(i)) continue;
		if (in_a_molecule[i])  continue;
		if (store.back_ptr(static_cast<MoleculeStore::Index>(i)) == nullptr) continue;
		orphan.push_back(static_cast<std::uint32_t>(i));
	}
	if (!orphan.empty())
		doc["system_atom_indices"] = std::move(orphan);

	os << doc.dump(indent);
}

void loadSystemJSON(System& sys, std::istream& is)
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
			"K0.6.5: JSON parse failure");
	}

	try
	{
		require_(doc.is_object(),                       "top-level must be object");
		require_(doc.contains("document_type"),         "missing document_type");
		require_(doc["document_type"].get<std::string>() == "System",
			"document_type is not 'System'");
		require_(doc.contains("format_version"),        "missing format_version");
		const int version = doc["format_version"].get<int>();
		require_(version >= 1 && version <= SYSTEM_JSON_VERSION,
			"unsupported format_version");
		require_(doc.contains("store"),                 "missing store");
		require_(doc.contains("molecules"),             "missing molecules");

		const json& store_doc = doc["store"];
		const json& a         = store_doc["atoms"];
		require_(store_doc.contains("size"), "store missing size");
		require_(a.is_object(),              "store.atoms must be object");
		const std::size_t n  = store_doc["size"].get<std::size_t>();

		// Every column must be present + correct length. Caught here so
		// we don't half-build atoms then fail mid-way.
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

		// Wipe the destination: drop all Composite children (Molecules +
		// any direct Atoms), then wipe the store.
		sys.destroy();
		sys.getStore().clear();

		if (doc.contains("name"))
			sys.setName(String(doc["name"].get<std::string>().c_str()));

		// K0.6.5b: restore System's own PropertyManager bag.
		if (doc.contains("properties"))
			detail::json_to_properties(sys, &doc["properties"]);

		// Pre-create Atom handles for every live save-slot. Freed slots
		// get no handle — they'll be allocated + immediately released
		// in the store to preserve the slot-count for bond translation.
		// Atoms are NEW (orphan-bound); molecule.insert below reparents
		// + triggers System adoption.
		std::vector<Atom*> atom_by_save_idx(n, nullptr);
		std::vector<bool>  saw_in_molecule(n, false);
		for (std::size_t i = 0; i < n; ++i)
		{
			const int freed_i = a["is_freed"][i].get<int>();
			require_(freed_i == 0 || freed_i == 1, "is_freed must be 0 or 1");
			if (freed_i == 0)
				atom_by_save_idx[i] = new Atom;
			// Else: freed slot — no Atom handle. The save-idx slot will
			// be represented as an unused free-list entry post-load via
			// the trailing release_atom calls below.
		}

		// Walk molecules. For each, build a Molecule, attach its atoms
		// in the doc's atom_indices order, insert into sys. The
		// molecule.insert(atom) path triggers System::adopt which moves
		// the atom out of the orphan store into sys.getStore() — that
		// allocates a fresh slot. Identity of the Atom* survives.
		const json& mols = doc["molecules"];
		require_(mols.is_array(), "molecules must be array");
		for (const auto& mol_obj : mols)
		{
			require_(mol_obj.is_object(),              "molecules[i] must be object");
			require_(mol_obj.contains("atom_indices"), "molecules[i].atom_indices missing");
			require_(mol_obj["atom_indices"].is_array(),"molecules[i].atom_indices must be array");

			Molecule* m = new Molecule;
			if (mol_obj.contains("name"))
				m->setName(String(mol_obj["name"].get<std::string>().c_str()));
			sys.insert(*m);

			// K0.6.5b: per-molecule PropertyManager bag.
			if (mol_obj.contains("properties"))
				detail::json_to_properties(*m, &mol_obj["properties"]);

			for (const auto& idx_json : mol_obj["atom_indices"])
			{
				const std::uint32_t save_idx = idx_json.get<std::uint32_t>();
				require_(save_idx < n,                "atom_indices[i] out of [0, size)");
				require_(atom_by_save_idx[save_idx],  "atom_indices[i] references freed slot");
				m->insert(*atom_by_save_idx[save_idx]);
				saw_in_molecule[save_idx] = true;
			}
		}

		// K0.6.5b: orphan atoms — System direct children not in any
		// Molecule. Insert their pre-created handles straight into sys.
		if (doc.contains("system_atom_indices"))
		{
			const json& orphan = doc["system_atom_indices"];
			require_(orphan.is_array(), "system_atom_indices must be array");
			for (const auto& idx_json : orphan)
			{
				const std::uint32_t save_idx = idx_json.get<std::uint32_t>();
				require_(save_idx < n,               "system_atom_indices[i] out of [0, size)");
				require_(atom_by_save_idx[save_idx], "system_atom_indices[i] references freed slot");
				require_(!saw_in_molecule[save_idx],
					"system_atom_indices[i] also listed inside a molecule");
				// AtomContainer::insert(Atom&) wires both the Composite-
				// tree side (System becomes the atom's parent so
				// countAtoms sees it) AND the store-side auto-adopt via
				// the K0.4.3 hook. System::insert(Molecule&) hides the
				// inherited overload, so call it through the base.
				sys.AtomContainer::insert(*atom_by_save_idx[save_idx]);
				saw_in_molecule[save_idx] = true;   // mark as placed
			}
		}

		// K0.6.5b: any live save-slot not placed in a molecule OR
		// system_atom_indices is a data inconsistency. The writer marks
		// every live atom in one of those two arrays.
		for (std::size_t i = 0; i < n; ++i)
		{
			if (atom_by_save_idx[i] != nullptr && !saw_in_molecule[i])
				throw Exception::ParseError(__FILE__, __LINE__,
					"live save-slot not referenced by any molecule or system_atom_indices",
					"K0.6.5b: orphan-atom contract violation");
		}

		// Build save_idx -> fresh_store_idx map. After all molecule
		// inserts, every live atom is in sys.getStore() at some idx.
		std::vector<std::uint32_t> slot_map(n, 0);
		std::vector<bool>          slot_live(n, false);
		for (std::size_t i = 0; i < n; ++i)
		{
			if (atom_by_save_idx[i] == nullptr) continue;
			slot_map[i]  = atom_by_save_idx[i]->getStoreIndex();
			slot_live[i] = true;
		}

		// Apply column data via the store using the fresh slot map.
		MoleculeStore& store = sys.getStore();
		for (std::size_t i = 0; i < n; ++i)
		{
			if (!slot_live[i]) continue;
			const std::uint32_t fi = slot_map[i];
			const json& p = a["positions"][i];
			const json& v = a["velocities"][i];
			const json& f = a["forces"][i];
			require_(p.is_array() && p.size() == 3, "positions[i] not [x,y,z]");
			require_(v.is_array() && v.size() == 3, "velocities[i] not [x,y,z]");
			require_(f.is_array() && f.size() == 3, "forces[i] not [x,y,z]");
			store.position(fi)      = Vector3(decode_float_(p[0]), decode_float_(p[1]), decode_float_(p[2]));
			store.velocity(fi)      = Vector3(decode_float_(v[0]), decode_float_(v[1]), decode_float_(v[2]));
			store.force(fi)         = Vector3(decode_float_(f[0]), decode_float_(f[1]), decode_float_(f[2]));
			store.charge(fi)        = decode_float_(a["charges"][i]);
			store.radius(fi)        = decode_float_(a["radii"][i]);
			store.atom_type(fi)     = static_cast<short>(a["atom_types"][i].get<int>());
			store.formal_charge(fi) = static_cast<short>(a["formal_charges"][i].get<int>());
			const int eli = a["element_indices"][i].get<int>();
			require_(eli >= 0 && eli <= 255, "element_index out of [0,255]");
			store.element_index(fi) = static_cast<std::uint8_t>(eli);
			const int sel = a["selection"][i].get<int>();
			require_(sel == 0 || sel == 1, "selection must be 0 or 1");
			store.set_selected(fi, sel != 0);
			store.set_name(fi,      a["names"][i].get<std::string>());
			store.set_type_name(fi, a["type_names"][i].get<std::string>());

			// K0.6.5b: per-atom PropertyManager bag, restored via the
			// fresh atom handle.
			if (a.contains("properties") && a["properties"].is_array()
			    && i < a["properties"].size())
			{
				Atom* h = atom_by_save_idx[i];
				if (h != nullptr)
					detail::json_to_properties(*h, &a["properties"][i]);
			}
		}

		// Restore bonds via the slot map. Bonds whose endpoints map to
		// freed save-slots are silently skipped — same shape as the
		// writer's emit which strips dead bonds.
		const json& bonds = store_doc["bonds"];
		require_(bonds.is_array(), "bonds must be array");
		for (const auto& br : bonds)
		{
			require_(br.is_object() && br.contains("a") && br.contains("b")
				&& br.contains("order") && br.contains("type") && br.contains("flags"),
				"bonds[i] missing field");
			const std::uint32_t save_a = br["a"].get<std::uint32_t>();
			const std::uint32_t save_b = br["b"].get<std::uint32_t>();
			require_(save_a < n, "bond.a out of [0, size)");
			require_(save_b < n, "bond.b out of [0, size)");
			if (!slot_live[save_a] || !slot_live[save_b])
				continue;   // skip — endpoint references a freed save-slot
			const int order_i = br["order"].get<int>();
			const int type_i  = br["type"].get<int>();
			require_(order_i >= 0 && order_i <= 255, "bond.order out of [0,255]");
			require_(type_i  >= 0 && type_i  <= 255, "bond.type out of [0,255]");
			store.add_bond(slot_map[save_a], slot_map[save_b],
			               static_cast<std::uint8_t>(order_i),
			               static_cast<std::uint8_t>(type_i));
		}
	}
	catch (const Exception::ParseError&) { throw; }
	catch (const json::exception& e)
	{
		throw Exception::ParseError(__FILE__, __LINE__, e.what(),
			"K0.6.5: JSON access error");
	}
	catch (const std::invalid_argument& e)
	{
		throw Exception::ParseError(__FILE__, __LINE__, e.what(),
			"K0.6.5: malformed value");
	}
	catch (const std::out_of_range& e)
	{
		throw Exception::ParseError(__FILE__, __LINE__, e.what(),
			"K0.6.5: numeric out-of-range");
	}
}

} // namespace BALL
