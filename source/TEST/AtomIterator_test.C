// $Id: AtomIterator_test.C,v 1.2 2001/12/22 14:14:04 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/atomContainer.h>
#include <algorithm>

///////////////////////////

START_TEST(AtomIterator, "$Id: AtomIterator_test.C,v 1.2 2001/12/22 14:14:04 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(typedefs)
	AtomContainer ac;
	ac.append(*new Atom);
	ac.beginAtom()->setName("A1");
	AtomIterator ai = ac.beginAtom();
	AtomIterator::value_type value = *ai;
	TEST_EQUAL(value.getName(), ai->getName())
	// BAUSTELLE
	// AtomIterator::difference_type diff = ac.endAtom() - ac.beginAtom();
	// TEST_EQUAL((Size)diff, 2)
	AtomIterator::reference ref = *ai;
	TEST_EQUAL(ref.getName(), ai->getName())
	AtomIterator::pointer ptr = &*ai;
	TEST_EQUAL(ptr, &*ai)
RESULT

CHECK(use in copy algorithm)
	AtomContainer ac;
	Atom a1;
	a1.setName("A1");
	Atom a2;
	a2.setName("A2");
	Atom a3;
	a3.setName("A3");
	
	ac.append(a1);
	ac.append(a2);
	ac.append(a3);
		
	std::vector<Atom> atom_vec(10);
	std::copy(ac.beginAtom(), ac.endAtom(), atom_vec.begin());
	
	TEST_EQUAL(atom_vec[0].getName(), "A1")
	TEST_EQUAL(atom_vec[1].getName(), "A2")
	TEST_EQUAL(atom_vec[2].getName(), "A3")
	TEST_EQUAL(atom_vec[3].getName(), "")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
