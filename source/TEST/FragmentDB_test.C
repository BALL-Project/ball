// $Id: FragmentDB_test.C,v 1.4 2000/01/18 16:48:31 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/residue.h>
using namespace BALL;
///////////////////////////

START_TEST(class_name, "$Id: FragmentDB_test.C,v 1.4 2000/01/18 16:48:31 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

FragmentDB db;
CHECK(getResidue(const String&))
const Residue* res = db.getResidue("GLY");
TEST_NOT_EQUAL(res, 0)
if (res != 0)
{
	TEST_EQUAL(res->isNTerminal(), false)
	TEST_EQUAL(res->isCTerminal(), false)
}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
