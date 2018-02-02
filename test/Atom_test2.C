// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

#include "ItemCollector.h"
///////////////////////////

START_TEST(Atom)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Vector3	null_vector(0, 0, 0);
Vector3	test_vector(1, 2, 3);

const Atom ac;
Atom* atom = new Atom;
Atom* atom3 = new Atom(*atom);
Atom* atom4 = new Atom(PTE[Element::LITHIUM], "TESTNAME2", "TESTTYPE2", 23, test_vector, test_vector, null_vector, 3.23456, 4.34567);

Atom atomx;
const Atom& acx = atomx;
Molecule molecule;
Fragment fragment;
molecule.insert(fragment);
fragment.insert(atomx);
CHECK(const Fragment* getFragment() const throw())
	TEST_EQUAL((atomx.getFragment() == &fragment), true)
RESULT

CHECK(Fragment* getFragment() throw())
	atomx.getFragment()->setName("1234");
	TEST_EQUAL(fragment.getName(), "1234")
RESULT

CHECK(const Molecule* getMolecule() const throw())
	TEST_EQUAL(acx.getMolecule(), &molecule)
RESULT

CHECK(Molecule* getMolecule() throw())
	atomx.getMolecule()->setName("asdff");
	TEST_EQUAL(molecule.getName(), "asdff")
RESULT

Bond*	bond;
CHECK(Bond* createBond(Atom& atom) throw())
	atom->createBond(*atom3);
	atom3->getBond(*atom);
	TEST_EQUAL(atom->countBonds(), 1)
	TEST_EQUAL(atom3->countBonds(), 1)
	TEST_EQUAL(atom->getBond(*atom3), atom3->getBond(*atom))
	atom->createBond(*atom4);
	atom->createBond(*atom4);
	bond = atom->createBond(*atom);
	TEST_EQUAL(bond, 0);

	TEST_EQUAL(atom4->countBonds(), 1)
	TEST_EQUAL(atom->countBonds(), 2)
	TEST_EQUAL(atom->getBond(*atom4), atom4->getBond(*atom))
RESULT

CHECK(bool destroyBond(const Atom& atom) throw())
	atom->destroyBond(*atom4);
	TEST_EQUAL(atom->countBonds(), 1)
	TEST_EQUAL(atom4->countBonds(), 0)
	TEST_EQUAL(atom4->getBond(*atom), 0)
RESULT
bond = 0;
			
CHECK(void destroyBonds() throw())
	atom->createBond(*atom4);
	atom->destroyBonds();
	TEST_EQUAL(atom->countBonds(), 0)
	TEST_EQUAL(atom3->countBonds(), 0)
	TEST_EQUAL(atom4->countBonds(), 0)
RESULT

CHECK(bool hasBond(const Bond& bond) const throw())
	atom->createBond(*atom4);
	TEST_EQUAL(atom->hasBond(*atom->getBond(*atom3)), false)
	TEST_EQUAL(atom->hasBond(*atom->getBond(*atom4)), true)
	TEST_EQUAL(atom->hasBond(*atom->getBond(*atom)), false)
RESULT

CHECK(bool isBoundTo(const Atom& atom) const throw())
	TEST_EQUAL(atom->isBoundTo(*atom3), false)
	TEST_EQUAL(atom->isBoundTo(*atom4), true)
	TEST_EQUAL(atom->isBoundTo(*atom), false)
RESULT

CHECK(bool isBound() const throw())
	TEST_EQUAL(atom->isBound(), true)
	TEST_EQUAL(atom3->isBound(), false)
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(atom->isValid(), true)
RESULT

TextPersistenceManager pm;
atom->setForce(Vector3(1.0, 2.0, 3.0));
atom->setPosition(Vector3(2.0, 3.0, 4.0));
atom->setVelocity(Vector3(3.0, 4.0, 5.0));
atom->setName("TESTNAME");
atom->setCharge(1.23456);
atom->setRadius(2.34567);
String filename;
using std::ofstream;
using std::ios;
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const)
	NEW_TMP_FILE(filename)
	ofstream	ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	using namespace RTTI;
	*atom >> pm;
	ofile.close();
RESULT

