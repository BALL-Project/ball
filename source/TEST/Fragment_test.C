// $Id: Fragment_test.C,v 1.6 2000/05/15 10:52:44 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>

START_TEST(Fragment, "$Id: Fragment_test.C,v 1.6 2000/05/15 10:52:44 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)

Fragment*	frag;
CHECK(default constructor)
frag = new Fragment;
TEST_NOT_EQUAL(frag, 0);
RESULT

CHECK(isValid)
TEST_EQUAL(frag->isValid(), true)
RESULT

CHECK(destructor)
delete frag;
frag = new Fragment;
delete frag;
RESULT

CHECK(Fragment(String&))
	Fragment* f1 = new Fragment("hello");
	TEST_NOT_EQUAL(f1, 0)
	if (f1 != 0)
	{
		TEST_EQUAL(f1->getName(), "hello")
		delete f1;
	}
RESULT

CHECK(Fragment(Fragment&, bool))
	Fragment* f1 = new Fragment;
	f1->setName("testname");
	Atom a;
	a.setName("a");
	f1->insert(a);
	Fragment* f2 = new Fragment(*f1, true);
	TEST_NOT_EQUAL(f2, 0)
	if (f2 != 0)
	{
		TEST_EQUAL(f2->getName(), "testname")
		TEST_EQUAL(f2->getAtom(0)->getName(), "a")
		delete f2;
	}
	f2 = new Fragment(*f1, false);
	TEST_NOT_EQUAL(f2, 0)
	if (f2 != 0)
	{
		TEST_EQUAL(f2->getName(), "testname")
		delete f2;
	}
	delete f1;
RESULT

CHECK(operator = (Fragment&))
	Fragment f1("name1");
	Atom a;
	f1.insert(a);
	Fragment f2;
	f2 = f1;
	TEST_EQUAL(f2.getName(), "name1");
	TEST_EQUAL(f2.countAtoms(), 1);
RESULT

CHECK(dump(ostream&, Size))
	Fragment f1;
	Fragment f2;
	f1.setName("f1");
	f2.setName("f2");
	f1.append(f2);
	Atom a1;
	a1.setName("A1");
	f2.append(a1);
	std::ofstream outfile(filename.c_str(), ios::out);
	f1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Fragment_test.txt", true)
RESULT

/*
TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Composite>(), Composite::createDefault);
pm.registerClass(getStreamName<Fragment>(), Fragment::createDefault);
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
String filename;
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	Fragment* f1 = new Fragment("name1");
	Fragment* f2 = new Fragment("name2");
	f1->insert(*f2);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT
*/
/*
CHECK(persistentRead(PersistenceManager&))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Fragment>(*ptr), true)
		Fragment*	f1 = castTo<Fragment>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countFragments(), 1)
		TEST_EQUAL(f1->getFragment(0)->getName(), "name2")
		delete f1;
	} else {
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT
*/


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
