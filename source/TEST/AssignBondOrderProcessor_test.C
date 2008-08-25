// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////
using namespace BALL;


void storeBondOrders(System& sys)
{
	AtomIterator a_it = sys.beginAtom();
	Atom::BondIterator b_it;
	BALL_FOREACH_BOND(sys, a_it, b_it)
	{
		b_it->setProperty("FORMER_ORDER", b_it->getOrder());
		b_it->setOrder(Bond::ORDER__UNKNOWN);
	}
}

void storeHBonds(System& sys)
{
	AtomIterator a_it = sys.beginAtom();
	Atom::BondIterator b_it;

	BALL_FOREACH_ATOM(sys, a_it)
	{
		int H_counter = 0;
		for (b_it=a_it->beginBond(); b_it != a_it->endBond(); b_it++)
		{
			if (b_it->getPartner(*a_it)->getElement() == PTE[Element::H])
				H_counter++;
		}
		a_it->setProperty("FORMER_NUM_HYD", H_counter);
	}
}


void deleteHBonds(System& sys)
{
	vector<Atom*> to_delete;
	AtomIterator a_it = sys.beginAtom();
	BALL_FOREACH_ATOM(sys, a_it)
	{
		if (a_it->getElement() == PTE[Element::H])
			to_delete.push_back(&*a_it);
	}
	for (Size i=0; i< to_delete.size(); i++)
		delete(to_delete[i]);
}


bool compareBondOrder(System& sys)
{
	bool all_bonds_equal=true;

	AtomIterator a_it = sys.beginAtom();
	Atom::BondIterator b_it;
	BALL_FOREACH_BOND(sys, a_it, b_it)
	{
		all_bonds_equal &= ( b_it->getProperty("FORMER_ORDER").getInt() == b_it->getOrder());
		/*if (b_it->getProperty("FORMER_ORDER").getInt() != b_it->getOrder())
			cout << b_it->getOrder()<<   " " << b_it->getProperty("FORMER_ORDER").getInt() << " " << b_it->getFirstAtom()->getFullName() << " " << b_it->getSecondAtom()->getFullName() << endl; */
	}
	return all_bonds_equal;
}


bool compareHBonds(System& sys)
{
	bool all_num_HYDs_equal=true;

	AtomIterator a_it = sys.beginAtom();
	Atom::BondIterator b_it;

	BALL_FOREACH_ATOM(sys, a_it)
	{
		int H_counter = 0;
		for (b_it=a_it->beginBond(); b_it != a_it->endBond(); b_it++)
		{
			if (b_it->getPartner(*a_it)->getElement() == PTE[Element::H])
				H_counter++;
		}
		if (a_it->getProperty("FORMER_NUM_HYD").getInt() != H_counter)
		{
			all_num_HYDs_equal = false;
		}
	}
	return all_num_HYDs_equal;
}



START_TEST(AssignBondOrderProcessor, "$Id:$")

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

AssignBondOrderProcessor* abop;
CHECK(AssignBondOrderProcessor())
  abop = new AssignBondOrderProcessor();
	TEST_NOT_EQUAL(abop,0)
RESULT


CHECK(~AssignBondOrderProcessor())
	delete abop;
RESULT

CHECK(getNumberOfBondOrdersSet())
  AssignBondOrderProcessor testbop;
	TEST_EQUAL(testbop.getNumberOfBondOrdersSet(), 0)
RESULT

CHECK(getNumberOfComputedSolutions())
  AssignBondOrderProcessor testbop;
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 0)
RESULT



CHECK(setDefaultOptions())
	AssignBondOrderProcessor testbop;
	testbop.setDefaultOptions();

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS),
												AssignBondOrderProcessor::Default::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS)

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS), 
	 											AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS)
	
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS),
												AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS)
	
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS),
												AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS)
	
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS),  
												AssignBondOrderProcessor::Default::OVERWRITE_SELECTED_BONDS)
		
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS),
												   AssignBondOrderProcessor::Default::ADD_HYDROGENS)

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::OVERWRITE_CHARGES),
												   AssignBondOrderProcessor::Default::OVERWRITE_CHARGES)
		
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::ASSIGN_CHARGES),
													 AssignBondOrderProcessor::Default::ASSIGN_CHARGES)
	
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS),
													 AssignBondOrderProcessor::Default::KEKULIZE_RINGS)	
		
	TEST_EQUAL(testbop.options.get(AssignBondOrderProcessor::Option::INIFile),
													 AssignBondOrderProcessor::Default::INIFile)		
		
	TEST_EQUAL(testbop.options.getInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER),
													 AssignBondOrderProcessor::Default::MAX_BOND_ORDER)

	TEST_EQUAL(testbop.options.getInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS),
				 							     AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS)

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS),
													 AssignBondOrderProcessor::Default::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS)

	TEST_EQUAL(testbop.options.get(AssignBondOrderProcessor::Option::ALGORITHM),
													 AssignBondOrderProcessor::Default::ALGORITHM)

	TEST_EQUAL(testbop.options.getReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING),
													 AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING)
