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
#include <BALL/STRUCTURE/hybridisationProcessor.h>

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
//#define DEBUG_ESTIMATE 1
#undef DEBUG_ESTIMATE

//#define DEBUG_ADD_HYDROGENS 1
#undef DEBUG_ADD_HYDROGENS

//#define UNNORMALIZED_DEBUG 1
#undef UNNORMALIZED_DEBUG


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
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS = "overwrite_selected_bonds";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SELECTED_BONDS = false;

	const char* AssignBondOrderProcessor::Option::ADD_HYDROGENS = "add_hydrogens_by_hybridisation_processor";
	const bool  AssignBondOrderProcessor::Default::ADD_HYDROGENS = false;

	const char* AssignBondOrderProcessor::Option::OVERWRITE_CHARGES = "overwrite_existing_charges";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::ASSIGN_CHARGES = "assign_charges";
	const bool  AssignBondOrderProcessor::Default::ASSIGN_CHARGES = false;
	
	const char* AssignBondOrderProcessor::Option::KEKULIZE_RINGS = "kekulize_aromatic_rings";
	const bool  AssignBondOrderProcessor::Default::KEKULIZE_RINGS = true;

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
		
	const char* AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING = "bond_length_weighting";
	const float AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING = 0.;

	AssignBondOrderProcessor::AssignBondOrderProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			valid_(true),
			evaluation_mode_(false),
			bond_fixed_(),
			free_bonds_(),
			bond_to_index_(),
			index_to_bond_(),
			number_of_virtual_hydrogens_(),
			virtual_bond_index_to_number_of_virtual_hydrogens_(),
			num_of_virtual_bonds_(0),
			virtual_bond_index_to_atom_(),
			atom_to_virtual_bond_index_(),
			virtual_bond_(NULL), 
			ilp_index_to_free_bond_(),
			ilp_number_of_free_bonds_(),
			total_num_of_bonds_(0),
			num_of_free_bonds_(0),
			fixed_val_(),
			solutions_(),
			atom_type_normalization_factor_(0.),
			bond_length_normalization_factor_(0.),
			last_applied_solution_(-1),
			ac_(0),
			max_bond_order_(),
			alpha_(),
			max_number_of_solutions_(),
			compute_also_non_optimal_solutions_(),
			add_missing_hydrogens_(),
			queue_(),
			penalties_(),
			block_to_start_idx_(),
			block_to_length_(),
			block_to_start_valence_(),
			block_definition_(),
			atom_to_block_(),
			bond_lengths_penalties_()
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
			evaluation_mode_(abop.evaluation_mode_),
			bond_fixed_(abop.bond_fixed_),
			free_bonds_(abop.free_bonds_),
			bond_to_index_(abop.bond_to_index_),
			index_to_bond_(abop.index_to_bond_),
			number_of_virtual_hydrogens_(abop.number_of_virtual_hydrogens_),
			virtual_bond_index_to_number_of_virtual_hydrogens_(abop.virtual_bond_index_to_number_of_virtual_hydrogens_),
			num_of_virtual_bonds_(abop.num_of_virtual_bonds_),
			virtual_bond_index_to_atom_(abop.virtual_bond_index_to_atom_),
			atom_to_virtual_bond_index_(abop.atom_to_virtual_bond_index_),
			virtual_bond_(abop.virtual_bond_), 
			ilp_index_to_free_bond_(abop.ilp_index_to_free_bond_),
			ilp_number_of_free_bonds_(abop.ilp_number_of_free_bonds_),
			total_num_of_bonds_(abop.total_num_of_bonds_),
			num_of_free_bonds_(abop.num_of_free_bonds_),
			fixed_val_(abop.fixed_val_),
			solutions_(abop.solutions_),
			atom_type_normalization_factor_(abop.atom_type_normalization_factor_),
			bond_length_normalization_factor_(abop.bond_length_normalization_factor_),
			last_applied_solution_(abop.last_applied_solution_),
			ac_(abop.ac_),
			max_bond_order_(abop.max_bond_order_),
			alpha_(abop.alpha_),
			max_number_of_solutions_(abop.max_number_of_solutions_),
			compute_also_non_optimal_solutions_(abop.compute_also_non_optimal_solutions_),
			add_missing_hydrogens_(abop.add_missing_hydrogens_),
			queue_(abop.queue_),
			penalties_(abop.penalties_),
			block_to_start_idx_(abop.block_to_start_idx_),
			block_to_length_(abop.block_to_length_),
			block_to_start_valence_(abop.block_to_start_valence_),
			block_definition_(abop.block_definition_),
			atom_to_block_(abop.atom_to_block_),
			bond_lengths_penalties_(abop.bond_lengths_penalties_)
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
		clear();
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
		evaluation_mode_ = abop.evaluation_mode_;
		bond_fixed_ = abop.bond_fixed_;
		free_bonds_ = abop.free_bonds_;
		bond_to_index_ = abop.bond_to_index_;
		index_to_bond_ = abop.index_to_bond_;
		number_of_virtual_hydrogens_ = abop.number_of_virtual_hydrogens_;
		virtual_bond_index_to_number_of_virtual_hydrogens_ = abop.virtual_bond_index_to_number_of_virtual_hydrogens_;
		num_of_virtual_bonds_ = abop.num_of_virtual_bonds_;
		virtual_bond_index_to_atom_ = abop.virtual_bond_index_to_atom_;
		atom_to_virtual_bond_index_ = abop.atom_to_virtual_bond_index_;
		virtual_bond_ = abop.virtual_bond_; 
		ilp_index_to_free_bond_ = abop.ilp_index_to_free_bond_;
		ilp_number_of_free_bonds_ = abop.ilp_number_of_free_bonds_;
		total_num_of_bonds_ = abop.total_num_of_bonds_;
		num_of_free_bonds_ = abop.num_of_free_bonds_;
		fixed_val_ = abop.fixed_val_;
		solutions_ = abop.solutions_;
		atom_type_normalization_factor_ = abop.atom_type_normalization_factor_;
		bond_length_normalization_factor_ = abop.bond_length_normalization_factor_;
		last_applied_solution_ = abop.last_applied_solution_;
		ac_ = abop.ac_;
		max_bond_order_ = abop.max_bond_order_;
		alpha_ = abop.alpha_;
		max_number_of_solutions_ = abop.max_number_of_solutions_;
		compute_also_non_optimal_solutions_ = abop.compute_also_non_optimal_solutions_;
		add_missing_hydrogens_ = abop.add_missing_hydrogens_;
		queue_ = abop.queue_;
		penalties_ = abop.penalties_;
		block_to_start_idx_ = abop.block_to_start_idx_;
		block_to_length_ = abop.block_to_length_;
		block_to_start_valence_ = abop.block_to_start_valence_;
		block_definition_ = abop.block_definition_;
		atom_to_block_ = abop.atom_to_block_;
		bond_lengths_penalties_ = abop.bond_lengths_penalties_;

