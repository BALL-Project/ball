// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — JSON persistence for System + Molecule (K0.6.5).
// Spec: .planning/v2.x/K0.6-SUBPHASES.md
//
// Builds on K0.6.1-K0.6.3b's MoleculeStore JSON layer; emits a
// containing document with the System's name + the embedded
// MoleculeStore block + a molecules[] array carrying each Molecule's
// name and its atom-index list (store indices). Per maintainer scope
// 2026-05-18: Chain / Residue / Fragment / SecondaryStructure
// sub-Composites are NOT serialized; the kernel-level System tree is
// just System -> Molecules -> Atoms.
//

#ifndef BALL_KERNEL_SYSTEMJSON_H
#define BALL_KERNEL_SYSTEMJSON_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_KERNEL_MOLECULESTOREJSON_H
# include <BALL/KERNEL/moleculeStoreJson.h>
#endif

#include <iosfwd>

namespace BALL
{
	class System;

	/** K0.6.5 System JSON schema version emitted by saveSystemJSON.
			Independent of MOLECULE_STORE_JSON_VERSION because the System
			schema can evolve faster than the inner store schema. Same
			major / minor policy as the store: major bump = incompatible,
			minor = additive.
	*/
	constexpr int SYSTEM_JSON_VERSION       = 1;   // major
	constexpr int SYSTEM_JSON_VERSION_MINOR = 0;

	/** Write `sys` to `os` as a JSON document with top-level
			"document_type": "System".

			Layout:
				{
					"document_type": "System",
					"format_version": 1,
					"format_minor":   0,
					"name":           "<system name>",
					"store":          { ...MoleculeStore JSON body... },
					"molecules": [
						{ "name": "...", "atom_indices": [0, 1, 2, ...] },
						...
					]
				}

			atom_indices reference slots in the embedded store. Atoms not
			belonging to any Molecule (sitting directly on the System)
			are NOT enumerated in any molecule entry but DO appear in the
			store's columns — the reader leaves them as System-level
			children after load.
	*/
	BALL_EXPORT void saveSystemJSON(const System&    sys,
	                                std::ostream&    os,
	                                int              indent = -1,
	                                JsonFloatFormat  floats = JsonFloatFormat::DECIMAL);

	/** Populate `sys` from a JSON document read from `is`. The system
			is cleared first (all child Molecules + Atoms removed; the
			underlying store wiped via MoleculeStore::clear). After load,
			sys.countMolecules() and sys.countAtoms() reflect the doc.

			Throws Exception::ParseError on the same conditions as
			loadStoreJSON (schema violations, version mismatch, etc.),
			plus: missing "molecules" key, atom_indices referencing slots
			out of [0, store.size()) or freed slots.
	*/
	BALL_EXPORT void loadSystemJSON(System&        sys,
	                                std::istream&  is);

} // namespace BALL

#endif // BALL_KERNEL_SYSTEMJSON_H
