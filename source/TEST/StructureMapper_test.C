// $Id: StructureMapper_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/MATHS/quaternion.h>
#include <vector>

START_TEST(class_name, "$Id: StructureMapper_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using std::vector;

StructureMapper*	m;
CHECK(Default constructor)
m = new StructureMapper();
TEST_NOT_EQUAL(m, 0)
RESULT

CHECK(Destructor)
delete m;
RESULT

Fragment*	frag1 = new Fragment;
Fragment*	frag2 = new Fragment;
Matrix4x4	t;
CHECK(Constructor)
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

CHECK(mapFragments)
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
