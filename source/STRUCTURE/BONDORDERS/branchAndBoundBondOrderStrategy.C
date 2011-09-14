#include <BALL/STRUCTURE/BONDORDERS/branchAndBoundBondOrderStrategy.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>

namespace BALL
{
	const char*  BranchAndBoundBondOrderStrategy::Option::BRANCH_AND_BOUND_CUTOFF = "branch_and_bound_cutoff";
	const float  BranchAndBoundBondOrderStrategy::Default::BRANCH_AND_BOUND_CUTOFF = 1;

	BranchAndBoundBondOrderStrategy::BranchAndBoundBondOrderStrategy(AssignBondOrderProcessor* parent)
		: KGreedyBondOrderStrategy(parent)
	{
	}

	BranchAndBoundBondOrderStrategy::~BranchAndBoundBondOrderStrategy()
	{
	}

	void BranchAndBoundBondOrderStrategy::clear()
	{
		KGreedyBondOrderStrategy::clear();
	}

	bool BranchAndBoundBondOrderStrategy::readOptions(const Options& options)
	{
		if (options.getReal(Option::BRANCH_AND_BOUND_CUTOFF) < 0)
		{
			Log.error() << __FILE__ << " " << __LINE__ 
				          << " : Error in options! Please check the option Option::BRANCH_AND_BOUND_CUTOFF."  << std::endl;

			return false;
		}

		return true;
	}

	void BranchAndBoundBondOrderStrategy::setDefaultOptions()
	{
		abop->options.setDefaultReal(BranchAndBoundBondOrderStrategy::Option::BRANCH_AND_BOUND_CUTOFF,
		                          	 BranchAndBoundBondOrderStrategy::Default::BRANCH_AND_BOUND_CUTOFF);
	}

	void BranchAndBoundBondOrderStrategy::init()
	{
		clear();

		// first, perform a greedy search to create a bound
		KGreedyBondOrderStrategy::init();

		greedy_atom_type_penalty_ = 0;
		greedy_bond_length_penalty_ = 0;

		if (greedy_set_.size() > 0)
		{
			// take the top solution as cutoff
			greedy_atom_type_penalty_   = greedy_set_[0].estimated_atom_type_penalty;
			greedy_bond_length_penalty_ = greedy_set_[0].estimated_bond_length_penalty; 
		}

		// clear the queue
		queue_ = std::priority_queue<PartialBondOrderAssignment>();

		// initialize the step counter with the number steps needed by k_greedy
		step_ = greedy_node_expansions_; 

		// Initialize the priority queue
		PartialBondOrderAssignment entry(abop);
		entry.bond_orders.resize(abop->total_num_of_bonds_ + abop->num_of_virtual_bonds_,-1);
		entry.last_bond = 0;

		// Is the first bond fixed?
		if (abop->bond_fixed_[abop->index_to_bond_[entry.last_bond]])
		{
			// Store this bond order
			entry.bond_orders[entry.last_bond] = abop->bond_fixed_[abop->index_to_bond_[entry.last_bond]];

			// Estimate the penalty and add the node into the queue if valid
			if (entry.estimatePenalty_())
			{
				queue_.push(entry);
			}
		}
		else // Bond is free -> try all bond orders
		{	
			// case 1: VIRTUAL__BOND 
			// This should not happen in the beginning, but who knows?
			if (entry.last_bond >= abop->total_num_of_bonds_ && abop->add_missing_hydrogens_)
			{
				int virtual_bond_index = entry.last_bond - abop->total_num_of_bonds_;
				int max_virtual_hydrogens = 
					abop->virtual_bond_index_to_number_of_virtual_hydrogens_[virtual_bond_index];

				for (int i = 0; i <= max_virtual_hydrogens; i++)
				{
					// Set the bond order
					entry.bond_orders[0] = i;

					// Estimate the penalty and add the node into the queue if valid
					if (entry.estimatePenalty_())
					{
						queue_.push(entry);
					}
				}
			}
			else // case 2: original bond
			{	
				for (int i = 1; i <= abop->max_bond_order_; i++)
				{
					// Set the bond order
					entry.bond_orders[0] = i;

					// Estimate the penalty and add the node into the queue if valid
					if (entry.estimatePenalty_())
					{
						queue_.push(entry);
					}
				}
			} // end of case 2
		} // end of bond is free
	}

	boost::shared_ptr<BondOrderAssignment> BranchAndBoundBondOrderStrategy::computeNextSolution()
	{
		boost::shared_ptr<BondOrderAssignment> result;

		if (current_solution_ != 0)
		{
			// branch and bound does not support computing next solutions!
			Log.warn() << " Branch&Bound does not support computing next solutions!" << std::endl;
		}
		else
		{
			PartialBondOrderAssignment entry(abop);

			float branch_and_bound_cutoff = 
								entry.coarsePenalty(greedy_atom_type_penalty_, greedy_bond_length_penalty_) 
							* abop->options.getReal(Option::BRANCH_AND_BOUND_CUTOFF); 

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
				cout << "    atom type penalty: "   << entry.estimated_atom_type_penalty << 
					"    bond length penalty: " << entry.estimated_bond_length_penalty << 
					"    bond orders: ( " ;
				for (Size i = 0; i< entry.bond_orders.size(); i++)
				{
					cout << " " <<   entry.bond_orders[i] ;
				}
				cout << ")" << endl;
#endif

				// is this a leaf?
				if (entry.last_bond == (abop->total_num_of_bonds_ - 1 + abop->num_of_virtual_bonds_))
				{
					// we found a solution
					result = entry.convertToFullAssignment();
				}
				else // no leaf
				{	
					entry.last_bond++;
					Bond* current_bond = abop->index_to_bond_[entry.last_bond];

					// Take the next bond and ...
					//   ... set to the prefixed value ... 
					if (abop->bond_fixed_[current_bond])
					{	
						// Set this fixed bond order
						entry.bond_orders[entry.last_bond] = abop->bond_fixed_[current_bond];

						// Estimate the penalty
						if (entry.estimatePenalty_())
						{
							queue_.push(entry);
						}
					}	
					//   ... or try all bond orders
					else 
					{
						// case 1: VIRTUAL__BOND
						if (entry.last_bond >= abop->total_num_of_bonds_)
						{	
							int virtual_bond_index = entry.last_bond - abop->total_num_of_bonds_;
							int max_virtual_hydrogens = 
								abop->virtual_bond_index_to_number_of_virtual_hydrogens_[virtual_bond_index];

							for (int i = 0; i <= max_virtual_hydrogens; i++)
							{
								// Set the bond order
								entry.bond_orders[entry.last_bond] = i;

								// Estimate the penalty and add the node into the queue if valid
								if (   entry.estimatePenalty_() 
										&& (entry.coarsePenalty() <= branch_and_bound_cutoff ))
								{
									queue_.push(entry);
								}
							}
						}
						else // case 2: original bond
						{	
							for (int i = 1; i <= abop->max_bond_order_; i++)
							{
								entry.bond_orders[entry.last_bond] = i;

								// Penalty estimation went well  
								if (entry.estimatePenalty_() && (entry.coarsePenalty() <= branch_and_bound_cutoff ))
								{
									// Add to the queue 
									queue_.push(entry);
								}
							} // end of all bond orders
						} // end of case 2
					}	// end of free bond	
				} // no valid bond order try -> do nothing
			}
		}

		return result;
	}

}
