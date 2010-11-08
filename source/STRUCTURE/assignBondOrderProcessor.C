#include <iostream>
#include <map>

// BALL includes
#include <BALL/kernel.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/KERNEL/expression.h>

// Qt
#include <QtXml/QtXml>
#include <QtXml/qdom.h>

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

//#define DEBUG_RULES
#undef DEBUG_RULES

//#define DEBUG_PRINT_RULES
#undef DEBUG_PRINT_RULES

//#define DEBUG_BOND_LENGTH
#undef DEBUG_BOND_LENGTH

//#define DEBUG_TIMER
#undef DEBUG_TIMER
 
#define INFINITE_PENALTY 1e5

using namespace std;

namespace BALL 
{	
	const String AssignBondOrderProcessor::Heuristic::SIMPLE = "heurisic_simple";
	const String AssignBondOrderProcessor::Heuristic::MEDIUM = "heuristic_medium";
	const String AssignBondOrderProcessor::Heuristic::TIGHT = "heuristic_tight";

	const String AssignBondOrderProcessor::Algorithm::A_STAR = "a_star";
	const String AssignBondOrderProcessor::Algorithm::ILP = "ilp";
	const String AssignBondOrderProcessor::Algorithm::K_GREEDY = "k_greedy";
	const String AssignBondOrderProcessor::Algorithm::BRANCH_AND_BOUND = "branch_and_bound";

	const char* AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS = "overwrite_single_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SINGLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS = "overwrite_double_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_DOUBLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS = "overwrite_triple_bond_orders";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_TRIPLE_BOND_ORDERS = true;
	
	const char* AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS = "overwrite_selected_bonds";
	const bool  AssignBondOrderProcessor::Default::OVERWRITE_SELECTED_BONDS = false;

	const char* AssignBondOrderProcessor::Option::ADD_HYDROGENS = "add_hydrogens_by_processor";
	const bool  AssignBondOrderProcessor::Default::ADD_HYDROGENS = false;
	
	const char* AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY = "compute_also_connectivity"; 
	const bool AssignBondOrderProcessor::Default::COMPUTE_ALSO_CONNECTIVITY = false; 
	
	const char* AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF = "bond_connectivity_cutoff";
	const float AssignBondOrderProcessor::Default::CONNECTIVITY_CUTOFF = 2.4; 

	const char* AssignBondOrderProcessor::Option::USE_FINE_PENALTY = "use_fine_penalty";
	const bool  AssignBondOrderProcessor::Default::USE_FINE_PENALTY = true;

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
		
	const char* AssignBondOrderProcessor::Option::HEURISTIC = "heuristic";
	const String AssignBondOrderProcessor::Default::HEURISTIC = AssignBondOrderProcessor::Heuristic::TIGHT;
	
	const char* AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING = "bond_length_weighting";
	const float AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING = 0.;

	const char* AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION = "apply_first_solution";
	const bool  AssignBondOrderProcessor::Default::APPLY_FIRST_SOLUTION = true;
	
	const char* AssignBondOrderProcessor::Option::GREEDY_K_SIZE = "size_of_greedy_priority_queue";
	const int  AssignBondOrderProcessor::Default::GREEDY_K_SIZE = 5;
	
	const char* AssignBondOrderProcessor::Option::BRANCH_AND_BOUND_CUTOFF = "branch_and_bound_cutoff";
	const float  AssignBondOrderProcessor::Default::BRANCH_AND_BOUND_CUTOFF = 1.2;

	float AssignBondOrderProcessor::PQ_Entry_::alpha = 0.;
	float AssignBondOrderProcessor::PQ_Entry_::atom_type_normalization_factor = 1.;
	float AssignBondOrderProcessor::PQ_Entry_::bond_length_normalization_factor = 1.;
	bool  AssignBondOrderProcessor::PQ_Entry_::use_fine_penalty = true;

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
			ilp_const_penalty_(0.f),
			total_num_of_bonds_(0),
			num_of_free_bonds_(0),
			fixed_val_(),
			solutions_(),
			starting_configuration_(),
			atom_type_normalization_factor_(0.),
			bond_length_normalization_factor_(0.),
			last_applied_solution_(-1),
			ac_(0),
			max_bond_order_(),
			alpha_(),
			max_number_of_solutions_(),
			compute_also_non_optimal_solutions_(),
			add_missing_hydrogens_(),
			compute_also_connectivity_(),
			use_fine_penalty_(),
			heuristic_index_(SIMPLE),
			greedy_atom_type_penalty_(0.),
			greedy_bond_length_penalty_(0.),
			greedy_node_expansions_(0),
			queue_(),
			penalties_(),
			block_to_start_idx_(),
			block_to_length_(),
			block_to_start_valence_(),
			block_definition_(),
			atom_to_block_(),
			bond_lengths_penalties_(),
			step_(0),
			timer_()
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
			ilp_const_penalty_(abop.ilp_const_penalty_),
			total_num_of_bonds_(abop.total_num_of_bonds_),
			num_of_free_bonds_(abop.num_of_free_bonds_),
			fixed_val_(abop.fixed_val_),
			solutions_(abop.solutions_),
			starting_configuration_(starting_configuration_),
			atom_type_normalization_factor_(abop.atom_type_normalization_factor_),
			bond_length_normalization_factor_(abop.bond_length_normalization_factor_),
			last_applied_solution_(abop.last_applied_solution_),
			ac_(abop.ac_),
			max_bond_order_(abop.max_bond_order_),
			alpha_(abop.alpha_),
			max_number_of_solutions_(abop.max_number_of_solutions_),
			compute_also_non_optimal_solutions_(abop.compute_also_non_optimal_solutions_),
			add_missing_hydrogens_(abop.add_missing_hydrogens_),	
			compute_also_connectivity_(abop.compute_also_connectivity_),
			use_fine_penalty_(abop.use_fine_penalty_),	
			heuristic_index_(abop.heuristic_index_),
			greedy_atom_type_penalty_(abop.greedy_atom_type_penalty_),
			greedy_bond_length_penalty_(abop.greedy_bond_length_penalty_),
			greedy_node_expansions_(abop.greedy_node_expansions_),
			queue_(abop.queue_),
			penalties_(abop.penalties_),
			block_to_start_idx_(abop.block_to_start_idx_),
			block_to_length_(abop.block_to_length_),
			block_to_start_valence_(abop.block_to_start_valence_),
			block_definition_(abop.block_definition_),
			atom_to_block_(abop.atom_to_block_),
			bond_lengths_penalties_(abop.bond_lengths_penalties_),
			step_(abop.step_),
			timer_()
	{
#ifdef BALL_HAS_LPSOLVE
		if (abop.ilp_)
		{
			ilp_ = copy_lp(abop.ilp_);
		}
		else
		{
			ilp_ = 0;
		}
#endif
	}

	AssignBondOrderProcessor::~AssignBondOrderProcessor()
	{
		clear();

		delete(virtual_bond_);

#ifdef BALL_HAS_LPSOLVE
		if (ilp_)
		{
			delete_lp(ilp_);
		}
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

		// if we already had a virtual bond, we need to get it out of the way
		delete(virtual_bond_);
		virtual_bond_ = abop.virtual_bond_; 

		ilp_index_to_free_bond_ = abop.ilp_index_to_free_bond_;
		ilp_number_of_free_bonds_ = abop.ilp_number_of_free_bonds_;
		ilp_const_penalty_ = abop.ilp_const_penalty_;
		total_num_of_bonds_ = abop.total_num_of_bonds_;
		num_of_free_bonds_ = abop.num_of_free_bonds_;
		fixed_val_ = abop.fixed_val_;
		solutions_ = abop.solutions_;
		starting_configuration_ = abop.starting_configuration_;
		atom_type_normalization_factor_ = abop.atom_type_normalization_factor_;
		bond_length_normalization_factor_ = abop.bond_length_normalization_factor_;
		last_applied_solution_ = abop.last_applied_solution_;
		ac_ = abop.ac_;
		max_bond_order_ = abop.max_bond_order_;
		alpha_ = abop.alpha_;
		max_number_of_solutions_ = abop.max_number_of_solutions_;
		compute_also_non_optimal_solutions_ = abop.compute_also_non_optimal_solutions_;
		add_missing_hydrogens_ = abop.add_missing_hydrogens_;	
		compute_also_connectivity_ = abop.compute_also_connectivity_;
		use_fine_penalty_ = abop.use_fine_penalty_;	
		heuristic_index_ = abop.heuristic_index_;	
		greedy_atom_type_penalty_ = abop.greedy_atom_type_penalty_,
		greedy_bond_length_penalty_ = abop.greedy_bond_length_penalty_,
		greedy_node_expansions_ = abop.greedy_node_expansions_;
		queue_ = abop.queue_;
		penalties_ = abop.penalties_;
		block_to_start_idx_ = abop.block_to_start_idx_;
		block_to_length_ = abop.block_to_length_;
		block_to_start_valence_ = abop.block_to_start_valence_;
		block_definition_ = abop.block_definition_;
		atom_to_block_ = abop.atom_to_block_;
		bond_lengths_penalties_ = abop.bond_lengths_penalties_;
		step_ = abop.step_;
		timer_ = abop.timer_;

#ifdef BALL_HAS_LPSOLVE
		if (ilp_)
		{
			delete_lp(ilp_);
		}
		if (abop.ilp_)
		{
			ilp_ = copy_lp(abop.ilp_);
		}
		else
		{
			ilp_ = 0;
		}
#endif

		return *this;
	}

