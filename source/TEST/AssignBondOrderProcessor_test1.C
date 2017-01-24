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


START_TEST(AssignBondOrderProcessor)

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

	TEST_EQUAL(testbop.options.get(AStarBondOrderStrategy::Option::HEURISTIC),
													 AStarBondOrderStrategy::Default::HEURISTIC)

	TEST_REAL_EQUAL(testbop.options.getReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING),
													 AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING)

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION),
													 AssignBondOrderProcessor::Default::APPLY_FIRST_SOLUTION)

	TEST_EQUAL(testbop.options.getInteger(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE),
													 KGreedyBondOrderStrategy::Default::GREEDY_K_SIZE)

	TEST_REAL_EQUAL(testbop.options.getReal(BranchAndBoundBondOrderStrategy::Option::BRANCH_AND_BOUND_CUTOFF),
													 BranchAndBoundBondOrderStrategy::Default::BRANCH_AND_BOUND_CUTOFF)

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY),
													 AssignBondOrderProcessor::Default::COMPUTE_ALSO_CONNECTIVITY)

	TEST_REAL_EQUAL(testbop.options.getReal(AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF),
													 AssignBondOrderProcessor::Default::CONNECTIVITY_CUTOFF)

	TEST_EQUAL(testbop.options.getBool(AssignBondOrderProcessor::Option::USE_FINE_PENALTY),
													 AssignBondOrderProcessor::Default::USE_FINE_PENALTY)

RESULT


CHECK(check Options for consistency)
	Log.error().disableOutput();

	AssignBondOrderProcessor testbop;
	testbop.setDefaultOptions();
	TEST_EQUAL(testbop.hasValidOptions(), true)

	testbop.setDefaultOptions();
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);
	testbop.options.set(AssignBondOrderProcessor::Option::ADD_HYDROGENS, true);
	TEST_EQUAL(testbop.hasValidOptions(), false)

	testbop.setDefaultOptions();
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);
	testbop.options.set(AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS, true);
	TEST_EQUAL(testbop.hasValidOptions(), false)

	testbop.setDefaultOptions();
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);
	TEST_EQUAL(testbop.hasValidOptions(), true)

	testbop.setDefaultOptions();
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);
	testbop.options.set(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING, 0);
	testbop.options.set(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, true);
	testbop.options.set(AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY, true);
	testbop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 0);
	TEST_EQUAL(testbop.hasValidOptions(), false)
	Log.error().enableOutput();
RESULT

///////////////////////  ALGORITHMS     //////////////////////
CHECK(Option::ALGORITHM: A_STAR)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::A_STAR);

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(abop);
	TEST_REAL_EQUAL(abop.getTotalPenalty(1), 6.f)
RESULT


CHECK(Option::ALGORITHM: FPT)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::FPT);

	System sys60;
	MOL2File mol60(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol60 >> sys60;
	sys60.apply(abop);
	TEST_EQUAL(abop.hasValidOptions(), true)

	TEST_EQUAL(abop.getNumberOfComputedSolutions(), 2)
	TEST_REAL_EQUAL(abop.getTotalPenalty(1), 6.f)
RESULT


#ifdef BALL_HAS_LPSOLVE
CHECK(Option::ALGORITHM: ILP - single solution)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	abop.options.set(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS, 1);

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(abop);
	TEST_REAL_EQUAL(abop.getTotalPenalty(0), 0.f)
RESULT
#endif

CHECK(ALGORITHM: K_GREEDY)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::K_GREEDY);

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(abop);
	TEST_REAL_EQUAL(abop.getTotalPenalty(1), 34.f)
RESULT

