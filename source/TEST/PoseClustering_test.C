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
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/KERNEL/PTE.h>
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

	TEST_REAL_EQUAL(test_pc.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD),
													 PoseClustering::Default::DISTANCE_THRESHOLD)

	TEST_EQUAL(test_pc.options.getInteger(PoseClustering::Option::CLUSTER_METHOD),
		                   PoseClustering::Default::CLUSTER_METHOD)

	TEST_EQUAL(test_pc.options.getInteger(PoseClustering::Option::RMSD_TYPE),
		                   PoseClustering::Default::RMSD_TYPE)

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

	TEST_REAL_EQUAL(pc.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), 5.00)
  TEST_EQUAL(pc.getConformationSet(), &cs)

	for (AtomConstIterator pc_at_it = pc.getSystem().beginAtom(), sys_at_it = sys.beginAtom();
			 +pc_at_it && +sys_at_it; ++pc_at_it, ++sys_at_it)
	{
		TEST_EQUAL(pc_at_it->getPosition() == sys_at_it->getPosition(), true)
	}

RESULT


CHECK(set/getConformationSet())
		ConformationSet cs;
		cs.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test.dcd));
		cs.resetScoring();
		TEST_EQUAL(cs.size(), 10)

		PoseClustering pc(&cs, 5.00);
		TEST_EQUAL(pc.getConformationSet(), &cs)

		ConformationSet cs2;
		cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
		cs2.resetScoring();
		TEST_EQUAL(cs2.size(), 8)

		pc.setConformationSet(&cs2);
		TEST_EQUAL(pc.getConformationSet(), &cs2)

RESULT


CHECK(compute() - general)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);

	TEST_EQUAL(pc.getNumberOfClusters(), 0)
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
	TEST_EQUAL(pc.getClusterSize(0), 4)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 7.4983)
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(0, pc.options, false), 7.4983)
	std::set<Index> c0 = pc.getCluster(0);
	TEST_EQUAL((c0.find(0)!=c0.end()), true)
	TEST_EQUAL((c0.find(3)!=c0.end()), true)
	TEST_EQUAL((c0.find(4)==c0.end()), true)

	TEST_EQUAL(pc.getClusterSize(2), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 2.65794)
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(2, pc.options, false), 2.65794)
	std::set<Index> c2 = pc.getCluster(2);
	TEST_EQUAL((c2.find(6)!=c2.end()), true)
	TEST_EQUAL((c2.find(7)!=c2.end()), true)

	// TODO find out how to catch an error
	//TEST_EQUAL(pc.getClusterSize(7), 0)

RESULT


CHECK(PoseClustering::Option::CLUSTER_METHOD with RMSD_TYPE = SNAPSHOT_RMSD)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	PoseClustering pc;
	pc.setConformationSet(&cs2);

	// SNAPSHOT_RMSD 
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);

	//               - TRIVIAL_COMPLETE_LINKAGE
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 100.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 60.6787)

	//pc.printClusterScores();

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 4.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 5)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 1.84453)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 0.)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 0.)
	TEST_REAL_EQUAL(pc.getClusterScore(3), 2.63344)
	TEST_REAL_EQUAL(pc.getClusterScore(4), 2.65794)

	//               - CLINK_DEFAYS
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 10.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 5)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 7.4983)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 0.)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 0.)
	TEST_REAL_EQUAL(pc.getClusterScore(3), 0.)
	TEST_REAL_EQUAL(pc.getClusterScore(4), 0.)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 6.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 7)


	//               - SLINK_SIBSON
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 6.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 3)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 50.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 2.63344)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 2.65794)

	//               - NEAREST_NEIGHBOR_CHAIN_WARD	
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 8)

	// TEST
	/*
	File f("test2.gv", std::ios::out);
	pc.exportWardClusterTreeToGraphViz(f);
	f.close();
	// END TEST	
	*/

	std::vector<std::set<Index> > clusters = pc.extractClustersForThreshold(0.5);
	TEST_EQUAL(clusters.size(), 8)
	TEST_EQUAL(pc.getNumberOfClusters(), 8)
	TEST_EQUAL(pc.getClusterScore(0), 0.)

	clusters = pc.extractClustersForThreshold(5.5);
	TEST_EQUAL(clusters.size(), 3)
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
	TEST_EQUAL(pc.getClusterSize(0), 2)
	PRECISION(1e-4)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 1.86212)
	TEST_EQUAL(pc.getClusterSize(1), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 1.87944)
	TEST_EQUAL(pc.getClusterSize(2), 4)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 5.26936)

	clusters = pc.extractClustersForThreshold(68);
	TEST_EQUAL(clusters.size(), 1)
	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_EQUAL(pc.getClusterSize(0), 8)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 67.8968)
	PRECISION(1e-5)
RESULT


CHECK(PoseClustering::Option::CLUSTER_METHOD with RMSD_TYPE = CENTER_OF_MASS_DISTANCE)
	PoseClustering pc;
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);

	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	pc.setConformationSet(&cs2);

	//               - TRIVIAL_COMPLETE_LINKAGE
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 40.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 38.8396)
	TEST_EQUAL(pc.getClusterSize(1), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 1.54954e-05)
	PRECISION(1e-5)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 3.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 4)
	TEST_EQUAL(pc.getClusterScore(0) < 3., true)
	TEST_EQUAL(pc.getClusterScore(1) < 3., true)
	TEST_EQUAL(pc.getClusterScore(2) < 3., true)
	TEST_EQUAL(pc.getClusterScore(3) < 3., true)
	//pc.printClusterScores();

	//               - CLINK_DEFAYS
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 40.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)
	TEST_EQUAL((pc.getClusterSize(0) + pc.getClusterSize(1)), pc.getNumberOfPoses())
	PRECISION(1e-3)
	TEST_EQUAL(pc.getClusterSize(0), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 1.54954e-05)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 38.8396)
	PRECISION(1e-5)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 4)
	TEST_EQUAL(pc.getClusterScore(0) < 5., true)
	TEST_EQUAL(pc.getClusterScore(1) < 5., true)
	TEST_EQUAL(pc.getClusterScore(2) < 5., true)
	TEST_EQUAL(pc.getClusterScore(3) < 5., true)

	//               - SLINK_SIBSON
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 3.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
	TEST_EQUAL((pc.getClusterSize(0) + pc.getClusterSize(1) + pc.getClusterSize(2)), pc.getNumberOfPoses())
	TEST_REAL_EQUAL(pc.getClusterScore(0), 2.31429)
	TEST_EQUAL(pc.getClusterSize(1), 2)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(1),  1.54954e-05)
	PRECISION(1e-5)

	TEST_REAL_EQUAL(pc.getClusterScore(2), 1.33687)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 36.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)

	//               - NEAREST_NEIGHBOR_CHAIN_WARD
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 8)

	pc.extractClustersForThreshold(2.4);
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
	TEST_EQUAL(pc.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0),  2.34918)
	PRECISION(1e-5)

	TEST_REAL_EQUAL(pc.getClusterScore(1), 1.09569e-05)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 0.945311)

	pc.extractClustersForThreshold(50.7);
	TEST_EQUAL(pc.getNumberOfClusters(), 2)
	TEST_EQUAL(pc.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 50.6247)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 2.34918)

	pc.extractClustersForThreshold(59);
	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 58.395)
	PRECISION(1e-5)
