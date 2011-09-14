#include <BALL/STRUCTURE/BONDORDERS/KGreedyBondOrderStrategy.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>

namespace BALL
{
	const char* KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE = "size_of_greedy_priority_queue";
	const int   KGreedyBondOrderStrategy::Default::GREEDY_K_SIZE = 5;

	KGreedyBondOrderStrategy::KGreedyBondOrderStrategy(AssignBondOrderProcessor* parent)
		: AStarBondOrderStrategy(parent),
			greedy_atom_type_penalty_(0.),
			greedy_bond_length_penalty_(0.),
			greedy_k_(0),
			greedy_node_expansions_(0)
	{
	}

	KGreedyBondOrderStrategy::~KGreedyBondOrderStrategy()
	{
	}

	void KGreedyBondOrderStrategy::clear()
	{
		AStarBondOrderStrategy::clear();

		greedy_atom_type_penalty_ = 0;
		greedy_bond_length_penalty_ = 0;
		greedy_node_expansions_ = 0;
	}

	bool KGreedyBondOrderStrategy::readOptions(const Options& options)
	{
		bool result = AStarBondOrderStrategy::readOptions(options);

		if (options.getReal(Option::GREEDY_K_SIZE) < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::GREEDY_K_SIZE."  << std::endl;

			return false;
		}

		return result;
	}

	void KGreedyBondOrderStrategy::setDefaultOptions()
	{
		AStarBondOrderStrategy::setDefaultOptions();

		abop->options.setDefaultInteger(KGreedyBondOrderStrategy::Option::GREEDY_K_SIZE,
		                          		  KGreedyBondOrderStrategy::Default::GREEDY_K_SIZE);
	}

	void KGreedyBondOrderStrategy::init()
	{
		clear();

		// in the k-greedy case, init() alredy computes the k solutions
		// computeNextSolution() then only returns them from the results vector
		current_solution_ = 0;
		greedy_set_.clear();

		greedy_k_ = abop->options.getInteger(Option::GREEDY_K_SIZE);

		// Create a dummy entry for initialization of the greedy set
		PartialBondOrderAssignment entry(abop);
		entry.bond_orders.resize(abop->total_num_of_bonds_ + abop->num_of_virtual_bonds_, -1);
		entry.last_bond = 0;

		// Perform the greedy search ...
		greedy_set_ = vector<PartialBondOrderAssignment>(greedy_k_, entry);

		// this is needed, since in the beginning, we have not yet seen k entries
		Size greedy_set_size = 1; 
		greedy_node_expansions_ = 0;
		
		AtomContainer* ac = abop->ac_;

		for (Position i = 0; i < ac->countBonds(); i++)
		{
			// Is this bond fixed?
			if (abop->bond_fixed_[abop->index_to_bond_[entry.last_bond]])
			{
				// Store this bond order in all nodes of our k-greedy-set
				Size min_size = std::min((Size)greedy_set_.size(), greedy_k_);

				for (Size j=0; j < min_size; j++)
				{
					PartialBondOrderAssignment& current_entry = greedy_set_[j];
					current_entry.last_bond = i;
					current_entry.bond_orders[current_entry.last_bond] = abop->bond_fixed_[abop->index_to_bond_[current_entry.last_bond]];
					// we don't need to check the return value since invalid solutions have INFINITE_PENALTY
					current_entry.estimatePenalty_(false);
				}
			}
			else // Bond is free -> try all bond orders for all nodes in our greedy set
			{	
				queue_ = std::priority_queue<PartialBondOrderAssignment>();
				Size min_size =  std::min(greedy_set_size, greedy_k_);
				for (Size j=0; j < min_size; j++)
				{
					greedy_node_expansions_++;
					PartialBondOrderAssignment& current_entry = greedy_set_[j];
					current_entry.last_bond = i;

					// case 1: VIRTUAL__BOND 
					if (current_entry.last_bond >= abop->total_num_of_bonds_ && abop->add_missing_hydrogens_)
					{
						int virtual_bond_index = current_entry.last_bond - abop->total_num_of_bonds_;
						int max_virtual_hydrogens = 
							abop->virtual_bond_index_to_number_of_virtual_hydrogens_[virtual_bond_index];
						
						for (int vh = 0; vh <= max_virtual_hydrogens; vh++)
						{
							// Set the bond order
							PartialBondOrderAssignment new_entry(current_entry);
							new_entry.bond_orders[i] = vh;

							// we don't need to check the return value since invalid solutions have INFINITE_PENALTY
							new_entry.estimatePenalty_(false);
							queue_.push(new_entry);
						}
					}
					else // case 2: original bond
					{	
						for (int bo = 1; bo <= abop->max_bond_order_; bo++)
						{
							// Set the bond order
							PartialBondOrderAssignment new_entry(current_entry);
							new_entry.bond_orders[i] = bo;

							// we don't need to check the return value since invalid solutions have INFINITE_PENALTY
							new_entry.estimatePenalty_(false);
							queue_.push(new_entry);
						}
					} // end of case 2
				} // end of greedy set 
				
				greedy_set_size = 0; 
				// now copy at most k elements from the queue to the greedy set
				for (Position current_element = 0; 
						     			(current_element < greedy_k_) && !queue_.empty(); 
								 			current_element++)
				{
					greedy_set_[current_element] = queue_.top();
					queue_.pop();
					greedy_set_size++;
				}
			} // end of bond is free
		}

		if (greedy_set_size < greedy_k_)
			greedy_set_.resize(greedy_set_size, entry);
	}

	boost::shared_ptr<BondOrderAssignment> KGreedyBondOrderStrategy::computeNextSolution()
	{
		boost::shared_ptr<BondOrderAssignment> result;

		if (current_solution_ < greedy_set_.size())
		{
			result = greedy_set_[current_solution_++].convertToFullAssignment();
		}

		return result;
	}

}
