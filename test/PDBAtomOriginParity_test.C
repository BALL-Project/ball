// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H3a.3b parity test (D-H3.15-R3). Verifies the PDB-origin marker
// (`origin_flags_` bit 0 / AtomHandle::isPDBOrigin) at every lifecycle
// boundary the design promises:
//   ctor / adoption (single + subtree) / release+recycle / detached
//   rebind / cross-store move / swap (typed + mixed-type) / deep-copy /
//   base-assignment from non-PDB / JSON round-trip
//
// The invariant (D-H3.13-R3): for every store slot i where
//   back_ptr_[i] != nullptr  =>  isKindOf<PDBAtom>(back_ptr_[i]) ==
//                                 (origin_flags_[i] & 0x01) != 0
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomHandle.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/systemJson.h>

#include <sstream>
///////////////////////////

START_TEST(PDBAtomOriginParity)

using namespace BALL;

// Helper: read the store-side bit for the slot a v0 atom is bound to.
auto storeBitOf = [](const Atom& a) -> bool {
	if (a.getStore() == 0) return false;
	return (a.getStore()->origin_flags(a.getStoreIndex()) & 0x01u) != 0u;
};

/////////////////////////////////////////////////////////////
// 1. Orphan ctor: PDBAtom -> bit = 1.
/////////////////////////////////////////////////////////////
CHECK(orphan PDBAtom ctor sets origin bit)
	PDBAtom p;
	TEST_EQUAL(storeBitOf(p), true)
RESULT

/////////////////////////////////////////////////////////////
// 2. Plain Atom ctor: bit = 0.
/////////////////////////////////////////////////////////////
CHECK(orphan plain Atom ctor leaves origin bit clear)
	Atom a;
	TEST_EQUAL(storeBitOf(a), false)
RESULT

/////////////////////////////////////////////////////////////
// 3. System adoption (single atom): src slot freed (bit=0); dst slot
//    bit=1.
/////////////////////////////////////////////////////////////
CHECK(System adoption preserves PDB origin across stores)
	PDBAtom* p = new PDBAtom;
	const std::uint32_t orphan_idx = p->getStoreIndex();
	MoleculeStore* orphan_store = p->getStore();
	TEST_EQUAL((orphan_store->origin_flags(orphan_idx) & 0x01u) != 0u, true)

	System sys;
	Molecule* m = new Molecule;
	m->insert(*p);
	sys.insert(*m);

	// After adoption: p is bound to sys's store.
	TEST_NOT_EQUAL(p->getStore(), orphan_store)
	TEST_EQUAL(storeBitOf(*p), true)
	// Orphan slot now freed -> bit reset to 0 by release_atom.
	TEST_EQUAL((orphan_store->origin_flags(orphan_idx) & 0x01u) != 0u, false)
RESULT

/////////////////////////////////////////////////////////////
// 4. System adoption (subtree): nested PDBAtom in Protein/Chain/Residue.
/////////////////////////////////////////////////////////////
CHECK(System adoption preserves PDB origin across subtree)
	System sys;
	Protein* prot = new Protein; prot->setName("P");
	Chain*   ch   = new Chain;   ch->setName("A");
	Residue* r    = new Residue; r->setName("ALA");
	PDBAtom* p1   = new PDBAtom; p1->setName("N");
	PDBAtom* p2   = new PDBAtom; p2->setName("CA");
	r->insert(*p1); r->insert(*p2);
	ch->insert(*r); prot->insert(*ch);
	sys.insert(*prot);

	TEST_EQUAL(storeBitOf(*p1), true)
	TEST_EQUAL(storeBitOf(*p2), true)
RESULT

/////////////////////////////////////////////////////////////
// 5. Release + recycle: free PDBAtom slot, allocate Atom at same idx
//    -> bit reset to 0.
/////////////////////////////////////////////////////////////
CHECK(release + recycle clears origin bit)
	// In the orphan store: allocate then release a PDBAtom, then a plain
	// Atom should reuse the freed slot (free-list) with bit=0.
	MoleculeStore& os = MoleculeStore::orphanStore();
	PDBAtom* p = new PDBAtom;
	const std::uint32_t freed_idx = p->getStoreIndex();
	TEST_EQUAL((os.origin_flags(freed_idx) & 0x01u) != 0u, true)
	delete p;
	TEST_EQUAL((os.origin_flags(freed_idx) & 0x01u) != 0u, false)  // release clears

	// A plain Atom allocated immediately afterward should reuse the slot
	// (LIFO free-list) and stay bit=0.
	Atom a;
	TEST_EQUAL(a.getStoreIndex(), freed_idx)
	TEST_EQUAL(storeBitOf(a), false)
