// $Id: BaseFragment_test.C,v 1.6 2000/01/10 15:51:16 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/baseFragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////


START_TEST(BaseFragment, "$Id: BaseFragment_test.C,v 1.6 2000/01/10 15:51:16 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

BaseFragment*	bf;
CHECK(BaseFragment())
bf = new BaseFragment;
TEST_NOT_EQUAL(bf, 0)
RESULT

CHECK(~BaseFragment())
delete bf;
RESULT

CHECK(setName(String&))
bf = new BaseFragment;
bf->setName("testname");
RESULT

CHECK(getName())
TEST_EQUAL(bf->getName(), "testname")
delete bf;
RESULT

CHECK(BaseFragment(BaseFragment&, bool))
BaseFragment* bf1 = new BaseFragment;
bf1->setName("testname");
BaseFragment* bf2 = new BaseFragment(*bf1, true);
TEST_NOT_EQUAL(bf2, 0)
if (bf2 != 0)
{
	TEST_EQUAL(bf2->getName(), "testname")
	delete bf2;
}
bf2 = new BaseFragment(*bf1, false);
TEST_NOT_EQUAL(bf2, 0)
if (bf2 != 0)
{
	TEST_EQUAL(bf2->getName(), "testname")
	delete bf2;
}
delete bf1;
RESULT

CHECK(BaseFragment(String&))
BaseFragment* bf1 = new BaseFragment("hello");
TEST_NOT_EQUAL(bf1, 0)
if (bf1 != 0)
{
	TEST_EQUAL(bf1->getName(), "hello")
	delete bf1;
}
RESULT

CHECK(insert(BaseFragment&))
BaseFragment bf1;
BaseFragment bf2;
bf1.insert(bf2);
RESULT

CHECK(remove(BaseFragment&))
BaseFragment bf1;
BaseFragment bf2;
bf1.insert(bf2);
bf1.remove(bf2);
RESULT

CHECK(countBaseFragments())
BaseFragment bf1;
BaseFragment bf2;
TEST_EQUAL(bf1.countBaseFragments(), 0)
bf1.insert(bf2);
TEST_EQUAL(bf1.countBaseFragments(), 1)
bf1.remove(bf2);
TEST_EQUAL(bf1.countBaseFragments(), 0)
bf1.insert(bf1);
TEST_EQUAL(bf1.countBaseFragments(), 0)
bf1.insert(bf2);
TEST_EQUAL(bf1.countBaseFragments(), 1)
bf2.insert(bf1);
TEST_EQUAL(bf1.countBaseFragments(), 1)
TEST_EQUAL(bf2.countBaseFragments(), 0)
bf1.remove(bf2);
TEST_EQUAL(bf1.countBaseFragments(), 0)
TEST_EQUAL(bf2.countBaseFragments(), 0)
bf2.remove(bf1);
TEST_EQUAL(bf1.countBaseFragments(), 0)
TEST_EQUAL(bf2.countBaseFragments(), 0)
RESULT

CHECK(getBaseFragment(Index))
BaseFragment bf1("bf1");
BaseFragment bf2("bf2");
BaseFragment* bf3;
bf3 = bf1.getBaseFragment(0);
TEST_EQUAL(bf3, 0)
bf1.insert(bf2);
bf3 = bf1.getBaseFragment(0);
TEST_NOT_EQUAL(bf3, 0)
if (bf3 != 0)
{
	TEST_EQUAL(bf3->getName(), "bf2")
}
bf3 = bf1.getBaseFragment(1);
TEST_EQUAL(bf3, 0)
TEST_EXCEPTION(BALL::Exception::IndexUnderflow, bf3 = bf1.getBaseFragment(-1))
RESULT

CHECK(getBaseFragment(Index) const)
BaseFragment bf1("bf1");
BaseFragment bf2("bf2");
const BaseFragment* bf3;
bf3 = bf1.getBaseFragment(0);
TEST_EQUAL(bf3, 0)
bf1.insert(bf2);
bf3 = bf1.getBaseFragment(0);
TEST_NOT_EQUAL(bf3, 0)
if (bf3 != 0)
{
	TEST_EQUAL(bf3->getName(), "bf2")
}
bf3 = bf1.getBaseFragment(1);
TEST_EQUAL(bf3, 0)
TEST_EXCEPTION(BALL::Exception::IndexUnderflow, bf3 = bf1.getBaseFragment(-1))
RESULT

CHECK(clear())
BaseFragment bf1("bf1");
BaseFragment bf2("bf2");
BaseFragment bf3("bf2");
bf1.insert(bf2);
bf2.insert(bf3);
bf2.clear();
TEST_EQUAL(bf1.getName(), "bf1")
TEST_EQUAL(bf2.getName(), "")
TEST_EQUAL(bf3.getName(), "")
TEST_EQUAL(bf1.countBaseFragments(), 1)
TEST_EQUAL(bf2.countBaseFragments(), 0)
RESULT

CHECK(destroy())
BaseFragment bf1("bf1");
BaseFragment bf2("bf2");
BaseFragment bf3("bf2");
bf1.insert(bf2);
bf2.insert(bf3);
bf2.destroy();
TEST_EQUAL(bf1.getName(), "bf1")
TEST_EQUAL(bf2.getName(), "")
TEST_EQUAL(bf3.getName(), "")
TEST_EQUAL(bf1.countBaseFragments(), 0)
TEST_EQUAL(bf2.countBaseFragments(), 0)
RESULT

CHECK(clone(bool))
BaseFragment* bf1 = new BaseFragment;
Composite*	composite = (Composite*)bf1;
bf1->setName("hello");

Atom a1, a2, a3, a4;
a1.createBond(a2);
a1.createBond(a3);
a1.createBond(a4);

bf1->insert(a1);
bf1->insert(a2);
bf1->insert(a3);
bf1->insert(a4);

TEST_EQUAL(bf1->countBonds(), 3)
TEST_EQUAL(bf1->countAtoms(), 4)

BaseFragment* bf2 = (BaseFragment*)composite->create(true);
TEST_NOT_EQUAL(bf2, 0)
if (bf2 != 0)
{
	TEST_EQUAL(bf2->getName(), "hello")
	TEST_EQUAL(bf2->countBonds(), 3)
	TEST_EQUAL(bf2->countAtoms(), 4)
	delete bf2;
}
bf2 = (BaseFragment*)composite->create(false);
TEST_NOT_EQUAL(bf2, 0)
if (bf2 != 0)
{
	TEST_EQUAL(bf2->getName(), "hello")
	TEST_EQUAL(bf2->countBonds(), 0)
	TEST_EQUAL(bf2->countAtoms(), 0)
	delete bf2;
}
delete bf1;
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Composite>(), getNew<Composite>);
pm.registerClass(getStreamName<BaseFragment>(), getNew<BaseFragment>);
pm.registerClass(getStreamName<Atom>(), getNew<Atom>);
String filename;
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
std::ofstream	ofile(filename.c_str(), std::ios::out);
BaseFragment* bf1 = new BaseFragment("name1");
BaseFragment* bf2 = new BaseFragment("name2");
bf1->insert(*bf2);
pm.setOstream(ofile);
*bf1 >> pm;
ofile.close();
delete bf1;
RESULT

