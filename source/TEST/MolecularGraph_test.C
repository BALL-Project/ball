// $Id: MolecularGraph_test.C,v 1.1.2.1 2002/05/31 22:59:27 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/molecularGraph.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(MolecularGraph, "$Id: MolecularGraph_test.C,v 1.1.2.1 2002/05/31 22:59:27 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

MolecularGraph* ptr = 0;
CHECK(MolecularGraph::MolecularGraph())
	ptr = new MolecularGraph;
	TEST_NOT_EQUAL(ptr, 0)
RESULT											

CHECK(MolecularGraph::~MolecularGraph())
	delete ptr;
RESULT

Molecule M;
CHECK(test molecule)
	System S;
	HINFile f("data/AlaGlySer.hin");
	f >> S;
	f.close();
	TEST_EQUAL(S.countAtoms(), 31)
	ABORT_IF(S.countMolecules() != 1)
	M = *S.beginMolecule();
	TEST_EQUAL(M.countAtoms(), 31)
RESULT

CHECK(MolecularGraph::getNumberOfNodes() const)
	MolecularGraph mg;
	TEST_EQUAL(mg.getNumberOfNodes(), 0)
RESULT
	
CHECK(MolecularGraph::getNumberOfEdges() const)
	MolecularGraph mg;
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
RESULT
	
CHECK(MolecularGraph::MolecularGraph(const Molecule& molecule))
	MolecularGraph mg(M);
	TEST_EQUAL(mg.getNumberOfNodes(), 31)
	TEST_EQUAL(mg.getNumberOfEdges(), 30)
RESULT

CHECK(MolecularGraph::newNode(const Atom& atom))
	MolecularGraph mg;
	Atom a1;
	Atom a2;
	mg.newNode(a1);
	TEST_EQUAL(mg.getNumberOfNodes(), 1)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
	mg.newNode(a1);
	TEST_EQUAL(mg.getNumberOfNodes(), 1)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
	mg.newNode(a2);
	TEST_EQUAL(mg.getNumberOfNodes(), 2)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
