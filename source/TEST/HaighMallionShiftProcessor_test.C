// $Id: HaighMallionShiftProcessor_test.C,v 1.6 2001/12/20 01:14:19 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(HaighMallionShiftProcessor, "$Id: HaighMallionShiftProcessor_test.C,v 1.6 2001/12/20 01:14:19 oliver Exp $")

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

PRECISION(0.002)

CHECK(chemical shifts/without rings)
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
				if (atom_it->getProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat() != 0.0)
				{
					i++;
				}
			}
		}
		TEST_EQUAL(i, 0)
	}	
RESULT

S.destroy();
f.open("data/HaighMallionShiftProcessor_test2.hin");
f >> S;
CHECK(chemical shifts/with rings)
	StringHashMap<float> shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		if (name != "")
		{
			shifts.insert(name, shift);
		}
	}
	TEST_EQUAL(shifts.size(), 97)

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
					TEST_EQUAL(shifts.has(atom_it->getFullName()), true)
					if (shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, shifts[atom_it->getFullName()])
						i++;
						shifts.erase(atom_it->getFullName());
					}
				}
			}
		}
		TEST_EQUAL(i, 97)
		TEST_EQUAL(shifts.size(), 0)
		if (shifts.size() != 0)
		{
			StringHashMap<float>::Iterator it = shifts.begin();
			for (; +it; ++it)
			{
				STATUS("unassigned shift for: " << it->first)
			}
		}
		
	}	
RESULT

PDBFile PDB_file;
FragmentDB frag_db;

PRECISION(0.00015)
CHECK(chemical shifts -- PHE)

	// check the shift caused by PHE rings
	S.destroy();
	PDBFile pdb_file("data/HaighMallionShiftProcessor_test_PHE.pdb");
	pdb_file >> S;
	pdb_file.close();
	S.apply(frag_db.build_bonds);

	StringHashMap<float> shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test_PHE.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		if (name != "")
		{
			shifts.insert(name, shift);
		}
	}
	TEST_EQUAL(shifts.size(), 2)

	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 20)
	
	if (S.countAtoms() == 20)
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
					TEST_EQUAL(shifts.has(atom_it->getFullName()), true)
					if (shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, shifts[atom_it->getFullName()])
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


CHECK(chemical shifts -- TRP)

	// check the shift caused by TRP rings
	S.destroy();
	PDBFile pdb_file("data/HaighMallionShiftProcessor_test_TRP.pdb");
	pdb_file >> S;
	pdb_file.close();
	S.apply(frag_db.build_bonds);

	StringHashMap<float> shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test_TRP.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		if (name != "")
		{
			shifts.insert(name, shift);
		}
	}
	TEST_EQUAL(shifts.size(), 2)

	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 24)
	
	if (S.countAtoms() == 24)
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
					TEST_EQUAL(shifts.has(atom_it->getFullName()), true)
					if (shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, shifts[atom_it->getFullName()])
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


CHECK(chemical shifts -- TYR)

	// check the shift caused by TYR rings
	S.destroy();
	PDBFile pdb_file("data/HaighMallionShiftProcessor_test_TYR.pdb");
	pdb_file >> S;
	pdb_file.close();
	S.apply(frag_db.build_bonds);

	StringHashMap<float> shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test_TYR.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		if (name != "")
		{
			shifts.insert(name, shift);
		}
	}
	TEST_EQUAL(shifts.size(), 2)

	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 21)
	
	if (S.countAtoms() == 21)
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
					TEST_EQUAL(shifts.has(atom_it->getFullName()), true)
					if (shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, shifts[atom_it->getFullName()])
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


CHECK(chemical shifts -- HIS)

	// check the shift caused by HIS rings
	S.destroy();
	PDBFile pdb_file("data/HaighMallionShiftProcessor_test_HIS.pdb");
	pdb_file >> S;
	pdb_file.close();
	S.apply(frag_db.build_bonds);

	StringHashMap<float> shifts;
	ifstream infile("data/HaighMallionShiftProcessor_test_HIS.dat");
	String name;
	float shift;
	while (infile.good())
	{
		infile >> name >> shift;
		if (name != "")
		{
			shifts.insert(name, shift);
		}
	}
	TEST_EQUAL(shifts.size(), 2)

	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(sp.isValid(), true)
	TEST_EQUAL(S.countAtoms(), 18)
	
	if (S.countAtoms() == 18)
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
					TEST_EQUAL(shifts.has(atom_it->getFullName()), true)
					if (shifts.has(atom_it->getFullName()))
					{
						TEST_REAL_EQUAL(shift, shifts[atom_it->getFullName()])
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
