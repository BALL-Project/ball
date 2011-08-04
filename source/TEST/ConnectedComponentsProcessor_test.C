// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

using namespace BALL;

START_TEST(ConnectedComponentsProcessor)

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

ConnectedComponentsProcessor* ccp;
CHECK(ConnectedComponentsProcessor())
  ccp = new ConnectedComponentsProcessor();
	TEST_NOT_EQUAL(ccp,0)
RESULT

CHECK(~ConnectedComponentsProcessor())
	delete ccp;
RESULT

CHECK( operator() )
	ConnectedComponentsProcessor ccp;
	System sys;	
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);
	mol >> sys;
	
	sys.apply(ccp);
RESULT

//New
CHECK( applied on empty System )
	ConnectedComponentsProcessor ccp;
	System sys;	
	sys.apply(ccp);
	
	TEST_EQUAL(ccp.getNumberOfConnectedComponents(), 0)
	
	ccp.splitIntoMolecules(sys);
	
	ConnectedComponentsProcessor::ComponentVector components = ccp.getComponents();
	TEST_EQUAL(components.size(), 0)

RESULT


CHECK( getNumberOfConnectedComponents() )
	ConnectedComponentsProcessor ccp;
	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);	
	mol >> sys;
	
	sys.apply(ccp);
	TEST_EQUAL(ccp.getNumberOfConnectedComponents(), 2)
RESULT


CHECK( splitIntoMolecules() )
	ConnectedComponentsProcessor ccp;
	System sys;	
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);	
	mol >> sys;
	
	sys.apply(ccp);

	ccp.splitIntoMolecules(sys);

	for (MoleculeIterator m_it = sys.beginMolecule(); +m_it; ++m_it)
	{
		TEST_EQUAL(m_it->countAtoms(), 10)
	}
RESULT

CHECK( splitIntoMolecules() twice )
	ConnectedComponentsProcessor ccp;
	System sys;	
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);	
	mol >> sys;

	sys.apply(ccp);

	ccp.splitIntoMolecules(sys);

	sys.apply(ccp);

	ccp.splitIntoMolecules(sys);


	for (MoleculeIterator m_it = sys.beginMolecule(); +m_it; ++m_it)
	{
		TEST_EQUAL(m_it->countAtoms(), 10)
	}
RESULT




CHECK( getComponents() )
	ConnectedComponentsProcessor ccp;
	System sys;	
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);	
	mol >> sys;
	
	sys.apply(ccp);

	ccp.splitIntoMolecules(sys);
	ConnectedComponentsProcessor::ComponentVector components = ccp.getComponents();
	TEST_EQUAL(components.size(), 2)
	TEST_EQUAL(components[0].size(), 10)

RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
