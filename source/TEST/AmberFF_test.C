// $Id: AmberFF_test.C,v 1.5 1999/12/28 18:14:26 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/FORMAT/HINFile.h>
///////////////////////////

START_TEST(AmberFF, "$Id: AmberFF_test.C,v 1.5 1999/12/28 18:14:26 oliver Exp $")

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

CHECK(energy test 1 (AlaGlySer) [AMBER91])
HINFile f("data/AlaGlySer.hin");	
System s;
f >> s;
f.close();
TEST_EQUAL(s.countAtoms(), 31)
Options options;
options[AmberFF::Option::FILENAME] = "Amber/amber91.ini";
options[AmberFF::Option::ASSIGN_CHARGES] = "false";
AmberFF amber91(s, options);
amber91.updateEnergy();
amber91.updateForces();
#undef PRECISION
#define PRECISION 5e-2
TEST_REAL_EQUAL(amber91.getEnergy(), -314.12)
TEST_REAL_EQUAL(amber91.getRMSGradient(), 35.2728)
TEST_REAL_EQUAL(amber91.getStretchEnergy(), 3.00453)
TEST_REAL_EQUAL(amber91.getBendEnergy(), 8.59268)
TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 0.0489528)
TEST_REAL_EQUAL(amber91.getVdWEnergy(), 21.03)
TEST_REAL_EQUAL(amber91.getESEnergy(), -346.797)
// rerun the same test to verify the static parameter files
amber91.options[AmberFF::Option::ASSIGN_TYPENAMES] = "false";
amber91.options[AmberFF::Option::ASSIGN_TYPES] = "false";
amber91.setup(s);
amber91.updateEnergy();
amber91.updateForces();
TEST_REAL_EQUAL(amber91.getEnergy(), -314.12)
TEST_REAL_EQUAL(amber91.getRMSGradient(), 35.2728)
TEST_REAL_EQUAL(amber91.getStretchEnergy(), 3.00453)
TEST_REAL_EQUAL(amber91.getBendEnergy(), 8.59268)
TEST_REAL_EQUAL(amber91.getTorsionEnergy(), 0.0489528)
TEST_REAL_EQUAL(amber91.getVdWEnergy(), 21.03)
TEST_REAL_EQUAL(amber91.getESEnergy(), -346.797)
RESULT

CHECK(energy test 2 (AlaGlySer) [AMBER94])
HINFile f("data/AlaGlySer2.hin");	
System s;
f >> s;
f.close();
TEST_EQUAL(s.countAtoms(), 31)
Options options;
options[AmberFF::Option::FILENAME] = "Amber/amber94.ini";
options[AmberFF::Option::ASSIGN_TYPENAMES] = "true";
options[AmberFF::Option::ASSIGN_CHARGES] = "true";
options[AmberFF::Option::ASSIGN_TYPES] = "true";
options[AmberFF::Option::OVERWRITE_CHARGES] = "true";
options[AmberFF::Option::OVERWRITE_TYPENAMES] = "true";
AmberFF amber94(s, options);
amber94.updateEnergy();
amber94.updateForces();
#undef PRECISION
#define PRECISION 5e-2
TEST_REAL_EQUAL(amber94.getEnergy(), -91.2239)
TEST_REAL_EQUAL(amber94.getRMSGradient(), 41.7585)
TEST_REAL_EQUAL(amber94.getStretchEnergy(), 3.754722)
TEST_REAL_EQUAL(amber94.getBendEnergy(), 9.1094048)
TEST_REAL_EQUAL(amber94.getTorsionEnergy(), 14.3574)
TEST_REAL_EQUAL(amber94.getVdWEnergy(), 45.4436)
TEST_REAL_EQUAL(amber94.getESEnergy(), -163.8814224)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

