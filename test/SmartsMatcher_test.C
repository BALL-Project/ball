// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(SmartsMatcher)

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

	SDFile infile(BALL_TEST_DATA_PATH(SmartsMatcher_test.sdf));
	System s;
	infile >> s;
	infile.close();

	sm = new SmartsMatcher();

	ifstream is(BALL_TEST_DATA_PATH(SmartsMatcher_test.txt));
	String line;

	while (line.getline(is))
	{
		String tmp(line);
		tmp.trim();
		vector<String> split;
		tmp.split(split, " ");
		
		vector<set<const Atom*> > matchings;
		sm->match(matchings, *s.getMolecule(0), split[2]);
		TEST_EQUAL(matchings.size(), split[0].toUnsignedInt())
	}

	delete sm;	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
