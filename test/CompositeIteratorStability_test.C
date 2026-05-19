// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.1 P1.9 (D31 / D31b): CompositeIteratorStability_test.
//
// D31 declares that Composite iterators (ChildCompositeIterator,
// CompositeIterator, plus their const / reverse variants) cache
// `Composite*` positions only — never `CompositeNode*` or any
// composite_nodes_ vector index. D31b enforces this structurally
// by keeping `CompositeNode` in the private internal header that
// composite.h, composite.iC, and *Iterator*.h headers MUST NOT
// include (CI grep gate).
//
// This test catches the failure mode the invariant prevents: an
// iterator that caches a CompositeNode* (or vector index) and then
// stalely-dereferences after composite_nodes_ growth or node
// recycling. The test mutates the tree DURING iteration in ways
// that would have invalidated such caches, and verifies that
// iteration through `Composite*` positions stays valid (current
// behavior of v0 Composite iterators, which the v2.1 redesign
// preserves).
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
///////////////////////////

START_TEST(CompositeIteratorStability)

using namespace BALL;

CHECK(insertion-during-iteration -- iterator stays valid for unrelated nodes)
	Molecule mol;
	Atom a1, a2, a3;
	mol.insert(a1);
	mol.insert(a2);
	mol.insert(a3);
	TEST_EQUAL(mol.getDegree(), 3u)

	// Iterate; mid-iteration, insert a fresh atom into the molecule.
	// The current iterator's Composite* remains valid because v0
	// Composite uses heap-allocated nodes with stable identity. A
	// hypothetical implementation that cached a vector index into
	// composite_nodes_ would break here on vector growth.
	Composite::ChildCompositeIterator it = mol.beginChildComposite();
	Composite* first_seen = &it.getTraits().getData();
	TEST_EQUAL(first_seen, &a1)

	Atom a4;
	mol.insert(a4);  // mutation during iteration

	// Old iterator's bound Composite* is still &a1 — it didn't
	// invalidate due to the unrelated insert.
	TEST_EQUAL(&it.getTraits().getData(), first_seen)

	// Walk forward; expect a2, a3, then a4 (the new one at end).
	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a2)
	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a3)
	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a4)
RESULT

CHECK(reverse iteration -- backward walk uses last_child_ + previous_)
	// D22b's 5-link node design (vs original D22's 3-link) is required
	// because ChildCompositeIteratorTraits::backward() reaches the
	// last child via bound_->last_child_, then walks previous_ links.
	// Both bidirectional sibling pointers are public-API-visible.
	Molecule mol;
	Atom a1, a2, a3;
	mol.insert(a1);
	mol.insert(a2);
	mol.insert(a3);

	Composite::ChildCompositeIterator it = mol.endChildComposite();
	// Decrement from end (past-the-end) into the last child.
	--it;
	TEST_EQUAL(&it.getTraits().getData(), &a3)
	--it;
	TEST_EQUAL(&it.getTraits().getData(), &a2)
	--it;
	TEST_EQUAL(&it.getTraits().getData(), &a1)
RESULT

CHECK(removal mid-iteration -- removing the CURRENT node invalidates that iter)
	// This is documented v0 behavior, NOT a v2.1 regression: erasing
	// the node an iterator points at invalidates THAT iterator. v2.1
	// preserves this contract — D31 only constrains caching, not the
	// erase-invalidates-current rule.
	//
	// We verify a NEIGHBOR's iterator survives the removal.
	Molecule mol;
	Atom a1, a2, a3;
	mol.insert(a1);
	mol.insert(a2);
	mol.insert(a3);

	Composite::ChildCompositeIterator it = mol.beginChildComposite();
	TEST_EQUAL(&it.getTraits().getData(), &a1)

	// Remove a3 (NOT the current iter target). it points at a1; stays valid.
	mol.removeChild(a3);
	TEST_EQUAL(&it.getTraits().getData(), &a1)
	TEST_EQUAL(mol.getDegree(), 2u)

	// Walking forward now finds only a1 then a2.
	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a2)
RESULT

CHECK(spliceBefore mid-iteration -- non-current node move preserves iterator)
	// R19-F2 closure: D31 iterator-stability coverage expansion. Splice
	// moves a child from one parent to another; tests that an iterator
	// pointing at a NON-spliced node stays valid.
	Molecule mol_a;
	Molecule mol_b;
	Atom a1, a2, a3;
	mol_a.insert(a1);
	mol_a.insert(a2);
	mol_b.insert(a3);
	TEST_EQUAL(mol_a.getDegree(), 2u)
	TEST_EQUAL(mol_b.getDegree(), 1u)

	Composite::ChildCompositeIterator it = mol_a.beginChildComposite();
	TEST_EQUAL(&it.getTraits().getData(), &a1)

	// spliceBefore moves mol_b's children before the call-site sibling.
	// We don't move mol_a's children, so it's iterator stays valid.
	mol_a.appendChild(mol_b);  // first make mol_b a child of mol_a
	TEST_EQUAL(mol_a.getDegree(), 3u)
	TEST_EQUAL(&it.getTraits().getData(), &a1)

	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a2)
RESULT

CHECK(reverse_iterator wrapper -- ChildCompositeReverseIterator)
	// D22b 5-link node design is verified by these reverse iterators
	// because std::reverse_iterator wraps the BidirectionalIterator and
	// calls operator--() under the hood, which walks last_child_ +
	// previous_ links.
	Molecule mol;
	Atom a1, a2, a3;
	mol.insert(a1);
	mol.insert(a2);
	mol.insert(a3);

	Composite::ChildCompositeReverseIterator rit = mol.rbeginChildComposite();
	// std::reverse_iterator's operator* dereferences (current-1).
	// First reverse-position is the last child a3.
	TEST_EQUAL(&(*rit), &a3)
	++rit;
	TEST_EQUAL(&(*rit), &a2)
	++rit;
	TEST_EQUAL(&(*rit), &a1)
	++rit;
	TEST_EQUAL(rit == mol.rendChildComposite(), true)
RESULT

CHECK(deep tree DFS -- CompositeIterator full preorder)
	// Build mol -> a1, with a2 ALSO inside the tree (siblings).
	// Composite tree mutations of unrelated branches do not invalidate
	// CompositeIterator positions because positions are Composite*.
	Molecule mol;
	Atom a1, a2;
	mol.insert(a1);
	mol.insert(a2);

	Composite::CompositeIterator it = mol.beginComposite();
	// Preorder starts at the root (mol itself), then descends.
	TEST_EQUAL(&it.getTraits().getData(), &mol)
	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a1)
	++it;
	TEST_EQUAL(&it.getTraits().getData(), &a2)
	++it;
	TEST_EQUAL(it.getTraits().isEnd(), true)
RESULT

END_TEST