RESULT

/////////////////////////////////////////////////////////////
// 6. Detached rebind via ensureStoreBinding_: a PDBAtom whose System is
//    destroyed, then mutated, rebinds to a NEW orphan slot using
//    origin_hint_ -- bit stays 1.
/////////////////////////////////////////////////////////////
CHECK(detached rebind after System teardown carries origin)
	PDBAtom* p = new PDBAtom;
	p->setAutoDeletable(false);  // survive ~System

	{
		System sys;
		Molecule* m = new Molecule;
		m->insert(*p);
		sys.insert(*m);
		TEST_EQUAL(storeBitOf(*p), true)
	}
	// sys destroyed -> p's store binding nulled. Trigger a rebind via a
	// mutation that goes through ensureStoreBinding_.
	p->setCharge(0.5f);
	TEST_NOT_EQUAL(p->getStore(), 0)
	TEST_EQUAL(storeBitOf(*p), true)
	delete p;
RESULT

/////////////////////////////////////////////////////////////
// 7. Cross-store move: PDBAtom in System A adopted by System B.
/////////////////////////////////////////////////////////////
CHECK(cross-store move preserves PDB origin)
	System sysA;
	Molecule* mA = new Molecule;
	PDBAtom* p = new PDBAtom; p->setAutoDeletable(false);
	mA->insert(*p);
	sysA.insert(*mA);

	TEST_EQUAL(storeBitOf(*p), true)
	MoleculeStore* storeA = p->getStore();

	// Move into sysB by inserting the atom directly into a new System's
	// Molecule. System::adopt is the migration path.
	System sysB;
	Molecule* mB = new Molecule;
	mB->insert(*p);
	sysB.insert(*mB);

	TEST_NOT_EQUAL(p->getStore(), storeA)
	TEST_EQUAL(storeBitOf(*p), true)
RESULT

/////////////////////////////////////////////////////////////
// 8. PDBAtom::swap(PDBAtom&): bits stay correct on both.
/////////////////////////////////////////////////////////////
CHECK(PDBAtom::swap preserves origin bit on both)
	PDBAtom a, b;
	a.setName("A");
	b.setName("B");
	const std::uint32_t aIdx = a.getStoreIndex();
	const std::uint32_t bIdx = b.getStoreIndex();
	a.swap(b);
	// Both atoms still bit=1; the slots themselves did not move.
	MoleculeStore& os = MoleculeStore::orphanStore();
	TEST_EQUAL((os.origin_flags(aIdx) & 0x01u) != 0u, true)
	TEST_EQUAL((os.origin_flags(bIdx) & 0x01u) != 0u, true)
RESULT

/////////////////////////////////////////////////////////////
// 9. Atom::swap(Atom&) mixed-type: payload swaps; origin bit stays with
//    SLOT identity (back_ptr), NOT with payload. (DR4 Fix 6.)
/////////////////////////////////////////////////////////////
CHECK(Atom::swap on mixed PDBAtom + plain Atom keeps origin with slot)
	PDBAtom p; p.setName("P");
	Atom    q; q.setName("Q");
	const std::uint32_t pIdx = p.getStoreIndex();
	const std::uint32_t qIdx = q.getStoreIndex();
	TEST_NOT_EQUAL(pIdx, qIdx)

	// Atom::swap on PDBAtom and plain Atom (cross-cast to base) swaps
	// store-backed payload columns. The store slots themselves stay put.
	// The PDBAtom object (back_ptr_[pIdx]) is still a PDBAtom; the plain
	// Atom (back_ptr_[qIdx]) is still a plain Atom. So:
	//   bit at pIdx should remain 1 (slot owns a PDBAtom)
	//   bit at qIdx should remain 0 (slot owns a plain Atom)
	p.Atom::swap(q);
	MoleculeStore& os = MoleculeStore::orphanStore();
	TEST_EQUAL((os.origin_flags(pIdx) & 0x01u) != 0u, true)
	TEST_EQUAL((os.origin_flags(qIdx) & 0x01u) != 0u, false)
RESULT