	void AssignBondOrderProcessor::clear()
	{
		//NOTE: options should remain!!
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

		delete(virtual_bond_);
		virtual_bond_ = NULL; 

		ilp_index_to_free_bond_.clear();
		ilp_number_of_free_bonds_ = 0;
		ilp_const_penalty_ = 0.f;
		total_num_of_bonds_ = 0;
		num_of_free_bonds_ = 0;

		fixed_val_.clear();
		solutions_.clear();
		starting_configuration_.clear();
		atom_type_normalization_factor_ = 0.;
		bond_length_normalization_factor_ = 0.;

		last_applied_solution_ = -1;
		ac_ = 0;
		
		greedy_atom_type_penalty_ = 0;
		greedy_bond_length_penalty_ = 0;
		greedy_node_expansions_ = 0;

		queue_ = priority_queue<PQ_Entry_>();
		penalties_.clear();
		block_to_start_idx_.clear();
		block_to_length_.clear();
		block_to_start_valence_.clear();
		block_definition_.clear();
		atom_to_block_.clear();
		bond_lengths_penalties_.clear();
		step_ = 0;
		timer_.clear(); 

#ifdef BALL_HAS_LPSOLVE
		if (ilp_)
		{
			delete_lp(ilp_);
		}
		ilp_ = 0;
#endif
	}
		
	bool AssignBondOrderProcessor::readOptions_()
	{
		bool ret = true;

		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);
		max_number_of_solutions_ = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
		compute_also_non_optimal_solutions_ = options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS);
		add_missing_hydrogens_ = options.getBool(Option::ADD_HYDROGENS);
		compute_also_connectivity_ = options.getBool(Option::COMPUTE_ALSO_CONNECTIVITY);
		use_fine_penalty_ = options.getBool(Option::USE_FINE_PENALTY);
		
		String heuristic = options.get(Option::HEURISTIC);
		if (heuristic == Heuristic::TIGHT)
			heuristic_index_ = TIGHT;
		else if (heuristic == Heuristic::MEDIUM)
			heuristic_index_ = MEDIUM;
		else if (heuristic == Heuristic::SIMPLE)
			heuristic_index_ = SIMPLE;

		if (max_bond_order_ <= 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::MAX_BOND_ORDER."  << endl;
			ret = false;
		}
		if (max_number_of_solutions_ < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::MAX_NUMBER_OF_SOLUTIONS."  << endl;
			ret = false;
		}
		if ((alpha_ < 0) || ((alpha_ > 1)))
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::BOND_LENGTH_WEIGHTING."  << endl;
			ret = false;
		}

		if (options.getReal(Option::CONNECTIVITY_CUTOFF) < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::CONNECTIVITY_CUTOFF."  << endl;
			ret = false;
		}
		
		if (options.getInteger(Option::GREEDY_K_SIZE) < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::GREEDY_K_SIZE."  << endl;
			ret = false;
		}
		
		if (options.getReal(Option::BRANCH_AND_BOUND_CUTOFF) < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::BRANCH_AND_BOUND_CUTOFF."  << endl;
			ret = false;
		}
		
		valid_ = ret;
		return ret;
	}

	bool AssignBondOrderProcessor::start()
	{
		clear();
		valid_ = readAtomPenalties_();
		evaluation_mode_ = false;
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
			step_++;

#ifdef DEBUG
cout << "    atom type penalty: " << entry.estimated_atom_type_penalty << 
				"    bond length penalty: " << entry.estimated_bond_length_penalty << 
				"    bond orders: ( " ;
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
				Solution_ sol(entry, this, step_, queue_.size());
				solutions_.push_back(sol);

				return true;
			}
			else // no leaf
			{	
				entry.last_bond++;
				Bond* current_bond = index_to_bond_[entry.last_bond];

				// Take the next bond and ...
				//   ... set to the prefixed value ... 
				if (bond_fixed_[current_bond])
				{	
					// Set this fixed bond order
					entry.bond_orders[entry.last_bond] = bond_fixed_[current_bond];

					// Estimate the penalty
					if (estimatePenalty_(entry))
					{
						queue_.push(entry);
					}
				}	
				//   ... or try all bond orders
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

							// Penalty estimation went well  
							if (estimatePenalty_(entry))
							{
								// Add to the queue 
								queue_.push(entry);
							}
						} // end of all bond orders
					} // end of case 2
				}	// end of free bond	
			} // no valid bond order try -> do nothing
		}

		// If we had found a solution, we would have bailed out already
		return false;
	}
 	
	bool AssignBondOrderProcessor::performBranchAndBound_()
	{
		PQ_Entry_ entry;

		float branch_and_bound_cutoff = 
		 			entry.coarsePenalty(greedy_atom_type_penalty_, greedy_bond_length_penalty_) 
				* options.getReal(Option::BRANCH_AND_BOUND_CUTOFF); 
					
		// try to find a solution
		while(!queue_.empty())
		{	
#ifdef DEBUG
cout << " Next Branch and Bound step : queue size : " << queue_.size();
#endif
			// take the top entry of the queue
			entry = queue_.top();
			queue_.pop();
			
			step_++;

#ifdef DEBUG
cout << "    atom type penalty: " << entry.estimated_atom_type_penalty << 
				"    bond length penalty: " << entry.estimated_bond_length_penalty << 
				"    bond orders: ( " ;
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
				Solution_ sol(entry, this, step_, queue_.size());
				solutions_.push_back(sol);

				return true;
			}
			else // no leaf
			{	
				entry.last_bond++;
				Bond* current_bond = index_to_bond_[entry.last_bond];
				
				// Take the next bond and ...
				//   ... set to the prefixed value ... 
				if (bond_fixed_[current_bond])
				{	
					// Set this fixed bond order
					entry.bond_orders[entry.last_bond] = bond_fixed_[current_bond];

					// Estimate the penalty
					if (estimatePenalty_(entry))
					{
						queue_.push(entry);
					}
				}	
				//   ... or try all bond orders
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
							if (   estimatePenalty_(entry) 
									&& (entry.coarsePenalty() <= branch_and_bound_cutoff ))
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

							estimatePenalty_(entry);

							// Penalty estimation went well  
							if (estimatePenalty_(entry) && (entry.coarsePenalty() <= branch_and_bound_cutoff ))
							{
								// Add to the queue 
								queue_.push(entry);
							}
						} // end of all bond orders
					} // end of case 2
				}	// end of free bond	
			} // no valid bond order try -> do nothing
		}

		// If we had found a solution, we would have bailed out already
		return false;
	}

	vector<AssignBondOrderProcessor::PQ_Entry_> AssignBondOrderProcessor::performGreedy_(PQ_Entry_& entry, Size greedy_k)
	{
		vector<PQ_Entry_> greedy_set(greedy_k, entry);
		// this is needed, since in the beginning, we have not yet seen k entries
		Size greedy_set_size = 1; 
		greedy_node_expansions_ = 0;
		
		for (Position i =0; i < ac_->countBonds(); i++)
		{
			// Is this bond fixed?
			if (bond_fixed_[index_to_bond_[entry.last_bond]])
			{
				// Store this bond order in all nodes of our k-greedy-set
				Size min_size = std::min((Size)greedy_set.size(), greedy_k);

				for (Size j=0; j < min_size; j++)
				{
					PQ_Entry_& current_entry = greedy_set[j];
					current_entry.last_bond = i;
					current_entry.bond_orders[current_entry.last_bond] = bond_fixed_[index_to_bond_[current_entry.last_bond]];
					// we don't need to check the return value since invalid solutions have INFINITE_PENALTY
					estimatePenalty_(current_entry, false);
				}
			}
			else // Bond is free -> try all bond orders for all nodes in our greedy set
			{	
				queue_ = priority_queue<PQ_Entry_>();
				Size min_size =  std::min(greedy_set_size, greedy_k);
				for (Size j=0; j < min_size; j++)
				{
					greedy_node_expansions_++;
					PQ_Entry_& current_entry = greedy_set[j];
					current_entry.last_bond = i;

					// case 1: VIRTUAL__BOND 
					if (current_entry.last_bond >= total_num_of_bonds_ && add_missing_hydrogens_)
					{
						int virtual_bond_index = current_entry.last_bond - total_num_of_bonds_;
						int max_virtual_hydrogens = 
							virtual_bond_index_to_number_of_virtual_hydrogens_[virtual_bond_index];
						
						for (int vh = 0; vh <= max_virtual_hydrogens; vh++)
						{
							// Set the bond order
							PQ_Entry_ new_entry(current_entry);
							new_entry.bond_orders[i] = vh;

							// we don't need to check the return value since invalid solutions have INFINITE_PENALTY
							estimatePenalty_(new_entry, false);
							queue_.push(new_entry);
						}
					}
					else // case 2: original bond
					{	
						for (int bo = 1; bo <= max_bond_order_; bo++)
						{
							// Set the bond order
							PQ_Entry_ new_entry(current_entry);
							new_entry.bond_orders[i] = bo;

							// we don't need to check the return value since invalid solutions have INFINITE_PENALTY
							estimatePenalty_(new_entry, false);
							queue_.push(new_entry);
						}
					} // end of case 2
				} // end of greedy set 
				
				greedy_set_size = 0; 
				// now copy at most k elements from the queue to the greedy set
				for (Position current_element = 0; 
						     			(current_element < greedy_k) && !queue_.empty(); 
								 			current_element++)
				{
					greedy_set[current_element] = queue_.top();
					queue_.pop();
					greedy_set_size++;
				}
			} // end of bond is free
		}
		if (greedy_set_size < greedy_k)
			greedy_set.resize(greedy_set_size);
		return greedy_set;
	}

	Processor::Result AssignBondOrderProcessor::operator () (AtomContainer& ac)
	{
#if defined DEBUG_TIMER	
		timer_.start();
#endif				

#ifdef DEBUG 
cout << "  OPTIONS:" << endl;
cout << " \t Algorithm: " <<  options[Option::Option::ALGORITHM] << endl;
cout << " \t Heuristic: " <<  options[Option::Option::HEURISTIC] << endl;

cout << " \t Overwrite bonds (single, double, triple, selected):" 
		 << options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS) << " " 
		 << options.getBool(Option::OVERWRITE_SELECTED_BONDS) << " " 
		<< endl;

