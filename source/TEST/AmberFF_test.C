// $Id: AmberFF_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/AMBER/amber.h>
///////////////////////////

START_TEST(AmberFF, "$Id: AmberFF_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

AmberFF* amber;
CHECK(AmberFF())
amber = new AmberFF();
TEST_NOT_EQUAL(amber, 0)
RESULT

CHECK(~AmberFF())
delete amber;
RESULT

CHECK(AmberFF(const AmberFF& force_field, bool clone_deep = true))
AmberFF a1;
AmberFF a2(a1);
RESULT
	
CHECK(specificSetup())
AmberFF a;
a.specificSetup();
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

