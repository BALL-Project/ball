// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/system.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/FORMAT/PDBFile.h>
#include <vector>

START_TEST(StructureMapper)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using std::vector;

StructureMapper*	m;
CHECK(StructureMapper())
	m = new StructureMapper();
	TEST_NOT_EQUAL(m, 0)
RESULT

CHECK(~StructureMapper())
	delete m;
RESULT

Fragment*	frag1 = new Fragment;
Fragment*	frag2 = new Fragment;
Matrix4x4	t;
CHECK(StructureMapper(AtomContainer& A, AtomContainer& B))
	Atom*	atom1 = new Atom;
	Atom*	atom2 = new Atom;
	Atom*	atom3 = new Atom;
	Atom*	atom4 = new Atom;
	Atom*	atom5 = new Atom;
	Atom*	atom6 = new Atom;
	Atom*	atom7 = new Atom;
	Atom*	atom8 = new Atom;
	// create two fragments containing atoms
	frag1->insert(*atom1);
	frag1->insert(*atom2);
	frag1->insert(*atom3);
	frag1->insert(*atom4);
	frag2->insert(*atom5);
	frag2->insert(*atom6);
	frag2->insert(*atom7);
	frag2->insert(*atom8);
	// set coordinates for all atoms
	Vector3	v(0,0,0);
	atom1->setPosition(v);
	v.set(1,1,3);
	atom2->setPosition(v);
	v.set(2,5,3);
	atom3->setPosition(v);
	v.set(2,3,2);
	atom4->setPosition(v);
	Quaternion	q(7,9,12,Constants::PI * 0.6);
	t = q.getRotationMatrix(t);
	atom5->setPosition(t * atom1->getPosition());
	atom6->setPosition(t * atom2->getPosition());
	atom7->setPosition(t * atom3->getPosition());
	atom8->setPosition(t * atom4->getPosition());
	atom1->setName("A1");
	atom2->setName("A2");
	atom3->setName("A3");
	atom4->setName("A4");
	atom5->setName("A1");
	atom6->setName("A2");
	atom7->setName("A3");
	atom8->setName("A4");
	m = new StructureMapper(*frag1, *frag2);
	TEST_NOT_EQUAL(m, 0)
RESULT

CHECK(bool mapFragments(const vector<Fragment*>& A, const vector<Fragment*>& B, Matrix4x4* transformation, double upper_bound = 8.0, double lower_bound = 2.5))
	Matrix4x4	map_result;
	vector<Fragment*>	A;
	vector<Fragment*>	B;
	A.push_back(frag1);
	B.push_back(frag2);
	TEST_EQUAL(m->mapFragments(A, B, &map_result), true)
	TEST_REAL_EQUAL(map_result.m11, t.m11)
	TEST_REAL_EQUAL(map_result.m12, t.m12)
	TEST_REAL_EQUAL(map_result.m13, t.m13)
	TEST_REAL_EQUAL(map_result.m14, t.m14)
	TEST_REAL_EQUAL(map_result.m21, t.m21)
	TEST_REAL_EQUAL(map_result.m22, t.m22)
	TEST_REAL_EQUAL(map_result.m23, t.m23)
	TEST_REAL_EQUAL(map_result.m24, t.m24)
	TEST_REAL_EQUAL(map_result.m31, t.m31)
	TEST_REAL_EQUAL(map_result.m32, t.m32)
	TEST_REAL_EQUAL(map_result.m33, t.m33)
	TEST_REAL_EQUAL(map_result.m34, t.m34)
	TEST_REAL_EQUAL(map_result.m41, t.m41)
	TEST_REAL_EQUAL(map_result.m42, t.m42)
	TEST_REAL_EQUAL(map_result.m43, t.m43)
	TEST_REAL_EQUAL(map_result.m44, t.m44)
RESULT

