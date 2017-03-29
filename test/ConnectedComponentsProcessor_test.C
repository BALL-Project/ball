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
	
	ConnectedComponentsProcessor::ComponentVector components;
	ccp.getComponents(components);
	
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

	ConnectedComponentsProcessor::MolVec molecules;
	ccp.getAllComponents(molecules);
	
	for (
	ConnectedComponentsProcessor::MolVec::iterator m_it = molecules.begin(); 
	m_it != molecules.end(); ++m_it)
	{
		TEST_EQUAL( (*m_it).countAtoms(), 10)
	}
RESULT

CHECK( splitIntoMolecules() twice )
	ConnectedComponentsProcessor ccp;
	System sys;	
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);	
	mol >> sys;

	sys.apply(ccp);
	
	ConnectedComponentsProcessor::MolVec molecules0;
	ccp.getAllComponents(molecules0);
	System sys2;
	
	for (
	ConnectedComponentsProcessor::MolVec::iterator m_it = molecules0.begin(); 
	m_it != molecules0.end(); ++m_it)
	{
		sys2.insert(*m_it);
	}
	
	sys2.apply(ccp);

	ConnectedComponentsProcessor::MolVec molecules1;
	ccp.getAllComponents(molecules1);
	for (
	ConnectedComponentsProcessor::MolVec::iterator m_it = molecules1.begin(); 
	m_it != molecules1.end(); ++m_it)
	{
		TEST_EQUAL( (*m_it).countAtoms(), 10)
	}
RESULT




CHECK( getComponents() )
	ConnectedComponentsProcessor ccp;
	System sys;	
	MOL2File mol(BALL_TEST_DATA_PATH(ConnectedComponentsProcessor_test.mol2), std::ios::in);	
	mol >> sys;
	
	sys.apply(ccp);

	ConnectedComponentsProcessor::ComponentVector components;
	ccp.getComponents(components);
	
	TEST_EQUAL(components.size(), 2)
	TEST_EQUAL(components[0].size(), 10)

RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