RESULT


CHECK(PoseClustering::Option::CLUSTER_METHOD with RMSD_TYPE = RIGID_RMSD)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	PoseClustering pc;
	pc.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	//               - TRIVIAL_COMPLETE_LINKAGE
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 15.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 20)
	TEST_EQUAL(pc.getClusterSize(0), 2)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 2.95941)
	TEST_EQUAL(pc.getClusterSize(17), 4)
	TEST_REAL_EQUAL(pc.getClusterScore(17), 7.86801)
	std::set<Index> c0 = pc.getCluster(4);
	TEST_EQUAL((c0.find(0)!=c0.end()), true)
	TEST_EQUAL((c0.find(7)!=c0.end()), true)
	TEST_EQUAL((c0.find(8)!=c0.end()), true)
	TEST_EQUAL((c0.find(19)!=c0.end()), true)
	TEST_EQUAL((c0.find(46)!=c0.end()), true)
	TEST_EQUAL((c0.find(47)!=c0.end()), true)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 46)
	TEST_EQUAL(pc.getClusterSize(28), 4)
	TEST_REAL_EQUAL(pc.getClusterScore(28), 3.51245)
	PRECISION(1e-5)
	std::set<Index> c35 = pc.getCluster(28);
	TEST_EQUAL((c35.find(31) != c35.end()), true)
	TEST_EQUAL((c35.find(32) != c35.end()), true)
	TEST_EQUAL((c35.find(51) != c35.end()), true)
	TEST_EQUAL((c35.find(52) != c35.end()), true)


	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 3.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 52)

	//               - CLINK_DEFAYS
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::CLINK_DEFAYS);

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 40.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 14)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 52)

	//               - SLINK_SIBSON
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::SLINK_SIBSON);

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 3.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 51)

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 36.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 1)

	//               - NEAREST_NEIGHBOR_CHAIN_WARD	
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 55)

	std::vector<std::set<Index> > clusters = pc.extractClustersForThreshold(15);
//pc.printClusterScores();

	TEST_EQUAL(clusters.size(), 20)
	TEST_EQUAL(pc.getNumberOfClusters(), 20)

	PRECISION(1e-3)
	TEST_EQUAL(pc.getClusterSize(0), 4)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 10.2837)
	TEST_EQUAL(pc.getClusterSize(3), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(3), 5.3962)
	TEST_EQUAL(pc.getClusterSize(7), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(7), 4.39475)

	std::set<Index> c19 = pc.getCluster(19);
	TEST_EQUAL(pc.getClusterSize(19), 6)
	TEST_REAL_EQUAL(pc.getClusterScore(19), 9.53159)
	PRECISION(1e-5)

	TEST_EQUAL((c19.find(0)  != c19.end()), true)
	TEST_EQUAL((c19.find(7)  != c19.end()), true)
	TEST_EQUAL((c19.find(8)  != c19.end()), true)
	TEST_EQUAL((c19.find(19) != c19.end()), true)
	TEST_EQUAL((c19.find(46) != c19.end()), true)
	TEST_EQUAL((c19.find(47) != c19.end()), true)

	clusters = pc.extractClustersForThreshold(5.0);

	TEST_EQUAL(pc.getNumberOfClusters(), 36)
	TEST_EQUAL(pc.getClusterSize(20), 4)
	std::set<Index> c1 = pc.getCluster(20);
	TEST_EQUAL((c1.find(31) != c1.end()), true)
	TEST_EQUAL((c1.find(32) != c1.end()), true)
	TEST_EQUAL((c1.find(51) != c1.end()), true)
	TEST_EQUAL((c1.find(52) != c1.end()), true)

	clusters = pc.extractClustersForThreshold(1.5);

	TEST_EQUAL(pc.getNumberOfClusters(), 53)

RESULT


CHECK(PoseClustering::refineClustering)
	// --------- TRIVIAL_COMPLETE_LINKAGE  --  SNAPSHOT_RMSD
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();

	PoseClustering pc;
	pc.setConformationSet(&cs2);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 51.00);
	pc.compute();

	TEST_EQUAL(pc.getNumberOfClusters(), 2)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 38.8396)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 1.54954e-05)
	PRECISION(1e-5)

	Options refined_options = pc.options;
	refined_options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	refined_options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5.00);

	pc.refineClustering(refined_options);

	TEST_EQUAL(pc.getNumberOfClusters(), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 4.42926)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 0)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 2.70327)
	TEST_REAL_EQUAL(pc.getClusterScore(3), 2.14352)
	PRECISION(1e-5)

	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::CLINK_DEFAYS);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);

	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 10);
	pc.compute();

	TEST_EQUAL(pc.getNumberOfClusters(), 5)
	refined_options = pc.options;
	refined_options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5);
	pc.refineClustering(refined_options);

	TEST_EQUAL(pc.getNumberOfClusters(), 7)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 0)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 1.84453)

	// --------- TRIVIAL_COMPLETE_LINKAGE  --  RIGID_RMSD

	PDBFile pdb2(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys2;
	pdb2.read(sys2);

	PoseClustering pc2;
	pc2.setBaseSystemAndTransformations(sys2, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc2.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	pc2.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc2.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 10.00);
	pc2.compute();

	TEST_EQUAL(pc2.getNumberOfClusters(), 28)
	TEST_EQUAL(pc2.getClusterSize(2), 2)

	refined_options = pc2.options;
	pc2.refineClustering(refined_options);
	TEST_EQUAL(pc2.getNumberOfClusters(), 28)
	TEST_EQUAL(pc2.getClusterSize(2), 2)

	refined_options = pc2.options;
	refined_options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5);
	pc2.refineClustering(refined_options);

	TEST_EQUAL(pc2.getNumberOfClusters(), 39)
	TEST_EQUAL(pc2.getClusterSize(2), 2)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 1.84453)


	// --------- NEAREST NEIGHBOR --  RIGID_RMSD

	PoseClustering pc3;
	pc3.setBaseSystemAndTransformations(sys2, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc3.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	pc3.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc3.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 10.00);

	pc3.compute();

	TEST_EQUAL(pc3.getNumberOfClusters(), 23)
	TEST_EQUAL(pc3.getClusterSize(2), 4)
	refined_options = pc3.options;
	refined_options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5);
	pc3.refineClustering(refined_options);

	TEST_EQUAL(pc3.getNumberOfClusters(), 30)
	TEST_EQUAL(pc3.getClusterSize(2), 4)

