// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — System + Molecule JSON persistence (K0.6.5).
//

#include <BALL/KERNEL/systemJson.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>          // v2.1 P4.2: createBond + Bond setters
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/moleculeStoreJson.h>
#include <BALL/KERNEL/propertyJson.h>
#include <BALL/COMMON/exception.h>
#include <BALL/EXTERNAL/nlohmann_json.hpp>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace
{
	// v2.1 P4.0: env-gated load profiler. Activated by setting
	// BALL_JSON_LOAD_PROFILE=1; prints a per-phase breakdown of
	// loadSystemJSON to stderr on completion. Zero behavioral cost
	// when the env var is unset (the only per-iteration cost — the
	// property-restore accumulator — is guarded by the `on` flag).
	// Buckets map to the P4.0 roadmap categories:
	//   parse    = nlohmann is >> doc
	//   validate = Phase 1 + molecule-index validation
	//   alloc    = new Atom loop
	//   insert   = Composite-tree molecule/atom inserts + adoption
	//   columns  = SoA store-column writes (positions/charges/...)
	//   props    = per-atom PropertyManager bag restoration (subset of columns window)
	//   stableid = stable-id bulk restore
	//   bonds    = bond-record restore
	struct LoadProfile
	{
		using clk = std::chrono::steady_clock;
		bool on = false;
		double parse_ms = 0, validate_ms = 0, alloc_ms = 0, insert_ms = 0,
		       columns_ms = 0, props_ms = 0, stableid_ms = 0, bonds_ms = 0;
		LoadProfile() { on = (std::getenv("BALL_JSON_LOAD_PROFILE") != nullptr); }
		static clk::time_point now() { return clk::now(); }
		static double ms(clk::time_point a, clk::time_point b)
		{ return std::chrono::duration<double, std::milli>(b - a).count(); }
		void report() const
		{
			if (!on) return;
			std::fprintf(stderr,
				"[json-load-profile] parse=%.1f validate=%.1f alloc=%.1f "
				"insert=%.1f columns=%.1f (of which props=%.1f) stableid=%.1f "
				"bonds=%.1f ms (total=%.1f)\n",
				parse_ms, validate_ms, alloc_ms, insert_ms, columns_ms,
				props_ms, stableid_ms, bonds_ms,
				parse_ms + validate_ms + alloc_ms + insert_ms + columns_ms
				+ stableid_ms + bonds_ms);
		}
	};

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
	LoadProfile prof;                       // v2.1 P4.0 profiler
	auto t_start = LoadProfile::now();
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
	auto t_after_parse = LoadProfile::now();
	prof.parse_ms = LoadProfile::ms(t_start, t_after_parse);

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

		// v2.1 P4.1 Layer 2 (V21-LOAD-BATCH): pre-reserve the
		// destination store's SoA columns to the known atom count.
		// Avoids ~17 incremental vector reallocations during the
		// per-atom adopt phase. (Layer 1's empty-bond CSR fast path
		// in moleculeStore.h is the primary O(n^2)->O(n) fix; this
		// reserve is the complementary constant-factor win.)
		sys.getStore().reserve(n + 64);

		if (doc.contains("name"))
			sys.setName(String(doc["name"].get<std::string>().c_str()));

		// K0.6.5b: restore System's own PropertyManager bag.
		if (doc.contains("properties"))
			detail::json_to_properties(sys, &doc["properties"]);

		// K0.6.5c (Codex R8 OPEN-3a): TWO-PHASE validation. PHASE 1 walks
		// all atom_indices + system_atom_indices BEFORE any allocation
		// or insertion, rejecting duplicate / out-of-range / freed-slot
		// references. Only after Phase 1 succeeds does Phase 2 mutate
		// state. This closes both the "duplicate save_idx within one
		// molecule" gap and the "second-molecule's duplicate reject
		// happens after first molecule's insert" sequencing bug.

		// Phase 1 — read is_freed strict 0/1.
		std::vector<bool> is_freed_in_doc(n, false);
		for (std::size_t i = 0; i < n; ++i)
		{
			const int freed_i = a["is_freed"][i].get<int>();
			require_(freed_i == 0 || freed_i == 1, "is_freed must be 0 or 1");
			is_freed_in_doc[i] = (freed_i != 0);
		}

		// Phase 1 — walk molecule atom_indices, recording placement.
		std::vector<bool> claimed_by_molecule(n, false);
		const json& mols = doc["molecules"];
		require_(mols.is_array(), "molecules must be array");
		for (const auto& mol_obj : mols)
		{
			require_(mol_obj.is_object(),               "molecules[i] must be object");
			require_(mol_obj.contains("atom_indices"),  "molecules[i].atom_indices missing");
			require_(mol_obj["atom_indices"].is_array(),"molecules[i].atom_indices must be array");
			for (const auto& idx_json : mol_obj["atom_indices"])
			{
				const std::uint32_t save_idx = idx_json.get<std::uint32_t>();
				require_(save_idx < n, "atom_indices[i] out of [0, size)");
				require_(!is_freed_in_doc[save_idx],
					"atom_indices[i] references freed slot");
				require_(!claimed_by_molecule[save_idx],
					"atom_indices[i] duplicated (claimed twice)");
				claimed_by_molecule[save_idx] = true;
			}
		}

		// Phase 1 — walk orphan atoms.
		std::vector<bool> claimed_by_system(n, false);
		if (doc.contains("system_atom_indices"))
		{
			const json& orphan = doc["system_atom_indices"];
			require_(orphan.is_array(), "system_atom_indices must be array");
			for (const auto& idx_json : orphan)
			{
				const std::uint32_t save_idx = idx_json.get<std::uint32_t>();
				require_(save_idx < n, "system_atom_indices[i] out of [0, size)");
				require_(!is_freed_in_doc[save_idx],
					"system_atom_indices[i] references freed slot");
				require_(!claimed_by_molecule[save_idx],
					"system_atom_indices[i] also listed inside a molecule");
				require_(!claimed_by_system[save_idx],
					"system_atom_indices[i] duplicated");
				claimed_by_system[save_idx] = true;
			}
		}

		// Phase 1 — every live save-slot must be claimed exactly once.
		for (std::size_t i = 0; i < n; ++i)
		{
			if (is_freed_in_doc[i]) continue;
			require_(claimed_by_molecule[i] || claimed_by_system[i],
				"live save-slot not referenced by any molecule or system_atom_indices");
		}

		// 2026-05-18 (Codex R11 fix D — extended Phase 1 validation).
		// Validate column shapes, numeric ranges, stable_id duplicates,
		// and bond endpoint ranges BEFORE Phase 2 mutates anything.
		// Cheap O(n) checks that previously interleaved with mutation
		// and could throw post-Composite-insert leaving sys in a
		// partial state. The remaining numeric-range checks in Phase 2
		// (decode_float_ throws, set_selected gates) are belt-and-
		// suspenders; the rollback guard catches them too.
		require_(a.contains("stable_ids") && a["stable_ids"].is_array()
			&& a["stable_ids"].size() == n, "stable_ids must be array of size n");
		require_(a.contains("positions")  && a["positions"].is_array()  && a["positions"].size() == n, "positions size mismatch");
		require_(a.contains("velocities") && a["velocities"].is_array() && a["velocities"].size() == n, "velocities size mismatch");
		require_(a.contains("forces")     && a["forces"].is_array()     && a["forces"].size() == n,     "forces size mismatch");
		require_(a.contains("charges")    && a["charges"].is_array()    && a["charges"].size() == n,    "charges size mismatch");
		require_(a.contains("radii")      && a["radii"].is_array()      && a["radii"].size() == n,      "radii size mismatch");
		require_(a.contains("atom_types") && a["atom_types"].is_array() && a["atom_types"].size() == n, "atom_types size mismatch");
		require_(a.contains("formal_charges") && a["formal_charges"].is_array() && a["formal_charges"].size() == n, "formal_charges size mismatch");
		require_(a.contains("element_indices") && a["element_indices"].is_array() && a["element_indices"].size() == n, "element_indices size mismatch");
		require_(a.contains("selection") && a["selection"].is_array() && a["selection"].size() == n, "selection size mismatch");
		require_(a.contains("names")      && a["names"].is_array()      && a["names"].size() == n,      "names size mismatch");
		require_(a.contains("type_names") && a["type_names"].is_array() && a["type_names"].size() == n, "type_names size mismatch");

		// stable_id uniqueness across LIVE slots (freed slots' ids are
		// allowed to alias; they'll be discarded).
		std::unordered_set<std::uint64_t> seen_sid;
		seen_sid.reserve(n);
		for (std::size_t i = 0; i < n; ++i)
		{
			if (is_freed_in_doc[i]) continue;
			const std::uint64_t sid = a["stable_ids"][i].get<std::uint64_t>();
			require_(seen_sid.insert(sid).second,
				"stable_ids[i] duplicate across live slots");
		}

		// Bond endpoint range + field presence.
		const json& bonds_phase1 = store_doc["bonds"];
		require_(bonds_phase1.is_array(), "bonds must be array");
		for (const auto& br : bonds_phase1)
		{
			require_(br.is_object() && br.contains("a") && br.contains("b")
				&& br.contains("order") && br.contains("type") && br.contains("flags"),
				"bonds[i] missing field");
			const std::uint32_t save_a = br["a"].get<std::uint32_t>();
			const std::uint32_t save_b = br["b"].get<std::uint32_t>();
			require_(save_a < n, "bond.a out of [0, size)");
			require_(save_b < n, "bond.b out of [0, size)");
			const int order_i = br["order"].get<int>();
			const int type_i  = br["type"].get<int>();
			require_(order_i >= 0 && order_i <= 255, "bond.order out of [0,255]");
			require_(type_i  >= 0 && type_i  <= 255, "bond.type out of [0,255]");
		}

		// Phase 2 — STATE MUTATION begins. Pre-create Atom handles for
		// every live save-slot. RAII guard (K0.6.5c R8 OPEN-3b + R11
		// fix D): if any subsequent step throws, fully roll back —
		// delete pre-created atoms (if not yet inserted) AND restore
		// the destination System to its pre-load empty state
		// (sys.destroy() + store.clear()). Guard stays armed across
		// the entire mutation window (was disarmed too early, leaving
		// sys partially populated on late-Phase-2 throws). Disarms
		// only at the very end of the try block.
		std::vector<Atom*> atom_by_save_idx(n, nullptr);
		struct RollbackGuard {
			std::vector<Atom*>* v;
			System*             sys;
			bool                inserted_started = false;
			bool                armed            = true;
			~RollbackGuard()
			{
				if (!armed) return;
				if (inserted_started && sys != nullptr)
				{
					// Atoms have been inserted into molecules owned by sys
					// (and/or directly into sys). sys.destroy() drops the
					// Composite subtree which deletes everything via the
					// virtual destruction chain. Then clear the store to
					// drop any column data we wrote.
					sys->destroy();
					sys->getStore().clear();
				}
				else if (v != nullptr)
				{
					// Atoms are still bare heap allocations not yet inserted
					// anywhere — delete them directly.
					for (Atom* a : *v) delete a;
				}
			}
		};
		RollbackGuard guard{&atom_by_save_idx, &sys};

		// v2.1 P4.0: validation window ends here (everything from the
		// post-parse try{ to this point is Phase-1 / molecule-index
		// validation + the destroy/clear + property header reads).
		auto t_after_validate = LoadProfile::now();
		prof.validate_ms = LoadProfile::ms(t_after_parse, t_after_validate);

		for (std::size_t i = 0; i < n; ++i)
		{
			if (!is_freed_in_doc[i])
				atom_by_save_idx[i] = new Atom;
		}
		auto t_after_alloc = LoadProfile::now();
		prof.alloc_ms = LoadProfile::ms(t_after_validate, t_after_alloc);

		// Phase 2 — molecules + atoms.
		guard.inserted_started = true;  // any throw past this point must rollback via sys.destroy + store.clear
		for (const auto& mol_obj : mols)
		{
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
				m->insert(*atom_by_save_idx[save_idx]);
			}
		}

		// Phase 2 — orphan atoms via System base-class insert (hidden-
		// overload note kept in K0.6.5b).
		if (doc.contains("system_atom_indices"))
		{
			for (const auto& idx_json : doc["system_atom_indices"])
			{
				const std::uint32_t save_idx = idx_json.get<std::uint32_t>();
				sys.AtomContainer::insert(*atom_by_save_idx[save_idx]);
			}
		}

		// (R11 fix D): guard stays ARMED across the remaining mutation
		// window (column data, stable_id restore, bond restore). Disarmed
		// only at the very end of the try block when the full load is
		// complete and rollback would be a bug.

		// v2.1 P4.0: Composite-tree insert window ends here.
		auto t_after_insert = LoadProfile::now();
		prof.insert_ms = LoadProfile::ms(t_after_alloc, t_after_insert);

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
				{
					// v2.1 P4.0: accumulate property-restore sub-time
					// (guarded so the 100k chrono calls only happen when
					// profiling is on).
					if (prof.on)
					{
						auto pp0 = LoadProfile::now();
						detail::json_to_properties(*h, &a["properties"][i]);
						prof.props_ms += LoadProfile::ms(pp0, LoadProfile::now());
					}
					else
					{
						detail::json_to_properties(*h, &a["properties"][i]);
					}
				}
			}
		}
		// v2.1 P4.0: column-write window (includes the props_ms subset).
		auto t_after_columns = LoadProfile::now();
		prof.columns_ms = LoadProfile::ms(t_after_insert, t_after_columns);

		// K0.6.5c (Codex R8 OPEN-5a): restore System-level stable_ids
		// via the slot map. Store-only loader does this via the private
		// restore_stable_ids_for_load_; the System loader has to
		// translate save_idx -> fresh_idx first, then write each
		// stable_id with set_stable_id_for_load_... wait, that path was
		// privatized in K0.6.3b. We use the same approach as the store
		// loader: build a vector<StableId> indexed by FRESH store index
		// + call the friend helper. The store loader friended the
		// store-only helper (detail::json_obj_to_store); we add a
		// dedicated System-loader path via the store's existing
		// public-but-validated bulk-restore.
		{
			std::vector<MoleculeStore::StableId> sid_vec(store.size(), 0);
			// Default to fresh-allocation ids for slots NOT in the doc
			// (any freed slots that got allocated post-load won't have
			// a doc-source id; keep whatever allocate_atom assigned).
			for (std::size_t i = 0; i < store.size(); ++i)
				sid_vec[i] = store.stable_id(static_cast<MoleculeStore::Index>(i));
			// Overwrite from the doc using slot_map.
			for (std::size_t i = 0; i < n; ++i)
			{
				if (!slot_live[i]) continue;
				const std::uint32_t fi = slot_map[i];
				if (fi < sid_vec.size())
					sid_vec[fi] = a["stable_ids"][i].get<std::uint64_t>();
			}
			// Friend access: detail::json_obj_to_store is already friended
			// by moleculeStore.h. systemJson.C is a different TU but the
			// helper restore_stable_ids_for_load_ is private. Workaround:
			// invoke it through a small store-internal path. For K0.6.5c
			// we expose a public wrapper that takes a fresh-index-keyed
			// vector and runs the same validation.
			store.restore_stable_ids_for_load_(sid_vec);
		}
		// v2.1 P4.0: stable-id restore window.
		auto t_after_stableid = LoadProfile::now();
		prof.stableid_ms = LoadProfile::ms(t_after_columns, t_after_stableid);

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

			// v2.1 P4.2 (V21-BOND-PROPERTY-JSON + D43): reconstruct the
			// Atom-side Bond* graph via createBond — which also creates
			// the store BondRecord AND wires bond_back_ptr — then restore
			// order, type, and the Bond's PropertyManager bag. Pre-P4.2
			// the loader called store.add_bond directly, leaving
			// atom.countBonds()==0 post-load (the D43 fidelity gap);
			// createBond closes that AND gives the bond a Bond* to hold
			// its restored properties.
			//
			// Note: createBond enforces one bond per atom pair (Atom's
			// inherent model). If the saved store held multiple records
			// between the same pair (a store-level multigraph, not
			// expressible via the Atom API), they collapse to one here.
			// The store-only loader (json_obj_to_store) preserves the
			// multigraph; the System loader is Atom-consistent.
			Atom* atom_a = atom_by_save_idx[save_a];
			Atom* atom_b = atom_by_save_idx[save_b];
			Bond* bond = atom_a->createBond(*atom_b);
			if (bond != nullptr)
			{
				bond->setOrder(static_cast<Bond::Order>(order_i));
				bond->setType(static_cast<Bond::Type>(type_i));
				if (br.contains("properties"))
					detail::json_to_properties(*bond, &br["properties"]);
			}
		}

		// v2.1 P4.0: bond-restore window.
		prof.bonds_ms = LoadProfile::ms(t_after_stableid, LoadProfile::now());
		prof.report();

		// (R11 fix D): full mutation succeeded — disarm the rollback
		// guard. Any throw before this point triggers full rollback to
		// the pre-load empty System.
		guard.armed = false;
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
