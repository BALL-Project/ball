// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/extractorsHandle.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/compiledExpression.h>
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

	// selected_only filter: H3b-CR FLAW 3 -- v0 semantics. Iterating SELECTED
	// atoms, accept all their bonds. Selecting a1 + a2: a1's bonds = {a1-a2},
	// a2's bonds = {a1-a2, a2-a3}. Dedup -> 2 unique bonds. (See the FLAW 3
	// CHECK below for the asymmetric-selection assertion.)
	ah[0].select();    // a1
	ah[1].select();    // a2
	std::vector<BondHandle> bh_sel = bondHandles(prot, /*selected_only*/ true);
	TEST_EQUAL(bh_sel.size(), 2)
RESULT

/////////////////////////////////////////////////////////////
// H3b.2: predicate-filtered atomHandles
/////////////////////////////////////////////////////////////

CHECK(atomHandles with Expression filter (carbons in named-atom set))
	System sys;
	Protein prot;  prot.setName("PROT");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA"); r1.setID("ALA");
	PDBAtom a1; a1.setName("N");  a1.setElement(PTE[Element::NITROGEN]);
	PDBAtom a2; a2.setName("CA"); a2.setElement(PTE[Element::CARBON]);
	PDBAtom a3; a3.setName("C");  a3.setElement(PTE[Element::CARBON]);
	PDBAtom a4; a4.setName("O");  a4.setElement(PTE[Element::OXYGEN]);
	r1.insert(a1); r1.insert(a2); r1.insert(a3); r1.insert(a4);
	ch.insert(r1);
	prot.insert(ch);
	sys.insert(prot);

	// empty expression -> all atoms
	std::vector<AtomHandle> all = atomHandles(prot, "");
	TEST_EQUAL(all.size(), 4)

	// element predicate via Expression -> only the 2 carbons
	std::vector<AtomHandle> carbons = atomHandles(prot, "element(C)");
	TEST_EQUAL(carbons.size(), 2)
	for (Size i = 0; i < carbons.size(); ++i)
		TEST_EQUAL(carbons[i].getElement().getAtomicNumber(), 6)
RESULT

CHECK(residueHandles / chainHandles / secondaryStructureHandles)
	System sys;
	Protein prot;  prot.setName("PROT");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA");
	Residue r2;    r2.setName("GLY");
	SecondaryStructure ss; ss.setName("H1"); ss.setType(SecondaryStructure::HELIX);
	Residue r3;    r3.setName("SER");
	PDBAtom a1; a1.setName("CA");
	PDBAtom a2; a2.setName("CA");
	PDBAtom a3; a3.setName("CA");
	r1.insert(a1); r2.insert(a2); r3.insert(a3);
	ss.insert(r3);
	ch.insert(r1); ch.insert(r2); ch.insert(ss);
	prot.insert(ch);
	sys.insert(prot);

	// 3 residues (r1, r2, r3 — r3 sits under SS but still has RESIDUE role)
	std::vector<FragmentHandle> rs = residueHandles(prot);
	TEST_EQUAL(rs.size(), 3)
	for (Size i = 0; i < rs.size(); ++i)
		TEST_EQUAL(rs[i].getFragmentRole() == FragmentRole::RESIDUE, true)

	// 1 chain
	std::vector<FragmentHandle> chs = chainHandles(prot);
	TEST_EQUAL(chs.size(), 1)
	TEST_EQUAL(chs[0].getFragmentRole() == FragmentRole::CHAIN, true)

	// 1 SS layer
	std::vector<FragmentHandle> sss = secondaryStructureHandles(prot);
	TEST_EQUAL(sss.size(), 1)
	TEST_EQUAL(sss[0].getFragmentRole() == FragmentRole::SECONDARY_STRUCTURE, true)
RESULT

