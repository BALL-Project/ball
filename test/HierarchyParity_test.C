// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H1a (A2 flat SoA topology): HierarchyParity_test.
//
// H1a adds a store-side container metadata table (ContainerTable) that
// mirrors the molecular hierarchy (Molecule/Chain/Residue/...) as
// ContainerRows + ordered ChildRef edges (D57) + per-kind payloads
// (D58) + selection counters (D59), with detached-subtree migration
// (D56). H1a is NON-INVASIVE: the v0 Composite object tree remains the
// source of truth (D60); nothing in the v0 consumer path reads or
// writes the table yet. This test is the parity skeleton R31 asked for
// (follow-up #3): it builds a representative v0 tree, mirrors it into a
// ContainerTable by hand (simulating what the H2 mutation-mirror wiring
// will do automatically), and asserts the mirror reproduces the tree --
// structure, ordered child sequence, reverse parent links, payload, and
// selection counters. It also exercises the ChildRef ops directly and
// the orphan->System subtree migration.
//
// What this LOCKS for H1a (the bounded acceptance criterion, R31 P31-7):
// a store mirror that can be built/updated from representative v0
// insert/append/prepend/insertBefore/insertAfter cases and compared by
// parity. The FULL mutation-mirror set + automatic wiring lands in H2.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/_moleculeStoreInternal.h>   // D31b: H1a wiring TU
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/rtti.h>
#include <sstream>
#include <string>
///////////////////////////

using namespace BALL;

namespace
{
	// Most-derived-first kind discrimination for a v0 Composite. Protein
	// IS-A Molecule and Residue/Nucleotide IS-A Fragment, so the derived
	// kinds MUST be tested first.
	ContainerKind kindOf(const Composite& c)
	{
		if (RTTI::isKindOf<Protein>(&c))            return ContainerKind::PROTEIN;
		if (RTTI::isKindOf<NucleicAcid>(&c))        return ContainerKind::NUCLEIC_ACID;
		if (RTTI::isKindOf<Molecule>(&c))           return ContainerKind::MOLECULE;
		if (RTTI::isKindOf<Residue>(&c))            return ContainerKind::RESIDUE;
		if (RTTI::isKindOf<Nucleotide>(&c))         return ContainerKind::NUCLEOTIDE;
		if (RTTI::isKindOf<SecondaryStructure>(&c)) return ContainerKind::SECONDARY_STRUCTURE;
		if (RTTI::isKindOf<Chain>(&c))              return ContainerKind::CHAIN;
		if (RTTI::isKindOf<Fragment>(&c))           return ContainerKind::FRAGMENT;
		return ContainerKind::NONE;
	}

	// Per-kind payload extraction from a v0 Composite into the row that
	// has already been allocated at `idx`. Re-derives id/insertion-code/
	// ss-type independently of the production path (strengthens parity).
	void fillPayloadFromV0(const Composite& c, ContainerTable& t, std::uint32_t idx)
	{
		if (const AtomContainer* ac = dynamic_cast<const AtomContainer*>(&c))
			t.row(idx).name_offset = t.intern(std::string(ac->getName()));

		if (const Protein* p = dynamic_cast<const Protein*>(&c))
			t.row(idx).payload.id_offset = t.intern(std::string(p->getID()));
		else if (const NucleicAcid* na = dynamic_cast<const NucleicAcid*>(&c))
			t.row(idx).payload.id_offset = t.intern(std::string(na->getID()));
		else if (const Residue* r = dynamic_cast<const Residue*>(&c))
		{
			t.row(idx).payload.id_offset      = t.intern(std::string(r->getID()));
			t.row(idx).payload.insertion_code = r->getInsertionCode();
		}
		else if (const Nucleotide* nt = dynamic_cast<const Nucleotide*>(&c))
		{
			t.row(idx).payload.id_offset      = t.intern(std::string(nt->getID()));
			t.row(idx).payload.insertion_code = nt->getInsertionCode();
		}
		else if (const SecondaryStructure* ss = dynamic_cast<const SecondaryStructure*>(&c))
			t.row(idx).payload.ss_type = static_cast<std::uint8_t>(ss->getType());
	}

