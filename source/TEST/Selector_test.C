// $Id: Selector_test.C,v 1.2 2001/07/10 16:35:07 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/selector.h>

///////////////////////////

START_TEST(class_name, "$Id: Selector_test.C,v 1.2 2001/07/10 16:35:07 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

Selector* ptr;

CHECK(Selector::Selector() throw())
	ptr = new Selector;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(Selector::~Selector() throw())
	delete ptr;
RESULT


CHECK(Selector::Selector(const Selector& selector) throw())
	Selector s1;
RESULT


CHECK(Selector::Selector(const String& expression_string) throw())
  //BAUSTELLE
RESULT


CHECK(Selector::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(Selector::start() throw())
  //BAUSTELLE
RESULT


CHECK(Selector::getNumberOfSelectedAtoms() const  throw())
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