/////////////////////////////////////////////////////////////
// 10. Deep copy: PDBAtom b(a, true) -- new slot bit=1.
/////////////////////////////////////////////////////////////
CHECK(PDBAtom deep copy marks new slot as PDB origin)
	PDBAtom a; a.setName("A");
	PDBAtom b(a, true);
	TEST_NOT_EQUAL(a.getStoreIndex(), b.getStoreIndex())
	TEST_EQUAL(storeBitOf(b), true)
RESULT

/////////////////////////////////////////////////////////////
// 11. Base-assignment from plain Atom into PDBAtom subobject: bit
//     stays 1. (DR4 Fix 7.) The slot's class-of-origin is independent
//     of any payload assigned later.
/////////////////////////////////////////////////////////////
CHECK(base-assign plain Atom into PDBAtom keeps origin bit)
	PDBAtom p; p.setName("P");
	Atom    q; q.setName("Q");
	const std::uint32_t pIdx = p.getStoreIndex();

	// Slice down to the base class and assign a non-PDB payload.
	static_cast<Atom&>(p) = q;

	// p's slot is still owned by a PDBAtom object (back_ptr unchanged)
	// so the bit must still be 1.
	MoleculeStore& os = MoleculeStore::orphanStore();
	TEST_EQUAL((os.origin_flags(pIdx) & 0x01u) != 0u, true)
RESULT

/////////////////////////////////////////////////////////////
// 12. Save+Load JSON round-trip: a System containing a PDBAtom saved
//     and loaded back has a PDBAtom (origin bit=1) at the same atom
//     index.
/////////////////////////////////////////////////////////////
CHECK(System JSON round-trip preserves PDB origin)
	System sys;
	Molecule* m = new Molecule; m->setName("M");
	PDBAtom* p = new PDBAtom; p->setName("CA");
	Atom*    a = new Atom;    a->setName("X");
	m->insert(*p);
	m->insert(*a);
	sys.insert(*m);

	std::stringstream ss;
	saveSystemJSON(sys, ss);

	System sys2;
	loadSystemJSON(sys2, ss);

	// Inspect every atom under sys2; expect ONE PDB-origin atom (named
	// "CA") and ONE plain Atom (named "X").
	int pdb_count = 0;
	int plain_count = 0;
	for (AtomIterator it = sys2.beginAtom(); +it; ++it)
	{
		if (RTTI::isKindOf<PDBAtom>(&*it))
		{
			++pdb_count;
			TEST_EQUAL(storeBitOf(*it), true)
		}
		else
		{
			++plain_count;
			TEST_EQUAL(storeBitOf(*it), false)
		}
	}
	TEST_EQUAL(pdb_count, 1)
	TEST_EQUAL(plain_count, 1)
RESULT

/////////////////////////////////////////////////////////////
// 13. Dual-existence parity (D-H3.13-R3) on a non-trivial fixture:
//     iterate every store slot with back_ptr != nullptr; assert
//     isKindOf<PDBAtom>(back_ptr) == bit.
/////////////////////////////////////////////////////////////
CHECK(D-H3.13-R3 dual-existence parity on full subtree)
	System sys;
	Protein* prot = new Protein; prot->setName("P");
	Chain*   ch   = new Chain;   ch->setName("A");
	Residue* r1   = new Residue; r1->setName("ALA");
	Residue* r2   = new Residue; r2->setName("GLY");
	PDBAtom* p1   = new PDBAtom; p1->setName("CA");
	PDBAtom* p2   = new PDBAtom; p2->setName("N");
	r1->insert(*p1);
	r2->insert(*p2);
	ch->insert(*r1); ch->insert(*r2);
	prot->insert(*ch);
	sys.insert(*prot);

	// Add a plain Atom under a sibling Molecule for the mixed-type case.
	Molecule* m = new Molecule;
	Atom*  plain = new Atom; plain->setName("X");
	m->insert(*plain);
	sys.insert(*m);

	MoleculeStore* store = p1->getStore();
	TEST_NOT_EQUAL(store, 0)
	bool ok = true;
	for (std::uint32_t i = 0; i < store->size(); ++i)
	{
		if (store->is_freed(i)) continue;
		Atom* back = store->back_ptr(i);
		if (back == 0) continue;          // store-only slot has no v0 truth
		const bool is_pdb = RTTI::isKindOf<PDBAtom>(back);
		const bool bit_set = (store->origin_flags(i) & 0x01u) != 0u;
		if (is_pdb != bit_set) { ok = false; break; }
	}
	TEST_EQUAL(ok, true)
RESULT

/////////////////////////////////////////////////////////////
END_TEST
