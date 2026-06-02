// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/bondHandle.h>
#include <BALL/KERNEL/atomHandle.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <unordered_set>
///////////////////////////

START_TEST(BondHandle)

using namespace BALL;

/////////////////////////////////////////////////////////////
// H3a.1: bond stable-ids in MoleculeStore (D-H3.8)
/////////////////////////////////////////////////////////////

CHECK(MoleculeStore bond stable-ids are unique + ABA-safe across remove/re-add)
	MoleculeStore store;
	MoleculeStore::Index a0 = store.allocate_atom();
	MoleculeStore::Index a1 = store.allocate_atom();
	MoleculeStore::Index a2 = store.allocate_atom();

	std::uint32_t b0 = store.add_bond(a0, a1, 1, 1);
	std::uint32_t b1 = store.add_bond(a1, a2, 2, 1);
	MoleculeStore::StableId sid0 = store.bond_stable_id(b0);
	MoleculeStore::StableId sid1 = store.bond_stable_id(b1);
	TEST_NOT_EQUAL(sid0, sid1)                // unique per live bond
	TEST_NOT_EQUAL(sid0, (MoleculeStore::StableId)0)

	// Tombstone b0 and re-add: the free-list hands back the same slot but a
	// FRESH stable id (ABA-safe).
	store.remove_bond(b0);
	TEST_EQUAL(store.is_bond_dead(b0), true)
	std::uint32_t b2 = store.add_bond(a0, a2, 1, 1);
	TEST_EQUAL(b2, b0)                        // free-list reused the slot
	TEST_NOT_EQUAL(store.bond_stable_id(b2), sid0)   // but a new identity
RESULT

/////////////////////////////////////////////////////////////
// H3a.2: BondHandle
/////////////////////////////////////////////////////////////

CHECK(BondHandle() null handle)
	BondHandle nullh;
	TEST_EQUAL((bool)nullh, false)
	TEST_EQUAL(nullh.isNull(), true)
	TEST_EQUAL(nullh.isValid(), false)
	TEST_EQUAL(nullh.getBond() == (Bond*)nullptr, true)
	TEST_EQUAL((bool)nullh.getFirstAtom(), false)
RESULT

CHECK(BondHandle bind + endpoints + order/type)
	MoleculeStore store;
	MoleculeStore::Index a0 = store.allocate_atom();
	MoleculeStore::Index a1 = store.allocate_atom();
	std::uint32_t b0 = store.add_bond(a0, a1, 2 /*DOUBLE*/, 1 /*COVALENT*/);

	BondHandle bh(store, b0);
	TEST_EQUAL((bool)bh, true)
	TEST_EQUAL(bh.isValid(), true)
	TEST_EQUAL(bh.getBondIndex(), b0)
	TEST_EQUAL(bh.getStableId(), store.bond_stable_id(b0))
	TEST_EQUAL(bh.getOrder(), 2)
	TEST_EQUAL(bh.getType(), 1)

	AtomHandle f = bh.getFirstAtom();
	AtomHandle s = bh.getSecondAtom();
	TEST_EQUAL(f.getStoreIndex(), a0)
	TEST_EQUAL(s.getStoreIndex(), a1)
	// getPartner: the endpoint that is NOT the given atom.
	TEST_EQUAL(bh.getPartner(f).getStoreIndex(), a1)
	TEST_EQUAL(bh.getPartner(s).getStoreIndex(), a0)
	// a non-endpoint atom -> null partner.
	MoleculeStore::Index a2 = store.allocate_atom();
	TEST_EQUAL((bool)bh.getPartner(AtomHandle(store, a2)), false)

	// H3a-CR fix (item 2): getPartner must reject stale + cross-store atom
	// handles even when the bare slot index happens to match an endpoint.
	AtomHandle stale_h(store, a0);
	store.release_atom(a0);                              // stale_h now invalid
	TEST_EQUAL(stale_h.isValid(), false)
	TEST_EQUAL((bool)bh.getPartner(stale_h), false)      // stale -> rejected

	MoleculeStore store2;
	MoleculeStore::Index a0_other = store2.allocate_atom();
	AtomHandle cross(store2, a0_other);                  // different store
	TEST_EQUAL((bool)bh.getPartner(cross), false)        // cross-store -> rejected
RESULT

CHECK(BondHandle isValid()/getBond() ABA-safe across remove + re-add)
	MoleculeStore store;
	MoleculeStore::Index a0 = store.allocate_atom();
	MoleculeStore::Index a1 = store.allocate_atom();
	std::uint32_t b0 = store.add_bond(a0, a1, 1, 1);
	BondHandle bh(store, b0);
	TEST_EQUAL(bh.isValid(), true)

	store.remove_bond(b0);
	TEST_EQUAL(bh.isValid(), false)                 // tombstoned -> stale
	TEST_EQUAL(bh.getBond() == (Bond*)nullptr, true)

	std::uint32_t b1 = store.add_bond(a0, a1, 1, 1); // reuses slot, fresh id
	BondHandle bh2(store, b1);
	TEST_EQUAL(bh2.isValid(), true)
	TEST_EQUAL(bh.isValid(), false)                 // old handle stays stale (ABA)
	if (b1 == b0) TEST_NOT_EQUAL(bh.getStableId(), bh2.getStableId())
RESULT

CHECK(BondHandle identity equality + stable-id hashing/ordering)
	MoleculeStore store;
	MoleculeStore::Index a0 = store.allocate_atom();
	MoleculeStore::Index a1 = store.allocate_atom();
	MoleculeStore::Index a2 = store.allocate_atom();
	std::uint32_t b0 = store.add_bond(a0, a1, 1, 1);
	std::uint32_t b1 = store.add_bond(a1, a2, 1, 1);
	BondHandle h0(store, b0), h0_again(store, b0), h1(store, b1);
	TEST_EQUAL(h0 == h0_again, true)
	TEST_EQUAL(h0 != h1, true)
	// D-H3.8: drop-in stable-id key for re-keying Bond*-identity containers.
	std::unordered_set<BondHandle> seen;
	seen.insert(h0); seen.insert(h0_again); seen.insert(h1);
	TEST_EQUAL(seen.size(), 2)                      // h0 == h0_again collapses
	TEST_EQUAL((h0 < h1) != (h1 < h0), true)        // strict-weak ordering
RESULT

/////////////////////////////////////////////////////////////
END_TEST
