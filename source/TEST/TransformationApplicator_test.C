// $Id: TransformationApplicator_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/KERNEL/fragment.h>

START_TEST(TransformationApplictor, "$Id: TransformationApplicator_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using BALL::TransformationProcessor;
TransformationProcessor*	app = new TransformationProcessor;
CHECK(default constructor)
TEST_NOT_EQUAL(app, 0)
RESULT

using BALL::Matrix4x4;
Matrix4x4	t(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
CHECK(getTransformation/setTransformation)
TEST_EQUAL(app->getTransformation(), t)
t.set(1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16);
app->setTransformation(t);
TEST_EQUAL(app->getTransformation(), t)
RESULT

CHECK(destructor/constructor)
delete app;
app = new TransformationProcessor(t);
TEST_NOT_EQUAL(app, 0)
TEST_EQUAL(app->getTransformation(), t)
RESULT

CHECK(operator ())
using BALL::Fragment;
using BALL::Atom;
Fragment* frag = new Fragment;
Atom* atom1 = new Atom;
Atom* atom2 = new Atom;
using BALL::Vector3;
Vector3	v(1,2,3);
atom1->setPosition(v);
v.set(0,0,0);
atom2->setPosition(v);
frag->insert(*atom1);
frag->insert(*atom2);

t.set(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,0);
app->setTransformation(t);
frag->apply(*app);
TEST_EQUAL(atom2->getPosition(), Vector3(0,0,0))
TEST_EQUAL(atom1->getPosition(), Vector3(1,2,3))
t.set(-1,0,0,2, 0,-1,0,2, 0,0,-1,2, 0,0,0,0);
app->setTransformation(t);
frag->apply(*app);
TEST_EQUAL(atom2->getPosition(), Vector3(2,2,2))
TEST_EQUAL(atom1->getPosition(), Vector3(1,0,-1))
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
