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
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/handleKey.h>     // v2.2 H4 7b: handle-as-key infra
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/DATATYPE/hashSet.h>
#include <set>
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

	// H3a.3b.2: hierarchy nav from an atom up the role-walk. Each atom must
	// land in the same Molecule (the Protein), Chain, Residue (its own), and
	// the SS-layered atom (a4) additionally has a SECONDARY_STRUCTURE ancestor.
	MoleculeHandle p_mol = ha[0].getMolecule();
	TEST_EQUAL((bool)p_mol, true)
	for (Size i = 0; i < ha.size(); ++i)
	{
		TEST_EQUAL(ha[i].getMolecule() == p_mol, true)   // all atoms -> same Protein
		TEST_EQUAL((bool)ha[i].getChain(), true)         // every atom has a chain
		TEST_EQUAL((bool)ha[i].getResidue(), true)       // every atom has a residue
		TEST_EQUAL((bool)ha[i].getFragment(), true)      // immediate fragment ancestor
	}
	// a4 sits under chain -> SS(H1) -> r3 -> a4; only a4 has a SS ancestor.
	int ss_count = 0;
	for (Size i = 0; i < ha.size(); ++i) if (ha[i].getSecondaryStructure()) ++ss_count;
	TEST_EQUAL(ss_count, 1)

	// getParent returns the immediate container row (a residue in this tree).
	TEST_EQUAL((bool)ha[0].getParent(), true)
	TEST_EQUAL(ha[0].getParent().getKind() == ContainerKind::RESIDUE, true)

	// H3a-CR fix (item 3): select()/deselect() forward through the v0 Atom so
	// the v0 selection cascade (container_selection_count_ counters +
	// selection_generation_ + the v0 Atom's own isSelected) stays in sync
	// with the store column. Round-trip: v0 + handle observers agree.
	AtomHandle sel = ha[0];
	TEST_EQUAL(sel.isSelected(), false)
	TEST_EQUAL(sel.getAtom()->isSelected(), false)
	sel.select();
	TEST_EQUAL(sel.isSelected(), true)
	TEST_EQUAL(sel.getAtom()->isSelected(), true)
	sel.deselect();
	TEST_EQUAL(sel.isSelected(), false)
	TEST_EQUAL(sel.getAtom()->isSelected(), false)

	// H3a.3b.3: property shim (forwards to the v0 Atom / PropertyManager surface).
	AtomHandle ah = ha[0];
	TEST_EQUAL(ah.hasProperty("TEST_FLOAT"), false)
	ah.setProperty("TEST_FLOAT", 1.5f);
	ah.setProperty("TEST_STR",   String("hello"));
	ah.setProperty("TEST_BOOL",  true);
	ah.setProperty("TEST_FLAG");                     // unset / name-only
	TEST_EQUAL(ah.hasProperty("TEST_FLOAT"), true)
	TEST_EQUAL(ah.hasProperty("TEST_STR"),   true)
	TEST_EQUAL(ah.hasProperty("TEST_BOOL"),  true)
	TEST_EQUAL(ah.hasProperty("TEST_FLAG"),  true)
	TEST_EQUAL(ah.hasProperty("MISSING"),    false)
	TEST_EQUAL(ah.countNamedProperties() >= 4, true)

	std::vector<String> names = ah.propertyNames();
	bool saw_float = false, saw_str = false;
	for (Size i = 0; i < names.size(); ++i) {
		if (names[i] == "TEST_FLOAT") saw_float = true;
		if (names[i] == "TEST_STR")   saw_str   = true;
	}
	TEST_EQUAL(saw_float, true)
	TEST_EQUAL(saw_str,   true)

	// eachProperty visitor with the value-semantic const NamedProperty&.
	int visited = 0;
	ah.eachProperty([&](const NamedProperty&){ ++visited; });
	TEST_EQUAL(visited, (int)ah.countNamedProperties())

	ah.clearProperty("TEST_FLOAT");
	TEST_EQUAL(ah.hasProperty("TEST_FLOAT"), false)

	// Property bit (flag) surface.
	const Property FLAG = 7;        // arbitrary bit
	TEST_EQUAL(ah.hasProperty(FLAG), false)
	ah.setProperty(FLAG);
	TEST_EQUAL(ah.hasProperty(FLAG), true)
	ah.clearProperty(FLAG);
	TEST_EQUAL(ah.hasProperty(FLAG), false)
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

	// element via PTE (atomic number column)
	h0.setElement(PTE[Element::CARBON]);
	TEST_EQUAL(h0.getElement().getAtomicNumber(), 6)
	TEST_EQUAL(h0.getElement().getSymbol(), String("C"))
	h0.setElement(PTE[Element::NITROGEN]);
	TEST_EQUAL(h0.getElement().getAtomicNumber(), 7)

	// selection bit (read-only here: a bare-store atom has no v0 backing, so
	// select()/deselect() — which forward through getAtom() — no-op without a
	// v0 Atom. The System-based CHECK below exercises the full select cascade.)
	TEST_EQUAL(h0.isSelected(), false)

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
// H3a.4: handle atom-processor (Processor::Result contract)
/////////////////////////////////////////////////////////////

