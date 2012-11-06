// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DOCKING/COMMON/poseClustering.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/DOCKING/COMMON/conformationSet.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/CONCEPT/property.h>
///////////////////////////

using namespace BALL;


START_TEST(PoseClustering)

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PoseClustering* test_pc;
CHECK(PoseClustering())
  test_pc = new PoseClustering();
	TEST_NOT_EQUAL(test_pc,0)
RESULT


CHECK(~PoseClustering())
	delete test_pc;
RESULT

CHECK(setDefaultOptions())
	PoseClustering test_pc;
	test_pc.setDefaultOptions();

	TEST_EQUAL(test_pc.options.getInteger(PoseClustering::Default::RMSD_LEVEL_OF_DETAIL),
													 PoseClustering::Default::RMSD_LEVEL_OF_DETAIL)

	TEST_REAL_EQUAL(test_pc.options.getReal(PoseClustering::Option::RMSD_THRESHOLD),
													 PoseClustering::Default::RMSD_THRESHOLD)
RESULT

CHECK(set/getConformationSet())
		ConformationSet cs;
		PoseClustering pc;
		pc.setConformationSet(&cs);
	  TEST_EQUAL(pc.getConformationSet(), &cs)
RESULT

CHECK(PoseClustering(ConformationSet* poses, float rmsd) ... )
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs(sys);
	PoseClustering pc(&cs, 5.00);

	TEST_REAL_EQUAL(pc.options.getReal(PoseClustering::Option::RMSD_THRESHOLD), 5.00)
  TEST_EQUAL(pc.getConformationSet(), &cs)

	// TODO why does it not work??
	//TEST_EQUAL(pc.getSystem(), sys)

RESULT


CHECK(set/getConformationSet())
		ConformationSet cs;
		cs.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test.dcd));
		cs.resetScoring();
		TEST_EQUAL(cs.size(), 10)

		PoseClustering pc(&cs, 5.00);
		TEST_EQUAL(pc.getConformationSet(), &cs)

		ConformationSet cs2;
		cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
		cs2.resetScoring();
		TEST_EQUAL(cs2.size(), 20)

		pc.setConformationSet(&cs2);
		TEST_EQUAL(pc.getConformationSet(), &cs2)

RESULT


CHECK(compute())
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);

	TEST_EQUAL(pc.getNumberOfClusters(), 0)
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 7)

	TEST_EQUAL(pc.getClusterSize(0), 2)
	std::set<Index> c0 = pc.getCluster(0);
	TEST_EQUAL((c0.find(0)!=c0.end()), true)
	TEST_EQUAL((c0.find(18)!=c0.end()), true)

	TEST_EQUAL(pc.getClusterSize(4), 1)
	std::set<Index> c3 = pc.getCluster(4);
	TEST_EQUAL((c3.find(7)!=c3.end()), true)

	TEST_EQUAL(pc.getClusterSize(2), 1)
	TEST_EQUAL(pc.getClusterSize(3), 8)
	TEST_EQUAL(pc.getClusterSize(5), 5)
	TEST_EQUAL(pc.getClusterSize(6), 1)

	// TODO find out how to catch an error
	//TEST_EQUAL(pc.getClusterSize(7), 0)

RESULT



CHECK(getClusterRepresentative(Index i))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);
	pc.compute();

	boost::shared_ptr<System> sys5 = pc.getClusterRepresentative(5);
	TEST_EQUAL(sys5->getProtein(0)->countAtoms(), 2145)
RESULT

CHECK(getClusterConformationSet(Index i))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);
	pc.compute();

	boost::shared_ptr<ConformationSet> cs3 = pc.getClusterConformationSet(6);
	TEST_EQUAL(cs3->size(), 1)

	boost::shared_ptr<ConformationSet> cs4 = pc.getClusterConformationSet(3);
	TEST_EQUAL(cs4->size(), 8)

	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs3->getSystem().getProtein(0)->countAtoms())
RESULT



CHECK(PoseClustering::Option::RMSD_THRESHOLD)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 100.00);
	TEST_REAL_EQUAL(pc.options.getReal(PoseClustering::Option::RMSD_THRESHOLD), 100.00)
	pc.compute();

	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_EQUAL(pc.getClusterSize(0), 20)
RESULT


CHECK(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();
	PoseClustering pc;

	// Option::RMSD_LEVEL_OF_DETAIL::C_ALPHA
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::RMSDLevelOfDetail::C_ALPHA);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::RMSDLevelOfDetail::C_ALPHA)
	pc.options.setReal(PoseClustering::Option::RMSD_THRESHOLD, 6.00);
	TEST_REAL_EQUAL(pc.options.getReal(PoseClustering::Option::RMSD_THRESHOLD), 6.00)

	pc.setConformationSet(&cs2);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 15)

	//TODO: HEAVY_ATOMS, add implementation and tests 


	// Option::RMSD_LEVEL_OF_DETAIL::BACKBONE;
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::RMSDLevelOfDetail::BACKBONE);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::RMSDLevelOfDetail::BACKBONE)
	pc.options.setReal(PoseClustering::Option::RMSD_THRESHOLD, 6.00);

	pc.setConformationSet(&cs2);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 15)

	// Option::RMSD_LEVEL_OF_DETAIL::ALL_ATOMS
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::RMSDLevelOfDetail::ALL_ATOMS);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::RMSDLevelOfDetail::ALL_ATOMS)
	pc.options.setReal(PoseClustering::Option::RMSD_THRESHOLD, 6.00);

	pc.setConformationSet(&cs2);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 15)
RESULT


CHECK(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL::PROPERTY_BASED_ATOM_BIJECTION)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	Selector sel;
	sys.deselect();
	Expression e("residue(ALA)");
	sel.setExpression(e);
	sys.apply(sel);
	list<Atom*> selected_atoms = sel.getSelectedAtoms();
	list<BALL::Atom*>::iterator it = selected_atoms.begin();
	for ( ; it != selected_atoms.end(); ++it)
	{
		(*it)->setProperty("ATOMBIJECTION_RMSD_SELECTION", true);
	}

	TEST_EQUAL(sel.getNumberOfSelectedAtoms(), 105)

	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();

	PoseClustering pc;
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::RMSDLevelOfDetail::PROPERTY_BASED_ATOM_BIJECTION);
	pc.setConformationSet(&cs2);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 18)
RESULT


CHECK(getReducedConformationSet())
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering2_test.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);
	pc.compute();

	boost::shared_ptr<ConformationSet> cs3 = pc.getReducedConformationSet();
	TEST_EQUAL(cs3->size(),  pc.getNumberOfClusters())

	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs3->getSystem().getProtein(0)->countAtoms())
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