	// Mirror a v0 container subtree into the table. Returns the new row
	// idx. Atoms become ATOM ChildRefs keyed by their store index.
	// Container children recurse. NB: t.allocate() reallocates rows_, so
	// nothing holds a ContainerRow& across the child loop.
	std::uint32_t mirror(const Composite& c, ContainerTable& t)
	{
		std::uint32_t idx = t.allocate(kindOf(c));
		fillPayloadFromV0(c, t, idx);
		for (Position i = 0; i < c.getDegree(); ++i)
		{
			const Composite* child = c.getChild(static_cast<Index>(i));
			if (RTTI::isKindOf<Atom>(child))
			{
				const Atom* a = dynamic_cast<const Atom*>(child);
				t.append_child(idx, ChildRef(ChildRef::ATOM, a->getStoreIndex()));
			}
			else
			{
				std::uint32_t child_idx = mirror(*child, t);
				t.append_child(idx, ChildRef(ChildRef::CONTAINER, child_idx));
			}
		}
		return idx;
	}

	// Flattened preorder description from a v0 subtree.
	void descV0(const Composite& c, std::string& out)
	{
		std::ostringstream os;
		os << "C:" << static_cast<int>(kindOf(c)) << ":";
		if (const AtomContainer* ac = dynamic_cast<const AtomContainer*>(&c))
			os << std::string(ac->getName());
		os << ":";
		if (const Protein* p = dynamic_cast<const Protein*>(&c)) os << std::string(p->getID());
		else if (const NucleicAcid* na = dynamic_cast<const NucleicAcid*>(&c)) os << std::string(na->getID());
		else if (const Residue* r = dynamic_cast<const Residue*>(&c)) os << std::string(r->getID());
		else if (const Nucleotide* nt = dynamic_cast<const Nucleotide*>(&c)) os << std::string(nt->getID());
		os << ":";
		if (const Residue* r = dynamic_cast<const Residue*>(&c)) os << r->getInsertionCode();
		else if (const Nucleotide* nt = dynamic_cast<const Nucleotide*>(&c)) os << nt->getInsertionCode();
		else os << ' ';
		os << ":";
		if (const SecondaryStructure* ss = dynamic_cast<const SecondaryStructure*>(&c))
			os << static_cast<int>(ss->getType());
		else os << 0;
		os << "|";
		out += os.str();
		for (Position i = 0; i < c.getDegree(); ++i)
		{
			const Composite* child = c.getChild(static_cast<Index>(i));
			if (RTTI::isKindOf<Atom>(child))
			{
				const Atom* a = dynamic_cast<const Atom*>(child);
				std::ostringstream as; as << "A:" << a->getStoreIndex() << "|";
				out += as.str();
			}
			else
			{
				descV0(*child, out);
			}
		}
	}

	// Flattened preorder description from the table -- MUST match descV0
	// byte-for-byte when the mirror is faithful.
	std::string descTable(const ContainerTable& t, std::uint32_t root)
	{
		std::string out;
		t.preorder(root,
			[&](std::uint32_t cidx)
			{
				const ContainerRow& r = t.row(cidx);
				std::ostringstream os;
				os << "C:" << static_cast<int>(r.kind) << ":"
				   << t.str(r.name_offset) << ":"
				   << t.str(r.payload.id_offset) << ":"
				   << r.payload.insertion_code << ":"
				   << static_cast<int>(r.payload.ss_type) << "|";
				out += os.str();
			},
			[&](std::uint32_t aidx)
			{
				std::ostringstream as; as << "A:" << aidx << "|";
				out += as.str();
			});
		return out;
	}
}

START_TEST(HierarchyParity)