RESULT


CHECK(evaluatePenalty(AtomContainer* ac))
  AssignBondOrderProcessor testbop;
	System sys;
	MOL2File mol_in("data/AssignBondOrderProcessor_test_AMHTAR01.mol2", std::ios::in);
	mol_in >> sys;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys),0. )
	
	System sys2;
	MOL2File mol_in2("data/AssignBondOrderProcessor_test_AN06.mol2", std::ios::in);
	mol_in2 >> sys2;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys2), 2 )//,0.00625) //2
	
	System sys3;
	MOL2File mol_in3("data/AssignBondOrderProcessor_test_BEWCUB.mol2", std::ios::in);
	mol_in3 >> sys3;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys3), 33)//0.0090411) // 33.)
RESULT


CHECK( operator() and apply(Position i) A* single solution )
	AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	
	System sys;
	MOL2File mol("data/AssignBondOrderProcessor_test_AGLYSL01_out.mol2", std::ios::in);
	mol >> sys;
	storeBondOrders(sys);	
	sys.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys), true)
	
	System sys2;
	MOL2File mol2("data/AssignBondOrderProcessor_test_CITSED10_sol_2.mol2", std::ios::in);
	mol2 >> sys2;
	storeBondOrders(sys2);	
	sys2.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys2), true)

	System sys3;
	MOL2File mol3("data/AssignBondOrderProcessor_test_CITSED10_sol_6.mol2", std::ios::in);
	mol3 >> sys3;
	storeBondOrders(sys3);	
	sys3.apply(testbop);
	testbop.apply(6);
	TEST_EQUAL(compareBondOrder(sys3), true)


	System sys4;
	MOL2File mol4("data/AssignBondOrderProcessor_test_COHKOZ_sol_1.mol2", std::ios::in);
	mol4 >> sys4;
	storeBondOrders(sys4);	
	sys4.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys4), true)

	System sys5;
	MOL2File mol5("data/AssignBondOrderProcessor_test_COHKOZ_sol_2.mol2", std::ios::in);
	mol5 >> sys5;
	storeBondOrders(sys5);	
	sys5.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys5), true)

	System sys6;
	MOL2File mol6("data/AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2", std::ios::in);
	mol6 >> sys6;
	storeBondOrders(sys6);	
	sys6.apply(testbop);
	testbop.apply(5);
	TEST_EQUAL(compareBondOrder(sys6), true)

	System sys7;
	MOL2File mol7("data/AssignBondOrderProcessor_test_CONFAM_sol_1.mol2", std::ios::in);
	mol7 >> sys7;
	storeBondOrders(sys7);	
	sys7.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys7), true)

	System sys8;
	MOL2File mol8("data/AssignBondOrderProcessor_test_CONFAM_sol_2.mol2", std::ios::in);
	mol8 >> sys8;
	storeBondOrders(sys8);	
	sys8.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys8), true)

	System sys9;
	MOL2File mol9("data/AssignBondOrderProcessor_test_CONFAM_sol_3.mol2", std::ios::in);
	mol9 >> sys9;
	storeBondOrders(sys9);	
	sys9.apply(testbop);
	testbop.apply(3);
	TEST_EQUAL(compareBondOrder(sys9), true)

	System sys10;
	MOL2File mol10("data/AssignBondOrderProcessor_test_CONFAM_sol_4.mol2", std::ios::in);
	mol10 >> sys10;
	storeBondOrders(sys10);	
	sys10.apply(testbop);
	testbop.apply(4);
	TEST_EQUAL(compareBondOrder(sys10), true)

	System sys11;
	MOL2File mol11("data/AssignBondOrderProcessor_test_CUDJAM_sol_1.mol2", std::ios::in);
	mol11 >> sys11;
	storeBondOrders(sys11);	
	sys11.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys11), true)

	System sys12;
	MOL2File mol12("data/AssignBondOrderProcessor_test_CUDJAM_sol_2.mol2", std::ios::in);
	mol12 >> sys12;
	storeBondOrders(sys12);	
	sys12.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys12), true)

	System sys13;
	MOL2File mol13("data/AssignBondOrderProcessor_test_CUDJAM_sol_3.mol2", std::ios::in);
	mol13 >> sys13;
	storeBondOrders(sys13);	
	sys13.apply(testbop);
	testbop.apply(3);
	TEST_EQUAL(compareBondOrder(sys13), true)

	System sys14;
	MOL2File mol14("data/AssignBondOrderProcessor_test_CUDJAM_sol_4.mol2", std::ios::in);
	mol14 >> sys14;
	storeBondOrders(sys14);	
	sys14.apply(testbop);
	testbop.apply(4);
	TEST_EQUAL(compareBondOrder(sys14), true)

	System sys15;
	MOL2File mol15("data/AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2", std::ios::in);
	mol15 >> sys15;
	storeBondOrders(sys15);	
	sys15.apply(testbop);
	testbop.apply(5);
	TEST_EQUAL(compareBondOrder(sys15), true)

