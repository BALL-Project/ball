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
#include <BALL/KERNEL/selector.h>

// Qt
#include <BALL/VIEW/KERNEL/common.h>
#include <QtXml/QtXml>
#include <Qt/qdom.h>

#ifdef BALL_HAS_LPSOLVE
# include <lpsolve/lp_lib.h>
#endif

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
	const bool  AssignBondOrderProcessor::Default::ENFORCE_OCTETT_RULE = false;

	const char* AssignBondOrderProcessor::Option::INIFile = "iniFile";
	const String  AssignBondOrderProcessor::Default::INIFile = "/bond_lengths/BondOrder.xml";

	const char* AssignBondOrderProcessor::Option::MAX_BOND_ORDER = "max_bond_order";
	const int  AssignBondOrderProcessor::Default::MAX_BOND_ORDER = 3;

	const char* AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS = "max_number_of_solutions";
	const int  AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS = 10;
	
	const char* AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS = "compute_also_non_optimal_solutions";
	const bool  AssignBondOrderProcessor::Default::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS = false;

	const char* AssignBondOrderProcessor::Option::ALGORITHM = "algorithm";
	const String AssignBondOrderProcessor::Default::ALGORITHM = AssignBondOrderProcessor::Algorithm::A_STAR;
	//const String AssignBondOrderProcessor::Default::ALGORITHM = AssignBondOrderProcessor::Algorithm::ILP;
		
	const char* AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING = "bond_length_weighting";
	const float AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING = 0.;

	AssignBondOrderProcessor::AssignBondOrderProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			valid_(true),
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
			max_number_of_solutions_(),
			compute_also_non_optimal_solutions_(),
			queue_()
	{
#ifdef BALL_HAS_LPSOLVE
		ilp_ = 0;
#endif
		setDefaultOptions();
	}

	AssignBondOrderProcessor::AssignBondOrderProcessor(const AssignBondOrderProcessor& abop)
		:	UnaryProcessor<AtomContainer>(abop),
			options(abop.options),
			valid_(abop.valid_),
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
			max_number_of_solutions_(abop.max_number_of_solutions_),
			compute_also_non_optimal_solutions_(abop.compute_also_non_optimal_solutions_),
			queue_(abop.queue_)
	{
#ifdef BALL_HAS_LPSOLVE
		if (abop.ilp_)
			ilp_ = copy_lp(abop.ilp_);
		else
			ilp_ = 0;
#endif
	}

	AssignBondOrderProcessor::~AssignBondOrderProcessor()
	{
		setDefaultOptions();
#ifdef BALL_HAS_LPSOLVE
		if (ilp_)
			delete_lp(ilp_);
#endif
	}

	AssignBondOrderProcessor& AssignBondOrderProcessor::operator = (const AssignBondOrderProcessor& abop)
	{
		// prevent self assignment
		if (&abop == this)
			return *this;

		options = abop.options;
		valid_ = abop.valid_;
		bond_to_index_ = abop.bond_to_index_;
		total_num_of_bonds_ = abop.total_num_of_bonds_;
		fixed_val_ = abop.fixed_val_;
		solutions_ = abop.solutions_;
		optimal_penalty_ = abop.optimal_penalty_;
		last_applied_solution_ = abop.last_applied_solution_;
		ac_ = abop.ac_; 
		max_bond_order_ = abop.max_bond_order_;
		alpha_ = abop.alpha_;
		atom_type_normalization_factor_ = abop.atom_type_normalization_factor_;
		bond_length_normalization_factor_ = abop.bond_length_normalization_factor_;	
		max_number_of_solutions_ = abop.max_number_of_solutions_;
		compute_also_non_optimal_solutions_ = abop.compute_also_non_optimal_solutions_;
		queue_ = abop.queue_;
#ifdef BALL_HAS_LPSOLVE
		if (abop.ilp_)
			ilp_ = copy_lp(abop.ilp_);
		else
			ilp_ = 0;
#endif

		return *this;
	}

	bool AssignBondOrderProcessor::start()
	{
		solutions_.clear();
		fixed_val_.clear();
		ac_ = 0;

		valid_ = readAtomPenalties_();
		
		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);
		max_number_of_solutions_ = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
		compute_also_non_optimal_solutions_ = options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);

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