CHECK(Option::ALGORITHM: BRANCH_AND_BOUND)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AssignBondOrderProcessor::Option::ALGORITHM, AssignBondOrderProcessor::Algorithm::BRANCH_AND_BOUND);
	//abop.options.setReal(BranchAndBoundBondOrderStrategy::Option::BRANCH_AND_BOUND_CUTOFF, 5);
	//abop.options.setReal(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE, 100);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AAA.mol2), std::ios::in);
	mol >> sys;
	sys.apply(abop);
	TEST_REAL_EQUAL(abop.getTotalPenalty(0), 64.f)
RESULT



///////////////////////  HEURISTICS     //////////////////////
CHECK(Option::HEURISTIC: SIMPLE)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AStarBondOrderStrategy::Option::HEURISTIC,AStarBondOrderStrategy::Heuristic::SIMPLE);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(abop);
RESULT
CHECK(Option::HEURISTIC: MEDIUM)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AStarBondOrderStrategy::Option::HEURISTIC,AStarBondOrderStrategy::Heuristic::MEDIUM);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(abop);
RESULT
CHECK(Option::HEURISTIC: TIGHT)
  // There is really not much we can test here, so we just execute the processor
	// to have a chance of catching bugs with valgrind if they sneak in
	AssignBondOrderProcessor abop;
	abop.options.set(AStarBondOrderStrategy::Option::HEURISTIC,AStarBondOrderStrategy::Heuristic::TIGHT);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(abop);
RESULT


CHECK(getNumberOfComputedSolutions())
  AssignBondOrderProcessor testbop;
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 0)
	// There are further test in e.g. computeNextSolution()

	// FPT
	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 3)

RESULT


CHECK(getAtomContainer())
  AssignBondOrderProcessor testbop;
	TEST_EQUAL(testbop.getAtomContainer()==NULL, true)

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_EQUAL(testbop.getAtomContainer()==(&*(sys.beginMolecule())), true)
RESULT


CHECK(evaluatePenalty(AtomContainer* ac))
  AssignBondOrderProcessor testbop;
	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys), 0.f)

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys2), 2.f)

	System sys3;
	MOL2File mol_in3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_BEWCUB.mol2), std::ios::in);
	mol_in3 >> sys3;
	TEST_REAL_EQUAL(testbop.evaluatePenalty(&sys3), 33.f)
RESULT


CHECK( operator() and apply(Position i) A* single solution )
	AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AGLYSL01_out.mol2), std::ios::in);
	mol >> sys;
	storeBondOrders(sys);
	sys.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys), true)

	System sys2;
	MOL2File mol2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_2.mol2), std::ios::in);
	mol2 >> sys2;
	storeBondOrders(sys2);
	sys2.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys2), true)

	System sys3;
	MOL2File mol3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol3 >> sys3;
	storeBondOrders(sys3);
	sys3.apply(testbop);
	testbop.apply(6);
	TEST_EQUAL(compareBondOrder(sys3), true)


	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_1.mol2), std::ios::in);
	mol4 >> sys4;
	storeBondOrders(sys4);
	sys4.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys4), true)

	System sys5;
	MOL2File mol5(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_2.mol2), std::ios::in);
	mol5 >> sys5;
	storeBondOrders(sys5);
	sys5.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys5), true)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	storeBondOrders(sys6);
	sys6.apply(testbop);
	testbop.apply(5);
	TEST_EQUAL(compareBondOrder(sys6), true)

	System sys7;
	MOL2File mol7(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_1.mol2), std::ios::in);
	mol7 >> sys7;
	storeBondOrders(sys7);
	sys7.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys7), true)

	System sys8;
	MOL2File mol8(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_2.mol2), std::ios::in);
	mol8 >> sys8;
	storeBondOrders(sys8);
	sys8.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys8), true)

	System sys9;
	MOL2File mol9(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_3.mol2), std::ios::in);
	mol9 >> sys9;
	storeBondOrders(sys9);
	sys9.apply(testbop);
	testbop.apply(3);
	TEST_EQUAL(compareBondOrder(sys9), true)

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	storeBondOrders(sys10);
	sys10.apply(testbop);
	testbop.apply(4);
	TEST_EQUAL(compareBondOrder(sys10), true)

	System sys11;
	MOL2File mol11(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_1.mol2), std::ios::in);
	mol11 >> sys11;
	storeBondOrders(sys11);
	sys11.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys11), true)

	System sys12;
	MOL2File mol12(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_2.mol2), std::ios::in);
	mol12 >> sys12;
	storeBondOrders(sys12);
	sys12.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys12), true)

	System sys13;
	MOL2File mol13(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_3.mol2), std::ios::in);
	mol13 >> sys13;
	storeBondOrders(sys13);
	sys13.apply(testbop);
	testbop.apply(3);
	TEST_EQUAL(compareBondOrder(sys13), true)

	System sys14;
	MOL2File mol14(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_4.mol2), std::ios::in);
	mol14 >> sys14;
	storeBondOrders(sys14);
	sys14.apply(testbop);
	testbop.apply(4);
	TEST_EQUAL(compareBondOrder(sys14), true)

	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	storeBondOrders(sys15);
	sys15.apply(testbop);
	testbop.apply(5);
	TEST_EQUAL(compareBondOrder(sys15), true)

	System sys16;
	MOL2File mol16(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AAA.mol2), std::ios::in);
	mol16 >> sys16;
	storeBondOrders(sys16);
	sys16.apply(testbop);
	testbop.apply(0);
	TEST_EQUAL(compareBondOrder(sys16), true)