CHECK(persistentRead(PersistenceManager&))
std::ifstream	ifile(filename.c_str());
pm.setIstream(ifile);
PersistentObject*	ptr = pm.readObject();
TEST_NOT_EQUAL(ptr, 0)
if (ptr != 0)
{
	TEST_EQUAL(isKindOf<BaseFragment>(*ptr), true)
	BaseFragment*	bf1 = castTo<BaseFragment>(*ptr);
	TEST_EQUAL(bf1->getName(), "name1")
	TEST_EQUAL(bf1->countBaseFragments(), 1)
	TEST_EQUAL(bf1->getBaseFragment(0)->getName(), "name2")
	delete bf1;
} else {
	throw Exception::NullPointer(__FILE__, __LINE__);
}
RESULT

CHECK(set(BaseFragment&, bool))
BaseFragment bf1("name1");
BaseFragment bf2;
bf1.insert(bf2);
BaseFragment bf3;
bf3.set(bf1, false);
TEST_EQUAL(bf3.getName(), "name1");
TEST_EQUAL(bf3.countBaseFragments(), 0);
bf1.setName("name2");
bf3.set(bf1, true);
TEST_EQUAL(bf3.getName(), "name2");
TEST_EQUAL(bf3.countBaseFragments(), 1);
RESULT

CHECK(operator = (BaseFragment&))
BaseFragment bf1("name1");
BaseFragment bf2;
bf1.insert(bf2);
BaseFragment bf3;
bf3.set(bf1, true);
TEST_EQUAL(bf3.getName(), "name1");
TEST_EQUAL(bf3.countBaseFragments(), 1);
RESULT