using namespace BALL;

CHECK(ContainerTable -- allocate / release / free-list reuse)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	// slot 0 reserved as sentinel; first allocate yields idx 1.
	TEST_EQUAL(t.size(), 1u)
	TEST_EQUAL(t.live_count(), 0u)

	std::uint32_t m = t.allocate(ContainerKind::MOLECULE);
	TEST_EQUAL(m, 1u)
	TEST_EQUAL(t.row(m).kind == ContainerKind::MOLECULE, true)
	TEST_EQUAL(t.size(), 2u)
	TEST_EQUAL(t.live_count(), 1u)

	std::uint32_t c = t.allocate(ContainerKind::CHAIN);
	TEST_EQUAL(c, 2u)
	TEST_EQUAL(t.live_count(), 2u)

	// release returns slot to free list; next allocate reuses it.
	t.release(c);
	TEST_EQUAL(t.freed_count(), 1u)
	TEST_EQUAL(t.is_freed(c), true)
	std::uint32_t r = t.allocate(ContainerKind::RESIDUE);
	TEST_EQUAL(r, c)
	TEST_EQUAL(t.row(r).kind == ContainerKind::RESIDUE, true)
	TEST_EQUAL(t.freed_count(), 0u)

	// double release is a no-op (no double-push).
	t.release(r);
	t.release(r);
	TEST_EQUAL(t.freed_count(), 1u)
	// releasing the sentinel / NONE is a no-op.
	t.release(0u);
	t.release(ContainerTable::NONE);
	TEST_EQUAL(t.freed_count(), 1u)
RESULT

CHECK(ContainerTable -- string intern dedups)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	TEST_EQUAL(t.intern(""), 0u)          // empty -> canonical offset 0
	std::uint32_t o1 = t.intern("ALA");
	std::uint32_t o2 = t.intern("ALA");   // intern hit
	TEST_EQUAL(o1, o2)
	std::uint32_t o3 = t.intern("GLY");
	TEST_NOT_EQUAL(o1, o3)
	TEST_EQUAL(t.str(o1), "ALA")
	TEST_EQUAL(t.str(o3), "GLY")
	TEST_EQUAL(t.str(0u), "")
RESULT

CHECK(D57 -- ChildRef ordered edges: append / prepend / insertBefore / insertAfter / remove)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	std::uint32_t parent = t.allocate(ContainerKind::CHAIN);

	// Append three atom children: order [10, 20, 30].
	t.append_child(parent, ChildRef(ChildRef::ATOM, 10));
	t.append_child(parent, ChildRef(ChildRef::ATOM, 20));
	t.append_child(parent, ChildRef(ChildRef::ATOM, 30));
	TEST_EQUAL(t.row(parent).children.size(), 3u)
	TEST_EQUAL(t.row(parent).children[0].idx, 10u)
	TEST_EQUAL(t.row(parent).children[2].idx, 30u)

	// Reverse edge for atoms.
	TEST_EQUAL(t.atom_parent(10), parent)
	TEST_EQUAL(t.atom_parent(20), parent)
	TEST_EQUAL(t.atom_parent(99), ContainerTable::NONE)

	// Prepend -> [5, 10, 20, 30].
	t.prepend_child(parent, ChildRef(ChildRef::ATOM, 5));
	TEST_EQUAL(t.row(parent).children[0].idx, 5u)
	TEST_EQUAL(t.row(parent).children[1].idx, 10u)

	// insertBefore(15, before 20) -> [5, 10, 15, 20, 30].
	t.insert_child_before(parent, ChildRef(ChildRef::ATOM, 15), ChildRef(ChildRef::ATOM, 20));
	TEST_EQUAL(t.row(parent).children[2].idx, 15u)
	TEST_EQUAL(t.row(parent).children[3].idx, 20u)

	// insertAfter(25, after 20) -> [5, 10, 15, 20, 25, 30].
	t.insert_child_after(parent, ChildRef(ChildRef::ATOM, 25), ChildRef(ChildRef::ATOM, 20));
	TEST_EQUAL(t.row(parent).children.size(), 6u)
	TEST_EQUAL(t.row(parent).children[4].idx, 25u)
	TEST_EQUAL(t.row(parent).children[5].idx, 30u)

	// remove(15) -> [5, 10, 20, 25, 30]; reverse edge cleared.
	bool removed = t.remove_child(parent, ChildRef(ChildRef::ATOM, 15));
	TEST_EQUAL(removed, true)
	TEST_EQUAL(t.row(parent).children.size(), 5u)
	TEST_EQUAL(t.atom_parent(15), ContainerTable::NONE)
	TEST_EQUAL(t.row(parent).children[2].idx, 20u)

	// remove of a missing child returns false.
	TEST_EQUAL(t.remove_child(parent, ChildRef(ChildRef::ATOM, 999)), false)

	// CONTAINER child sets parent_container_idx reverse edge.
	std::uint32_t child_c = t.allocate(ContainerKind::RESIDUE);
	t.append_child(parent, ChildRef(ChildRef::CONTAINER, child_c));
	TEST_EQUAL(t.row(child_c).parent_container_idx, parent)
	t.remove_child(parent, ChildRef(ChildRef::CONTAINER, child_c));
	TEST_EQUAL(t.row(child_c).parent_container_idx, ContainerTable::NONE)