RESULT

#ifdef BALL_HAS_LPSOLVE
CHECK(testing bond order assignment using ILP - operator() and apply(Position i))
	AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AGLYSL01_out.mol2), std::ios::in);
	mol >> sys;
	storeBondOrders(sys);
	sys.apply(testbop);
	TEST_EQUAL(compareBondOrder(sys), true)

	System sys2;
	MOL2File mol2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_2.mol2), std::ios::in);
	mol2 >> sys2;
	storeBondOrders(sys2);
	sys2.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys2), true)

	System sys3;
	MOL2File mol3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol3 >> sys3;
	storeBondOrders(sys3);
	sys3.apply(testbop);
	testbop.apply(7);
	TEST_EQUAL(compareBondOrder(sys3), true)


	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_1.mol2), std::ios::in);
	mol4 >> sys4;
	storeBondOrders(sys4);
	sys4.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys4), true)

	System sys5;
	MOL2File mol5(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_2.mol2), std::ios::in);
	mol5 >> sys5;
	storeBondOrders(sys5);
	sys5.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys5), true)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	storeBondOrders(sys6);
	sys6.apply(testbop);
	testbop.apply(5);
	TEST_EQUAL(compareBondOrder(sys6), true)

	System sys7;
	MOL2File mol7(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_1.mol2), std::ios::in);
	mol7 >> sys7;
	storeBondOrders(sys7);
	sys7.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys7), true)

	System sys8;
	MOL2File mol8(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_2.mol2), std::ios::in);
	mol8 >> sys8;
	storeBondOrders(sys8);
	sys8.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys8), true)

	System sys9;
	MOL2File mol9(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_3.mol2), std::ios::in);
	mol9 >> sys9;
	storeBondOrders(sys9);
	sys9.apply(testbop);
	testbop.apply(3);
	TEST_EQUAL(compareBondOrder(sys9), true)

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	storeBondOrders(sys10);
	sys10.apply(testbop);
	testbop.apply(4);
	TEST_EQUAL(compareBondOrder(sys10), true)

	System sys11;
	MOL2File mol11(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_1.mol2), std::ios::in);
	mol11 >> sys11;
	storeBondOrders(sys11);
	sys11.apply(testbop);
	testbop.apply(1);
	TEST_EQUAL(compareBondOrder(sys11), true)

	System sys12;
	MOL2File mol12(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_2.mol2), std::ios::in);
	mol12 >> sys12;
	storeBondOrders(sys12);
	sys12.apply(testbop);
	testbop.apply(2);
	TEST_EQUAL(compareBondOrder(sys12), true)

	System sys13;
	MOL2File mol13(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_3.mol2), std::ios::in);
	mol13 >> sys13;
	storeBondOrders(sys13);
	sys13.apply(testbop);
	testbop.apply(3);
	TEST_EQUAL(compareBondOrder(sys13), true)

	System sys14;
	MOL2File mol14(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_4.mol2), std::ios::in);
	mol14 >> sys14;
	storeBondOrders(sys14);
	sys14.apply(testbop);
	testbop.apply(4);
	TEST_EQUAL(compareBondOrder(sys14), true)

	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	storeBondOrders(sys15);
	sys15.apply(testbop);
	testbop.apply(5);
	TEST_EQUAL(compareBondOrder(sys15), true)

	System sys16;
	MOL2File mol16(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AAA.mol2), std::ios::in);
	mol16 >> sys16;
	storeBondOrders(sys16);
	sys16.apply(testbop);
	testbop.apply(0);
	TEST_EQUAL(compareBondOrder(sys16), true)

