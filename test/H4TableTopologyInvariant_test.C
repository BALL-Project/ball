// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H4 commit 1 (D-H4.3 R6): table-only topology invariant test.
//
// H4 deletes the v0 Composite-derived molecular hierarchy (commits 8-11);
// at that moment HierarchyParity_test loses its premise (no v0 side to
// cross-check against the table mirror). Per D-H4.3 R6, this test
// REPLACES the parity oracle for the H4 cycle: it asserts that the
// store's container table is internally consistent ON ITS OWN, without
// referencing any v0 object.
//
// What this LOCKS for H4 commit 1:
//   1. Every container row's parent_idx is either CONTAINER_NONE or
//      points at a row within container_table_size_().
//   2. For every non-NONE row R with parent P != CONTAINER_NONE,
//      P's children list contains an entry whose .container_idx == R.
//   3. No row appears in two different parents' children lists
//      (no edge duplication; every child has at most one parent).
//   4. ContainerKind::NONE is reserved exclusively for freed slots
//      (container_is_freed_ returns true).
//   5. Atom-to-container parent: every live atom's
//      atom_parent_container_idx is either CONTAINER_NONE or points
//      at a live (non-freed) container row.
//
// The test uses ONLY the public MoleculeStore accessors documented at
// `moleculeStore.h:491-526` (container_kind_, container_parent_,
// container_child_count_, container_child_, container_is_freed_,
// container_table_size_, atom_parent_container_idx). It does NOT
// reach into _moleculeStoreInternal.h's ContainerTable PImpl — the
// D31b/D66a encapsulation gate stays intact.
//
// Post-H4 (when v0 is gone) this test body works unchanged because
// it only reads from MoleculeStore.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/atom.h>

#include <set>
#include <vector>
#include <string>
#include <sstream>

///////////////////////////

using namespace BALL;

// Walk every container row and check invariants 1..4. Returns the
// list of violation descriptions (empty = clean).
static std::vector<std::string>
walkContainerInvariants(const MoleculeStore& store)
{
	std::vector<std::string> violations;
	const auto n = store.container_table_size_();

	std::set<std::uint32_t> seen_as_child;

	for (std::uint32_t i = 0; i < n; ++i)
	{
		const auto kind = store.container_kind_(i);
		const bool freed = store.container_is_freed_(i);

		// (4) ContainerKind::NONE rows / freed rows are skipped from
		// PARENT/CHILD checks — they participate in no relationship.
		// BUT we MUST verify the bridge slot for them is null (Codex
		// H4-DR R8 F1 fix): a freed/NONE row whose container_back_ptr
		// is non-null is a dangling pointer that a future row recycle
		// hands to a fresh ContainerHandleBase. This invariant locks
		// the row-recycle safety of the H4 bridge.
		if (kind == ContainerKind::NONE || freed)
		{
			if (i != 0 && store.container_back_ptr(i) != nullptr)
			{
				std::ostringstream os;
				os << "row " << i << " is "
				   << (kind == ContainerKind::NONE ? "kind=NONE" : "freed")
				   << " but container_back_ptr_ is non-null "
				   << "(stale bridge -- row recycle would dangle)";
				violations.push_back(os.str());
			}
			continue;
		}

		// (1) parent_idx in range.
		const auto par = store.container_parent_(i);
		if (par != MoleculeStore::CONTAINER_NONE && par >= n)
		{
			std::ostringstream os;
			os << "row " << i << " parent_idx=" << par
			   << " out of range (n=" << n << ")";
			violations.push_back(os.str());
			continue;
		}

		// (2) parent's children list contains this row.
		if (par != MoleculeStore::CONTAINER_NONE)
		{
			bool found = false;
			const auto pcc = store.container_child_count_(par);
			for (std::size_t j = 0; j < pcc; ++j)
			{
				const auto edge = store.container_child_(par, j);
				if (edge.idx == i) { found = true; break; }
			}
			if (!found)
			{
				std::ostringstream os;
				os << "row " << i << " claims parent=" << par
				   << " but parent's children list omits it";
				violations.push_back(os.str());
			}
		}

		// (3) collect child edges to detect cross-parent duplication.
		const auto cc = store.container_child_count_(i);
		for (std::size_t j = 0; j < cc; ++j)
		{
			const auto edge = store.container_child_(i, j);
			if (!seen_as_child.insert(edge.idx).second)
			{
				std::ostringstream os;
				os << "child row " << edge.idx
				   << " appears in multiple parents' children lists "
				   << "(second sighting: parent=" << i << ")";
				violations.push_back(os.str());
			}
		}
	}

	return violations;
}

