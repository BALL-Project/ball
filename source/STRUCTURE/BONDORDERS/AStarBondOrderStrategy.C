#include <BALL/STRUCTURE/BONDORDERS/AStarBondOrderStrategy.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/PTE.h>

#define INFINITE_PENALTY 1e5

namespace BALL
{
	const String AStarBondOrderStrategy::Heuristic::SIMPLE = "heurisic_simple";
	const String AStarBondOrderStrategy::Heuristic::MEDIUM = "heuristic_medium";
	const String AStarBondOrderStrategy::Heuristic::TIGHT  = "heuristic_tight";

	const char*  AStarBondOrderStrategy::Option::HEURISTIC  = "heuristic";
	const String AStarBondOrderStrategy::Default::HEURISTIC = AStarBondOrderStrategy::Heuristic::TIGHT;
	
	AStarBondOrderStrategy::AStarBondOrderStrategy(AssignBondOrderProcessor* parent)
		: BondOrderAssignmentStrategy(parent),
			heuristic_index_(PartialBondOrderAssignment::SIMPLE),
			queue_(),
		  step_(0)
	{
	}

	AStarBondOrderStrategy::~AStarBondOrderStrategy()
	{
	}

	void AStarBondOrderStrategy::clear()
	{
		queue_ = std::priority_queue<PartialBondOrderAssignment>();
		step_ = 0;
	}

	bool AStarBondOrderStrategy::readOptions(const Options& options)
	{
		String heuristic = options.get(Option::HEURISTIC);
		if (heuristic == Heuristic::TIGHT)
			heuristic_index_ = PartialBondOrderAssignment::TIGHT;
		else if (heuristic == Heuristic::MEDIUM)
			heuristic_index_ = PartialBondOrderAssignment::MEDIUM;
		else if (heuristic == Heuristic::SIMPLE)
			heuristic_index_ = PartialBondOrderAssignment::SIMPLE;

		return true;
	}

	void AStarBondOrderStrategy::setDefaultOptions()
	{
		abop->options.setDefault(AStarBondOrderStrategy::Option::HEURISTIC,
		                         AStarBondOrderStrategy::Default::HEURISTIC);		
	}

	void AStarBondOrderStrategy::init()
	{
		clear();
		// Initialize a priority queue and try to find a first solution

		// Initialize the priority queue
		PartialBondOrderAssignment entry(abop);
		entry.bond_orders.resize(abop->total_num_of_bonds_ + abop->num_of_virtual_bonds_, -1);
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

	boost::shared_ptr<BondOrderAssignment> AStarBondOrderStrategy::computeNextSolution()
	{
		// try to find a solution
		while (!queue_.empty())
		{

#ifdef DEBUG
cout << " Next ASTAR step : queue size : " << queue_.size();
#endif

			// take the top entry of the queue
			PartialBondOrderAssignment entry = queue_.top();
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
			if (entry.last_bond == (abop->total_num_of_bonds_ - 1 + abop->num_of_virtual_bonds_))
			{
				// we found a solution
				// store the solution :-)
				return (entry.convertToFullAssignment());
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
							entry.bond_orders[entry.last_bond] = i;

							// Penalty estimation went well  
							if (entry.estimatePenalty_())
							{
								// Add to the queue 
								queue_.push(entry);
							}
						} // end of all bond orders
					} // end of case 2
				} // end of free bond	
			} // no valid bond order try -> do nothing
		}

		// If we had found a solution, we would have bailed out already
		return boost::shared_ptr<BondOrderAssignment>();
	}


}