RESULT


CHECK(setBaseSystemAndTransformations(System const& base_system, String filename))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	PoseClustering pc2;
	pc2.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	TEST_EQUAL(pc2.getNumberOfPoses(), 55)

	pc2.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test2.txt));
	TEST_EQUAL(pc2.getNumberOfPoses(), 3)

RESULT


CHECK(Eigen::Matrix3f computeCovarianceMatrix(System const& system, Index rmsd_level_of_detail))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	PoseClustering pc;
	Eigen::Matrix3f mat = pc.computeCovarianceMatrix(sys, PoseClustering::C_ALPHA);

	PRECISION(1e-3)
	TEST_REAL_EQUAL(mat(0,0), 18.2237)
	TEST_REAL_EQUAL(mat(0,1), 1.29817)
	TEST_REAL_EQUAL(mat(0,2), 3.26258)
	TEST_REAL_EQUAL(mat(1,0), mat(0,1))
	TEST_REAL_EQUAL(mat(1,1), 57.2994)
	TEST_REAL_EQUAL(mat(1,2), -23.6853)
	TEST_REAL_EQUAL(mat(2,0), mat(0,2))
	TEST_REAL_EQUAL(mat(2,1), mat(1,2))
	TEST_REAL_EQUAL(mat(2,2), 33.833)
	PRECISION(1e-5)

RESULT


CHECK(float getRigidRMSD(Eigen::Vector3f const& t_ab, Eigen::Matrix3f const& M_ab, Eigen::Matrix3f const& covariance_matrix))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	GeometricCenterProcessor center;
	sys.apply(center);

	PoseClustering pc;

	TransformationProcessor tp;
	Matrix4x4 bm;
	bm.setTranslation(-center.getCenter());
	tp.setTransformation(bm);

	System S(sys);
	S.apply(tp);

	System S2(sys);
	S2.apply(tp);

	Eigen::Matrix3f covariance_matrix = PoseClustering::computeCovarianceMatrix(sys, PoseClustering::ALL_ATOMS);

	Vector3 axis(3., 1., 4.);
	axis.normalize();

	bm.setRotation(Angle(2.19), axis);
	bm.m14 = 1.0; bm.m24 = 2.0; bm.m34 = 4.5;

	Eigen::Vector3f t1(1.0, 2.0, 4.5);
	Eigen::Matrix3f m1;
	m1 << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;

	tp.setTransformation(bm);
	S.apply(tp);

	bm.setIdentity();
	Eigen::Vector3f t2(0.07, 1., 3.1);

	axis = Vector3(1., 2., 1.);
	axis.normalize();

	bm.setRotation(Angle(1.), axis);
	bm.m14 = 0.07; bm.m24 = 1.; bm.m34 = 3.1;
	Eigen::Matrix3f m2;
	m2 << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;

	tp.setTransformation(bm);
	S2.apply(tp);

	StructureMapper mapme(S, S2);

	// NOTE: the computation of the rmsd is not that stable that we should expect
	//       the results to be equal to more than the first digit
	PRECISION(1e-1)
	TEST_REAL_EQUAL(PoseClustering::getRigidRMSD(t1-t2, m1-m2, covariance_matrix), mapme.calculateRMSD())
	PRECISION(1e-5)

RESULT


CHECK(getReducedConformationSet())
	//SNAPSHOT
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs;
	cs.setup(sys);
	cs.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs.resetScoring();

	PoseClustering pc;
	pc.setConformationSet(&cs);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 4)

	boost::shared_ptr<ConformationSet> cs3 = pc.getReducedConformationSet();
	TEST_EQUAL(cs3->size(),  pc.getNumberOfClusters())
	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs3->getSystem().getProtein(0)->countAtoms())

	System sys3(cs3->getSystem());
	(*cs3)[3].applySnapShot(sys3);
	System sys0(cs3->getSystem());
	(*cs3)[0].applySnapShot(sys0);

	StructureMapper mapper(sys3, sys0);
  mapper.calculateDefaultBijection();

	PRECISION(1e-3)
	// should be larger than PoseClustering::Option::DISTANCE_THRESHOLD
  TEST_REAL_EQUAL(mapper.calculateRMSD(), 37.2132)
	TEST_REAL_EQUAL(pc.getScore(sys3, sys0, pc.options), 37.0582)
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	TEST_REAL_EQUAL(pc.getScore(sys3, sys0, pc.options), 37.1737)
	PRECISION(1e-5)

	// RIGID
	PoseClustering pc_rigid;
	pc_rigid.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc_rigid.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc_rigid.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc_rigid.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 15.00);

	pc_rigid.compute();
	TEST_EQUAL(pc_rigid.getNumberOfClusters(), 20)

	boost::shared_ptr<ConformationSet> cs_rigid = pc_rigid.getReducedConformationSet();
	TEST_EQUAL(cs_rigid->size(), pc_rigid.getNumberOfClusters())
	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs_rigid->getSystem().getProtein(0)->countAtoms())

	System sys18(cs_rigid->getSystem());
	(*cs_rigid)[18].applySnapShot(sys18);
	System sys1(cs3->getSystem());
	(*cs_rigid)[0].applySnapShot(sys1);

	StructureMapper mapper_rigid(sys18, sys1);
  mapper_rigid.calculateDefaultBijection();
	// should be larger than PoseClustering::Option::DISTANCE_THRESHOLD	
	TEST_EQUAL(mapper_rigid.calculateRMSD() >= pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	TEST_EQUAL(pc_rigid.getScore(sys18, sys1, pc_rigid.options) >= pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)

	PRECISION(1e-2)
	TEST_REAL_EQUAL(pc_rigid.getScore(sys18, sys1, pc_rigid.options), 62.821)
	PRECISION(1e-5)

	// NearestNeighborChain
	PoseClustering pc_nnw;
	pc_nnw.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc_nnw.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc_nnw.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc_nnw.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 15.00);

	pc_nnw.compute();
	TEST_EQUAL(pc_nnw.getNumberOfClusters(), 20)

	boost::shared_ptr<ConformationSet> cs_nnw = pc_nnw.getReducedConformationSet();
	TEST_EQUAL(cs_nnw->size(),  pc_nnw.getNumberOfClusters())
	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs_nnw->getSystem().getProtein(0)->countAtoms())

	System sys17(cs_nnw->getSystem());
	(*cs_nnw)[17].applySnapShot(sys17);
	System sys_nnw(cs3->getSystem());
	(*cs_nnw)[0].applySnapShot(sys_nnw);

	StructureMapper mapper_nnw(sys17, sys_nnw);
  mapper_nnw.calculateDefaultBijection();
	// should be larger than PoseClustering::Option::DISTANCE_THRESHOLD	
	//TODO: not really what we want to do: Ward distance vs RMSD
	TEST_EQUAL(mapper_nnw.calculateRMSD() >= pc_nnw.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)

	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc_nnw.getScore(sys17, sys_nnw, pc_nnw.options), 98.7483)
	PRECISION(1e-5)