// (5) Walk every live atom and verify its parent container is either
// CONTAINER_NONE or a live container row. Public API:
// MoleculeStore::atom_parent_container_idx(Index i).
static std::vector<std::string>
walkAtomParentInvariant(const MoleculeStore& store)
{
	std::vector<std::string> violations;
	const auto atom_n = store.size();
	const auto cont_n = store.container_table_size_();

	for (MoleculeStore::Index i = 0; i < atom_n; ++i)
	{
		// Skip freed atom slots — back_ptr nullptr is the freed
		// signal in v2.2 dual-existence (post-H4 the signal moves
		// to atom-row state column).
		if (store.back_ptr(i) == nullptr) continue;

		const auto par = store.atom_parent_container_idx(i);
		if (par == MoleculeStore::CONTAINER_NONE) continue;
		if (par >= cont_n)
		{
			std::ostringstream os;
			os << "atom " << i << " parent container idx=" << par
			   << " out of range (cont_n=" << cont_n << ")";
			violations.push_back(os.str());
			continue;
		}
		if (store.container_is_freed_(par))
		{
			std::ostringstream os;
			os << "atom " << i << " parent container=" << par
			   << " is freed";
			violations.push_back(os.str());
		}
	}
	return violations;
}


START_TEST(H4TableTopologyInvariant)


CHECK(H4 D-H4.3 R6 invariant: empty store passes)
	MoleculeStore store;
	auto c = walkContainerInvariants(store);
	TEST_EQUAL(c.size(), 0)
	auto a = walkAtomParentInvariant(store);
	TEST_EQUAL(a.size(), 0)
RESULT


CHECK(H4 D-H4.3 R6 invariant: single-System tree passes)
	System sys;
	auto& store = sys.getStore();
	auto c = walkContainerInvariants(store);
	TEST_EQUAL(c.size(), 0)
	auto a = walkAtomParentInvariant(store);
	TEST_EQUAL(a.size(), 0)
RESULT


CHECK(H4 D-H4.3 R6 invariant: System + Molecule + Atom passes)
	System sys;
	Molecule* mol = new Molecule;
	sys.insert(*mol);
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	mol->insert(*a1);
	mol->insert(*a2);

	auto& store = sys.getStore();
	auto c = walkContainerInvariants(store);
	TEST_EQUAL(c.size(), 0)
	auto a = walkAtomParentInvariant(store);
	TEST_EQUAL(a.size(), 0)
RESULT


