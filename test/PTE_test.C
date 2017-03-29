// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/PTE.h>
#include "ItemCollector.h"
///////////////////////////

START_TEST(Element)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for Element

Element*	e;
CHECK(Element() throw())
	e = new Element;
	TEST_NOT_EQUAL(e, 0)
RESULT

CHECK(~Element() throw())
	delete e;
RESULT

CHECK(Element(const Element& element) throw())
	Element* e1 = new Element;
	e1->setName("testname");
	Element* e2 = new Element(*e1);
	TEST_NOT_EQUAL(e2, 0)
	if (e2 != 0)
	{
		TEST_EQUAL(e2->getName(), "testname")
		delete e2;
	}
	delete e1;
RESULT


CHECK(Element(const String& name, const String& symbol, Group group, Period period, AtomicNumber atomic_umber, float atomic_weight, float atomic_radius, float covalent_radius, float van_der_waals_radius, float electronegativity) throw())

	Element static_element("Aluminum", "Al", 13, 3, 13, 26.981539,  1.43,  1.25, 2.05, true, 1.61);
	TEST_EQUAL(static_element.getName(), "Aluminum")
	TEST_EQUAL(static_element.getSymbol(), "Al")
	TEST_EQUAL(static_element.getGroup(), 13)
	TEST_EQUAL(static_element.getPeriod(), 3)
	TEST_EQUAL(static_element.getAtomicNumber(), 13)
	TEST_REAL_EQUAL(static_element.getAtomicWeight(), 26.9815390)
	TEST_REAL_EQUAL(static_element.getAtomicRadius(), 1.43)
	TEST_REAL_EQUAL(static_element.getCovalentRadius(), 1.25)
	TEST_REAL_EQUAL(static_element.getVanDerWaalsRadius(), 2.05)
	TEST_EQUAL(static_element.isMetal(), true)
	TEST_REAL_EQUAL(static_element.getElectronegativity(), 1.61)

	Element* e1 = new Element("e1", "id", 2, 3, 25, 25.0, 2.0, 3.0, 4.0, false, 5.0);
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
		TEST_EQUAL(e1->isMetal(), false)
		TEST_EQUAL(e1->getElectronegativity(), 5.0)
	}
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
		TEST_EQUAL(e2->isMetal(), false)
		TEST_EQUAL(e2->getElectronegativity(), 5.0)
		delete e2;
	}
	delete e1;
RESULT

Element e1;
Element e2;
Element e3;

CHECK(void setName(const String& name) throw())
	e1.setName("e1");
RESULT

CHECK(const String& getName() const throw())
	TEST_EQUAL(e1.getName(), "e1")
	TEST_EQUAL(e2.getName(), "Unknown")
RESULT

CHECK(void setSymbol(const String& symbol) throw())
	e1.setSymbol("s");
RESULT

CHECK(const String& getSymbol() const throw())
	TEST_EQUAL(e1.getSymbol(), "s")
	TEST_EQUAL(e2.getSymbol(), "?")
RESULT

CHECK(void setGroup(Group group) throw())
	e1.setGroup(2);
RESULT

CHECK(Group getGroup() const throw())
	TEST_EQUAL(e1.getGroup(), 2)
	TEST_EQUAL(e2.getGroup(), 0)
RESULT

CHECK(void setPeriod(Period period) throw())
	e1.setPeriod(3);
RESULT

CHECK(Period getPeriod() const throw())
	TEST_EQUAL(e1.getPeriod(), 3)
	TEST_EQUAL(e2.getPeriod(), 0)
RESULT

CHECK(void setAtomicNumber(AtomicNumber atomic_number) throw())
	e1.setAtomicNumber(4);
RESULT

CHECK(AtomicNumber getAtomicNumber() const throw())
	TEST_EQUAL(e1.getAtomicNumber(), 4)
	TEST_EQUAL(e2.getAtomicNumber(), 0)
RESULT

CHECK(void setAtomicWeight(float atomic_weight) throw())
	e1.setAtomicWeight(5.0);
RESULT

CHECK(float getAtomicWeight() const throw())
	TEST_EQUAL(e1.getAtomicWeight(), 5.0)
	TEST_EQUAL(e2.getAtomicWeight(), 0.0)
RESULT

CHECK(void setAtomicRadius(float atomic_radius) throw())
	e1.setAtomicRadius(6.0);
RESULT

CHECK(float getAtomicRadius() const throw())
	TEST_EQUAL(e1.getAtomicRadius(), 6.0)
	TEST_EQUAL(e2.getAtomicRadius(), 0.0)
RESULT