RESULT
#endif

CHECK(getTotalPenalty(Position i) and operator() A* single solution)
  AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

	//testbop.options.dump();

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.f)//0.00625) // 2.)	


	System sys3;
	MOL2File mol_in3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_BEWCUB.mol2), std::ios::in);
	mol_in3 >> sys3;
	sys3.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)


	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 2.f ) //0.0015528 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 5.f ) //0.00388199)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 7.f ) //0.00543478)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8.f ) //0.00621118)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 10.f) //0.00776398)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 33.f) //0.0256211 )

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 3.f )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 33.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 36.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 66.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 96.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 96.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 96.f)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 6.f )// 0.00802139)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 7.f )// 0.00935829)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 8.f )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8.f )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 15.f)// 0.0200535 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 34.f)// 0.0454545 )

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1.f )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1.f )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32.f)// 0.0625    )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)// 0.125     )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66.f)// 0.128906  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67.f)

RESULT

#ifdef BALL_HAS_LPSOLVE
CHECK(getTotalPenalty(Position i) and operator() ILP single solution)
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::ILP);
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

	//testbop.options.dump();

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.f)//0.00625) // 2.)	


	System sys3;
	MOL2File mol_in3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_BEWCUB.mol2), std::ios::in);
	mol_in3 >> sys3;
	sys3.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)


	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 2.f ) //0.0015528 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 5.f ) //0.00388199)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 7.f ) //0.00543478)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8.f ) //0.00621118)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 10.f) //0.00776398)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 33.f) //0.0256211 )

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 3.f )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 33.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 36.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 66.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 96.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 96.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 96.f)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 6.f )// 0.00802139)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 7.f )// 0.00935829)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 8.f )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8.f )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 15.f)// 0.0200535 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 34.f)// 0.0454545 )

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1.f )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1.f )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32.f)// 0.0625    )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)// 0.125     )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66.f)// 0.128906  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67.f)

RESULT
#endif