CHECK(moleculeHandles / fragmentHandles API + kind-set semantics)
	// Set up a Protein with chain/residue/atoms. The MOLECULE-kind extractor
	// matches the {MOLECULE, PROTEIN, NUCLEIC_ACID} set (forward-stable
	// across HCP-2's ContainerKind shrink); the FRAGMENT-kind extractor
	// matches {FRAGMENT, CHAIN, RESIDUE, SECONDARY_STRUCTURE, NUCLEOTIDE}.
	System sys;
	Protein  prot;  prot.setName("PROT");
	Chain    ch;    ch.setName("A");
	Residue  r1;    r1.setName("ALA");
	Residue  r2;    r2.setName("GLY");
	PDBAtom  a1; a1.setName("CA");
	PDBAtom  a2; a2.setName("CA");
	r1.insert(a1); r2.insert(a2);
	ch.insert(r1); ch.insert(r2);
	prot.insert(ch);
	sys.insert(prot);

	// System has no container row (it IS the materialiser, never
	// materialised itself). asContainerHandle returns null.
	TEST_EQUAL((bool)asContainerHandle(sys), false)
	TEST_EQUAL(moleculeHandles(sys).size(), 0)
	TEST_EQUAL(fragmentHandles(sys).size(), 0)

	// Rooting at prot: H3b-CR FLAW 1 fixed -- prot itself (PROTEIN, molecule-
	// kind) is now emitted as the first molecule. 3 FRAGMENT-kind descendants
	// (chain + r1 + r2) -- prot is not fragment-kind so fragmentHandles(prot)
	// stays at 3.
	TEST_EQUAL(moleculeHandles(prot).size(), 1)
	TEST_EQUAL(moleculeHandles(prot)[0].getKind() == ContainerKind::PROTEIN, true)

	std::vector<FragmentHandle> frags = fragmentHandles(prot);
	TEST_EQUAL(frags.size(), 3)
	bool saw_chain = false; Size n_residue = 0;
	for (Size i = 0; i < frags.size(); ++i)
	{
		if (frags[i].getKind() == ContainerKind::CHAIN)   saw_chain = true;
		if (frags[i].getKind() == ContainerKind::RESIDUE) n_residue++;
	}
	TEST_EQUAL(saw_chain, true)
	TEST_EQUAL(n_residue, 2)

	// Kind-set predicate sanity: residues + chains are both FRAGMENT-kind
	// in fragmentHandles; this is the forward-stable group. Calling
	// fragmentHandles(ch) yields ch itself + 2 residues (H3b-CR FLAW 1).
	TEST_EQUAL(fragmentHandles(ch).size(), 3)
RESULT

CHECK(atomHandlesIf with a generic callable predicate)
	System sys;
	Protein prot;  prot.setName("PROT");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA");
	PDBAtom a1; a1.setName("N");  a1.setElement(PTE[Element::NITROGEN]);
	PDBAtom a2; a2.setName("CA"); a2.setElement(PTE[Element::CARBON]);
	PDBAtom a3; a3.setName("C");  a3.setElement(PTE[Element::CARBON]);
	r1.insert(a1); r1.insert(a2); r1.insert(a3);
	ch.insert(r1);
	prot.insert(ch);
	sys.insert(prot);

	// lambda predicate over AtomHandle directly (handle-by-value, no v0 ptr)
	std::vector<AtomHandle> matches = atomHandlesIf(prot,
		[](const AtomHandle& h){ return h.getElement().getAtomicNumber() == 6; });
	TEST_EQUAL(matches.size(), 2)
RESULT

/////////////////////////////////////////////////////////////
// H3b-CR: review-fix coverage
/////////////////////////////////////////////////////////////

CHECK(FLAW 1: root self-inclusion -- moleculeHandles(prot) includes prot)
	// v0 molecules(c) iterates beginAtomContainer() which visits `c` first;
	// the handle extractor must include the root if it is molecule-kind.
	System sys;
	Protein prot;  prot.setName("PROT");
	PDBAtom a; a.setName("CA");
	prot.insert(a);
	sys.insert(prot);
	std::vector<MoleculeHandle> mh = moleculeHandles(prot);
	TEST_EQUAL(mh.size(), 1)
	TEST_EQUAL(mh[0].getKind() == ContainerKind::PROTEIN, true)
RESULT

