// $Id: PDBAtom_test.C,v 1.4 2001/06/26 19:34:56 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

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

START_TEST(class_name, "$Id: PDBAtom_test.C,v 1.4 2001/06/26 19:34:56 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


// tests for class PDBAtom::

CHECK(PDBAtom::BALL_CREATE_DEEP(PDBAtom))
  //BAUSTELLE
RESULT


PDBAtom* pa;

CHECK(PDBAtom)
	pa = new PDBAtom;
	TEST_NOT_EQUAL(pa, 0)
	TEST_EQUAL(pa->isValid(), true)
RESULT


CHECK(~PDBAtom)
	delete pa;	
RESULT


Vector3 position(0.1, 1.1, 2.1);
Vector3 velocity(3.1, 4.1, 5.1);
Vector3 force(6.1, 7.1, 8.1);
PDBAtom pdba;
String filename;
TextPersistenceManager  pm;
using std::ofstream;
using std::ifstream;
using std::ios;
using namespace RTTI;

CHECK(detailled cstr)
	pa = new PDBAtom(PTE.getElement(1), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);
	TEST_NOT_EQUAL(pa, 0)
	TEST_EQUAL(pa->isValid(), true)
	delete pa;
	pdba = PDBAtom(PTE.getElement(1), "name", "type_name" , 0, position, velocity, force, 9.1, 10.1, 'a', 'b', 'c', 11.1, 12.1);
RESULT


CHECK(cpy cstr)
	PDBAtom* p2 = new PDBAtom(pdba);
	TEST_NOT_EQUAL(p2, 0)
	TEST_EQUAL(testEqual(pdba, *p2), true)
RESULT


CHECK(PDBAtom::bool operator == (const PDBAtom& pdb_atom) const  throw())
  PDBAtom p2;
	TEST_EQUAL(pdba == p2, false)
	TEST_EQUAL(p2 == p2, true)
RESULT


CHECK(PDBAtom::bool operator != (const PDBAtom& pdb_atom) const  throw())
  PDBAtom p2;
	TEST_EQUAL(pdba != p2, true)
	TEST_EQUAL(p2 != p2, false)
RESULT


CHECK(PDBAtom::persistentWrite(PersistenceManager& pm, const char* name = 0) const  throw())
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), File::OUT);
	pm.setOstream(ofile);
	pm.registerClass(getStreamName<PDBAtom>(), PDBAtom::createDefault);
	pdba >> pm;
RESULT


CHECK(PDBAtom::persistentRead(PersistenceManager& pm) throw())
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
	}
RESULT


CHECK(PDBAtom::set(const PDBAtom& pdb_atom, bool deep = true) throw())
  PDBAtom p2;
	p2.set(pdba);
	TEST_EQUAL(testEqual(pdba, p2), true)
RESULT


CHECK(PDBAtom::get(PDBAtom& pdb_atom, bool deep = true) const  throw())
  PDBAtom p2;
	pdba.get(p2);
	TEST_EQUAL(testEqual(pdba, p2), true)
RESULT


CHECK(PDBAtom::PDBAtom& operator = (const PDBAtom& pdb_atom) throw())
  PDBAtom p2;
	p2 = pdba;
	TEST_EQUAL(testEqual(pdba, p2), true)
RESULT


CHECK(PDBAtom::swap(PDBAtom& pdb_atom) throw())
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

CHECK(PDBAtom::getProtein() throw())
  TEST_EQUAL(pdba.getProtein(), 0)
	p.insert(pdba);
	pdba.getProtein()->setName("protein");
  TEST_EQUAL(pdba.getProtein(), &p)
	TEST_EQUAL(p.getName(), "protein")
	p.remove(pdba);
RESULT


CHECK(PDBAtom::getProtein() const  throw())
	const PDBAtom& p2(pdba);
  TEST_EQUAL(p2.getProtein(), 0)
	p.insert(pdba);
  TEST_EQUAL(pdba.getProtein(), &p)
	TEST_EQUAL(p.getName(), "protein")
	p.remove(pdba);
RESULT

c.insert(r);

CHECK(PDBAtom::getChain() throw())
  TEST_EQUAL(pdba.getChain(), 0)
	r.insert(pdba);
	pdba.getChain()->setName("protein");
  TEST_EQUAL(pdba.getChain(), &c)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT


CHECK(PDBAtom::getChain() const  throw())
	const PDBAtom& p2(pdba);
  TEST_EQUAL(p2.getChain(), 0)
	r.insert(pdba);
  TEST_EQUAL(pdba.getChain(), &c)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT

CHECK(PDBAtom::getResidue() throw())
  TEST_EQUAL(pdba.getResidue(), 0)
	r.insert(pdba);
	pdba.getResidue()->setName("protein");
  TEST_EQUAL(pdba.getResidue(), &r)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT


CHECK(PDBAtom::getResidue() const  throw())
	const PDBAtom& p2(pdba);
  TEST_EQUAL(p2.getResidue(), 0)
	r.insert(pdba);
  TEST_EQUAL(pdba.getResidue(), &r)
	TEST_EQUAL(c.getName(), "protein")
	r.remove(pdba);
RESULT


CHECK(setBranchDesignator/getBranchDesignator)
	PDBAtom p2;
	p2.setBranchDesignator('A');
	TEST_EQUAL(p2.getBranchDesignator(), 'A')
RESULT


CHECK(setRemotenessIndicator/getRemotenessIndicator)
	PDBAtom p2;
	p2.setRemotenessIndicator('B');
	TEST_EQUAL(p2.getRemotenessIndicator(), 'B')
RESULT


CHECK(setAlternateLocationIndicator/getAlternateLocationIndicator)
	PDBAtom p2;
	p2.setAlternateLocationIndicator('B');
	TEST_EQUAL(p2.getAlternateLocationIndicator(), 'B')
RESULT


CHECK(setOccupancy/getOccupancy)
	PDBAtom p2;
	p2.setOccupancy(12.23);
	TEST_REAL_EQUAL(p2.getOccupancy(), (float) 12.23)
RESULT


CHECK(setTemperatureFactor/getTemperatureFactor)
	PDBAtom p2;
	p2.setTemperatureFactor(23.34);
	TEST_REAL_EQUAL(p2.getTemperatureFactor(), (float) 23.34)
RESULT


CHECK(PDBAtom::dump(std::ostream& s = std::cout, Size depth = 0) const  throw())
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	pdba.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/PDBAtom_test_dump.txt", true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

