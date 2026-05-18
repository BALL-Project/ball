// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — K0.7.1 sizeof measurements. Pins the K0 handle sizes so
// future kernel changes can't silently regress the memory budget;
// reports the per-atom column row cost for D13 (typical ≤160 B/atom)
// follow-up work in K0.7.3.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <iostream>
///////////////////////////

START_TEST(Sizeof)

using namespace BALL;

CHECK(K0.7.1 sizeof Atom handle pinned <= 512 bytes; surprise documented)
	// Atom holds (store*, store_idx, generation, number_of_bonds,
	// bond_[8], interactions*) — about 80-90 B of own data after the
	// K0.3b.LATER payload deletion. The rest is base-class overhead
	// from D2/D3/D4's "thin stub" compromise: Composite (parent +
	// children pointers, modification stamps, RTTI string),
	// PropertyManager (named_properties_ vector + BitVector),
	// Selectable (selection bit), plus virtual table.
	// Initial measurement on Darwin arm64 release: 360 B. The 512 B
	// upper bound here pins against silent regression; tighter
	// budgeting + the option to fully delete D2/D3/D4 (currently
	// deferred to v2.1) is what would bring this down.
	std::cerr << "  [K0.7.1] sizeof(Atom)           = " << sizeof(Atom) << " B (observed: 360 on Darwin arm64)" << std::endl;
	TEST_EQUAL(sizeof(Atom) <= 512, true)
RESULT

CHECK(K0.7.1 sizeof Bond handle pinned <= 512 bytes)
	// Bond similar story: own data ~32 B (two Atom*, two store_idx,
	// order/type byte, MoleculeStore*, bond_record_idx); rest is
	// Composite + PropertyManager + Selectable overhead.
	// Initial measurement on Darwin arm64 release: 288 B.
	std::cerr << "  [K0.7.1] sizeof(Bond)           = " << sizeof(Bond) << " B (observed: 288 on Darwin arm64)" << std::endl;
	TEST_EQUAL(sizeof(Bond) <= 512, true)
RESULT

CHECK(K0.7.1 sizeof BondRecord matches the on-disk 12 B contract)
	// BondRecord is the SoA bond table row; per the K0 spec it MUST be
	// 12 bytes (uint32 + uint32 + uint8 + uint8 + uint16). Padding could
	// silently bump it.
	std::cerr << "  [K0.7.1] sizeof(BondRecord)     = " << sizeof(BondRecord) << " B" << std::endl;
	TEST_EQUAL(sizeof(BondRecord), 12u)
RESULT

CHECK(K0.7.1 per-atom column row cost report)
	// Sum of the per-atom store column entry sizes. D13 budget is
	// 160 B/atom typical; this number is the SoA-only contribution
	// — handle overhead from Atom* + Composite/PropertyManager is
	// added by K0.7.3 to get the full per-atom footprint.
	const std::size_t bytes_per_atom_columns =
		  sizeof(Vector3)        // positions
		+ sizeof(Vector3)        // velocities
		+ sizeof(Vector3)        // forces
		+ sizeof(float)          // charges
		+ sizeof(float)          // radii
		+ sizeof(short)          // atom_types
		+ sizeof(short)          // formal_charges
		+ sizeof(std::uint8_t)   // element_indices
		+ sizeof(std::uint8_t)   // selection
		+ sizeof(std::uint32_t)  // name_offsets
		+ sizeof(std::uint32_t)  // type_name_offsets
		+ sizeof(String) * 2     // name_strings + type_name_strings
		+ sizeof(std::uint64_t)  // stable_ids
		+ sizeof(void*)          // back_ptr
		+ sizeof(std::uint8_t);  // is_freed
	std::cerr << "  [K0.7.1] per-atom SoA columns  = " << bytes_per_atom_columns << " B" << std::endl;
	std::cerr << "  [K0.7.1] D13 budget (target)   = 160 B/atom (column+handle+bases)" << std::endl;
	TEST_EQUAL(bytes_per_atom_columns < 160u, true)
RESULT

CHECK(K0.7.1 sizeof MoleculeStore + System + Molecule fixed overhead)
	std::cerr << "  [K0.7.1] sizeof(MoleculeStore)  = " << sizeof(MoleculeStore) << " B" << std::endl;
	std::cerr << "  [K0.7.1] sizeof(System)         = " << sizeof(System)        << " B" << std::endl;
	std::cerr << "  [K0.7.1] sizeof(Molecule)       = " << sizeof(Molecule)      << " B" << std::endl;
	// Per-system / per-molecule overhead is amortised across the atoms
	// they hold; no tight gate here, just a report.
	TEST_EQUAL(true, true)
RESULT

END_TEST
