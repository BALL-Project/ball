// $Id: AmberFF_test.C,v 1.2 1999/09/05 09:04:26 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/FORMAT/HINFile.h>
///////////////////////////

START_TEST(AmberFF, "$Id: AmberFF_test.C,v 1.2 1999/09/05 09:04:26 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Log.insert(cerr);

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

CHECK(energy test 1 (AlaGlySer))
HINFile f("data/AlaGlySer.hin");	
System s;
f >> s;
f.close();
TEST_EQUAL(s.countAtoms(), 31)
AmberFF amber89a(s);
amber89a.updateEnergy();
amber89a.updateForces();
#undef PRECISION
#define PRECISION 5e-2
TEST_REAL_EQUAL(amber89a.getEnergy(), -314.12)
TEST_REAL_EQUAL(amber89a.getRMSGradient(), 32.2732)
TEST_REAL_EQUAL(amber89a.getStretchEnergy(), 3.00453)
TEST_REAL_EQUAL(amber89a.getBendEnergy(), 8.59268)
TEST_REAL_EQUAL(amber89a.getTorsionEnergy(), 0.0489528)
TEST_REAL_EQUAL(amber89a.getVdWEnergy(), 21.03)
TEST_REAL_EQUAL(amber89a.getESEnergy(), -346.797)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

