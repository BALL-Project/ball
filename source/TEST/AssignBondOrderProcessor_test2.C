// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/STRUCTURE/BONDORDERS/AStarBondOrderStrategy.h>
#include <BALL/STRUCTURE/BONDORDERS/KGreedyBondOrderStrategy.h>
#include <BALL/STRUCTURE/BONDORDERS/branchAndBoundBondOrderStrategy.h>
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
	}
}

void deleteBondOrders(System& sys)
{
	AtomIterator a_it = sys.beginAtom();
	Atom::BondIterator b_it;
	BALL_FOREACH_BOND(sys, a_it, b_it)
	{
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
		//if ( b_it->getProperty("FORMER_ORDER").getInt() != b_it->getOrder())
			//cout << b_it->getFirstAtom()->getFullName() << " " << b_it->getSecondAtom()->getFullName() << "    new:" << b_it->getOrder() << "  old:" << b_it->getProperty("FORMER_ORDER").getInt() << endl;
		all_bonds_equal &= ( b_it->getProperty("FORMER_ORDER").getInt() == b_it->getOrder());
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



START_TEST(AssignBondOrderProcessor)

PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


CHECK(computeNextSolution() using A*)
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),1)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.f)//0.00625)// 2.)
	bool testbop_result = testbop.computeNextSolution();
	TEST_EQUAL(testbop_result, true)
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 32.f)//0.1)//32.)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),3)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 128.f)
	TEST_EQUAL(testbop.computeNextSolution(), false)
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),3)

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 1)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34.f)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34.f)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)// 0.125     )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66.f)// 0.128906  )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 67.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(10), 97.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(11), 99.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(12), 99.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(13), 132.f)
	TEST_EQUAL(testbop.computeNextSolution(), false)

RESULT


#ifdef BALL_HAS_LPSOLVE
CHECK(computeNextSolution() using ILP)
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 1)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.f)//0.00625)// 2.)
	TEST_EQUAL(testbop.computeNextSolution(), true)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 32.f)//0.1)//32.)
	TEST_EQUAL(testbop.computeNextSolution(), true)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 128.f)
	TEST_EQUAL(testbop.computeNextSolution(), false)
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),3)

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),1)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34.f)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34.f)// 0.0664062 )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)// 0.125     )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66.f)// 0.128906  )
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 67.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(10), 97.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(11), 99.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(12), 99.f)
	TEST_EQUAL(testbop.computeNextSolution(), true)

	TEST_REAL_EQUAL(testbop.getTotalPenalty(13), 132.f)
	TEST_EQUAL(testbop.computeNextSolution(), false)

RESULT
#endif

CHECK(computeNextSolution() using FPT)
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 1)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.f)//0.00625)// 2.)
	bool test = testbop.computeNextSolution();
	TEST_EQUAL(test, false)

RESULT


CHECK(resetBondOrders())
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	//testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);

	System sys2;
	MOL2File mol2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_2.mol2), std::ios::in);
	mol2 >> sys2;
	storeBondOrders(sys2);
	sys2.apply(testbop);
	testbop.apply(0);
	TEST_EQUAL(compareBondOrder(sys2), false)
	testbop.resetBondOrders();
	TEST_EQUAL(compareBondOrder(sys2), true)

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_1.mol2), std::ios::in);
	mol4 >> sys4;
	storeBondOrders(sys4);
	sys4.apply(testbop);
	testbop.apply(0);
	TEST_EQUAL(compareBondOrder(sys4), false)
	testbop.resetBondOrders();
	TEST_EQUAL(compareBondOrder(sys4), true)

	System sys7;
	MOL2File mol7(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_1.mol2), std::ios::in);
	mol7 >> sys7;
	storeBondOrders(sys7);
	sys7.apply(testbop);
	testbop.apply(0);
	TEST_EQUAL(compareBondOrder(sys7), false)
	testbop.resetBondOrders();
	TEST_EQUAL(compareBondOrder(sys7), true)

	System sys11;
	MOL2File mol11(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_1.mol2), std::ios::in);
	mol11 >> sys11;
	storeBondOrders(sys11);
	sys11.apply(testbop);
	testbop.apply(0);
	TEST_EQUAL(compareBondOrder(sys11), false)
	testbop.resetBondOrders();
	TEST_EQUAL(compareBondOrder(sys11), true)

RESULT

