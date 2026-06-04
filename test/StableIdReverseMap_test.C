// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H3c Phase 0 (D-H3c.0-R3) test: stable_id -> idx reverse-map
// correctness across allocate / release / recycle / load / bond
// add+remove / remove_bonds_between (private path).
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomHandle.h>
#include <BALL/KERNEL/bondHandle.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
///////////////////////////

START_TEST(StableIdReverseMap)

using namespace BALL;

/////////////////////////////////////////////////////////////
// 1. Allocation populates the map; lookup returns the slot.
/////////////////////////////////////////////////////////////
CHECK(allocate_atom inserts into atom_sid_to_idx_)
	System sys;
	Molecule* m = new Molecule;
	Atom* a = new Atom;
	m->insert(*a);
	sys.insert(*m);

	MoleculeStore* store = a->getStore();
	TEST_NOT_EQUAL(store, 0)
	MoleculeStore::StableId sid = store->stable_id(a->getStoreIndex());
	TEST_EQUAL(store->atom_idx_by_stable_id(sid), a->getStoreIndex())

	// AtomHandle::bySid round-trips.
	AtomHandle h = AtomHandle::bySid(*store, sid);
	TEST_EQUAL((bool)h, true)
	TEST_EQUAL(h.getStoreIndex(), a->getStoreIndex())
	TEST_EQUAL(h.getStableId(), sid)
RESULT

/////////////////////////////////////////////////////////////
// 2. release_atom erases; lookup of the old sid returns UNKNOWN.
/////////////////////////////////////////////////////////////
CHECK(release_atom erases atom_sid_to_idx_ entry)
	Atom* a = new Atom;
	MoleculeStore* store = a->getStore();
	MoleculeStore::StableId sid = store->stable_id(a->getStoreIndex());
	TEST_EQUAL(store->atom_idx_by_stable_id(sid), a->getStoreIndex())

	delete a;   // -> release_atom

	TEST_EQUAL(store->atom_idx_by_stable_id(sid), MoleculeStore::UNKNOWN_STABLE_ID)
	TEST_EQUAL((bool)AtomHandle::bySid(*store, sid), false)
RESULT

/////////////////////////////////////////////////////////////
// 3. ABA-recycle: free a slot, allocate a new atom at the same idx;
//    old sid still returns UNKNOWN; new sid resolves to the slot.
/////////////////////////////////////////////////////////////
CHECK(ABA recycle -- old sid stays unknown / new sid resolves)
	Atom* a = new Atom;
	MoleculeStore* store = a->getStore();
	std::uint32_t old_idx = a->getStoreIndex();
	MoleculeStore::StableId old_sid = store->stable_id(old_idx);

	delete a;

	Atom* b = new Atom;   // LIFO free-list -> same idx
	TEST_EQUAL(b->getStoreIndex(), old_idx)
	MoleculeStore::StableId new_sid = store->stable_id(b->getStoreIndex());
	TEST_NOT_EQUAL(new_sid, old_sid)   // fresh sid (next_stable_id_alloc_)

	// Old sid stays unknown (NOT re-resolved to the recycled slot).
	TEST_EQUAL(store->atom_idx_by_stable_id(old_sid), MoleculeStore::UNKNOWN_STABLE_ID)
	// New sid resolves.
	TEST_EQUAL(store->atom_idx_by_stable_id(new_sid), old_idx)
	delete b;
RESULT

/////////////////////////////////////////////////////////////
// 4. add_bond inserts; remove_bond erases.
/////////////////////////////////////////////////////////////
CHECK(add_bond / remove_bond round-trip bond_sid_to_idx_)
	System sys;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom; a1->setName("A1");
	Atom* a2 = new Atom; a2->setName("A2");
	m->insert(*a1); m->insert(*a2);
	sys.insert(*m);

	a1->createBond(*a2);
	MoleculeStore* store = a1->getStore();

	// Get the bond stable id via the AtomHandle::getBond bridge.
	AtomHandle ah(*store, a1->getStoreIndex());
	BondHandle bh = ah.getBond(0);
	TEST_EQUAL((bool)bh, true)
	MoleculeStore::StableId bond_sid = bh.getStableId();
	TEST_NOT_EQUAL(bond_sid, MoleculeStore::StableId(0))
	TEST_NOT_EQUAL(store->bond_idx_by_stable_id(bond_sid), ~std::uint32_t(0))

	a1->destroyBond(*a2);   // -> remove_bond_unsafe_ via public path
	TEST_EQUAL(store->bond_idx_by_stable_id(bond_sid), ~std::uint32_t(0))
RESULT

/////////////////////////////////////////////////////////////
// 5. DR2 FLAW 2 fix: remove_bonds_between (which uses
//    remove_bond_unsafe_ DIRECTLY, bypassing public remove_bond)
//    also erases the map entry.
/////////////////////////////////////////////////////////////
CHECK(remove_bonds_between erases bond_sid_to_idx_ via unsafe path)
	System sys;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom; Atom* a2 = new Atom;
	m->insert(*a1); m->insert(*a2);
	sys.insert(*m);

	a1->createBond(*a2);
	MoleculeStore* store = a1->getStore();
	AtomHandle ah(*store, a1->getStoreIndex());
	BondHandle bh = ah.getBond(0);
	MoleculeStore::StableId bond_sid = bh.getStableId();
	TEST_NOT_EQUAL(store->bond_idx_by_stable_id(bond_sid), ~std::uint32_t(0))

	// remove_bonds_between uses remove_bond_unsafe_ directly.
	store->remove_bonds_between(a1->getStoreIndex(), a2->getStoreIndex());

	// DR2 FLAW 2 fix verification: the bond sid is NOT resolvable.
	// Without the unsafe-path hook this would have leaked.
	TEST_EQUAL(store->bond_idx_by_stable_id(bond_sid), ~std::uint32_t(0))
RESULT

/////////////////////////////////////////////////////////////
END_TEST
