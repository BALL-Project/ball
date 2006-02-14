// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SmartsMatcher_test.C,v 1.2.4.1 2006/02/14 15:03:11 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/DATATYPE/hashSet.h>

#include <fstream>
#include <vector>

///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(SmartsMatcher, "$Id: SmartsMatcher_test.C,v 1.2.4.1 2006/02/14 15:03:11 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

SmartsMatcher* sm = 0;

CHECK(SmartsMatcher())
	sm = new SmartsMatcher();
RESULT

CHECK(~SmartsMatcher())
	delete sm;
RESULT

CHECK(match(Molecule& mol, const String& smarts))

	SDFile infile("data/SmartsMatcher_test.sdf");
	System s;
	infile >> s;
	infile.close();

	sm = new SmartsMatcher();

	ifstream is("data/SmartsMatcher_test.txt");
	String line;

	while (line.getline(is))
	{
		String tmp(line);
		tmp.trim();
		vector<String> split;
		tmp.split(split, " ");

		vector<HashSet<const Atom*> > matchings = sm->match(*s.getMolecule(0), split[2]);
		TEST_EQUAL(matchings.size(), split[0].toUnsignedInt())
	}

	delete sm;	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
