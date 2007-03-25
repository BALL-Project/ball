// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
<<<<<<< SmartsMatcher_test.C
// $Id: SmartsMatcher_test.C,v 1.4.8.1 2007/03/25 21:48:59 oliver Exp $
=======
// $Id: SmartsMatcher_test.C,v 1.4.8.1 2007/03/25 21:48:59 oliver Exp $
>>>>>>> 1.1.2.1
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/FORMAT/SDFile.h>

#include <fstream>
#include <vector>
#include <set>

///////////////////////////

using namespace BALL;
using namespace std;

<<<<<<< SmartsMatcher_test.C
START_TEST(SmartsMatcher, "$Id: SmartsMatcher_test.C,v 1.4.8.1 2007/03/25 21:48:59 oliver Exp $")
=======
START_TEST(SmartsMatcher, "$Id: SmartsMatcher_test.C,v 1.4.8.1 2007/03/25 21:48:59 oliver Exp $")
>>>>>>> 1.1.2.1

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
<<<<<<< SmartsMatcher_test.C
		
		vector<set<const Atom*> > matchings;
		sm->match(matchings, *s.getMolecule(0), split[2]);
=======

		vector<HashSet<const Atom*> > matchings;
		sm->match(matchings, *s.getMolecule(0), split[2]);
>>>>>>> 1.1.2.1
		TEST_EQUAL(matchings.size(), split[0].toUnsignedInt())
	}

	delete sm;	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