///////////////////           options                 ///////////////////////////

CHECK(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS and Option:: MAX_NUMBER_OF_SOLUTIONS=0 using A*)
  AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 3)


	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);

	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 3)

	sys10.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 22)

	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 14)

RESULT

CHECK(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS and Option:: MAX_NUMBER_OF_SOLUTIONS=0 using FPT)
  AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 3)

	// Option::MAX_NUMBER_OF_SOLUTIONS==0 is used to automatically enumerate all optimal solutions
	// this currently does not work for the FPT
//	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
//	testbop.options.setBool(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);

//	sys2.apply(testbop);
//	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 3)

//	sys10.apply(testbop);
//	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 22)

//	sys4.apply(testbop);
//	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 14)
RESULT



CHECK(Option::BOND_LENGTH_WEIGHTING and getTotalPenalty())
  AssignBondOrderProcessor testbop;

	/// 1.0
	testbop.options.setReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,1);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys), 0.187165f) //0.112424)

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys2),0.00879483f)// 0.416132)

	System sys3;
	MOL2File mol_in3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_BEWCUB.mol2), std::ios::in);
	mol_in3 >> sys3;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys3),0.224469f) // 0.084565)

	/// 0.5
	testbop.options.setReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,0.5);

	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys),0.0935824f)//  0.0562121)
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys2),0.0122099f )//	0.215878)
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys3),0.11981f)// 0.0498582)


	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.0527984f)// 0.022713  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.0782613f)// 0.042013  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.079033f )// 0.053144  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.079033f )// 0.0559914 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.0805762f)//0.0562938 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.0805762f)// 0.0578149 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.0836669f)// 0.0680484 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.103724f )//0.0696795 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.103724f )//0.0719661 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.103724f ) //0.0722199 )



	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.182807f)// 0.034702)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.240099f)// 0.062828)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.29739f )//  0.101387)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.349474f)// 0.107255)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.397803f)// 0.219378)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.406765f)// 0.245037)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.449641f)//0.289464)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.455095f)// 0.291931)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.50346f )//0.29904 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.506933f)//0.31759 )

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.260709f)//0.0186741 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.266806f)//0.04406   )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.295261f)//0.0550543 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.295261f)//0.0595762 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.295261f)//0.064149  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.295261f)//0.0694856 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.297294f)// 0.0707986 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.297294f)//0.0712783 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.297294f)//0.0727204 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.297294f)//0.0783781 )

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.175545f)//0.0463171)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 0.2848f  )//0.053751 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 0.306504f)//0.109568 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 0.344816f)//0.157167 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 0.45407f )//0.346593 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 0.475775f)//0.365591 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 0.582425f)//0.409844 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 0.671486f)//0.421408 )	
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 0.710181f)//0.48037  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 0.751696f)//0.487804 )

RESULT



CHECK(Option::MAX_NUMBER_OF_SOLUTIONS  - Default and value  using A*)
  AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), (Size)AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 5);
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 5)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 15)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 5);
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), (Size)5)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 15)

	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys6.apply(testbop);
	// NOTE: the operator() has to compute all optimal +1 solutions 
	// to get aware that all following solutions are not optimal any longer! 
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 1)

RESULT

CHECK(Option::MAX_NUMBER_OF_SOLUTIONS  - Default and value  using FPT)
  AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), (Size)AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 5);
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 5)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys2.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 15)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 5);
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 5)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 15)

	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, false);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 15);
	sys6.apply(testbop);
	// NOTE: the operator() has to compute all optimal +1 solutions 
	// to get aware that all following solutions are not optimal any longer! 
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 2)

	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);
	sys6.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 1)

RESULT

#ifdef BALL_HAS_LPSOLVE
CHECK(Option::ADD_HYDROGENS using ILP )
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	testbop.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, true);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C2_sol0.mol2), std::ios::in);
	mol >> sys;
	storeHBonds(sys);
	deleteHBonds(sys);
	sys.apply(testbop);
	TEST_EQUAL(compareHBonds(sys), true)


	System sys2;
	MOL2File mol2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C2_sol1.mol2), std::ios::in);
	mol2 >> sys2;
	storeHBonds(sys2);
	deleteHBonds(sys2);
	sys2.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareHBonds(sys2), true)

	System sys3;
	MOL2File mol3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C2_sol2.mol2), std::ios::in);
	mol3 >> sys3;
	storeHBonds(sys3);
	deleteHBonds(sys3);
	sys3.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareHBonds(sys3), true)