RESULT


CHECK(getClusterRepresentative(Index i))
	//SNAPSHOT
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs;
	cs.setup(sys);
	cs.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs.resetScoring();
	PoseClustering pc(&cs, 5.00);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
	pc.compute();

	boost::shared_ptr<System> sys4 = pc.getClusterRepresentative(3);
	TEST_EQUAL(sys4->getProtein(0)->countAtoms(), sys.getProtein(0)->countAtoms())
	boost::shared_ptr<System> sys3 = pc.getClusterRepresentative(2);
	TEST_EQUAL(sys3->getProtein(0)->countAtoms(), 454)

	StructureMapper mapper(*sys3, *sys4);
  mapper.calculateDefaultBijection();
	// should be larger than PoseClustering::Option::DISTANCE_THRESHOLD	
	PRECISION(1e-3)
  TEST_REAL_EQUAL(mapper.calculateRMSD(), 53.895)
	TEST_REAL_EQUAL(pc.getScore(*sys4, *sys3, pc.options), 53.9642)
	PRECISION(1e-5)

	// RIGID
	PoseClustering pc_rigid;
	pc_rigid.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc_rigid.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc_rigid.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc_rigid.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 15.00);

	pc_rigid.compute();
	TEST_EQUAL(pc_rigid.getNumberOfClusters(), 20)

	boost::shared_ptr<System> sys19 = pc_rigid.getClusterRepresentative(18);
	TEST_EQUAL(sys19->getProtein(0)->countAtoms(), sys.getProtein(0)->countAtoms())
	boost::shared_ptr<System> sys6 = pc_rigid.getClusterRepresentative(5);
	TEST_EQUAL(sys6->getProtein(0)->countAtoms(), 454)

	boost::shared_ptr<System> sys34 = pc_rigid.getPose(*(++pc_rigid.getCluster(18).begin())); //34);
	boost::shared_ptr<System> sys54 = pc_rigid.getPose(*(++pc_rigid.getCluster( 5).begin())); //54);

	PoseClustering pc_test;
	pc_test.options = pc_rigid.options;

	TEST_EQUAL(pc_test.getScore(*sys6,  *sys19, pc_rigid.options) > pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	TEST_EQUAL(pc_test.getScore(*sys6,  *sys34, pc_rigid.options) > pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	TEST_EQUAL(pc_test.getScore(*sys54, *sys19, pc_rigid.options) > pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	TEST_EQUAL(pc_test.getScore(*sys54, *sys34, pc_rigid.options) > pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)

	pc_rigid.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc_test.getScore(*sys6,  *sys19, pc_rigid.options), 75.1715)
	TEST_REAL_EQUAL(pc_test.getScore(*sys6,  *sys34, pc_rigid.options), 77.8636)
	TEST_REAL_EQUAL(pc_test.getScore(*sys54, *sys19, pc_rigid.options), 76.2723)
	TEST_REAL_EQUAL(pc_test.getScore(*sys54, *sys34, pc_rigid.options), 79.1081)
	PRECISION(1e-5)


	// NearestNeighborChain
	PoseClustering pc_nnw;
	pc_nnw.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc_nnw.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc_nnw.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc_nnw.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 15.00);

	pc_nnw.compute();
	TEST_EQUAL(pc_nnw.getNumberOfClusters(), 20)

	boost::shared_ptr<System> sys18 = pc_nnw.getClusterRepresentative(17);
	TEST_EQUAL(sys18->getProtein(0)->countAtoms(), sys.getProtein(0)->countAtoms())
	boost::shared_ptr<System> sys5 = pc_nnw.getClusterRepresentative(4);
	TEST_EQUAL(sys5->getProtein(0)->countAtoms(), 454)

	PRECISION(1e-1)
	TEST_REAL_EQUAL(pc_nnw.getScore(*sys18, *sys5, pc_nnw.options), 73.7363)
	PRECISION(1e-5)

RESULT


CHECK(getClusterConformationSet(Index i))
	//SNAPSHOT
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs;
	cs.setup(sys);
	cs.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs.resetScoring();
	PoseClustering pc(&cs, 10.00);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);

	pc.compute();

	boost::shared_ptr<ConformationSet> cs2 = pc.getClusterConformationSet(2);
	TEST_EQUAL(cs2->size(), 2)

	boost::shared_ptr<ConformationSet> cs1 = pc.getClusterConformationSet(1);
	TEST_EQUAL(cs1->size(), 2)

	boost::shared_ptr<ConformationSet> cs0 = pc.getClusterConformationSet(0);
	TEST_EQUAL(cs0->size(), 4)

	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs2->getSystem().getProtein(0)->countAtoms())

	// different clusters
	System sys2(cs2->getSystem());
	(*cs2)[1].applySnapShot(sys2);
	System sys1(cs1->getSystem());
	(*cs0)[2].applySnapShot(sys1);

	StructureMapper mapper(sys2, sys1);
  mapper.calculateDefaultBijection();
	// should be larger than PoseClustering::Option::DISTANCE_THRESHOLD
  TEST_EQUAL(mapper.calculateRMSD() > pc.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	PRECISION(1e-1)
	TEST_REAL_EQUAL(pc.getScore(sys2, sys1, pc.options), 36.2734)
  PRECISION(1e-5)

	// same cluster
	(*cs0)[0].applySnapShot(sys2);
	// should be smaller than PoseClustering::Option::DISTANCE_THRESHOLD
	PRECISION(1e-1)
  TEST_REAL_EQUAL(mapper.calculateRMSD(), 2.996)
  TEST_EQUAL(mapper.calculateRMSD() < pc.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	TEST_REAL_EQUAL(pc.getScore(sys2, sys1, pc.options), 2.97102)
	PRECISION(1e-5)

	// RIGID
	PoseClustering pc_rigid(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc_rigid.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc_rigid.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc_rigid.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 15.00);

	pc_rigid.compute();
	TEST_EQUAL(pc_rigid.getNumberOfClusters(), 20)

	boost::shared_ptr<ConformationSet> cs4_r = pc_rigid.getClusterConformationSet(4);
	TEST_EQUAL(cs4_r->size(), 6)

	boost::shared_ptr<ConformationSet> cs17_r = pc_rigid.getClusterConformationSet(17);
	TEST_EQUAL(cs17_r->size(), 4)

	boost::shared_ptr<ConformationSet> cs19_r = pc_rigid.getClusterConformationSet(19);
	TEST_EQUAL(cs19_r->size(), 4)

	System sys1_r(cs4_r->getSystem());
	(*cs4_r)[cs4_r->size()-1].applySnapShot(sys1_r);

	for (Position i=0; i<cs4_r->size()-1; ++i)
	{
		System sys2_r(cs4_r->getSystem());
		(*cs4_r)[i].applySnapShot(sys2_r);
		StructureMapper mapper_rigid(sys1_r, sys2_r);
		AtomBijection atom_bijection;
		atom_bijection.assignCAlphaAtoms(sys1_r, sys2_r);
		TEST_EQUAL(mapper_rigid.calculateRMSD() < pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
		TEST_EQUAL(pc_rigid.getScore(sys2_r, sys1_r, pc_rigid.options) < pc_rigid.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	}

	// NearestNeighborChain
	PoseClustering pc_nnw(&cs, 10.00);
	pc_nnw.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc_nnw.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);

	pc_nnw.compute();

	boost::shared_ptr<ConformationSet> cs0_nnw = pc_nnw.getClusterConformationSet(0);
	TEST_EQUAL(cs0_nnw->size(), 2)

	boost::shared_ptr<ConformationSet> cs1_nnw = pc_nnw.getClusterConformationSet(1);
	TEST_EQUAL(cs1_nnw->size(), 2)

	boost::shared_ptr<ConformationSet> cs2_nnw = pc_nnw.getClusterConformationSet(2);
	TEST_EQUAL(cs2_nnw->size(), 4)

	TEST_EQUAL(sys.getProtein(0)->countAtoms(), cs2_nnw->getSystem().getProtein(0)->countAtoms())

	// different clusters
	System sys2_nnw(cs2_nnw->getSystem());
	(*cs2_nnw)[2].applySnapShot(sys2_nnw);
	System sys1_nnw(cs1_nnw->getSystem());
	(*cs1_nnw)[1].applySnapShot(sys1_nnw);

	StructureMapper mapper_nnw(sys2_nnw, sys1_nnw);
  mapper_nnw.calculateDefaultBijection();
	// should be larger than PoseClustering::Option::DISTANCE_THRESHOLD	
	//TODO: not really what we want to do: Ward distance vs RMSD
  TEST_EQUAL(mapper_nnw.calculateRMSD() > pc_nnw.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	PRECISION(1e-1)
	TEST_REAL_EQUAL(pc_nnw.getScore(sys2_nnw, sys1_nnw, pc_nnw.options), 36.2734)
	PRECISION(1e-5)

	// same cluster
	(*cs1_nnw)[0].applySnapShot(sys2_nnw);
	// should be smaller than PoseClustering::Option::DISTANCE_THRESHOLD
	PRECISION(1e-1)
  TEST_REAL_EQUAL(mapper_nnw.calculateRMSD(), 2.84235)
	//TODO: not really what we want to do: Ward distance vs RMSD
  TEST_EQUAL(mapper_nnw.calculateRMSD() < pc_nnw.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), true)
	TEST_REAL_EQUAL(pc_nnw.getScore(sys2_nnw, sys1_nnw, pc_nnw.options), 2.675794)
	PRECISION(1e-5)

RESULT


CHECK(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL for SNAPSHOT_RMSD)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.add(0, sys);

	// move backbone nitrogens away
	System sys2(sys);
	for (AtomIterator at_it = sys2.beginAtom(); +at_it; ++at_it)
	{
		if (at_it->getName() == "N")
		{
			at_it->setPosition(at_it->getPosition() + Vector3(100, 0, 0));
		}
	}
	cs2.add(0, sys2);

	// move non-backbone atoms away
	for (AtomIterator at_it = sys2.beginAtom(); +at_it; ++at_it)
	{
		if (at_it->getName() == "CG")
		{
			at_it->setPosition(at_it->getPosition() + Vector3(100, 0, 0));
		}
	}
	cs2.add(0, sys2);

	PoseClustering pc;

	// Option::RMSD_LEVEL_OF_DETAIL::C_ALPHA	
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);

	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::C_ALPHA);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::C_ALPHA)
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 25);
	TEST_REAL_EQUAL(pc.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), 25.00)

	pc.setConformationSet(&cs2);
	pc.compute();

	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_EQUAL(pc.getClusterConformationSet(0)->size(), 3)

	// Option::RMSD_LEVEL_OF_DETAIL::BACKBONE;
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::BACKBONE);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::BACKBONE)
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 25);

	pc.setConformationSet(&cs2);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)

	// Option::RMSD_LEVEL_OF_DETAIL::ALL_ATOMS
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::ALL_ATOMS);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::ALL_ATOMS)
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 25);

	pc.setConformationSet(&cs2);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