CHECK(static Matrix4x4 matchPoints(const Vector3& w1, const Vector3& w2, const Vector3& w3, const Vector3& v1, const Vector3& v2, const Vector3& v3))
	PRECISION(1e-5)
	Vector3 v1(2.0, 2.0, 2.0);
	Vector3 v2(3.0, 2.0, 2.0);
	Vector3 v3(2.0, 3.0, 2.0);

	Vector3 w1(1.0, 1.0, 1.0);
	Vector3 w2(2.0, 1.0, 1.0);
	Vector3 w3(1.0, 2.0, 1.0);

	Matrix4x4 T = StructureMapper::matchPoints(w1, w2, w3, v1, v2, v3);
	STATUS("transformation:\n" << T)
	TEST_REAL_EQUAL(T.m11, 1.0)
	TEST_REAL_EQUAL(T.m12, 0.0)
	TEST_REAL_EQUAL(T.m13, 0.0)
	TEST_REAL_EQUAL(T.m14, 1.0)
	TEST_REAL_EQUAL(T.m21, 0.0)
	TEST_REAL_EQUAL(T.m22, 1.0)
	TEST_REAL_EQUAL(T.m23, 0.0)
	TEST_REAL_EQUAL(T.m24, 1.0)
	TEST_REAL_EQUAL(T.m31, 0.0)
	TEST_REAL_EQUAL(T.m32, 0.0)
	TEST_REAL_EQUAL(T.m33, 1.0)
	TEST_REAL_EQUAL(T.m34, 1.0)
	TEST_REAL_EQUAL(T.m41, 0.0)
	TEST_REAL_EQUAL(T.m42, 0.0)
	TEST_REAL_EQUAL(T.m43, 0.0)
	TEST_REAL_EQUAL(T.m44, 1.0)

	TEST_REAL_EQUAL((T * w1 - v1).getSquareLength(), 0.0)
	TEST_REAL_EQUAL((T * w2 - v2).getSquareLength(), 0.0)
	TEST_REAL_EQUAL((T * w3 - v3).getSquareLength(), 0.0)

	w1.set(5.0, 0.0, 0.0);
	w2.set(5.0, 1.0, 0.0);
	w3.set(5.0, 2.0, 1.0);
	
	T = StructureMapper::matchPoints(w1, w2, w3, v1, v2, v3);
	STATUS("transformation:\n" << T)
	TEST_REAL_EQUAL(T.m11, 0.0)
	TEST_REAL_EQUAL(T.m12, 1.0)
	TEST_REAL_EQUAL(T.m13, 0.0)
	TEST_REAL_EQUAL(T.m14, 2.0)
	TEST_REAL_EQUAL(T.m21, 0.0)
	TEST_REAL_EQUAL(T.m22, 0.0)
	TEST_REAL_EQUAL(T.m23, 1.0)
	TEST_REAL_EQUAL(T.m24, 2.0)
	TEST_REAL_EQUAL(T.m31, 1.0)
	TEST_REAL_EQUAL(T.m32, 0.0)
	TEST_REAL_EQUAL(T.m33, 0.0)
	TEST_REAL_EQUAL(T.m34, -3.0)
	TEST_REAL_EQUAL(T.m41, 0.0)
	TEST_REAL_EQUAL(T.m42, 0.0)
	TEST_REAL_EQUAL(T.m43, 0.0)
	TEST_REAL_EQUAL(T.m44, 1.0)

	TEST_REAL_EQUAL((T * w1 - v1).getSquareLength(), 0.0)
	TEST_REAL_EQUAL((T * w2 - v2).getSquareLength(), 0.0)
	TEST_REAL_EQUAL((T * w3 - v3).getSquareLength(), 4.0)

	// test co-linear case
	w1.set(5.0, 0.0, 0.0);
	w2.set(5.0, 1.0, 0.0);
	w3.set(5.0, 2.0, 0.0);
	
	T = StructureMapper::matchPoints(w1, w2, w3, v1, v2, v3);
	STATUS("transformation:\n" << T)
	TEST_REAL_EQUAL(T.m14, 2.0)
	TEST_REAL_EQUAL(T.m24, -3.0)
	TEST_REAL_EQUAL(T.m34, 2.0)
	TEST_REAL_EQUAL(T.m44, 1.0)

	TEST_REAL_EQUAL((T * w1 - v1).getSquareLength(), 0.0)
	TEST_REAL_EQUAL((T * w2 - v2).getSquareLength(), 0.0)

	// Test some weird pathological case found in 1BNA
	// when using ReconstructFragmentProcessor
	v1.set(17.221, 25.499, 18.629);
	v2.set(18.217, 24.603, 18.897);
	v3.set(19.526, 24.945, 18.571);
	w1.set(22.167, 23.241, 33.694);
	w2.set(21.100, 23.907, 33.243);
	w3.set(20.022, 24.211, 34.147);
	T = StructureMapper::matchPoints(w1, w2, w3, v1, v2, v3);
	STATUS(T)
	TEST_EQUAL(Maths::isNan(T(0,0)), false)
	TEST_EQUAL(Maths::isNan(T(1,0)), false)
	TEST_EQUAL(Maths::isNan(T(2,0)), false)
	TEST_EQUAL(Maths::isNan(T(0,1)), false)
	TEST_EQUAL(Maths::isNan(T(1,1)), false)
	TEST_EQUAL(Maths::isNan(T(2,1)), false)
	TEST_EQUAL(Maths::isNan(T(0,2)), false)
	TEST_EQUAL(Maths::isNan(T(1,2)), false)
	TEST_EQUAL(Maths::isNan(T(2,2)), false)
	TEST_REAL_EQUAL(T(3,3), 1.0)
