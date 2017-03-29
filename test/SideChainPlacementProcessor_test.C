// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/sideChainPlacementProcessor.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

using namespace BALL;

START_TEST(SideChainPlacementProcessor)

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

SideChainPlacementProcessor* scpp;
CHECK(SideChainPlacementProcessor())
  scpp = new SideChainPlacementProcessor();
	TEST_NOT_EQUAL(scpp,0)
RESULT

CHECK(~SideChainPlacementProcessor())
	delete scpp;
RESULT

CHECK(setDefaultOptions())	
	SideChainPlacementProcessor scpp;
	scpp.setDefaultOptions();	

	TEST_EQUAL(scpp.options.get(SideChainPlacementProcessor::Option::METHOD),
														 SideChainPlacementProcessor::Default::METHOD)
	
	//if no SCWRL installed, no path available!
// 	TEST_EQUAL(scpp.options.get(SideChainPlacementProcessor::Option::SCWRL_BINARY_PATH),
// 														 SideChainPlacementProcessor::Default::SCWRL_BINARY_PATH)

	TEST_EQUAL(scpp.options.getBool(SideChainPlacementProcessor::Option::MUTATE_SELECTED_SIDE_CHAINS),
																 SideChainPlacementProcessor::Default::MUTATE_SELECTED_SIDE_CHAINS)

	TEST_EQUAL(scpp.options.get(SideChainPlacementProcessor::Option::SCWRL_INPUT_FILE),
														 SideChainPlacementProcessor::Default::SCWRL_INPUT_FILE)

	TEST_EQUAL(scpp.options.get(SideChainPlacementProcessor::Option::SCWRL_SEQUENCE_FILE),
														 SideChainPlacementProcessor::Default::SCWRL_SEQUENCE_FILE)

	TEST_EQUAL(scpp.options.get(SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE),
														 SideChainPlacementProcessor::Default::SCWRL_OUTPUT_FILE)
RESULT


CHECK( setMutations() / getMutations() )
	SideChainPlacementProcessor scpp;
	TEST_EQUAL(scpp.getMutations(), "")
	scpp.setMutations("CCHHPPTTAA");
	TEST_EQUAL(scpp.getMutations(), "CCHHPPTTAA")
RESULT


CHECK( applied on empty System )
	SideChainPlacementProcessor scpp;		
	scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_INPUT_FILE, BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_input_1.pdb));
	scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE, BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_output_1.pdb));

	System sys;	
	sys.apply(scpp); 
RESULT


CHECK( operator() apply to a system )
	SideChainPlacementProcessor scpp;
	//TODO shouldn't we test the input file as well?
	//scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_INPUT_FILE, 
  //                 BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_input_1.pdb));
	//scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_SEQUENCE_FILE,
	//                 BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_sequence_1.dat));
	scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE, 
	                 BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_output_1.pdb));

	System sys;	
	PDBFile mol(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test.pdb), std::ios::in); 
	mol >> sys;

	sys.apply(scpp);
	
	// store the result in a temporary file
	String temp; 
  File::createTemporaryFilename(temp);
	PDBFile PDB_temp(temp, std::ios::out);	
	PDB_temp << sys;
	PDB_temp.close();

	// get the precomputed result 	
	PDBFile PDB_pre( BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_output_pre_1.pdb), std::ios::out);	

	// compare
	//////// does not work like this: TEST_EQUAL(PDB_temp==PDB_pre, true) //TODO	

	// remove the temp file
	File::remove(temp); 
RESULT

///////////////////TODO////////////////////////////

CHECK(apply to a protein)
	SideChainPlacementProcessor scpp;	

	System sys;	
	PDBFile mol(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test.pdb), std::ios::in); 
	mol >> sys;
	Protein* prot = sys.getProtein(0);
RESULT

CHECK(apply to a chain)
	SideChainPlacementProcessor scpp;	

	System sys;	
	PDBFile mol(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test.pdb), std::ios::in); 
	mol >> sys;
	Chain* chain3 = (sys.getProtein(0)->getChain(2));

RESULT


CHECK( apply to a selection)	
	SideChainPlacementProcessor scpp;	

	System sys;	
	PDBFile mol(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test.pdb), std::ios::in); 
	mol >> sys;
	Residue* res = (sys.getProtein(0)->getChain(1))->getResidue(0);
	res->select();
	Chain* chain2 = (sys.getProtein(0)->getChain(1));
	chain2->select();
RESULT

CHECK(mutate)	
	SideChainPlacementProcessor scpp;
 	scpp.options.set(SideChainPlacementProcessor::Option::MUTATE_SELECTED_SIDE_CHAINS, true);
	//TODO   scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE, BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_output_mut1.pdb));

	System sys;	
	PDBFile mol(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test.pdb), std::ios::in); 
	mol >> sys;
	
	//check if SCWRL path is set and the required file exists
	try {
		File file(scpp.options.get(SideChainPlacementProcessor::Option::SCWRL_BINARY_PATH));
		if(file.isExecutable())
		{
			String mutated_seq = "arCdcCeg";
			scpp.setMutations(mutated_seq);

			printf("blubblubb\n");
			sys.apply(scpp); //TODO Are both Cysteins set correctly?
			TEST_EQUAL(sys.getProtein(0)->getChain(0)->getResidue(2)->getName(), "CYS")
			TEST_EQUAL(sys.getProtein(0)->getChain(1)->getResidue(1)->getName(), "CYS")
			scpp.clear();

			scpp.options.set(SideChainPlacementProcessor::Option::MUTATE_SELECTED_SIDE_CHAINS, true);
			scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE, 
											 BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_output_mut2.pdb));

			System sys2;	
			PDBFile mol2(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test.pdb), std::ios::in); 
			mol2 >> sys2;
			mutated_seq = "arCd";
			scpp.setMutations(mutated_seq);
			Chain* chain1 = (sys.getProtein(0)->getChain(0));
			chain1->apply(scpp);
			
			sys2.apply(scpp);
			TEST_EQUAL(sys.getProtein(0)->getChain(0)->getResidue(2)->getName(), "CYS")
			TEST_EQUAL(sys.getProtein(0)->getChain(1)->getResidue(1)->getName(), "GLN")
		}
	} 
	catch (...) 
	{
		std::cout << "Could not find Scwrl binary" << std::endl;
	}

RESULT

CHECK(check energy) //TODO
/*	SideChainPlacementProcessor scpp;
	scpp.options.set(SideChainPlacementProcessor::Option::SCWRL_OUTPUT_FILE, BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_output_AA.pdb));

	System sys;	
	PDBFile mol(BALL_TEST_DATA_PATH(SideChainPlacementProcessor_test_AA.pdb), std::ios::in); 
	mol >> sys;
	
	// energy before
	AmberFF amber_ff;
	amber_ff.setup(sys);
	float before =  amber_ff.updateEnergy();

	sys.apply(scpp);
	
	// energy after	
	amber_ff.setup(sys);
	float after =  amber_ff.updateEnergy();
	TEST_REAL_EQUAL(before > after)
*/
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