RESULT


CHECK(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL for RIGID_RMSD)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	// move a backbone nitrogen and a CG away
	for (AtomIterator at_it = sys.beginAtom(); +at_it; ++at_it)
	{
		if (at_it->getName() == "N")
		{
			at_it->setPosition(at_it->getPosition() + Vector3(100, 0, 0));
		}
		if (at_it->getName() == "CG")
		{
			at_it->setPosition(at_it->getPosition() + Vector3(0, 5000, 0));
		}
	}

	PoseClustering pc;
	pc.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test2.txt));

	// Option::RMSD_LEVEL_OF_DETAIL::C_ALPHA	
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD); //TRIVIAL_COMPLETE_LINKAGE);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::C_ALPHA);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::C_ALPHA)
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 25);
	TEST_REAL_EQUAL(pc.options.getReal(PoseClustering::Option::DISTANCE_THRESHOLD), 25.00)

	pc.compute();

	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_EQUAL(pc.getClusterConformationSet(0)->size(), 3)

	// Option::RMSD_LEVEL_OF_DETAIL::BACKBONE;
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::BACKBONE);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::BACKBONE)
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 25);
	pc.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test2.txt));

	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)

	// Option::RMSD_LEVEL_OF_DETAIL::ALL_ATOMS
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::ALL_ATOMS);
	TEST_EQUAL(pc.options.getInteger(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL), PoseClustering::ALL_ATOMS)
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 25);
	pc.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test2.txt));

	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