RESULT

CHECK(double calculateRMSD())
	Fragment f1,f2;
	Atom a1,a2,b1,b2;
	a1.setName("a1");
	a2.setName("a2");
	b1.setName("b1");
	b2.setName("b2");
	a1.setPosition(Vector3(1,0,0));
	b1.setPosition(Vector3(0,-1,0));
	f1.insert(a1);
	f1.insert(b1);
	f2.insert(a2);
	f2.insert(b2);
	StructureMapper m(f1,f2);
	TEST_REAL_EQUAL(m.calculateRMSD(), 1.0)
RESULT

CHECK((Matrix4x4 mapProteins(Protein& P1, Protein& P2, map<String, Size>& type_map, Size& no_matched_ca, double& rmsd, double upper_bound = 8.0, double lower_bound = 4.0, double tolerance = 0.6)))
  // ???
RESULT

CHECK((Size mapResiduesByBackbone(const list<Residue*>& l1, const list<Residue*>& l2)))
  // ???
RESULT

CHECK((StructureMapper::AtomBijection calculateFragmentBijection(const vector<Fragment*>& A, const vector<Fragment*>& B)))
  // ???
RESULT

CHECK(bool calculateTransformation())
  // ???
RESULT

CHECK((static Matrix4x4 matchBackboneAtoms(const Residue& r1, const Residue& r2)))
  // ???
RESULT

CHECK((vector<vector<Fragment*>& searchPattern(vector<Fragment*>& pattern, AtomContainer& composite, double max_rmsd = 4.0, double max_center_tolerance = 2.0, double upper_bound = 8.0, double lower_bound = 4.0)))
  // ???
RESULT

CHECK(const AtomBijection& getBijection() const)
	// not much to be tested here...
	StructureMapper sm;
	TEST_EQUAL(sm.getBijection().size(), 0)
RESULT

