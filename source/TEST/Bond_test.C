// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/COMMON/exception.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(Bond)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
	
Bond*	b;

CHECK(Bond() throw())
	b = new Bond;
	TEST_NOT_EQUAL(b, 0)
RESULT											

CHECK(~Bond() throw())
	delete b;
RESULT

CHECK(Bond(const Bond& bond, bool deep = true) throw())
	Atom a1;
	Atom a2;
	Bond b1, b2;
	b1.setFirstAtom(&a1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	b1.setFirstAtom(&a2);
	TEST_EQUAL(b1.getFirstAtom(), &a2);
RESULT		

CHECK(Bond(const String& name, Atom& first, Atom& second, Order order = BALL_BOND_DEFAULT_ORDER, Type type = BALL_BOND_DEFAULT_TYPE) throw(TooManyBonds))
	Atom a1;
	Atom a2;
	Bond b1("name", a1, a2, 1, 2);
	TEST_EQUAL(b1.getName(), "name")
	TEST_EQUAL(b1.getFirstAtom(), &a1)
	TEST_EQUAL(b1.getSecondAtom(), &a2)
	TEST_EQUAL(b1.getOrder(), 1)
	TEST_EQUAL(b1.getType(), 2)
RESULT

CHECK(static Bond* createBond(Bond& bond, Atom& first, Atom& second) throw(TooManyBonds))
	Atom a1;
	Atom a2;
	Atom a3;
	Bond b1;
	Bond b2;

	TEST_NOT_EQUAL(b1.createBond(b1, a2, a1), 0)
	TEST_EQUAL(a2.countBonds(), 1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	TEST_EQUAL(b1.getSecondAtom(), &a2);

	TEST_NOT_EQUAL(b1.createBond(b1, a1, a2), 0);
	TEST_EQUAL(a1.countBonds(), 1);
	TEST_EQUAL(a2.countBonds(), 1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	TEST_EQUAL(b1.getSecondAtom(), &a2);

	TEST_EQUAL(b2.createBond(b2, a1, a2), &b1);
	TEST_EQUAL(a1.countBonds(), 1);
	TEST_EQUAL(a2.countBonds(), 1);

	TEST_NOT_EQUAL(b2.createBond(b2, a1, a3), 0);
	TEST_EQUAL(a1.countBonds(), 2);
	TEST_EQUAL(a3.countBonds(), 1);

	Bond bonds[Atom::MAX_NUMBER_OF_BONDS + 10];
	Atom atoms[Atom::MAX_NUMBER_OF_BONDS + 11];
	for (Position i = 1; i < (Atom::MAX_NUMBER_OF_BONDS + 10); ++i)
	{
		STATUS("# bonds: " << i)
		if (i <= Atom::MAX_NUMBER_OF_BONDS)
		{
			TEST_EQUAL(bonds[i].createBond(bonds[i], atoms[0], atoms[i + 1]), &(bonds[i]));
			TEST_EQUAL(atoms[0].countBonds(), i);
		}
		else
		{
			TEST_EXCEPTION(Exception::TooManyBonds, bonds[i].createBond(bonds[i], atoms[0], atoms[i + 1]))
			TEST_EQUAL(atoms[0].countBonds(), Atom::MAX_NUMBER_OF_BONDS);
		}
	}		
	TEST_EQUAL(bonds[0].isAutoDeletable(), false)
	TEST_EQUAL(bonds[1].isAutoDeletable(), false)
	atoms[0].destroyBonds();
RESULT

CHECK(void clear() throw())
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2);
	b1.clear();
	TEST_EQUAL(a1.countBonds(), 0)
	TEST_EQUAL(a2.countBonds(), 0)
	TEST_EQUAL(b1.getFirstAtom(), 0)
	TEST_EQUAL(b1.getSecondAtom(), 0)
	TEST_EQUAL(b1.getName(), "")
	TEST_EQUAL(b1.getOrder(), Bond::ORDER__UNKNOWN)
	TEST_EQUAL(b1.getType(), Bond::TYPE__UNKNOWN)
RESULT

CHECK(void destroy() throw())
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2);
	b1.destroy();
	TEST_EQUAL(a1.countBonds(), 0)
	TEST_EQUAL(a2.countBonds(), 0)
RESULT

CHECK(Bond& operator = (const Bond& bond) throw())
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2), b2;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("abc");
	b2 = b1;
	TEST_EQUAL(b1.getFirstAtom(), b2.getFirstAtom())
	TEST_EQUAL(b1.getSecondAtom(), b2.getSecondAtom())
	TEST_EQUAL(b1.getName(), b2.getName())
	TEST_EQUAL(b1.getOrder(), b2.getOrder())
	TEST_EQUAL(b1.getType(), b2.getType())
	// for safety...
	a1.destroyBonds();
	a2.destroyBonds();
	b1.setFirstAtom(0);
	b1.setSecondAtom(0);
	b2.setFirstAtom(0);
	b2.setSecondAtom(0);
