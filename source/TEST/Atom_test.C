// $Id: Atom_test.C,v 1.4 2000/03/14 19:35:45 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PSE.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

START_TEST(Atom, "$Id: Atom_test.C,v 1.4 2000/03/14 19:35:45 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Atom*		atom;
CHECK(Atom())
atom = new Atom;
TEST_NOT_EQUAL(atom, 0)
RESULT

CHECK(setCharge(float)/getCharge())
TEST_REAL_EQUAL(atom->getCharge(), 0.0)
atom->setCharge(1.23456);
TEST_REAL_EQUAL(atom->getCharge(), 1.23456);
RESULT

CHECK(setname(String&)/getName())
TEST_EQUAL(atom->getName(), "")
atom->setName("ATOMNAME");
TEST_EQUAL(atom->getName(), "ATOMNAME");
RESULT
			
CHECK(setElement(Element&)/getElement())
TEST_EQUAL(atom->getElement(), Element::UNKNOWN)
atom->setElement(PSE.getElement(1));
TEST_EQUAL(atom->getElement(), PSE.getElement(1))
RESULT
			
Vector3	null_vector(0, 0, 0);
Vector3	test_vector(1, 2, 3);
CHECK(set/getPosition)
TEST_EQUAL(atom->getPosition(), null_vector)
atom->setPosition(test_vector);
TEST_EQUAL(atom->getPosition(), test_vector)
RESULT
			
CHECK(setRadius(float)/getRadius())
TEST_REAL_EQUAL(atom->getRadius(), 0.0)
atom->setRadius(1.23456);
TEST_REAL_EQUAL(atom->getRadius(), 1.23456)
RESULT

CHECK(setType(Atom::Type)/getType())
TEST_EQUAL(atom->getType(), Atom::UNKNOWN_TYPE)
atom->setType(27);
TEST_EQUAL(atom->getType(), 27);
RESULT
		
CHECK(setVelocity(Vector3&)/getVelocity())
TEST_EQUAL(atom->getVelocity(), null_vector)
atom->setVelocity(test_vector);
TEST_EQUAL(atom->getVelocity(), test_vector)
RESULT
			
CHECK(setForce(Vector3&)/getForce())
TEST_EQUAL(atom->getForce(), null_vector)
atom->setForce(test_vector);
TEST_EQUAL(atom->getForce(), test_vector)
RESULT
			
CHECK(countBonds())
TEST_EQUAL(atom->countBonds(), 0)
RESULT

Atom* atom2;			
CHECK(Atom(Atom&))
atom2 = new Atom(*atom);
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getName(), atom->getName())
TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
TEST_EQUAL(atom2->getPosition(), atom->getPosition())
TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom2->getForce(), atom->getForce())
TEST_EQUAL(atom2->getType(), atom->getType())
TEST_EQUAL(atom2->countBonds(), atom->countBonds())
TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(destructor)
delete atom2;
RESULT
			
CHECK(Atom(Element&, String&, Atom:::Type, Vector3&, Vector3&, Vector3&, float, float))
atom2 = new Atom(PSE[Element::HELIUM],
								 "TESTNAME",
								 "TESTTYPE",
								 12,
								 null_vector,
								 test_vector,
								 test_vector,
								 1.23456,
								 2.34567);
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getElement(), PSE[Element::He])
TEST_EQUAL(atom2->getName(), "TESTNAME")
TEST_EQUAL(atom2->getType(), 12)
TEST_EQUAL(atom2->getPosition(), null_vector)
TEST_EQUAL(atom2->getVelocity(), test_vector)
TEST_EQUAL(atom2->getForce(), test_vector)
TEST_REAL_EQUAL(atom2->getCharge(), 1.23456)
TEST_REAL_EQUAL(atom2->getRadius(), 2.34567)
RESULT

CHECK(createBond(Atom&))
TEST_EQUAL(atom2->countBonds(), 0)
TEST_EQUAL(atom->countBonds(), 0)
atom->createBond(*atom2);
TEST_EQUAL(atom->countBonds(), 1)
TEST_EQUAL(atom2->countBonds(), 1)
RESULT