RESULT


CHECK(getTotalPenalty(Position i) and operator() A* single solution)
  AssignBondOrderProcessor testbop;		
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol_in("data/AssignBondOrderProcessor_test_AMHTAR01.mol2", std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.)

	//testbop.options.dump();

	System sys2;
	MOL2File mol_in2("data/AssignBondOrderProcessor_test_AN06.mol2", std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2)//0.00625) // 2.)	


	System sys3;
	MOL2File mol_in3("data/AssignBondOrderProcessor_test_BEWCUB.mol2", std::ios::in);
	mol_in3 >> sys3;	
	sys3.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0)

	
	System sys15;
	MOL2File mol15("data/AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2", std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 2 ) //0.0015528 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 5 ) //0.00388199)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 7 ) //0.00543478)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8 ) //0.00621118)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 10) //0.00776398)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 33) //0.0256211 )



	System sys10;
	MOL2File mol10("data/AssignBondOrderProcessor_test_CONFAM_sol_4.mol2", std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 3 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 33)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 36)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 66)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 96)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 96)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 96)
	
	System sys6;
	MOL2File mol6("data/AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2", std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 6 )// 0.00802139)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 7 )// 0.00935829)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 8 )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8 )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 15)// 0.0200535 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 34)// 0.0454545 )

	System sys4;
	MOL2File mol4("data/AssignBondOrderProcessor_test_CITSED10_sol_6.mol2", std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1 )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1 )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32)// 0.0625    )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66)// 0.125     )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66)// 0.128906  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67)

RESULT

CHECK(getTotalCharge(Position i))
  AssignBondOrderProcessor testbop;		
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	//testbop.options.dump();
	
	System sys;
	MOL2File mol_in("data/AssignBondOrderProcessor_test_AMHTAR01.mol2", std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), 1.)
	TEST_REAL_EQUAL(testbop.getTotalCharge(3), 3.)
	TEST_REAL_EQUAL(testbop.getTotalCharge(7), 5.)

	System sys2;
	MOL2File mol_in2("data/AssignBondOrderProcessor_test_AN06.mol2", std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), 1.)	
	TEST_REAL_EQUAL(testbop.getTotalCharge(2), -1.)	

	System sys3;
	MOL2File mol_in3("data/AssignBondOrderProcessor_test_BEWCUB.mol2", std::ios::in);
	mol_in3 >> sys3;	
	sys3.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), -2.)
	TEST_REAL_EQUAL(testbop.getTotalCharge(2), -4 )
	
	System sys40;
	MOL2File mol40("data/AssignBondOrderProcessor_test_C3_input.mol2", std::ios::in);
	mol40 >> sys40;	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), -4.)

RESULT


CHECK(testing bond order assignment using ILP - single sol)
 	//TODO
RESULT

CHECK(computeNextSolution() using A*)
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);

	System sys;
	MOL2File mol_in("data/AssignBondOrderProcessor_test_AN06.mol2", std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),1)	
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.)//0.00625)// 2.)
	TEST_EQUAL(testbop.computeNextSolution(), true)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 32)//0.1)//32.)
	TEST_EQUAL(testbop.computeNextSolution(), true)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 128)
	TEST_EQUAL(testbop.computeNextSolution(), false)
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),3)

	System sys4;
	MOL2File mol4("data/AssignBondOrderProcessor_test_CITSED10_sol_6.mol2", std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),1)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1.)
	TEST_EQUAL(testbop.computeNextSolution(), true)	

	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1.)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66)// 0.125     )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66)// 0.128906  )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 67)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(10), 97)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(11), 99)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(12), 99)
	TEST_EQUAL(testbop.computeNextSolution(), true)
	
	TEST_REAL_EQUAL(testbop.getTotalPenalty(13), 132)
	TEST_EQUAL(testbop.computeNextSolution(), false)

