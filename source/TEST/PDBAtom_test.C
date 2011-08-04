// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////
using namespace BALL;

bool testEqual(const PDBAtom& a, const PDBAtom& b)
{
	return a.getElement() == b.getElement() &&
				 a.getName() == b.getName() &&
				 a.getTypeName() == b.getTypeName() &&
				 a.getType() == b.getType() &&
				 a.getPosition() == b.getPosition() &&
				 a.getVelocity() == b.getVelocity() &&
				 a.getForce() == b.getForce() &&
				 a.getCharge() == b.getCharge() &&
				 a.getRadius() == b.getRadius() &&
				 a.getBranchDesignator() == b.getBranchDesignator() &&
				 a.getRemotenessIndicator() == b.getRemotenessIndicator() &&
				 a.getAlternateLocationIndicator() == b.getAlternateLocationIndicator() &&
				 a.getOccupancy() == b.getOccupancy() &&
				 a.getTemperatureFactor() == b.getTemperatureFactor();
}

START_TEST(PDBAtom)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


// tests for class PDBAtom::

PDBAtom* pa = 0;

CHECK(PDBAtom() throw())
	pa = new PDBAtom;
	TEST_NOT_EQUAL(pa, 0)
	TEST_EQUAL(pa->isValid(), true)
RESULT


CHECK(~PDBAtom() throw())
	delete pa;	
RESULT


Vector3 position(0.1, 1.1, 2.1);
Vector3 velocity(3.1, 4.1, 5.1);
Vector3 force(6.1, 7.1, 8.1);
String filename;
TextPersistenceManager  pm;
using std::ofstream;
using std::ifstream;
using std::ios;
using namespace RTTI;

CHECK(PDBAtom(Element& element, const String& name, const String& type_name = BALL_ATOM_DEFAULT_TYPE_NAME, Atom::Type atom_type = BALL_ATOM_DEFAULT_TYPE, const Vector3& position = Vector3(BALL_ATOM_DEFAULT_POSITION), const Vector3& velocity = Vector3(BALL_ATOM_DEFAULT_VELOCITY), const Vector3& force = Vector3(BALL_ATOM_DEFAULT_FORCE), float charge = BALL_ATOM_DEFAULT_CHARGE, float radius = BALL_ATOM_DEFAULT_RADIUS, char branch_designator =BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR, char remoteness_indicator =BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR, char alternate_location_indicator = BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR, float occupancy = BALL_PDBATOM_DEFAULT_OCCUPANCY, float temperature_factor = BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR) throw())
	pa = new PDBAtom(PTE.getElement(1), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);
	TEST_NOT_EQUAL(pa, 0)
	TEST_EQUAL(pa->isValid(), true)
	delete pa;
RESULT

PDBAtom pdba(PTE.getElement(13), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);

CHECK(PDBAtom(const PDBAtom& pdb_atom, bool deep = true) throw())
	PDBAtom* p2 = new PDBAtom(pdba);
	TEST_NOT_EQUAL(p2, 0)
	TEST_EQUAL(testEqual(pdba, *p2), true)
	delete p2;
RESULT


CHECK(bool operator == (const PDBAtom& pdb_atom) const throw())
  PDBAtom p2;
	TEST_EQUAL(pdba == p2, false)
	TEST_EQUAL(p2 == p2, true)
RESULT


CHECK(bool operator != (const PDBAtom& pdb_atom) const throw())
  PDBAtom p2;
	TEST_EQUAL(pdba != p2, true)
	TEST_EQUAL(p2 != p2, false)
RESULT


CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	NEW_TMP_FILE(filename)
	ofstream ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	pm.registerClass(getStreamName<PDBAtom>(), PDBAtom::createDefault);
	pdba >> pm;
	ofile.close();
RESULT


CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	ifstream  ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject* ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<PDBAtom>(*ptr), true)
		if (isKindOf<PDBAtom>(*ptr))
		{
			PDBAtom* p2 = castTo<PDBAtom>(*ptr);
			TEST_EQUAL(testEqual(*p2, pdba), true)
		}
		delete ptr;
	}
RESULT


CHECK(void set(const PDBAtom& pdb_atom, bool deep = true) throw())
  PDBAtom p2;
	p2.set(pdba);
	TEST_EQUAL(testEqual(pdba, p2), true)
RESULT


CHECK(void get(PDBAtom& pdb_atom, bool deep = true) const throw())
  PDBAtom p2;
	pdba.get(p2);
	TEST_EQUAL(testEqual(pdba, p2), true)
RESULT


CHECK(PDBAtom& operator = (const PDBAtom& pdb_atom) throw())
  PDBAtom p2;
	p2 = pdba;
	TEST_EQUAL(testEqual(pdba, p2), true)
RESULT


CHECK(void swap(PDBAtom& pdb_atom) throw())
  PDBAtom p2(pdba);
	p2.setTemperatureFactor((float)99.9);
	pdba.swap(p2);
	TEST_REAL_EQUAL(p2.getTemperatureFactor(), (float) 12.1)
	TEST_REAL_EQUAL(pdba.getTemperatureFactor(), (float) 99.9)
  p2.swap(pdba);
	TEST_REAL_EQUAL(p2.getTemperatureFactor(), (float) 99.9)
	TEST_REAL_EQUAL(pdba.getTemperatureFactor(), (float) 12.1)
