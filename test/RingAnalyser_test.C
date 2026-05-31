// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(RingAnalyser)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

RingAnalyser* ra;

CHECK(RingAnalyser())
	ra = new RingAnalyser;
	TEST_NOT_EQUAL(ra, 0)
RESULT

CHECK(~RingAnalyser())
	delete ra;
RESULT

// BUG-540: RingAnalyser::sequenceRing must terminate even when the supplied ring
// vector cannot be fully threaded into a single cycle. Against the unfixed loop
// (`while(!ring_atoms.empty())` with no no-progress guard) this CHECK hangs
// forever: once the running chain runs out of bound neighbours the inner for-loop
// erases nothing and the while-condition never changes. The fix breaks the while
// when a full pass threads no atom. The real assertion here is "control returns"
// (the test process terminates) -- the ctest timeout is the safety net.
CHECK(sequenceRing() terminates on a non-threadable ring [BUG-540])
	Molecule mol;

	// Four atoms: a0-a1-a2 form a chain, a3 is bonded only to a0.
	// Passed as a "ring" {a0, a1, a2, a3}, the running chain a0->a1->a2 cannot
	// reach a3 (a3 is not bound to a2), so the old loop would spin forever.
	Atom* a0 = new Atom; a0->setElement(PTE[Element::C]);
	Atom* a1 = new Atom; a1->setElement(PTE[Element::C]);
	Atom* a2 = new Atom; a2->setElement(PTE[Element::C]);
	Atom* a3 = new Atom; a3->setElement(PTE[Element::C]);
	mol.insert(*a0);
	mol.insert(*a1);
	mol.insert(*a2);
	mol.insert(*a3);

	a0->createBond(*a1);
	a1->createBond(*a2);
	a0->createBond(*a3);

	std::vector<Atom*> ring;
	ring.push_back(a0);
	ring.push_back(a1);
	ring.push_back(a2);
	ring.push_back(a3);

	RingAnalyser analyser;
	// Must RETURN (no hang). Against the unfixed code this never returns.
	analyser.sequenceRing(ring);

	// Sanity: sequenceRing operates in place and preserves the vector size.
	TEST_EQUAL(ring.size(), 4)
	// The first atom always stays in place.
	TEST_EQUAL(ring[0], a0)
RESULT

END_TEST
