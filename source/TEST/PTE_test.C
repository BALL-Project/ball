// $Id: PTE_test.C,v 1.1 2000/05/18 20:21:08 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/PTE.h>
///////////////////////////

START_TEST(Element, "$Id: PTE_test.C,v 1.1 2000/05/18 20:21:08 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Element*	e;
CHECK(Element())
	e = new Element;
	TEST_NOT_EQUAL(e, 0)
RESULT

CHECK(~Element())
	delete e;
RESULT

CHECK(Element(Element&, bool))
	Element* e1 = new Element;
	e1->setName("testname");
	Element* e2 = new Element(*e1, true);
	TEST_NOT_EQUAL(e2, 0)
	if (e2 != 0)
	{
		TEST_EQUAL(e2->getName(), "testname")
		delete e2;
	}
RESULT

CHECK(Element(const String& name,
						 const String& symbol,
						 Group group,
						 Period period,
						 AtomicNumber atomic_umber,
						 float atomic_weight,
						 float atomic_radius,
						 float covalent_radius,
						 float van_der_waals_radius,
						 float electronegativity))
	Element* e1 = new Element("e1", "id", 2, 3, 25, 25.0, 2.0, 3.0, 4.0, 5.0);
	TEST_NOT_EQUAL(e1, 0)
	if (e1 != 0)
	{
		TEST_EQUAL(e1->getName(), "e1")
		TEST_EQUAL(e1->getSymbol(), "id")
		TEST_EQUAL(e1->getGroup(), 2)
		TEST_EQUAL(e1->getPeriod(), 3)
		TEST_EQUAL(e1->getAtomicNumber(), 25)
		TEST_EQUAL(e1->getAtomicWeight(), 25.0)
		TEST_EQUAL(e1->getAtomicRadius(), 2.0)
		TEST_EQUAL(e1->getCovalentRadius(), 3.0)
		TEST_EQUAL(e1->getVanDerWaalsRadius(), 4.0)
		TEST_EQUAL(e1->getElectronegativity(), 5.0)
		delete e1;
	}/*
	Element* e2 = new Element(*e1);
	TEST_NOT_EQUAL(e2, 0)
	if (e2 != 0)
	{
		TEST_EQUAL(e2->getName(), "e1")
		TEST_EQUAL(e2->getSymbol(), "id")
		TEST_EQUAL(e2->getGroup(), 2)
		TEST_EQUAL(e2->getPeriod(), 3)
		TEST_EQUAL(e2->getAtomicNumber(), 25)
		TEST_EQUAL(e2->getAtomicWeight(), 25.0)
		TEST_EQUAL(e2->getAtomicRadius(), 2.0)
		TEST_EQUAL(e2->getCovalentRadius(), 3.0)
		TEST_EQUAL(e2->getVanDerWaalsRadius(), 4.0)
		TEST_EQUAL(e2->getElectronegativity(), 5.0)
		delete e2;
	}*/
RESULT

Element e1;
Element e2;
Element e3;

CHECK(setName(const String& name))
	e1.setName("e1");
RESULT

CHECK(getName())
	TEST_EQUAL(e1.getName(), "e1")
	TEST_EQUAL(e2.getName(), "Unknown")
RESULT

CHECK(setSymbol(const String& symbol))
	e1.setSymbol("s");
RESULT

CHECK(getSymbol())
	TEST_EQUAL(e1.getSymbol(), "s")
	TEST_EQUAL(e2.getSymbol(), "?")
RESULT

CHECK(setGroup(Group group))
	e1.setGroup(2);
RESULT

CHECK(getGroup())
	TEST_EQUAL(e1.getGroup(), 2)
	TEST_EQUAL(e2.getGroup(), 0)
RESULT

CHECK(setPeriod(Period Period))
	e1.setPeriod(3);
RESULT

CHECK(getPeriod())
	TEST_EQUAL(e1.getPeriod(), 3)
	TEST_EQUAL(e2.getPeriod(), 0)
RESULT

CHECK(setAtomicNumber(AtomicNumber AtomicNumber))
	e1.setAtomicNumber(4);
RESULT

CHECK(getAtomicNumber())
	TEST_EQUAL(e1.getAtomicNumber(), 4)
	TEST_EQUAL(e2.getAtomicNumber(), 0)
RESULT

CHECK(setAtomicWeight(float AtomicWeight))
	e1.setAtomicWeight(5.0);
RESULT

CHECK(getAtomicWeight())
	TEST_EQUAL(e1.getAtomicWeight(), 5.0)
	TEST_EQUAL(e2.getAtomicWeight(), 0.0)
RESULT

CHECK(setAtomicRadius(float AtomicRadius))
	e1.setAtomicRadius(6.0);
RESULT

CHECK(getAtomicRadius())
	TEST_EQUAL(e1.getAtomicRadius(), 6.0)
	TEST_EQUAL(e2.getAtomicRadius(), 0.0)
RESULT

CHECK(setCovalentRadius(float CovalentRadius))
	e1.setCovalentRadius(7.0);
RESULT

CHECK(getCovalentRadius())
	TEST_EQUAL(e1.getCovalentRadius(), 7.0)
	TEST_EQUAL(e2.getCovalentRadius(), 0.0)
RESULT

CHECK(setVanDerWaalsRadius(float VanDerWaalsRadius))
	e1.setVanDerWaalsRadius(8.0);
RESULT

CHECK(getVanDerWaalsRadius())
	TEST_EQUAL(e1.getVanDerWaalsRadius(), 8.0)
	TEST_EQUAL(e2.getVanDerWaalsRadius(), 0.0)
RESULT

CHECK(setElectronegativity(float Electronegativity))
	e1.setElectronegativity(9.0);
RESULT

CHECK(getElectronegativity())
	TEST_EQUAL(e1.getElectronegativity(), 9.0)
	TEST_EQUAL(e2.getElectronegativity(), 0.0)
RESULT

CHECK(operator ==(const Element& element) const)
	TEST_EQUAL(e1 == e2, false)
	TEST_EQUAL(e3 == e2, true)
RESULT

CHECK(operator !=(const Element& element) const)
	TEST_EQUAL(e3 != e1, true)
	TEST_EQUAL(e3 != e2, false)
RESULT

CHECK(operator <(const Element& element) const)
	TEST_EQUAL(e2 < e1, true)
	TEST_EQUAL(e2 < e3, false)
RESULT

CHECK(operator <=(const Element& element) const)
	TEST_EQUAL(e1 <= e2, false)
	TEST_EQUAL(e3 <= e2, true)
	TEST_EQUAL(e2 <= e1, true)
RESULT

CHECK(operator >=(const Element& element) const)
	TEST_EQUAL(e2 >= e1, false)
	TEST_EQUAL(e1 >= e2, true)
	TEST_EQUAL(e2 >= e2, true)
RESULT

CHECK(operator >(const Element& element) const)
	TEST_EQUAL(e2 > e1, false)
	TEST_EQUAL(e1 > e2, true)
	TEST_EQUAL(e2 > e3, false)
RESULT

CHECK(isUnknown() const)
	TEST_EQUAL(e1.isUnknown(), false)
	TEST_EQUAL(e2.isUnknown(), true)
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const Element& element))
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << e1;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/PTE_test.txt", false)
RESULT

CHECK(getElement(Position position))
	TEST_EQUAL(PTE.getElement(1).getName(), "Aluminium")
RESULT

CHECK(getElement(const String& symbol))
	TEST_EQUAL(PTE.getElement("Al").getName(), "Aluminium")
RESULT

CHECK(Element &operator [](const String& symbol))
	TEST_EQUAL(PTE["Al"].getName(), "Aluminium")
RESULT

CHECK(Element &operator [](Element::Symbol symbol))
	TEST_EQUAL(PTE[Element::Al].getName(), "Aluminium")
RESULT

CHECK(Element &operator [](Position position))
	TEST_EQUAL(PTE[13].getName(), "Aluminium")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
