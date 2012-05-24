// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/disulfidBondProcessor.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
///////////////////////////

using namespace BALL;


Residue* getResidueByIndex(Size index, System* sys)
{
	Size counter = 1;
	ResidueIterator r_it = sys->beginResidue();
	for ( ; +r_it && counter < index; ++r_it)
	{
		counter++;
	}
	return &*r_it;
}

Atom* getSulfurByResidueIndex(Size index, System* sys)
{
	Size counter = 1;
	ResidueIterator r_it = sys->beginResidue();
	for ( ; +r_it && counter < index; ++r_it)
	{
		counter++;
	}
	AtomIterator a_it = r_it->beginAtom();
	for ( ; +a_it; ++a_it)
	{
		if (a_it->getElement() == PTE[Element::S])
			return &*a_it;
	}
	return NULL;
}



START_TEST(DisulfidBondProcessor)

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

DisulfidBondProcessor* dbp;
CHECK(DisulfidBondProcessor())
  dbp = new DisulfidBondProcessor();
	TEST_NOT_EQUAL(dbp,0)
RESULT

CHECK(~DisulfidBondProcessor())
	delete dbp;
RESULT

CHECK( operator() )
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_AACCQQQQCCRR.pdb), std::ios::in);
	mol >> sys;
	sys.apply(dbp);
RESULT


CHECK( applied on empty System )
	DisulfidBondProcessor dbp;
	System sys;
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)
	sys.apply(dbp);

	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)

	DisulfidBondProcessor::DisulfidBonds bridges = dbp.getDisulfidBonds();
	TEST_EQUAL(bridges.size(), 0)

RESULT


CHECK( getNumberOfDetectedDisulfidBonds() )
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_ACCQQCEECQEICH.pdb), std::ios::in);
	mol >> sys;
	sys.apply(dbp);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 2)
RESULT


CHECK( connect(Residue*) )
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_AACCQQQQCCRR.pdb), std::ios::in);
	mol >> sys;
	sys.apply(dbp);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)

	Residue* res3  = getResidueByIndex(3, &sys);
	Residue* res10 = getResidueByIndex(10, &sys);

	TEST_EQUAL(dbp.connect(res3, res10), true)

	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 2)

	TEST_EQUAL(dbp.connect(res3, res10), false)

RESULT

CHECK( connect(Atom*) )
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_AACCQQQQCCRR.pdb), std::ios::in);
	mol >> sys;;
	sys.apply(dbp);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)
	Atom *s_res3 = getSulfurByResidueIndex(  3, &sys);
	Atom *s_res10= getSulfurByResidueIndex( 10, &sys);
	dbp.connect(s_res3, s_res10);

	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 2)

RESULT

CHECK( disconnect())
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_AACCQQQQCCRR.pdb), std::ios::in);
	mol >> sys;

	sys.apply(dbp);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)

	Residue* res4  = getResidueByIndex(4, &sys);
	Residue* res9 = getResidueByIndex( 9, &sys);

	TEST_EQUAL(dbp.disconnect(res4, res9), true);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)

RESULT

CHECK( disconnect() / connect())
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_AACCQQQQCCRR.pdb), std::ios::in);
	mol >> sys;

	sys.apply(dbp);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)

	Residue* res4  = getResidueByIndex(4, &sys);
	Residue* res9 = getResidueByIndex( 9, &sys);

	dbp.disconnect(res4, res9);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)
	dbp.connect(res4, res9);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)
	dbp.disconnect(res4, res9);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)

	Atom *s_res4 = getSulfurByResidueIndex( 4, &sys);
	Atom *s_res9 = getSulfurByResidueIndex( 9, &sys);

	TEST_EQUAL(dbp.connect(s_res4, s_res9), true);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)

	TEST_EQUAL(dbp.disconnect(s_res4, s_res9), true);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)

	TEST_EQUAL(dbp.connect(s_res4, s_res9), true);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 1)

	TEST_EQUAL(dbp.disconnect(s_res4, s_res9), true);
	TEST_EQUAL(dbp.getNumberOfDetectedDisulfidBonds(), 0)

RESULT

CHECK( getDisulfidBonds())
	DisulfidBondProcessor dbp;
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(DisulfidBondProcessor_test_AACCQQQQCCRR.pdb), std::ios::in);
	mol >> sys;

	sys.apply(dbp);

	DisulfidBondProcessor::DisulfidBonds bridges = dbp.getDisulfidBonds();

	TEST_EQUAL(bridges.size(), 1)

	Residue* res3  = getResidueByIndex(3, &sys);
	Residue* res10 = getResidueByIndex(10, &sys);

	dbp.connect(res3, res10);
	bridges = dbp.getDisulfidBonds();
	TEST_EQUAL(bridges.size(), 2)

	dbp.disconnect(res3, res10);
	bridges = dbp.getDisulfidBonds();
	TEST_EQUAL(bridges.size(), 1)

RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