RESULT

CHECK(void swap(Bond& bond) throw())
	Atom a1, a2;
	Atom a3, a4;
	Bond b1("bond1", a1, a2), b2;
	Bond b3("bond2", a3, a4), b4;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("a");
	b3.setType(0);
	b3.setOrder(0);
	b3.setName("b");
	b2 = b1;
	b4 = b3;
	b4.swap(b1);
	TEST_EQUAL(b1.getFirstAtom(), b3.getFirstAtom())
	TEST_EQUAL(b1.getSecondAtom(), b3.getSecondAtom())
	TEST_EQUAL(b1.getName(), b3.getName())
	TEST_EQUAL(b1.getOrder(), b3.getOrder())
	TEST_EQUAL(b1.getType(), b3.getType())

	TEST_EQUAL(b2.getFirstAtom(), b4.getFirstAtom())
	TEST_EQUAL(b2.getSecondAtom(), b4.getSecondAtom())
	TEST_EQUAL(b2.getName(), b4.getName())
	TEST_EQUAL(b2.getOrder(), b4.getOrder())
	TEST_EQUAL(b2.getType(), b4.getType())
	
	// for safety...
	a1.destroyBonds();
	a2.destroyBonds();
	a3.destroyBonds();
	a4.destroyBonds();
	b1.setFirstAtom(0);
	b1.setSecondAtom(0);
	b2.setFirstAtom(0);
	b2.setSecondAtom(0);
	b3.setFirstAtom(0);
	b3.setSecondAtom(0);
	b4.setFirstAtom(0);
	b4.setSecondAtom(0);
RESULT

CHECK(void setFirstAtom(Atom* atom) throw())
	Atom a1;
	Bond b1;
	b1.setFirstAtom(&a1);
RESULT