CHECK(FLAW 1: residueHandles / chainHandles / SS root-inclusion)
	System sys;
	Chain ch; ch.setName("A");
	Residue r1; r1.setName("ALA"); ch.insert(r1);
	Residue r2; r2.setName("GLY"); ch.insert(r2);
	PDBAtom a1; a1.setName("CA"); r1.insert(a1);
	PDBAtom a2; a2.setName("CA"); r2.insert(a2);
	// Need a Molecule-rooted owner because Chain alone won't be materialised
	// by an inserting System the same way (Chain isn't a Molecule kind).
	Protein owner; owner.setName("P"); owner.insert(ch);
	sys.insert(owner);

	// chainHandles(ch) -> include ch (CHAIN-role) itself; no chain descendants.
	std::vector<FragmentHandle> chs = chainHandles(ch);
	TEST_EQUAL(chs.size(), 1)
	TEST_EQUAL(chs[0].getFragmentRole() == FragmentRole::CHAIN, true)

	// residueHandles(r1) -> include r1 itself; no residue descendants.
	std::vector<FragmentHandle> rs = residueHandles(r1);
	TEST_EQUAL(rs.size(), 1)
	TEST_EQUAL(rs[0].getFragmentRole() == FragmentRole::RESIDUE, true)

	// residueHandles(ch) -> 2 residues (r1, r2); ch itself is CHAIN-role.
	TEST_EQUAL(residueHandles(ch).size(), 2)
RESULT

CHECK(FLAW 3: bondHandles selected_only matches v0 -- any endpoint selected)
	// v0 extractors.C:108-130 inserts ALL bonds of selected atoms (the partner
	// need not be selected). H3b-CR aligns. Build a-b-c; select only `a`. The
	// a-b bond is in the result; the b-c bond is NOT (b unselected here).
	System sys;
	Protein prot; prot.setName("P");
	Chain ch; ch.setName("A"); prot.insert(ch);
	Residue r; r.setName("R"); ch.insert(r);
	PDBAtom pa; pa.setName("a");
	PDBAtom pb; pb.setName("b");
	PDBAtom pc; pc.setName("c");
	r.insert(pa); r.insert(pb); r.insert(pc);
	sys.insert(prot);
	pa.createBond(pb);
	pb.createBond(pc);

	std::vector<AtomHandle> all = atomHandles(prot);
	TEST_EQUAL(all.size(), 3)
	// Select ONLY a (the first atom).
	all[0].select();

	std::vector<BondHandle> bh = bondHandles(prot, /*selected_only*/ true);
	TEST_EQUAL(bh.size(), 1)   // a-b bond (because a is selected)
RESULT

CHECK(NIT 8: atom preorder over interleaved-sibling children)
	// Mixed atom/container/atom under one parent -- the H2c ordered ChildRef
	// preorder claim. v0 AtomIterator order is the truth. Use a Molecule as
	// the parent: it's the highest-level container that accepts both Atoms
	// and AtomContainers as children via Composite::appendChild.
	System sys;
	Molecule mol; mol.setName("M");
	PDBAtom a1; a1.setName("a1");
	Residue rinner; rinner.setName("R");
	PDBAtom aInner; aInner.setName("ai");
	rinner.insert(aInner);
	PDBAtom a2; a2.setName("a2");
	// Mix children deliberately: atom (a1), container (rinner with aInner),
	// atom (a2). v0 Composite preorder visits a1, then descends into rinner
	// to visit aInner, then a2.
	mol.appendChild(a1);
	mol.appendChild(rinner);
	mol.appendChild(a2);
	sys.insert(mol);
	AtomContainer& prot = mol;   // alias the rest of the test uses

	std::vector<AtomHandle> hatoms = atomHandles(prot);
	std::vector<std::uint32_t> v0idx;
	for (AtomIterator it = prot.beginAtom(); +it; ++it) v0idx.push_back(it->getStoreIndex());

	TEST_EQUAL(hatoms.size(), v0idx.size())
	TEST_EQUAL(hatoms.size(), 3)
	for (Size i = 0; i < hatoms.size(); ++i)
		TEST_EQUAL(hatoms[i].getStoreIndex(), v0idx[i])
RESULT

