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

	PRECISION(1e-4)

	PDBFile pdb(BALL_TEST_DATA_PATH(PoseClustering_test.pdb));
	System sys;
	pdb.read(sys);
	ConformationSet cs2;
	cs2.setup(sys);
	cs2.readDCDFile(BALL_TEST_DATA_PATH(PoseClustering_test2.dcd));
	cs2.resetScoring();
	PoseClustering pc(&cs2, 10.00);

	pc.convertSnaphots2Transformations();

	LineBasedFile in(BALL_TEST_DATA_PATH(PoseClustering_transformFile.dat), std::ios::in);

	for (Size i=0; i< pc.getPoses().size(); i++)
	{
		Eigen::Matrix3f const & rot    = pc.getPoses()[i].trafo->rotation;
		Eigen::Vector3f const & transl = pc.getPoses()[i].trafo->translation;

		in.readLine();

		TEST_REAL_EQUAL(rot(0),    in.getField(3).toFloat());
		TEST_REAL_EQUAL(rot(1),    in.getField(4).toFloat());
		TEST_REAL_EQUAL(rot(2),    in.getField(5).toFloat());
		TEST_REAL_EQUAL(transl[0], in.getField(6).toFloat());
		TEST_REAL_EQUAL(rot(3),    in.getField(7).toFloat());
		TEST_REAL_EQUAL(rot(4),    in.getField(8).toFloat());
		TEST_REAL_EQUAL(rot(5),    in.getField(9).toFloat());
		TEST_REAL_EQUAL(transl[1], in.getField(10).toFloat());
		TEST_REAL_EQUAL(rot(6),    in.getField(11).toFloat());
		TEST_REAL_EQUAL(rot(7),    in.getField(12).toFloat());
		TEST_REAL_EQUAL(rot(8),    in.getField(13).toFloat());
		TEST_REAL_EQUAL(transl[2], in.getField(14).toFloat());
	}

	in.close();

RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