RESULT
#endif


CHECK(getNumberOfAddedHydrogens() using A*)
	AssignBondOrderProcessor testbop;
	//false
	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol >> sys;
	sys.apply(testbop);
	for (Size i = 0; i < testbop.getNumberOfComputedSolutions(); i++)
	{
		TEST_EQUAL(testbop.getNumberOfAddedHydrogens(i), 0)
	}

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	for (Size i = 0; i < testbop.getNumberOfComputedSolutions(); i++)
	{
		TEST_EQUAL(testbop.getNumberOfAddedHydrogens(i), 0)
	}

	//true
	testbop.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, true);

	System sys20;
	MOL2File mol20(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C2.mol2), std::ios::in);
	mol20 >> sys20;
	sys20.apply(testbop);

	// NOTE: operator () __has to__ compute 1 solution more than optimal!
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 4)

	testbop.apply(0);
	TEST_EQUAL(testbop.getNumberOfAddedHydrogens(0), 6)

	testbop.apply(1);
	TEST_EQUAL(testbop.getNumberOfAddedHydrogens(1), 4)

	testbop.apply(2);
	TEST_EQUAL(testbop.getNumberOfAddedHydrogens(2), 2)

RESULT

#ifdef BALL_HAS_LPSOLVE
CHECK(getNumberOfAddedHydrogens() using ILP)
	AssignBondOrderProcessor testbop_ilp;
	testbop_ilp.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	//false
	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol >> sys;
	sys.apply(testbop_ilp);
	for (Size i = 0; i < testbop_ilp.getNumberOfComputedSolutions(); i++)
	{
		TEST_EQUAL(testbop_ilp.getNumberOfAddedHydrogens(i), 0)
	}

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop_ilp);
	for (Size i = 0; i < testbop_ilp.getNumberOfComputedSolutions(); i++)
	{
		TEST_EQUAL(testbop_ilp.getNumberOfAddedHydrogens(i), 0)
	}

	//true
	testbop_ilp.options.setBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS, true);

	System sys20;
	MOL2File mol20(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C2.mol2), std::ios::in);
	mol20 >> sys20;
	sys20.apply(testbop_ilp);

	// NOTE: Usually operator () has to compute 1 solution more than optimal but not in this case.
	// The resulting ILP for another solution is not feasible!
	TEST_EQUAL(testbop_ilp.getNumberOfComputedSolutions(), 3)

	testbop_ilp.apply(0);
	TEST_EQUAL(testbop_ilp.getNumberOfAddedHydrogens(0), 6)

	testbop_ilp.apply(1);
	TEST_EQUAL(testbop_ilp.getNumberOfAddedHydrogens(1), 2)

	testbop_ilp.apply(2);
	TEST_EQUAL(testbop_ilp.getNumberOfAddedHydrogens(2), 4)
RESULT
#endif


CHECK(Option:::KEKULIZE_RINGS using A* )
	// NOTE :this option only depends on the AROMATICITY processor
	//       so there is really not much we can test here, so we 
	//       just execute the processor to have a chance of 
	//       catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor testbop;
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS), true)
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION), true)

	// true
	System sys30;
	MOL2File mol30(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMPTRB10_kek_sol0.mol2), std::ios::in);
	mol30 >> sys30;
	storeBondOrders(sys30);
	sys30.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys30), true)

	// false
	testbop.options.setBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS, false);

	System sys31;
	MOL2File mol31(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMPTRB10_arom_sol0.mol2), std::ios::in);
	mol31 >> sys31;
	storeBondOrders(sys31);
	//deleteBondOrders(sys31);
	sys31.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys31), true)

RESULT


CHECK(Option:::KEKULIZE_RINGS using FPT )
	// NOTE :this option only depends on the AROMATICITY processor
	//       so there is really not much we can test here, so we 
	//       just execute the processor to have a chance of 
	//       catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);
	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION), true)

	// there exist 7 optimal solutions for AssignBondOrderProcessor_test_AMPTRB10_kek_sol0.mol2 
	// and FPT returns a different solution than A* ... we tested kekulized solutions implicitely

	testbop.options.setBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS, false);

	System sys31;
	MOL2File mol31(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMPTRB10_arom_sol0.mol2), std::ios::in);
	mol31 >> sys31;
	storeBondOrders(sys31);
	//deleteBondOrders(sys31);
	sys31.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys31), true)

