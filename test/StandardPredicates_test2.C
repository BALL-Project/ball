// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(standardPredicates)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
HINFile f(BALL_TEST_DATA_PATH(Predicate_test.hin));
System S;
f >> S;
f.close();

// tests for class DoubleBondsPredicate::

CHECK([EXTRA]DoubleBondsPredicate::operator () (const Atom& atom) const )
	DoubleBondsPredicate doubleBonds0;
	doubleBonds0.setArgument("0");
	DoubleBondsPredicate doubleBonds1;
	doubleBonds1.setArgument("1");
	DoubleBondsPredicate doubleBonds2;
	doubleBonds2.setArgument("2");
	DoubleBondsPredicate doubleBonds3;
	doubleBonds3.setArgument("3");
	DoubleBondsPredicate doubleBonds4;
	doubleBonds4.setArgument("4");
	DoubleBondsPredicate doubleBonds5;
	doubleBonds5.setArgument("5");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		STATUS(i)
		switch (i) 
		{
			case 4:
			case 5:
			case 6:
				TEST_EQUAL(doubleBonds0(*it), false)
				break;
			default:
				TEST_EQUAL(doubleBonds0(*it), true)
		}
		switch (i) 
		{
			case 4:
			case 6:
				TEST_EQUAL(doubleBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds1(*it), false)
		}
		switch (i) 
		{
			case 5:
				TEST_EQUAL(doubleBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds2(*it), false)
		}
		TEST_EQUAL(doubleBonds3(*it), false)
		TEST_EQUAL(doubleBonds4(*it), false)
		TEST_EQUAL(doubleBonds5(*it), false)
	}
RESULT

// tests for class SingleBondsPredicate::

CHECK([EXTRA]SingleBondsPredicate::operator () (const Atom& atom) const )
	SingleBondsPredicate singleBonds0;
	singleBonds0.setArgument("0");
	SingleBondsPredicate singleBondslt1;
	singleBondslt1.setArgument("<1");
	SingleBondsPredicate singleBonds1;
	singleBonds1.setArgument("1");
	SingleBondsPredicate singleBonds2;
	singleBonds2.setArgument("2");
	SingleBondsPredicate singleBonds3;
	singleBonds3.setArgument("3");
	SingleBondsPredicate singleBonds4;
	singleBonds4.setArgument("4");
	SingleBondsPredicate singleBonds5;
	singleBonds5.setArgument("5");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i);
		switch (i) 
		{
			case 5:
			case 8:
			case 9:
				TEST_EQUAL(singleBonds0(*it), true)
				break;
			default:
				TEST_EQUAL(singleBonds0(*it), false)
		}
		switch (i) 
		{
			case 5:
			case 8:
			case 9:
				TEST_EQUAL(singleBondslt1(*it), true)
				break;
			default:
				TEST_EQUAL(singleBondslt1(*it), false)
		}
		switch (i) 
		{
			case 3:
			case 4:
			case 5:
			case 6:
			case 8:
			case 9:
			case 10:
			case 11:
				TEST_EQUAL(singleBonds1(*it), false)
				break;
			default:
				TEST_EQUAL(singleBonds1(*it), true)
		}
		switch (i) 
		{
			case 4:
			case 6:
				TEST_EQUAL(singleBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(singleBonds2(*it), false)
		}
		TEST_EQUAL(singleBonds3(*it), false)
		switch (i) 
		{
			case 3:
			case 10:
			case 11:
				TEST_EQUAL(singleBonds4(*it), true)
				break;
			default:
				TEST_EQUAL(singleBonds4(*it), false)
		}
		TEST_EQUAL(singleBonds5(*it), false)
	}
RESULT

// tests for class TripleBondsPredicate::

CHECK([EXTRA]TripleBondsPredicate::operator () (const Atom& atom) const )
	TripleBondsPredicate tripleBonds0;
	tripleBonds0.setArgument("0");
	TripleBondsPredicate tripleBonds1;
	tripleBonds1.setArgument("1");
	TripleBondsPredicate tripleBonds2;
	tripleBonds2.setArgument("2");
	TripleBondsPredicate tripleBonds3;
	tripleBonds3.setArgument("3");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 1:
			case 2:
				TEST_EQUAL(tripleBonds0(*it), false)
				break;
			default:
				TEST_EQUAL(tripleBonds0(*it), true)
		}
		switch (i) 
		{
			case 1:
			case 2:
				TEST_EQUAL(tripleBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(tripleBonds1(*it), false)
		}
		TEST_EQUAL(tripleBonds2(*it), false)
		TEST_EQUAL(tripleBonds3(*it), false)
	}
RESULT

// tests for class SpHybridizedPredicate::

CHECK([EXTRA]SpHybridizedPredicate::operator () (const Atom& atom) const )
	SpHybridizedPredicate isSp;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 1:
			case 2:
			case 5:
				TEST_EQUAL(isSp(*it), true)
				break;
			default:
				TEST_EQUAL(isSp(*it), false)
		}
	}
RESULT


// tests for class sp2HybridizedPredicate::

CHECK([EXTRA]Sp2HybridizedPredicate::operator () (const Atom& atom) const )
	Sp2HybridizedPredicate isSp2;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 4:
			case 6:
			case 7:
				TEST_EQUAL(isSp2(*it), true)
				break;
			default:
				TEST_EQUAL(isSp2(*it), false)
		}
	}
RESULT


// tests for class sp3HybridizedPredicate::
CHECK([EXTRA]Sp3HybridizedPredicate::operator () (const Atom& atom) const )
	Sp3HybridizedPredicate isSp3;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 3:
			case 10:
			case 11:
				TEST_EQUAL(isSp3(*it), true)
				break;
			default:
				TEST_EQUAL(isSp3(*it), false)
		}
	}
RESULT


CHECK([EXTRA]ChargePredicate::operator ())
  // ???
RESULT

S.clear();
f.open(BALL_TEST_DATA_PATH(LacNAc.hin));
f >> S;
f.close();

CHECK([EXTRA]AxialPredicate::operator () (const Atom& atom) )
	AxialPredicate isAxial;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		STATUS(it->getFullName())
		switch (i) 
		{
			case 27:
				TEST_EQUAL(isAxial(*it), true)
				break;
			default:
				TEST_EQUAL(isAxial(*it), false)
		}
	}
RESULT

CHECK([EXTRA]Conformation4C1Predicate::operator () (const Atom& atom) )
	Conformation4C1Predicate is_4C1;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		STATUS(it->getFullName())
		switch (i) 
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 11:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 33:
				TEST_EQUAL(is_4C1(*it), true)
				break;
			default:
				TEST_EQUAL(is_4C1(*it), false)
		}
	}
RESULT

CHECK(~ConnectedToPredicate() throw())
	ConnectedToPredicate* ptr = new ConnectedToPredicate;
	delete ptr;
RESULT

CHECK(~InRingPredicate() throw())
	InRingPredicate* ptr = new InRingPredicate;
	delete ptr;
RESULT

CHECK(ConnectedToPredicate() throw())
	ConnectedToPredicate c;
RESULT

CHECK(InRingPredicate() throw())
	InRingPredicate i;
RESULT

CHECK(InRingPredicate(Size n) throw())
	InRingPredicate i(1);
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