using std::ifstream;
using std::cout;
using namespace RTTI;
CHECK(void persistentRead(PersistenceManager& pm))
	ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = 0;
	ptr = pm.readObject();
	ifile.close();
	STATUS("PO ptr = " << (void*)ptr)

	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
        TEST_EQUAL(isKindOf<Atom>(ptr), true)
        if (isKindOf<Atom>(ptr))
		{
			Atom* pers_atom = castTo<Atom>(*ptr);
			TEST_EQUAL(pers_atom->getName(), atom->getName())
			TEST_REAL_EQUAL(pers_atom->getCharge(), atom->getCharge())
			TEST_REAL_EQUAL(pers_atom->getRadius(), atom->getRadius())
			TEST_EQUAL(pers_atom->getPosition(), atom->getPosition())
			TEST_EQUAL(pers_atom->getVelocity(), atom->getVelocity())
			TEST_EQUAL(pers_atom->getForce(), atom->getForce())
			TEST_EQUAL(pers_atom->getType(), atom->getType())
			TEST_EQUAL(pers_atom->countBonds(), atom->countBonds())
			TEST_NOT_EQUAL(pers_atom->getHandle(), atom->getHandle())
			delete pers_atom->getBond(0)->getPartner(*pers_atom);
		}
		delete ptr;
		ptr = 0;
	}
	ptr = 0;
RESULT
delete atom;
delete atom3;
delete atom4;

atom = 0;

CHECK(bool operator == (const Atom& atom) const throw())
	Atom a1;
	Atom a2;
	TEST_EQUAL(a1 == a2, false)
	a1 = a2;
	TEST_EQUAL(a1 == a2, false)

	TEST_EQUAL(a2 == a2, true)
RESULT

CHECK(bool operator != (const Atom& atom) const throw())
	Atom a1;
	Atom a2;
	TEST_EQUAL(a1 != a2, true)
	a1 = a2;
	TEST_EQUAL(a1 != a2, true)

	TEST_EQUAL(a2 != a2, false)
RESULT

CHECK([EXTRA] bond iteration)
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	Atom* a3 = new Atom;
	Atom* a4 = new Atom;
	a1->createBond(*a2);
	a1->createBond(*a3);
	a1->createBond(*a4);
	TEST_EQUAL(a1->countBonds(), 3)
	TEST_EQUAL(a2->countBonds(), 1)
	TEST_EQUAL(a3->countBonds(), 1)
	TEST_EQUAL(a4->countBonds(), 1)
	ABORT_IF(a1->countBonds() != 3)
	
	Atom::BondIterator it(a1->beginBond());
	TEST_EQUAL(+it, true)
	TEST_EQUAL(&*it, a1->getBond(0))
	++it;
	TEST_EQUAL(+it, true)
	TEST_EQUAL(&*it, a1->getBond(1))
	++it;
	TEST_EQUAL(+it, true)	
	TEST_EQUAL(&*it, a1->getBond(2))
	++it;
	TEST_EQUAL(+it, false)	
	TEST_EQUAL(it == a1->endBond(), true)

	delete a1;
	delete a2;
	delete a3;
	delete a4;

	// Empty iteration
	a1 = new Atom;
	it = a1->beginBond();
	TEST_EQUAL(+it, false)
	TEST_EQUAL(it == a1->endBond(), true)
	delete a1;
RESULT

CHECK(Bond* cloneBond(Bond& bond, Atom& atom) throw())
	Atom a, b, c;
	Bond bond("",a,b);
	TEST_EQUAL(a.cloneBond(bond, b), &bond)
	Bond* bond2 = a.cloneBond(bond,c);
	TEST_EQUAL(bond2 != 0, true)
	TEST_EQUAL(a.countBonds(), 2)
RESULT

CHECK(Bond* createBond(Bond& bond, Atom& atom) throw())
	Atom a,b,c;
	Bond bond;
	TEST_EQUAL(a.createBond(bond, a), 0)
	TEST_EQUAL(a.countBonds(), 0)
	TEST_EQUAL(a.createBond(bond, b), &bond)
	TEST_EQUAL(a.countBonds(), 1)
	TEST_EQUAL(a.isBoundTo(b), true)
	TEST_EQUAL(a.createBond(bond, b), &bond)
	TEST_EQUAL(a.countBonds(), 1)
	TEST_EQUAL(a.isBoundTo(b), true)
	TEST_EQUAL(a.createBond(bond, c), &bond)
	TEST_EQUAL(a.countBonds(), 1)
	TEST_EQUAL(a.isBoundTo(b), false)
	TEST_EQUAL(a.isBoundTo(c), true)
RESULT

CHECK(Bond* getBond(Position index))
	Atom a,b;
	Bond bond("", a,b);
	a.getBond(0)->setName("1234");
	TEST_EQUAL(bond.getName(), "1234")
	TEST_EXCEPTION(Exception::IndexOverflow, a.getBond(12))
RESULT

