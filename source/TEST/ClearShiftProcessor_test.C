// $Id: ClearShiftProcessor_test.C,v 1.3 2000/09/22 11:46:06 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/clearShiftProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>

///////////////////////////

START_TEST(ClearShiftProcessor, "$Id: ClearShiftProcessor_test.C,v 1.3 2000/09/22 11:46:06 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ClearShiftProcessor* sp = 0;
CHECK(ClearShiftProcessor::ClearShiftProcessor() throw())
	sp = new ClearShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(ClearShiftProcessor::~ClearShiftProcessor() throw())
  delete sp;
RESULT


System S;
Molecule molecule;
S.insert(molecule);
Atom atom1;
Atom atom2;
Atom atom3;
molecule.insert(atom1);
molecule.insert(atom2);
molecule.insert(atom3);

CHECK(shift assignment)
	AtomIterator atom_it = S.beginAtom();
	Position i = 0;
	for (; +atom_it; ++atom_it)
	{
		atom_it->setProperty(ShiftModule::PROPERTY__SHIFT, float(i));
		TEST_EQUAL(atom_it->hasProperty(ShiftModule::PROPERTY__SHIFT), true)
		i++;
	}
RESULT

CHECK(ClearShiftProcessor::Processor::Result operator () (Composite& composite) throw())
	// BAUSTELLE
RESULT

CHECK(apply)
	ClearShiftProcessor sp;
	TEST_EQUAL(S.countAtoms(), 3)
	S.apply(sp);
	AtomIterator atom_it = S.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		TEST_EQUAL(atom_it->hasProperty(ShiftModule::PROPERTY__SHIFT), false)
		TEST_EQUAL(atom_it->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat(), 0)
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
