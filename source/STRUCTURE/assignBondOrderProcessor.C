#include <iostream>
#include <map>


// BALL includes
#include <BALL/kernel.h>
#include <BALL/SYSTEM/timer.h>

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/resourceFile.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/STRUCTURE/buildBondsProcessor.h>

// Qt
#include <BALL/VIEW/KERNEL/common.h>
#include <QtXml/QtXml>
#include <Qt/qdom.h>

// For lp_solve
#include <lpsolve/lp_lib.h>

//#define DEBUG 1
#undef DEBUG
//#define DEBUG_READ 1
#undef DEBUG_READ
//#define DEBUG_ESTIMATE
#undef DEBUG_ESTIMATE


using namespace std;
using namespace BALL::VIEW;
namespace BALL 
{	
	const String AssignBondOrderProcessor::Algorithm::A_STAR = "a_star";
	const String AssignBondOrderProcessor::Algorithm::ILP = "ilp";

	const char* AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS = "overwrite_single_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS = "overwrite_double_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS = "overwrite_triple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_QUADRUPLE_BOND_ORDERS = "overwrite_quadruple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_QUADRUPLE_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_AROMATIC_BOND_ORDERS = "overwrite_aromatic_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_AROMATIC_BOND_ORDERS = true;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_CHARGES = "overwrite_existing_charges";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::ASSIGN_CHARGES = "assign_charges";
	const bool  AssignBondOrderProcessor::Default::ASSIGN_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::KEKULIZE_RINGS = "kekulize_aromatic_rings";
	const bool  AssignBondOrderProcessor::Default::KEKULIZE_RINGS = true;

	const char* AssignBondOrderProcessor::Option::ENFORCE_OCTETT_RULE = "enforce_octett_rule";
	const bool  AssignBondOrderProcessor::Default::ENFORCE_OCTETT_RULE = true;

	const char* AssignBondOrderProcessor::Option::INIFile = "iniFile";
	const String  AssignBondOrderProcessor::Default::INIFile = "/bond_lengths/BondOrder.xml";

	const char* AssignBondOrderProcessor::Option::MAX_BOND_ORDER = "max_bond_order";
	const int  AssignBondOrderProcessor::Default::MAX_BOND_ORDER = 3;

	const char* AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS = "max_number_of_solutions";
	const int  AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS = 10;

	const char* AssignBondOrderProcessor::Option::ALGORITHM = "algorithm";
	const String AssignBondOrderProcessor::Default::ALGORITHM = AssignBondOrderProcessor::Algorithm::A_STAR;
		
	const char* AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING = "bond_length_weighting";
	const float AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING = 0.;

	AssignBondOrderProcessor::AssignBondOrderProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			valid_(true),
			atomic_penalty_scores_(),
			bond_free_(),
			bond_to_index_(),
			total_num_of_bonds_(),
			fixed_val_(),
			solutions_(),
			optimal_penalty_(),
			last_applied_solution_(),
			ac_(),	
			max_bond_order_(),
			alpha_(),
			atom_type_normalization_factor_(),
			bond_length_normalization_factor_(),
			queue_()
	{
		setDefaultOptions();
	}

	AssignBondOrderProcessor::AssignBondOrderProcessor(const AssignBondOrderProcessor& abop)
		:	UnaryProcessor<AtomContainer>(abop),
			options(abop.options),
			valid_(abop.valid_),
			atomic_penalty_scores_(abop.atomic_penalty_scores_),	
			bond_free_(abop.bond_free_),
			bond_to_index_(abop.bond_to_index_),
			total_num_of_bonds_(abop.total_num_of_bonds_),
			fixed_val_(abop.fixed_val_),
			solutions_(abop.solutions_),
			optimal_penalty_(abop.optimal_penalty_),
			last_applied_solution_(abop.last_applied_solution_),
			ac_(abop.ac_), 
			max_bond_order_(abop.max_bond_order_),
			alpha_(abop.alpha_),
			atom_type_normalization_factor_(abop.atom_type_normalization_factor_),
			bond_length_normalization_factor_(abop.bond_length_normalization_factor_),
			queue_(abop.queue_)
	{
	}

	AssignBondOrderProcessor::~AssignBondOrderProcessor()
	{
		setDefaultOptions();
	}

	AssignBondOrderProcessor& AssignBondOrderProcessor::operator = (const AssignBondOrderProcessor& abop)
	{
		return *this;
	}

	bool AssignBondOrderProcessor::start()
	{
		solutions_.clear();
		fixed_val_.clear();
		bond_free_.clear();
		atomic_penalty_scores_.clear();
		ac_ = 0;
		valid_ = readAtomPenalties_();
		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);

		return true;
	}

	bool AssignBondOrderProcessor::performAStarStep_()
	{
		// try to find a solution
		while(!queue_.empty())
		{
#ifdef DEBUG
cout << " Next ASTAR step : queue size : " << queue_.size();
#endif

			// take the top entry of the queue
			PQ_Entry_ entry = queue_.top();
			queue_.pop();

#ifdef DEBUG
cout << "  atom type penaty: " << entry.estimated_atom_type_penalty << 
				"  bond length penalty: " << entry.estimated_bond_length_penalty << " ( " ;
for (Size i = 0; i< entry.bond_orders.size(); i++)
{
	cout << " " <<   entry.bond_orders[i] ;
}
cout << ")" << endl;
#endif

			// is this a leaf?
			if (entry.last_bond == total_num_of_bonds_-1)
			{
				// we found a solution
				// store the solution :-)
				Solution_ sol(this, entry);
				solutions_.push_back(sol);

				return true;
			}
			else
			{
				entry.last_bond++;

				// take the next bond and try all bond orders
				if (bond_fixed_[index_to_bond_[entry.last_bond]])
				{	
					// set this fixed bond order
					entry.bond_orders[entry.last_bond] = bond_fixed_[index_to_bond_[entry.last_bond]];

					// estimate the penalty
					if (estimatePenalty_(entry))
					{
						queue_.push(entry);
					}
				}
				else // the next bond is free
				{
					// try all bond orders
					for (int i = 1; i <= max_bond_order_; i++)
					{
						entry.bond_orders[entry.last_bond] = i;

						// penalty estimation went well  
						if (estimatePenalty_(entry))
						{
							// add to the queue 
							queue_.push(entry);
						}
					} // end of all bond orders
				}	// end of free bond	
			} // no valid bond order try -> do nothing
		}

		// if we had found a solution, we would have bailed out already
		return false;
	}

	Processor::Result AssignBondOrderProcessor::operator () (AtomContainer& ac)
	{
#ifdef DEBUG
cout << "  OPTIONS:" << endl;
cout << " \t Algorithm: " <<  options[Option::Option::ALGORITHM] << endl;
cout << " \t Overwrite bonds (single, double, triple, quad, aroma):" 
		 << options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_QUADRUPLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_AROMATIC_BOND_ORDERS) << endl;

//cout << " \t Oktett Regel: " <<options.getBool(Option::ENFORCE_OCTETT_RULE) << endl;
cout << " \t Ladung überschreiben: " << options.getBool(Option::OVERWRITE_CHARGES) << endl;
cout << " \t Ladung zuweisen: " << options.getBool(Option::ASSIGN_CHARGES) << endl;

