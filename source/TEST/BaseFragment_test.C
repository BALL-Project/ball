// $Id: BaseFragment_test.C,v 1.17 2000/05/31 01:01:46 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/baseFragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(BaseFragment, "$Id: BaseFragment_test.C,v 1.17 2000/05/31 01:01:46 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)


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
	Atom a;
	a.setName("a");
	bf1->insert(a);
	BaseFragment* bf2 = new BaseFragment(*bf1, true);
	TEST_NOT_EQUAL(bf2, 0)
	if (bf2 != 0)
	{
		TEST_EQUAL(bf2->getName(), "testname")
		TEST_EQUAL(bf2->getAtom(0)->getName(), "a")
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

CHECK(getBaseFragment(Position))
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
	TEST_EQUAL(bf1.getBaseFragment(-1), 0)
RESULT

CHECK(getBaseFragment(Position) const)
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
pm.registerClass(getStreamName<BaseFragment>(), BaseFragment::createDefault);
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	BaseFragment* f1 = new BaseFragment("name1");
	BaseFragment* f2 = new BaseFragment("name2");
	f1->insert(*f2);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT

CHECK(persistentRead(PersistenceManager&))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<BaseFragment>(*ptr), true)
		BaseFragment*	f1 = castTo<BaseFragment>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countBaseFragments(), 1)
		TEST_EQUAL(f1->getBaseFragment(0)->getName(), "name2")
		delete f1;
	} 
	else 
	{
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
	Atom a;
	bf1.insert(a);
	BaseFragment bf2;
	bf2 = bf1;
	TEST_EQUAL(bf2.getName(), "name1");
	TEST_EQUAL(bf2.countAtoms(), 1);
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

CHECK(getAtom(Position))
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
	TEST_EQUAL(bf1.getAtom(-1), 0)
RESULT

CHECK(getAtom(Position) const)
	BaseFragment bf1;
	Atom a;
	TEST_EQUAL(bf1.getAtom(0), 0)
	bf1.insert(a);
	TEST_EQUAL(bf1.getAtom(0), &a)
	bf1.remove(a);
	TEST_EQUAL(bf1.getAtom(0), 0)
	TEST_EQUAL(bf1.getAtom(24), 0)
	TEST_EQUAL(bf1.getAtom(-1), 0)
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
	BaseFragment bf1;
	Atom a1;
	Atom a2;
	bf1.insert(a1);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	bf1.prepend(a2);
	TEST_EQUAL(bf1.getAtom(0), &a2)
	TEST_EQUAL(bf1.getAtom(1), &a1)
RESULT

CHECK(insert(Atom&))
	BaseFragment bf1;
	Atom a1;
	Atom a2;
	bf1.insert(a1);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	bf1.insert(a2);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a2)
RESULT

CHECK(append(Atom&))
	BaseFragment bf1;
	Atom a1;
	Atom a2;
	bf1.insert(a1);
	bf1.append(a2);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a2)
RESULT

CHECK(insertBefore(Atom&, Composite&))
	BaseFragment bf1;
	Atom a1;
	Atom a2;
	Atom a3;
	bf1.insert(a1);
	bf1.append(a2);
	bf1.insertBefore(a3, a2);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a3)
	TEST_EQUAL(bf1.getAtom(2), &a2)
RESULT

CHECK(insertAfter(Atom&, Composite&))
	BaseFragment bf1;
	Atom a1;
	Atom a2;
	Atom a3;
	bf1.insert(a1);
	bf1.append(a2);
	bf1.insertAfter(a3, a1);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a3)
	TEST_EQUAL(bf1.getAtom(2), &a2)
RESULT

