// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(DefaultProcessors)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

// tests for class ClearChargeProcessor::
System s;
Molecule m1, m2;
s.insert(m1);
s.insert(m2);
Atom a1, a2; 
m1.insert(a1);
a1.setCharge(1.1);
a1.setName("a1");
m2.insert(a2);
a2.setCharge(2.2);
a2.setName("a2");

CHECK(ClearChargeProcessor::Processor::Result operator()(Atom& atom))
	ClearChargeProcessor ccp;
	s.apply(ccp);
	TEST_REAL_EQUAL(a1.getCharge(), 0.0)
	TEST_REAL_EQUAL(a2.getCharge(), 0.0)
RESULT
	
// tests for class AssignRadiusProcessor::

CHECK(AssignRadiusProcessor::AssignRadiusProcessor())
	AssignRadiusProcessor arp;
	TEST_EQUAL(arp.getNumberOfAssignments(), 0)
	TEST_EQUAL(arp.getNumberOfErrors(), 0)
	TEST_EQUAL(arp.getFilename(), "")
RESULT


CHECK(AssignRadiusProcessor::AssignRadiusProcessor(const String& filename))
	AssignRadiusProcessor* arp = 0;
	arp = new AssignRadiusProcessor(BALL_TEST_DATA_PATH(DefaultProcessor_test.1));
	ABORT_IF(arp == 0)
	TEST_EQUAL(arp->getFilename(), BALL_TEST_DATA_PATH(DefaultProcessor_test.1))
	delete arp;
	arp = 0;
	TEST_EXCEPTION(Exception::FileNotFound, arp = new AssignRadiusProcessor("XXX"))
	delete arp;
RESULT

AssignRadiusProcessor arp;
CHECK(AssignRadiusProcessor::start())
	arp.setFilename(BALL_TEST_DATA_PATH(DefaultProcessor_test.1));
  TEST_EQUAL(arp.start(), true) 
RESULT

CHECK(AssignRadiusProcessor::finish())
  TEST_EQUAL(arp.finish(), true) 
RESULT


CHECK(AssignRadiusProcessor::Processor::Result operator()(Atom& atom))
	HINFile f(BALL_TEST_DATA_PATH(AnisotropyShiftProcessor_test.hin));
  System S;
	f >> S;
	S.insert(m1);
	S.insert(m2);
	StringHashMap<float> shm;
	ifstream infile(BALL_TEST_DATA_PATH(DefaultProcessor_test.1));
	String name;  
	float radius;
	while (infile.good())
  { 
    infile >> name >> radius;
		if (name != "")
		{ 
			shm.insert(name, radius);
		}
	}

	TEST_EQUAL(S.countAtoms(), 33)

	CAPTURE_OUTPUT_LEVEL(2000)
		S.apply(arp);
	COMPARE_OUTPUT("Cannot assign radius for :a1\nCannot assign radius for :a2\n")
	
	AtomIterator atom_it = S.beginAtom();
	Position i = 1;
	for (; +atom_it; ++atom_it)
	{
		if (shm.has(atom_it->getFullName()))
		{
			TEST_REAL_EQUAL(shm[atom_it->getFullName()], atom_it->getRadius())
			i++;
		}
	}			
	TEST_EQUAL(i - 1, 31)         
	TEST_EQUAL(arp.getNumberOfErrors(), 2)
	TEST_EQUAL(arp.getNumberOfAssignments(), 31)
RESULT

a1.setName("a1");
a2.setName("a2");

CHECK(AssignRadiusProcessor::setFilename(const String& filename))
	TEST_EXCEPTION(Exception::FileNotFound, arp.setFilename("XXX"));
	arp.setFilename(BALL_TEST_DATA_PATH(../DefaultProcessors_test.C));
RESULT


CHECK(AssignRadiusProcessor::getFilename())
	TEST_EQUAL(arp.getFilename(), BALL_TEST_DATA_PATH(../DefaultProcessors_test.C));
RESULT

