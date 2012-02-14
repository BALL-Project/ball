#include <BALL/STRUCTURE/BONDORDERS/partialBondOrderAssignment.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/PTE.h>

namespace BALL
{
#define INFINITE_PENALTY 1e5

	// Default constructor
	PartialBondOrderAssignment::PartialBondOrderAssignment(AssignBondOrderProcessor* parent)
		: estimated_atom_type_penalty(0.),
			estimated_bond_length_penalty(0.),
			bond_orders(),
			last_bond(),
			abop(parent)
	{
	}

	// Destructor
	PartialBondOrderAssignment::~PartialBondOrderAssignment()
	{
	}

	// clear-method
	void PartialBondOrderAssignment::clear()
	{
		// Do *NOT* clear the abop pointer!
		estimated_atom_type_penalty = 0.;
		estimated_bond_length_penalty = 0.;
		last_bond = 0;
		bond_orders.clear();
	}

	// the less operator
	// note: we want a reverse sort, hence we actually return a "greater"
	bool PartialBondOrderAssignment::operator < (const PartialBondOrderAssignment& b) const
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
				if (abop->use_fine_penalty_ && (finePenalty() > b.finePenalty()))
				{
					value = true;
				}
			}
		}

#ifdef DEBUG
cout << "PartialBondOrderAssignment::operator <: " <<  coarsePenalty() << " > " << b.coarsePenalty() << "   " <<  finePenalty() << " > " << b.finePenalty() <<
	" yields" << value <<  std::endl;
