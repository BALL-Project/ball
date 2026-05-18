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
			Bumped whenever the on-disk layout changes incompatibly.
	*/
	constexpr int MOLECULE_STORE_JSON_VERSION = 1;

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
	                               int                  indent = -1);

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

} // namespace BALL

#endif // BALL_KERNEL_MOLECULESTOREJSON_H