//cout << " \t Octet Rule: " <<options.getBool(Option::ENFORCE_OCTETT_RULE) << endl;
cout << " \t Overwrite charges: " << options.getBool(Option::OVERWRITE_CHARGES) << endl;
cout << " \t Assign charges: " << options.getBool(Option::ASSIGN_CHARGES) << endl;
cout << " \t Kekulizer: " << options.getBool(Option::KEKULIZE_RINGS)  << endl;
cout << " \t Penalty files " << options[Option::Option::INIFile] << endl;
cout << " \t alpha: " << options[Option::Option::BOND_LENGTH_WEIGHTING] << endl;
cout << " \t max bond order: " << options[Option::MAX_BOND_ORDER] << endl;
cout << " \t max number of solutions " << options[Option::MAX_NUMBER_OF_SOLUTIONS] << endl;
cout << " \t compute also non-optimal solutions: " << options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS) << endl;
cout << " \t valid : " << valid_ << endl;
cout << endl;
#endif

		// do we have bonds in the molecule at all?
		if (ac.countBonds() == 0)
			return Processor::CONTINUE;

		// Is the processor in a valid state?
		if (valid_)
		{
			// Speed up the code by temporarily storing the options locally 
			bool overwrite_single_bonds = options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS);
			bool overwrite_double_bonds = options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS);
			bool overwrite_triple_bonds = options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS);
			bool overwrite_quadruple_bonds = options.getBool(Option::OVERWRITE_QUADRUPLE_BOND_ORDERS);
			bool overwrite_aromatic_bonds = options.getBool(Option::OVERWRITE_AROMATIC_BOND_ORDERS);
//			bool enforce_octett_rule = options.getBool(Option::ENFORCE_OCTETT_RULE);

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
										bond_fixed_[bnd] 	= 0;	 
									}
									else
									{
										fixed += 1;
										bond_fixed_[bnd] 	= 1;
										num_fixed_bonds++;
									}
									break;
							}
							case Bond::ORDER__DOUBLE:
							{
								if (overwrite_double_bonds)
								{
									bond_fixed_[bnd] = 0;
								}
								else
								{
									fixed += 2;
									bond_fixed_[bnd] 	= 2;
									num_fixed_bonds++;
								}
								break;
							}
							case Bond::ORDER__TRIPLE:
							{
								if (overwrite_triple_bonds)
								{
									bond_fixed_[bnd] = 0;
								}
								else
								{	
									fixed += 3;
									bond_fixed_[bnd] = 3;
									num_fixed_bonds++;
								}
								break;
							}
							case Bond::ORDER__QUADRUPLE:
							{
								if (overwrite_quadruple_bonds)
								{
										bond_fixed_[bnd] = 0;
								}
								else
								{	
									fixed += 4;
									bond_fixed_[bnd] = 4;
									num_fixed_bonds++;
								}
								break;
							}
							case Bond::ORDER__AROMATIC:
							{
								if (overwrite_aromatic_bonds)
								{
									bond_fixed_[bnd] 	= 0; 
								}
								else
								{	
									fixed += 1;
									bond_fixed_[bnd] 	= 1; 
									num_fixed_bonds++;
								}
								break;
							}	
							default: //Bond::ORDER__UNKNOWN:
							{
									bond_fixed_[bnd] = 0;
							}
						}
					}

					fixed_val_[i] = fixed;
				}
				
				// NOTE: we count each bond twice (one for each atom), hence we divide num_fixed_bonds by 2
				num_of_free_bonds_ = total_num_of_bonds_ - num_fixed_bonds/2;