CHECK(prepend(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	bf1.prepend(bf2);
	bf1.prepend(bf3);
	TEST_EQUAL(bf1.getBaseFragment(1), &bf2)
	TEST_EQUAL(bf1.getBaseFragment(0), &bf3)
RESULT

CHECK(append(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	bf1.append(bf2);
	bf1.append(bf3);
	TEST_EQUAL(bf1.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf1.getBaseFragment(1), &bf3)
RESULT

CHECK(insertBefore(BaseFragment&, Composite&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	bf1.append(bf2);
	bf1.append(bf3);
	bf1.insertBefore(bf4, bf3);
	TEST_EQUAL(bf1.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf1.getBaseFragment(1), &bf4)
	TEST_EQUAL(bf1.getBaseFragment(2), &bf3)
RESULT

CHECK(insertAfter(BaseFragment&, Composite&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	bf1.append(bf2);
	bf1.append(bf3);
	bf1.insertAfter(bf4, bf2);
	TEST_EQUAL(bf1.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf1.getBaseFragment(1), &bf4)
	TEST_EQUAL(bf1.getBaseFragment(2), &bf3)
RESULT

CHECK(spliceBefore(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	bf1.append(bf2);
	bf4.append(bf3);
	bf4.spliceBefore(bf1);
	TEST_EQUAL(bf1.getBaseFragment(0), 0)
	TEST_EQUAL(bf4.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(1), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(1), &bf3)
RESULT

CHECK(spliceAfter(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	bf1.append(bf2);
	bf4.append(bf3);
	bf4.spliceAfter(bf1);
	TEST_EQUAL(bf1.getBaseFragment(0), 0)
	TEST_EQUAL(bf4.getBaseFragment(1), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(0), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getBaseFragment(1), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(0), &bf3)
RESULT

CHECK(splice(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	BaseFragment bf5;
	BaseFragment bfx;
	BaseFragment bfy;
	bf1.append(bf2);
	bf1.append(bfx);
	bf1.append(bf3);
	bfx.append(bfy);
	bf4.append(bf5);
	bf1.splice(bfx);
	TEST_EQUAL(bf1.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf1.getBaseFragment(1), &bfy)
	TEST_EQUAL(bf1.getBaseFragment(2), &bf3)
	bf1.splice(bf4);
	TEST_EQUAL(bf1.getBaseFragment(0), &bf5)
	TEST_EQUAL(bf1.getBaseFragment(1), &bf2)
RESULT

CHECK(destroyBonds())
	BaseFragment bf1;
	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a4);
	bf1.append(a1);
	bf1.append(a2);
	bf1.destroyBonds();
	TEST_EQUAL(a1.countBonds(), 0)	
	TEST_EQUAL(a2.countBonds(), 0)	
	TEST_EQUAL(a3.countBonds(), 1)	
RESULT

CHECK(isSubBaseFragmentOf(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	bf1.append(bf2);
	bf2.append(bf3);
	TEST_EQUAL(bf1.isSubBaseFragmentOf(bf2), false)	
	TEST_EQUAL(bf2.isSubBaseFragmentOf(bf2), false)	
	TEST_EQUAL(bf2.isSubBaseFragmentOf(bf1), true)	
	TEST_EQUAL(bf3.isSubBaseFragmentOf(bf1), true)	
RESULT

CHECK(isSuperBaseFragmentOf(BaseFragment&))
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	bf1.append(bf2);
	bf2.append(bf3);
	TEST_EQUAL(bf1.isSuperBaseFragmentOf(bf2), true)	
	TEST_EQUAL(bf1.isSuperBaseFragmentOf(bf3), true)	
	TEST_EQUAL(bf2.isSuperBaseFragmentOf(bf2), false)	
	TEST_EQUAL(bf2.isSuperBaseFragmentOf(bf1), false)	
	TEST_EQUAL(bf3.isSuperBaseFragmentOf(bf1), false)	
RESULT

CHECK(isValid())
	BaseFragment bf1;
	BaseFragment bf2;
	BaseFragment bf3;
	bf1.append(bf2);
	bf2.append(bf3);
	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a3);
	bf1.append(a1);
	bf2.append(a2);
	bf3.append(a3);
	TEST_EQUAL(bf1.isValid(), true)	
	TEST_EQUAL(bf2.isValid(), true)	
	TEST_EQUAL(bf3.isValid(), true)	
RESULT

CHECK(dump(ostream&, Size))
	BaseFragment bf1;
	BaseFragment bf2;
	bf1.setName("BF1");
	bf2.setName("BF2");
	bf1.append(bf2);
	Atom a1;
	a1.setName("A1");
	bf2.append(a1);
	std::ofstream outfile(filename.c_str(), ios::out);
	bf1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Base_Fragment.txt", true)
RESULT

CHECK(read(istream&)) 
// NotImplemented
RESULT

CHECK(write(ostream&))
// NotImplemented
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