CHECK(StructureQuery::applyAtomProcessor - start/finish + CONTINUE/BREAK/ABORT)
	System sys;
	Protein prot;  prot.setName("P");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA");
	Residue r2;    r2.setName("GLY");
	PDBAtom a1; a1.setName("N");
	PDBAtom a2; a2.setName("CA");
	PDBAtom a3; a3.setName("C");
	PDBAtom a4; a4.setName("O");
	r1.insert(a1); r1.insert(a2);
	r2.insert(a3); r2.insert(a4);
	ch.insert(r1); ch.insert(r2);
	prot.insert(ch);
	sys.insert(prot);
	MoleculeStore* store = prot.getContainerRowStore_();
	ContainerHandleBase root(*store, prot.getContainerRow_());

	// CONTINUE on every atom: visits all 4, succeeds, start()+finish() run.
	struct CountProc {
		Size n = 0; bool started = false, finished = false;
		bool start()  { started = true;  return true; }
		bool finish() { finished = true; return true; }
		Processor::Result operator () (AtomHandle&) { ++n; return Processor::CONTINUE; }
	} cp;
	TEST_EQUAL(StructureQuery::applyAtomProcessor(root, cp), true)
	TEST_EQUAL(cp.n, 4)
	TEST_EQUAL(cp.started && cp.finished, true)

	// BREAK after the 2nd atom: successful early stop; finish() runs.
	struct BreakProc {
		Size n = 0; bool finished = false;
		bool start()  { return true; }
		bool finish() { finished = true; return true; }
		Processor::Result operator () (AtomHandle&)
		{ ++n; return (n >= 2) ? Processor::BREAK : Processor::CONTINUE; }
	} bp;
	TEST_EQUAL(StructureQuery::applyAtomProcessor(root, bp), true)
	TEST_EQUAL(bp.n, 2)
	TEST_EQUAL(bp.finished, true)

	// ABORT after the 2nd atom: failure; finish() skipped (v0 && semantics).
	struct AbortProc {
		Size n = 0; bool finished = false;
		bool start()  { return true; }
		bool finish() { finished = true; return true; }
		Processor::Result operator () (AtomHandle&)
		{ ++n; return (n >= 2) ? Processor::ABORT : Processor::CONTINUE; }
	} ap;
	TEST_EQUAL(StructureQuery::applyAtomProcessor(root, ap), false)
	TEST_EQUAL(ap.n, 2)
	TEST_EQUAL(ap.finished, false)

	// start() == false: the walk never runs.
	struct NoStartProc {
		Size n = 0;
		bool start()  { return false; }
		bool finish() { return true; }
		Processor::Result operator () (AtomHandle&) { ++n; return Processor::CONTINUE; }
	} np;
	TEST_EQUAL(StructureQuery::applyAtomProcessor(root, np), false)
	TEST_EQUAL(np.n, 0)
RESULT


CHECK(v2.2 H4 7b: handleKey -- AtomHandle/BondHandle as container keys)
	// Foundational infra for the gate-surfaced STRUCTURE pointer-key
	// migrations: handles usable as keys in BALL HashMap (boost), BALL
	// HashSet (BALL::Hash), std::set/map (operator<), std::unordered_*
	// (std::hash); + makeHandle() Atom*/Bond* -> handle and getAtom()/
	// getBond() resolution.
	System sys;
	Molecule* mol = new Molecule;
	sys.insert(*mol);
	Atom* a1 = new Atom; Atom* a2 = new Atom; Atom* a3 = new Atom;
	mol->insert(*a1); mol->insert(*a2); mol->insert(*a3);
	a1->createBond(*a2);

	// makeHandle + resolve round-trip.
	AtomHandle h1 = makeHandle(*a1);
	TEST_EQUAL(h1.isValid(), true)
	TEST_EQUAL(h1.getAtom() == a1, true)

	// BALL HashMap<AtomHandle,int> (boost::hash -> hash_value).
	HashMap<AtomHandle, int> hm;
	hm[makeHandle(*a1)] = 11;
	hm[makeHandle(*a2)] = 22;
	TEST_EQUAL(hm[makeHandle(*a1)], 11)
	TEST_EQUAL(hm[makeHandle(*a2)], 22)
	TEST_EQUAL(hm.has(makeHandle(*a3)), false)

	// BALL HashSet<AtomHandle> (BALL::Hash).
	HashSet<AtomHandle> hs;
	hs.insert(makeHandle(*a1));
	hs.insert(makeHandle(*a1));   // dedup
	hs.insert(makeHandle(*a2));
	TEST_EQUAL(hs.size(), 2)
	TEST_EQUAL(hs.has(makeHandle(*a1)), true)
	TEST_EQUAL(hs.has(makeHandle(*a3)), false)

	// std::set<AtomHandle> (operator<).
	std::set<AtomHandle> ss;
	ss.insert(makeHandle(*a1)); ss.insert(makeHandle(*a1)); ss.insert(makeHandle(*a2));
	TEST_EQUAL(ss.size(), (std::size_t)2)

	// BondHandle key path.
	const Bond* bd = a1->getBond((Size)0);
	BondHandle bh = makeHandle(*bd);
	TEST_EQUAL(bh.isValid(), true)
	TEST_EQUAL(bh.getBond() == bd, true)
	HashSet<BondHandle> bset; bset.insert(makeHandle(*bd));
	TEST_EQUAL(bset.has(makeHandle(*bd)), true)
	std::set<BondHandle> bss; bss.insert(makeHandle(*bd)); bss.insert(makeHandle(*bd));
	TEST_EQUAL(bss.size(), (std::size_t)1)

	// Unbound atom -> null handle (no store crash).
	Atom orphan_local;   // bound to the process orphan store, still valid
	AtomHandle oh = makeHandle(orphan_local);
	TEST_EQUAL(oh.isNull(), false)   // orphan store is a real store
RESULT

/////////////////////////////////////////////////////////////
END_TEST