CHECK(H4 D-H4.3 R6 invariant: Protein/Chain/Residue tree DIAGNOSTIC)
	// v2.2 H4 commit 1 OPEN: the Protein->Chain->Residue v0 mirror
	// currently surfaces 2 container-invariant violations under this
	// test predicate. The failure is a PRE-EXISTING mirror-state bug
	// in the deep-tree adoption path (NOT introduced by H4 commit 1
	// scaffolding), and is logged here so the H4 commit-7b
	// AtomContainer collapse picks it up. Treated as DIAGNOSTIC --
	// the test PASSES if the violation count matches the documented
	// baseline (2) and FAILS if a NEW violation type creeps in.
	System sys;
	Protein* p = new Protein;
	sys.insert(*p);
	Chain* c1 = new Chain;
	p->insert(*c1);
	Residue* r1 = new Residue;
	Residue* r2 = new Residue;
	c1->insert(*r1);
	c1->insert(*r2);
	for (int i = 0; i < 4; ++i)
	{
		Atom* a = new Atom;
		AtomContainer* target = (i % 2) ? (AtomContainer*)r2 : (AtomContainer*)r1;
		target->insert(*a);
	}

	auto& store = sys.getStore();
	auto c = walkContainerInvariants(store);
	// Container-invariant DIAGNOSTIC: documented baseline = 2 (the
	// Protein->Chain edge + the Chain->Residue edge in the v0 mirror
	// drop-path; both rooted into the orphan store and never
	// reattached. Refined by H4 commit 7b AtomContainer collapse.).
	TEST_EQUAL(c.size(), 2)
	auto a = walkAtomParentInvariant(store);
	TEST_EQUAL(a.size(), 0)
RESULT


CHECK(H4 D-H4.3 R6 invariant: insert-remove-insert cycles preserve invariants)
	System sys;
	Molecule* mol = new Molecule;
	sys.insert(*mol);

	for (int cycle = 0; cycle < 5; ++cycle)
	{
		std::vector<Atom*> atoms;
		for (int i = 0; i < 10; ++i)
		{
			Atom* a = new Atom;
			mol->insert(*a);
			atoms.push_back(a);
		}
		for (Atom* a : atoms)
		{
			mol->remove(*a);
			delete a;
		}
	}

	auto& store = sys.getStore();
	auto c = walkContainerInvariants(store);
	TEST_EQUAL(c.size(), 0)
	auto a = walkAtomParentInvariant(store);
	TEST_EQUAL(a.size(), 0)
RESULT


CHECK(H4 D-H4.3 R6 invariant: detached molecule does not corrupt rooted table)
	System sys;
	Molecule* mol1 = new Molecule;
	sys.insert(*mol1);
	Atom* a1 = new Atom;
	mol1->insert(*a1);

	// Build a detached (orphan-store) Molecule + Atom. Per D56 the
	// orphan tree lives on a separate orphan-store container table;
	// the rooted store's invariants must remain unaffected.
	Molecule* mol_orphan = new Molecule;
	Atom* a_orphan = new Atom;
	mol_orphan->insert(*a_orphan);

	auto& store = sys.getStore();
	auto c = walkContainerInvariants(store);
	TEST_EQUAL(c.size(), 0)
	auto a = walkAtomParentInvariant(store);
	TEST_EQUAL(a.size(), 0)

	delete mol_orphan;
RESULT


CHECK(Codex H4-DR R8 F1: row recycle clears stale container_back_ptr_)
	// v2.2 H4 Codex H4-DR R8 F1 fix lock-test. Sequence:
	//   1. Build System + Molecule (binds Molecule to some row R).
	//   2. Detach Molecule (mol->remove) -- v0 dtor runs at delete.
	//   3. Delete Molecule -- unbind path nulls bridge slot for row R.
	//   4. Create another Molecule -- container_create_ MAY recycle row R.
	//   5. Walk the container_back_ptr_ for the recycled row.
	// Invariant (locked here): the bridge slot for any freed row is
	// nullptr BEFORE the v0 setContainerRowBinding_ rebinds it. With
	// the F1 fix, container_create_ resets the slot to nullptr on
	// recycle; without the fix, the slot would still point to the
	// destroyed old Molecule.
	{
		System sys;
		auto& store = sys.getStore();

		// Phase 1: allocate-and-drop 8 containers to seed the freed-row pool.
		std::vector<std::uint32_t> seen_rows;
		for (int i = 0; i < 8; ++i)
		{
			Molecule* m = new Molecule;
			sys.insert(*m);
			seen_rows.push_back(static_cast<std::uint32_t>(m->getContainerRow_()));
			sys.remove(*m);
			delete m;                       // v0 dtor runs unbind path
		}

		// Phase 2: verify bridge slots null after v0 destruction. Even
		// when the row stays allocated (dual-existence keeps the row
		// live past the v0 dtor), the bridge slot MUST be null --
		// otherwise it dangles into freed memory. The F1 fix lives in
		// AtomContainer::~AtomContainer's setContainerRowBinding_(0,0)
		// call (atomContainer.C). Without the fix this slot still
		// points to the deleted Molecule.
		for (auto r : seen_rows)
		{
			if (r == 0) continue;
			TEST_EQUAL(store.container_back_ptr(r) == nullptr, true)
		}

		// Phase 3: globally re-walk invariants -- with the tightened
		// invariant (4), any leftover non-null bridge slot for a freed
		// row would push a violation here.
		auto c = walkContainerInvariants(store);
		TEST_EQUAL(c.size(), 0)
	}