RESULT

Protein p;
Chain c;
Residue r;

CHECK(Protein* getProtein() throw())
  TEST_EQUAL(pdba.getProtein(), 0)
	p.insert(pdba);
	pdba.getProtein()->setName("protein");
  TEST_EQUAL(pdba.getProtein(), &p)
	TEST_EQUAL(p.getName(), "protein")
	p.remove(pdba);
RESULT


CHECK(const Protein* getProtein() const throw())
	const PDBAtom& p2(pdba);
  TEST_EQUAL(p2.getProtein(), 0)
	p.insert(pdba);
  TEST_EQUAL(pdba.getProtein(), &p)
	TEST_EQUAL(p.getName(), "protein")
	p.remove(pdba);
RESULT

c.insert(r);

CHECK(Chain* getChain() throw())
  TEST_EQUAL(pdba.getChain(), 0)
	r.insert(pdba);
	pdba.getChain()->setName("protein");
  TEST_EQUAL(pdba.getChain(), &c)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT


CHECK(const Chain* getChain() const throw())
	const PDBAtom& p2(pdba);
  TEST_EQUAL(p2.getChain(), 0)
	r.insert(pdba);
  TEST_EQUAL(pdba.getChain(), &c)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT

CHECK(Residue* getResidue() throw())
  TEST_EQUAL(pdba.getResidue(), 0)
	r.insert(pdba);
	pdba.getResidue()->setName("protein");
  TEST_EQUAL(pdba.getResidue(), &r)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT


CHECK(const Residue* getResidue() const throw())
	const PDBAtom& p2(pdba);
  TEST_EQUAL(p2.getResidue(), 0)
	r.insert(pdba);
  TEST_EQUAL(pdba.getResidue(), &r)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT


CHECK(void setBranchDesignator(char branch_designator) throw())
	PDBAtom p2;
	p2.setBranchDesignator('A');
	TEST_EQUAL(p2.getBranchDesignator(), 'A')
RESULT

CHECK(char getBranchDesignator() const throw())
	PDBAtom p;
	TEST_EQUAL(p.getBranchDesignator(), ' ')
RESULT

CHECK(void setRemotenessIndicator(char remoteness_indicator) throw())
	PDBAtom p2;
	p2.setRemotenessIndicator('B');
	TEST_EQUAL(p2.getRemotenessIndicator(), 'B')
RESULT

CHECK(char getRemotenessIndicator() const throw())
	PDBAtom p;
	TEST_EQUAL(p.getRemotenessIndicator(), ' ')
RESULT

CHECK(void setAlternateLocationIndicator(char alternate_location_indicator) throw())
	PDBAtom p2;
	p2.setAlternateLocationIndicator('B');
	TEST_EQUAL(p2.getAlternateLocationIndicator(), 'B')
RESULT

CHECK(char getAlternateLocationIndicator() const throw())
	PDBAtom p;
	TEST_EQUAL(p.getAlternateLocationIndicator(), ' ')
RESULT

CHECK(void setOccupancy(float occupancy) throw())
	PDBAtom p2;
	p2.setOccupancy(12.23);
	TEST_REAL_EQUAL(p2.getOccupancy(), (float) 12.23)
RESULT

CHECK(float getOccupancy() const throw())
	PDBAtom p;
	TEST_REAL_EQUAL(p.getOccupancy(), 1)
RESULT

CHECK(void setTemperatureFactor(float temperature_factor) throw())
	PDBAtom p2;
	p2.setTemperatureFactor(23.34);
	TEST_REAL_EQUAL(p2.getTemperatureFactor(), (float) 23.34)
RESULT

CHECK(float getTemperatureFactor() const throw())
	PDBAtom p;
	TEST_REAL_EQUAL(p.getTemperatureFactor(), 0)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	pdba.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PDBAtom_test_dump.txt))
RESULT

CHECK(BALL_CREATE_DEEP(PDBAtom))
	PDBAtom empty;
	PDBAtom p(PTE.getElement(1), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);
	PDBAtom* test;
	test = (PDBAtom*) p.create(false, true);
	TEST_EQUAL(testEqual(*test, empty), true)
	delete test;
	test = (PDBAtom*) p.create(true, false);
	TEST_EQUAL(testEqual(*test, p), true)
	delete test;
RESULT

CHECK(PDBAtom(const String& name) throw())
	PDBAtom p("pdbatom1");
	TEST_EQUAL(p.getName(), "pdbatom1")
	p.setName("");
	PDBAtom empty;
	TEST_EQUAL(testEqual(empty, p), true)
RESULT

CHECK(void clear() throw())
	PDBAtom empty;
	PDBAtom p(PTE.getElement(1), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);
	p.clear();
	TEST_EQUAL(testEqual(empty, p), true)
RESULT

CHECK(void destroy() throw())
	PDBAtom empty;
	PDBAtom p(PTE.getElement(1), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);
	p.destroy();
	TEST_EQUAL(testEqual(empty, p), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