CHECK(void setCovalentRadius(float covalent_radius) throw())
	e1.setCovalentRadius(7.0);
RESULT

CHECK(float getCovalentRadius() const throw())
	TEST_EQUAL(e1.getCovalentRadius(), 7.0)
	TEST_EQUAL(e2.getCovalentRadius(), 0.0)
RESULT

CHECK(void setVanDerWaalsRadius(float van_der_waals_radius) throw())
	e1.setVanDerWaalsRadius(8.0);
RESULT

CHECK(float getVanDerWaalsRadius() const throw())
	TEST_EQUAL(e1.getVanDerWaalsRadius(), 8.0)
	TEST_EQUAL(e2.getVanDerWaalsRadius(), 0.0)
RESULT

CHECK(void setElectronegativity(float electronegativity) throw())
	e1.setElectronegativity(9.0);
RESULT

CHECK(float getElectronegativity() const throw())
	TEST_EQUAL(e1.getElectronegativity(), 9.0)
	TEST_EQUAL(e2.getElectronegativity(), 0.0)
RESULT

CHECK(bool operator == (const Element& element) const throw())
	TEST_EQUAL(e1 == e2, false)
	TEST_EQUAL(e3 == e2, true)
RESULT

CHECK(bool operator != (const Element& element) const throw())
	TEST_EQUAL(e3 != e1, true)
	TEST_EQUAL(e3 != e2, false)
RESULT

CHECK(bool operator < (const Element& element) const throw())
	TEST_EQUAL(e2 < e1, true)
	TEST_EQUAL(e2 < e3, false)
RESULT

CHECK(bool operator <= (const Element& element) const throw())
	TEST_EQUAL(e1 <= e2, false)
	TEST_EQUAL(e3 <= e2, true)
	TEST_EQUAL(e2 <= e1, true)
RESULT

CHECK(bool operator >= (const Element& element) const throw())
	TEST_EQUAL(e2 >= e1, false)
	TEST_EQUAL(e1 >= e2, true)
	TEST_EQUAL(e2 >= e2, true)
RESULT

CHECK(bool operator >(const Element& element) const throw())
	TEST_EQUAL(e2 > e1, false)
	TEST_EQUAL(e1 > e2, true)
	TEST_EQUAL(e2 > e3, false)
RESULT

CHECK(bool isUnknown() const throw())
	TEST_EQUAL(e1.isUnknown(), false)
	TEST_EQUAL(e2.isUnknown(), true)
RESULT

CHECK(BALL_CREATE(Element))
  // Not to be tested
RESULT

CHECK(Element& operator = (const Element& element) throw())
	Element rhs1("Aluminum", "Al", 13, 3, 13, 26.981539,  1.43,  1.25, 2.05, true, 1.61);

	Element el;
	el = rhs1;

	TEST_EQUAL(el.getName(), "Aluminum")
	TEST_EQUAL(el.getSymbol(), "Al")
	TEST_EQUAL(el.getGroup(), 13)
	TEST_EQUAL(el.getPeriod(), 3)
	TEST_EQUAL(el.getAtomicNumber(), 13)
	TEST_REAL_EQUAL(el.getAtomicWeight(), 26.9815390)
	TEST_REAL_EQUAL(el.getAtomicRadius(), 1.43)
	TEST_REAL_EQUAL(el.getCovalentRadius(), 1.25)
	TEST_REAL_EQUAL(el.getVanDerWaalsRadius(), 2.05)
	TEST_EQUAL(el.isMetal(), true)
	TEST_REAL_EQUAL(el.getElectronegativity(), 1.61)

	Element rhs2("ALUMINUM", "lA", 31, 5, 26, 27.333333,  2.45,  7.89, 1.12, true, 3.67);
	el = rhs2;

	TEST_EQUAL(el.getName(), rhs2.getName())
	TEST_EQUAL(el.getSymbol(), rhs2.getSymbol())
	TEST_EQUAL(el.getGroup(), rhs2.getGroup())
	TEST_EQUAL(el.getPeriod(), rhs2.getPeriod())
	TEST_EQUAL(el.getAtomicNumber(), rhs2.getAtomicNumber())
	TEST_EQUAL(el.getAtomicWeight(), rhs2.getAtomicWeight())
	TEST_EQUAL(el.getAtomicRadius(), rhs2.getAtomicRadius())
	TEST_EQUAL(el.getCovalentRadius(), rhs2.getCovalentRadius())
	TEST_EQUAL(el.getVanDerWaalsRadius(), rhs2.getVanDerWaalsRadius())
	TEST_EQUAL(el.isMetal(), rhs2.isMetal())
	TEST_EQUAL(el.getElectronegativity(), rhs2.getElectronegativity())
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(friend std::ostream& operator << (std::ostream& s, const Element& element) throw())
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << e1;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(PTE_test.txt))
RESULT