RESULT

CHECK(D59 -- selection counters propagate up the parent chain)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;

	// protein -> chain -> residue, with two atoms under the residue.
	std::uint32_t prot = t.allocate(ContainerKind::PROTEIN);
	std::uint32_t ch   = t.allocate(ContainerKind::CHAIN);
	std::uint32_t res  = t.allocate(ContainerKind::RESIDUE);
	t.append_child(prot, ChildRef(ChildRef::CONTAINER, ch));
	t.append_child(ch,   ChildRef(ChildRef::CONTAINER, res));
	t.append_child(res,  ChildRef(ChildRef::ATOM, 100));
	t.append_child(res,  ChildRef(ChildRef::ATOM, 101));

	TEST_EQUAL(t.row(res).selection_count, 0u)

	// Select both atoms: bump up from the residue twice.
	t.bump_selection_up(res, +1);
	t.bump_selection_up(res, +1);
	TEST_EQUAL(t.row(res).selection_count, 2u)
	TEST_EQUAL(t.row(ch).selection_count, 2u)
	TEST_EQUAL(t.row(prot).selection_count, 2u)

	// Deselect one.
	t.bump_selection_up(res, -1);
	TEST_EQUAL(t.row(res).selection_count, 1u)
	TEST_EQUAL(t.row(ch).selection_count, 1u)
	TEST_EQUAL(t.row(prot).selection_count, 1u)

	// Underflow clamps at 0 (defensive against unbalanced deselect).
	t.bump_selection_up(res, -5);
	TEST_EQUAL(t.row(res).selection_count, 0u)
	TEST_EQUAL(t.row(prot).selection_count, 0u)
RESULT

CHECK(D58/D59 -- container property column keyed by container_idx)
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;
	std::uint32_t res = t.allocate(ContainerKind::RESIDUE);

	// Residue flag bits (AMINO_ACID etc.) live as a container property,
	// not a bespoke payload field (D59 / R31 P31-3).
	PropertyColumnBase* col =
		t.container_properties_.registerColumn("RESIDUE_FLAGS", PropertyColumnType::INT);
	TEST_NOT_EQUAL(col, (PropertyColumnBase*)nullptr)
	auto* int_col = static_cast<PropertyColumn<std::int32_t>*>(col);
	int_col->set(res, 0x5);
	TEST_EQUAL(int_col->isSet(res), true)
	TEST_EQUAL(int_col->get(res), 0x5)
RESULT