#ifdef DEBUG
cout << "preassignPenaltyClasses_:" << preassignPenaltyClasses_() << " precomputeBondLengthPenalties_:" << precomputeBondLengthPenalties_() << endl;
#endif

				// Generate penalty values for all atoms in the AtomContainer ac
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
cout << "\nNach initialisierung : queue size = " << queue_.size() << endl;
#endif

						// Try to find a first solution
						bool found_a_sol = performAStarStep_();

						// Do we have a solution? 
						if (!found_a_sol)
						{
							Log.info() << "AssignBondOrderProcessor: No solution found!" << endl;
						}
						else
						{	
							// Do we have to find more solutions?
							Size max_n = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
							bool compute_also_non_optimal_solutions = options.getReal(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);
							if (max_n > 1)
							{
								bool found_another = computeNextSolution();
								bool next_solution_is_optimal = (getTotalPenalty(0) == getTotalPenalty(1)); 

								while (     found_another 
											  && ((getNumberOfComputedSolutions() < max_n) || (!max_n)      )
												&& ( next_solution_is_optimal || compute_also_non_optimal_solutions )
										)
								{	
									found_another = computeNextSolution();
									next_solution_is_optimal = (getTotalPenalty(0) == getTotalPenalty(getNumberOfComputedSolutions()-1)); 
								}
							}
						}
					}	
					else // Solve a ILP
					{
#ifdef BALL_HAS_LPSOLVE
						// Get a first solution
						Solution_ sol;
						bool found_a_sol = createILP_();
						found_a_sol &= solveILP_(sol);
						solutions_.push_back(sol);

						if (!solutions_[0].valid)
						{
							last_applied_solution_=-1;
						}

						if ( (solutions_.size() > 0) 
								//&&  (total_num_of_bonds_ - num_fixed_bonds < MAX__SOLUTIONS) 
								&&  (solutions_[0].valid)
							 )
						{
							// find some more :-)
						}
						else
						{	
							cout << "No solution computed! -solsize:" <<  solutions_.size() << " - no free bonds:" <<  total_num_of_bonds_ - num_fixed_bonds << " - valid " << (solutions_.size()>0 ? solutions_[0].valid : false) << std::endl;
							//cout << "Too many bonds " << total_no_bonds - num_fixed_bonds << std::endl;
						}
#else
						Log.error() << "AssignBondOrderProcessor: BALL was configured without lpsolve support! Try A_STAR instead!" << std::endl;
#endif
					} // end of ILP

					if (solutions_.size() > 0)
					{				
						apply(0);	

						// set informations required for atom type assignment.
						// TODO: move somewhere else!

						// select all carboxyl anions and nitro groups for 
						// delocalized bond types in GAFF
						//TODO clear Selection for system!!!

						// find conjugated atoms
						ac.deselect();
						Selector select("SMARTS([#16D1,#8D1]) AND SMARTS([#16D1,#8D1]~[*D3]~[#16D1,#8D1])");
//						Selector select("SMARTS([#16D1,#8D1]) AND SMARTS([#16D1,#8D1]~*~[#16D1,#8D1]) AND (SMARTS(a) OR SMARTS(*=,#*-,=*=,#*) OR SMARTS([N,P,O,S]=,#*-[*;!H0]) OR SMARTS(*=,#*-[F,Cl,Br,I]) OR SMARTS(*=,#*-[N,P,O,S;!H0]))");
						ac.apply(select);

						// we know that the selected atoms only have one bond each. so we only need to make sure it really is a double bond
						List<Atom*> selected_atoms = select.getSelectedAtoms();
						List<Atom*>::iterator it = selected_atoms.begin();					
						for(;it != selected_atoms.end(); ++it)
						{
							Atom::BondIterator bond_it = (*it)->beginBond();
							for(;+bond_it;++bond_it)
							{
								if (bond_it->getOrder() == Bond::ORDER__DOUBLE)
									bond_it->setProperty("GAFFBondType", DL);
							}
						}
						ac.deselect();

						AtomIterator a_it = ac.beginAtom();
						Atom::BondIterator b_it;
						BALL_FOREACH_BOND(ac, a_it, b_it)
						{
							//TODO definition of  AB aromatic bond??
							// b_it is no delocalized bond 
							if(!b_it->hasProperty("GAFFBondType") || (b_it->getProperty("GAFFBondType").getInt() != DL))
							{
								switch(b_it->getOrder())
								{
									case 1:
										if (b_it->getProperty("IsAromatic").getBool())
										{
											b_it->setProperty("GAFFBondType", sb);
										}
										else
										{
											b_it->setProperty("GAFFBondType",  SB);
										}
										break;
									case 2:
										if (b_it->getProperty("IsAromatic").getBool())
										{
											b_it->setProperty("GAFFBondType",  db);
										}
										else
										{
											b_it->setProperty("GAFFBondType",  DB);
										}
										break;
									case 3:
										b_it->setProperty("GAFFBondType", TB);
										break;
								}
							}
						}
					}
				} // end of if preassign worked out
			} // end of if molecule
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
			if (current_atom_index >= (int)atom_to_block_.size())
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

						for (Size j = 1; j <= (Size)up_to; j++)
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
		
		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER,
													 AssignBondOrderProcessor::Default::MAX_BOND_ORDER);

		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS,
													    AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS);

		options.setDefault(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS,
											 AssignBondOrderProcessor::Default::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);

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

	// TODO: could be given a flag indicating whether the next solution should be applied or not
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
		else // ILP
		{
#ifdef BALL_HAS_LPSOLVE
			// first find out if there is a further solution
			int number_of_solutions = get_solutioncount(ilp_);

			if (solutions_.size() >= number_of_solutions)
				return false;

			// set the solution number of the next solution to return
			// NOTE: count starts at 1
			set_solutionlimit(ilp_, solutions_.size());
			
			Solution_ sol;

			bool found_a_sol = solveILP_(sol);
			if (found_a_sol)
			{
				solutions_.push_back(sol);
				return true;
			}

#else
			Log.error() << "AssignBondOrderProcessor: BALL was configured without lp_solve support! Try using A_STAR instead!" << std::endl;
#endif

			return false;
		}

		return false;
	}