cout << " \t Kekulizer: " << options.getBool(Option::KEKULIZE_RINGS)  << endl;
cout << " \t Penalty files " << options[Option::Option::INIFile] << endl;
cout << " \t alpha: " << options[Option::Option::BOND_LENGTH_WEIGHTING] << endl;
cout << " \t max bond order " << options[Option::MAX_BOND_ORDER] << endl;
cout << " \t max number of solutions " << options[Option::MAX_NUMBER_OF_SOLUTIONS] << endl;
cout << " \t valid : " << valid_ << endl;
cout << endl;
#endif

		// Is the processor in a valid state?
		if (valid_)
		{
			// Speed up the code by temporarily storing the options locally 
			bool overwrite_single_bonds = options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS);
			bool overwrite_double_bonds = options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS);
			bool overwrite_triple_bonds = options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS);
			bool overwrite_quadruple_bonds = options.getBool(Option::OVERWRITE_QUADRUPLE_BOND_ORDERS);
			bool overwrite_aromatic_bonds = options.getBool(Option::OVERWRITE_AROMATIC_BOND_ORDERS);
			bool enforce_octett_rule = options.getBool(Option::ENFORCE_OCTETT_RULE);

			// What kind of composite do we have?
			if (RTTI::isKindOf<Molecule>(ac))
			{
				// Store the AtomContainer
				ac_ = &ac;

				// Store the total number of bonds
				total_num_of_bonds_ = ac.countBonds();
				
				// Store the Bonds indices 
				index_to_bond_.resize(total_num_of_bonds_, (Bond*)0);

				// Number of fixed bonds
				Size num_fixed_bonds = 0; 

				// Number of atoms in the system
				Position no_atoms = ac.countAtoms();

				// Vector for storing fixed atom valences
				fixed_val_ = std::vector<Position>(no_atoms, 0);

				// Enumerate all bonds
				Index bond_index_counter = 0;

				// Find out, which bonds should be considered in general
				for (Position i = 0; i < no_atoms; ++i)
				{
					Atom* at1 = ac.getAtom(i);
					Position fixed = 0;

					for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
					{
						Bond* bnd = &(*bit);

						// To speed up the computations, assign each bond a unique index
						if (bond_to_index_.find(bnd) == bond_to_index_.end())
						{
							bond_to_index_[bnd] = bond_index_counter;
							index_to_bond_[bond_index_counter] = bnd;
							bond_index_counter++;
						}

						// According to the options and current bond order 
						// a bond is a free variable of the ILP or not.
						// YES: add a variable in the bond side constraint + ????TODO 
						// NO: equality in the bonds side constraint + ?????TODO
						switch (bnd->getOrder())
						{
							case Bond::ORDER__SINGLE:
							{
									if (overwrite_single_bonds)
									{	
// c	out << "overwrite single bonds" << endl;
										bond_free_[bnd] 	= true;
										bond_fixed_[bnd] 	= 0;	 // Remember: 0 = false, 1 = true
									}
									else
									{
//		cout << "don't overwrite single bonds" << endl;
										fixed += 1;
										bond_free_[bnd] 	= false;
										bond_fixed_[bnd] 	= 1;
									}
									break;
							}
							case Bond::ORDER__DOUBLE:
							{
								if (overwrite_double_bonds)
								{
									bond_free_[bnd]	 = true;
									bond_fixed_[bnd] = 0;
								}
								else
								{
									fixed += 2;
									bond_free_[bnd] 	= false;
									bond_fixed_[bnd] 	= 2;
								}
								break;
							}
							case Bond::ORDER__TRIPLE:
							{
								if (overwrite_triple_bonds)
								{
									bond_free_[bnd] = true;
									bond_fixed_[bnd] = 0;
								}
								else
								{	
									fixed += 3;
									bond_free_[bnd] = false;
									bond_fixed_[bnd] = 3;
								}
								break;
							}
							case Bond::ORDER__QUADRUPLE:
							{
								if (overwrite_quadruple_bonds)
								{
										bond_free_[bnd] = true;
										bond_fixed_[bnd] = 0;
								}
								else
								{	
									fixed += 4;
									bond_free_[bnd] = false;
									bond_fixed_[bnd] = 4;
								}
								break;
							}
							case Bond::ORDER__AROMATIC:
							{
								if (overwrite_aromatic_bonds)
								{
									bond_free_[bnd] 	= true;
									bond_fixed_[bnd] 	= 0; 
								}
								else
								{	
									fixed += 1;         /// TODO: wie soll das im ILP dargestellt werden? 1,5?	
									bond_free_[bnd] 	= false;
									bond_fixed_[bnd] 	= 1; 
								}
								break;
							}	
							default: //Bond::ORDER__UNKNOWN:
							{
									bond_free_[bnd] = true;
									bond_fixed_[bnd] = false;  //0
							}
						}
					}

					fixed_val_[i] = fixed;
					num_fixed_bonds += fixed;
				}
				
				num_of_free_bonds_ = total_num_of_bonds_ - num_fixed_bonds;
#ifdef DEBUG
cout << "preassignPenaltyClasses_:" << preassignPenaltyClasses_() << " precomputeBondLengthPenalties_:" << precomputeBondLengthPenalties_() << endl;
#endif


				// Generate penalty values for all atoms in the AtomContainer ac
				calculateAtomPenalties_(ac); // TODO: Umstellung auf readAtomPenalties!
				if (preassignPenaltyClasses_() && precomputeBondLengthPenalties_())
				{
					if (options.get(Option::ALGORITHM) == Algorithm::A_STAR)
					{
						// Initialize a priority queue and try to find a first solution
						// Further solutions will be computed calling the method computeNextSolution

						// Initialize the priority queue
						PQ_Entry_ entry(alpha_, atom_type_normalization_factor_, bond_length_normalization_factor_);
						entry.bond_orders.resize(total_num_of_bonds_,0);
						entry.last_bond = 0;

						// TODO: can we improve the heuristic by pre-setting all 
						// fixed bonds in the bond_order datastructure? 

						// Is the first bond fixed?
						if (bond_fixed_[index_to_bond_[entry.last_bond]])
						{
							// Store this bond order
							entry.bond_orders[entry.last_bond] = bond_fixed_[index_to_bond_[entry.last_bond]];

							// Estimate the penalty and add the node into the queue if valid
							if (estimatePenalty_(entry))
							{
								queue_.push(entry);
							}
						}
						else // Bond is free -> try all bond orders
						{
							for (int i = 1; i <= max_bond_order_; i++)
							{
								// Set the bond order
								entry.bond_orders[0] = i;

								// Estimate the penalty and add the node into the queue if valid
								if (estimatePenalty_(entry))
								{
									queue_.push(entry);
								}
							}
						}

#ifdef DEBUG
cout << "\nNach initialisierung : queue siue = " << queue_.size() << endl;
#endif

						// Try to find a first solution
						bool found_a_sol = performAStarStep_();

						// Do we have a solution? 
						if (!found_a_sol)
						{
							Log.info() << "AssignBondOrderProcessor: No solution found!" << endl;
						}	
					}
					else // Solve a ILP
					{
						// Get a first solution
						solutions_.push_back(Solution_(this, ac));
						if (solutions_[0].valid)
						{
							last_applied_solution_ = 0;
							apply(0);	
						}
						else
						{
							last_applied_solution_=-1;
						}

						if ( (solutions_.size() > 0) 
								//&&  (total_num_of_bonds_ - num_fixed_bonds < MAX__SOLUTIONS) 
								&&  (solutions_[0].valid)
							 )
						{
							/*if (options.get(Option::COMPUTE_ALL_SOLUTIONS) == ComputeAllSolutions::ONE_BOND_HEURISTIC)
							{

								// now iterate over all free bonds and inhibit the order we already found
								map<Bond*, bool>::iterator it = bond_free_.begin();
								for (; it!= bond_free_.end(); it++)
								{
									if (it->second)
									{	
										// We want to inhibit the current bond to have the order, we already found
										// Since lpsolve does not offer constraints of type unequal, we have to check
										// if there is a solution with constraint x_bond LE order-1  AND x_bond GE order+1 
										// the type of the extra constraint is controlled by the last parameter value.

										Solution_ solutionLE; 
										Solution_ solutionGE; 

										// case LE order-1
										if (solutions_[0].bond_orders[it->first]-1 > 0)
										{	
											solutionLE = Solution_(this, ac, it->first, solutions_[0].bond_orders[it->first], true);
										}
										// case GE order+1
										if (solutions_[0].bond_orders[it->first]+1 < 5)
										{	
											solutionGE = Solution_(this, ac, it->first, solutions_[0].bond_orders[it->first], false);
										}

										if (solutionLE.valid)
										{
											if (solutionGE.valid)
											{
												if (solutionLE.penalty <= solutionGE.penalty)
												{
													solutions_.push_back(solutionLE);  
												}
												if (solutionLE.penalty >= solutionGE.penalty)
												{
													solutions_.push_back(solutionGE);  
												}
											}
											else
											{
												solutions_.push_back(solutionLE);  
											}
										} // solution_LE not valid
										else
										{
											if (solutionGE.valid)
											{			
												solutions_.push_back(solutionGE);  
											}
										}
									}
									else
									{
										Log.error() << "There is a unknown bond" << endl;
									}
								} // end of all bonds

								if (!options.getBool(Option::KEKULIZE_RINGS))
								{
									// find all rings
									vector<vector<Atom*> > rings;
									RingPerceptionProcessor rpp;
									rpp.calculateSSSR(rings, ac);

									// set the aromatic rings	
									AromaticityProcessor ap;
									ap.aromatize(rings, ac);
								}

								// set the last solution // TODO: Do we want to apply the LAST solution?
								last_applied_solution_ = solutions_.size()-1;
								apply(solutions_.size()-1);
							}
							else if (options.get(Option::COMPUTE_ALL_SOLUTIONS) == ComputeAllSolutions::ENUMERATION_TREE)
							{
								/ * // print the optimal sol
									HashMap<Bond*, int>::Iterator mit = solutions_[0].bond_orders.begin();
									for (; mit != solutions_[0].bond_orders.end(); ++mit)
									{
									cout << bond_to_index_[mit->first] << "---"<< mit->second <<  " " << 
									mit->first->getFirstAtom()->getName() << ".."<<	mit->first->getSecondAtom()->getName()   << std::endl;
									}
									cout << std::endl; * /
								// get the current optimum
								optimal_penalty_ = solutions_[0].penalty;

								// prepare the compare String
								map<Bond*, bool>::iterator it = bond_free_.begin();
								for (; it!= bond_free_.end(); it++)
								{
									current_orders_ +=" ";
								}
								it = bond_free_.begin();

								for (; it!= bond_free_.end(); it++)
								{
									if (it->second && !(  (it->first->getFirstAtom()->getElement() ==PTE[Element::H])
												||( it->first->getSecondAtom()->getElement()==PTE[Element::H]) 
												)
										 )
									{	
										// denote the bond as fixed
										bond_free_[it->first]= false;
										for (int order = 1; order <= 4; order++)
										{	
											//	//cout << "vorher: " << current_orders_ << "-----" << bond_to_index_[it->first] << std::endl;//"--" << order << "--" << char(order) << std::endl;
											// denote the choosen bond order in the order string
											current_orders_[bond_to_index_[it->first]] = '0'+order;
											//	//cout << "nachher: " << current_orders_ << "-----" << bond_to_index_[it->first] << "--" << (current_orders_[bond_to_index_[it->first]]) << "--" << std::endl;

											// prohibit the bonds order to get the order found on the last (optimal) solution
											if (   (solutions_[0].bond_orders[it->first] != order)
													&& (checked_.find(current_orders_) == checked_.end()))
											{

												//cout << "Bond " << bond_to_index_[it->first] << "--Order " << order << " situation: " << current_orders_ << std::endl;

												// denote the choosen bond order in the atoms fixed valences
												fixed_val_[it->first->getFirstAtom()->getIndex()] += order;
												fixed_val_[it->first->getSecondAtom()->getIndex()] += order;

												//current_orders_.set(String(order), bond_to_index_[it->first], bond_to_index_[it->first]);
												// try to solve this bond order
												recursive_solve_(ac, 0);

												// remember, that we already tried this combination
												checked_.insert(current_orders_);

												// clean up the order string 
												current_orders_[bond_to_index_[it->first]] = ' ';

												//current_orders_.set(" ", bond_to_index_[it->first], bond_to_index_[it->first]);

												// reset the fixed valences
												fixed_val_[it->first->getFirstAtom()->getIndex()] -= order;
												fixed_val_[it->first->getSecondAtom()->getIndex()] -= order;
											}
										}
										// reset the bond status
										bond_free_[it->first] = true;
									}
								}	

							}*/
						}
						else
						{	
							cout << "No solution compted! -solsize:" <<  solutions_.size() << " - no free bonds:" <<  total_num_of_bonds_ - num_fixed_bonds << " - valid " << (solutions_.size()>0 ? solutions_[0].valid : false) << std::endl;
							//cout << "Too many bonds " << total_no_bonds - num_fixed_bonds << std::endl;
						}
					}

					if (solutions_.size() > 0)
					{	
						last_applied_solution_ = 0;
						// set the bond orders of the first solution
						AtomIterator a_it = ac.beginAtom();
						Atom::BondIterator b_it = a_it->beginBond();
						BALL_FOREACH_BOND(ac, a_it, b_it)
						{
							b_it->setOrder(solutions_[0].bond_orders[&(*b_it)]);
						}
					}
				}
			}
		}
		return Processor::CONTINUE;
	}
	
	bool AssignBondOrderProcessor::finish()
	{		
		return true;
	}




	bool AssignBondOrderProcessor::estimatePenalty_(PQ_Entry_& entry)
	{
#if defined DEBUG || defined DEBUG_ESTIMATE
//#ifdef DEBUG
cout << " - - PE - - - - - - - - " << endl;
#endif

		AtomIterator a_it = ac_->beginAtom();	
		Atom::BondIterator b_it; 
		int valence = 0; 			// the so far fixed valence of the considered atom
		bool closed = true;  	// are all bonds of this atom fixed? 
		float estimated_atom_penalty = 0.;
		float estimated_bond_penalty = 0.;
		float current_bond_length_penalty = 0; // lenght penalty of the bonds already set
		bool valid = true;
		int num_free_bonds = 0;

		// NOTE: this variable indexes the currently addressed atom. We cannot rely on
		// 			 getIndex for this task since this is relative to the static atom array,
		// 			 not to the current atom container!
		Index current_atom_index = 0; 

		// evaluate all atom type and bond length penalties
		for (; a_it != ac_->endAtom() && valid ; a_it++, current_atom_index++)
		{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " ++A+++++++++++++++" << endl;
#endif
			// store the free bonds for bond length penalty calculation
			vector<Bond*> free_bonds;
			valence = 0; 	                     // sum of bond orders of all fixed bonds  
			current_bond_length_penalty = 0;   // sum of all bond length penalties "
			closed = true;
			num_free_bonds = 0;

			for (b_it = a_it->beginBond(); +b_it; b_it++) 
			{				
				// do we have a valid bond? 
				if (   (bond_to_index_.find(&*b_it) != bond_to_index_.end())
						&& (bond_lengths_penalties_.find(&*b_it)) != bond_lengths_penalties_.end())
				{
					// is this bond already fixed?
					if (bond_to_index_[&*b_it] <= (Index)entry.last_bond)
					{
							
						int order = entry.bond_orders[bond_to_index_[&*b_it]];
						valence += order;
						
						// store the bond length penalty
						current_bond_length_penalty += bond_lengths_penalties_[&*b_it][order]; 

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  * fixed bond num " << bond_to_index_[&*b_it] << " (" << b_it->getFirstAtom()->getFullName() << "-" << b_it->getSecondAtom()->getFullName() <<")"
				<< " by order " << entry.bond_orders[bond_to_index_[&*b_it]]  
				<< " :bond len penalty: " << bond_lengths_penalties_[&*b_it][order] 
				<< " -> cur bond len pen:"<< current_bond_length_penalty << endl;
#endif

					}
					else
					{
						closed = false;
						num_free_bonds++;
						free_bonds.push_back(&*b_it);
					}
				}
				else
				{
					Log.error() << "There was an error in AssignBondOrderProcessor" << endl;
				}
			}
			// every fixed bond was counted twice --> undo
			current_bond_length_penalty = current_bond_length_penalty /2.;


	/*	// TODO: are there exceptions from the octett rule? 
			// Yes: P<5, S<6
			if ( options.getBool(Option::ENFORCE_OCTETT_RULE) && 
					 (  (a_it->getElement().getSymbol()=="S" && valence >6)
						||(a_it->getElement().getSymbol()=="P" && valence >5)
						||(a_it->getElement().getSymbol()!="P" && a_it->getElement().getSymbol()!="S" 
							 && valence >4)
					 )
				 )
			{
				return false;
			}*/

			// Now all orders of already fixed bonds are summed up in valence
			// and all bond length deviation penalties are summed up in current_bond_length_penalty
			
			// Remember, we start counting with 0
			if (current_atom_index >= atom_to_block_.size())
			{
				Log.error() << "AssignBondOrderProcessor: No penalty type found for atom " << a_it->getFullName() << " with index " << endl;
				return false; 
			}

			int block = atom_to_block_[current_atom_index];
			// This should not happen, but who knows ...
			if (block == -1)
			{
				Log.error() << "AssignBondOrderProcessor: No penalty type found for atom " << a_it->getFullName() << endl;
				return false; 
			}

#if defined DEBUG 
cout << "  * atom " << a_it->getFullName() << " is block " << block +1 << " : " 
		 <<  block_to_start_valence_[block] << " " << block_to_length_[block] 
		 << " " <<  block_to_start_idx_[block] << endl;
#endif

			int current_start_valence = block_to_start_valence_[block];
			int current_block_length  = block_to_length_[block];
			int current_end_valence   = current_start_valence + current_block_length-1;
			int current_start_index   = block_to_start_idx_[block];

			// is the valence found already greater than the largest for this atom type?
			if (valence + num_free_bonds > current_end_valence)
			{

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  * valence explosion for atom " <<   a_it->getFullName() << endl;
#endif
				return false;
			}

			// are all bonds of this atom already fixed?
			if (closed)
			{

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  * atom " <<   a_it->getFullName() << " closed with valence " << valence ;
#endif
				// is the valence large enough for this atom type?
				if (valence >= current_start_valence)
				{
					// add the actual penalty
					estimated_atom_penalty   +=  penalties_[current_start_index + valence - current_start_valence];
					estimated_bond_penalty   +=  current_bond_length_penalty; 

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " and atom type penalty +" << penalties_[current_start_index + valence - current_start_valence] << " = " << estimated_atom_penalty 
	   << " and bond length penalty +" << current_bond_length_penalty << " = " << estimated_bond_penalty << endl;
#endif
				}
				else
				{

#ifdef DEBUG
cout << " but too small" << endl;
#endif
					return false;
				}
			}
			else // the atom is not yet closed => heuristic kicks in
			{

#ifdef DEBUG
cout << "  * atom " <<   a_it->getFullName() << " has to be evaluted starting from valence " << valence << " ("; 
#endif

				// the minimal penalty of all possible valence states for this atom
				float min_atom_type_penalty = std::numeric_limits<float>::max();
			  float min_bond_length_penalty = std::numeric_limits<float>::max();

				// atom type penalty estimator
				float estimated_atom_type_penalty_per_valence = 0;
				float min_bond_length_penalty_per_valence = 0;

				// we have to estimate the penalty heuristically 
				// at which penalty position are we at the moment?
				Position i = (valence + num_free_bonds < current_start_valence) 
										? current_start_valence
										: valence + num_free_bonds;

#if defined DEBUG || defined DEBUG_ESTIMATE				
cout << " test valence from i: " << i << " up to current end valence " << current_end_valence << endl;
#endif
				// for every remaining valence of the atom under consideration
				// (we know there is at least one)
				for (; i <= (Position)current_end_valence; i++)
				{

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << " valence: " << i<< endl;
#endif
					// first estimate the bond-length-penalty for choice valence=i by
					min_bond_length_penalty_per_valence = 0;

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << "      free bonds: " <<  free_bonds.size() << endl;
#endif
					// iterating over all free bonds 
					// (we know there is at least one free bond)
					for (Size fb=0; fb < free_bonds.size(); fb++)
					{
						// and computing the minimal bond length penalty for each bond
						float min_bond_length_penalty_per_bond = std::numeric_limits<float>::max();
						// Note: for simplification we do not share the free bond 
						//       orders correctly! We just find the min!
						// the number of the remaining bond orders to be assigned 
						// to the free bonds are : i - valence -(num_free_bonds-1)
						std::vector<float>& current_bond_length_penalties = bond_lengths_penalties_[free_bonds[fb]];
						int up_to = std::min(current_bond_length_penalties.size()-1, i-valence-(num_free_bonds-1));

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << "            fb:"<< fb <<"  up_to" << up_to << endl;
#endif

						for (Size j = 1; j <= up_to; j++)
						{
							float deviation = current_bond_length_penalties[j]; 

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << "               deviation:" << deviation << " current min len pen per bond: " << min_bond_length_penalty_per_bond ;
#endif

							if (deviation < min_bond_length_penalty_per_bond )
							{
								min_bond_length_penalty_per_bond = deviation;
							}

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << "--->" << 	min_bond_length_penalty_per_bond << endl;
#endif

						}
						min_bond_length_penalty_per_valence += min_bond_length_penalty_per_bond;
					}

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << "      min for given bond and valence: "	<<	min_bond_length_penalty_per_valence << " bisheriges min: " << min_bond_length_penalty;
#endif

					// has this choice of valence i a minimal bond length penalty?
					if (min_bond_length_penalty_per_valence < min_bond_length_penalty)
					{
						min_bond_length_penalty = min_bond_length_penalty_per_valence;
					}

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << " neues min:" << min_bond_length_penalty << endl;
#endif

#if defined DEBUG 
cout << i<< "-" << current_start_index + i - current_start_valence << "-"<< penalties_[current_start_index + i - current_start_valence] <<  " , ";
#endif
					// secondly: estimate the atom_type penalty
					// Remember: the current_start_index corresponds to the current_start_valence
					estimated_atom_type_penalty_per_valence = penalties_[current_start_index + i - current_start_valence];
					if (estimated_atom_type_penalty_per_valence < min_atom_type_penalty)
					{
						 min_atom_type_penalty = estimated_atom_type_penalty_per_valence;
					}
				}

#if defined DEBUG || defined DEBUG_ESTIMATE	
cout << " min for given valence:" << min_bond_length_penalty << " + " << current_bond_length_penalty ;
#endif

				//now add the minima to the atomcontainer's penalty sums
				estimated_atom_penalty += min_atom_type_penalty;
				estimated_bond_penalty += min_bond_length_penalty + current_bond_length_penalty;	

#if defined DEBUG || defined DEBUG_ESTIMATE	
cout << " => " << 	estimated_bond_penalty  << endl;
#endif
			} // end of heuristic
		} // end of for all atoms

		// store the computed atom type penalty
		entry.estimated_atom_type_penalty = estimated_atom_penalty;
		
		//store the computed bond length penalty
		entry.estimated_bond_length_penalty = estimated_bond_penalty;

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << " End of estimator for ";
for (Size i=0; i < entry.bond_orders.size(); i++)
{
	cout << " " << entry.bond_orders[i];
}
	
cout << " atom type pen: " << entry.estimated_atom_type_penalty << " bond len pen: "<< entry.estimated_bond_length_penalty << endl;
#endif

		return true;
	}
	

	void AssignBondOrderProcessor::calculateAtomPenalties_(AtomContainer& ac)
	{
		// This method computes for every atom its possible atomic valences 
		// and the corresponding possible atomic penalty scores and stores them 
		// per atom in the vector atomic_penalty_scores_
		
		const Atom* partner_atom;	
		const Atom* sec_partner_atom;	
		const Atom* other_partner_atom;	
		AtomIterator atom_it;	//iterates over all atoms in the observed molecule
		Bond* bond;	//bond of the observed atom
		Bond* next_bond;	//another bond of the observed atom
		Bond* partner_bond;	//bond of the partner_atom
		Bond* next_partner_bond;	//next bond of the partner_atom	
		Atom::BondIterator bond_it;	//iterate over all bonds of one atom
		Atom::BondIterator bond_it2;
		RingPerceptionProcessor(ringPP);	//marks all atoms in a ring structure with property "InRing"
		pair <int, int > atomic_score;	//pair of atomic valence and corresponding atomic penalty score
		vector <pair <int, int > > aps_vector;	//vector of atomic_scores

		//mark all atoms in a ring structure with property "InRing"
		ac.apply(ringPP);	
		atom_it = ac.beginAtom();	

		//if bonds exist
		if (ac.countBonds() != 0)
		{
			//assign to each atom in molecule a penalty score if necessary
			for (;+atom_it;++atom_it)
			{
				aps_vector.clear();
				//Oxygen
				if (atom_it->getElement().getName()=="Oxygen")
				{
					//O(x1)
					if (atom_it->countBonds() == 1)
					{
						bond_it = atom_it->beginBond();
						if (bond_it != atom_it->beginBond())
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							//O(x1) in pyridine-1-oxide
							if (((partner_atom->getElement().getName()=="Nitrate") or (partner_atom->getElement().getName()=="Nitrogen")) and partner_atom->getProperty("InRing").getBool())
							{
								atomic_score.first = 1;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 2;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);
							}
							else 
							{
								atomic_score.first = 1;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 2;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 3;
								atomic_score.second = 64;
								aps_vector.push_back(atomic_score);
							}
						}
					}
					//O(x2)
					else if (atom_it->countBonds() == 2)
					{
						atomic_score.first = 1;
						atomic_score.second = 32;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 2;
						atomic_score.second = 0;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 3;
						atomic_score.second = 64;
						aps_vector.push_back(atomic_score);
					}
				}

				//H, F, CL, Br, I
				if ((atom_it->getElement().getName()=="Hydrogen") or (atom_it->getElement().getName()=="Fluorine") or 		(atom_it->getElement().getName()=="Iodine ") or (atom_it->getElement().getName()=="Chlorine") or 	(atom_it->getElement().getName()=="Bromine") )
				{
					atomic_score.first = 1;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 0;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 2;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);
				}


				//Carbon
				if (atom_it->getElement().getName()=="Carbon")
				{
					if (atom_it->countBonds() ==1)
					{
						//C in CN-R
						bond_it = atom_it->beginBond();
						if (bond_it != atom_it->endBond())
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Nitrate") or (partner_atom->getElement().getName()=="Nitrogen"))
							{
								if (partner_atom->countBonds() == 2)
								{
									atomic_score.first = 3;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 4;
									atomic_score.second = 1;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 5;
									atomic_score.second = 32;
									aps_vector.push_back(atomic_score);				
								}
							}
							//C(x1)		
							else 
							{
								atomic_score.first = 3;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 4;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 5;
								atomic_score.second = 32;
								aps_vector.push_back(atomic_score);
							}
						}
					}
					//C in COO-
					bond_it = atom_it->beginBond();
					if (bond_it != atom_it->endBond())
					{
						partner_atom = bond_it->getBoundAtom(*atom_it);
						if (partner_atom->getElement().getName() =="Oxygen")
						{
							sec_partner_atom = (++bond_it)->getBoundAtom(*atom_it);
							if (sec_partner_atom->getElement().getName() =="Oxygen")
							{
								if ((partner_atom->countBonds() == 1) || (sec_partner_atom->countBonds() ==1))
								{
									atomic_score.first = 5;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 4;
									atomic_score.second = 32;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 6;
									atomic_score.second = 32;
									aps_vector.push_back(atomic_score);
								}		
							}
						}
						//if atom type still not found
						if (aps_vector.empty())
						{
							//C
							atomic_score.first = 2;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);
						}		
					}
				}

				// Si
				if ((atom_it->getElement().getName()== "Silicium") || (atom_it->getElement().getName()== "Silicon"))
				{
					atomic_score.first = 4;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);
				}



				//N(x1) in N=N=R
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds() ==1))
				{
					bond = &(*(atom_it->beginBond()));
					if (atom_it->beginBond() != atom_it->endBond())
					{
						partner_atom = bond->getBoundAtom(*atom_it);
						if ((partner_atom->getElement().getName()=="Nitrate") || (partner_atom->getElement().getName()=="Nitrogen"))
						{
							if (partner_atom->countBonds()==2)
							{  
								partner_bond = const_cast <Bond*> (&(*(partner_atom->beginBond())));
								next_partner_bond = const_cast <Bond*> (&(*(++(partner_atom->beginBond()))));
								const Atom* next_partner_atom = next_partner_bond->getBoundAtom(*partner_atom);
								if (next_partner_atom->countBonds() == 1)
								{
									atomic_score.first = 2;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 3;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);
								}
							}
						}
						//N(x1)
						if (aps_vector.empty())
						{
							atomic_score.first = 2;
							atomic_score.second = 3;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}	
					}		
				}
				//N(x2) in N=N=R
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds()== 2))
				{
					bond = &(*(atom_it->beginBond()));
					if (atom_it->beginBond() != atom_it->endBond())
					{
						next_bond = const_cast<Bond*>(&(*(++(atom_it->beginBond()))));
						partner_atom = bond->getBoundAtom(*atom_it);
						other_partner_atom = next_bond->getBoundAtom(*atom_it);
						//if one of the neighbours is a Nitrate
						if ((partner_atom->getElement().getName()=="Nitrate") || (partner_atom->getElement().getName()=="Nitrogen"))
						{
							//check bonds (x1 of N)
							if (partner_atom->countBonds()==1)	
							{ 
								atomic_score.first = 3;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 4;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);							
							}
						}
						if ((other_partner_atom->getElement().getName()=="Nitrate") || (other_partner_atom->getElement().getName()=="Nitrogen"))
						{
							//check bonds (x2 of N)
							if (other_partner_atom->countBonds()==1)
							{
								atomic_score.first = 3;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 4;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);
							} 
						}
					}
					//N(x2)
					if (aps_vector.empty())
					{
						atomic_score.first = 2;
						atomic_score.second = 4;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 3;
						atomic_score.second = 0;
						aps_vector.push_back(atomic_score);

						atomic_score.first = 4;
						atomic_score.second = 2;
						aps_vector.push_back(atomic_score);
					}
				}
				//N(x3) in nitro
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds()==3))
				{
					bond_it = atom_it->beginBond();
					if (atom_it->countBonds() > 0)
					{
						int count_oxygen_atoms= 0;
						for (;bond_it != atom_it->endBond();++bond_it)
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Oxygen") && (partner_atom->countBonds()==1 ))
							{
								++count_oxygen_atoms; 
							}
						}
						if (count_oxygen_atoms==2)
						{
							atomic_score.first = 3;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}
						//N(x3) in pyridine-1-oxide
						else 
						{
							bond_it2 = atom_it->beginBond();
							if (atom_it->countBonds() > 0)
							{
								Size count_oxygen_atoms=0;
								Size countCarb=0;
								for (;bond_it2 != atom_it->endBond();++bond_it2)
								{
									partner_atom = bond_it2->getBoundAtom(*atom_it);
									if (partner_atom->getElement().getName()=="Carbon" and partner_atom->getProperty("InRing").getName()=="InRing")
									{
										++countCarb;
									}
									if (partner_atom->getElement().getName()=="Oxygen")
									{
										++count_oxygen_atoms;
									}
								}
								if (count_oxygen_atoms==1 and countCarb==2)	
								{
									atomic_score.first = 3;
									atomic_score.second = 1;
									aps_vector.push_back(atomic_score);

									atomic_score.first = 4;
									atomic_score.second = 0;
									aps_vector.push_back(atomic_score);
								}
							}
						}
						//N(x3)
						if (aps_vector.empty())
						{
							atomic_score.first = 2;
							atomic_score.second =32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 1;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 2;
							aps_vector.push_back(atomic_score);
						}
					}	
				}
				//N(x4)
				if (((atom_it->getElement().getName()=="Nitrate") || (atom_it->getElement().getName()=="Nitrogen")) && (atom_it->countBonds()==4))
				{
					atomic_score.first = 2;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);		

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 64;
					aps_vector.push_back(atomic_score);
				}


				//P(x1)
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds() ==1))
				{
					atomic_score.first = 2;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);
				}
				//P(x2)
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds()==2))
				{
					atomic_score.first = 2;
					atomic_score.second = 4;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);
				}
				//P(x3)
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds()==3))
				{
					atomic_score.first = 2;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 1;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 5;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);
				}
				//P(x4) 
				if ((atom_it->getElement().getName()=="Phosphorus") && (atom_it->countBonds()==4))
				{
					bond_it = atom_it->beginBond();
					if (atom_it->countBonds() > 0)
					{
						Size count_oxygen_atoms = 0;
						Size count_sulfur_atoms = 0;
						for (;bond_it != atom_it->endBond(); ++bond_it)
						{
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Oxygen") && (partner_atom->countBonds()== 1))
							{
								++count_oxygen_atoms;
							}
							else if (((partner_atom->getElement().getName()=="Sulphur") ||  (partner_atom->getElement().getName()=="Sulfur")) && (partner_atom->countBonds()== 1))
							{
								++count_sulfur_atoms;
							}					
						}
						//bonded to 2 O(x1) or 2 S(x1)
						if (count_oxygen_atoms==2 or count_sulfur_atoms==2)
						{
							atomic_score.first = 5;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}	
						//bonded to 3 O(x1) or 3 S(x1)
						if (count_oxygen_atoms==3 or count_sulfur_atoms==3)
						{
							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}
						//P(x4)
						if (aps_vector.empty())
						{
							atomic_score.first = 3;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 4;
							atomic_score.second = 1;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}
					}
				}


				//S(x1)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==1))
				{
					bond_it = atom_it->beginBond();
					if (bond_it != atom_it->endBond())
					{
						partner_atom = bond_it->getBoundAtom(*atom_it);
						//S(x1) in pyridine-1-thiol anion
						if (((partner_atom->getElement().getName()=="Nitrate") || (partner_atom->getElement().getName()=="Nitrogen")) && (partner_atom->getProperty("InRing").getName() =="InRing"))
						{
							//check if Nitrate is inRing of Carbons
							const Atom* partner_of_partner_atom;
							Size count_carbon_atoms = 0;

							for (;+bond_it2;++bond_it2)
							{
								partner_of_partner_atom = bond_it2->getBoundAtom(*partner_atom);
								if (partner_of_partner_atom->getElement().getName() == "Carbon")
								{
									++count_carbon_atoms;
								}
							}
							if ((partner_atom->countBonds()== 2) && (count_carbon_atoms >= 1))
							{
								atomic_score.first = 1;
								atomic_score.second = 0;
								aps_vector.push_back(atomic_score);

								atomic_score.first = 2;
								atomic_score.second = 1;
								aps_vector.push_back(atomic_score);
							}
						}
						//S(x1)
						if (aps_vector.empty())
						{
							atomic_score.first = 1;
							atomic_score.second = 2;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 2;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 3;
							atomic_score.second = 64;
							aps_vector.push_back(atomic_score);
						}
					}
				}

				//S(x2)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==2))
				{
					atomic_score.first = 1;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 2;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 3;
					atomic_score.second = 32;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 1;
					aps_vector.push_back(atomic_score);
				}

				//S(x3)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==3))
				{
					atomic_score.first = 3;
					atomic_score.second = 1;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 4;
					atomic_score.second = 0;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 5;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);

					atomic_score.first = 6;
					atomic_score.second = 2;
					aps_vector.push_back(atomic_score);
				}



				//S(x4)
				if (((atom_it->getElement().getName()=="Sulphur") ||  (atom_it->getElement().getName()=="Sulfur")) && (atom_it->countBonds()==4))
				{
					bond_it = atom_it->beginBond();
					if (atom_it->countBonds() > 0)
					{
						Size count_oxygen_atoms = 0;
						Size count_sulfur_atoms = 0;
						for (;bond_it != atom_it->endBond(); ++bond_it){
							partner_atom = bond_it->getBoundAtom(*atom_it);
							if ((partner_atom->getElement().getName()=="Oxygen") && (partner_atom->countBonds()== 1))
							{
								++count_oxygen_atoms;
							}
							else if (  (  (partner_atom->getElement().getName()=="Sulphur") 
											    ||(partner_atom->getElement().getName()=="Sulfur") ) 
									    && (partner_atom->countBonds()== 1))
							{
								++count_sulfur_atoms;
							}					
						}
						//bonded to 2 O(x1) or S(x1)
						if (count_oxygen_atoms ==2 or count_sulfur_atoms==2)
						{
							atomic_score.first = 6;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);
						}	
						//bonded to 3 O(x1) or S(x1)
						if (count_oxygen_atoms== 3 or count_sulfur_atoms==3)
						{
							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}
						//bonded to 4 O(x1) or S(x1)
						if (count_oxygen_atoms==4 or count_sulfur_atoms==4)
						{
							atomic_score.first = 6;
							atomic_score.second = 32;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 7;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}		
						//S(x4)
						if (aps_vector.empty())
						{
							atomic_score.first = 4;
							atomic_score.second = 4;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 5;
							atomic_score.second = 2;
							aps_vector.push_back(atomic_score);

							atomic_score.first = 6;
							atomic_score.second = 0;
							aps_vector.push_back(atomic_score);
						}	
					}
				}
				atomic_penalty_scores_.push_back(aps_vector);
			}
		}
	}
	
	bool AssignBondOrderProcessor::readAtomPenalties_()	
		throw(Exception::FileNotFound())
	{
		// Open parameter file
		Path    path;
		String  inifilename(path.find(options[Option::Option::INIFile]));
		
		if (inifilename == "") 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, options[Option::Option::INIFile]);
		}

		QString errorStr;
		int errorLine;
		int errorColumn;

		QFile file((inifilename.c_str()));
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			Log.error() << "AssignBondOrderProcessor: cannot read file " << inifilename << std::endl;
			Log.error() << "Reason was: " << ascii(file.errorString()) << std::endl;
			return 1;
		}

		// read the document
		QDomDocument domDocument;
		if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
					&errorColumn)) 
		{
			Log.error() << "Parse error in line " << errorLine << " column " << errorColumn <<  " of file " << inifilename << endl;
			Log.error() << "Reason was: " << ascii(errorStr) << std::endl;
			return 1;
		}

		// get the root element...
		QDomElement root = domDocument.documentElement();	
		
		// ... and get all entries
		QDomNodeList entries = domDocument.elementsByTagName("entry");
		for (unsigned int i= 0; i < entries.length(); i++)
		{
			pair<String, String> tmp;
			int start_valence;
			QDomNodeList penalties; 
			Position start_idx;

			// get the element type (tag elementstring) 
			// NOTE: each entry should have just ONE element tag)
			QDomNodeList elementstrings = entries.item(i).toElement().elementsByTagName("elementstring");
			if (elementstrings.length() ==1)
			{
				// read the element type
				QDomNode element = elementstrings.item(0);
				tmp.first = ascii(element.firstChild().nodeValue()); 

				// read the SMARTS-string
				QDomNodeList smartstring =  entries.item(i).toElement().elementsByTagName("smartstring");
				if (smartstring.length() == 1)
				{
					tmp.second = ascii(smartstring.item(0).toElement().firstChild().nodeValue());
				} 
				else if (smartstring.length() == 0)
				{
					Log.warn() << "In file " << inifilename << " : no SMARTS-string found for element " << ascii(element.firstChild().nodeValue()) << endl;
				}  
				else
				{
					Log.error() <<  "Parse error in file " << inifilename << " : more than on3 SMARTS-string for element " << ascii(element.firstChild().nodeValue()) << endl;
					return false;
				}

				// now read the penalties
				penalties =  entries.item(i).toElement().elementsByTagName("penalty");
				if (penalties.length() > 0)
				{
					start_valence = (penalties.item(0).toElement().attribute("valence")).toInt();
					start_idx = penalties_.size();
					for (unsigned int k = 0; k < penalties.length(); k++)
					{
						// NOTE: we assume, that the valences come "en block" 
						// without any leftouts 
						penalties_.push_back((penalties.item(k).toElement().firstChild().nodeValue()).toInt());
					}
				}
				else
				{
					Log.error() << "In file " << inifilename << " : no penalties found for element " << ascii(element.firstChild().nodeValue()) << endl;
					return false;
				}
			}
			else
			{
				Log.error() << "Parse error in file " << inifilename << endl;
				return false;
			}

			// now store the entry
			block_definition_.push_back(tmp);
			block_to_length_.push_back(penalties.length());
			block_to_start_idx_.push_back(start_idx);
			block_to_start_valence_.push_back(start_valence);

		} // next block

	
		return true;
	}
	
	bool AssignBondOrderProcessor::precomputeBondLengthPenalties_()	
	{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " start AssignBondOrderProcessor::precomputeBondLengthPenalties_:   " << endl;
#endif

		// get the relevant options
		// this is also done in the start-method, 
		// but when performing an evalution the start-method is not called!
		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);

		// initialize the inverse bond length penalty normalization factor
		bond_length_normalization_factor_ = 0.;
		
		if (!valid_)
		{	
			Log.error() << "AssignBondOrderProcessor: The processors state is invalid." << endl;
			return false;
		}

		// determine for each bond its average bond lengths	
		// and store them in bond_lengths_penalties_
		if (ac_)
		{	
			// get the averaged bond lengths
			BuildBondsProcessor bbp; 
			HashMap<Size, HashMap<Size, HashMap<Bond::BondOrder, float> > > bond_lengths = bbp.getBondMap();
			// 
			AtomIterator a_it = ac_->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(*ac_, a_it, b_it)
			{
				// the precomputed bond length penalties
				vector<float> penalties(max_bond_order_ + 1, 0.);
				// the bond atoms 
				Size atom_num1 = b_it->getFirstAtom()->getElement().getAtomicNumber();
				Size atom_num2 = b_it->getSecondAtom()->getElement().getAtomicNumber();
				
				// the current max bond length deviation
				float max_bond_length_deviation = 0.;

				bool found_bond_lengths = false;
				if (   bond_lengths.find(atom_num1) != bond_lengths.end() 
						&& (bond_lengths[atom_num1].find(atom_num2) != bond_lengths[atom_num1].end()))
				{
					found_bond_lengths = true;
				}
				else if ( bond_lengths.find(atom_num2) != bond_lengths.end() 
						&& (bond_lengths[atom_num2].find(atom_num1) != bond_lengths[atom_num2].end()))
				{
					atom_num1 = b_it->getSecondAtom()->getElement().getAtomicNumber();
					atom_num2 = b_it->getFirstAtom()->getElement().getAtomicNumber();
					found_bond_lengths = true;
				}

				if (found_bond_lengths) 
				{
					bool complete = true;
					max_bond_length_deviation = 0.;
					// the averaged bond length of atom1 and atom2
					HashMap<Bond::BondOrder, float> tmp_bond_lengths =  bond_lengths[atom_num1][atom_num2];
					
					// the current bond length
					float bond_length = b_it->getLength();	

					// for all possible bond orders precompute the penalties
					// here we try square deviation
					// NOTE: we ommit the aromatic bonds!
					for (Size i = 1; i <= (unsigned int)max_bond_order_; i++)
					{
						if (tmp_bond_lengths.find((Bond::BondOrder)i) != tmp_bond_lengths.end())
						{
							penalties[i] = pow((bond_length - tmp_bond_lengths[(Bond::BondOrder)i]),(int)2);
							if (max_bond_length_deviation < penalties[i])
							{
								max_bond_length_deviation = penalties[i];
							}
						}
						else
						{
							complete = false;
						}
					}

					// in case we got no complete set, we assume 
					// the missing bond orders are really unlikely
					// and we set a penalty of 2*max_deviation_found (for this bond)
					if (!complete)
					{	
						max_bond_length_deviation *= 2.;

						for (Size i = 1; i <= (unsigned int)max_bond_order_; i++)
						{
							if (tmp_bond_lengths.find((Bond::BondOrder)i) == tmp_bond_lengths.end())
							{
								penalties[i] = max_bond_length_deviation; //pow((bond_length - max_bond_length),(int)2);
							}
					  }
					}
				}
				else
				{
Log.info() << "AssignBondOrderProcessor::precomputeBondLengthPenalties_: no bond length information for " 
					 << b_it->getFirstAtom()->getElement() << " " << b_it->getSecondAtom()->getElement() << endl;
				}

				//store the b_it's penalties in the hashmap
				//else: since we have no information, we handle 
				//every bond order the same, namely assign 0. :-)	
				bond_lengths_penalties_[&(*b_it)] = penalties;

				// add the bonds max deviation to the inverse bond length normalization factor
				bond_length_normalization_factor_ += max_bond_length_deviation;

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "     " << b_it->getFirstAtom()->getName() << " -- "  
		 << b_it->getSecondAtom()->getName()<< " : ";
for (Size i = 1; i <= (unsigned int)max_bond_order_; i++)
{
	cout << penalties[i] << " " ;
}
cout << endl;
#endif

			} // end of for all bonds

			// since the estimation-method  
			

// cout << "bond_length_normalization_factor_:" << 	bond_length_normalization_factor_<< endl;

		}
		else
		{
			valid_ = false;
			return false;
		}

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "AssignBondOrderProcessor::precomputeBondLengthPenalties_:   " << endl << endl;
#endif

		return true;
	}

	bool AssignBondOrderProcessor::preassignPenaltyClasses_()	
	{
#if defined DEBUG_READ 
cout << " - - preassign penalty classes  - - - - - - - - " << endl;
#endif

		if (!valid_)
		{	
			Log.error() << "AssignBondOrderProcessor: The processors state is invalid." << endl;
			return false;
		}

		// Determine for each atom the corresponding valence- and penalty-block	
		if (ac_)
		{
			// Since the Expression-matcher changes the bond orders we need to copy it
			// TODO: this should be fixed in expression matching!
			// NOTE: this works as expected since currently, the processor is always applied to molecules!
			Molecule a_tmp = *((Molecule*)ac_);

			// get the number of atoms
			Position no_atoms = a_tmp.countAtoms();
			
			// resize the data structure
			atom_to_block_.resize(no_atoms,-1);

			// initialize the inverse atom type penalty normalization factor 
			atom_type_normalization_factor_ = 0.;
			
			// for each atom
			for (Position i = 0; i < no_atoms; ++i)
			{
				Atom* at = a_tmp.getAtom(i);
				bool found = false; 
#ifdef DEBUG_READ
cout << at->getFullName() << endl;
#endif
			
				// find the first matching atom definition, whos index gives the block
				for (Size j = 0; !found && (j < block_definition_.size()); j++)
				{	
					Expression exp(block_definition_[j].second);

					if (   (at->getElement().getSymbol() ==  block_definition_[j].first) 
							&& (exp(*at)))
					{

#ifdef DEBUG_READ
cout << "Treffer : " << at->getFullName() << " with index " << at->getIndex() << " " << block_definition_[j].first << ": "
		<<  block_definition_[j].second << " block: " << j+1 << endl;
#endif
						// store the blocks index
						// NOTE: we start counting at 0!
						atom_to_block_[i]=j;
						found = true;

						// add maximal penalty to the inverse atom type normalization factor
						float max_penalty = 0;
						for(Size k = 0; k < block_to_length_[j]; k++)
						{
							float current_pen = penalties_[k + block_to_start_idx_[j]];
							if (max_penalty < current_pen)
							{
								max_penalty = current_pen;
							}
						}
						atom_type_normalization_factor_+= max_penalty;

						//break; //continue; 
					}
				}

				if (!found)
				{
					Log.error() << "AssignBondOrderProcessor: No penalty type found for atom " 
											<< at->getFullName() << endl;
					return false;
				}
			}
		}
		else
		{
			Log.error() << "AssignBondOrderProcessor: There was no valid AtomContainer set." << endl;
		}
		
		return true;
	}

	void AssignBondOrderProcessor::setDefaultOptions()
	{		
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS); 
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS); 
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS); 

	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_QUADRUPLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_QUADRUPLE_BOND_ORDERS); 

	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_AROMATIC_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_AROMATIC_BOND_ORDERS); 

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_CHARGES,
												   AssignBondOrderProcessor::Default::OVERWRITE_CHARGES);	
		
		options.setDefaultBool(AssignBondOrderProcessor::Option::ASSIGN_CHARGES,
													 AssignBondOrderProcessor::Default::ASSIGN_CHARGES);	
	
		options.setDefaultBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS,
													 AssignBondOrderProcessor::Default::KEKULIZE_RINGS);	
		
		options.setDefaultBool(AssignBondOrderProcessor::Option::ENFORCE_OCTETT_RULE,
													 AssignBondOrderProcessor::Default::ENFORCE_OCTETT_RULE);
		
		options.setDefault(AssignBondOrderProcessor::Option::INIFile,
													 AssignBondOrderProcessor::Default::INIFile);		
		
		options.setDefault(AssignBondOrderProcessor::Option::MAX_BOND_ORDER,
													 AssignBondOrderProcessor::Default::MAX_BOND_ORDER);

		options.setDefault(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS,
													 AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS);

		options.setDefault(AssignBondOrderProcessor::Option::ALGORITHM,
													 AssignBondOrderProcessor::Default::ALGORITHM);		

		options.setDefaultReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,
													 AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING);		
	}

	Size  AssignBondOrderProcessor::getNumberOfBondOrdersSet()
	{
		if (solutions_.size() > 0)
		{
			return num_of_free_bonds_;
		}
		return 0;
	}

	bool AssignBondOrderProcessor::apply(Position i)
	{
		if (i < solutions_.size())
		{
			if (solutions_[i].valid)
			{
				// we assume, that the AtomContainer is valid and the correct one! //TODO: is the ok?
				AtomIterator a_it = ac_->beginAtom();
				Atom::BondIterator b_it = a_it->beginBond();
				BALL_FOREACH_BOND(*ac_, a_it, b_it)
				{
					HashMap<Bond*, int>::Iterator it = solutions_[i].bond_orders.find(&*b_it);
					if (it != solutions_[i].bond_orders.end())
					{
						b_it->setOrder(it->second);
					}
				}
				last_applied_solution_ = i;

				if (!options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS))
				{
					// find all rings
					vector<vector<Atom*> > rings;
					RingPerceptionProcessor rpp;
					rpp.calculateSSSR(rings, *ac_);

					// set the aromatic rings	
					AromaticityProcessor ap;
					ap.aromatize(rings, *ac_);
				}
			}
			return solutions_[i].valid;
		}
		else return false;
	}
	
	bool AssignBondOrderProcessor::computeNextSolution()
	{
		if (options.get(Option::ALGORITHM) == Algorithm::A_STAR)
		{
			if (performAStarStep_())
			{
				apply(solutions_.size()-1);
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------------
	
	AssignBondOrderProcessor::Solution_::Solution_()
		: valid(false),
			bond_orders(),
			atom_type_penalty(0.),
			bond_length_penalty(0.)
	{
	}

	AssignBondOrderProcessor::Solution_::Solution_(AssignBondOrderProcessor* ap, const PQ_Entry_& entry)
		: valid(true),
			bond_orders(),
		  atom_type_penalty(entry.estimated_atom_type_penalty),
			bond_length_penalty(entry.estimated_bond_length_penalty)
	{
		if (entry.bond_orders.size() != ap->index_to_bond_.size())
		{
			Log.error() << "Error in AssignBondOrderProcessor::Solution_ Constructor"  << endl;
			return;
		}

		// convert the entry's bond order vector into a HashMap
		for (Size i=0; i< entry.bond_orders.size(); i++)
		{	
			bond_orders[ap->index_to_bond_[i]] =  entry.bond_orders[i];
		} 
	}

	AssignBondOrderProcessor::Solution_::Solution_(AssignBondOrderProcessor* ap, AtomContainer& ac, Bond* bond, int order, bool lessEqualConstraint)
	{		
		// Check number of variables with prefix 'x'
		Position total_no_bonds = ac.countBonds();

		// Number of atoms in the system
		Position no_atoms = ac.countAtoms();

		// Mapping from (free) bonds onto bond variable indices
		map<Bond*, Position> bond_map;

		// Vector for mapping from variable indices onto bonds
		std::vector<Bond*> ind_bond(total_no_bonds, (Bond*)0);

		// Count the choice constraints
		Position no_y = 0;

		// Count the number of bonds whose orders should be calculated
		Position no_x = 0;

		// Maximum number of row entries (in penalty table)
		Position max_no_ent = 0;

		// Maximum number of bonds of an atom
		Position max_no_atom_bonds = 0;
	
		// compute the maps, variables ...
		for (Position i = 0; i < no_atoms; ++i)
		{
			Atom* at1 = ac.getAtom(i);
			//Position fixed = 0;
			Position consider_bonds = 0;

			bool consider_atom = false;
			for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{		
				Bond* bnd = &(*bit);
				map<Bond*, bool>::iterator it = ap->bond_free_.find(bnd);
				if (it != ap->bond_free_.end())
				{
					// If this atom has a free bond we should calculate the bond order
					if (it->second && (bond!=bnd))
					{
						++consider_bonds;	
						consider_atom = true;
						pair<map<Bond*, unsigned int>::iterator, bool> p 
							= bond_map.insert(make_pair(bnd, no_x+1));
						if (p.second)
						{
							ind_bond[no_x] = bnd;
							++no_x;
						}
					}
				}
			} // end of for all bonds of this atom

			if (consider_atom)
			{
				no_y += ap->atomic_penalty_scores_[i].size();
				if (ap->atomic_penalty_scores_[i].size() > max_no_ent)
				{
					max_no_ent = ap->atomic_penalty_scores_[i].size();
				}
			}

			if (consider_bonds > max_no_atom_bonds)
			{
				max_no_atom_bonds = consider_bonds;
			}

		} // end of for all atoms

		Position no_vars = no_x + no_y;
		
		// Create a new model with 'no_vars' variables 
		// (columns) and 0 rows
		lprec *lp = make_lp(0, no_vars);
		if (!lp)
		{
			Log.error() << "Creation of ILP failed" << endl;
		}

		// Set the binary and integer constraints
		for(Position i = 1; i <= no_x; ++i)
		{
			set_int(lp, i, TRUE);
		}

		for(Position i = no_x+1; i <= no_vars; ++i)
		{
			set_binary(lp, i, TRUE);
		}

		// Create space large enough for one row and the objective function,
		// use lp_solves internal types.
		std::vector<int> colno(max_no_atom_bonds + 1 + max_no_ent,0);
		std::vector<REAL> row(max_no_atom_bonds + 1 + max_no_ent,0);
		std::vector<int> obj_colno(no_y+1,0);
		std::vector<REAL> obj_row(no_y+1,0);

		// Factors for choice constraints
		std::vector<REAL> choices(max_no_ent, 1);

		// Makes building the model faster if it's done
		// row by row
		set_add_rowmode(lp, TRUE);

		// Set the lower and upper constraints for bonds
		for(Position i = 1; i <= no_x; ++i)
		{
			colno[0] = i;
			if (!add_constraintex(lp, 1, &choices[0], &colno[0], LE, 4))
			{
				Log.error() << "Setting upper bound for bond constraint failed" << endl;
			}
			if (!add_constraintex(lp, 1, &choices[0], &colno[0], GE, 1))
			{
				Log.error() << "Setting lower bound for bond constraint failed" << endl;
			}
		}

		// Set the extra bond constraint
		if (bond && (order-1 > -1) && (order+1 < 5))
		{	
			// get the bonds index 
			map<Bond*, unsigned int>::iterator it = bond_map.find(bond);
			if (it != bond_map.end())
			{
				Position i = it->second;
				colno[0] = i;
				if (lessEqualConstraint)
				{
					if (!add_constraintex(lp, 1, &choices[0], &colno[0], LE, order-1))
					{
						Log.error() << "Setting extra upper bound for bond constraint failed" << endl;
					}
				}
				else
				{
					if (!add_constraintex(lp, 1, &choices[0], &colno[0], GE, order+1))
					{
						Log.error() << "Setting extra lower bound for bond constraint failed" << endl;
					}
				}
			}
	 	}

		// Create all remaining constraints and the objective function

		// Atom indices
		Position i = 0;

		// Indices of the choice variables (y)
		Position ind_y = 0;

		for(; i < no_atoms; ++i)
		{
			// Find corresponding variables
			Atom* at1 = ac.getAtom(i);

			// Count the bonds of this atom whose
			// orders should be calculated
			Position count_b = 0;
			for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{
				Bond* bnd = &(*bit);
				map<Bond*, unsigned int>::iterator it = bond_map.find(bnd);

				if (it != bond_map.end())
				{
					colno[count_b] = it->second;
					row[count_b]   = -1;
					++count_b;
				}
			}

			if (count_b)
			{
				// This atom has bonds whose orders should be calculated

				// Create indices for the choice variables and set
				// the entries for the valence constraint and the 
				// objective function

				// Count the number of variables participating in the current row
				Position count_vars = count_b;
				for(Position j = 0; j < ap->atomic_penalty_scores_[i].size(); ++j, ++ind_y, ++count_vars)
				{
					// Choice variables have indices > no_x in variable vector for ILP
					obj_colno[ind_y]   = ind_y + no_x + 1;
					colno[count_vars] = obj_colno[ind_y];

					// Set valences
					row[count_vars]   = ap->atomic_penalty_scores_[i][j].first;

					// Set penalties
					obj_row[ind_y]     = ap->atomic_penalty_scores_[i][j].second;
				}

				// in case we got an empty penalty row
				if (ap->atomic_penalty_scores_[i].size() != 0)
				{
					// Add valence constraint
					if (!add_constraintex(lp, count_vars, &row[0], &colno[0], EQ, 
								ap->fixed_val_[i]))
					{
						Log.error() << "Setting valence constraint for ILP failed" << endl;
					}

					// Add choice constraint
					if (!add_constraintex(lp, ap->atomic_penalty_scores_[i].size(), &choices[0], &colno[count_b], 
								EQ, 1))
					{
						Log.error() << "Setting choice constraint for ILP failed" << endl;
					}

					if (ap->options.getBool(Option::ENFORCE_OCTETT_RULE))
					{
						// Add constraint for the octett-rule
						if ((at1->getElement()!= PTE[Element::HELIUM]) && at1->getElement().getGroup() > (short)3)
						{
							if (!add_constraintex(lp, count_b,  &row[0], &colno[0], 
										GE, -4))
							{
								Log.error() << "Setting octett constraint for ILP failed" << endl;
							} 
						}
					}
				}
			}
		}

		// Rowmode should be turned off again after building the model
		set_add_rowmode(lp, FALSE);

		// Set objective function
		if (!set_obj_fnex(lp, no_y, &obj_row[0], &obj_colno[0]))
		{
			Log.error() << "Setting objective function for ILP failed" << endl;
		}

		// Tell lp_solve that this problem is a minimization problem
		set_minim(lp);

		// We only want to see important messages on screen while solving
		set_verbose(lp, IMPORTANT);

		// Show the generated MIP
		//write_LP(lp, stdout);

		// Let lp_solve solve our problem
		int ret = solve(lp);

		// Check whether lp_solve could do its job successfully
		if (ret == OPTIMAL)
		{
			// Get the value of the objective function
			atom_type_penalty = (int)get_objective(lp);
//cout << " Penalty: " << estimated_atom_type_penalty << endl;
			// Get variables
			REAL *vars;
			get_ptr_variables(lp, &vars);

			// Do the assignment of the bond orders
			for(Position i = 0; i < no_x; ++i)
			{
				if (fabs(vars[i] - 1.) < 1.e-10)
				{
					// TODO: should we really set the orders here? No
					ind_bond[i]->setOrder(Bond::ORDER__SINGLE);
					//orders_[ind_bond[i]] = Bond::ORDER__SINGLE;
					continue;
				}
				if (fabs(vars[i] - 2.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__DOUBLE);	
					//orders_[ind_bond[i]] = Bond::ORDER__DOUBLE;
					continue;
				}
				if (fabs(vars[i] - 3.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__TRIPLE);	
					//orders_[ind_bond[i]] = Bond::ORDER__TRIPLE;
					continue;
				}
				if (fabs(vars[i] - 4.) < 1.e-10)
				{
					ind_bond[i]->setOrder(Bond::ORDER__QUADRUPLE);
					//orders_[ind_bond[i]] = Bond::ORDER__QUADRUPLE;
					continue;
				}
			}
			
			// we got a valid solution
			valid = true;

			// store the bond orders of _ALL_ bonds to offer a complete solution 
			AtomIterator a_it = ac.beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(ac, a_it, b_it)
			{
				bond_orders[&(*b_it)] = (b_it->getOrder());
			}
		}
		else
		{
			//Log.error() << "ILP could not be solved successfully" << endl;
			valid = false;
		}

		// free
		delete_lp(lp);
	}

	AssignBondOrderProcessor::Solution_::~Solution_()
	{
	}
	
	void AssignBondOrderProcessor::Solution_::clear()
	{
		valid = false;
		bond_orders.clear();	
		atom_type_penalty = 0.;
		bond_length_penalty = 0.;
	}
	
	//////////////////////////// the PQ_Entry_ - class
	
	// Default constructor
	AssignBondOrderProcessor::PQ_Entry_::PQ_Entry_(float alpha, float atom_type_normalization_factor, float bond_length_normalization_factor)
		: estimated_atom_type_penalty(0.), 
			estimated_bond_length_penalty(0.),
			bond_orders(),
			last_bond(),
			alpha_(alpha),
			atom_type_normalization_factor_(atom_type_normalization_factor), 
			bond_length_normalization_factor_(bond_length_normalization_factor)
	{
	}

	// Copy constructor
	AssignBondOrderProcessor::PQ_Entry_::PQ_Entry_(const AssignBondOrderProcessor::PQ_Entry_& entry)
		: estimated_atom_type_penalty(entry.estimated_atom_type_penalty),
			estimated_bond_length_penalty(entry.estimated_bond_length_penalty),
			bond_orders(entry.bond_orders),
			last_bond(entry.last_bond),
			alpha_(entry.alpha_),
			atom_type_normalization_factor_(entry.atom_type_normalization_factor_), 
			bond_length_normalization_factor_(entry.bond_length_normalization_factor_)
	{	
	}
	
	// Destructor
	AssignBondOrderProcessor::PQ_Entry_::~PQ_Entry_()
	{
	}
	
	// clear-method
	void AssignBondOrderProcessor::PQ_Entry_::clear()
	{ 
		estimated_atom_type_penalty = 0.;
		estimated_bond_length_penalty = 0.;
		last_bond = 0;
		bond_orders.clear();
	}

	// the less operator
	// note: we want a reverse sort, hence we actually return a "greater"
	bool AssignBondOrderProcessor::PQ_Entry_::operator < (const PQ_Entry_& b) const 
	{
		bool value = false;
		if (coarsePenalty() > b.coarsePenalty())
		{
			value = true;
		}
		else
		{ 
			if (coarsePenalty() == b.coarsePenalty() && finePenalty() > b.finePenalty())
			{
				value = true;
			}
		}

#ifdef DEBUG
cout << "AssignBondOrderProcessor::PQ_Entry_::operator <: " <<  coarsePenalty() << " > " << b.coarsePenalty() << "   " <<  finePenalty() << " > " << b.finePenalty() <<
	" macht " << value <<  endl;
#endif

		return value; 
	}
	
	// For testing 
	float AssignBondOrderProcessor::evaluatePenalty(AtomContainer* ac)
	{

		if (valid_ && readAtomPenalties_())
		{
			// Assign penalty classes
			ac_ = ac;
			if (preassignPenaltyClasses_() && precomputeBondLengthPenalties_())
			{
				PQ_Entry_ entry(alpha_, atom_type_normalization_factor_, bond_length_normalization_factor_);
				
				AtomIterator a_it = ac_->beginAtom();
				Atom::BondIterator b_it = a_it->beginBond();
				bond_to_index_.clear();
				index_to_bond_.clear();
				index_to_bond_.resize(ac->countBonds(),0);
				entry.bond_orders.resize(ac->countBonds(),0);
				int i=0;
				BALL_FOREACH_BOND(*ac, a_it, b_it)
				{
					if (bond_to_index_.find(&(*b_it)) == bond_to_index_.end())
					{
						bond_to_index_[&(*b_it)] = i;	
						index_to_bond_[i] = &(*b_it);
						entry.bond_orders[i] = b_it->getOrder();	
						i++;
					}
				}
				entry.last_bond = ac->countBonds()-1;
				if (estimatePenalty_(entry))
					return entry.coarsePenalty();//entry.estimated_atom_type_penalty;
				else
					return -1.;
			}
		}
		return -1.;
	}


} // namespace BALL