RESULT


CHECK(computeNextSolution() using ILP all optimal solutions)
	//TODO
RESULT

///////////// options

CHECK(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS / MAX_NUMBER_OF_SOLUTIONS=0 using A*)
  AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);

	System sys2;
	MOL2File mol_in2("data/AssignBondOrderProcessor_test_AN06.mol2", std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 2)	

	System sys15;
	MOL2File mol15("data/AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2", std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 2)	

	System sys10;
	MOL2File mol10("data/AssignBondOrderProcessor_test_CONFAM_sol_4.mol2", std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 2)	

	System sys6;
	MOL2File mol6("data/AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2", std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 2)	

	System sys4;
	MOL2File mol4("data/AssignBondOrderProcessor_test_CITSED10_sol_6.mol2", std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 3)	


	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);

	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 3)	
	
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 22)	

	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 14)	

RESULT


CHECK(Option::BOND_LENGTH_WEIGHTING and getTotalPenalty())
  AssignBondOrderProcessor testbop;
	
	/// 1.0
	testbop.options.setReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,1);

	System sys;
	MOL2File mol_in("data/AssignBondOrderProcessor_test_AMHTAR01.mol2", std::ios::in);
	mol_in >> sys;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys), 0.112424)
	
	System sys2;
	MOL2File mol_in2("data/AssignBondOrderProcessor_test_AN06.mol2", std::ios::in);
	mol_in2 >> sys2;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys2), 0.416132)
	
	System sys3;
	MOL2File mol_in3("data/AssignBondOrderProcessor_test_BEWCUB.mol2", std::ios::in);
	mol_in3 >> sys3;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys3), 0.084565)
	
	/// 0.5
	testbop.options.setReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,0.5);
	
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys), 0.0562121)
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys2),	0.215878)
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys3), 0.0498582)


	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys15;
	MOL2File mol15("data/AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2", std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.022713  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.042013  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.053144  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.0559914 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.0562938 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.0578149 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.0680484 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.0696795 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.0719661 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.0722199 )



	System sys10;
	MOL2File mol10("data/AssignBondOrderProcessor_test_CONFAM_sol_4.mol2", std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.034702)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.062828)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.101387)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.107255)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.219378)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.245037)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.289464)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.291931)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.29904 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.31759 )

	System sys6;
	MOL2File mol6("data/AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2", std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.0186741 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.04406   )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.0550543 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.0595762 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.064149  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.0694856 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.0707986 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.0712783 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.0727204 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.0783781 )

	System sys4;
	MOL2File mol4("data/AssignBondOrderProcessor_test_CITSED10_sol_6.mol2", std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.0463171)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.053751 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.109568 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.157167 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.346593 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.365591 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.409844 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.421408 )	
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.48037  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.487804 )

RESULT






CHECK(Option::MAX_NUMBER_OF_SOLUTIONS  - Default and value  using A*)
  AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys2;
	MOL2File mol_in2("data/AssignBondOrderProcessor_test_AMHTAR01.mol2", std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS)
	
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 5);
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 5)	
	
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 15)

	System sys6;
	MOL2File mol6("data/AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2", std::ios::in);
	mol6 >> sys6;

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 5);
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 5)	
	
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 15)	
	
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys6.apply(testbop);
	// NOTE: the operator() has to compute all optimal +1 solutions 
	// to get aware that all following solutions are not optimal any longer! 
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 2)	
	
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getNumberOfComputedSolutions(), 1)

RESULT



CHECK(Option::ADD_HYDROGENS using A* )
  AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, true);

	System sys;
	MOL2File mol("data/AssignBondOrderProcessor_test_C2_sol0.mol2", std::ios::in);
	mol >> sys;
	storeHBonds(sys);
	deleteHBonds(sys);
	sys.apply(testbop);
	TEST_EQUAL(compareHBonds(sys), true)


	System sys2;
	MOL2File mol2("data/AssignBondOrderProcessor_test_C2_sol1.mol2", std::ios::in);
	mol2 >> sys2;
	storeHBonds(sys2);
	deleteHBonds(sys2);
	sys2.apply(testbop);	
	testbop.apply(1);
	TEST_EQUAL(compareHBonds(sys2), true)

	System sys3;
	MOL2File mol3("data/AssignBondOrderProcessor_test_C2_sol2.mol2", std::ios::in);
	mol3 >> sys3;
	storeHBonds(sys3);
	deleteHBonds(sys3);
	sys3.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareHBonds(sys3), true)
	