#ifdef BALL_HAS_LPSOLVE
		if (abop.ilp_)
			ilp_ = copy_lp(abop.ilp_);
		else
			ilp_ = 0;
#endif

		return *this;
	}

	void AssignBondOrderProcessor::clear()
	{
		valid_ = true;
		evaluation_mode_ = false;
		bond_fixed_.clear();
		free_bonds_.clear();
		bond_to_index_.clear();
		index_to_bond_.clear();
		number_of_virtual_hydrogens_.clear();
		virtual_bond_index_to_number_of_virtual_hydrogens_.clear();
		num_of_virtual_bonds_ = 0;
		virtual_bond_index_to_atom_.clear();
		atom_to_virtual_bond_index_.clear();
		virtual_bond_ = NULL; 

		ilp_index_to_free_bond_.clear();
		ilp_number_of_free_bonds_ = 0;
		total_num_of_bonds_ = 0;
		num_of_free_bonds_ = 0;

		fixed_val_.clear();
		solutions_.clear();
		atom_type_normalization_factor_ = 0.;
		bond_length_normalization_factor_ = 0.;

		last_applied_solution_ = -1;
		ac_ = 0;

		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);
		max_number_of_solutions_ = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
		compute_also_non_optimal_solutions_ = options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);
		add_missing_hydrogens_ = options.getBool(Option::ADD_HYDROGENS);
		
		queue_ = priority_queue<PQ_Entry_>();
		penalties_.clear();
		block_to_start_idx_.clear();
		block_to_length_.clear();
		block_to_start_valence_.clear();
		block_definition_.clear();
		atom_to_block_.clear();
		bond_lengths_penalties_.clear();
		#ifdef BALL_HAS_LPSOLVE
			ilp_ = NULL;
		#endif

	}

	bool AssignBondOrderProcessor::start()
	{
		clear();	
		valid_ = readAtomPenalties_();
		evaluation_mode_ = false;
		return true;
	}

	bool AssignBondOrderProcessor::performAStarStep_(Solution_& sol)
	{
		// try to find a solution
		while(!queue_.empty())
		{

#ifdef DEBUG
cout << " Next ASTAR step : queue size : " << queue_.size();
#endif

//TODO: warum terminiert BEWCUB nicht ?
//cout << " Hi Anne:  Next ASTAR step : queue size : " << queue_.size();

			// take the top entry of the queue
			PQ_Entry_ entry = queue_.top();
			queue_.pop();

//cout << "  atom type penalty: " << entry.estimated_atom_type_penalty << 
//				"  bond length penalty: " << entry.estimated_bond_length_penalty << endl;

#ifdef DEBUG
cout << "  atom type penalty: " << entry.estimated_atom_type_penalty << 
				"  bond length penalty: " << entry.estimated_bond_length_penalty << " ( " ;
for (Size i = 0; i< entry.bond_orders.size(); i++)
{
	cout << " " <<   entry.bond_orders[i] ;
}
cout << ")" << endl;
#endif


			// is this a leaf?
			if (entry.last_bond == (total_num_of_bonds_ - 1 + num_of_virtual_bonds_))
			{
				// we found a solution
				// store the solution :-)
				sol.valid = true;
				sol.atom_type_penalty = entry.estimated_atom_type_penalty;
				sol.bond_length_penalty = entry.estimated_bond_length_penalty;

				if (entry.bond_orders.size() != (total_num_of_bonds_ + num_of_virtual_bonds_))
				{
					Log.error() << "Error: " << __FILE__ << " " << __LINE__  << endl;

					return false;
				}

				// convert the entry's bond order vector into a HashMap
				//    first the original bonds
				for (Size i=0; i < total_num_of_bonds_; i++)
				{	
					sol.bond_orders[index_to_bond_[i]] =  entry.bond_orders[i];
				} 
				//   then the virtual bonds
				for (Size i = 0; i < num_of_virtual_bonds_; i++)
				{	
					int num_H = entry.bond_orders[total_num_of_bonds_ + i];
					if (num_H > 0)
					{
						Atom* atom = virtual_bond_index_to_atom_[i];
						sol.number_of_virtual_hydrogens[atom] = num_H;
					}
				}
				
				return true;
			}
			else // no leaf
			{	
				entry.last_bond++;
				Bond* current_bond = index_to_bond_[entry.last_bond];
				
				// take the next bond and 
				//  set to the prefixed value ... 
				if (bond_fixed_[current_bond])
				{	
					// set this fixed bond order
					entry.bond_orders[entry.last_bond] = bond_fixed_[current_bond];

					// estimate the penalty
					if (estimatePenalty_(entry))
					{
						queue_.push(entry);
					}
				}	
				// take the next bond and 
				//  ... or try all bond orders
				else 
				{
					// case 1: VIRTUAL__BOND
					if (entry.last_bond >= total_num_of_bonds_)
					{	
						int virtual_bond_index = entry.last_bond - total_num_of_bonds_;
						int max_virtual_hydrogens = 
						 		virtual_bond_index_to_number_of_virtual_hydrogens_[virtual_bond_index];
								
						for (int i = 0; i <= max_virtual_hydrogens; i++)
						{
							// Set the bond order
							entry.bond_orders[entry.last_bond] = i;

							// Estimate the penalty and add the node into the queue if valid
							if (estimatePenalty_(entry))
							{
								queue_.push(entry);
							}
						}
					}
					else // case 2: original bond
					{	
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
					} // end of case 2
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
		 << options.getBool(Option::OVERWRITE_SELECTED_BONDS) << " " 
		<< endl;

cout << " \t Add hydrogens : " << options.getBool(Option::ADD_HYDROGENS) << endl;
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
			bool overwrite_selected_bonds = options.getBool(Option::OVERWRITE_SELECTED_BONDS);

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

						if (overwrite_selected_bonds)
						{
							if (bnd->isSelected())
							{
								bond_fixed_[bnd] 	= 0;	 
							}
							else
							{
								fixed += 1;
								bond_fixed_[bnd] 	= 1;
								num_fixed_bonds++;
							}
						}
						else
						{
							// According to the options and current bond order 
							// a bond is a free variable of the ILP or not.
							// YES: add a variable in the bond side constraint   
							// NO: equality in the bonds side constraint 
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
									fixed += 4;
									bond_fixed_[bnd] = 4;
									num_fixed_bonds++;
									break;
								}
								case Bond::ORDER__AROMATIC:
								{
									fixed += 1;
									bond_fixed_[bnd] 	= 1; 
									num_fixed_bonds++;
									break;
								}	
								default: //Bond::ORDER__UNKNOWN:
								{
									bond_fixed_[bnd] = 0;
								}
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
					bool found_a_sol = false;
					Solution_ sol;

					if (options.get(Option::ALGORITHM) == Algorithm::A_STAR)
					{
						// Initialize a priority queue and try to find a first solution
						// Further solutions will be computed calling the method computeNextSolution

						// Initialize the priority queue
						PQ_Entry_ entry(alpha_, atom_type_normalization_factor_, bond_length_normalization_factor_);
						entry.bond_orders.resize(total_num_of_bonds_ + num_of_virtual_bonds_,-1);
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

							// case 1: VIRTUAL__BOND 
							// should not happen in the beginning, but who knows?
							/*if ( add_missing_hydrogens_
									 && current_bond->hasProperty("VIRTUAL__BOND")  
									 && current_bond->getProperty("VIRTUAL__BOND").getBool())*/
							if (entry.last_bond >= total_num_of_bonds_)
							{

								int virtual_bond_index = entry.last_bond - total_num_of_bonds_;
								int max_virtual_hydrogens = 
									virtual_bond_index_to_number_of_virtual_hydrogens_[virtual_bond_index];
								
								for (int i = 0; i <= max_virtual_hydrogens; i++)
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
							else // case 2: original bond
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
							} // end of case 2
						} // end of bond is free

#ifdef DEBUG
cout << "\nNach initialisierung : queue size = " << queue_.size() << endl;
#endif

						// Try to find a first solution
						found_a_sol = performAStarStep_(sol);
					}	
					else // Solve a ILP
					{
#ifdef BALL_HAS_LPSOLVE
						// Get a first solution
						found_a_sol = createILP_();
						found_a_sol &= solveILP_(sol);
					#else
						Log.error() << "AssignBondOrderProcessor: BALL was configured without lpsolve support! Try A_STAR instead!" << std::endl;
#endif
					} // end of ILP

					// Do we have a solution? 
					if (!found_a_sol)
					{
						Log.info() << "AssignBondOrderProcessor: No solution found!" << endl;
					}
					else
					{	
						solutions_.push_back(sol);

						// Do we have to find more solutions?
						Size max_n = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
						bool found_another = true;//computeNextSolution();
						bool last_sol_is_optimal = true;

						while (    found_another  
								    && ((getNumberOfComputedSolutions() < max_n) || (!max_n))
									  && (last_sol_is_optimal || (compute_also_non_optimal_solutions_))
									)
						{	
							found_another = computeNextSolution();
							last_sol_is_optimal &= (getTotalPenalty(0)==getTotalPenalty(solutions_.size()-1));
						}

					}
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
cout << " - - PE - - - - - - - - " << endl;
#endif

		AtomIterator a_it = ac_->beginAtom();	
		Atom::BondIterator b_it; 
		int valence = 0; 			// the so far fixed valence of the considered atom
		bool closed = true;  	// are all bonds of this atom fixed? 
		float estimated_atom_penalty = 0.;
		float estimated_bond_penalty = 0.;
		float current_bond_length_penalty = 0; // length penalty of the bonds already set
		bool valid = true;
		int  num_free_bonds = 0; // without VIRTUAL__BOND's!
		

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
			vector<Bond*> free_bonds;	       	// store the free bonds for bond length penalty 
																			 	// calculation (without VIRTUAL__BOND's!
			valence = 0; 	                    // sum of bond orders               of all fixed bonds inclusiv VIRTUAL BOND's ! 
			current_bond_length_penalty = 0;  // sum of all bond length penalties       "
			closed = true;
			num_free_bonds = 0;

			// iterate over all bonds of this atom
			// first its original bonds
			for (b_it = a_it->beginBond(); +b_it; b_it++) 
			{			
				Bond* current_bond = &*b_it;
				// do we have a valid bond? 
				if ( bond_to_index_.find(current_bond) != bond_to_index_.end())
				{
					// is this bond already fixed?
					if (    (bond_to_index_[current_bond] <= (Index)entry.last_bond)
							 && (entry.bond_orders[bond_to_index_[current_bond]] > 0)   )
					{
						int order = entry.bond_orders[bond_to_index_[current_bond]];
						valence += order;

						// store the bond length penalty
						// we just consider non-virtual bonds here!
						if (bond_lengths_penalties_.find(current_bond) != bond_lengths_penalties_.end())
						{
							current_bond_length_penalty += bond_lengths_penalties_[&*b_it][order]; 
						}
						else
						{
							Log.error() << "Error in AssignBondOrderProcessor " << __FILE__ << " " << __LINE__ << endl;
						}

#if defined DEBUG || defined DEBUG_ESTIMATE
						cout << "  * fixed bond num " << bond_to_index_[&*b_it] << " (" << b_it->getFirstAtom()->getFullName() << "-" << b_it->getSecondAtom()->getFullName() <<")"
							<< " by order " << entry.bond_orders[bond_to_index_[&*b_it]]  
							<< " :bond len penalty: " << bond_lengths_penalties_[&*b_it][order] 
							<< " -> cur bond len pen:"<< current_bond_length_penalty << endl;
#endif

					}
					else // a free bond 
					{
						closed = false;
						num_free_bonds++;
						free_bonds.push_back(&*b_it); 	// store the bond length penalty
						
						// add the minimum
						if (bond_lengths_penalties_.find(current_bond) != bond_lengths_penalties_.end())
						{
							current_bond_length_penalty += bond_lengths_penalties_[&*b_it][0]; 
						}
						else
						{
							Log.error() << "Error in AssignBondOrderProcessor " << __FILE__ << " " << __LINE__ << endl;
						}

					}
				} 
				else  // no valid bond
				{
					Log.error() << "Error: invalid bond!" << __FILE__ << " " << __LINE__ << endl;
				}
			}

			// and now get its virtual hydrogens!
			int virtual_order = 0;
			if (     add_missing_hydrogens_
					 && (atom_to_virtual_bond_index_.find(&*a_it) != atom_to_virtual_bond_index_.end()))
			{
				virtual_order = std::max(0,entry.bond_orders[atom_to_virtual_bond_index_[&*a_it] + total_num_of_bonds_]);
				valence += virtual_order;
			}

			// decide, whether there is a free virtual bond left => atom not closed yet!
			if (   add_missing_hydrogens_ 
					&& (entry.last_bond != (total_num_of_bonds_ + num_of_virtual_bonds_ - 1)))
			{
				closed = false;
			}

			// Now all orders of already fixed bonds and fixed virtual 
			// hydrogen bonds are summed up in valence.
			// And all bond length deviation penalties are summed up 
			// in current_bond_length_penalty - excluded the virtual hydrogen bonds  //TODO
			// We have to distinguish two cases:
			// 	(a) atom closed and   (b) atom has still bonds with undefined orders
			//
			// Case (a)  the bond length penatly is already given in current_bond_length_penalty
			// 					 the atom type penalty can simply be computed by evaluating the penalty-vector 
			// 					 	at the already computed valence in the case with virtal_order many virtual H's
			// Case (b)  estimate the bond lenght penalty by adding the current bond lenght deviation of 
			// 						all bonds that are already set and take the minimal possible bond length deviation
			// 						for all un-set bonds. Leave out the virtual hydrogens! This summation is already 
			// 						done!
			// 					  TODO : improve the heuristic 
			// 					  				iterate over all free bonds of this atom and try to distribute the 
			// 					  				free valence (given by the max index of all relevant penalty vectors) 
			// 					  				such that the summed bond length penalty is minimal.
			// 					 estimate the atom penalty by finding the min in all remaining possible penalties
			// 					  in the possible penalty vectors.
			// 
			// For speed up return false if something exploded :-) 

			// Something very strange:
			// Remember, we start atom counting with 0
			if (current_atom_index >= (int)atom_to_block_.size())
			{
				Log.error() << "Error: Atom  " << a_it->getFullName() << " out of index ! " 
										<<  __FILE__ << " "  << __LINE__ << endl;
				return false; 
			}
			
			// should not happen: how many virtual hydrogens are choosen? 
			if (virtual_order > (int)atom_to_block_[current_atom_index].size())
			{
				Log.error() << "Error: too many virtual bonds  " 
										<<  __FILE__ << " " << __LINE__ << endl;
				return false; 
			}

			// are all bonds of this atom already fixed?
			if (closed)
			{
				// get the current penalty vector 
				int block = atom_to_block_[current_atom_index][virtual_order];

				if (block < 0)
				{
					Log.error() << "AssignBondOrderProcessor::estimatePenalty(): Error: could not find current penalty vector for " << current_atom_index << " " << virtual_order << "!" << std::endl;
										
					return false; 
				}

				int current_start_valence = block_to_start_valence_[block];
				int current_block_length  = block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;
				int current_start_index   = block_to_start_idx_[block];
				
//Hi Anne
//if ( a_it->getElement() == PTE[Element::H])
//{
//	cout << a_it->getFullName() << " " << valence << " " << block << " " 
//				<< current_start_valence << " " << current_end_valence << endl;
//}
			
				// is the valence found already greater than the largest for this atom type?
				if (valence > current_end_valence)
				{

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  * valence explosion for atom " <<   a_it->getFullName() << endl;
#endif
					if (evaluation_mode_)
					{
						Log.info() << "  AssignBondOrderProcessor: valence explosion for atom " 
												<<   a_it->getFullName()  << " val: " << valence 
												<< " free bo: "<<  num_free_bonds <<  " : Ruleid " <<  block +1 
												<< " with length:" <<  block_to_length_[block] << endl;
					}

					return false;
				}

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  * atom " <<   a_it->getFullName() << " closed with valence " << valence ;
#endif

				// is the valence large enough for this atom type?
				if (valence >= current_start_valence)
				{
					// NOTE: case valence too large was handled before

					// add the actual penalty
					estimated_atom_penalty   +=  penalties_[current_start_index + valence  - current_start_valence];
					estimated_bond_penalty   +=  current_bond_length_penalty; 

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " and atom type penalty +" << penalties_[current_start_index + valence - current_start_valence] << " = " << estimated_atom_penalty 
	   << " and bond length penalty +" << current_bond_length_penalty << " = " << estimated_bond_penalty << endl;

					if (evaluation_mode_ && ( penalties_[current_start_index + valence - current_start_valence] >0) )
					{
						Log.info() << "   estimatePenalty() : " << a_it->getFullName() 
											 << " gets penalty "  <<  penalties_[current_start_index + valence - current_start_valence] 
											 << " in Rule with id " <<  block +1 << endl;
					}
#endif
	
		
				}
				else // valence too small
				{
#if defined DEBUG 
cout << "valence of "<< a_it->getFullName() << " too small : " << valence <<   " < " <<  current_start_valence << endl;
#endif
					if (evaluation_mode_)
					{
						Log.info() << "AssignBondOrderProcessor::estimatePenalty(): Error in evaluation: Valence of " << a_it->getFullName() 
												<< " is too small : " << valence   
												<<   " < " <<  current_start_valence 
												<< " Consider adding hydrogen atoms!" << endl;
					}
					return false;
				}

			}
			else // the atom is not yet closed => heuristic kicks in
			{

/*
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
					} // end of for all free bonds

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
				} // end of for all remaining valences 

#if defined DEBUG || defined DEBUG_ESTIMATE	
cout << " min for given valence:" << min_bond_length_penalty << " + " << current_bond_length_penalty ;
#endif

				//now add the minima to the atomcontainer's penalty sums
				estimated_atom_penalty += min_atom_type_penalty;
				estimated_bond_penalty += min_bond_length_penalty + current_bond_length_penalty;	

#if defined DEBUG || defined DEBUG_ESTIMATE	
cout << " => " << 	estimated_bond_penalty  << endl;
#endif

cout << "			 Hi Anne estimatePenalty () atom " << a_it->getFullName() << " heuristical best pen: " << min_atom_type_penalty << endl;*/

				// estimate the penalties //TODO	
				float current_atom_type_penalty = estimateAtomTypePenalty_(&*a_it, current_atom_index, 
			                         valence, virtual_order, num_free_bonds);

				if (current_atom_type_penalty >= 0)
					estimated_atom_penalty += current_atom_type_penalty;			
				else
					return false;
			

				//float max_bond_pen_of_free_bonds =  estimateBondLengthPenalty_(current_atom_index,	
				//		free_bonds, virtual_order,  valence, num_free_bonds);
				// if (max_bond_pen_of_free_bonds >=0)
				// {
				// 	estimated_bond_penalty += max_bond_pen_of_free_bonds;//TODO
				// }
				//estimateBondLengthPenalty_(current_atom_index,	
				//		free_bonds, virtual_order,  valence, num_free_bonds);
			
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

	// Computes the atom type penalty for a given atom
	// returns -1 if no valid atom type penalty for this atom could be computed!
	float AssignBondOrderProcessor::estimateAtomTypePenalty_(Atom* atom, Index atom_index, 
			                         int fixed_valence, int fixed_virtual_order, int num_free_bonds) // TODO const & ??? //TODO atom used??
	{	
		bool found_a_value = false;
		// we have to estimate the penalty heuristically 
		// by finding the minimum in all further possible penalty vector entries.

		// the minimal penalty of all possible penalty vectors for this atom
		float min_atom_type_penalty = std::numeric_limits<float>::max();
		
		// just look into the penalty vectors starting from the already fixed virtual order
		for (Size vi = fixed_virtual_order; vi < atom_to_block_[atom_index].size(); vi++)
		{
			// get the corresponding penalty vector
			int block = atom_to_block_[atom_index][vi];

			// certain numbers of additional hydrogens, for which no valid penalty classes exist
			// are excluded by block index -1
			if (block >= 0)
			{
				int current_start_valence = block_to_start_valence_[block];
				int current_block_length  = block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;
				int current_start_index   = block_to_start_idx_[block];
				
				if (fixed_valence <= current_end_valence)
				{
					// at which position in the current penalty vector are we at the moment?
					Position i = (fixed_valence + num_free_bonds < current_start_valence) 
						? current_start_valence
						: fixed_valence + num_free_bonds;

					// for every remaining valence of the atom under consideration
					// (we know there is at least one)
					for (; i <= (Position)current_end_valence; i++)
					{
						float current_atom_type_penalty = 	penalties_[current_start_index + i - current_start_valence];

						if (current_atom_type_penalty < min_atom_type_penalty)
						{
							min_atom_type_penalty = current_atom_type_penalty;
							found_a_value = true;
						}
					}
				}
				else
				{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  * valence explosion for atom " <<   atom->getFullName() << endl;
#endif
					if (evaluation_mode_)
					{
						Log.info() << "  AssignBondOrderProcessor: valence explosion for atom " 
							<<   atom->getFullName()  << " val: " << fixed_valence << " free bo: "<<  num_free_bonds <<  " : Ruleid " <<  block +1 << " with length:" <<  block_to_length_[block] << endl;
					}
				}
			} // end of valid block
		} // end of all virtual orders
		
		if (!found_a_value)
			min_atom_type_penalty = -1;
	
		return min_atom_type_penalty; 
	}
	

	// computes the min possible bond length penalty for all free bonds of this atom
	// returns -1 if no bond length penalty could be found!
	// Trying to distribute all free valences with minimal bond length penalty as possible 
	// over all free bonds of this atom seems to be far to expensive.
	// In addition take care of the fact, that all bonds between two unclosed atoms 
	// are counted twice -> all bonds should be counted twice!
	float AssignBondOrderProcessor::estimateBondLengthPenalty_(Index atom_index, 
			vector<Bond*> free_bonds, int fixed_virtual_order,  int fixed_valence, int num_free_bonds )
	{	
		// NOTE: virtual bond are excluded!
		
		// we have to estimate the bond length penalty heuristically 
		// by trying to distribute the remaining free valences over all remaining free bonds
		// such that the summed bond length penalty is minimal

		// the minimal penalty of all possible penalty vectors for this atom
		float min_bond_length_penalty = std::numeric_limits<float>::max();
		float min_bond_length_penalty_current_set_up =  std::numeric_limits<float>::max();	
	
		bool found_a_value = false; 

		// all possible free valences
		// NOTE: virtual hydrogens may use or don't use one free valence
		//int free_virtual_val = 0;
		//if (number_of_virtual_hydrogens_.find(atom) != number_of_virtual_hydrogens_.end())
		//	free_virtual_val = number_of_virtual_hydrogens_[atom] - fixed_virtual_order;
		
		// just look into the penalty vectors starting from the already fixed virtual order
		for (Size vi = fixed_virtual_order; vi < atom_to_block_[atom_index].size(); vi++)
		{
			// get the corresponding penalty vector
			int block = atom_to_block_[atom_index][vi];

			// certain numbers of additional hydrogens, for which no valid penalty classes exist
			// are excluded by block index -1
			if (block >= 0)
			{
				int current_start_valence = block_to_start_valence_[block];
				int current_block_length  = block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;
				int current_start_index   = block_to_start_idx_[block];
				
				// do we have an explosion ?
				if (fixed_valence <= current_end_valence)
				{
					min_bond_length_penalty_current_set_up =  std::numeric_limits<float>::max();	
					
					// iterating over all free bonds 
					// (we know there is at least one free bond)
					for (Size fb=0; fb < free_bonds.size(); fb++) 
					{
						float current_bond_min = std::numeric_limits<float>::max();	
						// Note: for simplification we do not share the free bond 
						//       orders correctly! We just find the min!
						// the number of the remaining bond orders to be assigned 
						// to the free bonds are :
						std::vector<float>& current_bond_length_penalties = bond_lengths_penalties_[free_bonds[fb]];
						int up_to = std::min((int)current_bond_length_penalties.size()-1, 
																 current_end_valence -(num_free_bonds-1)-fixed_valence); //i-fixed_valence-(num_free_bonds-1));
						
						// for all possible valences for this bond
						for (Size j = 1; j <= (Size)up_to; j++)
						{
							float deviation = current_bond_length_penalties[j]; 

							if (deviation < current_bond_min)
							{
								current_bond_min = deviation;
							}
						} // end of for all possible valences for this bond
						min_bond_length_penalty_current_set_up	+=current_bond_min;
					} // end of for all free bonds
					
					if (min_bond_length_penalty_current_set_up < min_bond_length_penalty_current_set_up )
						 min_bond_length_penalty = min_bond_length_penalty_current_set_up;

				} // end of no explosion
			}// end of valid block
		}
		if (!found_a_value)
			min_bond_length_penalty = -1; 

		return min_bond_length_penalty;
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
				
				// the current max and min bond length deviation
				// bonds without predefined lenght penalty will get penalty zero!
				// This is no problem for the heuristic, since this bond will
				// get zero in all PQEntries.
				float max_bond_length_deviation = 0.; 
				float min_bond_length_deviation = std::numeric_limits<float>::max();

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
							if (min_bond_length_deviation > penalties[i])
							{
								min_bond_length_deviation = penalties[i];
							}
						}
						else
						{
							complete = false;
						}
					}
					
					// store the min at position zero
					penalties[0] = min_bond_length_deviation;

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

		// Hi Anne
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
			

// Hi Anne
 //cout << "bond_length_normalization_factor_:" << 	bond_length_normalization_factor_<< endl;
//cout << " atom_type_normalizsation_factor_:" << atom_type_normalization_factor_ << endl;
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

	// finds the index of the __first__ matching penalty definition 
	// returns -1 otherwise 
	int AssignBondOrderProcessor::getPenaltyClass_(Atom* atom)
	{
		// find the __first__ matching atom definition, whose index gives the block
		for (Size j = 0; j < block_definition_.size(); j++)
		{	
			Expression exp(block_definition_[j].second);
			if (   (atom->getElement().getSymbol() ==  block_definition_[j].first) 
					&& (exp(*atom)))
			{

#ifdef DEBUG_READ
cout << "Treffer : " << atom->getFullName() << " with index " << atom->getIndex() << " " << block_definition_[j].first << ": "
		<<  block_definition_[j].second << " block: " << j+1 << endl;
#endif
				// NOTE: we start counting at 0!
				return j; 
			}
		}
		Log.info() << "Warning: No penalty type found for atom " 
								<< atom->getFullName() << " in " << __FILE__ << " " <<  __LINE__ << endl;

		return -1;
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
			atom_to_block_.clear(); // needed in case this function is called twice!
			vector<int> tmp(1,-1);
			atom_to_block_.resize(no_atoms,tmp);

			// initialize the inverse atom type penalty normalization factor 
			atom_type_normalization_factor_ = 0.;
		
			// create the dummy bond needed for all virtual bonds 
			if (add_missing_hydrogens_) 
			{
				if (!virtual_bond_)
				{
					virtual_bond_ = new Bond;
					virtual_bond_->setProperty("VIRTUAL__BOND", true);
					index_to_bond_.push_back(virtual_bond_);
					bond_to_index_[virtual_bond_] = index_to_bond_.size()-1;
					bond_fixed_[virtual_bond_] 	= 0;
				}
			
				// clear the virtual bond stuff
				num_of_virtual_bonds_ = 0;
				number_of_virtual_hydrogens_.clear();
				virtual_bond_index_to_number_of_virtual_hydrogens_.clear();
				virtual_bond_index_to_atom_.clear();
				atom_to_virtual_bond_index_.clear();
			}

			// for each atom
			for (Position i = 0; i < no_atoms; ++i)
			{
				Atom* at = a_tmp.getAtom(i);

				// for reasons only known to BALL, getIndex yields completely strange results for
				// the atoms here, so we need to add a Property which we can later read out
				ac_->getAtom(i)->setProperty("AssignBondOrderProcessorAtomNo", i);
				bool found = false; 
#ifdef DEBUG_READ
cout << at->getFullName() << endl;
#endif
				// add maximal penalty to the inverse atom type normalization factor
				float max_penalty_per_atom = 0;
			
				// find the __first__ matching atom definition, whose index gives the block
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
						// store the penalty block's index
						// NOTE: we start counting at 0!
						atom_to_block_[i][0]=j;
						found = true;

						for(Size k = 0; k < block_to_length_[j]; k++)
						{
							float current_pen = penalties_[k + block_to_start_idx_[j]];
							max_penalty_per_atom = std::max(max_penalty_per_atom, current_pen);
						}

					}
				}
				
				if (add_missing_hydrogens_)
				{
					// at this point, we cannot use the atom pointer "at", since it points
					// into a copy of the molecule we really want to process. hence, we have
					// to take the atom from the original atom container
					float max_virtual_penalty = computeVirtualHydrogens_(ac_->getAtom(i));

					max_penalty_per_atom = std::max(max_virtual_penalty, max_penalty_per_atom);
				}

				if (max_penalty_per_atom >= 0)
					atom_type_normalization_factor_+= max_penalty_per_atom;
				else
					found = false;


				if (!found)
				{
					Log.error() << "AssignBondOrderProcessor: No penalty type found for atom " 
											<< at->getFullName() << endl;

					atom_to_block_[i][0]=-1;
					return false;
				}
			}


#ifdef UNNORMALIZED_DEBUG
atom_type_normalization_factor_ = 0.;
#endif 			

#if defined DEBUG_ADD_HYDROGENS || defined DEBUG
cout << " ~~~~~~~~~atom_to_block~~~~~~~~~~~~~" << endl;
for (Size i=0; i < 	atom_to_block_.size() ; i++)
{	
	Atom* at = a_tmp.getAtom(i);
	cout << at-> getFullName()<< " : ";
	for (Size j=0; j < atom_to_block_[i].size(); j++)
	{
		cout << atom_to_block_[i][j] << " "; 
	}
	cout << endl;
}
cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
#endif

		}
		else
		{
			Log.error() << "AssignBondOrderProcessor: There was no valid AtomContainer set." << endl;
			return false;
		}

		return true;
	}


	// NOTE: add the moment we assume, that this method is called 
	//       AFTER preassignPenaltyClasses() and precomputeBondLengthPenalties()
	float AssignBondOrderProcessor::computeVirtualHydrogens_(Atom* atom)
	{
	#if defined DEBUG_ADD_HYDROGENS || defined DEBUG
		cout << " - - computeVirtualHydrogens  - - - - - - - - " << endl;
	#endif
		
		float max_penalty = 0;

		// iterate over all atoms and add a hydrogen atom and bond 
		// 	if suggested by the hybridisation state 
		int free_valences = 0;

		vector<Atom*> atoms_to_delete;

		int num_of_atoms = ac_->countAtoms() +1; 

		if (  (atom->getElement() == PTE[Element::H])
				||(atom->getElement() == PTE[Element::He]))
			free_valences = atom->getElement().getGroup() - atom->countBonds();
		else
			free_valences = 18 - atom->getElement().getGroup() - atom->countBonds();
		
		if (free_valences > 0)
		{
			//
			//  insert this virtual bond into the processors speed-up data structures
			//
			// NOTE: a virtual bond represents all possible hydrogen bond for an atom
			num_of_virtual_bonds_++;
			// store the max number of possible additional hydrogens 
			number_of_virtual_hydrogens_[atom] = free_valences;
			virtual_bond_index_to_number_of_virtual_hydrogens_.push_back(free_valences);
			// connect the virtual bond to this atom via its index
			virtual_bond_index_to_atom_.push_back(atom);
			atom_to_virtual_bond_index_[atom] = num_of_virtual_bonds_ -1;

			// NOTE: these datastructures are for both original and virtual bonds!
			index_to_bond_.push_back(virtual_bond_);
			free_bonds_.push_back(virtual_bond_);	

			for (Size i = 1; i <= free_valences; i++)
			{
				// add a hydrogen atom  
				Atom* hydrogen = new Atom;
				hydrogen->setElement(PTE[Element::H]);	
				hydrogen->setName((PTE[Element::H].getSymbol())+String(num_of_atoms));
				num_of_atoms++;
				hydrogen->setProperty("VIRTUAL__ATOM", true);
				atom->getFragment()->insert(*hydrogen);
				atoms_to_delete.push_back(hydrogen);
				
				// and a bond to the atomcontainer
				Bond* new_bond = atom->createBond(*hydrogen);
				new_bond->setProperty("VIRTUAL__BOND", true);
				new_bond->setOrder(1);

				// update the penalty classes! 
				int penalty_class = getPenaltyClass_(atom);
				int atom_index = atom->getProperty("AssignBondOrderProcessorAtomNo").getUnsignedInt();
				atom_to_block_[atom_index].push_back(penalty_class);

				if (penalty_class != -1)
				{
					for(Size k = 0; k < block_to_length_[penalty_class]; k++)
					{
						float current_pen = penalties_[k + block_to_start_idx_[penalty_class]];
						max_penalty = std::max(max_penalty, current_pen);
					}
				}
			}
			
			// delete all virtual atoms and bonds
			for (Size i=0; i < atoms_to_delete.size(); i++)
			{
				//atoms_to_delete[i]->destroy();
				delete(atoms_to_delete[i]);//->destroy(); //TODO: what? valgrind
				//NOTE: doing so, all adjacent bonds will be deleted automatically
			}
		}

		return max_penalty;
	}


	void AssignBondOrderProcessor::setDefaultOptions()
	{		
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS); 
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS); 
	
	 	options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS); 
	 	
		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS, 
	 												 AssignBondOrderProcessor::Default::OVERWRITE_SELECTED_BONDS); 

		options.setDefaultBool(AssignBondOrderProcessor::Option::ADD_HYDROGENS,
												   AssignBondOrderProcessor::Default::ADD_HYDROGENS);	

		options.setDefaultBool(AssignBondOrderProcessor::Option::OVERWRITE_CHARGES,
												   AssignBondOrderProcessor::Default::OVERWRITE_CHARGES);	
		
		options.setDefaultBool(AssignBondOrderProcessor::Option::ASSIGN_CHARGES,
													 AssignBondOrderProcessor::Default::ASSIGN_CHARGES);	
	
		options.setDefaultBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS,
													 AssignBondOrderProcessor::Default::KEKULIZE_RINGS);	
		
		options.setDefault(AssignBondOrderProcessor::Option::INIFile,
													 AssignBondOrderProcessor::Default::INIFile);		
		
		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_BOND_ORDER,
													 AssignBondOrderProcessor::Default::MAX_BOND_ORDER);

		options.setDefaultInteger(AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS,
													    AssignBondOrderProcessor::Default::MAX_NUMBER_OF_SOLUTIONS);

		options.setDefaultBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS,
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
				
				// delete all former VIRTUAL BONDs
				// did we already applied a solution?
				if (last_applied_solution_>=0)
				{	
					for (Size j=0; j < solutions_[last_applied_solution_].atoms_to_delete.size(); j++)
					{
						solutions_[last_applied_solution_].atoms_to_delete[j]->destroy();
						//NOTE: all adajacent bonds of these atoms will be deleted automatically
					}
				}

				// set all the original bonds to the assigned order
				AtomIterator a_it = ac_->beginAtom();
				Atom::BondIterator b_it = a_it->beginBond();
				
				HashMap<Bond*, int> tmp_bond_orders = solutions_[i].bond_orders;

				BALL_FOREACH_BOND(*ac_, a_it, b_it)
				{
					HashMap<Bond*, int>::Iterator it = tmp_bond_orders.find(&*b_it);
					if (it != solutions_[i].bond_orders.end())
					{
						b_it->setOrder(it->second);
					}
				}

				// for each virtual bond add the corresponding number of hydrogens
				// TODO can we make use of this??
				solutions_[i].atoms_to_delete.clear();
				//solutions_[i].bonds_to_delete.clear();

				HashMap<Atom*, int> tmp_virtual_hydrogens = solutions_[i].number_of_virtual_hydrogens;
				// get the averaged bond lengths
		//	BuildBondsProcessor bbp; 
		//	HashMap<Size, HashMap<Size, HashMap<Bond::BondOrder, float> > > bond_lengths = bbp.getBondMap();
			
				int num_of_atoms = ac_->countAtoms()+1;

				vector<Vector3> pos;
				pos.push_back(Vector3(1.,0.,0.));
				pos.push_back(Vector3(0.,1.,0.));
				pos.push_back(Vector3(0.,0.,1.));
				pos.push_back(Vector3(0.7,0.,0.7));

				a_it = ac_->beginAtom();
				b_it = a_it->beginBond();
				BALL_FOREACH_ATOM(*ac_, a_it)
				{
					HashMap<Atom*, int>::Iterator it = tmp_virtual_hydrogens.find(&*a_it);
					if (it != solutions_[i].number_of_virtual_hydrogens.end())
					{
						Size missing_hydrogens = (Size)it->second;
						for (Size m=0; m < missing_hydrogens; m++)
						{
							// add a hydrogen atom  
							Atom* hydrogen = new Atom;
							hydrogen->setElement(PTE[Element::H]);
							hydrogen->setName(PTE[Element::H].getSymbol()+String(num_of_atoms));
							num_of_atoms++;
							hydrogen->setProperty("VIRTUAL__ATOM", true);
							a_it->getFragment()->insert(*hydrogen);
							solutions_[i].atoms_to_delete.push_back(hydrogen);

							// TODO set the Position correctly
							hydrogen->setPosition(a_it->getPosition() + pos[m] );
							
							// and a bond
							Bond* new_bond = a_it->createBond(*hydrogen);
							new_bond->setProperty("VIRTUAL__BOND", true);
							new_bond->setOrder(1);
							//solutions_[i].bonds_to_delete.push_back(new_bond);
						}
					}
				}


				// denote the application of this solution
				last_applied_solution_ = i;

				if (!options.getBool(AssignBondOrderProcessor::Option::KEKULIZE_RINGS))
				{
					// find all rings
					vector<vector<Atom*> > rings;
					RingPerceptionProcessor rpp;
					rpp.calculateSSSR(rings, *ac_);

					// set the aromatic rings	
					AromaticityProcessor ap;
					ap.options.setBool(AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS, true); 
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
		Solution_ sol;
		bool found_a_sol = false;

		if (add_missing_hydrogens_)
		{
			// the method apply may have been called before
			// such that we have to delete the virtual hydrogens
			if (last_applied_solution_>=0)
			{	
				for (Size j=0; j < solutions_[last_applied_solution_].atoms_to_delete.size(); j++)
				{
					//NOTE: all adajacent bonds of these atoms will be deleted automatically
					solutions_[last_applied_solution_].atoms_to_delete[j]->destroy();
				}
			}
		}

		if (options.get(Option::ALGORITHM) == Algorithm::A_STAR)
		{
			found_a_sol = performAStarStep_(sol);
		}
		else // ILP
		{
#ifdef BALL_HAS_LPSOLVE
			// first find out if there is a further solution
			int number_of_solutions = get_solutioncount(ilp_);

			if (solutions_.size() >= (Size)number_of_solutions)
				return false;

			// set the solution number of the next solution to return
			// NOTE: count starts at 1
			set_solutionlimit(ilp_, solutions_.size());

			found_a_sol = solveILP_(sol);
#else
			Log.error() << "AssignBondOrderProcessor: BALL was configured without lp_solve support! Try using A_STAR instead!" << std::endl;
#endif
		}

		if (found_a_sol)
		{
			bool next_solution_is_optimal = (getTotalPenalty(solutions_[0]) == getTotalPenalty(sol)); 

				solutions_.push_back(sol);
				apply(solutions_.size()-1);
				
				return true;
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
				// TODO ALEX
				Size current_length = block_to_length_[atom_to_block_[current_atom][0]];
				
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
				Position count_vars = count_b; //TODO ALEX
				for(Position j = 0; j < block_to_length_[atom_to_block_[i][0]]; ++j, ++ind_y, ++count_vars)
				{
					// Choice variables have indices > ilp_number_of_free_bonds_ in variable vector for ILP
					obj_indices[ind_y]  = ind_y + ilp_number_of_free_bonds_ + 1;
					cons_indices[count_vars] = obj_indices[ind_y];

					// Set valences  //TODO ALEX
					cons_prefactors[count_vars] = block_to_start_valence_[atom_to_block_[i][0]] + j;

					// Set penalties //TODO ALEX
					obj_prefactors[ind_y]       = penalties_[block_to_start_idx_[atom_to_block_[i][0]] + j];
				}

				// in case we got an empty penalty cons_prefactors //TODO ALEX
				if (block_to_length_[atom_to_block_[i][0]] != 0)
				{
					// Add valence constraint
					if (!add_constraintex(ilp_, count_vars, &cons_prefactors[0], &cons_indices[0], EQ,
								fixed_val_[i]))
					{
						Log.error() << "AssignBondOrderProcessor: Setting valence constraint for ILP failed" << endl;
						return false;
					}
					// Add choice constraint //TODO ALEX
					if (!add_constraintex(ilp_, block_to_length_[atom_to_block_[i][0]], &choices[0], &cons_indices[count_b], EQ, 1))
					{
						Log.error() << "AssignBondOrderProcessor: Setting choice constraint for ILP failed" << endl;
						return false;
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
			number_of_virtual_hydrogens(),
			atoms_to_delete(),
			atom_type_penalty(0.),
			bond_length_penalty(0.)
	{
	}

	AssignBondOrderProcessor::Solution_::~Solution_()
	{
	}
	
	void AssignBondOrderProcessor::Solution_::clear()
	{
		valid = false;
		bond_orders.clear();	
		number_of_virtual_hydrogens.clear();
		atoms_to_delete.clear();
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
		alpha_ = 0;
		estimated_atom_type_penalty = 0.;
		estimated_bond_length_penalty = 0.;
		last_bond = 0;
		bond_orders.clear();
		atom_type_normalization_factor_ = 0;
		bond_length_normalization_factor_ = 0;

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
		// NOTE: this function is *never* allowed to add missing hydrogens 
		//  => store its flag temporarily and set the flag again in the end
		bool add_missing_hydrogens_backup = add_missing_hydrogens_;
		add_missing_hydrogens_ = false;

		float ret = -1;

		evaluation_mode_ = true;
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
				entry.bond_orders.resize(ac->countBonds(),-1);
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
				{	
					evaluation_mode_ = false;
					ret = entry.coarsePenalty();
				}
				else
				{
					Log.info() << "AssignBondOrderProcessor::evaluatePenalty(): estimatePenalty() returned an error!" << endl;
					evaluation_mode_ = false;
					ret = -1.;
				}
			}
		}
		else
		{
			Log.info() << "Error in evaluatePenalty: valid: " <<  valid_ << " readAtomPenalties:" << readAtomPenalties_() << endl;
		}
	
		evaluation_mode_ = false;

		add_missing_hydrogens_ = add_missing_hydrogens_backup;

		return ret;
	}


} // namespace BALL
