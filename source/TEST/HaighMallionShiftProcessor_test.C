// $Id: HaighMallionShiftProcessor_test.C,v 1.4 2000/09/24 13:27:45 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(HaighMallionShiftProcessor, "$Id: HaighMallionShiftProcessor_test.C,v 1.4 2000/09/24 13:27:45 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

HaighMallionShiftProcessor* sp = 0;
CHECK(HaighMallionShiftProcessor::HaighMallionShiftProcessor() throw())
	sp = new HaighMallionShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(HaighMallionShiftProcessor::~HaighMallionShiftProcessor() throw())
  delete sp;
RESULT

Parameters parameters("data/HaighMallionShiftProcessor_test.ini");
HINFile f("data/HaighMallionShiftProcessor_test.hin");
System S;
f >> S;
f.close();


CHECK(HaighMallionShiftProcessor::HaighMallionShiftProcessor(const HaighMallionShiftProcessor& processor) throw())
  //BAUSTELLE
RESULT
/*

CHECK(HaighMallionShiftProcessor::init() throw())
  HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	TEST_EQUAL(sp.isValid(), false)
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
RESULT


CHECK(HaighMallionShiftProcessor::start() throw())
	HaighMallionShiftProcessor sp;
	TEST_EQUAL(sp.start(), false)
	sp.setParameters(parameters);
	TEST_EQUAL(sp.start(), false)
	sp.init();	
	TEST_EQUAL(sp.start(), true)
RESULT


CHECK(HaighMallionShiftProcessor::finish() throw())
  //BAUSTELLE
RESULT


CHECK(HaighMallionShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(chemical shifts/without rings)
	PRECISION(0.0001)
	HaighMallionShiftProcessor sp;
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
			if (atom_it->hasProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT))
			{
				i++;
			}
		}
		TEST_EQUAL(i, 0)
	}	
RESULT


f.open("data/HaighMallionShiftProcessor_test2.hin");
f >> S;
CHECK(chemical shifts/with rings)
	StringHashMap<float> rc_shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		rc_shifts.insert(name, shift);
	}
	TEST_EQUAL(rc_shifts.size(), 79)

	HaighMallionShiftProcessor sp;
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
			if (atom_it->hasProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT))
			{
				shift = atom_it->getProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat();
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
f.open("data/HaighMallionShiftProcessor_test3.hin");
f >> S;

CHECK(chemical shifts/with rings)
	StringHashMap<float> rc_shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test3.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		rc_shifts.insert(name, shift);
	}
	TEST_EQUAL(rc_shifts.size(), 2)

	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 32)
	
	if (S.countAtoms() == 32)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT))
			{
				shift = atom_it->getProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat();
				if (shift != 0)
				{
					STATUS("shift of " << atom_it->getFullName() << ": " << shift)
					TEST_EQUAL(rc_shifts.has(atom_it->getFullName()), true)
					if (rc_shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, rc_shifts[atom_it->getFullName()])
						i++;
					}
					else
					{
						TEST_REAL_EQUAL(shift, 0.0)
					}
				}
			}
		}
		TEST_EQUAL(i, 2)
	}	
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
