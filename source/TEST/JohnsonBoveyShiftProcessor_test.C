// $Id: JohnsonBoveyShiftProcessor_test.C,v 1.4 2000/09/24 13:27:45 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/johnsonBoveyShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(JohnsonBoveyShiftProcessor, "$Id: JohnsonBoveyShiftProcessor_test.C,v 1.4 2000/09/24 13:27:45 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

JohnsonBoveyShiftProcessor* sp = 0;
CHECK(JohnsonBoveyShiftProcessor::JohnsonBoveyShiftProcessor() throw())
	sp = new JohnsonBoveyShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(JohnsonBoveyShiftProcessor::~JohnsonBoveyShiftProcessor() throw())
  delete sp;
RESULT

Parameters parameters("data/JohnsonBoveyShiftProcessor_test.ini");
HINFile f("data/JohnsonBoveyShiftProcessor_test.hin");
System S;
f >> S;
f.close();

/*
CHECK(JohnsonBoveyShiftProcessor::JohnsonBoveyShiftProcessor(const JohnsonBoveyShiftProcessor& processor) throw())
  //BAUSTELLE
RESULT


CHECK(JohnsonBoveyShiftProcessor::init() throw())
  JohnsonBoveyShiftProcessor sp;
	sp.setParameters(parameters);
	TEST_EQUAL(sp.isValid(), false)
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
RESULT


CHECK(JohnsonBoveyShiftProcessor::start() throw())
	JohnsonBoveyShiftProcessor sp;
	TEST_EQUAL(sp.start(), false)
	sp.setParameters(parameters);
	TEST_EQUAL(sp.start(), false)
	sp.init();	
	TEST_EQUAL(sp.start(), true)
RESULT


CHECK(JohnsonBoveyShiftProcessor::finish() throw())
  //BAUSTELLE
RESULT


CHECK(JohnsonBoveyShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(chemical shifts/without rings)
	PRECISION(0.0001)
	JohnsonBoveyShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 31)
	
	if (S.countAtoms() == 31)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty(JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT))
			{
				i++;
			}
		}
		TEST_EQUAL(i, 0)
	}	
RESULT


f.open("data/JohnsonBoveyShiftProcessor_test2.hin");
f >> S;
CHECK(chemical shifts/with rings)
	StringHashMap<float> rc_shifts;
	ifstream infile("data/JohnsonBoveyShiftProcessor_test.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		rc_shifts.insert(name, shift);
	}
	TEST_EQUAL(rc_shifts.size(), 79)

	JohnsonBoveyShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 328)
	
	if (S.countAtoms() == 328)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty(JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT))
			{
				shift = atom_it->getProperty(JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat();
				if (shift != 0)
				{
					STATUS("shift of " << atom_it->getFullName() << ": " << shift)
					TEST_EQUAL(rc_shifts.has(atom_it->getFullName()), true)
					if (rc_shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, rc_shifts[atom_it->getFullName()])
						i++;
					}
				}
			}
		}
		TEST_EQUAL(i, 79)
	}	
RESULT
*/

f.open("data/JohnsonBoveyShiftProcessor_test3.hin");
f >> S;
PDBFile outf("test.pdb", File::OUT);
outf << S;
outf.close();
CHECK(chemical shifts/with rings)
	StringHashMap<float> rc_shifts;
	ifstream infile("data/JohnsonBoveyShiftProcessor_test3.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		rc_shifts.insert(name, shift);
	}
	TEST_EQUAL(rc_shifts.size(), 79)

	JohnsonBoveyShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 328)
	
	if (S.countAtoms() == 328)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty(JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT))
			{
				shift = atom_it->getProperty(JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat();
				if (shift != 0)
				{
					STATUS("shift of " << atom_it->getFullName() << ": " << shift)
					TEST_EQUAL(rc_shifts.has(atom_it->getFullName()), true)
					if (rc_shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, rc_shifts[atom_it->getFullName()])
						i++;
					}
				}
			}
		}
		TEST_EQUAL(i, 79)
	}	
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
