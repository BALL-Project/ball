// $Id: PrimitiveManager_test.C,v 1.1.2.1 2002/11/23 20:06:27 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/VIEW/GUI/KERNEL/primitiveManager.h>
///////////////////////////

/////////////////////////////////////////////////////////////
START_TEST(PrimitiveManager, "$Id: PrimitiveManager_test.v")
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::VIEW;
//using namespace std;

Composite c,d,e,f;
GeometricObject g,h,i,j;

CHECK(PrimitiveManager())
	PrimitiveManager* pm = new PrimitiveManager;
	TEST_NOT_EQUAL(pm, 0)
RESULT

PrimitiveManager pm;

CHECK(insertGeometricObject(..))
	pm.insertGeometricObject(&g, &c);
	pm.insertGeometricObject(&h, &c);
	pm.insertGeometricObject(&i, &d);
	pm.insertGeometricObject(&i, 0);
	TEST_EQUAL(pm.hasComposite(&c), true)
	TEST_EQUAL(pm.hasComposite(&d), true)
	TEST_EQUAL(pm.hasComposite(0), false)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