CHECK(get(BaseFragment&, bool))
BaseFragment bf1("name1");
BaseFragment bf2;
bf1.insert(bf2);
BaseFragment bf3;
bf1.get(bf3, false);
TEST_EQUAL(bf3.getName(), "name1");
TEST_EQUAL(bf3.countBaseFragments(), 0);
bf1.setName("name2");
bf1.get(bf3, true);
TEST_EQUAL(bf3.getName(), "name2");
TEST_EQUAL(bf3.countBaseFragments(), 1);
RESULT

CHECK(swap(BaseFragment&))
BaseFragment bf1("bf1");
BaseFragment bf2("bf2");
BaseFragment bf3("bf3");
BaseFragment bf4("bf4");
bf1.insert(bf2);
bf3.insert(bf4);
TEST_EQUAL(bf1.getName(), "bf1")
TEST_EQUAL(bf1.getBaseFragment(0), &bf2)
TEST_EQUAL(bf3.getName(), "bf3")
TEST_EQUAL(bf3.getBaseFragment(0), &bf4)
bf1.swap(bf3);
TEST_EQUAL(bf1.getName(), "bf3")
TEST_EQUAL(bf1.getBaseFragment(0), &bf4)
TEST_EQUAL(bf3.getName(), "bf1")
TEST_EQUAL(bf3.getBaseFragment(0), &bf2)
bf1.swap(bf3);
TEST_EQUAL(bf1.getName(), "bf1")
TEST_EQUAL(bf1.getBaseFragment(0), &bf2)
TEST_EQUAL(bf3.getName(), "bf3")
TEST_EQUAL(bf3.getBaseFragment(0), &bf4)
RESULT

CHECK(getMolecule())
BaseFragment bf1("bf1");
Molecule* ptr;
ptr = bf1.getMolecule();
TEST_EQUAL(ptr, 0)
Molecule m;
m.insert(bf1);
ptr = bf1.getMolecule();
TEST_EQUAL(ptr, &m)
m.remove(bf1);
BaseFragment bf2;
bf2.insert(bf1);
m.insert(bf2);
ptr = bf1.getMolecule();
TEST_EQUAL(ptr, &m)
ptr = bf2.getMolecule();
TEST_EQUAL(ptr, &m)
RESULT

CHECK(getMolecule() const)
BaseFragment bf1("bf1");
TEST_EQUAL(bf1.getMolecule(), 0)
Molecule m;
m.insert(bf1);
TEST_EQUAL(bf1.getMolecule(), &m)
m.remove(bf1);
BaseFragment bf2;
bf2.insert(bf1);
m.insert(bf2);
TEST_EQUAL(bf2.getMolecule(), &m)
TEST_EQUAL(bf1.getMolecule(), &m)
RESULT

CHECK(getSuperBaseFragment())
BaseFragment bf1("bf1");
BaseFragment* ptr = bf1.getSuperBaseFragment();
TEST_EQUAL(ptr, 0)
BaseFragment bf2;
BaseFragment bf3;
bf2.insert(bf3);
bf1.insert(bf2);
ptr = bf3.getSuperBaseFragment();
TEST_EQUAL(ptr, &bf2)
ptr = bf2.getSuperBaseFragment();
TEST_EQUAL(ptr, &bf1)
RESULT

CHECK(getSuperBaseFragment() const)
BaseFragment bf1("bf1");
const BaseFragment* ptr = bf1.getSuperBaseFragment();
TEST_EQUAL(ptr, 0)
BaseFragment bf2;
BaseFragment bf3;
bf2.insert(bf3);
bf1.insert(bf2);
ptr = bf3.getSuperBaseFragment();
TEST_EQUAL(ptr, &bf2)
ptr = bf2.getSuperBaseFragment();
TEST_EQUAL(ptr, &bf1)
RESULT

CHECK(insert(Atom&))
BaseFragment bf1;
Atom a;
bf1.insert(a);
RESULT

CHECK(remove(Atom&))
BaseFragment bf1;
Atom a;
bf1.insert(a);
bf1.remove(a);
RESULT

CHECK(getAtom(Index))
BaseFragment bf1;
Atom a;
Atom* ptr = bf1.getAtom(0);
TEST_EQUAL(ptr, 0)
bf1.insert(a);
ptr = bf1.getAtom(0);
TEST_EQUAL(ptr, &a)
bf1.remove(a);
ptr = bf1.getAtom(0);
TEST_EQUAL(ptr, 0)
ptr = bf1.getAtom(25);
TEST_EQUAL(ptr, 0)
TEST_EXCEPTION(Exception::IndexUnderflow, ptr = bf1.getAtom(-1))
RESULT