RESULT


CHECK(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL::PROPERTY_BASED_ATOM_BIJECTION)
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	ConformationSet cs2;
	// the properties have to be defined in the base system 	
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

	cs2.setup(sys);
	cs2.add(0, sys);

	sys.apply(sel);
	selected_atoms = sel.getSelectedAtoms();
	it = selected_atoms.begin();
	for ( ; it != selected_atoms.end(); ++it)
	{
		(*it)->setPosition((*it)->getPosition() + Vector3(100, 0, 0));
	}
	cs2.add(0, sys);

	TEST_EQUAL(sel.getNumberOfSelectedAtoms(), 31)

	PoseClustering pc;
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,  PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::PROPERTY_BASED_ATOM_BIJECTION);
	pc.options.set(PoseClustering::Option::DISTANCE_THRESHOLD, 99);//25);
	pc.setConformationSet(&cs2);
	TEST_EQUAL(pc.getNumberOfPoses(), 2)
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 2)

	pc.options.set(PoseClustering::Option::DISTANCE_THRESHOLD, 101);//30);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 1)

	PoseClustering pc_rigid;
	pc_rigid.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test2.txt));
	pc_rigid.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);
	pc_rigid.options.set(PoseClustering::Option::CLUSTER_METHOD,   PoseClustering::TRIVIAL_COMPLETE_LINKAGE);
	pc_rigid.options.set(PoseClustering::Option::DISTANCE_THRESHOLD, 25);

	pc_rigid.compute();
	TEST_EQUAL(pc_rigid.getNumberOfClusters(), 2)

	pc_rigid.options.set(PoseClustering::Option::DISTANCE_THRESHOLD, 30);
	pc_rigid.compute();
	TEST_EQUAL(pc_rigid.getNumberOfClusters(), 1)

	//               - NEAREST_NEIGHBOR_CHAIN_WARD	
	PoseClustering pc_nncw;
	pc_nncw.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc_nncw.options.set(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL, PoseClustering::PROPERTY_BASED_ATOM_BIJECTION);
	pc_nncw.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc_nncw.setConformationSet(&cs2);

	pc_nncw.compute();
	TEST_EQUAL(pc_nncw.getNumberOfClusters(), 2)

	pc_nncw.extractClustersForThreshold(70);
	TEST_EQUAL(pc_nncw.getNumberOfClusters(), 2)

	pc_nncw.extractClustersForThreshold(71);
	TEST_EQUAL(pc_nncw.getNumberOfClusters(), 1)

RESULT


CHECK(computeCompleteLinkageRMSD())

	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);

	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 7.4983)
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(0, pc.options, false), 7.4983)
	Options temp_options = pc.options;
	temp_options[PoseClustering::Option::RMSD_TYPE] = PoseClustering::RIGID_RMSD;
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(0, temp_options, true), 7.37874)
	temp_options[PoseClustering::Option::RMSD_TYPE] = PoseClustering::CENTER_OF_MASS_DISTANCE;
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(0, temp_options, true), 3.52106)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 2.65794)
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(2, pc.options, false), 2.65794)

	// SNAPSHOT_RMSD -- NN-ChainWard 
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 100.00);

	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 1)

	PRECISION(1e-2)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 67.8968)
	// NOTE: Ward score and RMSD do not have to be the same!
	TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(0, pc.options, true),  60.6787)
	// TODO: find out how to catch an error in tests
	/*try {
		TEST_REAL_EQUAL(pc.computeCompleteLinkageRMSD(2, pc.options, false), 0)
	}catch(...)
	{
	}*/
	PRECISION(1e-5)

RESULT


CHECK(extractNBestClusters(Size n, Size i))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs;
	cs.setup(sys);
	cs.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs.resetScoring();

	PoseClustering pc;
	pc.setConformationSet(&cs);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD,   PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 5.00);
	pc.compute();

	TEST_EQUAL(pc.getNumberOfClusters(), 4)

	std::vector<std::set<Index> > result = pc.extractNBestClusters(4);
	TEST_EQUAL(result.size(), 4)
	TEST_EQUAL(pc.getNumberOfClusters(), 4)

	result = pc.extractNBestClusters(4);
	TEST_EQUAL(result.size(), 4)
	TEST_EQUAL(pc.getNumberOfClusters(), 4)

	result = pc.filterClusters(3);
	TEST_EQUAL(result.size(), 1)
	TEST_EQUAL(pc.getNumberOfClusters(), 1)


	// TODO: find out how to catch an error in tests