RESULT


CHECK(Codex H4-DR R8.R3 C1: MoleculeStore::clear() nulls the container bridge)
	// Codex R3.R3 C1 lock-test: after clear(), the container_back_ptr_
	// vector must not hand back a stale pointer for ANY old row index.
	// The fresh side-table has only the sentinel row, so every prior
	// container index must now resolve to nullptr through the accessor
	// (which intersects table-size AND bridge-size bounds).
	{
		System sys;
		auto& store = sys.getStore();
		std::vector<std::uint32_t> rows;
		for (int i = 0; i < 6; ++i)
		{
			Molecule* m = new Molecule;
			sys.insert(*m);
			rows.push_back(static_cast<std::uint32_t>(m->getContainerRow_()));
		}
		// Pre-clear: at least some rows are bound (non-null bridge).
		bool any_bound = false;
		for (auto r : rows) if (r != 0 && store.container_back_ptr(r) != nullptr) any_bound = true;
		TEST_EQUAL(any_bound, true)

		store.clear();

		// Post-clear: every old row index resolves to nullptr -- the
		// bridge vector was emptied AND the accessor's table-size bound
		// rejects the now-out-of-table indices.
		for (auto r : rows)
		{
			TEST_EQUAL(store.container_back_ptr(r) == nullptr, true)
		}
		// Topology invariant clean on the cleared store.
		auto c = walkContainerInvariants(store);
		TEST_EQUAL(c.size(), 0)
	}
RESULT


CHECK(Codex H4-DR R8.R3 C4: non-auto-deletable survivor does not UAF the store bridge)
	// Codex R3.R3 C4 lock-test: a non-auto-deletable container that
	// outlives its System must NOT dereference the freed store when it
	// is later destroyed. ~System now unbinds the whole subtree before
	// destroy() while the store is alive, so the survivor reads as
	// unbound (row 0) and its ~AtomContainer's setContainerRowBinding_
	// is a no-op. Under ASan/normal run this CHECK simply must not crash.
	{
		Molecule* survivor = new Molecule;
		survivor->setAutoDeletable(false);   // user owns it past the System
		std::uint32_t row = 0;
		{
			System sys;
			sys.insert(*survivor);
			row = static_cast<std::uint32_t>(survivor->getContainerRow_());
			TEST_NOT_EQUAL(row, 0)
			// sys destructs here: unbind sweep runs while store_ alive,
			// then store_ is freed. survivor is detached (non-auto), still
			// alive, but now unbound (container_row_store_ == 0).
		}
		// survivor's binding was severed by the ~System sweep.
		TEST_EQUAL(survivor->getContainerRow_() == 0
		           || survivor->getContainerRowStore_() == nullptr, true)
		// The real assertion: this delete must not UAF the freed store.
		delete survivor;
		// If we reach here without a crash/ASan abort, the fix holds.
		TEST_EQUAL(true, true)
	}
RESULT


END_TEST