AssignRadiusProcessor arp2;
CHECK(AssignRadiusProcessor::getNumberOfAssignments())
  TEST_EQUAL(arp.getNumberOfAssignments(), 31)
	TEST_EQUAL(arp2.getNumberOfAssignments(), 0)
RESULT


CHECK(AssignRadiusProcessor::getNumberOfErrors())
  TEST_EQUAL(arp.getNumberOfErrors(), 2)
	TEST_EQUAL(arp2.getNumberOfErrors(), 0)
RESULT


// tests for class AssignChargeProcessor::
CHECK(AssignChargeProcessor::AssignChargeProcessor())
  AssignChargeProcessor acp;
	TEST_REAL_EQUAL(acp.getTotalCharge(), 0)
RESULT


AssignChargeProcessor acp;
CHECK(AssignChargeProcessor::AssignChargeProcessor(const String& filename))
	TEST_EXCEPTION(Exception::FileNotFound, AssignChargeProcessor acp("XXX"))
	acp = AssignChargeProcessor(BALL_TEST_DATA_PATH(DefaultProcessor_test.1));
	TEST_EQUAL(acp.getFilename(), BALL_TEST_DATA_PATH(DefaultProcessor_test.1))
RESULT


CHECK(AssignChargeProcessor::start())
  TEST_EQUAL(acp.start(), true) 
RESULT

CHECK(AssignChargeProcessor::finish())
  TEST_EQUAL(acp.finish(), true) 
RESULT


CHECK(AssignChargeProcessor::Processor::Result operator()(Atom& atom))
	HINFile f(BALL_TEST_DATA_PATH(AnisotropyShiftProcessor_test.hin));
  System S;
	f >> S;
	S.insert(m1);
	m1.insert(a1);
	S.insert(m2);
	m2.insert(a2);
	TEST_EQUAL(S.countAtoms(), 33)
	StringHashMap<float> shm;
	ifstream infile(BALL_TEST_DATA_PATH(DefaultProcessor_test.1));
	String name;  
	float radius;
	while (infile.good())
  { 
    infile >> name >> radius;
		if (name != "")
		{ 
			shm.insert(name, radius);
		}
	}

	TEST_EQUAL(S.countAtoms(), 33)
	
	CAPTURE_OUTPUT_LEVEL(2000)
		S.apply(acp);
	COMPARE_OUTPUT("Cannot assign charge for :a1\nCannot assign charge for :a2\n")
	
	AtomIterator atom_it = S.beginAtom();
	Position i = 1;
	for (; +atom_it; ++atom_it)
	{
		if (shm.has(atom_it->getFullName()))
		{
			TEST_REAL_EQUAL(shm[atom_it->getFullName()], atom_it->getCharge())
			i++;
		}
	}			
	TEST_EQUAL(i - 1, 31)         
	TEST_EQUAL(acp.getNumberOfErrors(), 2)
	TEST_EQUAL(acp.getNumberOfAssignments(), 31)
RESULT


CHECK(AssignChargeProcessor::setFilename(const String& filename))
	TEST_EXCEPTION(Exception::FileNotFound, acp.setFilename("XXX"));
	acp.setFilename(BALL_TEST_DATA_PATH(../DefaultProcessors_test.C));
RESULT


CHECK(AssignRadiusProcessor::getFilename())
	TEST_EQUAL(acp.getFilename(), BALL_TEST_DATA_PATH(../DefaultProcessors_test.C));
RESULT

AssignRadiusProcessor acp2;
CHECK(AssignRadiusProcessor::getNumberOfAssignments())
  TEST_EQUAL(acp.getNumberOfAssignments(), 31)
	TEST_EQUAL(acp2.getNumberOfAssignments(), 0)
RESULT


CHECK(AssignRadiusProcessor::getNumberOfErrors())
  TEST_EQUAL(acp.getNumberOfErrors(), 2)
	TEST_EQUAL(acp2.getNumberOfErrors(), 0)
RESULT


CHECK(AssignChargeProcessor::getTotalCharge())
	float total(0);
	for (Size s = 1; s < 32; s++)
	{
		total += s;
	}
	TEST_REAL_EQUAL( acp.getTotalCharge(), total)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
