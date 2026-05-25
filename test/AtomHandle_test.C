// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atomHandle.h>
#include <BALL/KERNEL/bondHandle.h>
#include <BALL/KERNEL/structureQuery.h>
#include <unordered_set>
#include <BALL/KERNEL/containerHandle.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <vector>
///////////////////////////

START_TEST(AtomHandle)

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK(AtomHandle() null handle)
	AtomHandle nullh;
	TEST_EQUAL((bool)nullh, false)
	TEST_EQUAL(nullh.isNull(), true)
	TEST_EQUAL(nullh.isValid(), false)
	TEST_EQUAL(nullh.getStore() == (MoleculeStore*)nullptr, true)
	TEST_EQUAL(nullh.getAtom() == (Atom*)nullptr, true)
RESULT

CHECK(AtomHandle(store, idx) bind + validity + getters)
	MoleculeStore store;
	MoleculeStore::Index i1 = store.allocate_atom();
	AtomHandle h(store, i1);
	TEST_EQUAL((bool)h, true)
	TEST_EQUAL(h.isNull(), false)
	TEST_EQUAL(h.isValid(), true)
	TEST_EQUAL(h.getStore() == &store, true)
	TEST_EQUAL(h.getStoreIndex(), i1)
	TEST_EQUAL(h.getStableId(), store.stable_id(i1))
RESULT

CHECK(AtomHandle identity equality)
	MoleculeStore store;
	MoleculeStore::Index i1 = store.allocate_atom();
	MoleculeStore::Index i2 = store.allocate_atom();
	AtomHandle a(store, i1), a_again(store, i1), b(store, i2);
	TEST_EQUAL(a == a_again, true)
	TEST_EQUAL(a != b, true)
RESULT

CHECK(AtomHandle isValid() is ABA-safe across release + realloc)
	MoleculeStore store;
	MoleculeStore::Index i1 = store.allocate_atom();
	AtomHandle h(store, i1);
	TEST_EQUAL(h.isValid(), true)

	// Free the slot: the handle must go stale immediately.
	store.release_atom(i1);
	TEST_EQUAL(h.isValid(), false)

	// Reallocate: the free-list typically hands back the SAME slot index,
	// but it draws a FRESH stable id -- so the old handle must STAY stale
	// (no ABA alias), while a fresh handle on the recycled slot is valid.
	MoleculeStore::Index i2 = store.allocate_atom();
	AtomHandle h2(store, i2);
	TEST_EQUAL(h2.isValid(), true)
	TEST_EQUAL(h.isValid(), false)
	if (i2 == i1)
	{
		// same slot, different identity -> the ABA case we guard against
		TEST_NOT_EQUAL(h.getStableId(), h2.getStableId())
	}

	// H2c-CR: getAtom() is identity-aware -- a stale handle into a recycled
	// slot must NOT ABA-alias the new atom's back_ptr; it stays null.
	store.set_back_ptr(i2, (Atom*)0x1);   // mark the recycled slot as bound
	TEST_EQUAL(h.getAtom() == (Atom*)nullptr, true)   // stale -> null
	TEST_EQUAL(h2.getAtom() == (Atom*)0x1, true)      // fresh -> bridges
	store.set_back_ptr(i2, (Atom*)0);                 // restore (no real Atom)
RESULT

CHECK(sizeof(AtomHandle) within the value-handle budget)
	// {store* 8, idx u32 4, stable_id u64 8} -> 24 B (padded). The D13 16 B
	// target is the H4 flipped-Atom layout, not this transient handle.
	TEST_EQUAL(sizeof(AtomHandle) <= 24, true)
RESULT

/////////////////////////////////////////////////////////////
// H2c (D71/D71a): handle-yielding traversal order-parity vs v0 AtomIterator
/////////////////////////////////////////////////////////////