#ifdef BALL_HAS_LPSOLVE
	bool AssignBondOrderProcessor::createILP_()
	{
		// Check number of variables with prefix 'x'
		Position total_no_bonds = ac_->countBonds();

		// Number of atoms in the system
		Position no_atoms = ac_->countAtoms();

		// Mapping from (free) bonds onto bond variable indices
		map<Bond*, Position> bond_map;

		// Vector for mapping from variable indices onto bonds
		ilp_index_to_free_bond_.resize(total_no_bonds, (Bond*)0);

		// Count the choice constraints
		Position no_y = 0;

		// Count the number of bonds whose orders should be calculated
		ilp_number_of_free_bonds_ = 0;

		// Maximum number of row entries (in penalty table)
		// NOTE: we determine this number later in the code, since we do not
		//			 know yet which atoms have to be considered and which are fixed
		Position max_no_ent=0;

		// Maximum number of bonds of an atom
		Position max_no_atom_bonds = 0;
	
		// compute the maps, variables ...
		for (Position current_atom = 0; current_atom < no_atoms; ++current_atom)
		{
			Atom* at1 = ac_->getAtom(current_atom);
			Position consider_bonds = 0;

			for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{		
				if (!bond_fixed_[&*bit]) // this bond is free => calculate bond order
				{
					++consider_bonds;
					if (bond_map.find(&*bit) == bond_map.end())
					{ 
						bond_map[&*bit] = ilp_number_of_free_bonds_;
						ilp_index_to_free_bond_[ilp_number_of_free_bonds_] = &*bit;
						++ilp_number_of_free_bonds_;
					}
				}
			} // end of for all bonds of this atom

			if (consider_bonds)
			{
				Size current_length = block_to_length_[atom_to_block_[current_atom]];
				
				no_y += current_length;
				max_no_ent = std::max(max_no_ent, current_length);
			}

			max_no_atom_bonds = std::max(consider_bonds, max_no_atom_bonds);

		} // end of for all atoms

		// Number of variables of the whole ILP
		Position no_vars = ilp_number_of_free_bonds_ + no_y;
		
		// Create a new model with 'no_vars' variables 
		// (columns) and 0 rows
		ilp_ = make_lp(0, no_vars);
		if (!ilp_)
		{
			Log.error() << "AssignBondOrderProcessor: Creation of ILP failed" << endl;
			return false;
		}

		// Set the binary and integer constraints
		// Variables for bond orders are integer
		for(Position i = 1; i <= ilp_number_of_free_bonds_; ++i)
		{
			set_int(ilp_, i, TRUE);
		}
		// Decision variables for atom valences are binary 
		for(Position i = ilp_number_of_free_bonds_+1; i <= no_vars; ++i)
		{
			set_binary(ilp_, i, TRUE);
		}

		// Create space large enough for one row and the objective function,
		// use lp_solves internal types.

		// Indices of the variables (constraints)
		std::vector<int> cons_indices(max_no_atom_bonds + 1 + max_no_ent,0); // colno
		// Prefactors (constraints)
		std::vector<REAL> cons_prefactors(max_no_atom_bonds + 1 + max_no_ent,0); // row
		// Indices of the variables (objective function)
		std::vector<int> obj_indices(no_y+1,0); // obj_colno
		// Prefactors (objective function)
		std::vector<REAL> obj_prefactors(no_y+1,0); // obj_row

		// Factors for choice constraints
		std::vector<REAL> choices(max_no_ent, 1);

		// Makes building the model faster if it's done
		// row by row
		set_add_rowmode(ilp_, TRUE);

		// Set the lower and upper constraints for bonds
		for(Position i = 1; i <= ilp_number_of_free_bonds_; ++i)
		{
			cons_indices[0] = i;
			if (!add_constraintex(ilp_, 1, &choices[0], &cons_indices[0], LE, max_bond_order_))
			{
				Log.error() << "AssignBondOrderProcessor: Setting upper bound for bond constraint failed" << endl;
				return false;
			}
			if (!add_constraintex(ilp_, 1, &choices[0], &cons_indices[0], GE, 1))
			{
				Log.error() << "AssignBondOrderProcessor: Setting lower bound for bond constraint failed" << endl;
				return false;
			}
		}

		// Create all remaining constraints and the objective function
		// Indices of the choice variables (y)
		Position ind_y = 0;

		for(Position i = 0; i < no_atoms; ++i)
		{
			// Find corresponding variables
			Atom* at1 = ac_->getAtom(i);

			// Count the bonds of this atom whose
			// orders should be calculated
			Position count_b = 0;
			for(Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{
				map<Bond*, unsigned int>::iterator it = bond_map.find(&(*bit));
				if (it != bond_map.end())
				{
					cons_indices[count_b] = it->second + 1;
					cons_prefactors[count_b] = -1;
					++count_b;
				}
			}

			if (count_b)
			{
				// This atom has bonds whose orders should be calculated

				// Create indices for the choice variables and set
				// the entries for the valence constraint and the
				// objective function

				// Count the number of variables participating in the current cons_prefactors
				Position count_vars = count_b;
				for(Position j = 0; j < block_to_length_[atom_to_block_[i]]; ++j, ++ind_y, ++count_vars)
				{
					// Choice variables have indices > ilp_number_of_free_bonds_ in variable vector for ILP
					obj_indices[ind_y]  = ind_y + ilp_number_of_free_bonds_ + 1;
					cons_indices[count_vars] = obj_indices[ind_y];

					// Set valences
					cons_prefactors[count_vars] = block_to_start_valence_[atom_to_block_[i]] + j;

					// Set penalties
					obj_prefactors[ind_y]       = penalties_[block_to_start_idx_[atom_to_block_[i]] + j];
				}

				// in case we got an empty penalty cons_prefactors
				if (block_to_length_[atom_to_block_[i]] != 0)
				{
					// Add valence constraint
					if (!add_constraintex(ilp_, count_vars, &cons_prefactors[0], &cons_indices[0], EQ,
								fixed_val_[i]))
					{
						Log.error() << "AssignBondOrderProcessor: Setting valence constraint for ILP failed" << endl;
						return false;
					}
					// Add choice constraint
					if (!add_constraintex(ilp_, block_to_length_[atom_to_block_[i]], &choices[0], &cons_indices[count_b], EQ, 1))
					{
						Log.error() << "AssignBondOrderProcessor: Setting choice constraint for ILP failed" << endl;
						return false;
					}

					if (options.getBool(Option::ENFORCE_OCTETT_RULE))
					{
						// Add constraint for the octett-rule
						if ((at1->getElement()!= PTE[Element::HELIUM]) && at1->getElement().getGroup() > (short)3)
						{
							if (!add_constraintex(ilp_, count_b,  &cons_prefactors[0], &cons_indices[0], GE, -4))
							{
								Log.error() << "AssignBondOrderProcessor: Setting octett constraint for ILP failed" << endl;
								return false;
							}
						}
					}
				}
			}
		}

		// Rowmode should be turned off again after building the model
		set_add_rowmode(ilp_, FALSE);

		// Set objective function
		if (!set_obj_fnex(ilp_, no_y, &obj_prefactors[0], &obj_indices[0]))
		{
			Log.error() << "AssignBondOrderProcessor: Setting objective function for ILP failed" << endl;
		}

		// Tell lp_solve that this problem is a minimization problem
		set_minim(ilp_);

		// We only want to see important messages on screen while solving
		set_verbose(ilp_, IMPORTANT);

		// Show the generated MIP
		//write_LP(ilp_, stdout);
		return true;
	}

	bool AssignBondOrderProcessor::solveILP_(Solution_& solution)
	{
		// Let lp_solve solve our problem
		int ret = solve(ilp_);

		// Get the value of the objective function
		solution.atom_type_penalty = (int)get_objective(ilp_);

		// Check whether lp_solve could do its job successfully
		if (ret == OPTIMAL)
		{
			// we got a valid solution
			solution.valid = true;

			// store the bond orders of _ALL_ bonds to offer a complete solution 
			AtomIterator a_it = ac_->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(*ac_, a_it, b_it)
			{
				solution.bond_orders[&(*b_it)] = (b_it->getOrder());
			}

			// Get variables
			REAL *vars;
			get_ptr_variables(ilp_, &vars);

			// Do the assignment of the bond orders
			for(Position i = 0; i < ilp_number_of_free_bonds_; ++i)
			{
				if (fabs(vars[i] - 1.) < 1.e-10)
				{
					solution.bond_orders[ilp_index_to_free_bond_[i]] = Bond::ORDER__SINGLE;
					continue;
				}
				if (fabs(vars[i] - 2.) < 1.e-10)
				{
					solution.bond_orders[ilp_index_to_free_bond_[i]] = Bond::ORDER__DOUBLE;
					continue;
				}
				if (fabs(vars[i] - 3.) < 1.e-10)
				{
					solution.bond_orders[ilp_index_to_free_bond_[i]] = Bond::ORDER__TRIPLE;
					continue;
				}
				if (fabs(vars[i] - 4.) < 1.e-10)
				{
					solution.bond_orders[ilp_index_to_free_bond_[i]] = Bond::ORDER__QUADRUPLE;
					continue;
				}
			}
		}
		else
		{
			Log.error() << "AssignBondOrderProcessor: ILP could not be solved successfully" << endl;
			solution.valid = false;
			return false;
		}

		return true;
	}
#endif

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
