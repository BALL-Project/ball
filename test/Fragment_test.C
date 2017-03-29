// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(Fragment)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)

Fragment*	frag;
CHECK(Fragment() throw())
frag = new Fragment;
TEST_NOT_EQUAL(frag, 0);
RESULT

CHECK([EXTRA] isValid)
TEST_EQUAL(frag->isValid(), true)
RESULT

CHECK(~Fragment() throw())
delete frag;
frag = new Fragment;
delete frag;
RESULT

CHECK(Fragment(const String& name) throw())
	Fragment* f1 = new Fragment("hello");
	TEST_NOT_EQUAL(f1, 0)
	if (f1 != 0)
	{
		TEST_EQUAL(f1->getName(), "hello")
		delete f1;
	}
RESULT

CHECK(Fragment(const Fragment& fragment, bool deep = true) throw())
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

CHECK(Fragment& operator = (const Fragment& fragment) throw())
	Fragment f1("name1");
	Atom a;
	f1.insert(a);
	Fragment f2;
	f2 = f1;
	TEST_EQUAL(f2.getName(), "name1");
	TEST_EQUAL(f2.countAtoms(), 1);
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Fragment f1;
	Fragment f2;
	f1.setName("f1");
	f2.setName("f2");
	f1.append(f2);
	Atom a1;
	a1.setName("A1");
	f2.append(a1);
	std::ofstream outfile(filename.c_str(), std::ios::out);
	f1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Fragment_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Fragment>(), Fragment::createDefault);
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	Fragment* f1 = new Fragment("name1");
	Atom* f2 = new Atom();
	Atom* f3 = new Atom();
	f2->setName("name2");
	f3->setName("name3");
	f1->insert(*f2);
	f1->insert(*f3);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
        TEST_EQUAL(isKindOf<Fragment>(ptr), true)
		Fragment*	f1 = castTo<Fragment>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countAtoms(), 2)
		TEST_EQUAL(f1->getAtom(0)->getName(), "name2")
		TEST_EQUAL(f1->getAtom(1)->getName(), "name3")
		delete f1;
	} else {
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(bool operator == (const Fragment& fragment) const throw())
	Fragment b1;
	Fragment b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const Fragment& fragment) const throw())
	Fragment b1;
	Fragment b2;
	TEST_EQUAL(b1 != b2, true)
	b1 = b2;
	TEST_EQUAL(b1 != b1, false)
RESULT

CHECK(BALL_CREATE_DEEP(Fragment))
	Fragment b1;
	b1.setName("asddd");
	Atom a;
	b1.insert(a);

	Fragment* b2 = (Fragment*)b1.create(false, true);
	TEST_EQUAL(b2->getName(), "")
	TEST_EQUAL(b2->countAtoms(), 0)
	delete b2;

	b2 = (Fragment*)b1.create(true, false);
	TEST_EQUAL(b2->getName(), "asddd")
	TEST_EQUAL(b2->countAtoms(), 1)
	delete b2;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