CHECK(clear())
atom2->clear();
TEST_EQUAL(atom2->getName(), "")
TEST_EQUAL(atom2->getType(), Atom::UNKNOWN_TYPE)
TEST_EQUAL(atom2->getElement(), Element::UNKNOWN)
TEST_EQUAL(atom2->getPosition(), null_vector)
TEST_EQUAL(atom2->getVelocity(), null_vector)
TEST_EQUAL(atom2->getForce(), null_vector)
TEST_REAL_EQUAL(atom2->getCharge(), 0.0)
TEST_REAL_EQUAL(atom2->getRadius(), 0.0)
TEST_EQUAL(atom2->countBonds(), 0)
TEST_EQUAL(atom->countBonds(), 0)
RESULT


CHECK(create(bool))
delete atom2;
atom2 = (Atom*)atom->create(false);
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getName(), atom->getName())
TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
TEST_EQUAL(atom2->getPosition(), atom->getPosition())
TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom2->getForce(), atom->getForce())
TEST_EQUAL(atom2->getType(), atom->getType())
TEST_EQUAL(atom2->countBonds(), atom->countBonds())
TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
delete atom2;
atom2 = (Atom*)atom->create(true);
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getName(), atom->getName())
TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
TEST_EQUAL(atom2->getPosition(), atom->getPosition())
TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom2->getForce(), atom->getForce())
TEST_EQUAL(atom2->getType(), atom->getType())
TEST_EQUAL(atom2->countBonds(), atom->countBonds())
TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(destroy())
atom->createBond(*atom2);
atom2->destroy();
TEST_EQUAL(atom2->getName(), "")
TEST_EQUAL(atom2->getType(), Atom::UNKNOWN_TYPE)
TEST_EQUAL(atom2->getElement(), Element::UNKNOWN)
TEST_EQUAL(atom2->getPosition(), null_vector)
TEST_EQUAL(atom2->getVelocity(), null_vector)
TEST_EQUAL(atom2->getForce(), null_vector)
TEST_REAL_EQUAL(atom2->getCharge(), 0.0)
TEST_REAL_EQUAL(atom2->getRadius(), 0.0)
TEST_EQUAL(atom2->countBonds(), 0)
TEST_EQUAL(atom->countBonds(), 0)
RESULT

CHECK(set(Atom&))
atom2->set(*atom);
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getName(), atom->getName())
TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
TEST_EQUAL(atom2->getPosition(), atom->getPosition())
TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom2->getForce(), atom->getForce())
TEST_EQUAL(atom2->getType(), atom->getType())
TEST_EQUAL(atom2->countBonds(), atom->countBonds())
TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(operator = (Atom&))
atom2->clear();
*atom2 = *atom;
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getName(), atom->getName())
TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
TEST_EQUAL(atom2->getPosition(), atom->getPosition())
TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom2->getForce(), atom->getForce())
TEST_EQUAL(atom2->getType(), atom->getType())
TEST_EQUAL(atom2->countBonds(), atom->countBonds())
TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(get(Atom&))
atom2->clear();
atom2->get(*atom);
TEST_NOT_EQUAL(atom2, 0)
TEST_EQUAL(atom2->getName(), atom->getName())
TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
TEST_EQUAL(atom2->getPosition(), atom->getPosition())
TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom2->getForce(), atom->getForce())
TEST_EQUAL(atom2->getType(), atom->getType())
TEST_EQUAL(atom2->countBonds(), atom->countBonds())
TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT


Atom*		atom3;
Atom*		atom4;
CHECK(swap(Atom&))
atom2 = new Atom(PSE[Element::LITHIUM],
											"TESTNAME2",
											"TESTTYPE2",
											23,
											test_vector,
											test_vector,
											null_vector,
											3.23456,
											4.34567);

