// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/HBondProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/chainIterator.h>
///////////////////////////

START_TEST(HBondProcessor)

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HBondProcessor* hbp = 0;
CHECK(HBondProcessor())
  hbp = new HBondProcessor();
	TEST_NOT_EQUAL(hbp, 0)
RESULT
			
CHECK(~HBondProcessor())
	delete hbp;
RESULT

CHECK(setDefaultOptions())
	HBondProcessor hbond_pro;
	hbond_pro.setDefaultOptions();	

	TEST_EQUAL(hbond_pro.options.get(HBondProcessor::Option::PREDICTION_METHOD),
											             HBondProcessor::Default::PREDICTION_METHOD)

	TEST_EQUAL(hbond_pro.options.getBool(HBondProcessor::Option::ADD_HBONDS),
											             HBondProcessor::Default::ADD_HBONDS)		

	TEST_EQUAL(hbond_pro.options.getReal(HBondProcessor::Option::KABSCH_SANDER_ENERGY_CUTOFF),
											             HBondProcessor::Default::KABSCH_SANDER_ENERGY_CUTOFF)		

RESULT

		
CHECK( PredictionMethod::KABSCH_SANDER)
	// method KABSCH_SANDER
	HBondProcessor hbond_pro;  
  hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);
	hbond_pro.options.set(HBondProcessor::Option::ADD_HBONDS, true);

	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;

  sys.apply(hbond_pro);
	std::vector<std::vector<Position> > backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	std::vector<HBondProcessor::HBond>  HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 181)
  TEST_EQUAL(backbone_HBonds.size(), 439)
	
	// test reapply	
	sys.apply(hbond_pro);
	HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 181)	
  TEST_EQUAL(backbone_HBonds.size(), 439)
	
RESULT


CHECK(Option::ADD_HBONDS)
	HBondProcessor hbond_pro;
  hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD,
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);
	hbond_pro.options.set(HBondProcessor::Option::ADD_HBONDS, true);

	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;

  sys.apply(hbond_pro);
	std::vector<std::vector<Position> > backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	std::vector<HBondProcessor::HBond>  HBonds          = hbond_pro.getHBonds();

  TEST_EQUAL(backbone_HBonds.size(), 439)
  TEST_EQUAL(HBonds.size(), 181)

	hbond_pro.options.set(HBondProcessor::Option::ADD_HBONDS, false);
	sys.apply(hbond_pro);
	backbone_HBonds = hbond_pro.getBackboneHBondPattern(); 
	HBonds = hbond_pro.getHBonds(); 

	TEST_EQUAL(backbone_HBonds.size(), 439)
  TEST_EQUAL(HBonds.size(), 181)	
	bool found_h_bond = false;
	
	AtomIterator at_it;
	Atom::BondIterator bond_it;
	BALL_FOREACH_BOND(sys, at_it, bond_it)
	{
		if (bond_it->getType() == Bond::TYPE__HYDROGEN)
			found_h_bond=true;
	}
 
	TEST_EQUAL(found_h_bond, false)
RESULT


CHECK(Option::KABSCH_SANDER_ENERGY_CUTOFF)
	HBondProcessor hbond_pro;  
  hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);
  // Default energy is -0.5
	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;

  sys.apply(hbond_pro);
	std::vector<HBondProcessor::HBond> HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 181)

	hbond_pro.options.set(HBondProcessor::Option::KABSCH_SANDER_ENERGY_CUTOFF, -1.5); 

	sys.apply(hbond_pro);
	HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 114)
	
RESULT


CHECK( PredictionMethod::WISHART_ET_AL)	
	HBondProcessor hbond_pro;  
	hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::WISHART_ET_AL);

 	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;

 	sys.apply(hbond_pro);
	std::vector<HBondProcessor::HBond> HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 104)

	// test reapply	
	sys.apply(hbond_pro);	
	HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 104)

RESULT

CHECK( apply to protein)
	HBondProcessor hbond_pro;  
  hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);
	hbond_pro.options.set(HBondProcessor::Option::ADD_HBONDS, true);

	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;
  sys.getProtein(0)->apply(hbond_pro);

	std::vector<std::vector<Position> > backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	std::vector<HBondProcessor::HBond>  HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 181)
  TEST_EQUAL(backbone_HBonds.size(), 439)

RESULT


CHECK( apply to chain)
	HBondProcessor hbond_pro;  
  hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);
	hbond_pro.options.set(HBondProcessor::Option::ADD_HBONDS, true);

	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;
	ChainIterator cit = sys.beginChain();
  cit->apply(hbond_pro);

	std::vector<std::vector<Position> > backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	std::vector<HBondProcessor::HBond>  HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 144)
  TEST_EQUAL(backbone_HBonds.size(), 223)
	
	hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::WISHART_ET_AL);
 	cit->apply(hbond_pro);

	backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 72)
  TEST_EQUAL(backbone_HBonds.size(), 223)

	cit++; 
	hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);

 	cit->apply(hbond_pro);
	backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 31)
  TEST_EQUAL(backbone_HBonds.size(), 58)
	
	hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::WISHART_ET_AL);
 	cit->apply(hbond_pro);

	backbone_HBonds = hbond_pro.getBackboneHBondPattern();
	HBonds = hbond_pro.getHBonds();
  TEST_EQUAL(HBonds.size(), 13)
  TEST_EQUAL(backbone_HBonds.size(), 58)

RESULT


