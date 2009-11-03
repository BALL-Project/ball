// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MolecularGraph_test.C,v 1.3.34.2 2007/03/25 21:47:21 oliver Exp $
#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/molecularGraph.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(MolecularGraph, "$Id: MolecularGraph_test.C,v 1.3.34.2 2007/03/25 21:47:21 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

MolecularGraph* ptr = 0;
CHECK(MolecularGraph::MolecularGraph())
	ptr = new MolecularGraph();
	TEST_NOT_EQUAL(ptr, 0)
RESULT											

CHECK(MolecularGraph::~MolecularGraph())
	delete ptr;
RESULT

Molecule M;
CHECK(test molecule)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(AlaGlySer.hin));
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

CHECK(MolecularGraph::newEdge(const Bond& atom))
	MolecularGraph mg;
	Atom a1;
	Atom a2;
	Atom a3;
	Bond& bond1 = *a1.createBond(a2);
	Bond& bond2 = *a1.createBond(a3);
	
	mg.newNode(a1);
	mg.newNode(a2);
	mg.newNode(a3);

	mg.newEdge(bond1);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 1)

	mg.newEdge(bond1);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 1)

	mg.newEdge(bond2);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 2)
RESULT

CHECK(MolecularGraph::deleteNode(const Atom& atom))
	MolecularGraph mg;
	Atom a1;
	Atom a2;
	TEST_EQUAL(mg.getNumberOfNodes(), 0)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
	mg.newNode(a1);
	TEST_EQUAL(mg.getNumberOfNodes(), 1)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
	mg.newNode(a2);
	TEST_EQUAL(mg.getNumberOfNodes(), 2)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)

	mg.deleteNode(a1);
	TEST_EQUAL(mg.getNumberOfNodes(), 1)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)

	mg.deleteNode(a1);
	TEST_EQUAL(mg.getNumberOfNodes(), 1)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)

	mg.deleteNode(a2);
	TEST_EQUAL(mg.getNumberOfNodes(), 0)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)

	mg.deleteNode(a2);
	TEST_EQUAL(mg.getNumberOfNodes(), 0)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
RESULT

CHECK(MolecularGraph::deleteEdge(const Bond& atom))
	MolecularGraph mg;
	Atom a1;
	Atom a2;
	Atom a3;
	Bond& bond1 = *a1.createBond(a2);
	Bond& bond2 = *a1.createBond(a3);
	
	mg.newNode(a1);
	mg.newNode(a2);
	mg.newNode(a3);

	mg.newEdge(bond1);
	mg.newEdge(bond2);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 2)

	mg.deleteEdge(bond2);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 1)

	mg.deleteEdge(bond2);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 1)

	mg.deleteEdge(bond1);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)

	mg.deleteEdge(bond1);
	TEST_EQUAL(mg.getNumberOfNodes(), 3)
	TEST_EQUAL(mg.getNumberOfEdges(), 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
