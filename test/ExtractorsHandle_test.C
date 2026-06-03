// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/extractorsHandle.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atomIterator.h>
///////////////////////////

START_TEST(ExtractorsHandle)

using namespace BALL;

/////////////////////////////////////////////////////////////
// H3b.1: handle-based extractor helpers (atomHandles / bondHandles)
/////////////////////////////////////////////////////////////

CHECK(asContainerHandle: unbound container -> null handle)
	Protein orphan;
	orphan.setName("ORPH");
	TEST_EQUAL((bool)asContainerHandle(orphan), false)
RESULT

CHECK(atomHandles + bondHandles round-trip vs v0)
	System sys;
	Protein prot;  prot.setName("PROT");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA"); r1.setID("ALA");
	Residue r2;    r2.setName("GLY"); r2.setID("GLY");
	PDBAtom a1; a1.setName("N");
	PDBAtom a2; a2.setName("CA");
	PDBAtom a3; a3.setName("C");
	r1.insert(a1); r1.insert(a2);
	r2.insert(a3);
	ch.insert(r1); ch.insert(r2);
	prot.insert(ch);
	sys.insert(prot);

	// Some bonds in the v0 tree (a1-a2, a2-a3). createBond goes through
	// the store add_bond path so each gets a fresh bond StableId.
	a1.createBond(a2);
	a2.createBond(a3);

	// Atom count parity: handle extractor == v0 AtomIterator order/length.
	std::vector<AtomHandle> ah = atomHandles(prot);
	TEST_EQUAL(ah.size(), 3)
	std::vector<std::uint32_t> v0idx;
	for (AtomIterator it = prot.beginAtom(); +it; ++it) v0idx.push_back(it->getStoreIndex());
	TEST_EQUAL(ah.size(), v0idx.size())
	for (Size i = 0; i < ah.size(); ++i)
		TEST_EQUAL(ah[i].getStoreIndex(), v0idx[i])

	// Bond extractor — dedup-keyed on bond StableId (D-H3.8). 2 unique bonds
	// even though each appears in TWO endpoint atoms' bond lists.
	std::vector<BondHandle> bh = bondHandles(prot, /*selected_only*/ false);
	TEST_EQUAL(bh.size(), 2)
	TEST_NOT_EQUAL(bh[0].getStableId(), bh[1].getStableId())   // unique

	// selected_only filter: select only a1 + a2, so only the a1-a2 bond
	// qualifies (a2-a3 is dropped because a3 isn't selected).
	ah[0].select();    // a1
	ah[1].select();    // a2
	std::vector<BondHandle> bh_sel = bondHandles(prot, /*selected_only*/ true);
	TEST_EQUAL(bh_sel.size(), 1)
RESULT

/////////////////////////////////////////////////////////////
END_TEST