CHECK(D60 -- v0 tree mirrors faithfully into the ContainerTable)
	// Build a representative v0 hierarchy (the heavy PDB path): System ->
	// Protein -> Chain -> { Residue ALA(ins A) [N, CA], Residue GLY [C],
	// SecondaryStructure HELIX -> Residue SER [O] }. v0 typed-insert API:
	// Residue holds PDBAtom, Chain holds Residue+SecondaryStructure,
	// SecondaryStructure holds Residue, System holds Molecule (Protein
	// IS-A Molecule). The v0 object tree is the source of truth (D60).
	System sys;
	Protein prot;  prot.setName("PROT");  prot.setID("1ABC");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA"); r1.setID("ALA"); r1.setInsertionCode('A');
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
	sys.insert(prot);

	// Mirror the Protein subtree into a standalone table.
	MoleculeStore mirror_store;
	ContainerTable& t = mirror_store.sideTables_().container_table_;
	std::uint32_t root = mirror(prot, t);

	// Structure + order + payload parity: flattened preorder must match.
	std::string v0_desc;
	descV0(prot, v0_desc);
	std::string table_desc = descTable(t, root);
	TEST_EQUAL(table_desc, v0_desc)

	// Reverse-edge parity: every CONTAINER child's parent_container_idx
	// points back at its parent; every ATOM child's atom_parent matches.
	// Root protein has no parent.
	TEST_EQUAL(t.row(root).parent_container_idx, ContainerTable::NONE)
	std::uint32_t chain_idx = t.row(root).children[0].idx;
	TEST_EQUAL(t.row(chain_idx).parent_container_idx, root)
	std::uint32_t r1_idx = t.row(chain_idx).children[0].idx;
	TEST_EQUAL(t.row(r1_idx).parent_container_idx, chain_idx)
	// r1's first atom child reverse edge.
	const ChildRef first_atom = t.row(r1_idx).children[0];
	TEST_EQUAL(first_atom.kind == ChildRef::ATOM, true)
	TEST_EQUAL(t.atom_parent(first_atom.idx), r1_idx)

	// Payload spot checks.
	TEST_EQUAL(t.row(root).kind == ContainerKind::PROTEIN, true)
	TEST_EQUAL(t.str(t.row(root).payload.id_offset), "1ABC")
	TEST_EQUAL(t.row(r1_idx).kind == ContainerKind::RESIDUE, true)
	TEST_EQUAL(t.str(t.row(r1_idx).payload.id_offset), "ALA")
	TEST_EQUAL(t.row(r1_idx).payload.insertion_code, 'A')
RESULT

