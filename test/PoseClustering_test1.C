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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