RESULT


CHECK(Option::OVERWRITE_SINGLE_BOND_ORDERS  using A* )
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, false);

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C4_input.mol2), std::ios::in);
	mol40 >> sys40;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128.f)

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64.f)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

RESULT

#ifdef BALL_HAS_LPSOLVE
CHECK(Option::OVERWRITE_SINGLE_BOND_ORDERS  using ILP )
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, false);

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C4_input.mol2), std::ios::in);
	mol40 >> sys40;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128.f)

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64.f)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

RESULT
#endif


CHECK(Option::OVERWRITE_DOUBLE_BOND_ORDERS using A* )
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, false);

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C3_input.mol2), std::ios::in);
	mol40 >> sys40;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128.f)

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64.f)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

RESULT

#ifdef BALL_HAS_LPSOLVE
CHECK(Option::OVERWRITE_DOUBLE_BOND_ORDERS using ILP )
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, false);

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C3_input.mol2), std::ios::in);
	mol40 >> sys40;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128.f)

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64.f)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

RESULT
#endif

CHECK(Option::OVERWRITE_TRIPLE_BOND_ORDERS using A* )
	// This test is very hand made 
	// By construction the optimal solution can be reached if _all_ 
	// bonds were considered, but with the current option 
	// only a better solution is found!
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, false);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C4_input.mol2), std::ios::in);
	mol40 >> sys40;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128.f)

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 64.f)

	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, true);
	testbop.options.setBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, true);

	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)
RESULT

CHECK(Option::OVERWRITE_SELECTED_BONDS using A* )
	// There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
  AssignBondOrderProcessor testbop;

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C4_input.mol2), std::ios::in);
	mol40 >> sys40;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys40), 128.f)

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
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

RESULT


CHECK(Option::COMPUTE_ALSO_CONNCETIVITY and Option::CONNECTIVITY_CUTOFF)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	// NOTE: this functionality is still experimental! //TODO
	AssignBondOrderProcessor abop;
	abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY, true);
	abop.options.setReal(AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF, 3);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AAA.mol2), std::ios::in);
	mol >> sys;
	sys.apply(abop);

	abop.clear();
	abop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY, false);
	abop.options.setReal(AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF, 5);
	sys.apply(abop);
RESULT

CHECK(Option::USE_FINE_PENALTY)
	AssignBondOrderProcessor abop;
	abop.options.setBool(AssignBondOrderProcessor::Option::USE_FINE_PENALTY, true);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMPTRB10_kek_sol0.mol2), std::ios::in);
	mol >> sys;
	storeBondOrders(sys);
	sys.apply(abop);
	//abop.apply(3);
	TEST_EQUAL(compareBondOrder(sys), true)
	abop.apply(3);
	TEST_EQUAL(compareBondOrder(sys), false)

	abop.clear();
	abop.options.setBool(AssignBondOrderProcessor::Option::USE_FINE_PENALTY, false);
	storeBondOrders(sys);
	sys.apply(abop);
	TEST_EQUAL(compareBondOrder(sys), false)
	abop.apply(5); //3);
	TEST_EQUAL(compareBondOrder(sys), true)
RESULT

CHECK(Option::INIFile)
	// There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::INIFile, "/bond_lengths/BondOrderGAFF.xml");

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C4_input.mol2), std::ios::in);
	mol40 >> sys40;
	sys40.apply(abop);
RESULT

CHECK(Option::MAX_BOND_ORDER)
	AssignBondOrderProcessor abop;
	abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER, 1);

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C4_input.mol2), std::ios::in);
	mol40 >> sys40;
	sys40.apply(abop);
	TEST_REAL_EQUAL(abop.evaluatePenalty(&sys40), 192.f)

	abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER, 2);
	sys40.apply(abop);
	TEST_REAL_EQUAL(abop.evaluatePenalty(&sys40), 0.f)

	// FPT		
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);
	abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER, 1);

	sys40.apply(abop);
	TEST_REAL_EQUAL(abop.evaluatePenalty(&sys40), 192.f)

	abop.options.setInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER, 2);
	sys40.apply(abop);
	TEST_REAL_EQUAL(abop.evaluatePenalty(&sys40), 0.f)