#endif

		return value;
	}

	boost::shared_ptr<BondOrderAssignment> PartialBondOrderAssignment::convertToFullAssignment()
	{
		boost::shared_ptr<BondOrderAssignment> result(new BondOrderAssignment(abop));

		result->valid = true;
		result->last_bond = last_bond;
		result->bond_orders = bond_orders;

		Size total_num_of_bonds = abop->total_num_of_bonds_;
		Size num_of_virtual_bonds = abop->num_of_virtual_bonds_;

		result->ac = abop->ac_;

		if (bond_orders.size() != (total_num_of_bonds + num_of_virtual_bonds))
		{
			Log.error() << "Error: " << __FILE__ << " " << __LINE__  << std::endl;
			result->valid = false;
		}

		result->estimated_atom_type_penalty   = estimated_atom_type_penalty;
		result->estimated_bond_length_penalty = estimated_bond_length_penalty;

		// convert the entry's bond order vector into a HashMap
		//    first the original bonds
		for (Size i=0; i < total_num_of_bonds; i++)
		{
			result->bond_order_map[abop->index_to_bond_[i]] =  bond_orders[i];
		}
		//   then the virtual bonds
		for (Size i = 0; i < num_of_virtual_bonds; i++)
		{
			int num_H = bond_orders[total_num_of_bonds + i];
			if (num_H > 0)
			{
				Atom* atom = abop->virtual_bond_index_to_atom_[i];
				result->number_of_virtual_hydrogens[atom] = num_H;
			}
		}

		// Nothing has to be done, since this datastructure is handled by the
		// methods apply_() and computeNextSolution()	
		//atoms_to_delete;

		// compute the total charge
		// TODO: the favoured Nobel gas configuration should be stored in class PTE
		// NOTE: this is just a workaround as long no core-developer decision has been made!
		float charge = 0;
		for (AtomIterator a_it = abop->ac_->beginAtom(); a_it != abop->ac_->endAtom(); a_it++)
		{
			int valence = 0;
			int atom_charge = 0;
			for (Atom::BondIterator b_it = a_it->beginBond(); b_it != a_it->endBond(); b_it++)
			{
				valence += result->bond_order_map[&*b_it];
			}

			if (result->number_of_virtual_hydrogens.find(&*a_it) != result->number_of_virtual_hydrogens.end())
				valence += result->number_of_virtual_hydrogens[&*a_it];

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
		}
		result->total_charge = charge;

		// store the runtimes
		// TODO: we should probably get rid of these...
//		result->node_expansions = number_of_node_expansions;
//		result->queue_size = search_queue_size;

		return result;
	}

	float PartialBondOrderAssignment::coarsePenalty(float atom_type_penalty, float bond_length_penalty) const
	{
		return ( ( (    (abop->atom_type_normalization_factor_ < 0.0001)
						     || (abop->bond_length_normalization_factor_ < 0.0001)
						     || (abop->alpha_ < 0.0001)) ?
					atom_type_penalty :
					((1.-abop->alpha_) * (atom_type_penalty / abop->atom_type_normalization_factor_)
					 + (abop->alpha_* bond_length_penalty / abop->bond_length_normalization_factor_))));
	}

	float PartialBondOrderAssignment::coarsePenalty() const
	{
		return coarsePenalty(estimated_atom_type_penalty, estimated_bond_length_penalty);
	}

	float PartialBondOrderAssignment::getAtomTypePenalty(bool include_heuristic_term, HEURISTIC_INDEX heuristic_index)
	{
		if (estimated_atom_type_penalty >= INFINITE_PENALTY)
			estimatePenalty_(include_heuristic_term, heuristic_index);

		return estimated_atom_type_penalty;
	}

	bool PartialBondOrderAssignment::estimatePenalty_(bool include_heuristic_term, HEURISTIC_INDEX heuristic_index)
	{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "PartialBondOrderAssignment::called estimatePenalty_()"<< std::endl;
#endif

		AtomIterator a_it = abop->ac_->beginAtom();
		Atom::BondIterator b_it;
		int valence = 0;       // the so far fixed valence of the considered atom
		bool closed = true;    // are all bonds of this atom fixed? 
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
		for (; a_it != abop->ac_->endAtom() && valid ; a_it++, current_atom_index++)
		{
#if defined DEBUG || defined DEBUG_ESTIMATE
		cout << a_it->getFullName()<< " : " << std::endl;
#endif
			vector<Bond*> free_bonds;    // store the free bonds for bond length penalty 
			                             //   calculation (without VIRTUAL__BOND's!)
			valence = 0;                 // sum of bond orders of all fixed bonds 
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
				if (abop->bond_to_index_.find(current_bond) != abop->bond_to_index_.end())
				{
					// Is this bond already fixed?
					if (    (abop->bond_to_index_[current_bond] <= (Index)last_bond)
							 && (bond_orders[abop->bond_to_index_[current_bond]] > 0)   )
					{
						int order = bond_orders[abop->bond_to_index_[current_bond]];
						valence += order;

						// Store the bond length penalty
						// We just consider non-virtual bonds here!
						if (abop->bond_lengths_penalties_.find(current_bond) != abop->bond_lengths_penalties_.end())
						{
							current_bond_length_penalty += abop->bond_lengths_penalties_[&*b_it][order];
						}
						else
						{
							Log.error() << "Error " << __FILE__ << " " << __LINE__ << std::endl;
						}

#if defined DEBUG || defined DEBUG_ESTIMATE
						cout << "   bond " << abop->bond_to_index_[&*b_it] << " (" << b_it->getFirstAtom()->getFullName() << "-" << b_it->getSecondAtom()->getFullName() << ") fixed"
							<< " by order " << bond_orders[abop->bond_to_index_[&*b_it]]
							<< " : bond len penalty: " << abop->bond_lengths_penalties_[&*b_it][order]
							<< " -+-> cur bond len pen:"<< current_bond_length_penalty << std::endl;
#endif

					}
					else // a free bond 
					{
						closed = false;
						num_free_bonds++;
						free_bonds.push_back(&*b_it); // store the bond length penalty
					}
				}
				else  // no valid bond
				{
					Log.error() << "Error: invalid bond!" << __FILE__ << " " << __LINE__ << std::endl;
				}
			}

			// Iterator over all its virtual hydrogens!
			int virtual_order = 0;
			if (     abop->add_missing_hydrogens_
					 && (abop->atom_to_virtual_bond_index_.find(&*a_it) != abop->atom_to_virtual_bond_index_.end()))
			{
				virtual_order = std::max((short)0,bond_orders[abop->atom_to_virtual_bond_index_[&*a_it] + abop->total_num_of_bonds_]);
				valence += virtual_order;
			}

			// Decide, whether there is a free virtual bond left => atom not closed yet!
			if (   abop->add_missing_hydrogens_
					&& (last_bond != (abop->total_num_of_bonds_ + abop->num_of_virtual_bonds_ - 1)))
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
			//           * the atom type penalty can simply be computed by evaluating the penalty-vector 
			//             at the already computed valence at position "virtal_order many virtual H's"
			// Case (b)  We have to take the penalties resulting from the already set bonds as a minimum
			//           and have to add the best possible (minimal) penalty of any valid distribution 
			//           of the non-fixed bonds.
			//           * so far, the bond length penalty and atom type penalty contribution resulting  
			//             from the __fixed__ bonds are stored in
			//             -- current_bond_length_penalty and 
			//             -- and given by the penalty-vector at the already computed valence
			//                at position "virtal_order many virtual H's"
			//             Leave out the virtual hydrogens since they have no correct position so far!
			//           * for the __non-fixed__ bonds we have to 
			//             compute an estimate of the bond length penalty, the minimal possible 
			//             bond_length_penalties for the remaining bonds --> estimateBondLengthPenalty_()
			//             and  estimate the atom penalty by finding the min in all remaining possible 
			//             penalties in the possible penalty vectors. --> estimateAtomTypePenalty_()
			// For speed up return false if something exploded :-) 

			// In case something very strange happens:
			// Remember, we start atom counting with 0
			if (current_atom_index >= (int)abop->atom_to_block_.size())
			{
				Log.error() << "Error: Atom  " << a_it->getFullName() << " out of index ! "
				            <<  __FILE__ << " "  << __LINE__ << std::endl;

				estimated_atom_type_penalty = INFINITE_PENALTY;
				return false;
			}

			// This should not happen either: Far to many virtual hydrogens choosen 
			if (virtual_order > (int)abop->atom_to_block_[current_atom_index].size())
			{
				Log.error() << "Error: Too many virtual bonds  "
				            <<  __FILE__ << " " << __LINE__ << std::endl;
				estimated_atom_type_penalty = INFINITE_PENALTY;
				return false;
			}

			// Are all bonds of this atom already fixed?
			if (closed)
			{
				// Get the current penalty vector 
				int block = abop->atom_to_block_[current_atom_index][virtual_order];

				if (block < 0)
				{
					Log.error() << "Error: No penalty vector for atom " << current_atom_index << " "
					            << virtual_order << "! " << __FILE__ << " " << __LINE__ << std::endl;

					estimated_atom_type_penalty = INFINITE_PENALTY;
					return false;
				}

				int current_start_valence = abop->block_to_start_valence_[block];
				int current_block_length  = abop->block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;
				int current_start_index   = abop->block_to_start_idx_[block];

				// Is the valence found already greater than the largest for this atom type?
				if (valence > current_end_valence)
				{

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  Error: valence explosion for atom " << a_it->getFullName() << std::endl;
#endif
					if (abop->evaluation_mode_)
					{
						Log.info() << "  AssignBondOrderProcessor: Valence explosion for atom "
						           <<   a_it->getFullName()  << " :  val " << valence
					                   << ", free bonds "<<  num_free_bonds <<  ", Ruleid " <<  block +1
						           << " with length " <<  abop->block_to_length_[block] << std::endl;
					}

					estimated_atom_type_penalty = INFINITE_PENALTY;
					return false;
				}

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << "  CLOSED atom " <<   a_it->getFullName() << " with valence " << valence << std::endl;
#endif

				// is the valence large enough for this atom type?
				if (valence >= current_start_valence)
				{
					// NOTE: case valence too large was handled before

					// add the actual penalty
					estimated_atom_penalty   +=  abop->penalties_[current_start_index + valence  - current_start_valence];
					estimated_bond_penalty   +=  current_bond_length_penalty;

#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " ESTIMATE RESULT:  atom type penalty   +" << penalties_[current_start_index + valence - current_start_valence] << " = " << estimated_atom_penalty << std::endl
	   << "                   bond length penalty +" << current_bond_length_penalty << " = " << estimated_bond_penalty << std::endl;
#endif


				}
				else // valence too small
				{
#if defined DEBUG || defined DEBUG_ESTIMATE
cout << " ERROR: "<< a_it->getFullName() << " valence too small : " << valence <<   " < " <<  current_start_valence << std::endl;
#endif
					if (abop->evaluation_mode_)
					{
						Log.info() << "AssignBondOrderProcessor: Valence of " << a_it->getFullName()
						           << " too small : " << valence
						           <<   " < " <<  current_start_valence
						           << ". Consider adding hydrogen atoms!" << std::endl;
					}
					estimated_atom_type_penalty = INFINITE_PENALTY;
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
			                         valence, virtual_order, num_free_bonds, heuristic_index);

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
		estimated_atom_type_penalty = estimated_atom_penalty;

		//store the computed bond length penalty
		estimated_bond_length_penalty = estimated_bond_penalty;

#if defined DEBUG || defined DEBUG_ESTIMATE		
cout << " ESTIMATE RESULT for bond orders ( ";
for (Size i=0; i < bond_orders.size(); i++)
{
	cout << " " << bond_orders[i];
}
cout << " ) : atom type pen = " << estimated_atom_type_penalty << ", bond len pen = "<< estimated_bond_length_penalty << std::endl;
#endif

		return true;
	}

	// Computes the atom type penalty for a given atom
	// returns -1 if no valid atom type penalty for this atom could be computed!
	float PartialBondOrderAssignment::estimateAtomTypePenalty_(Atom* atom, Index atom_index, int fixed_valence,
																														 int fixed_virtual_order, int num_free_bonds,
																														 HEURISTIC_INDEX heuristic_index)
	{
		bool found_a_value = false;
		// We have to estimate the penalty heuristically 
		// by finding the minimum in all further possible penalty vector entries.

		// the minimal penalty of all possible penalty vectors for this atom
		float min_atom_type_penalty = std::numeric_limits<float>::max();

		// just look into the penalty vectors starting from the already fixed virtual order
		for (Size vi = fixed_virtual_order; vi < abop->atom_to_block_[atom_index].size(); vi++)
		{
			// get the corresponding penalty vector
			int block = abop->atom_to_block_[atom_index][vi];

			// certain numbers of additional hydrogens, for which no valid penalty classes exist
			// are excluded by block index -1
			if (block >= 0)
			{
				int current_start_valence = abop->block_to_start_valence_[block];
				int current_block_length  = abop->block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;
				int current_start_index   = abop->block_to_start_idx_[block];

				if (fixed_valence <= current_end_valence)
				{
					// at which position in the current penalty vector are we at the moment?
					int i = current_start_valence;
					if (heuristic_index == SIMPLE)
					{
						// this is the default case
						// Position i = current_start_valence;
					}
					else if (heuristic_index == MEDIUM)
					{
						// at which position in the current penalty vector are we at the moment?
						i = (fixed_valence + num_free_bonds < current_start_valence) 
							? current_start_valence
							: fixed_valence + num_free_bonds;
					}
					else if (heuristic_index == TIGHT)
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
							//   the number of free bonds
							//   the number of virtual bonds
							Atom* neighbor =  b_it->getPartner(*atom);
							int neighbors_bond_order = 0;
							int neighbors_num_free_bonds = 0;
							int neighbors_virtual_bonds = 0;

							// is this bond free?
							if (   (abop->bond_to_index_[&*b_it] > (Index)last_bond)
									|| (bond_orders[abop->bond_to_index_[&*b_it]] <= 0)   ) // ???
							{

								// check all bonds of the neighbor atom
								Atom::BondIterator neighbor_b_it = neighbor->beginBond();
								for (; neighbor_b_it != neighbor->endBond(); neighbor_b_it++)
								{
									// Do we have a valid bond? 
									if ( abop->bond_to_index_.find(&*neighbor_b_it) != abop->bond_to_index_.end())
									{
										// Is this bond already fixed?
										if (    (abop->bond_to_index_[&*neighbor_b_it] <= (Index)last_bond)
												&& (bond_orders[abop->bond_to_index_[&*neighbor_b_it]] > 0)   )
										{
											neighbors_bond_order += bond_orders[abop->bond_to_index_[&*neighbor_b_it]];
										}
										else //free
										{
											neighbors_num_free_bonds++;
										}
									}
								}
								// count the virtual bonds
								if (    abop->add_missing_hydrogens_
										&& (abop->atom_to_virtual_bond_index_.find(neighbor) != abop->atom_to_virtual_bond_index_.end()))
								{
									neighbors_virtual_bonds = std::max((short)0,bond_orders[abop->atom_to_virtual_bond_index_[neighbor]
									                                   + abop->total_num_of_bonds_]);
								}
							}
							int n_atom_index = neighbor->getProperty("AssignBondOrderProcessorAtomNo").getUnsignedInt();
							// compute the maximal bond order for the connecting bond (neighbor -- atom)
							int n_block = abop->atom_to_block_[n_atom_index][neighbors_virtual_bonds];
							int n_current_start_valence = abop->block_to_start_valence_[n_block];
							int n_current_block_length  = abop->block_to_length_[n_block];
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
						Log.error() << __FILE__ << " " << __LINE__ << " : No heuristic defined! Please check the option Option::HEURISTIC."  << std::endl;
						return min_atom_type_penalty;
					}

					// for every remaining valence of the atom under consideration
					// (we know there is at least one)
					for (; i <= current_end_valence; i++)
					{
						float current_atom_type_penalty = abop->penalties_[current_start_index + i - current_start_valence];

						if (current_atom_type_penalty < min_atom_type_penalty)
						{
							min_atom_type_penalty = current_atom_type_penalty;
							found_a_value = true;
						}
					}
				}
				else
				{
					if (abop->evaluation_mode_)
					{
						Log.info() << "  AssignBondOrderProcessor: Valence explosion for atom "
						           <<   atom->getFullName()  << " :  val " << fixed_valence
						           << ", free bonds "<<  num_free_bonds <<  ", Ruleid " <<  block +1
						           << " with length " <<  abop->block_to_length_[block] << std::endl;
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
	float PartialBondOrderAssignment::estimateBondLengthPenalty_(Index atom_index, const vector<Bond*>& free_bonds,
	                                                         int fixed_virtual_order,  int fixed_valence, int num_free_bonds)
	{
		// NOTE: virtual bond are excluded!

		// we have to estimate the bond length penalty heuristically 
		// by trying to distribute the remaining free valences over all remaining free bonds
		// such that the summed bond length penalty is minimal

		// the minimal penalty of all possible penalty vectors for this atom
		float min_bond_length_penalty = std::numeric_limits<float>::max();

		bool found_a_value = false;

		// all possible free valences
		// NOTE: virtual hydrogens may use or don't use one free valence

		// just look into the penalty vectors starting from the already fixed virtual order
		for (Size vi = fixed_virtual_order; vi < abop->atom_to_block_[atom_index].size(); vi++)
		{
			// get the corresponding penalty vector
			int block = abop->atom_to_block_[atom_index][vi];

			// certain numbers of additional hydrogens, for which no valid penalty classes exist
			// are excluded by block index -1
			if (block >= 0)
			{
				int current_start_valence = abop->block_to_start_valence_[block];
				int current_block_length  = abop->block_to_length_[block];
				int current_end_valence   = current_start_valence + current_block_length-1;

				// do we have an explosion ?
				if (fixed_valence <= current_end_valence)
				{
					float bond_length_penalty_current_set_up = 0;

					// iterating over all free bonds 
					// (we know there is at least one free bond)
					for (Size fb=0; fb < free_bonds.size(); fb++)
					{
						float current_bond_min = std::numeric_limits<float>::max();
						// Note: for simplification we do not share the free bond 
						//       orders correctly! We just find the min!
						// the number of the remaining bond orders to be assigned 
						// to the free bonds are :
						std::vector<float>& current_bond_length_penalties = abop->bond_lengths_penalties_[free_bonds[fb]];
						int up_to = std::min( (int)current_bond_length_penalties.size()-1,
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
						bond_length_penalty_current_set_up += current_bond_min;
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




}
