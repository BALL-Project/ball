// $Id: FragmentDB_test.C,v 1.3 1999/10/30 12:53:39 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/residue.h>
using namespace BALL;
///////////////////////////

START_TEST(class_name, "$Id: FragmentDB_test.C,v 1.3 1999/10/30 12:53:39 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

FragmentDB db("data/fragments/Fragments.db");
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