//	result = pc.extractNBestClusters(pc.getNumberOfPoses()+1);
//	result = pc.extractNBestClusters(-1);
	result = pc.extractNBestClusters(0);
	TEST_EQUAL(pc.getNumberOfClusters(), 1)

	result = pc.extractNBestClusters(pc.getNumberOfPoses());
	TEST_EQUAL(result.size(), 8)
	TEST_EQUAL(pc.getNumberOfClusters(), 8)

	result = pc.extractNBestClusters(pc.getNumberOfPoses());
	TEST_EQUAL(result.size(), 8)
	TEST_EQUAL(pc.getNumberOfClusters(), 8)

	result = pc.filterClusters(2);
	TEST_EQUAL(result.size(), 0)
	TEST_EQUAL(pc.getNumberOfClusters(), 0)

	result = pc.extractNBestClusters(1);
	TEST_EQUAL(result.size(), 1)
	TEST_EQUAL(pc.getNumberOfClusters(), 1)

RESULT



CHECK(extractClustersForThreshold(Size n, Size i))
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	PoseClustering pc;
	pc.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	//               - NEAREST_NEIGHBOR_CHAIN_WARD	
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 55)

	std::vector<std::set<Index> > clusters = pc.extractClustersForThreshold(15);
	TEST_EQUAL(clusters.size(), 20)
	TEST_EQUAL(pc.getNumberOfClusters(), 20)

	std::vector<std::set<Index> > clusters_2 = pc.extractClustersForThreshold(15, 2);
	TEST_EQUAL(clusters_2.size(), 20)
	TEST_EQUAL(pc.getNumberOfClusters(), 20)

	std::vector<std::set<Index> > clusters_5 = pc.extractClustersForThreshold(15, 5);
	TEST_EQUAL(clusters_5.size(), 1)
	TEST_EQUAL(pc.getNumberOfClusters(), 1)

	PRECISION(1e-3)
	TEST_EQUAL(pc.getClusterSize(0), 6)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 9.53159)

	std::set<Index> c0 = pc.getCluster(0);
	TEST_EQUAL(pc.getClusterSize(0), 6)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 9.53159)
	PRECISION(1e-5)

	TEST_EQUAL((c0.find(0)  != c0.end()), true)
	TEST_EQUAL((c0.find(7)  != c0.end()), true)
	TEST_EQUAL((c0.find(8)  != c0.end()), true)
	TEST_EQUAL((c0.find(19) != c0.end()), true)
	TEST_EQUAL((c0.find(46) != c0.end()), true)
	TEST_EQUAL((c0.find(47) != c0.end()), true)

	////////////////////////////////////////////////
	clusters = pc.extractClustersForThreshold(5.0);

	TEST_EQUAL(pc.getNumberOfClusters(), 36)
	TEST_EQUAL(pc.getClusterSize(20), 4)
	std::set<Index> c1 = pc.getCluster(20);
	TEST_EQUAL((c1.find(31) != c1.end()), true)
	TEST_EQUAL((c1.find(32) != c1.end()), true)
	TEST_EQUAL((c1.find(51) != c1.end()), true)
	TEST_EQUAL((c1.find(52) != c1.end()), true)
	clusters = pc.extractClustersForThreshold(5.0);

	TEST_EQUAL(pc.getNumberOfClusters(), 36)
	TEST_EQUAL(pc.getClusterSize(20), 4)

	std::set<Index> c2 = pc.getCluster(20);
	TEST_EQUAL((c1.find(31) != c1.end()), true)
	TEST_EQUAL((c1.find(32) != c1.end()), true)
	TEST_EQUAL((c1.find(51) != c1.end()), true)
	TEST_EQUAL((c1.find(52) != c1.end()), true)

	///////////////////////////////////////////////
	pc.extractClustersForThreshold(1.5);

	TEST_EQUAL(pc.getNumberOfClusters(), 53)

	pc.extractClustersForThreshold(1.5, 2);

	TEST_EQUAL(pc.getNumberOfClusters(), 2)

RESULT


CHECK(serializeWardClusterTree(std::ostream& out))
	String filename;
	NEW_TMP_FILE(filename)

	// First, cluster and serialize ClusterTree
	File temp_tree(filename, std::ios::out);

	PoseClustering pc;
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);

	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();

	pc.setConformationSet(&cs2);
	pc.compute();
	pc.serializeWardClusterTree(temp_tree);

	temp_tree.close();


	// Second, deserialize and check ClusterTree
	temp_tree.open(filename, std::ios::in);

	PoseClustering pc2;
	pc2.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
	pc2.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc2.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0);

	pc2.deserializeWardClusterTree(temp_tree);

	std::vector<std::set<Index> > clusters = pc2.extractClustersForThreshold(0.5);
	TEST_EQUAL(clusters.size(), 6)
	TEST_EQUAL(pc2.getNumberOfClusters(), 6)
	TEST_EQUAL(pc2.getClusterScore(0), 0.)

	pc2.extractClustersForThreshold(2.4);
	TEST_EQUAL(pc2.getNumberOfClusters(), 3)
	TEST_EQUAL(pc2.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc2.getClusterScore(0),  2.34918)
	PRECISION(1e-5)

	TEST_REAL_EQUAL(pc2.getClusterScore(1), 1.09569e-05)
	TEST_REAL_EQUAL(pc2.getClusterScore(2), 0.945311)

	pc2.extractClustersForThreshold(50.7);
	TEST_EQUAL(pc2.getNumberOfClusters(), 2)
	TEST_EQUAL(pc2.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc2.getClusterScore(0), 50.6247)
	TEST_REAL_EQUAL(pc2.getClusterScore(1), 2.34918)

	pc2.extractClustersForThreshold(59);
	TEST_EQUAL(pc2.getNumberOfClusters(), 1)
	TEST_REAL_EQUAL(pc2.getClusterScore(0), 58.395)
	PRECISION(1e-5)

	temp_tree.close();

RESULT


CHECK(deserializeWardClusterTree(std::istream& out))
	File tree(BALL_TEST_DATA_PATH(PoseClustering_wardtree.dat));
	PoseClustering pc;
	pc.options.set(PoseClustering::Option::RMSD_TYPE, PoseClustering::SNAPSHOT_RMSD);
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0);

	pc.deserializeWardClusterTree(tree);
	std::vector<std::set<Index> > clusters = pc.extractClustersForThreshold(0.5);
	TEST_EQUAL(clusters.size(), 6)
	TEST_EQUAL(pc.getNumberOfClusters(), 6)
	TEST_EQUAL(pc.getClusterScore(0), 0.)

	pc.extractClustersForThreshold(2.4);
	TEST_EQUAL(pc.getNumberOfClusters(), 3)
	TEST_EQUAL(pc.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0),  2.34918)
	PRECISION(1e-5)

	TEST_REAL_EQUAL(pc.getClusterScore(1), 1.09569e-05)
	TEST_REAL_EQUAL(pc.getClusterScore(2), 0.945311)

	pc.extractClustersForThreshold(50.7);
	TEST_EQUAL(pc.getNumberOfClusters(), 2)
	TEST_EQUAL(pc.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 50.6247)
	TEST_REAL_EQUAL(pc.getClusterScore(1), 2.34918)

	pc.extractClustersForThreshold(59);
	TEST_EQUAL(pc.getNumberOfClusters(), 1)
	TEST_REAL_EQUAL(pc.getClusterScore(0), 58.395)
	PRECISION(1e-5)

