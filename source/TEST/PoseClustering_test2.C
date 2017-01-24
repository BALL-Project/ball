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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