CHECK(D56 -- detached subtree migrates orphan->System table)
	// Build a small detached v0 Chain subtree (atoms live in the orphan
	// store, so they still carry store indices) and mirror it into a
	// "source" table.
	Chain ch; ch.setName("A");
	Residue r1; r1.setName("ALA"); r1.setID("ALA");
	Residue r2; r2.setName("GLY"); r2.setID("GLY");
	PDBAtom a1; a1.setName("N");
	PDBAtom a2; a2.setName("CA");
	r1.insert(a1); r2.insert(a2);
	ch.insert(r1); ch.insert(r2);

	MoleculeStore src_store;
	ContainerTable& src = src_store.sideTables_().container_table_;
	std::uint32_t src_root = mirror(ch, src);
	std::string src_desc = descTable(src, src_root);
	std::size_t src_live_before = src.live_count();   // chain + 2 residues = 3
	TEST_EQUAL(src_live_before, 3u)

	// Migrate into a destination table with a COMPLETE atom remap (here an
	// identity map -- the explicit same-index/test mode; the real
	// System::adoptSubtree supplies the actual atom-slot remap). Build it
	// by collecting every migrated atom (a complete map is mandatory: a
	// missing entry throws, per R32 HIGH).
	MoleculeStore dst_store;
	ContainerTable& dst = dst_store.sideTables_().container_table_;
	std::unordered_map<std::uint32_t, std::uint32_t> identity;
	src.preorder(src_root,
		[](std::uint32_t){},
		[&](std::uint32_t aidx){ identity[aidx] = aidx; });

	std::uint32_t dst_root = dst.migrate_subtree_from(src, src_root, identity);

	// Migrated subtree reproduces the source description (same atom idx
	// under identity remap) and the new root is detached (parent NONE).
	TEST_EQUAL(descTable(dst, dst_root), src_desc)
	TEST_EQUAL(dst.row(dst_root).parent_container_idx, ContainerTable::NONE)
	// Reverse edges rebuilt in the destination table.
	std::uint32_t dst_r1 = dst.row(dst_root).children[0].idx;
	TEST_EQUAL(dst.row(dst_r1).parent_container_idx, dst_root)
	const ChildRef dst_atom = dst.row(dst_r1).children[0];
	TEST_EQUAL(dst.atom_parent(dst_atom.idx), dst_r1)

	// R32 HIGH: it is a true MOVE -- the source subtree is released. All
	// source rows freed and source atom reverse edges cleared.
	TEST_EQUAL(src.live_count(), 0u)
	TEST_EQUAL(src.is_freed(src_root), true)
	TEST_EQUAL(src.atom_parent(a1.getStoreIndex()), ContainerTable::NONE)
	TEST_EQUAL(src.atom_parent(a2.getStoreIndex()), ContainerTable::NONE)
RESULT

CHECK(R32 -- reparent_child does remove-before-add (no stale old-parent edge))
	MoleculeStore store;
	ContainerTable& t = store.sideTables_().container_table_;
	std::uint32_t chA = t.allocate(ContainerKind::CHAIN);
	std::uint32_t chB = t.allocate(ContainerKind::CHAIN);
	std::uint32_t res = t.allocate(ContainerKind::RESIDUE);

	// res starts under chA, with one atom under res.
	t.append_child(chA, ChildRef(ChildRef::CONTAINER, res));
	t.append_child(res, ChildRef(ChildRef::ATOM, 7));
	TEST_EQUAL(t.row(res).parent_container_idx, chA)
	TEST_EQUAL(t.row(chA).children.size(), 1u)

	// Reparent the container child chA->chB: chA loses it, chB gains it,
	// reverse edge updated, NO stale edge left in chA.
	t.reparent_child(chB, ChildRef(ChildRef::CONTAINER, res));
	TEST_EQUAL(t.row(chA).children.size(), 0u)
	TEST_EQUAL(t.row(chB).children.size(), 1u)
	TEST_EQUAL(t.row(chB).children[0].idx, res)
	TEST_EQUAL(t.row(res).parent_container_idx, chB)

	// Reparent an atom child likewise: old parent edge removed.
	std::uint32_t res2 = t.allocate(ContainerKind::RESIDUE);
	t.reparent_child(res2, ChildRef(ChildRef::ATOM, 7));
	TEST_EQUAL(t.atom_parent(7), res2)
	TEST_EQUAL(t.row(res).children.size(), 0u)
	TEST_EQUAL(t.row(res2).children.size(), 1u)
RESULT

CHECK(layout pins -- ChildRef / ContainerPayload small + ContainerRow NONE sentinel)
	// Pin the small-struct sizes so a layout regression shows up in ctest.
	TEST_EQUAL(sizeof(ChildRef) <= 8u, true)
	TEST_EQUAL(ContainerRow::NONE, 0xFFFFFFFFu)
	TEST_EQUAL(ContainerTable::NONE, 0xFFFFFFFFu)
RESULT

// ===== H2a: the PRODUCTION mirror (adoption materialisation + removeChild
// hook + destruction guard), exercised through the real v0 API rather than
// the by-hand mirror() helper above. =====