cout << " \t Add hydrogens : " << options.getBool(Option::ADD_HYDROGENS) << endl;
cout << " \t Use fine penalty : " << options.getBool(Option::USE_FINE_PENALTY) << endl;
cout << " \t Kekulizer: " << options.getBool(Option::KEKULIZE_RINGS)  << endl;
cout << " \t Penalty file " << options[Option::Option::INIFile] << endl;
cout << " \t alpha: " << options[Option::BOND_LENGTH_WEIGHTING] << endl;
cout << " \t max bond order: " << options[Option::MAX_BOND_ORDER] << endl;
cout << " \t max number of solutions " << options[Option::MAX_NUMBER_OF_SOLUTIONS] << endl;
cout << " \t compute also non-optimal solutions: " << options.getBool(Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS) << endl;
cout << " \t compute also connectivity: " << options.getBool(Option::COMPUTE_ALSO_CONNECTIVITY) << endl;
cout << " \t connectivity cutoff: " << options[Option::CONNECTIVITY_CUTOFF] << endl;
cout << " \t apply first solution: " <<  options.getBool(Option::APPLY_FIRST_SOLUTION) << endl;
cout << " \t size of the greedy priority queue: " << options[Option::GREEDY_K_SIZE] << endl;
cout << " \t branch_and_bound_cutoff: " << options[Option::BRANCH_AND_BOUND_CUTOFF] << endl;
cout << " \t valid : " << valid_ << endl;
cout << endl;
#endif

		// Do we have bonds in the molecule at all?
		if (ac.countBonds() == 0)
			return Processor::CONTINUE;
		
		// Is the processor in a valid state?
		if (readOptions_() && valid_)
		{
			// Speed up the code by temporarily storing the options locally 
			bool overwrite_single_bonds = options.getBool(Option::OVERWRITE_SINGLE_BOND_ORDERS);
			bool overwrite_double_bonds = options.getBool(Option::OVERWRITE_DOUBLE_BOND_ORDERS);
			bool overwrite_triple_bonds = options.getBool(Option::OVERWRITE_TRIPLE_BOND_ORDERS);
			bool overwrite_selected_bonds = options.getBool(Option::OVERWRITE_SELECTED_BONDS);

			// check the options

			// What kind of composite do we have?
			// Do we have a molecule? (Nothingelse is allowed)
			if (RTTI::isKindOf<Molecule>(ac))
			{
				// Store the AtomContainer
				ac_ = &ac;

				// Store the original bond orders as a "special" solution
				storeOriginalConfiguration_();

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

#if defined DEBUG || DEBUG_READ
cout << "preassignPenaltyClasses_:" << preassignPenaltyClasses_() << " precomputeBondLengthPenalties_:" << precomputeBondLengthPenalties_() << endl;
#endif

				// Generate penalty values for all atoms in the AtomContainer ac
				if (preassignPenaltyClasses_() && precomputeBondLengthPenalties_())
				{
					// set the static variables in the PQ entries for faster access
					PQ_Entry_::alpha = alpha_;
					PQ_Entry_::atom_type_normalization_factor = atom_type_normalization_factor_;
					PQ_Entry_::bond_length_normalization_factor = bond_length_normalization_factor_;
					PQ_Entry_::use_fine_penalty = use_fine_penalty_;

					bool found_a_sol = false;

#if defined DEBUG_TIMER	
					cout << "TIMER: Rule-Assignment: " << timer_.getClockTime() << endl;
					timer_.reset();
#endif					
					if (options.get(Option::ALGORITHM) == Algorithm::A_STAR) 	
					{
						// Initialize a priority queue and try to find a first solution
						// Further solutions will be computed calling the method computeNextSolution

						// Initialize the priority queue
						PQ_Entry_ entry;
						entry.bond_orders.resize(total_num_of_bonds_ + num_of_virtual_bonds_,-1);
						entry.last_bond = 0;

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
							// This should not happen in the beginning, but who knows?
							if (entry.last_bond >= total_num_of_bonds_ && add_missing_hydrogens_)
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

						// Try to find a first solution
						found_a_sol = performAStarStep_();
					}
					else if (options.get(Option::ALGORITHM) == Algorithm::K_GREEDY)
					{
						int greedy_k = options.getInteger(Option::GREEDY_K_SIZE);
						// Create a dummy entry for initialization of the greedy set
						PQ_Entry_ entry;
						entry.bond_orders.resize(total_num_of_bonds_ + num_of_virtual_bonds_, -1);
						entry.last_bond = 0;
						
						// Perfrom the greedy search ...
						vector<PQ_Entry_> greedy_set = performGreedy_(entry, greedy_k);
						// ... and create the solutions.
						for (Position i=0; i<greedy_set.size();i++)//std::min(greedy_k, expanded_sols); i++)
						{
							PQ_Entry_& entry = greedy_set[i];
							Solution_ sol(entry, this, greedy_node_expansions_, greedy_k);
							solutions_.push_back(sol);
							found_a_sol = true;
						}
					}	
					else if (options.get(Option::ALGORITHM) == Algorithm::BRANCH_AND_BOUND)
					{
						// Initialize the greedy search
						int greedy_k = options.getInteger(Option::GREEDY_K_SIZE);
						// Create a dummy entry for initialization of the greedy set
						PQ_Entry_ entry;
						entry.bond_orders.resize(total_num_of_bonds_ + num_of_virtual_bonds_, -1);
						entry.last_bond = 0;
						
						// Perform the greedy search
						vector<PQ_Entry_> greedy_set = performGreedy_(entry, greedy_k);
						
						greedy_atom_type_penalty_ = 0;
						greedy_bond_length_penalty_ = 0;
						
						if (greedy_set.size() > 0)
						{
							// take the top solution as cutoff
							greedy_atom_type_penalty_   = greedy_set[0].estimated_atom_type_penalty;
							greedy_bond_length_penalty_ = greedy_set[0].estimated_bond_length_penalty; 
						}

						// clear the queue
						queue_ = priority_queue<PQ_Entry_>();
						
						// initialize the step counter with the 
						// number steps needed by k_greedy
						step_ = greedy_node_expansions_; 

						// Initialize the priority queue
						entry = PQ_Entry_();
						entry.bond_orders.resize(total_num_of_bonds_ + num_of_virtual_bonds_,-1);
						entry.last_bond = 0;

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
							// This should not happen in the beginning, but who knows?
							if (entry.last_bond >= total_num_of_bonds_ && add_missing_hydrogens_)
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

						// find solutions via branch and bound
						found_a_sol = performBranchAndBound_();
					}
					else if (options.get(Option::ALGORITHM) == Algorithm::ILP)// Solve a ILP
					{
#ifdef BALL_HAS_LPSOLVE
						// Get a first solution
						found_a_sol = createILP_();
						found_a_sol &= solveILP_();
#else
						Log.error() << "Error: BALL was configured without lpsolve support! Try A_STAR instead!" << 
												__FILE__ << " " << __LINE__<< std::endl;
#endif
					} // end of ILP
					else
					{
						Log.error() << __FILE__ << " " << __LINE__ << ": no valid algorithm specified." << endl;
					}

				
					// Do we have a solution? 
					if (!found_a_sol)
					{
						Log.info() << "No valid bond order assignment found!" << endl;
#if defined DEBUG_TIMER					
					timer_.stop();
					cout << "TIMER: Algorithm: " << timer_.getClockTime() << endl; 
#endif	
					}
					else
					{
						// Do we have to find more solutions?
						Size max_n = options.getInteger(Option::MAX_NUMBER_OF_SOLUTIONS);
						bool found_another = true;
						bool last_sol_is_optimal = true;

						while (    found_another  
								    && ((getNumberOfComputedSolutions() < max_n) || (!max_n))
									  && (last_sol_is_optimal || (compute_also_non_optimal_solutions_))
									)
						{	
							found_another = computeNextSolution(options.getBool(Option::APPLY_FIRST_SOLUTION));
							last_sol_is_optimal &= (fabs(getTotalPenalty(0)-getTotalPenalty(solutions_.size()-1)) < 1.e-4);
						}
#if defined DEBUG_TIMER		
					timer_.stop();
					cout << "TIMER: Algorithm: " << timer_.getClockTime() << endl; 
#endif	
					}

					if (solutions_.size() > 0)
					{
						if (options.getBool(Option::APPLY_FIRST_SOLUTION))
						{
							apply(0);
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

	bool AssignBondOrderProcessor::estimatePenalty_(PQ_Entry_& entry, bool include_heuristic_term)
	{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "AssignBondOrderProcessor::called estimatePenalty_()"<< endl;
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

		// Evaluate all atom type and bond length penalties
		for (; a_it != ac_->endAtom() && valid ; a_it++, current_atom_index++)
		{
#if defined DEBUG || defined DEBUG_ESTIMATE
		cout << a_it->getFullName()<< " : " << endl;
#endif
			vector<Bond*> free_bonds;	       	// store the free bonds for bond length penalty 
																			 	//   calculation (without VIRTUAL__BOND's!)
			valence = 0; 	                    // sum of bond orders of all fixed bonds 
																				//   inclusiv VIRTUAL BOND's ! 
			current_bond_length_penalty = 0;  // sum of all bond length penalties of all fixed bonds 
																				//   exclusiv VIRTUAL BOND's !
			closed = true;
			num_free_bonds = 0;

			// Iterate over all bonds of this atom
			//   first its original bonds
			for (b_it = a_it->beginBond(); +b_it; b_it++) 
			{			
				Bond* current_bond = &*b_it;
				// Do we have a valid bond? 
				if (bond_to_index_.find(current_bond) != bond_to_index_.end())
				{
					// Is this bond already fixed?
					if (    (bond_to_index_[current_bond] <= (Index)entry.last_bond)
							 && (entry.bond_orders[bond_to_index_[current_bond]] > 0)   )
					{
						int order = entry.bond_orders[bond_to_index_[current_bond]];
						valence += order;

						// Store the bond length penalty
						// We just consider non-virtual bonds here!
						if (bond_lengths_penalties_.find(current_bond) != bond_lengths_penalties_.end())
						{
							current_bond_length_penalty += bond_lengths_penalties_[&*b_it][order]; 
						}
						else
						{
							Log.error() << "Error " << __FILE__ << " " << __LINE__ << endl;
						}

#if defined DEBUG || defined DEBUG_ESTIMATE
						cout << "   bond " << bond_to_index_[&*b_it] << " (" << b_it->getFirstAtom()->getFullName() << "-" << b_it->getSecondAtom()->getFullName() << ") fixed"
							<< " by order " << entry.bond_orders[bond_to_index_[&*b_it]]  
							<< " : bond len penalty: " << bond_lengths_penalties_[&*b_it][order] 
							<< " -+-> cur bond len pen:"<< current_bond_length_penalty << endl;
#endif

					}
					else // a free bond 
					{
						closed = false;
						num_free_bonds++;
						free_bonds.push_back(&*b_it); 	// store the bond length penalty
					}
				} 
				else  // no valid bond
				{
					Log.error() << "Error: invalid bond!" << __FILE__ << " " << __LINE__ << endl;
				}
			}

			// Iterator over all its virtual hydrogens!
			int virtual_order = 0;
			if (     add_missing_hydrogens_
					 && (atom_to_virtual_bond_index_.find(&*a_it) != atom_to_virtual_bond_index_.end()))
			{
				virtual_order = std::max((short)0,entry.bond_orders[atom_to_virtual_bond_index_[&*a_it] + total_num_of_bonds_]);
				valence += virtual_order;
			}

			// Decide, whether there is a free virtual bond left => atom not closed yet!
			if (   add_missing_hydrogens_ 
					&& (entry.last_bond != (total_num_of_bonds_ + num_of_virtual_bonds_ - 1)))
			{
				closed = false;
			}

			// Now for the current atom all orders of already fixed bonds and fixed virtual 
			// hydrogen bonds are summed up in 'valence' and all bond-length-deviation-penalties of 
			// fixed bonds are summed up in current_bond_length_penalty - excluding the virtual hydrogen 
			// bonds .
			//
			// We have to distinguish two cases:
			// 	(a) atom closed and   (b) atom still has bonds with undefined orders
			//
			// Case (a)  * the bond length penalty is already given in current_bond_length_penalty
			// 					 * the atom type penalty can simply be computed by evaluating the penalty-vector 
			// 					 	   at the already computed valence at position "virtal_order many virtual H's"
			// Case (b)  We have to take the penalties resulting from the already set bonds as a minimum
			// 					 and have to add the best possible (minimal) penalty of any valid distribution 
			// 					 of the non-fixed bonds.
			// 					 * so far, the bond length penalty and atom type penalty contribution resulting  
			// 						 from the __fixed__ bonds are stored in
			// 						  -- current_bond_length_penalty and 
			// 						  -- and given by the penalty-vector at the already computed valence
			// 						       at position "virtal_order many virtual H's"
			// 						 Leave out the virtual hydrogens since they have no correct position so far!
			// 					 * for the __non-fixed__ bonds we have to 
			// 					 			compute an estimate of the bond length penalty, the minimal possible 
			// 					  		bond_length_penalties for the remaining bonds --> estimateBondLengthPenalty_()
			// 					   and  estimate the atom penalty by finding the min in all remaining possible 
			// 					      penalties in the possible penalty vectors. --> estimateAtomTypePenalty_()
			// For speed up return false if something exploded :-) 

			// In case something very strange happens:
			// Remember, we start atom counting with 0
			if (current_atom_index >= (int)atom_to_block_.size())
			{
				Log.error() << "Error: Atom  " << a_it->getFullName() << " out of index ! " 
										<<  __FILE__ << " "  << __LINE__ << endl;

				entry.estimated_atom_type_penalty = INFINITE_PENALTY;
				return false; 
			}
			
			// This should not happen either: Far to many virtual hydrogens choosen 
			if (virtual_order > (int)atom_to_block_[current_atom_index].size())
			{
				Log.error() << "Error: Too many virtual bonds  " 
										<<  __FILE__ << " " << __LINE__ << endl;
				entry.estimated_atom_type_penalty = INFINITE_PENALTY;
				return false; 
			}

			// Are all bonds of this atom already fixed?
			if (closed)
			{
				// Get the current penalty vector 
				int block = atom_to_block_[current_atom_index][virtual_order];

				if (block < 0)
				{
					Log.error() << "Error: No penalty vector for atom " << current_atom_index << " " 
											<< virtual_order << "! " << __FILE__ << " " << __LINE__ << std::endl;
										
					entry.estimated_atom_type_penalty = INFINITE_PENALTY;
					return false; 
				}

				int current_start_valence = block_to_start_valence_[block];
				int current_block_length  = block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;
				int current_start_index   = block_to_start_idx_[block];
				
				// Is the valence found already greater than the largest for this atom type?
				if (valence > current_end_valence)
				{

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  Error: valence explosion for atom " << a_it->getFullName() << endl;
#endif
					if (evaluation_mode_)
					{
						Log.info() << "  AssignBondOrderProcessor: Valence explosion for atom " 
												<<   a_it->getFullName()  << " :  val " << valence 
												<< ", free bonds "<<  num_free_bonds <<  ", Ruleid " <<  block +1 
												<< " with length " <<  block_to_length_[block] << endl;
					}

					entry.estimated_atom_type_penalty = INFINITE_PENALTY;
					return false;
				}

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  CLOSED atom " <<   a_it->getFullName() << " with valence " << valence << endl;
#endif

				// is the valence large enough for this atom type?
				if (valence >= current_start_valence)
				{
					// NOTE: case valence too large was handled before

					// add the actual penalty
					estimated_atom_penalty   +=  penalties_[current_start_index + valence  - current_start_valence];
					estimated_bond_penalty   +=  current_bond_length_penalty; 

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " ESTIMATE RESULT:  atom type penalty   +" << penalties_[current_start_index + valence - current_start_valence] << " = " << estimated_atom_penalty << endl
	   << "                   bond length penalty +" << current_bond_length_penalty << " = " << estimated_bond_penalty << endl;
#endif
	
		
				}
				else // valence too small
				{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " ERROR: "<< a_it->getFullName() << " valence too small : " << valence <<   " < " <<  current_start_valence << endl;
#endif
					if (evaluation_mode_)
					{
						Log.info() << "AssignBondOrderProcessor: Valence of " << a_it->getFullName() 
												<< " too small : " << valence   
												<<   " < " <<  current_start_valence 
												<< ". Consider adding hydrogen atoms!" << endl;
					}
					entry.estimated_atom_type_penalty = INFINITE_PENALTY;
					return false;
				}

			}
			else if (include_heuristic_term) // the atom is not yet closed => heuristic kicks in
			{
				// estimate the penalties
				// NOTE: To speed up the code, we already added up the minimal possible bond length deviation 
				//       (which was precomputed in a preprocessing step and stored in the 0-th entry)
				// 			 in the check-if-atom-is-closed-loop, such that we do not have to do this 
				// 			 in the heuristic! 
				
				float current_atom_type_penalty = 0.;
				current_atom_type_penalty = estimateAtomTypePenalty_(&*a_it, current_atom_index, 
			                         valence, virtual_order, num_free_bonds, entry);

				if (current_atom_type_penalty >= 0)
					estimated_atom_penalty += current_atom_type_penalty;			
				else
					return false;

				float current_estimated_bond_length_penalty = estimateBondLengthPenalty_(current_atom_index,
							free_bonds, virtual_order, valence, num_free_bonds);

				if (current_estimated_bond_length_penalty >= 0)
					estimated_bond_penalty += current_bond_length_penalty + current_estimated_bond_length_penalty;
				else
					return false;
			} // end of heuristic
		} // end of for all atoms

		// store the computed atom type penalty
		entry.estimated_atom_type_penalty = estimated_atom_penalty;
		
		//store the computed bond length penalty
		entry.estimated_bond_length_penalty = estimated_bond_penalty;

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << " ESTIMATE RESULT for bond orders ( ";
for (Size i=0; i < entry.bond_orders.size(); i++)
{
	cout << " " << entry.bond_orders[i];
}
cout << " ) : atom type pen = " << entry.estimated_atom_type_penalty << ", bond len pen = "<< entry.estimated_bond_length_penalty << endl;
#endif

		return true;
	}

	// Computes the atom type penalty for a given atom
	// returns -1 if no valid atom type penalty for this atom could be computed!
	float AssignBondOrderProcessor::estimateAtomTypePenalty_(Atom* atom, Index atom_index, 
			                         int fixed_valence, int fixed_virtual_order, int num_free_bonds, 
															 PQ_Entry_& entry) 
	{	
		bool found_a_value = false;
		// We have to estimate the penalty heuristically 
		// by finding the minimum in all further possible penalty vector entries.

		// the minimal penalty of all possible penalty vectors for this atom
		float min_atom_type_penalty = Limits<float>::max();

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
					int i = current_start_valence;
					if (heuristic_index_ == SIMPLE)
					{
				 		// this is the default case
				 		// Position i = current_start_valence;
					}
					else if (heuristic_index_ == MEDIUM)
					{
						// at which position in the current penalty vector are we at the moment?
						i = (fixed_valence + num_free_bonds < current_start_valence) 
							? current_start_valence
							: fixed_valence + num_free_bonds;
					}
					else if (heuristic_index_ == TIGHT)
					{
						// find a lower bound
						// at which position in the current penalty vector are we at the moment?
						// NOTE: all variables are signed ints, since we need -1 to flag invalid conformations
						i = (fixed_valence + num_free_bonds < current_start_valence) 
							? current_start_valence
							: fixed_valence + num_free_bonds;

						// find an upper bound for sum of all bond orders	
						int max_valence = fixed_valence;

						// check all neighboring atoms, whose connecting bonds are free
						Atom::BondIterator b_it = atom->beginBond();
						for (; b_it != atom->endBond(); b_it++)
						{
							// compute the sum of bond orders that are already set
							// 				 the number of free bonds
							// 				 the number of virtual bonds
							Atom* neighbor =  b_it->getPartner(*atom);
							int neighbors_bond_order = 0;
							int neighbors_num_free_bonds = 0;
							int neighbors_virtual_bonds = 0;

							// is this bond free?
							if (    (bond_to_index_[&*b_it] > (Index)entry.last_bond)
									|| (entry.bond_orders[bond_to_index_[&*b_it]] <= 0)   ) // ???
							{

								// check all bonds of the neighbor atom
								Atom::BondIterator neighbor_b_it = neighbor->beginBond();
								for (; neighbor_b_it != neighbor->endBond(); neighbor_b_it++)
								{
									// Do we have a valid bond? 
									if ( bond_to_index_.find(&*neighbor_b_it) != bond_to_index_.end())
									{
										// Is this bond already fixed?
										if (    (bond_to_index_[&*neighbor_b_it] <= (Index)entry.last_bond)
												&& (entry.bond_orders[bond_to_index_[&*neighbor_b_it]] > 0)   )
										{
											neighbors_bond_order += entry.bond_orders[bond_to_index_[&*neighbor_b_it]];
										}
										else //free
										{
											neighbors_num_free_bonds++;
										}
									}
								}
								// count the virtual bonds
								if (    add_missing_hydrogens_
										&& (atom_to_virtual_bond_index_.find(neighbor) != atom_to_virtual_bond_index_.end()))
								{
									neighbors_virtual_bonds = std::max((short)0,entry.bond_orders[atom_to_virtual_bond_index_[neighbor] + total_num_of_bonds_]);
								}
							}
							int n_atom_index = neighbor->getProperty("AssignBondOrderProcessorAtomNo").getUnsignedInt();
							// compute the maximal bond order for the connecting bond (neighbor -- atom)
							int n_block = atom_to_block_[n_atom_index][neighbors_virtual_bonds];
							int n_current_start_valence = block_to_start_valence_[n_block];
							int n_current_block_length  = block_to_length_[n_block];
							int n_current_end_valence   = n_current_start_valence + n_current_block_length-1;

							// add the maximal possible bond order
							max_valence += (n_current_end_valence - (neighbors_num_free_bonds-1) 
									-  neighbors_virtual_bonds 
									-  neighbors_bond_order);

						}// end of all neighbors

						current_end_valence = std::min(current_end_valence, max_valence);
					}
					else 
					{
						Log.error() << __FILE__ << " " << __LINE__ << " : No heuristic defined! Please check the option Option::HEURISTIC."  << endl;
						return min_atom_type_penalty; 
					}
				
					// for every remaining valence of the atom under consideration
					// (we know there is at least one)
					for (; i <= current_end_valence; i++)
					{
						float current_atom_type_penalty = penalties_[current_start_index + i - current_start_valence];

						if (current_atom_type_penalty < min_atom_type_penalty)
						{
							min_atom_type_penalty = current_atom_type_penalty;
							found_a_value = true;
						}
					}
				}
				else
				{
					if (evaluation_mode_)
					{
						Log.info() << "  AssignBondOrderProcessor: Valence explosion for atom " 
												<<   atom->getFullName()  << " :  val " << fixed_valence 
												<< ", free bonds "<<  num_free_bonds <<  ", Ruleid " <<  block +1 
												<< " with length " <<  block_to_length_[block] << endl;
					}
				}
			} // end of valid block
		} // end of all virtual orders
		
		if (!found_a_value)
		{
			min_atom_type_penalty = -1;
		}
		return min_atom_type_penalty; 
	}
	

	// Computes the min possible bond length penalty for all free bonds of this atom
	// returns -1 if no bond length penalty could be found!
	// Trying to distribute all free valences with minimal bond length penalty as possible 
	// over all free bonds of this atom seems to be far to expensive.
	// In addition take care of the fact, that all bonds between two unclosed atoms 
	// are counted twice -> all bonds should be counted twice!
	float AssignBondOrderProcessor::estimateBondLengthPenalty_(Index atom_index, 
			const vector<Bond*>& free_bonds, int fixed_virtual_order,  int fixed_valence, int num_free_bonds )
	{	
		// NOTE: virtual bond are excluded!
		
		// we have to estimate the bond length penalty heuristically 
		// by trying to distribute the remaining free valences over all remaining free bonds
		// such that the summed bond length penalty is minimal

		// the minimal penalty of all possible penalty vectors for this atom
		float min_bond_length_penalty = Limits<float>::max();

		bool found_a_value = false;
	
		// all possible free valences
		// NOTE: virtual hydrogens may use or don't use one free valence
		
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
				
				// do we have an explosion ?
				if (fixed_valence <= current_end_valence)
				{
					float bond_length_penalty_current_set_up = 0;
					
					// iterating over all free bonds 
					// (we know there is at least one free bond)
					for (Size fb=0; fb < free_bonds.size(); fb++) 
					{
						float current_bond_min = Limits<float>::max();	
						// Note: for simplification we do not share the free bond 
						//       orders correctly! We just find the min!
						// the number of the remaining bond orders to be assigned 
						// to the free bonds are :
						std::vector<float>& current_bond_length_penalties = bond_lengths_penalties_[free_bonds[fb]];
						int up_to = std::min((int)current_bond_length_penalties.size()-1, 
																 current_end_valence -(num_free_bonds-1)-fixed_valence); 
						
						// for all possible valences for this bond
						for (int j = 1; j <= up_to; j++)
						{
							float deviation = current_bond_length_penalties[j]; 

							if (deviation < current_bond_min)
							{
								current_bond_min = deviation;
							}
						} // end of for all possible valences for this bond
						bond_length_penalty_current_set_up	+= current_bond_min;
					} // end of for all free bonds
					
					if (bond_length_penalty_current_set_up < min_bond_length_penalty)
					{
						found_a_value = true;
						min_bond_length_penalty = bond_length_penalty_current_set_up;
					}

				} // end of no explosion
			}// end of valid block
		}

		if (!found_a_value)
			return -1;

		return min_bond_length_penalty;
	}



	bool AssignBondOrderProcessor::readAtomPenalties_()	
		throw(Exception::FileNotFound())
	{
		// Open parameter file
		Path    path;
		String  inifilename(path.find(options[Option::INIFile]));
		
		if (inifilename == "") 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, options[Option::INIFile]);
		}

		QString errorStr;
		int errorLine;
		int errorColumn;

		QFile file((inifilename.c_str()));
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			Log.error() << "Error: cannot read file " << inifilename <<" " << __FILE__ << " " << __LINE__ << std::endl;
			Log.error() << "Reason was: " << file.errorString().toAscii().constData() << std::endl;
			return 1;
		}

		// read the document
		QDomDocument domDocument;
		if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
					&errorColumn)) 
		{
			Log.error() << "Parse error in line " << errorLine << " column " << errorColumn <<  " of file " << inifilename << endl;
			Log.error() << "Reason was: " << errorStr.toAscii().constData() << std::endl;
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
				tmp.first = element.firstChild().nodeValue().toAscii().constData(); 

				// read the SMARTS-string
				QDomNodeList smartstring =  entries.item(i).toElement().elementsByTagName("smartstring");
				if (smartstring.length() == 1)
				{
					tmp.second = smartstring.item(0).toElement().firstChild().nodeValue().toAscii().constData();
				} 
				else if (smartstring.length() == 0)
				{
					Log.warn() << "In file " << inifilename << " : no SMARTS-string found for element " << element.firstChild().nodeValue().toAscii().constData() << endl;
				}  
				else
				{
					Log.error() <<  "Parse error in file " << inifilename << " : more than on3 SMARTS-string for element " << element.firstChild().nodeValue().toAscii().constData() << endl;
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
					Log.error() << "In file " << inifilename << " : no penalties found for element " << element.firstChild().nodeValue().toAscii().constData() << endl;
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
cout << " AssignBondOrderProcessor::precomputeBondLengthPenalties_:   " << endl;
#endif

		// this is a heuristic value that worked quite ok in our tests
		float diff_cutoff = 0.5;

		// get the relevant options
		// this is also done in the start-method, 
		// but when performing an evalution the start-method is not called!
		max_bond_order_ = options.getInteger(Option::MAX_BOND_ORDER);
		alpha_ = options.getReal(Option::BOND_LENGTH_WEIGHTING);

		// initialize the inverse bond length penalty normalization factor
		bond_length_normalization_factor_ = 0.;
		
		if (!valid_)
		{	
			Log.error() << "Error: The processors state is invalid. " << __FILE__ << " " << __LINE__ << endl;
			return false;
		}

		// determine for each bond its average bond lengths	
		// and store them in bond_lengths_penalties_
		if (ac_)
		{	
			// read the bond length parameters
			Path path;
			String pfilename(path.find("bond_lengths/bond_lengths_mmff94.ini"));

			Parameters parameter_file(pfilename);

			ParameterSection bond_lengths;
			bond_lengths.extractSection(parameter_file, "BondLengths");

			AtomIterator a_it = ac_->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();
			BALL_FOREACH_BOND(*ac_, a_it, b_it)
			{
				// the precomputed bond length penalties
				vector<float> penalties(max_bond_order_ + 1, 0.);
				// the bond atoms 
				String atom_type_1 = b_it->getFirstAtom()->getElement().getSymbol();
				String atom_con_1(b_it->getFirstAtom()->countBonds());

				String atom_type_2 = b_it->getSecondAtom()->getElement().getSymbol();
				String atom_con_2(b_it->getSecondAtom()->countBonds());
				
				String key = atom_type_1+" "+atom_con_1+" "+atom_type_2+" "+atom_con_2+" ";

				// the current max and min bond length deviation
				// bonds without predefined lenght penalty will get penalty zero!
				// This is no problem for the heuristic, since this bond will
				// get zero in all PQEntries.
				float max_bond_length_deviation = 0.; 
				float min_bond_length_deviation = Limits<float>::max();

				bool complete = true;
				max_bond_length_deviation = 0.;
					
				// the current bond length
				float bond_length = b_it->getLength();
				Size min_order = 0;

#ifdef  DEBUG_BOND_LENGTH
cout << b_it->getSecondAtom()->getFullName() << "-" << b_it->getOrder() <<"-" <<  b_it->getFirstAtom()->getFullName() << endl;
#endif
				// for all possible bond orders, precompute the penalties
				// here we try square deviation
				// NOTE: we omit the aromatic bonds!
				for (Position i = 1; i <= (Position)max_bond_order_; i++)
				{
					String current_key = key+String(i);

					if (bond_lengths.has(current_key, "r0"))
					{
						const String& bond_length_string = bond_lengths.getValue(current_key, "r0");
						penalties[i] = pow((bond_length - bond_length_string.toFloat()),(int)2);

#ifdef  DEBUG_BOND_LENGTH
cout << "        order " << i << " : " << 	penalties[i] << endl;
#endif
						if (max_bond_length_deviation < penalties[i])
						{
							max_bond_length_deviation = penalties[i];
						}
						if (min_bond_length_deviation > penalties[i])
						{
							min_bond_length_deviation = penalties[i];
							min_order = i;
						}
					}
					else
					{
						complete = false;
						penalties[i] = -1.;
					}
				}

				// store the min at position zero
				penalties[0] = min_bond_length_deviation;

				// in case we got no complete set, we assume 
				// the missing bond orders are really unlikely
				// and we set a penalty to 2*max_deviation_found (for this bond)
				if (!complete)
				{
#ifdef  DEBUG_BOND_LENGTH
	cout << "   NOT COMPLETE" << endl;
#endif
					max_bond_length_deviation *= 2.;

					for (Position i = 1; i <= (Position)max_bond_order_; i++)
					{
						if (penalties[i] < 0)
						{
							penalties[i] = max_bond_length_deviation;
						}
					}
					
					// filter our irrelevant penalty differences 
					// like aromatic bonds stuck between order 1 and 2
					for (Position i = 1; i < (Position)max_bond_order_; i++)
					{
						if (fabs(penalties[i]-penalties[i+1]) < diff_cutoff * (penalties[i]+penalties[i+1]))
						{
#ifdef	DEBUG_BOND_LENGTH
							Log.info() << "			penalty for order " << i+1 << " changed to that of order " << i << endl;
#endif
							penalties[i+1] = penalties[i];
						}
					}
				}

				//store the b_it's penalties in the hashmap
				//else: since we have no information, we handle 
				//every bond order the same, namely assign 0. :-)	
				bond_lengths_penalties_[&(*b_it)] = penalties;

				// add the bonds max deviation to the inverse bond length normalization factor
				bond_length_normalization_factor_ += max_bond_length_deviation;
			} // end of for all bonds
		}
		else
		{
			valid_ = false;
			return false;
		}

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
cout << "getPenaltyClass_ HIT   " << atom->getFullName() << " with index " << atom->getIndex() << " assigned to block: " << j+1 << " : "  << block_definition_[j].first << "  "	<<  block_definition_[j].second << endl;
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
cout << " AssignBondOrderProcessor::preassignPenaltyClasses_()" << endl;
#endif

		if (!valid_)
		{	
			Log.error() << "AssignBondOrderProcessor: The processors state is invalid." << endl;
			return false;
		}

		// Determine for each atom the corresponding valence- and penalty-block	
		if (ac_)
		{
			// get the number of atoms
			Position no_atoms = ac_->countAtoms();
			
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
				Atom* at = ac_->getAtom(i);

				// for reasons only known to BALL, getIndex yields completely strange results for
				// the atoms here, so we need to add a Property which we can later read out
				ac_->getAtom(i)->setProperty("AssignBondOrderProcessorAtomNo", i);
				bool found = false; 
				
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
cout << "preassignPenaltyClasses_() HIT : " << at->getFullName() << " with index " << at->getIndex() << " assigned to block  "<< j+1 << " : " << block_definition_[j].first << "   "
		<<  block_definition_[j].second << endl;
#endif	
#ifdef DEBUG_PRINT_RULES
cout << at->getFullName() << " " << at->getPosition() << "  assigned to block  " << j+1 << " : " << block_definition_[j].first << "   " <<  block_definition_[j].second << " --- penalties: (" ;
int current_start_index   = block_to_start_idx_[j];
int current_start_valence = block_to_start_valence_[j];

if (current_start_valence !=0)
{
	cout << "-";
}

for(Size k=1; k < current_start_valence; k++)
{
	cout << ", - ";
}
for(Size k = 0; k < block_to_length_[j]; k++)
{
	cout << ", " << penalties_[k + block_to_start_idx_[j]];
}
cout << ") " <<  endl;
#endif

#ifdef DEBUG_RULES
	int valence = 0;
	Atom::BondIterator b_it;	
	for (b_it=at->beginBond(); b_it != at->endBond(); b_it++)
	{
		valence+=b_it->getOrder();
	}

	int current_start_index   = block_to_start_idx_[j];
	int current_start_valence = block_to_start_valence_[j];

	if (penalties_[current_start_index + valence  - current_start_valence] > 0)
	{ 	
		cout <<   at->getFullName()  << " :  Rule " << j+1 << "  penalty: " << 	penalties_[current_start_index + valence  - current_start_valence] << endl;
	}
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
cout << " ~~~~~~~~ added hydrogen dump ~~~~~~~~~~~~~~~ " << endl;
for (Size i=0; i < 	atom_to_block_.size() ; i++)
{	
	Atom* at = ac_->getAtom(i);
	cout << at-> getFullName()<< " : ";
	for (Size j=0; j < atom_to_block_[i].size(); j++)
	{
		cout << atom_to_block_[i][j] << " "; 
	}
	cout << endl;
}
cout << " ~~~~~~~~ added hydrogen dump ~~~~~~~~~~~~~~~~" << endl;
#endif

		}
		else
		{
			Log.error() << "Error: No valid AtomContainer. " << __FILE__ << " " << __LINE__ << endl;
			return false;
		}
		return true;
	}


	// NOTE: add the moment we assume, that this method is called 
	//       AFTER preassignPenaltyClasses() and precomputeBondLengthPenalties()
	float AssignBondOrderProcessor::computeVirtualHydrogens_(Atom* atom)
	{
	#if defined DEBUG_ADD_HYDROGENS || defined DEBUG
		cout << " AssignBondOrderProcessor::computeVirtualHydrogens_" << endl;
	#endif
		
		float max_penalty = 0;

		// iterate over all atoms and add a hydrogen atom and bond 
		// 	if suggested by the free valences. 
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

			for (int i = 1; i <= free_valences; i++)
			{
				// add a hydrogen atom  
				Atom* hydrogen = new Atom;
				hydrogen->setElement(PTE[Element::H]);	
				hydrogen->setName((PTE[Element::H].getSymbol())+String(num_of_atoms));
				num_of_atoms++;
				hydrogen->setProperty("VIRTUAL__ATOM", true);
				if (atom->getFragment())
				{
					atom->getFragment()->insert(*hydrogen);
				}
				else
				{
					atom->getMolecule()->insert(*hydrogen);
				}
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
				delete(atoms_to_delete[i]);
			}
		}

		return max_penalty;
	}

	void AssignBondOrderProcessor::storeOriginalConfiguration_()
	{
		starting_configuration_.valid = true;

		AtomIterator a_it = ac_->beginAtom();
		Atom::BondIterator b_it;
		BALL_FOREACH_BOND(*ac_, a_it, b_it)
		{
			starting_configuration_.bond_orders[&*b_it] = b_it->getOrder();
		}
	}

	void AssignBondOrderProcessor::resetBondOrders()
	{
		apply_(starting_configuration_);
		last_applied_solution_ = -1;
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

		options.setDefaultBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_CONNECTIVITY,
													 AssignBondOrderProcessor::Default::COMPUTE_ALSO_CONNECTIVITY);
		
		options.setDefaultReal(AssignBondOrderProcessor::Option::CONNECTIVITY_CUTOFF,
													 AssignBondOrderProcessor::Default::CONNECTIVITY_CUTOFF);

		options.setDefaultBool(AssignBondOrderProcessor::Option::USE_FINE_PENALTY,
												   AssignBondOrderProcessor::Default::USE_FINE_PENALTY);	

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
		
		options.setDefault(AssignBondOrderProcessor::Option::HEURISTIC,
													 AssignBondOrderProcessor::Default::HEURISTIC);		

		options.setDefaultReal(AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING,
													 AssignBondOrderProcessor::Default::BOND_LENGTH_WEIGHTING);	

		options.setDefaultBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION,
 														AssignBondOrderProcessor::Default::APPLY_FIRST_SOLUTION);
		
		options.setDefaultInteger(AssignBondOrderProcessor::Option::GREEDY_K_SIZE,
													 AssignBondOrderProcessor::Default::GREEDY_K_SIZE);
		
		options.setDefaultReal(AssignBondOrderProcessor::Option::BRANCH_AND_BOUND_CUTOFF,
													 AssignBondOrderProcessor::Default::BRANCH_AND_BOUND_CUTOFF);	

	}

	bool AssignBondOrderProcessor::apply_(Solution_& solution)
	{
		if (solution.valid)
		{
			// we assume, that the AtomContainer is valid and the correct one! 

			// delete all former VIRTUAL BONDs
			// did we already apply a solution?
			if (last_applied_solution_>=0)
			{	
				for (Size j=0; j < solutions_[last_applied_solution_].atoms_to_delete.size(); j++)
				{
					//NOTE: all adjacent bonds of these atoms will be deleted automatically
					delete(solutions_[last_applied_solution_].atoms_to_delete[j]); 
				}
			}

			// set all the original bonds to the assigned order
			AtomIterator a_it = ac_->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();

			HashMap<Bond*, int> tmp_bond_orders = solution.bond_orders;

			BALL_FOREACH_BOND(*ac_, a_it, b_it)
			{
				HashMap<Bond*, int>::Iterator it = tmp_bond_orders.find(&*b_it);
				if (it != solution.bond_orders.end())
				{
					b_it->setOrder(it->second);
				}
			}

			// for each virtual bond add the corresponding number of hydrogens
			// NOTE: the positions of the hydrogens are kind of randomly!
			solution.atoms_to_delete.clear();

			HashMap<Atom*, int> tmp_virtual_hydrogens = solution.number_of_virtual_hydrogens;

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
				if (it != tmp_virtual_hydrogens.end())
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
						if (a_it->getFragment())
						{
							a_it->getFragment()->insert(*hydrogen);
						}
						else
						{
							a_it->getMolecule()->insert(*hydrogen);
						}
						solution.atoms_to_delete.push_back(hydrogen);

						// TODO: set the hydrogen's positions correctly!
						hydrogen->setPosition(a_it->getPosition() + pos[m] );

						// and a bond
						Bond* new_bond = a_it->createBond(*hydrogen);
						new_bond->setProperty("VIRTUAL__BOND", true);
						new_bond->setOrder(1);
					}
				}
			}
		}

		return solution.valid;
	}

	bool AssignBondOrderProcessor::apply(Position i)
	{
		bool result = false;
		if (i < solutions_.size())
		{
			result = apply_(solutions_[i]);

			if (result)
			{
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
				else
				{
					// to be on the safe side, we have to mark all bonds we may have changed as non-aromatic
					for (Position i=0; i<free_bonds_.size(); ++i)
						free_bonds_[i]->clearProperty(Bond::IS_AROMATIC);
				}
			}
		}

		return result;
	}


	const System& AssignBondOrderProcessor::getSolution(Position i)
		throw(Exception::IndexOverflow)
	{
		if (i >= solutions_.size())
		{
			//Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;
			Exception::IndexOverflow e(__FILE__, __LINE__, i, solutions_.size());

			throw(e);
		}
		else
		{	
			// apply the i-th solution
			apply(i);

			// What kind of composite do we have?
			if (RTTI::isKindOf<System>(*ac_))
			{
				return *(RTTI::castTo<System>(*ac_));
			}
			else if (RTTI::isKindOf<Molecule>(*ac_))
			{
				Molecule* m = RTTI::castTo<Molecule>(*ac_);
				return *(m->getSystem());
			}
			else
			{ 
				Exception::GeneralException e(__FILE__, __LINE__);
				throw(e);
			}
		}
	}


	bool AssignBondOrderProcessor::computeNextSolution(bool apply_solution)
	{
		bool found_a_sol = false;

		if (add_missing_hydrogens_)
		{
			// the method apply() may have been called before
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

		if (options.get(Option::ALGORITHM) == Algorithm::A_STAR )
		{
			found_a_sol = performAStarStep_();
		}
		else if (options.get(Option::ALGORITHM) == Algorithm::K_GREEDY)
		{
			// k-greedy does not support computing next solutions!
			Log.warn() << " K-greedy does not support computing next solutions!" << endl;
			return false;
		}	
		else if (options.get(Option::ALGORITHM) == Algorithm::BRANCH_AND_BOUND)
		{
			// branch and bound does not support computing next solutions!
			Log.warn() << " Branch&Bound does not support computing next solutions!" << endl;
			found_a_sol = performBranchAndBound_();
		}
		else if (options.get(Option::ALGORITHM) == Algorithm::ILP) // ILP
		{
#ifdef BALL_HAS_LPSOLVE
			// NOTE: lpsolve5.5 despite claiming does not support this!
			// Thus, we include this feature by incorporating constraints which forbid old solutions
			
			// Get old variables
			REAL *vars;
			get_ptr_variables(ilp_, &vars);
			
			set_add_rowmode(ilp_, TRUE);
			
			// Indices of the variables (constraints)
			std::vector<int> cons_indices(ilp_number_of_free_bonds_+1); // colno
			// Prefactors (constraints)
			std::vector<REAL> cons_prefactors(ilp_number_of_free_bonds_+1,1.); // row
			
			Position j = 0;
			for(Position i = 0; i < ilp_number_of_free_bonds_; ++i)
			{
				if (fabs(vars[i]-1.) < 1.e-5)
				{
					cons_indices[j] = i+1;
					++j;
				}
			}
			
			if (!add_constraintex(ilp_, j, &cons_prefactors[0], &cons_indices[0], LE, (REAL)j-1))
			{
				Log.error() << "AssignBondOrderProcessor: Adding constraint for next solution failed" << endl;
				return false;
			}
			
			set_add_rowmode(ilp_, FALSE);
			
			// Reset the basis to reinitialize the solve process
			reset_basis(ilp_);
			
			// Resolve ILP with the new constraint
			found_a_sol = solveILP_();
			
#else
			Log.error() << "AssignBondOrderProcessor: BALL was configured without lp_solve support! Try using A_STAR instead!" << std::endl;
#endif
		}
		else
		{
			Log.info() << __FILE__ << " " << __LINE__ << ": no valid algorithm specified." << endl;
		}

		if (found_a_sol)
		{
			if (apply_solution)
				apply(solutions_.size()-1);
			
			return true;
		}

		return false;
	}

#ifdef BALL_HAS_LPSOLVE
	bool AssignBondOrderProcessor::createILP_()
	{
		ilp_const_penalty_ = 0.f;
		
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
			int valence = 0;
			for (Atom::BondIterator bit = at1->beginBond(); +bit; ++bit)
			{
				valence += bit->getOrder();
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

			if (add_missing_hydrogens_)
			{
				Size total_length = 0;
				for(Position j = 0; j < atom_to_block_[current_atom].size(); ++j)
				{
					Size current_length = block_to_length_[atom_to_block_[current_atom][j]];
					no_y += current_length;
					total_length += current_length;
				}
				max_no_ent = std::max(max_no_ent, total_length);
			}
			else if (consider_bonds)
			{
				Size current_length = block_to_length_[atom_to_block_[current_atom][0]];
				
				no_y += current_length;
				max_no_ent = std::max(max_no_ent, current_length);
			}
			else
			{
				int pos = valence - block_to_start_valence_[atom_to_block_[current_atom][0]];
				if (pos >= 0)
				{
					ilp_const_penalty_ += penalties_[block_to_start_idx_[atom_to_block_[current_atom][0]] + pos];
				}
			}
			
			max_no_atom_bonds = std::max(consider_bonds, max_no_atom_bonds);

		} // end of for all atoms
		
		// Decision variables for every bond order
		ilp_number_of_free_bonds_ *= max_bond_order_;
		
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
		/*for(Position i = 1; i <= ilp_number_of_free_bonds_; ++i)
		{
			set_int(ilp_, i, TRUE);
		}*/
		// Decision variables for atom valences are binary 
		//for(Position i = ilp_number_of_free_bonds_+1; i <= no_vars; ++i)
		for(Position i = 1; i <= no_vars; ++i)
		{
			set_binary(ilp_, i, TRUE);
		}

		// Create space large enough for one row and the objective function,
		// use lp_solves internal types.

		// Indices of the variables (constraints)
		std::vector<int> cons_indices(max_no_atom_bonds*max_bond_order_ + 1 + max_no_ent,0); // colno
		// Prefactors (constraints)
		std::vector<REAL> cons_prefactors(max_no_atom_bonds*max_bond_order_ + 1 + max_no_ent, 0.); // row
		// Indices of the variables (objective function)
		std::vector<int> obj_indices(no_y,0); // obj_colno
		// Prefactors (objective function)
		std::vector<REAL> obj_prefactors(no_y, 0.); // obj_row

		// Factors for choice constraints
		std::vector<REAL> choices(max_no_ent, 1.);

		// Makes building the model faster if it's done
		// row by row
		set_add_rowmode(ilp_, TRUE);

		// Set the choice constraints for bonds
		for(Position i = 1; i <= ilp_number_of_free_bonds_;)
		{
			for (int j = 0; j < max_bond_order_; ++j, ++i)
			{
				cons_indices[j] = i;
			}
			if (!add_constraintex(ilp_, max_bond_order_, &choices[0], &cons_indices[0], EQ, 1))
			{
				Log.error() << "AssignBondOrderProcessor: Setting bond choice constraint for ILP failed" << endl;
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
					for(int k = 1; k <= max_bond_order_; ++k, ++count_b)
					{
						cons_indices[count_b] = it->second*max_bond_order_ + k;
						cons_prefactors[count_b] = -k;
					}
				}
			}

			if (add_missing_hydrogens_)
			{
				// Create indices for the choice variables and set
				// the entries for the valence constraint and the
				// objective function

				// Count the number of variables participating in the current cons_prefactors
				Position count_vars = count_b;
				Position total_length = 0;
				for(Position h = 0; h < atom_to_block_[i].size(); ++h)
				{
					total_length += block_to_length_[atom_to_block_[i][h]];
					for(Position j = 0; j < block_to_length_[atom_to_block_[i][h]]; ++j, ++ind_y, ++count_vars)
					{
						// Choice variables have indices > ilp_number_of_free_bonds_ in variable vector for ILP
						obj_indices[ind_y]  = ind_y + ilp_number_of_free_bonds_ + 1;
						cons_indices[count_vars] = obj_indices[ind_y];
						
						// Set valences
						cons_prefactors[count_vars] = std::max(((REAL)block_to_start_valence_[atom_to_block_[i][h]]) + j - h, 0.);
						
						// Set penalties 
						obj_prefactors[ind_y] = (penalties_[block_to_start_idx_[atom_to_block_[i][h]] + j]);
					}
				}
				
				// in case we got an empty penalty cons_prefactors 
				if (total_length != 0)
				{
					// Add valence constraint
					if (!add_constraintex(ilp_, count_vars, &cons_prefactors[0], &cons_indices[0], EQ,
							 fixed_val_[i]))
					{
						Log.error() << "AssignBondOrderProcessor: Setting valence constraint for ILP failed" << endl;
						return false;
					}
					// Add choice constraint 
					if (!add_constraintex(ilp_, total_length, &choices[0], &cons_indices[count_b], EQ, 1))
					{
						Log.error() << "AssignBondOrderProcessor: Setting valence choice constraint for ILP failed" << endl;
						return false;
					}
				}
				
			}
			else if (count_b)
			{
				// This atom has bonds whose orders should be calculated

				// Create indices for the choice variables and set
				// the entries for the valence constraint and the
				// objective function

				// Count the number of variables participating in the current cons_prefactors
				Position count_vars = count_b; 
				for(Position j = 0; j < block_to_length_[atom_to_block_[i][0]]; ++j, ++ind_y, ++count_vars)
				{
					// Choice variables have indices > ilp_number_of_free_bonds_ in variable vector for ILP
					obj_indices[ind_y]  = ind_y + ilp_number_of_free_bonds_ + 1;
					cons_indices[count_vars] = obj_indices[ind_y];

					// Set valences  
					cons_prefactors[count_vars] = block_to_start_valence_[atom_to_block_[i][0]] + j;

					// Set penalties 
					obj_prefactors[ind_y]       = (penalties_[block_to_start_idx_[atom_to_block_[i][0]] + j]);
				}

				// in case we got an empty penalty cons_prefactors 
				if (block_to_length_[atom_to_block_[i][0]] != 0)
				{
					// Add valence constraint
					if (!add_constraintex(ilp_, count_vars, &cons_prefactors[0], &cons_indices[0], EQ,
								fixed_val_[i]))
					{
						Log.error() << "AssignBondOrderProcessor: Setting valence constraint for ILP failed" << endl;
						return false;
					}
					// Add choice constraint 
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
		if (!set_obj_fnex(ilp_, obj_prefactors.size(), &obj_prefactors[0], &obj_indices[0]))
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
	
	
	bool AssignBondOrderProcessor::solveILP_()
	{
		Solution_ solution;

		// Let lp_solve solve our problem
		int ret = solve(ilp_);
		
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
			
			solution.atom_type_penalty = get_objective(ilp_) + ilp_const_penalty_;
			
			// Do the assignment of the bond orders
			for(Position i = 0; i < ilp_number_of_free_bonds_; )
			{
				for(int j = 1; j <= max_bond_order_; ++j, ++i)
				{
					if (fabs(vars[i] - 1.) < 1.e-10)
					{
						switch (j)
						{
							case 1:
							{
								solution.bond_orders[ilp_index_to_free_bond_[i/max_bond_order_]] = Bond::ORDER__SINGLE;
								break;
							}
							case 2:
							{
								solution.bond_orders[ilp_index_to_free_bond_[i/max_bond_order_]] = Bond::ORDER__DOUBLE;
								break;
							}
							case 3:
							{
								solution.bond_orders[ilp_index_to_free_bond_[i/max_bond_order_]] = Bond::ORDER__TRIPLE;
								break;
							}
							default:
							{
								solution.bond_orders[ilp_index_to_free_bond_[i/max_bond_order_]] = Bond::ORDER__QUADRUPLE;
							}
						}
					}
				}
			}
			
			
			if (add_missing_hydrogens_)
			{
				// Store the atoms which need additional hydrogens
				
				Position no_atoms = ac_->countAtoms();
				Position current_var = ilp_number_of_free_bonds_;
				for (Position current_atom = 0; current_atom < no_atoms; ++current_atom)
				{
					current_var += block_to_length_[atom_to_block_[current_atom][0]];
					
					// Note: If there are no hydrogens to be added there is no need to check the variables
					for(Position j = 1; j < atom_to_block_[current_atom].size(); ++j)
					{
						Size current_length = block_to_length_[atom_to_block_[current_atom][j]];
						
						for(Position k = 0; k < current_length; ++k, ++current_var)
						{
							if (fabs(vars[current_var] - 1.) < 1.e-10)
							{
								solution.number_of_virtual_hydrogens[ac_->getAtom(current_atom)] = j;
							}
						}
					}
				}
			}
		}
		else
		{
			Log.error() << "AssignBondOrderProcessor: ILP could not be solved successfully, reason: " << ret << endl;
			solution.valid = false;
			return false;
		}
		solutions_.push_back(solution);
		return true;
	}
#endif

	//--------------------------------------------------
	//
	//                the nested classes
	//
	//---------------------------------------------------
	
	AssignBondOrderProcessor::Solution_::Solution_()
		: valid(false),
			bond_orders(),
			number_of_virtual_hydrogens(),
			atoms_to_delete(),
			atom_type_penalty(0.),
			bond_length_penalty(0.),
			total_charge(0.),
			node_expansions(0),
			queue_size(0),
			parent(NULL)
	{
	}

	AssignBondOrderProcessor::Solution_::Solution_(PQ_Entry_& entry, AssignBondOrderProcessor* abop,
																									int number_of_node_expansions, int search_queue_size)
	{	
		valid = true;
		Size total_num_of_bonds = abop->total_num_of_bonds_;
		Size num_of_virtual_bonds = abop->num_of_virtual_bonds_;
		parent = abop;
	  if (entry.bond_orders.size() != (total_num_of_bonds + num_of_virtual_bonds))
	  {
		  Log.error() << "Error: " << __FILE__ << " " << __LINE__  << endl;
			valid = false;
	  }
		
		atom_type_penalty   = entry.estimated_atom_type_penalty;
		bond_length_penalty = entry.estimated_bond_length_penalty;

	 	// convert the entry's bond order vector into a HashMap
	 	//    first the original bonds
		for (Size i=0; i < total_num_of_bonds; i++)
		{	
			bond_orders[abop->index_to_bond_[i]] =  entry.bond_orders[i];
		} 
		//   then the virtual bonds
		for (Size i = 0; i < num_of_virtual_bonds; i++)
		{	
			int num_H = entry.bond_orders[total_num_of_bonds + i];
			if (num_H > 0)
			{
				Atom* atom = parent->virtual_bond_index_to_atom_[i];
				number_of_virtual_hydrogens[atom] = num_H;
			}
		}

		// Nothing has to be done, since this datastructure is handled by the
		// methods apply_() and computeNextSolution()	
		//atoms_to_delete;
		
		// compute the total charge
		// TODO: the favoured Nobel gas configuration should be stored in class PTE
		// NOTE: this is just a workaround as long no core-developer decision has been made!
		float charge = 0;
		for (AtomIterator a_it = parent->ac_->beginAtom(); a_it != parent->ac_->endAtom(); a_it++)
		{
			int valence = 0;
			int atom_charge = 0;				
			for (Atom::BondIterator b_it = a_it->beginBond(); b_it != a_it->endBond(); b_it++)
			{
				valence += bond_orders[&*b_it];	
			}
			
			if (number_of_virtual_hydrogens.find(&*a_it) != number_of_virtual_hydrogens.end())
				valence += number_of_virtual_hydrogens[&*a_it];

			// NOTE: This is just an heuristic! 
			if (  a_it->getElement() == PTE[Element::H])
					//|(a_it->getElement() == PTE[Element::He]))
			{	
				atom_charge -= a_it->getElement().getGroup() - valence;
			}
			else if (  (a_it->getElement().getGroup() == 1)
					    ||(a_it->getElement().getGroup() == 2) )
			{
				atom_charge += a_it->getElement().getGroup() - valence;
			}
			else if ((a_it->getElement().getAtomicNumber() == 5))
			{
				//TODO ask Dirk
			}
			else if (a_it->getElement().getAtomicNumber() <= 9)
			{
				atom_charge -= 18 - a_it->getElement().getGroup() - valence;
			}
			else if ((a_it->getElement().getAtomicNumber() == 13))
			{
				atom_charge += a_it->getElement().getGroup() -10 - valence;
			}
			else if ((a_it->getElement().getAtomicNumber() <= 17) &&
								(a_it->getElement().getAtomicNumber() >= 14) )
			{
				atom_charge -= 18 - a_it->getElement().getGroup() - valence;
			}
			else
			{
				//TODO ask Dirk
			}
			charge += atom_charge;
/*	//Anne
	if (atom_charge != 0)
		cout << a_it->getFullName() << "  charge: " << atom_charge << endl; 
*/					
		}
		total_charge = charge;

		// store the runtimes
		node_expansions = number_of_node_expansions;
		queue_size = search_queue_size;
		parent = abop;
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
		total_charge = 0.;
		node_expansions = 0;
		queue_size = 0;
		parent = NULL;
	}
	
	//////////////////////////// the PQ_Entry_ - class
	
	// Default constructor
	AssignBondOrderProcessor::PQ_Entry_::PQ_Entry_()
		: estimated_atom_type_penalty(0.), 
			estimated_bond_length_penalty(0.),
			bond_orders(),
			last_bond()
	{
	}

	// Copy constructor
	AssignBondOrderProcessor::PQ_Entry_::PQ_Entry_(const AssignBondOrderProcessor::PQ_Entry_& entry)
		: estimated_atom_type_penalty(entry.estimated_atom_type_penalty),
			estimated_bond_length_penalty(entry.estimated_bond_length_penalty),
			bond_orders(entry.bond_orders),
			last_bond(entry.last_bond)
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
			if (coarsePenalty() == b.coarsePenalty())
			{
				if (use_fine_penalty && (finePenalty() > b.finePenalty()))
				{
					value = true;
				}
			}
		}

#ifdef DEBUG
cout << "AssignBondOrderProcessor::PQ_Entry_::operator <: " <<  coarsePenalty() << " > " << b.coarsePenalty() << "   " <<  finePenalty() << " > " << b.finePenalty() <<
	" yields" << value <<  endl;
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
				// set the static variables in the PQ entries for faster access
				PQ_Entry_::alpha = alpha_;
				PQ_Entry_::atom_type_normalization_factor = atom_type_normalization_factor_;
				PQ_Entry_::bond_length_normalization_factor = bond_length_normalization_factor_;
				PQ_Entry_::use_fine_penalty = use_fine_penalty_;

				PQ_Entry_ entry;
				
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
						entry.bond_orders[i] = (short)b_it->getOrder();	
						i++;
					}
				}

				entry.last_bond = ac->countBonds()-1;

				if (estimatePenalty_(entry))
				{	
					evaluation_mode_ = false;
					ret = entry.coarsePenalty();

#ifdef  DEBUG_BOND_LENGTH
cout << "coarse: " << ret << "  fine" << entry.finePenalty() << endl;
#endif
				}
				else
				{
					Log.info() << "AssignBondOrderProcessor estimatePenalty() returned an error!"<< __LINE__ << endl;
					evaluation_mode_ = false;
					ret = -1.;
				}
			}
		}
		else
		{
			Log.info() << "Error in evaluatePenalty: valid = " <<  valid_ << ", readAtomPenalties = " << readAtomPenalties_() << endl;
		}
	
		evaluation_mode_ = false;

		add_missing_hydrogens_ = add_missing_hydrogens_backup;

		return ret;
	}


} // namespace BALL