CHECK(getHBonds() PredictionMethod::WISHART_ET_AL)
	HBondProcessor hbond_pro;  
	hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::WISHART_ET_AL);

 	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;

 	sys.apply(hbond_pro);
	std::vector<HBondProcessor::HBond> HBonds = hbond_pro.getHBonds();
	TEST_EQUAL(HBonds.size(), 104)
	if (HBonds.size()>8)
	{
		TEST_REAL_EQUAL(HBonds[0].getLength(), 2.02534);
		TEST_EQUAL(HBonds[0].getDonor()->getFullName(), "LYS:HA");
		TEST_EQUAL(HBonds[0].getAcceptor()->getFullName(), "CYS-S:O");
		TEST_REAL_EQUAL(HBonds[1].getLength(), 2.08096);
		TEST_EQUAL(HBonds[1].getDonor()->getFullName(), "GLY:1HA");
		TEST_EQUAL(HBonds[1].getAcceptor()->getFullName(), "THR:O");
		TEST_REAL_EQUAL(HBonds[2].getLength(), 2.09803);
		TEST_EQUAL(HBonds[2].getDonor()->getFullName(), "LYS:HA");
		TEST_EQUAL(HBonds[2].getAcceptor()->getFullName(), "VAL:O");
		TEST_REAL_EQUAL(HBonds[3].getLength(), 2.12439);
		TEST_EQUAL(HBonds[3].getDonor()->getFullName(), "ASN:HA");
		TEST_EQUAL(HBonds[3].getAcceptor()->getFullName(), "ASP:O");
		TEST_REAL_EQUAL(HBonds[4].getLength(), 2.13134);
		TEST_EQUAL(HBonds[4].getDonor()->getFullName(), "PRO:HA");
		TEST_EQUAL(HBonds[4].getAcceptor()->getFullName(), "THR:O");
		TEST_REAL_EQUAL(HBonds[5].getLength(), 2.14385);
		TEST_EQUAL(HBonds[5].getDonor()->getFullName(), "THR:HA");
		TEST_EQUAL(HBonds[5].getAcceptor()->getFullName(), "ARG:O");	
		TEST_REAL_EQUAL(HBonds[6].getLength(), 2.21164);
		TEST_EQUAL(HBonds[6].getDonor()->getFullName(), "TYR:HA");
		TEST_EQUAL(HBonds[6].getAcceptor()->getFullName(), "GLN:O");	
		TEST_REAL_EQUAL(HBonds[7].getLength(), 2.24834);
		TEST_EQUAL(HBonds[7].getDonor()->getFullName(), "GLY:2HA");
		TEST_EQUAL(HBonds[7].getAcceptor()->getFullName(), "CYS-S:O");
	}
RESULT


CHECK(getHBonds() PredictionMethod::KABSCH_SANDER)
	HBondProcessor hbond_pro;  
	hbond_pro.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
			                  HBondProcessor::PredictionMethod::KABSCH_SANDER);

 	System sys;
	PDBFile mol(BALL_TEST_DATA_PATH(2ptc_H.pdb), std::ios::in);
	mol >> sys;

 	sys.apply(hbond_pro);
	std::vector<HBondProcessor::HBond> HBonds = hbond_pro.getHBonds();
	TEST_EQUAL(HBonds.size(), 181)
	if (HBonds.size()>8)
	{
		TEST_REAL_EQUAL(HBonds[0].getLength(), 4.27251);
		TEST_EQUAL(HBonds[0].getDonor()->getFullName(), "GLY:N");
		TEST_EQUAL(HBonds[0].getAcceptor()->getFullName(), "ILE-N:O");
		TEST_REAL_EQUAL(HBonds[1].getLength(), 2.96266);
		TEST_EQUAL(HBonds[1].getDonor()->getFullName(), "ASP:N");
		TEST_EQUAL(HBonds[1].getAcceptor()->getFullName(), "VAL:O");
		TEST_REAL_EQUAL(HBonds[2].getLength(), 2.81219);
		TEST_EQUAL(HBonds[2].getDonor()->getFullName(), "CYS-S:N");
		TEST_EQUAL(HBonds[2].getAcceptor()->getFullName(), "TYR:O");
		TEST_REAL_EQUAL(HBonds[3].getLength(), 3.55726);
		TEST_EQUAL(HBonds[3].getDonor()->getFullName(), "THR:N");
		TEST_EQUAL(HBonds[3].getAcceptor()->getFullName(), "GLY:O");
		TEST_REAL_EQUAL(HBonds[4].getLength(), 3.79745);
		TEST_EQUAL(HBonds[4].getDonor()->getFullName(), "PRO:N");
		TEST_EQUAL(HBonds[4].getAcceptor()->getFullName(), "THR:O");
		TEST_REAL_EQUAL(HBonds[5].getLength(), 3.15854);
		TEST_EQUAL(HBonds[5].getDonor()->getFullName(), "GLN:N");
		TEST_EQUAL(HBonds[5].getAcceptor()->getFullName(), "VAL:O");	
		TEST_REAL_EQUAL(HBonds[6].getLength(), 3.13868);
		TEST_EQUAL(HBonds[6].getDonor()->getFullName(), "ALA:N");
		TEST_EQUAL(HBonds[6].getAcceptor()->getFullName(), "PRO:O");	
		TEST_REAL_EQUAL(HBonds[7].getLength(), 2.83116);
		TEST_EQUAL(HBonds[7].getDonor()->getFullName(), "LEU:N");
		TEST_EQUAL(HBonds[7].getAcceptor()->getFullName(), "TYR:O");
	}

RESULT



END_TEST