CHECK(H2a -- production adoption (System::insert) mirrors the v0 tree)
	System sys;
	Protein prot;  prot.setName("PROT");  prot.setID("1ABC");
	Chain   ch;    ch.setName("A");
	Residue r1;    r1.setName("ALA"); r1.setID("ALA"); r1.setInsertionCode('A');
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
	const ContainerTable& t = store->sideTables_().container_table_;

	std::string v0_desc; descV0(prot, v0_desc);
	TEST_EQUAL(descTable(t, root), v0_desc)
	TEST_EQUAL(t.row(root).parent_container_idx, ContainerTable::NONE)  // root
RESULT

CHECK(H2a -- removeChild mirrors a container + an atom detach)
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
	const ContainerTable& t = store->sideTables_().container_table_;

	// Detach a residue (container child) from the chain.
	TEST_EQUAL(ch.removeChild(r2), true)
	std::string d1; descV0(prot, d1);
	TEST_EQUAL(descTable(t, root), d1)

	// Detach an atom from a residue.
	TEST_EQUAL(r1.removeChild(a1), true)
	std::string d2; descV0(prot, d2);
	TEST_EQUAL(descTable(t, root), d2)
RESULT

CHECK(H2a -- destroy() on a LIVE container mirrors its detach from the live parent)
	System sys;
	Protein prot; prot.setName("P");
	Chain ch; ch.setName("A");
	Residue r; r.setName("ALA"); r.setID("ALA");
	PDBAtom a; a.setName("N");
	r.insert(a);
	ch.insert(r);
	prot.insert(ch);
	sys.insert(prot);

	MoleculeStore* store = ch.getContainerRowStore_();
	std::uint32_t ch_row = ch.getContainerRow_();
	TEST_EQUAL(store->container_child_count_(ch_row), 1u)   // chain has residue r

	// destroy() on a LIVE residue first detaches it from its LIVE parent
	// (chain.removeChild(r)) -> the H2a hook mirrors that (chain row loses
	// r), since the chain is not being destroyed (D69). NB: emptying r's
	// OWN row depends on the clear()/destroyChildren_ direct-detach branch
	// for non-auto-deletable children, which is H2b scope (D70/D72) -- not
	// asserted here.
	r.destroy();
	TEST_EQUAL(store->container_child_count_(ch_row), 0u)
RESULT

CHECK(H2a -- delete cascade detaches from the live parent + is heap-safe)
	System sys;
	Protein prot; prot.setName("P");
	Chain ch; ch.setName("A");
	prot.insert(ch);
	sys.insert(prot);

	MoleculeStore* store = ch.getContainerRowStore_();
	std::uint32_t ch_row = ch.getContainerRow_();

	Residue* rh = new Residue; rh->setName("HEAP"); rh->setID("HIS");
	PDBAtom* ah = new PDBAtom; ah->setName("CA");
	rh->insert(*ah);
	ch.insert(*rh);
	TEST_EQUAL(store->container_child_count_(ch_row), 1u)

	// ~Residue sets being_destroyed_ then detaches from the LIVE chain
	// (mirrors: chain loses rh), then deletes its atom WITHOUT mirroring
	// (rh being destroyed -> P2.1.1 trap avoided). Must not crash.
	delete rh;
	TEST_EQUAL(store->container_child_count_(ch_row), 0u)
RESULT

CHECK(H2a -- ~System wholesale teardown is crash-free under the guard)
	{
		System sys;
		Protein* prot = new Protein; prot->setName("P");
		Chain*   ch   = new Chain;   ch->setName("A");
		Residue* r    = new Residue; r->setName("ALA");
		PDBAtom* a    = new PDBAtom; a->setName("N");
		r->insert(*a);
		ch->insert(*r);
		prot->insert(*ch);
		sys.insert(*prot);
	}  // ~System cascade-tears-down the owned subtree; every removeChild on
	   // a being-destroyed parent skips the mirror (D69). No crash == pass.
	TEST_EQUAL(true, true)
RESULT

END_TEST
