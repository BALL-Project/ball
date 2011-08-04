// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/chainBuilder.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(ChainBuilder)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.0001)

ChainBuilder* cb; 
CHECK(ChainBuilder())
  cb = new ChainBuilder;
	TEST_NOT_EQUAL(cb, 0)
RESULT


ChainBuilder cb_2;
System molecule_sys;
MOLFile infile(BALL_TEST_DATA_PATH(input_ChainBuilder_test.mol));
infile >> molecule_sys;
infile.close();

CHECK(vector<vector<Atom*> > buildChains(System& molecule_sys))

	Size i = 0;
	
	for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
	{
		switch (i)
		{
			case 1:
			{
				atom_it -> setProperty(SDGenerator::CORE_CHAIN);
				 break;
			}
			case 2:
			{
				atom_it -> setProperty(SDGenerator::CORE_CHAIN);
				 break;
			}
			case 3:
			{
				atom_it -> setProperty(SDGenerator::CORE_CHAIN);
				 break;
			}
			case 6:
			{
				atom_it -> setProperty(SDGenerator::CORE_CHAIN);
				 break;
			}
			case 7:
			{
				atom_it -> setProperty(SDGenerator::CORE_CHAIN);
				 break;
			}
		}

		i++;
	}

	vector<vector<Atom*> > chains = cb_2.buildChains(molecule_sys);
	
	// 	i =	     	 7  	 6  	 3  	 1  	 2
	//	ATOM-NAME 	[8]	[7]	[4]	[2]	[3]
	//	chains[0][...]	0	 1	 2	 3	 4


	i = 0;
	
	for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
	{
		switch (i)
		{
			case 1:
			{
				TEST_EQUAL(&*chains[0][3], &*atom_it)
				 break;
			}
			case 2:
			{
				TEST_EQUAL(&*chains[0][4], &*atom_it)
				 break;
			}
			case 3:
			{
				TEST_EQUAL(&*chains[0][2], &*atom_it)
				 break;
			}
			case 6:
			{
				TEST_EQUAL(&*chains[0][1], &*atom_it)
				 break;
			}
			case 7:
			{
				TEST_EQUAL(&*chains[0][0], &*atom_it)
				 break;
			}
		}

		i++;
	}
	
RESULT


CHECK(~ChainBuilder())

	delete cb;

RESULT


END_TEST