CHECK(const Atom* getFirstAtom() const throw())
	Atom a1;
	Atom a2;
	Bond b1;
	b1.setFirstAtom(&a1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	b1.setFirstAtom(&a2);
	TEST_EQUAL(b1.getFirstAtom(), &a2);
RESULT

CHECK(void setSecondAtom(Atom* atom) throw())
	Atom a1;
	Bond b1;
	b1.setSecondAtom(&a1);
RESULT

CHECK(const Atom* getSecondAtom() const throw())
	Atom a1;
	Atom a2;
	Bond b1;
	b1.setSecondAtom(&a1);
	TEST_EQUAL(b1.getSecondAtom(), &a1);
	b1.setSecondAtom(&a2);
	TEST_EQUAL(b1.getSecondAtom(), &a2);
RESULT

CHECK(void setName(const String& name) throw())
	Bond b1;
	b1.setName("abc");
	b1.setName("");
RESULT

CHECK(const String& getName() const throw())
	Bond b1;
	b1.setName("abc");
	TEST_EQUAL(b1.getName(), "abc")
	b1.setName("");
	TEST_EQUAL(b1.getName(), "")
RESULT

CHECK(void setOrder(Order bond_order) throw())
	Bond b1;
	b1.setOrder(0);
	b1.setOrder(1);
RESULT

CHECK(Order getOrder() const throw())
	Bond b1;
	b1.setOrder(1);
	TEST_EQUAL(b1.getOrder(), 1)
	b1.setOrder(0);
	TEST_EQUAL(b1.getOrder(), 0)
RESULT

CHECK(void setType(Type bond_type) throw())
	Bond b1;
	b1.setType(0);
	b1.setType(1);
RESULT

CHECK(Type getType() const throw())
	Bond b1;
	b1.setType(1);
	TEST_EQUAL(b1.getType(), 1)
	b1.setType(0);
	TEST_EQUAL(b1.getType(), 0)
RESULT

CHECK(float getLength() const throw(NotBound))
	Atom a1;
	Atom a2;
	a1.setPosition(Vector3(1.0, 1.0, 1.0));
	a2.setPosition(Vector3(2.0, 2.0, 2.0));
	Bond b1("bond", a1, a2);
	TEST_REAL_EQUAL(b1.getLength(), ::sqrt(3.0))
	Bond b2;
	TEST_EXCEPTION(Bond::NotBound, b2.getLength())
	b2.setFirstAtom(&a1);
	TEST_EXCEPTION(Bond::NotBound, b2.getLength())
	b2.setSecondAtom(&a2);
	TEST_REAL_EQUAL(b2.getLength(), ::sqrt(3.0))
RESULT

CHECK(const Atom* getBoundAtom(const Atom& atom) const throw())
	Atom a1, a2;
	a2.setCharge(0.1);
	Bond b1("bond", a1, a2);
	const Atom* a3 = b1.getBoundAtom(a1);
	TEST_REAL_EQUAL(a3->getCharge(), 0.1)
RESULT

CHECK(bool isBondOf(const Atom& atom) const throw())
	Atom a1, a2, a3;
	Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.isBondOf(a1), true)
	TEST_EQUAL(b1.isBondOf(a2), true)
	TEST_EQUAL(b1.isBondOf(a3), false)
RESULT

CHECK(bool isBound() const throw())
	Atom a1, a2, a3;
	Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.isBound(), true)
	TEST_EQUAL(b1.isBondOf(a3), false)
RESULT

CHECK(bool isInterBond() const throw())
	Fragment f("F1");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isInterBond(), false)
	
	Bond b2("bond2", a3, a4);

	TEST_EQUAL(b2.isInterBond(), false)
	
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isInterBond(), true)

	Molecule m("m1");
	System s("S1");
	Atom a5, a6, a7, a8;
	m.append(a5);
	m.append(a6);
	Bond b4("bond2", a5, a6);
	TEST_EQUAL(b4.isInterBond(), false)
RESULT

CHECK(bool isInterBondOf(const AtomContainer& atom_container) const throw())
	Fragment f("F1");
	Fragment f2("F2");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	f2.append(a3);
	f2.append(a4);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isInterBondOf(f), false)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isInterBondOf(f), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isInterBondOf(f), true)
RESULT

CHECK(bool isIntraBond() const throw())
	Fragment f("F1");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isIntraBond(), true)
	
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isIntraBond(), false)
	
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isIntraBond(), false)
RESULT

CHECK(bool isIntraBondOf(const AtomContainer& atom_container) const throw())
	Fragment f("F1");
	Fragment f2("F2");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	f2.append(a3);
	f2.append(a4);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isIntraBondOf(f), true)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isIntraBondOf(f), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isIntraBondOf(f), false)
RESULT

CHECK(bool isValid() const throw())
	Atom a1, a2;
	Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Atom a1;
	Atom a2;
	a1.setName("a1");
	a2.setName("a2");
	Bond b1("bond", a1, a2);
	b1.setType(1);
	b1.setOrder(1);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	b1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Bond_test.txt))