CHECK(getTotalPenalty(Position i) and operator() FPT single solution)
  AssignBondOrderProcessor testbop;
	testbop.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)

	//testbop.options.dump();

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	TEST_EQUAL(testbop.getNumberOfComputedSolutions(), 10)

	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 2.f)//0.00625) 

	System sys3;
	MOL2File mol_in3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_BEWCUB.mol2), std::ios::in);
	mol_in3 >> sys3;
	sys3.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 0.f)


	System sys15;
	MOL2File mol15(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CUDJAM_sol_5.mol2), std::ios::in);
	mol15 >> sys15;
	sys15.apply(testbop);

	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 2.f ) //0.0015528 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 5.f ) //0.00388199)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 7.f ) //0.00543478)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8.f ) //0.00621118)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 10.f) //0.00776398)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 33.f) //0.0256211 )

	System sys10;
	MOL2File mol10(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CONFAM_sol_4.mol2), std::ios::in);
	mol10 >> sys10;
	sys10.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 3.f )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 33.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 36.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 66.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 96.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 96.f)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(9), 96.f)

	System sys6;
	MOL2File mol6(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_COHKOZ_sol_5.mol2), std::ios::in);
	mol6 >> sys6;
	sys6.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 6.f )// 0.00802139)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 7.f )// 0.00935829)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 8.f )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 8.f )// 0.0106952 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 15.f)// 0.0200535 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 34.f)// 0.0454545 )

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalPenalty(0), 1.f )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(1), 1.f )// 0.00195312)
	TEST_REAL_EQUAL(testbop.getTotalPenalty(2), 32.f)// 0.0625    )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(3), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(4), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(5), 34.f)// 0.0664062 )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(6), 66.f)// 0.125     )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(7), 66.f)// 0.128906  )
	TEST_REAL_EQUAL(testbop.getTotalPenalty(8), 67.f)

RESULT

CHECK(operator() FPT vs A*)
	AssignBondOrderProcessor testbop_a;
	testbop_a.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::A_STAR);
	testbop_a.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	AssignBondOrderProcessor testbop_fpt;
	testbop_fpt.options.set(AssignBondOrderProcessor::Option::ALGORITHM,AssignBondOrderProcessor::Algorithm::FPT);
	testbop_fpt.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	System sys4;
	MOL2File mol4(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_CITSED10_sol_6.mol2), std::ios::in);
	mol4 >> sys4;
	sys4.apply(testbop_a);
	sys4.apply(testbop_fpt);
	TEST_EQUAL(testbop_a.getNumberOfComputedSolutions(), testbop_fpt.getNumberOfComputedSolutions())

	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(0), testbop_fpt.getTotalPenalty(0)) //1.f )
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(1), testbop_fpt.getTotalPenalty(1)) //1.f )
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(2), testbop_fpt.getTotalPenalty(2)) //32.f)
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(3), testbop_fpt.getTotalPenalty(3)) //34.f)
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(4), testbop_fpt.getTotalPenalty(4)) //34.f)
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(5), testbop_fpt.getTotalPenalty(5)) //34.f)
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(6), testbop_fpt.getTotalPenalty(6)) //66.f)
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(7), testbop_fpt.getTotalPenalty(7)) //66.f)
	TEST_REAL_EQUAL(testbop_a.getTotalPenalty(8), testbop_fpt.getTotalPenalty(8)) //67.f)

RESULT


CHECK(getTotalCharge(Position i))
	// This feature is experimental!! 
  AssignBondOrderProcessor testbop;
	testbop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);

	//testbop.options.dump();
	System sys;
	MOL2File mol_in(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AMHTAR01.mol2), std::ios::in);
	mol_in >> sys;
	sys.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), -1.f)
	TEST_REAL_EQUAL(testbop.getTotalCharge(1), -1.f)
	TEST_REAL_EQUAL(testbop.getTotalCharge(2),  1.f)

	System sys2;
	MOL2File mol_in2(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_AN06.mol2), std::ios::in);
	mol_in2 >> sys2;
	sys2.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), -1.f)
	TEST_REAL_EQUAL(testbop.getTotalCharge(1),  1.f)

	System sys3;
	MOL2File mol_in3(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_BEWCUB.mol2), std::ios::in);
	mol_in3 >> sys3;
	sys3.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), 4.f)
	TEST_REAL_EQUAL(testbop.getTotalCharge(2), 2.f)

	System sys40;
	MOL2File mol40(BALL_TEST_DATA_PATH(AssignBondOrderProcessor_test_C3_input.mol2), std::ios::in);
	mol40 >> sys40;
	sys40.apply(testbop);
	TEST_REAL_EQUAL(testbop.getTotalCharge(0), 0.f)
	TEST_REAL_EQUAL(testbop.getTotalCharge(1), 2.f)

RESULT

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



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