CHECK(NIT 9: atomHandlesBy survives a sparse-bitmap store (freed slots))
	// Build a store with some atoms that get released before compile() so the
	// resulting bitmap has freed-slot zeros interleaved with live atom indices.
	System sys;
	Protein prot; prot.setName("P");
	Chain ch; ch.setName("A"); prot.insert(ch);
	Residue r; r.setName("R"); ch.insert(r);
	PDBAtom a1; a1.setName("N"); a1.setElement(PTE[Element::NITROGEN]);
	PDBAtom a2; a2.setName("CA"); a2.setElement(PTE[Element::CARBON]);
	PDBAtom a3; a3.setName("C"); a3.setElement(PTE[Element::CARBON]);
	r.insert(a1); r.insert(a2); r.insert(a3);
	sys.insert(prot);

	// Burn a few orphan PDBAtoms in the store, then release them -- the
	// container_table bitmap will have holes at those store indices.
	Residue scratch; scratch.setName("X");
	PDBAtom s1, s2;
	scratch.insert(s1); scratch.insert(s2);
	// don't ever insert scratch into sys -- destructor releases the atoms.
	{
		Residue tmp; tmp.setName("Y");
		PDBAtom t1; t1.setName("t1");
		tmp.insert(t1);
		// tmp + t1 destroyed at end of scope, releasing their store slots
	}

	std::vector<AtomHandle> live = atomHandles(prot);
	TEST_EQUAL(live.size(), 3)
	MoleculeStore* store = live[0].getStore();

	std::shared_ptr<const CompiledExpression> ce =
		CompiledExpression::compile(*store, "element(C)");
	std::vector<AtomHandle> carbons = atomHandlesBy(prot, *ce);
	TEST_EQUAL(carbons.size(), 2)
RESULT

/////////////////////////////////////////////////////////////
// H3b.5: ExpressionPredicate + CompiledExpression handle integration
/////////////////////////////////////////////////////////////

CHECK(atomHandlesIf with v0 ExpressionPredicate (ElementPredicate))
	System sys;
	Protein prot;  prot.setName("PROT");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA");
	PDBAtom a1; a1.setName("N"); a1.setElement(PTE[Element::NITROGEN]);
	PDBAtom a2; a2.setName("CA"); a2.setElement(PTE[Element::CARBON]);
	PDBAtom a3; a3.setName("C"); a3.setElement(PTE[Element::CARBON]);
	r1.insert(a1); r1.insert(a2); r1.insert(a3);
	ch.insert(r1); prot.insert(ch);
	sys.insert(prot);

	ElementPredicate ep;
	ep.setArgument("C");
	std::vector<AtomHandle> carbons = atomHandlesIf(prot, ep);
	TEST_EQUAL(carbons.size(), 2)
	for (Size i = 0; i < carbons.size(); ++i)
		TEST_EQUAL(carbons[i].getElement().getAtomicNumber(), 6)
RESULT

CHECK(atomHandlesBy with CompiledExpression (bitmap evaluator))
	System sys;
	Protein prot;  prot.setName("PROT");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA");
	PDBAtom a1; a1.setName("N"); a1.setElement(PTE[Element::NITROGEN]);
	PDBAtom a2; a2.setName("CA"); a2.setElement(PTE[Element::CARBON]);
	PDBAtom a3; a3.setName("C"); a3.setElement(PTE[Element::CARBON]);
	PDBAtom a4; a4.setName("O"); a4.setElement(PTE[Element::OXYGEN]);
	r1.insert(a1); r1.insert(a2); r1.insert(a3); r1.insert(a4);
	ch.insert(r1); prot.insert(ch);
	sys.insert(prot);

	// Resolve the store via an arbitrary atom handle, then compile against it.
	std::vector<AtomHandle> all = atomHandles(prot);
	TEST_EQUAL(all.size(), 4)
	MoleculeStore* store = all[0].getStore();
	TEST_NOT_EQUAL(store, 0)

	std::shared_ptr<const CompiledExpression> ce =
		CompiledExpression::compile(*store, "element(C)");

	std::vector<AtomHandle> carbons = atomHandlesBy(prot, *ce);
	TEST_EQUAL(carbons.size(), 2)
	for (Size i = 0; i < carbons.size(); ++i)
		TEST_EQUAL(carbons[i].getElement().getAtomicNumber(), 6)
RESULT

/////////////////////////////////////////////////////////////
END_TEST