RESULT

CHECK(void finalize() throw(Exception::GeneralException))
	Atom a1;
	Atom a2;
	Atom a3;
	Bond b1("bond", a2, a1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	TEST_EQUAL(b1.getSecondAtom(), &a2);
	b1.setFirstAtom(&a3);
	TEST_EQUAL(b1.getFirstAtom(), &a3);
	b1.finalize();
	TEST_EQUAL(b1.getFirstAtom(), &a2);
	TEST_EQUAL(b1.getSecondAtom(), &a3);
	TEST_EQUAL(a1.countBonds(), 1);
	TEST_EQUAL(a2.countBonds(), 1);
	a1.destroyBonds();
	a2.destroyBonds();
	a3.destroyBonds();
	b1.setFirstAtom(0);
	b1.setSecondAtom(0);
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Bond>(), Bond::createDefault);
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	Atom a1;
	a1.setName("a1");
	Atom a2;
	a2.setName("a2");
	Bond f1("name1", a1, a2);
	pm.setOstream(ofile);
	f1 >> pm;
	ofile.close();
RESULT

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Bond>(*ptr), true)
		Bond*	f1 = castTo<Bond>(*ptr);
		if (f1->getFirstAtom()->getName() == "a1")
		{
			TEST_EQUAL(f1->getFirstAtom()->getName(), "a1")
			TEST_EQUAL(f1->getSecondAtom()->getName(), "a2")
		}
		else
		{
			TEST_EQUAL(f1->getFirstAtom()->getName(), "a2")
			TEST_EQUAL(f1->getSecondAtom()->getName(), "a1")
		}
		TEST_EQUAL(f1->getName(), "name1")
		const Atom* a1 = f1->getFirstAtom();
		const Atom* a2 = f1->getSecondAtom();
		const_cast<Atom*>(a1)->destroyBonds();
		delete a1;
		delete a2;
		// delete f1;
		ptr = 0;
		f1 = 0;
	} 
RESULT

CHECK(bool operator == (const Bond& bond) const throw())
	Atom a1, a2, a3;
	Bond b1("test", a1, a2);
	Bond b2("test", a1, a2);
	TEST_EQUAL(b1 == b2, false)
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const Bond& bond) const throw())
	Atom a1, a2, a3;
	Bond b1("test", a1, a2);
	Bond b2("test", a1, a2);
	TEST_EQUAL(b1 != b2, true)
	TEST_EQUAL(b1 != b1, false)
RESULT

CHECK(Atom* getPartner(const Atom& atom) const throw())
	Atom a1, a2, a3;
	Bond b1("test", a1, a2);
	TEST_EQUAL(b1.getPartner(a1), &a2)
	TEST_EQUAL(b1.getPartner(a2), &a1)
	TEST_EQUAL(b1.getPartner(a3), 0)
RESULT

CHECK(BALL_CREATE_DEEP(Bond))
	Atom a1, a2, a3;
	Bond b1("test", a1, a2);
	Bond* b_ptr = (Bond*)b1.create(false, true);
	Bond b2 = *b_ptr;
	Bond empty;
	TEST_EQUAL(b2.getName(), "")
	TEST_EQUAL(b2.getFirstAtom(), 0)
	TEST_EQUAL(b2.getSecondAtom(), 0)
	delete b_ptr;
	b_ptr = (Bond*) b1.create();
	b2 = *b_ptr;
	TEST_EQUAL(b2.getName(), "test")
	TEST_EQUAL(b2.getFirstAtom(), &a1)
	TEST_EQUAL(b2.getSecondAtom(), &a2)
	delete b_ptr;
RESULT

CHECK(NotBound(const char* file, int line) throw())
	TEST_EXCEPTION(Bond::NotBound, throw(Bond::NotBound(__FILE__, __LINE__)))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