CHECK(const Bond* getBond(Position index) const)
	Atom a,b;
	Bond bond("", a,b);
	const Atom& ac = a;
	TEST_EQUAL(ac.getBond(0), &bond)
	TEST_EQUAL(ac.getBond(1), 0)
	TEST_EXCEPTION(Exception::IndexOverflow, ac.getBond(12))
RESULT

CHECK(Bond* getBond(const Atom& atom) throw())
	Atom a,b;
	Bond bond("", a,b);
	a.getBond(b)->setName("asdad");
	TEST_EQUAL(bond.getName(), "asdad")
RESULT

CHECK(const Bond* getBond(const Atom& atom) const throw())
	Atom a,b,c;
	Bond bond("", a,b);
	const Atom& ac = a;
	TEST_EQUAL(ac.getBond(b), &bond)
	TEST_EQUAL(ac.getBond(c), 0)
RESULT

CHECK(String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const throw())
	Fragment f;
	f.setName("fragment1");
	Atom a;
	a.setName("atom1");
	f.append(a);
	TEST_EQUAL(a.getFullName(), "fragment1:atom1")
RESULT

CHECK(void setTypeName(const String& name) throw())
	Atom a;
	a.setTypeName("asdd");
	TEST_EQUAL(a.getTypeName(), "asdd")
RESULT

CHECK(String getTypeName() const throw())
	TEST_EQUAL(ac.getTypeName(), BALL_ATOM_DEFAULT_TYPE_NAME)
RESULT

CHECK(bool applyBonds(UnaryProcessor<Bond>& processor) throw())
	ItemCollector<Bond> ic;
	Atom a,b,c,x;
	Bond bond1("", a,b);
	Bond bond2("", a,c);
	a.applyBonds(ic);
	TEST_EQUAL(ic.getList().size(), 2)
	list<Bond*>::iterator it = ic.getList().begin();
	TEST_EQUAL(*it, &bond1)
	it++;
	TEST_EQUAL(*it, &bond2)
	b.applyBonds(ic);
	TEST_EQUAL(ic.getList().size(), 1)
	x.applyBonds(ic);
	TEST_EQUAL(ic.getList().size(), 0)
RESULT

CHECK(bool isGeminal(const Atom& atom) const throw())
	Atom a,b,c,d;
	TEST_EQUAL(a.isGeminal(a), false)
	TEST_EQUAL(a.isGeminal(b), false)
	Bond bond1("", a,b);
	Bond bond2("", b,c);
	TEST_EQUAL(a.isGeminal(b), false)
	TEST_EQUAL(a.isGeminal(c), true)
	TEST_EQUAL(c.isGeminal(a), true)
RESULT

CHECK(bool isVicinal(const Atom& atom) const throw())
 	Atom a,b,c,d;
	TEST_EQUAL(a.isVicinal(a), false)
	TEST_EQUAL(a.isVicinal(b), false)
	Bond bond1("", a,b);
	Bond bond2("", b,c);
	Bond bond3("", c,d);
	TEST_EQUAL(a.isVicinal(b), false)
	TEST_EQUAL(a.isVicinal(c), false)
	TEST_EQUAL(a.isVicinal(d), true)
	TEST_EQUAL(d.isVicinal(a), true)
RESULT

CHECK(const Residue* getResidue() const throw())
	Atom a;
	TEST_EQUAL(a.getResidue(), 0)
	Fragment f;
	f.append(a);
	TEST_EQUAL(a.getResidue(), 0)
	Residue r;
	r.append(*(PDBAtom*) &a);
	TEST_EQUAL(a.getResidue(), &r)
RESULT

CHECK(const Chain* getChain() const throw())
	Atom a;
	TEST_EQUAL(a.getChain(), 0)
	Fragment f;
	f.append(a);
	TEST_EQUAL(a.getChain(), 0)
	Residue r;
	SecondaryStructure ss;
	ss.insert(r);
	Chain c;
	c.insert(ss);
	r.append(*(PDBAtom*) &a);
	TEST_EQUAL(a.getChain(), &c)
RESULT

CHECK(const SecondaryStructure* getSecondaryStructure() const throw())
	Atom a;
	TEST_EQUAL(a.getSecondaryStructure(), 0)
	Fragment f;
	f.append(a);
	TEST_EQUAL(a.getSecondaryStructure(), 0)
	Residue r;
	SecondaryStructure ss;
	ss.insert(r);
	r.append(*(PDBAtom*) &a);
	TEST_EQUAL(a.getSecondaryStructure(), &ss)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Atom a(PTE[Element::HELIUM], "Atom1", "my_type_name", 'A',Vector3(1,2,3), Vector3(4,5,6), Vector3(7,8,9), 10.1, 11.2);

  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	a.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Atom_test.txt))
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