CHECK(getAtom(Index) const)
BaseFragment bf1;
Atom a;
TEST_EQUAL(bf1.getAtom(0), 0)
bf1.insert(a);
TEST_EQUAL(bf1.getAtom(0), &a)
bf1.remove(a);
TEST_EQUAL(bf1.getAtom(0), 0)
TEST_EQUAL(bf1.getAtom(24), 0)
TEST_EXCEPTION(Exception::IndexUnderflow, bf1.getAtom(-1))
RESULT

CHECK(countAtoms())
BaseFragment bf1;
BaseFragment bf2;
bf1.insert(bf2);
Atom a1;
Atom a2;
Atom a3;
TEST_EQUAL(bf1.countAtoms(), 0)
TEST_EQUAL(bf2.countAtoms(), 0)
bf2.insert(a1);
TEST_EQUAL(bf1.countAtoms(), 1)
TEST_EQUAL(bf2.countAtoms(), 1)
bf1.insert(a2);
TEST_EQUAL(bf1.countAtoms(), 2)
TEST_EQUAL(bf2.countAtoms(), 1)
bf2.insert(a3);
TEST_EQUAL(bf1.countAtoms(), 3)
TEST_EQUAL(bf2.countAtoms(), 2)
bf1.remove(bf2);
TEST_EQUAL(bf1.countAtoms(), 1)
TEST_EQUAL(bf2.countAtoms(), 2)
RESULT

CHECK(countBonds())
BaseFragment bf1;
BaseFragment bf2;
Atom a1;
Atom a2;
Atom a3;
Atom a4;
bf1.insert(a1);
bf1.insert(a2);
bf2.insert(a3);
bf2.insert(a4);
TEST_EQUAL(bf1.countBonds(), 0);
TEST_EQUAL(bf2.countBonds(), 0);
a1.createBond(a2);
a2.createBond(a3);
a3.createBond(a4);
a1.createBond(a4);
TEST_EQUAL(bf1.countBonds(), 3);
TEST_EQUAL(bf2.countBonds(), 3);
RESULT

CHECK(countInterBonds())
BaseFragment bf1;
BaseFragment bf2;
Atom a1;
Atom a2;
Atom a3;
Atom a4;
bf1.insert(a1);
bf1.insert(a2);
bf2.insert(a3);
bf2.insert(a4);
TEST_EQUAL(bf1.countInterBonds(), 0);
TEST_EQUAL(bf2.countInterBonds(), 0);
a1.createBond(a2);
a2.createBond(a3);
a3.createBond(a4);
TEST_EQUAL(bf1.countInterBonds(), 1);
TEST_EQUAL(bf2.countInterBonds(), 1);
RESULT

CHECK(countIntraBonds())
BaseFragment bf1;
BaseFragment bf2;
Atom a1;
Atom a2;
Atom a3;
Atom a4;
bf1.insert(a1);
bf1.insert(a2);
bf2.insert(a3);
bf2.insert(a4);
TEST_EQUAL(bf1.countIntraBonds(), 0);
TEST_EQUAL(bf2.countIntraBonds(), 0);
a1.createBond(a2);
a2.createBond(a3);
a3.createBond(a4);
TEST_EQUAL(bf1.countIntraBonds(), 1);
TEST_EQUAL(bf2.countIntraBonds(), 1);
RESULT

CHECK(prepend(Atom&))
//BAUSTELLE
RESULT

CHECK(append(Atom&))
//BAUSTELLE
RESULT

CHECK(insertBefore(Atom&, Composite&))
//BAUSTELLE
RESULT

CHECK(insertAfter(Atom&, Composite&))
//BAUSTELLE
RESULT

CHECK(prepend(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(append(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(insertBefore(BaseFragment&, Composite&))
//BAUSTELLE
RESULT

CHECK(insertAfter(BaseFragment&, Composite&))
//BAUSTELLE
RESULT

CHECK(spliceBefore(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(spliceAfter(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(splice(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(destroyBonds())
//BAUSTELLE
RESULT

CHECK(isSubBaseFragmentOf(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(isSuperBaseFragmentOf(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(isValid())
//BAUSTELLE
RESULT

CHECK(dump(ostream&, Size))
//BAUSTELLE
RESULT

CHECK(read(istream&))
//BAUSTELLE
RESULT

CHECK(write(ostream&))
//BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