CHECK(void calculateDefaultBijection())
	{
		AtomContainer ac1;
		Residue* r1 = new Residue;
		Residue* r2 = new Residue;
		ac1.insert(*r1);
		ac1.insert(*r2);
		r1->setName("RES1");
		r2->setName("RES2");
		PDBAtom* a1 = new PDBAtom;
		PDBAtom* a2 = new PDBAtom;
		PDBAtom* a3 = new PDBAtom;
		PDBAtom* a4 = new PDBAtom;
		a1->setName("A1");
		a2->setName("A2");
		a3->setName("A3");
		a4->setName("A4");
		r1->insert(*a1);
		r1->insert(*a2);
		r2->insert(*a3);
		r2->insert(*a4);

		AtomContainer ac2;
		Residue* r3 = new Residue;
		Residue* r4 = new Residue;
		ac2.insert(*r3);
		ac2.insert(*r4);
		r3->setName("RES1");
		r4->setName("RES2");
		PDBAtom* a5 = new PDBAtom;
		PDBAtom* a6 = new PDBAtom;
		PDBAtom* a7 = new PDBAtom;
		PDBAtom* a8 = new PDBAtom;
		a5->setName("A4");
		a6->setName("A3");
		a7->setName("A2");
		a8->setName("A1");
		r4->insert(*a6);
		r4->insert(*a5);
		r3->insert(*a8);
		r3->insert(*a7);

		StructureMapper sm(ac1, ac2);
		sm.calculateDefaultBijection();
		TEST_EQUAL(sm.getBijection().size(), 4)
		ABORT_IF(sm.getBijection().size() != 4)

		const AtomBijection& b = sm.getBijection();
		TEST_EQUAL(b[0].first->getFullName(), b[0].second->getFullName())
		TEST_NOT_EQUAL(b[0].first, b[0].second)
		TEST_EQUAL(b[1].first->getFullName(), b[1].second->getFullName())
		TEST_NOT_EQUAL(b[1].first, b[1].second)
		TEST_EQUAL(b[2].first->getFullName(), b[2].second->getFullName())
		TEST_NOT_EQUAL(b[2].first, b[2].second)
		TEST_EQUAL(b[3].first->getFullName(), b[3].second->getFullName())
		TEST_NOT_EQUAL(b[3].first, b[3].second)
	}

	{
		AtomContainer ac1;
		Residue* r1 = new Residue;
		Residue* r2 = new Residue;
		ac1.insert(*r1);
		ac1.insert(*r2);
		r1->setName("RES1");
		r2->setName("RES2");
		PDBAtom* a1 = new PDBAtom;
		PDBAtom* a2 = new PDBAtom;
		PDBAtom* a3 = new PDBAtom;
		PDBAtom* a4 = new PDBAtom;
		a1->setName("A1");
		a2->setName("A2");
		a3->setName("A3");
		a4->setName("A4");
		r1->insert(*a1);
		r1->insert(*a2);
		r2->insert(*a3);
		r2->insert(*a4);

		AtomContainer ac2;
		Residue* r3 = new Residue;
		Residue* r4 = new Residue;
		ac2.insert(*r3);
		ac2.insert(*r4);
		r4->setName("RES1");
		r3->setName("RES2");
		PDBAtom* a5 = new PDBAtom;
		PDBAtom* a6 = new PDBAtom;
		PDBAtom* a7 = new PDBAtom;
		PDBAtom* a8 = new PDBAtom;
		a5->setName("A4");
		a6->setName("A3");
		a7->setName("A2");
		a8->setName("A1");
		r4->insert(*a6);
		r4->insert(*a5);
		r3->insert(*a8);
		r3->insert(*a7);

		StructureMapper sm(ac1, ac2);
		sm.calculateDefaultBijection();
		TEST_EQUAL(sm.getBijection().size(), 4)
		ABORT_IF(sm.getBijection().size() != 4)

		const AtomBijection& b = sm.getBijection();
		TEST_EQUAL(b[0].first->getName(), b[0].second->getName())
		TEST_NOT_EQUAL(b[0].first->getFullName(), b[0].second->getFullName())
		TEST_NOT_EQUAL(b[0].first, b[0].second)
		TEST_EQUAL(b[1].first->getName(), b[1].second->getName())
		TEST_NOT_EQUAL(b[1].first->getFullName(), b[1].second->getFullName())
		TEST_NOT_EQUAL(b[1].first, b[1].second)
		TEST_EQUAL(b[2].first->getName(), b[2].second->getName())
		TEST_NOT_EQUAL(b[2].first->getFullName(), b[2].second->getFullName())
		TEST_NOT_EQUAL(b[2].first, b[2].second)
		TEST_EQUAL(b[3].first->getName(), b[3].second->getName())
		TEST_NOT_EQUAL(b[3].first->getFullName(), b[3].second->getFullName())
		TEST_NOT_EQUAL(b[3].first, b[3].second)
	}

	// test for unnamed atoms -- they should be mapped by order only.
	{
		Molecule m1;
		Atom* a1 = new Atom;
		Atom* a2 = new Atom;
		Atom* a3 = new Atom;
		Atom* a4 = new Atom;
		a1->setName("A1");
		a2->setName("A2");
		a3->setName("A3");
		a4->setName("A4");
		m1.insert(*a1);
		m1.insert(*a2);
		m1.insert(*a3);
		m1.insert(*a4);

		Molecule m2;
		Atom* a5 = new Atom;
		Atom* a6 = new Atom;
		Atom* a7 = new Atom;
		Atom* a8 = new Atom;
		a5->setName("B1");
		a6->setName("B2");
		a7->setName("B3");
		a8->setName("B4");
		m2.insert(*a8);
		m2.insert(*a7);
		m2.insert(*a6);
		m2.insert(*a5);

		StructureMapper sm(m1, m2);
		sm.calculateDefaultBijection();
		TEST_EQUAL(sm.getBijection().size(), 4)
		ABORT_IF(sm.getBijection().size() != 4)

		const AtomBijection& b = sm.getBijection();
		TEST_EQUAL(b[0].first->getName(), "A1")
		TEST_EQUAL(b[0].second->getName(), "B4")
		TEST_EQUAL(b[1].first->getName(), "A2")
		TEST_EQUAL(b[1].second->getName(), "B3")
		TEST_EQUAL(b[2].first->getName(), "A3")
		TEST_EQUAL(b[2].second->getName(), "B2")
		TEST_EQUAL(b[3].first->getName(), "A4")
		TEST_EQUAL(b[3].second->getName(), "B1")
	}

RESULT

CHECK((void set(AtomContainer& A, AtomContainer& B)))
  // ???
RESULT

CHECK(RSMD for two bptis)
	System s1, s2;
	PDBFile pf(BALL_TEST_DATA_PATH(StructureMapper_test.pdb));
	pf >> s1;
	pf >> s2;
	StructureMapper sm(s1, s2);	
	STATUS("Number of atoms in s1: " << s1.countAtoms())
	STATUS("Number of atoms in s2: " << s2.countAtoms())
	TEST_REAL_EQUAL(sm.calculateRMSD(), 0.0)
RESULT
delete frag1;
delete frag2;
delete m;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
