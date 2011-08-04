// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/smilesParser.h>
#include <BALL/KERNEL/PTE.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

///////////////////////////

using namespace BALL;

typedef std::map<String, Size> Formula;
typedef std::pair<String, Formula> Line;

std::ostream& operator << (std::ostream& os, const std::pair<BALL::String, Size>& p)
{
	return os << p.first << p.second;
}

std::ostream& operator << (std::ostream& os, const std::map<String, Size>& f)
{
	std::map<String, Size>::const_iterator it(f.begin());
	for (; it != f.end(); it++)
	{
		os << it->first << "/" << it->second << " ";
	}
	return os;
}

Formula computeFormula(const System& s)
{
	Formula f;
	AtomConstIterator atom(s.beginAtom());
	for (; atom != s.endAtom(); ++atom)
	{
		f[atom->getElement().getSymbol()]++;
	}
	return f;
}

Line readSmilesLine(std::istream& is) 
{
	String line;
	line.getline(is);
	std::pair<String, Formula> s;	

	if (line != "")
	{
		s.first = line.getField(0);
		for (Position i = 1; i < line.countFields(); i += 2)
		{
			s.second[line.getField(i)] = line.getField(i + 1).toUnsignedInt();
		}
	}
	return s;
}

START_TEST(SmilesParser)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

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
	
CHECK(example library)
	std::ifstream is(BALL_TEST_DATA_PATH(SmilesParser_test.txt));
	std::pair<String, Formula> smiles;
	while (is.good())
	{
		smiles = readSmilesLine(is);
		STATUS(smiles.first << "/" << smiles.second)
		if (smiles.first == "") 
		{
			break;
		}
		sp.parse(smiles.first);
		TEST_EQUAL(computeFormula(sp.getSystem()), smiles.second)
	}
	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