CHECK(StructureQuery::atoms() preorder == v0 Composite AtomIterator preorder)
	System sys;
	Protein prot;  prot.setName("PROT"); prot.setID("1ABC");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA"); r1.setID("ALA");
	Residue r2;    r2.setName("GLY"); r2.setID("GLY");
	SecondaryStructure ss; ss.setName("H1"); ss.setType(SecondaryStructure::HELIX);
	Residue r3;    r3.setName("SER"); r3.setID("SER");
	PDBAtom a1; a1.setName("N");
	PDBAtom a2; a2.setName("CA");
	PDBAtom a3; a3.setName("C");
	PDBAtom a4; a4.setName("O");
	r1.insert(a1); r1.insert(a2);
	r2.insert(a3);
	r3.insert(a4);
	ss.insert(r3);
	ch.insert(r1); ch.insert(r2); ch.insert(ss);
	prot.insert(ch);
	sys.insert(prot);   // production adoption materialises the whole subtree

	MoleculeStore* store = prot.getContainerRowStore_();
	TEST_NOT_EQUAL(store, (MoleculeStore*)nullptr)
	std::uint32_t root = prot.getContainerRow_();
	TEST_NOT_EQUAL(root, 0u)

	// v0 source-of-truth order: Composite preorder atom traversal.
	std::vector<std::uint32_t> v0order;
	for (AtomIterator ai = prot.beginAtom(); +ai; ++ai)
	{
		v0order.push_back(ai->getStoreIndex());
	}
	TEST_EQUAL(v0order.size(), 4)

	// handle-yielding order over the container table's ordered ChildRef edges.
	ContainerHandleBase rootH(*store, root);
	std::vector<AtomHandle> ha = StructureQuery::atoms(rootH);

	std::vector<std::uint32_t> horder;
	for (Size i = 0; i < ha.size(); ++i) horder.push_back(ha[i].getStoreIndex());

	TEST_EQUAL(horder.size(), v0order.size())
	TEST_EQUAL(horder == v0order, true)

	// Each yielded AtomHandle is valid and bridges back to its v0 Atom.
	bool all_valid = true, all_bridge = true;
	for (Size i = 0; i < ha.size(); ++i)
	{
		if (!ha[i].isValid()) all_valid = false;
		if (ha[i].getAtom() == 0) all_bridge = false;
	}
	TEST_EQUAL(all_valid, true)
	TEST_EQUAL(all_bridge, true)
RESULT

CHECK(StructureQuery::apply() preorder visits all containers + atoms)
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

	MoleculeStore* store = prot.getContainerRowStore_();
	std::uint32_t root = prot.getContainerRow_();
	ContainerHandleBase rootH(*store, root);

	// Processor counting container + atom visits; first visit is the root.
	struct Counter
	{
		Size containers = 0, atoms = 0;
		bool first_is_root = false;
		bool first = true;
		std::uint32_t root_idx = 0;
		void operator () (const ContainerHandleBase& c)
		{
			if (first) { first_is_root = (c.getStoreIndex() == root_idx); first = false; }
			++containers;
		}
		void operator () (const AtomHandle&) { ++atoms; }
	} counter;
	counter.root_idx = root;

	StructureQuery::apply(rootH, counter);

	// containers: Protein + Chain + Residue + Residue = 4; atoms = 3.
	TEST_EQUAL(counter.containers, 4)
	TEST_EQUAL(counter.atoms, 3)
	TEST_EQUAL(counter.first_is_root, true)
RESULT

/////////////////////////////////////////////////////////////
// H3a.3: AtomHandle migration shim (scalar / selection / bonds / key)
/////////////////////////////////////////////////////////////

CHECK(AtomHandle shim - scalar forwarding + selection + bonds + stable-id key)
	MoleculeStore store;
	MoleculeStore::Index a0 = store.allocate_atom();
	MoleculeStore::Index a1 = store.allocate_atom();
	AtomHandle h0(store, a0), h1(store, a1);

	// scalar round-trips through the shared store columns
	h0.setPosition(Vector3(1.0f, 2.0f, 3.0f));
	TEST_REAL_EQUAL(h0.getPosition().x, 1.0)
	TEST_REAL_EQUAL(h0.getPosition().z, 3.0)
	h0.setCharge(-0.5f);    TEST_REAL_EQUAL(h0.getCharge(), -0.5)
	h0.setRadius(1.7f);     TEST_REAL_EQUAL(h0.getRadius(), 1.7)
	h0.setType(6);          TEST_EQUAL(h0.getType(), 6)
	h0.setFormalCharge(-1); TEST_EQUAL(h0.getFormalCharge(), -1)
	h0.setName("CA");       TEST_EQUAL(h0.getName(), String("CA"))
	h0.setTypeName("C.3");  TEST_EQUAL(h0.getTypeName(), String("C.3"))

	// selection bit
	TEST_EQUAL(h0.isSelected(), false)
	h0.select();   TEST_EQUAL(h0.isSelected(), true)
	h0.deselect(); TEST_EQUAL(h0.isSelected(), false)

	// bonds via the store CSR (getBond -> BondHandle, bonds() range)
	TEST_EQUAL(h0.countBonds(), 0)
	store.add_bond(a0, a1, 1, 1);
	TEST_EQUAL(h0.countBonds(), 1)
	BondHandle bh = h0.getBond(0);
	TEST_EQUAL((bool)bh, true)
	TEST_EQUAL(bh.getPartner(h0).getStoreIndex(), a1)   // bond connects a0--a1
	TEST_EQUAL(h0.bonds().size(), 1)
	TEST_EQUAL((bool)h0.getBond(1), false)              // out of range -> null

	// D-H3.8 stable-id key: AtomHandle is a drop-in hash/set key.
	std::unordered_set<AtomHandle> seen;
	seen.insert(h0); seen.insert(AtomHandle(store, a0)); seen.insert(h1);
	TEST_EQUAL(seen.size(), 2)                          // h0 == AtomHandle(store,a0)
RESULT

/////////////////////////////////////////////////////////////
END_TEST