RESULT

CHECK(Option::APPLY_FIRST_SOLUTION)
	AssignBondOrderProcessor abop;
	abop.options.setBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION, true);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_2.mol2), std::ios::in);
	mol >> sys;
	storeBondOrders(sys);
	sys.apply(abop);
	TEST_EQUAL(compareBondOrder(sys), false)

	// reset the BondOrders
	abop.resetBondOrders();
	TEST_EQUAL(compareBondOrder(sys), true)

	abop.clear();
	abop.options.setBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION, false);
	storeBondOrders(sys);
	sys.apply(abop);
	TEST_EQUAL(compareBondOrder(sys), true)
	abop.apply(0);
	TEST_EQUAL(compareBondOrder(sys), false)

	// now the FPT
	abop.resetBondOrders();
	TEST_EQUAL(compareBondOrder(sys), true)

	abop.clear();
	abop.options.setBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION, false);
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);

	storeBondOrders(sys);
	sys.apply(abop);
	TEST_EQUAL(compareBondOrder(sys), true)
	abop.apply(0);
	TEST_EQUAL(compareBondOrder(sys), false)

RESULT


CHECK(Option::GREEDY_K_SIZE)
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::K_GREEDY);
	abop.options.setInteger(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE, 1);
	abop.options.set(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol >> sys;
	sys.apply(abop);
	TEST_EQUAL(abop.getNumberOfComputedSolutions(), 1)

	abop.options.setInteger(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE, 2);
	sys.apply(abop);
	TEST_EQUAL(abop.getNumberOfComputedSolutions(), 2)

	abop.options.setInteger(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE, 5);
	sys.apply(abop);
	TEST_EQUAL(abop.getNumberOfComputedSolutions(), 5)

	CAPTURE_OUTPUT_LEVEL(1000)
		abop.options.setInteger(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE, -1);
		sys.apply(abop);
		TEST_EQUAL(abop.getNumberOfComputedSolutions(), 0)
	COMPARE_OUTPUT("")
RESULT

CHECK(Option::BRANCH_AND_BOUND_CUTOFF)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::BRANCH_AND_BOUND);
	abop.options.setReal(BranchAndBoundBondOrderStrategy::Option::BRANCH_AND_BOUND_CUTOFF, 1);
	abop.options.setReal(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE, 5);
	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol >> sys;
	sys.apply(abop);
	// NOTE: we cannot guarantee the value, since our b&b is just a heuristic...
	//TEST_REAL_EQUAL(abop.getTotalPenalty(0), 0.f)
RESULT


CHECK(getSolution(Position i))
	// There is really not much we can test here, so we just execute the method
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);

	Size limit = testbop.getNumberOfComputedSolutions();
	System sys;
	for (Size i=0; i<limit; i++)
	{
		sys = testbop.getSolution(i);
		bool no_exception = true;
		try {
			sys = testbop.getSolution(i);
			const System& ref_sys = testbop.getSolution(i);
			TEST_EQUAL(ref_sys.countBonds(), 8);
		} catch (...) {
			no_exception = false;
		}
		TEST_EQUAL(no_exception, true)
	}

	TEST_EXCEPTION(Exception::IndexOverflow, testbop.getSolution(limit))

RESULT


CHECK(clear())
	// There is really not much we can test here, so we just execute the method
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	System sys;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys;
	sys.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),10)
	TEST_EQUAL(testbop.getNumberOfAddedHydrogens(0), 0)
	TEST_EQUAL(testbop.getAtomContainer()==(&*(sys.beginMolecule())), true)
	TEST_EQUAL(testbop.getSolution(0) == sys, true)
	TEST_EQUAL(testbop.getTotalPenalty(0), 1)
	TEST_EQUAL(testbop.apply(0), true)
	testbop.resetBondOrders();
	TEST_EQUAL(testbop.computeNextSolution(), true)

	testbop.clear();
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(),0)
	TEST_EQUAL(testbop.getNumberOfAddedHydrogens(0), 0)
	TEST_EQUAL(testbop.getAtomContainer()==NULL, true)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3),std::numeric_limits<float>::max())
	TEST_EQUAL(testbop.apply(0),false)
	testbop.resetBondOrders();
	TEST_EQUAL(testbop.computeNextSolution(), false)

RESULT




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