atom3 = new Atom(*atom);
atom4 = new Atom(*atom2);
atom3->swap(*atom4);
TEST_EQUAL(atom4->getName(), atom->getName())
TEST_REAL_EQUAL(atom4->getCharge(), atom->getCharge())
TEST_REAL_EQUAL(atom4->getRadius(), atom->getRadius())
TEST_EQUAL(atom4->getPosition(), atom->getPosition())
TEST_EQUAL(atom4->getVelocity(), atom->getVelocity())
TEST_EQUAL(atom4->getForce(), atom->getForce())
TEST_EQUAL(atom4->getType(), atom->getType())
TEST_EQUAL(atom4->countBonds(), atom->countBonds())
TEST_EQUAL(atom3->getName(), atom2->getName())
TEST_REAL_EQUAL(atom3->getCharge(), atom2->getCharge())
TEST_REAL_EQUAL(atom3->getRadius(), atom2->getRadius())
TEST_EQUAL(atom3->getPosition(), atom2->getPosition())
TEST_EQUAL(atom3->getVelocity(), atom2->getVelocity())
TEST_EQUAL(atom3->getForce(), atom2->getForce())
TEST_EQUAL(atom3->getType(), atom2->getType())
TEST_EQUAL(atom3->countBonds(), atom2->countBonds())
RESULT

Molecule*				molecule;
Fragment*				fragment;
CHECK(getMolecule())
molecule = new Molecule;
fragment = new Fragment;
molecule->insert(*fragment);
fragment->insert(*atom);
TEST_EQUAL((atom->getFragment() == fragment), true)
RESULT

CHECK(getMolecule())
TEST_EQUAL(atom->getMolecule(), molecule)
RESULT

Bond*		bond;
CHECK(createBond(Atom&)/getBond(Atom&))
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

CHECK(destroyBond(Atom&))
atom->destroyBond(*atom4);
TEST_EQUAL(atom->countBonds(), 1)
TEST_EQUAL(atom4->countBonds(), 0)
TEST_EQUAL(atom4->getBond(*atom), 0)
RESULT
			
CHECK(destroyBonds(Atom&))
atom->createBond(*atom4);
atom->destroyBonds();
TEST_EQUAL(atom->countBonds(), 0)
TEST_EQUAL(atom3->countBonds(), 0)
TEST_EQUAL(atom4->countBonds(), 0)
RESULT

			
CHECK(hasBond(Bond&))
atom->createBond(*atom4);
TEST_EQUAL(atom->hasBond(*atom->getBond(*atom3)), false)
TEST_EQUAL(atom->hasBond(*atom->getBond(*atom4)), true)
TEST_EQUAL(atom->hasBond(*atom->getBond(*atom)), false)
RESULT

CHECK(isBondedTo(Atom&))
TEST_EQUAL(atom->isBondedTo(*atom3), false)
TEST_EQUAL(atom->isBondedTo(*atom4), true)
TEST_EQUAL(atom->isBondedTo(*atom), false)
RESULT

CHECK(isBonded())
TEST_EQUAL(atom->isBonded(), true)
TEST_EQUAL(atom3->isBonded(), false)
RESULT

CHECK(isValid())
TEST_EQUAL(atom->isValid(), true)
RESULT


TextPersistenceManager	pm;
atom->setForce(Vector3(1.0, 2.0, 3.0));
atom->setPosition(Vector3(2.0, 3.0, 4.0));
atom->setVelocity(Vector3(3.0, 4.0, 5.0));
atom->setName("TESTNAME");
atom->setCharge(1.23456);
atom->setRadius(2.34567);
String filename;
using std::ofstream;
using std::ios;
CHECK(persistentWrite(TextPersistenceManager&, String&, bool))
NEW_TMP_FILE(filename)
ofstream	ofile(filename.c_str(), ios::out);
pm.setOstream(ofile);
using namespace RTTI;
pm.registerClass(getStreamName<Composite>(), Composite::createDefault);
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
pm.registerClass(getStreamName<Bond>(), Bond::createDefault);
fragment->remove(*atom);
*atom >> pm;
ofile.close();
RESULT

using std::ifstream;
using std::cout;
using namespace RTTI;
CHECK(persistentRead(TextPersistenceManager()))
ifstream	ifile(filename.c_str());
pm.setIstream(ifile);
PersistentObject*	ptr;
ptr = pm.readObject();
ifile.close();
TEST_NOT_EQUAL(ptr, 0)
if (ptr != 0)
{
	TEST_EQUAL(isKindOf<Atom>(*ptr), true)
	if (isKindOf<Atom>(*ptr))
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
	}
}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