// test for PTE_:

CHECK(static Element& getElement(Position position) throw())
	TEST_EQUAL(PTE.getElement(13).getName(), "Aluminum")
	TEST_EQUAL(PTE.getElement(13).getSymbol(), "Al")
	TEST_EQUAL(PTE.getElement(13).getGroup(), 13)
	TEST_EQUAL(PTE.getElement(13).getPeriod(), 3)
	TEST_EQUAL(PTE.getElement(13).getAtomicNumber(), 13)
	TEST_REAL_EQUAL(PTE.getElement(13).getAtomicWeight(), 26.9815390)
	TEST_REAL_EQUAL(PTE.getElement(13).getAtomicRadius(), 1.43)
	TEST_REAL_EQUAL(PTE.getElement(13).getCovalentRadius(), 1.25)
	TEST_REAL_EQUAL(PTE.getElement(13).getVanDerWaalsRadius(), 2.05)
	TEST_EQUAL(PTE.getElement(13).isMetal(), true)
	TEST_REAL_EQUAL(PTE.getElement(13).getElectronegativity(), 1.61)
RESULT

CHECK(static Element& getElement(const String& symbol) throw())
	TEST_EQUAL(PTE["B"].getName(), "Boron")
RESULT

CHECK(Element& operator [] (const String& symbol) throw())
	PTE_ p(PTE);
	p["B"].setName("MyLovelyBoron2");
	TEST_EQUAL(p[Element::BORON].getName(), "MyLovelyBoron2")
RESULT

CHECK(Element& operator [] (Element::Symbol symbol) throw())
	PTE_ p(PTE);
	p[Element::B].setName("MyLovelyBoron3");
	TEST_EQUAL(p[Element::BORON].getName(), "MyLovelyBoron3")
RESULT

CHECK(Element& operator [] (Position position) throw())
	PTE_ p(PTE);
	p[1].setName("MyLovelyHydrogen1");
	TEST_EQUAL(p[1].getName(), "MyLovelyHydrogen1")
RESULT

CHECK(static bool apply(UnaryProcessor<Element>& applicator) throw())
	ItemCollector<Element> myproc;
	PTE.apply(myproc);
	TEST_EQUAL(myproc.getSize(), 112)
	TEST_EQUAL(myproc.getPointer()->getName(), "Actinium")
RESULT

CHECK(PTE_& operator = (const PTE_& /*pte*/) throw())
	PTE_ p1, p2;
	p1 = p2;
RESULT

CHECK(bool operator == (const PTE_& pte) const throw())
	PTE_ p1, p2;
	TEST_EQUAL(p1 == p2, true)
RESULT

CHECK(void clear() throw())
	PTE_ p1;
	p1.clear();
RESULT

CHECK(BALL_CREATE(PTE_))
	PTE_* ptr = (PTE_*) PTE.create(true, false);
	TEST_EQUAL((*ptr)[13].getName(), "Aluminum")
	delete ptr;
RESULT

PTE_* p_ptr;
CHECK(PTE_() throw())
	p_ptr = new PTE_;
RESULT

CHECK(~PTE_() throw())
	delete p_ptr;
RESULT

CHECK(PTE_(const PTE_& pse) throw())
	PTE_ p(PTE);
	TEST_EQUAL(p[13].getName(), "Aluminum")
RESULT

CHECK(Element& operator [] (Element::Name name) throw())
	PTE_ p(PTE);
	p[Element::BORON].setName("MyLovelyBoron");
	TEST_EQUAL(p[Element::BORON].getName(), "MyLovelyBoron")
RESULT

CHECK(const Element& operator [] (Element::Name name) const throw())
	TEST_EQUAL(PTE[Element::BORON].getName(), "MyLovelyBoron")
RESULT

CHECK(const Element& operator [] (Element::Symbol symbol) const throw())
	TEST_EQUAL(PTE[Element::H].getName(), "MyLovelyHydrogen1")
RESULT

CHECK(const Element& operator [] (Position position) const throw())
	TEST_EQUAL(PTE[1].getName(), "MyLovelyHydrogen1")
	TEST_EQUAL(PTE[113].isUnknown(), true)
RESULT

CHECK(const Element& operator [] (const String& symbol) const throw())
	TEST_EQUAL(PTE[Element::H].getName(), "MyLovelyHydrogen1")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