RESULT


CHECK(deserialize/serialize WardClusterTree(std::ostream& out))

	String filename;
  NEW_TMP_FILE(filename)
  File temp_tree(filename, std::ios::out);

	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);

	PoseClustering pc;
	pc.setBaseSystemAndTransformations(sys, BALL_TEST_DATA_PATH(PoseClustering_test.txt));
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::RIGID_RMSD);

	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 55)

	std::vector<std::set<Index> > clusters = pc.extractClustersForThreshold(15);
	TEST_EQUAL(clusters.size(), 20)
	TEST_EQUAL(pc.getNumberOfClusters(), 20)
	pc.serializeWardClusterTree(temp_tree);


/*
	PoseClustering pc;
	pc.options.setInteger(PoseClustering::Option::RMSD_TYPE, PoseClustering::CENTER_OF_MASS_DISTANCE);
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	pc.setConformationSet(&cs2);

	//               - NEAREST_NEIGHBOR_CHAIN_WARD
	pc.options.set(PoseClustering::Option::CLUSTER_METHOD, PoseClustering::NEAREST_NEIGHBOR_CHAIN_WARD);
	pc.options.setReal(PoseClustering::Option::DISTANCE_THRESHOLD, 0.00);
	pc.compute();
	TEST_EQUAL(pc.getNumberOfClusters(), 0)

	pc.extractClustersForThreshold(2.4);
	pc.exportWardClusterTreeToGraphViz(temp_tree);

*/

	temp_tree.reopen(std::ios::in);

	PoseClustering pc2;
	pc2.options = pc.options;
	pc2.deserializeWardClusterTree(temp_tree);

	pc2.extractClustersForThreshold(15);

	PRECISION(1e-3)
	TEST_EQUAL(pc2.getClusterSize(0), 4)
	TEST_REAL_EQUAL(pc2.getClusterScore(0), 10.2837)
	TEST_EQUAL(pc2.getClusterSize(3), 2)
	TEST_REAL_EQUAL(pc2.getClusterScore(3), 5.3962)
	TEST_EQUAL(pc2.getClusterSize(7), 2)
	TEST_REAL_EQUAL(pc2.getClusterScore(7), 4.39475)

	std::set<Index> c19 = pc2.getCluster(19);
	TEST_EQUAL(pc2.getClusterSize(19), 6)
	TEST_REAL_EQUAL(pc2.getClusterScore(19), 9.53159)
	PRECISION(1e-5)

	TEST_EQUAL((c19.find(0)  != c19.end()), true)
	TEST_EQUAL((c19.find(7)  != c19.end()), true)
	TEST_EQUAL((c19.find(8)  != c19.end()), true)
	TEST_EQUAL((c19.find(19) != c19.end()), true)
	TEST_EQUAL((c19.find(46) != c19.end()), true)
	TEST_EQUAL((c19.find(47) != c19.end()), true)

	clusters = pc2.extractClustersForThreshold(5.0);

	TEST_EQUAL(pc2.getNumberOfClusters(), 36)
	TEST_EQUAL(pc2.getClusterSize(20), 4)
	std::set<Index> c1 = pc2.getCluster(20);
	TEST_EQUAL((c1.find(31) != c1.end()), true)
	TEST_EQUAL((c1.find(32) != c1.end()), true)
	TEST_EQUAL((c1.find(51) != c1.end()), true)
	TEST_EQUAL((c1.find(52) != c1.end()), true)

	clusters = pc2.extractClustersForThreshold(1.5);

	TEST_EQUAL(pc2.getNumberOfClusters(), 53)

/*
	pc2.extractClustersForThreshold(2.4);

	TEST_EQUAL(pc2.getNumberOfClusters(), 3)
	TEST_EQUAL(pc2.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc2.getClusterScore(0),  2.34918)
	PRECISION(1e-5)

	TEST_REAL_EQUAL(pc2.getClusterScore(1), 1.09569e-05)
	TEST_REAL_EQUAL(pc2.getClusterScore(2), 0.945311)

	pc.extractClustersForThreshold(50.7);
	TEST_EQUAL(pc2.getNumberOfClusters(), 2)
	TEST_EQUAL(pc2.getClusterSize(0), 4)
	PRECISION(1e-3)
	TEST_REAL_EQUAL(pc2.getClusterScore(0), 50.6247)
	TEST_REAL_EQUAL(pc2.getClusterScore(1), 2.34918)

	pc.extractClustersForThreshold(59);
	TEST_EQUAL(pc2.getNumberOfClusters(), 1)
	TEST_REAL_EQUAL(pc2.getClusterScore(0), 58.395)
	PRECISION(1e-5)
	*/
RESULT


//CHECK convertTransformations2Snaphots_();

CHECK (convertSnaphots2Transformations())
	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);

	pc.convertSnaphots2Transformations();

	String filename;
	NEW_TMP_FILE(filename)
	LineBasedFile file(filename, std::ios::out);

	std::vector< PoseClustering::PosePointer > poses = pc.getPoses();
	for (Size i=0; i<poses.size(); i++)
	{
		Eigen::Matrix3f const & rot    = poses[i].trafo->rotation;
		Eigen::Vector3f const & transl = poses[i].trafo->translation;
		file << "A A " << " 0.0 "
			   << rot(0) << " " << rot(1) << " " << rot(2) << " " << transl[0] << " "
				 << rot(3) << " " << rot(4) << " " << rot(5) << " " << transl[1] << " "
				 << rot(6) << " " << rot(7) << " " << rot(8) << " " << transl[2]
				 << std::endl;
	}

	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(PoseClustering_transformFile.dat))


RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

