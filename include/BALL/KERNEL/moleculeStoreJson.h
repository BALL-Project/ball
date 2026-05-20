// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — JSON persistence for MoleculeStore (K0.6).
// Spec: .planning/v2.x/K0.6-SUBPHASES.md
// Sub-phase: K0.6.1 (writer). Reader lands in K0.6.2.
//
// Free functions rather than members so the nlohmann::json header
// (~900KB) stays out of every TU that pulls in moleculeStore.h.
//

#ifndef BALL_KERNEL_MOLECULESTOREJSON_H
#define BALL_KERNEL_MOLECULESTOREJSON_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <iosfwd>

namespace BALL
{
	class MoleculeStore;

	/** K0.6 JSON schema version emitted by saveStoreJSON.

			K0.6.3 split into major/minor:
				- MAJOR bump on incompatible layout change (rename/remove
					of an existing column, change of a field type's
					semantics). Readers REJECT documents with a higher
					major than they support.
				- MINOR bump on backward-compatible additions (new
					optional top-level keys, new optional bond fields).
					Readers SILENTLY ACCEPT a higher minor and ignore
					unknown keys; older readers don't see new data but
					still load the rest.
			On-disk doc carries `format_version` (the major, kept for
			K0.6.1/.2 compat: docs written by K0.6.1 had this == 1, no
			minor) and `format_minor` (added in K0.6.3, default 0).
	*/
	constexpr int MOLECULE_STORE_JSON_VERSION       = 1;   // major
	constexpr int MOLECULE_STORE_JSON_VERSION_MINOR = 2;   // K0.6.3=1; v2.1 P4.2 bond "properties"=2

	/** Float encoding selector for saveStoreJSON.
			- DECIMAL (default): the nlohmann::json default — readable,
				round-trips to a decimal-correct float, but not guaranteed
				bit-exact across reader/writer/compiler combinations.
			- BIT_EXACT_HEX: every float is emitted as a "0x%08x" string
				of the IEEE-754 bit pattern. Bit-exact for normals,
				subnormals, +/-0, +/-infinity, and signaling/quiet NaNs.
				Use this for MD trajectory replay or any regression
				baseline where decimal drift matters. Reader auto-detects
				per-element (string -> hex decode; number -> decimal).
	*/
	enum class JsonFloatFormat
	{
		DECIMAL,
		BIT_EXACT_HEX
	};

	/** Write `store` to `os` as a JSON document.

			Layout (per K0.6-SUBPHASES.md): per-column SoA arrays
			(positions, velocities, forces, charges, radii, atom_types,
			formal_charges, element_indices, selection, names,
			type_names, stable_ids) of length store.size(), plus a
			parallel is_freed[] mask and a bond table (array of
			{a,b,order,type,flags} records, dead bonds skipped). Top-
			level keys: format_version (= MOLECULE_STORE_JSON_VERSION),
			size, live_atom_count, atoms{...columns...}, bonds[...].

			`indent`: nlohmann::json::dump indent argument; pass -1 for
			compact (single-line) output; 2 for human-readable.
	*/
	BALL_EXPORT void saveStoreJSON(const MoleculeStore& store,
	                               std::ostream&        os,
	                               int                  indent = -1,
	                               JsonFloatFormat      floats = JsonFloatFormat::DECIMAL);

	/** Populate `store` from a JSON document read from `is` (K0.6.2).

			Expects the schema written by saveStoreJSON (this version's
			MOLECULE_STORE_JSON_VERSION). The store is cleared first: every
			existing slot is released, every bond removed. After reading,
			the store has size() == document.size, live_atom_count ==
			document.live_atom_count, bond_count == document.bonds.size().

			Throws Exception::ParseError on:
				- malformed JSON
				- missing required top-level keys
				- format_version > MOLECULE_STORE_JSON_VERSION (no forward-
					compat policy yet — K0.6.3 may relax this for minor bumps)
				- column-length mismatch vs document.size
	*/
	BALL_EXPORT void loadStoreJSON(MoleculeStore& store,
	                               std::istream&  is);

	/** K0.6.5 internal helpers — emit / consume the MoleculeStore JSON
			as a nested nlohmann::json object instead of via std::ostream.
			Lets the System-level writer/reader embed the store under the
			"store" key without serialising to a string first. Declared in
			detail:: rather than as private members so System's TU can use
			them without making MoleculeStoreJson a friend.

			Forward-declared by void* to keep nlohmann::json out of the
			public header; the .C casts back. NOT for caller use — only
			moleculeStoreJson.C + systemJson.C call these.
	*/
	namespace detail {
		BALL_EXPORT void store_to_json_obj(const MoleculeStore& s,
		                                   void* json_out,
		                                   JsonFloatFormat ff);
		BALL_EXPORT void json_obj_to_store(MoleculeStore& s,
		                                   const void* json_in);
	}

} // namespace BALL

#endif // BALL_KERNEL_MOLECULESTOREJSON_H