RESULT



CHECK(getNumberOfAddedHydrogens())
	AssignBondOrderProcessor testbop;	
	//false
	System sys;
	MOL2File mol("data/AssignBondOrderProcessor_test_CITSED10_sol_6.mol2", std::ios::in);
	mol >> sys;
	sys.apply(testbop);
	for (Size i = 0; i < testbop.getNumberOfComputedSolutions(); i++)
	{	
		TEST_EQUAL(testbop.getNumberOfAddedHydrogens(i), 0)
	}

	System sys6;
	MOL2File mol6("data/AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2", std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	for (Size i = 0; i < testbop.getNumberOfComputedSolutions(); i++)
	{	
		TEST_EQUAL(testbop.getNumberOfAddedHydrogens(i), 0)
	}

	//true
	testbop.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, true);
	
	System sys20;
	MOL2File mol20("data/AssignBondOrderProcessor_test_C2.mol2", std::ios::in);
	mol20 >> sys20;
	sys20.apply(testbop);

	// NOTE: operator () __has to__ compute 1 solution more than optimal!
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 4)

	testbop.apply(0);
	TEST_REAL_EQUAL(testbop.getNumberOfAddedHydrogens(0), 6)
	
	testbop.apply(1);
	TEST_REAL_EQUAL(testbop.getNumberOfAddedHydrogens(1), 4)
	
	testbop.apply(2);
	TEST_REAL_EQUAL(testbop.getNumberOfAddedHydrogens(2), 2)

	//AMHTAR01

RESULT


CHECK(Option:::KEKULIZE_RINGS using A* )
	// NOTE :this option only depends on the AROMATICITY processor
	//       so there is really not much we can test here, so we 
	//       just execute the processor to have a chance of 
	//       catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor testbop;	
	
	// true
	System sys30;
	MOL2File mol30("data/AssignBondOrderProcessor_test_AMPTRB10_kek_sol0.mol2", std::ios::in);
	mol30 >> sys30;
	storeBondOrders(sys30);	
	sys30.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys30), true)
	
	// false
	testbop.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, false);
	
	System sys31;
	MOL2File mol31("data/AssignBondOrderProcessor_test_AMPTRB10_arom_sol0.mol2", std::ios::in);
	mol31 >> sys31;
	storeBondOrders(sys31);	
	sys31.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys31), true)

RESULT


CHECK(OVERWRITE_SINGLE_BOND_ORDERS  using A* )	
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, false);
	
	System sys40;
	MOL2File mol40("data/AssignBondOrderProcessor_test_C4_input.mol2", std::ios::in);
	mol40 >> sys40;	
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128)
	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);
	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0)

RESULT

CHECK(OVERWRITE_DOUBLE_BOND_ORDERS using A* )
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, false);
	
	System sys40;
	MOL2File mol40("data/AssignBondOrderProcessor_test_C3_input.mol2", std::ios::in);
	mol40 >> sys40;	
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128)
	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);
	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0)

RESULT

CHECK(OVERWRITE_TRIPLE_BOND_ORDERS using A* )	
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;	
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);
	
	System sys40;
	MOL2File mol40("data/AssignBondOrderProcessor_test_C4_input.mol2", std::ios::in);
	mol40 >> sys40;	
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128)
	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);
	
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0)
RESULT



CHECK(OVERWRITE_SELECTED_BONDS using A* )
  AssignBondOrderProcessor testbop;	
	
	System sys40;
	MOL2File mol40("data/AssignBondOrderProcessor_test_C4_input.mol2", std::ios::in);
	mol40 >> sys40;	
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128)
	
	// now select 3 bonds
	AtomIterator a_it;
	Atom::BondIterator b_it;
	int counter = 0;
	for (a_it = sys40.beginAtom(); a_it != sys40.endAtom(); a_it++)
	{
		for (b_it = a_it->beginBond(); (b_it != a_it->endBond()) && (counter < 7); b_it++)
		{
			counter++;
			b_it->setSelected(true);
		}
	}

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS, true);
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0)

RESULT



CHECK(AssignBondOrderProcessor(const AssignBondOrderProcessor& abop))
  // There is really not much we can test here, so we just execute the copy constructor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop1;
	AssignBondOrderProcessor abop2(abop1);
RESULT


CHECK(AssignBondOrderProcessor& operator = (const AssignBondOrderProcessor& abop))
  // There is really not much we can test here, so we just execute the assignment
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop1, abop2;
	abop2 = abop1;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

