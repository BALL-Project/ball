// $Id: FragmentDB_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/fragmentDB.h>
#include <BALL/KERNEL/residue.h>
using namespace BALL;
///////////////////////////

START_TEST(class_name, "$Id: FragmentDB_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

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
	AtomIterator it;
	for (it = res->beginAtom(); +it; ++it)
	{
		cout << it->getName() << endl;
	}
}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
