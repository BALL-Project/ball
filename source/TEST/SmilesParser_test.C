// $Id: SmilesParser_test.C,v 1.1 2002/01/08 00:47:16 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/smilesParser.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(SmilesParser, "$Id: SmilesParser_test.C,v 1.1 2002/01/08 00:47:16 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SmilesParser sp;
CHECK(C)
	sp.parse("C");
	TEST_EQUAL(sp.getSystem().countAtoms(), 5)

	ABORT_IF(sp.getSystem().countAtoms() != 5)
	const Atom& atom = *sp.getSystem().getAtom(0);
	TEST_EQUAL(atom.getElement(), PTE[Element::C])	
	TEST_EQUAL(atom.countBonds(), 4)	
RESULT											

CHECK([C])
	sp.parse("[C]");
	TEST_EQUAL(sp.getSystem().countAtoms(), 1)

	ABORT_IF(sp.getSystem().countAtoms() != 1)
	const Atom& atom = *sp.getSystem().getAtom(0);
	TEST_EQUAL(atom.getElement(), PTE[Element::C])	
	TEST_EQUAL(atom.countBonds(), 0)	
RESULT											

CHECK(parse errors)
	TEST_EXCEPTION(Exception::ParseError, sp.parse("X"))
	TEST_EXCEPTION(Exception::ParseError, sp.parse("+"))
	TEST_EXCEPTION(Exception::ParseError, sp.parse("+4"))
	TEST_EXCEPTION(Exception::ParseError, sp.parse("C+"))
RESULT
	

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
