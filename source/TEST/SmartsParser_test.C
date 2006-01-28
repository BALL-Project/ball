// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SmartsParser_test.C,v 1.1 2005/11/17 14:36:26 bertsch Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/smartsParser.h>

#include <fstream>

///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(SmartsParser, "$Id: SmartsParser_test.C,v 1.1 2005/11/17 14:36:26 bertsch Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


SmartsParser * sp = 0;
CHECK(SmartsParser())
	sp = new SmartsParser();
	TEST_NOT_EQUAL(sp, 0)
RESULT

CHECK(~SmartsParser())
	delete sp;
RESULT

CHECK(parse(const String& s))
	ifstream is("data/SmartsParser_test.txt");
	sp = new SmartsParser();
	String line;
	while (line.getline(is))
	{
		String smarts(line);
		smarts.trim();
		sp->parse(smarts);
	}

	TEST_EXCEPTION(Exception::ParseError, sp->parse("["))
	TEST_EXCEPTION(Exception::ParseError, sp->parse("E"))
	TEST_EXCEPTION(Exception::ParseError, sp->parse("[]"))
	TEST_EXCEPTION(Exception::ParseError, sp->parse("C(CO"))
	TEST_EXCEPTION(Exception::ParseError, sp->parse("[C,]"))
	TEST_EXCEPTION(Exception::ParseError, sp->parse("[Cl*]"))
	delete sp;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
