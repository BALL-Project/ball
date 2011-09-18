#include <BALL/STRUCTURE/BONDORDERS/FPTBondOrderStrategy.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/forEach.h>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/iteration_macros.hpp>

namespace BALL
{

	const FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::infinite_penalty = 1e5;
	const FPTBondOrderStrategy::Valence FPTBondOrderStrategy::max_valence      = 8;

	String  FPTBondOrderStrategy::Option::UPPER_PENALTY_BOUND  = "upper_penalty_bound";

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::Default::UPPER_PENALTY_BOUND = infinite_penalty;

	FPTBondOrderStrategy::FPTBondOrderStrategy(AssignBondOrderProcessor* parent)
		: BondOrderAssignmentStrategy(parent)
	{
	}

	FPTBondOrderStrategy::~FPTBondOrderStrategy()
	{
		clear();
	}

	void FPTBondOrderStrategy::clear()
	{
		computing_data_ = boost::shared_ptr<ComputingData_>(new ComputingData_());
	}

	bool FPTBondOrderStrategy::readOptions(const Options& options)
	{
		upper_bound_ = options.get(Option::UPPER_PENALTY_BOUND).toFloat();

		return true;
	}

	void FPTBondOrderStrategy::setDefaultOptions()
	{
		abop->options.setDefault(FPTBondOrderStrategy::Option::UPPER_PENALTY_BOUND,
		                         FPTBondOrderStrategy::Default::UPPER_PENALTY_BOUND);
	}

	void FPTBondOrderStrategy::init()
	{
		clear();

		initPenaltyData_();

		// build graph from molecule
		// TODO: do we really need to pointer this? let's at least use boost::shared_ptr
		MolecularGraph* mol_graph = new MolecularGraph(*abop->ac_);
		computing_data_->molecule_graph = mol_graph;

		// TODO: do we need to sort the bonds in a particular way?

		// build NiceTreeDecomposition
		computing_data_->tw = boost::shared_ptr<TreeWidth<MolecularGraph> >(new TreeWidth<MolecularGraph>(*mol_graph));

		// compute BondAssignments
		std::vector<boost::shared_ptr<TreeDecomposition> > & ntds      = computing_data_->tw->getNiceTreeDecompositions();
		std::vector<FPTBondOrderAssignment_*>& bond_assignments = computing_data_->bond_assignments;

		bond_assignments.reserve(ntds.size());
		for (Position i = 0; i < ntds.size(); ++i)
		{
			bond_assignments.push_back(new FPTBondOrderAssignment_(*this, ntds[i], upper_bound_));
			bond_assignments[i]->compute();
		}

		/*
		// initialize backtracking
		vector<FPTBondOrderAssignment*> & bond_assignments = computing_data_->bond_assignments;

		combiner_ = new DPBackTrackingCombiner(bond_assignments, num_solutions, upper_bound);
		if (combiner->hasMoreSolutions())
		{
			hasSolution = true;
			combiner->nextSolution();
		} 
		else
		{
			hasSolution = false;
		}
	*/
	}

	boost::shared_ptr<BondOrderAssignment> FPTBondOrderStrategy::computeNextSolution()
	{
		boost::shared_ptr<BondOrderAssignment> solution;

		return solution;
	}

	void FPTBondOrderStrategy::initPenaltyData_()
	{
		penalties_              = &abop->penalties_;	
		block_to_start_idx_     = &abop->block_to_start_idx_;
		block_to_length_        = &abop->block_to_length_;
		block_to_start_valence_ = &abop->block_to_start_valence_;
		atom_to_block_          = &abop->atom_to_block_;
	}

	// TODO: is this already contained in the AssignBondOrderProcessor?
	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::getPenaltyFor_(MolecularGraphTraits::VertexType vertex, Valence valence) const
	{
		int atom_block = (*atom_to_block_)[vertex].front();

		Size block_length = (*block_to_length_)[atom_block];

		Position start_index = (*block_to_start_idx_)[atom_block];

		int start_valence = (*block_to_start_valence_)[atom_block];

		int position      = valence - start_valence;

		if (position >= 0 && static_cast<Size>(position) < block_length)
		{
			int penalty = (*penalties_)[start_index + position];

			return (penalty < 0) ? infinite_penalty : static_cast<float>(penalty);
		} 
		else
		{
			return infinite_penalty;
		}
	}

  //*****************************************************************************************************
	//*           DPConfig_                                                                               *
	//*****************************************************************************************************

	FPTBondOrderStrategy::DPConfig_::DPConfig_() 
		: consumed_valences(0), 
		  bond_assignments(0)
	{
	}

	FPTBondOrderStrategy::DPConfig_::DPConfig_(Size atoms, Size bonds) 
		: consumed_valences(atoms, 0), 
		  bond_assignments(bonds, 0)
	{
	}

	FPTBondOrderStrategy::DPConfig_::DPConfig_(std::vector<Valence> const& v, std::vector<BondOrder> const& bo) 
		: consumed_valences(v), 
		  bond_assignments(bo)
	{
	}

	int FPTBondOrderStrategy::DPConfig_::compare(DPConfig_ const& conf) const
	{
		Size b_length = conf.numberOfBonds();
		Size a_length = conf.numberOfAtoms();

		if (b_length != numberOfBonds() || a_length != numberOfAtoms())
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "you can compare only DPConfig entries from the same table");
		}

		for (Size i = 0; i < b_length; ++i)
		{
			BondOrder left  = bond_assignments[i];
			BondOrder right = conf.bond_assignments[i];

			if (left < right)
				return -1;
			if (left > right)
				return 1;
		}

		for (Size i = 0; i < a_length; ++i)
		{
			Valence left  = consumed_valences[i];
			Valence right = conf.consumed_valences[i];

			if (left < right)
				return -1;
			if (left > right)
				return 1;
		}

		return 0;
	}

	Size FPTBondOrderStrategy::DPConfig_::numberOfAtoms() const
	{
		return consumed_valences.size();
	}

	Size FPTBondOrderStrategy::DPConfig_::numberOfBonds() const
	{
		return bond_assignments.size();
	}

	bool FPTBondOrderStrategy::DPConfig_::operator < (DPConfig_ const& conf) const
	{
		return compare(conf) < 0;
	}

	bool FPTBondOrderStrategy::DPConfig_::operator > (DPConfig_ const& conf) const
	{
		return compare(conf) > 0;
	}

	bool FPTBondOrderStrategy::DPConfig_::operator == (DPConfig_ const& conf) const
	{
		return compare(conf) == 0;
	}

	bool FPTBondOrderStrategy::DPConfig_::operator <= (DPConfig_ const& conf) const
	{
		return compare(conf) <= 0;
	}

	bool FPTBondOrderStrategy::DPConfig_::operator >= (DPConfig_ const& conf) const
	{
		return compare(conf) >= 0;
	}

	FPTBondOrderStrategy::DPConfig_& FPTBondOrderStrategy::DPConfig_::operator = (DPConfig_ const& copy)
	{
		if (this != &copy)
		{
			consumed_valences = copy.consumed_valences;
			bond_assignments  = copy.bond_assignments;
		}

		return *this;
	}

  //*****************************************************************************************************
	//*           DPTable_                                                                                *
	//*****************************************************************************************************

	FPTBondOrderStrategy::DPTable_::DPTable_() 
	  : table()
	{
	}

	FPTBondOrderStrategy::DPTable_::DPTable_(DPTable_ const& dptab) 
		: table(dptab.table)
	{
	}

	FPTBondOrderStrategy::DPTable_::iterator FPTBondOrderStrategy::DPTable_::begin()
	{
		return table.begin();
	}

	FPTBondOrderStrategy::DPTable_::iterator FPTBondOrderStrategy::DPTable_::end()
	{
		return table.end();
	}

	FPTBondOrderStrategy::DPTable_::const_iterator FPTBondOrderStrategy::DPTable_::begin() const
	{
		return table.begin();
	}

	FPTBondOrderStrategy::DPTable_::const_iterator FPTBondOrderStrategy::DPTable_::end() const
	{
		return table.end();
	}

	Size FPTBondOrderStrategy::DPTable_::size() const
	{
		return table.size();
	}

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::DPTable_::bestPenalty() const
	{
		Penalty pen = FPTBondOrderStrategy::infinite_penalty;

		for (const_iterator iter = begin(); iter != end(); ++iter)
		{
			pen = std::min(pen, iter->second);
		}

		return pen;
	}

	FPTBondOrderStrategy::DPConstRow_ FPTBondOrderStrategy::DPTable_::bestEntry() const
	{
		if (table.empty())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, 1, 0);
		}

		const_iterator iter = begin();
		const_iterator best = iter;

		for (++iter; iter != end(); ++iter)
		{
			if (iter->second < best->second)
			{
				best = iter;
			}
		}

		return *best;
	}

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::DPTable_::operator[](FPTBondOrderStrategy::DPConfig_ const& config) const
	{
		DPMap_::const_iterator iter = table.find(config);

		return (iter == table.end()) ? FPTBondOrderStrategy::infinite_penalty : iter->second;
	}

	bool FPTBondOrderStrategy::DPTable_::insert(DPConfig_ const& config, Penalty penalty)
	{
		if (penalty == FPTBondOrderStrategy::infinite_penalty)
			return false;

		std::pair<DPConfig_, Penalty> entry(config, penalty);

		std::pair<DPMap_::iterator, bool> insertion = table.insert(entry);

		if (insertion.second)
		{
			return true;
		} 
		else
		{
			if (insertion.first->second > penalty)
			{
				insertion.first->second = penalty;

				return true;
			} 
			else
			{
				return false;
			}
		}
	}

  //*****************************************************************************************************
	//*           AdditionalBagProperties_                                                                *
	//*****************************************************************************************************

	FPTBondOrderStrategy::AdditionalBagProperties_::AdditionalBagProperties_() 
		: bonds(), 
		  table(new DPTable_()) 
	{
	}

	FPTBondOrderStrategy::AdditionalBagProperties_::AdditionalBagProperties_(AdditionalBagProperties_ const& copy) 
		: bonds(copy.bonds), 
			table(new DPTable_(*copy.table))
	{
	}

	FPTBondOrderStrategy::AdditionalBagProperties_::~AdditionalBagProperties_()
	{
		delete table;
	}

	FPTBondOrderStrategy::AdditionalBagProperties_& FPTBondOrderStrategy::AdditionalBagProperties_::operator=(AdditionalBagProperties_ const& copy)
	{
		if (&copy != this) 
		{
			bonds = copy.bonds;
			table = new DPTable_(*copy.table);
		}

		return *this;
	}

  //*****************************************************************************************************
	//*           FPTBondOrderAssignment_                                                                 *
	//*****************************************************************************************************

	FPTBondOrderStrategy::FPTBondOrderAssignment_::FPTBondOrderAssignment_(FPTBondOrderStrategy& parent,
								             										 boost::shared_ptr<TreeDecomposition>& ntd, Penalty upper_bound) 
		: parent_(&parent), 
		  molecule_(parent.computing_data_->molecule_graph),
			ntd_(ntd), 
			properties_(boost::num_vertices(ntd->_g)), 
			upper_bound_(upper_bound),
			max_bond_order_(parent.abop->max_bond_order_),
			max_valence_(parent.max_valence)
	{
	}

	FPTBondOrderStrategy::FPTBondOrderAssignment_::~FPTBondOrderAssignment_()
	{
	}

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::FPTBondOrderAssignment_::compute()
	{
		GRAPH::PostOrderFolding<TreeDecomposition, TreeDecompositionBag, DPTable_*, FPTBondOrderAssignment_> pof(*ntd_, *this);

		DPTable_* table = pof.getResult();

		return table->bestPenalty();
	}

	FPTBondOrderStrategy::DPTable_* FPTBondOrderStrategy::FPTBondOrderAssignment_::operator() (TreeDecompositionBag& bag, 
	                                                                                           std::vector<DPTable_*>::const_iterator begin, 
																																														 std::vector<DPTable_*>::const_iterator end)
	{
		AdditionalBagProperties_& bag_properties = properties_[boost::get(boost::vertex_index, *ntd_, bag)];

		std::vector<DPTable_*>::const_iterator first  = begin;
		std::vector<DPTable_*>::const_iterator second = begin;
		++second;

		// check the pointers
		switch (boost::get(boost::vertex_bag_type, *ntd_, bag))
		{
			case TreeWidth<MolecularGraph>::JOIN_BAG:
				if (first == end || second == end || *second == NULL)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
			case TreeWidth<MolecularGraph>::INTRODUCE_BAG:
			case TreeWidth<MolecularGraph>::FORGET_BAG:
			case TreeWidth<MolecularGraph>::ROOT_BAG:
				if (first == end || *first == NULL)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
			case TreeWidth<MolecularGraph>::LEAF_BAG: 
				break;
			case TreeWidth<MolecularGraph>::END_BAG:
				if (boost::num_vertices(*molecule_) == 0 && (bag == root(*ntd_)))
				{
					// empty molecule -> empty table
					bag_properties.table->insert(DPConfig_(), 0);
					return bag_properties.table;
				} 
				else
				{
					// else nice tree decomposition is damaged
					throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
				}
		}

		// remember bonds in bag
		bag_properties.bonds = getBondsInBag(bag);

		// compute the table
		switch (boost::get(boost::vertex_bag_type, *ntd_, bag))
		{
			case TreeWidth<MolecularGraph>::INTRODUCE_BAG:
				computeIntroduceBag(bag, **first, bag_properties);
				break;
			case TreeWidth<MolecularGraph>::FORGET_BAG:
				computeForgetBag(bag, **first, bag_properties);
				break;
			case TreeWidth<MolecularGraph>::JOIN_BAG:
				computeJoinBag(bag, **first, **second, bag_properties);
				break;
			case TreeWidth<MolecularGraph>::LEAF_BAG:
				computeLeafIntroduceBag(bag_properties);
				break;
			case TreeWidth<MolecularGraph>::ROOT_BAG:
				computeRootBag(bag, **first, bag_properties);
				break;
			default:
				break;
				// can't happen
		}
		//PRINT_TABLE(bag_properties, bag, *ntd_);
		return bag_properties.table;
	}

	void FPTBondOrderStrategy::FPTBondOrderAssignment_::computeLeafIntroduceBag(AdditionalBagProperties_& bag_properties)
	{
		for (BondOrder i = 0; i < max_bond_order_; ++i)
		{
			bag_properties.table->insert(DPConfig_(1, 0), 0);
		}
	}

	std::vector<MolecularGraphTraits::EdgeType> FPTBondOrderStrategy::FPTBondOrderAssignment_::getBondsInBag(TreeDecompositionBag& bag)
	{
		typedef TreeWidth<MolecularGraph>::TreeDecompositionContent TreeDecompositionContent;

		TreeDecompositionContent vertices = boost::get(boost::vertex_bag_content, *ntd_, bag);

		std::vector<MolecularGraphTraits::EdgeType> bonds;
		bonds.reserve((vertices.size()-1 * vertices.size())/2);

		for (TreeDecompositionContent::const_iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
		{
			TreeDecompositionContent::const_iterator iter2 = iter;

			for (++iter2; iter2 != vertices.end(); ++iter2)
			{
				if (boost::edge(*iter, *iter2, *molecule_).second)
				{
					bonds.push_back(boost::edge(*iter, *iter2, *molecule_).first);
				}
			}
		}

		return bonds;
	}

	void FPTBondOrderStrategy::FPTBondOrderAssignment_::computeIntroduceBag(TreeDecompositionBag& bag, DPTable_& child, 
	                                                                        AdditionalBagProperties_& property)
	{
		typedef TreeWidth<MolecularGraph>::TreeDecompositionContent TreeDecompositionContent;

		DPTable_& table = *property.table;

		MolecularGraphTraits::VertexType iv = boost::get(boost::vertex_bag_special, *ntd_, bag);

		TreeDecompositionContent vertices = boost::get(boost::vertex_bag_content, *ntd_, bag);
		Size num_valences = vertices.size();
		
		std::vector<MolecularGraphTraits::EdgeType>& bonds = property.bonds;
		Size num_bonds = bonds.size();

		// for each entry in child table, copy this entry 3^n times, where n is the number of introduced bonds. Then fill each
		// copy with n new bondvalue-columns with different values
		for (DPTable_::const_iterator iter = child.begin(); iter != child.end(); ++iter)
		{
			DPConstRow_ child_entry = *iter;
			DPConfig_ conf(num_valences, num_bonds);

			Size vindex = 0;
			Size cindex = 0;

			// because in our new entry we have one more valence-column, each vertex after the introduced vertex
			// have to be copied one column next to it's previous position
			for (TreeDecompositionContent::const_iterator it = vertices.begin(); it != vertices.end(); ++it, ++vindex)
			{
				if (*it != iv)
				{
					conf.consumed_valences[vindex] = child_entry.first.consumed_valences[cindex++];
				}
			}

			// the same happens with the bonds. Copy each bond which exists also in the child into it's correct position
			vindex = 0;
			cindex = 0;
			vector<Size> indices;

			for (vector<Edge>::const_iterator it = bonds.begin(); it != bonds.end(); ++it, ++vindex)
			{
				Edge e = *it;

				MolecularGraphTraits::VertexType source = boost::source(*it, *molecule_);
				MolecularGraphTraits::VertexType target = boost::target(*it, *molecule_);

				if (iv == source || iv == target)
				{
					// remember the indizes of the new introduced bonds
					indices.push_back(vindex);
				} 
				else
				{
					conf.bond_assignments[vindex] = child_entry.first.bond_assignments[cindex++];
				}
			}

			// if there are any introduced bonds we have to fill them with each possible value
			if (indices.size() > 0) 
			{
				std::vector<int> bond_values(indices.size());

				Size current_index = 0;
				while (current_index < bond_values.size())
				{
					current_index = 0;

					// insert entry
					for (Size k = 0; k < bond_values.size(); ++k)
					{
						conf.bond_assignments[indices[k]] = bond_values[k];
					}

					table.insert(conf, child_entry.second);
					// increase value in index
					while (current_index < bond_values.size() && ++bond_values[current_index] >= max_bond_order_)
					{
						bond_values[current_index] = 0;
						++current_index;
					}
				}
			} 
			else
			{
				table.insert(conf, child_entry.second);
			}
		}
	}

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::FPTBondOrderAssignment_::forgetInnerVertexIn(TreeDecompositionBag & bag,
			                                         DPConstRow_ child_row, DPConfig_& entry, std::vector<Edge>& child_bonds, Size forgotten_index)
	{
		typedef TreeWidth<MolecularGraph>::TreeDecompositionContent TreeDecompositionContent;

		DPConfig_ const& child_entry = child_row.first;

		MolecularGraphTraits::VertexType forgotten_vertex = boost::get(boost::vertex_bag_special, *ntd_, bag);

		TreeDecompositionContent vertices = boost::get(boost::vertex_bag_content, *ntd_, bag);

		Size bond_index = 0;
		Size child_bond_index = 0;

		Size forgotten_valence = child_entry.consumed_valences[forgotten_index];
		Size max_forgotten_valence = std::min(static_cast<BondOrder>(max_valence_-1),
																					static_cast<BondOrder>(boost::out_degree(forgotten_vertex, *molecule_) * max_bond_order_));

		// copy valence values in child entry into new entry
		for (Size i = 0; i < forgotten_index; ++i)
		{
			entry.consumed_valences[i] = child_entry.consumed_valences[i];
		}

		for (Size i = forgotten_index+1; i < child_entry.consumed_valences.size(); ++i)
		{
			entry.consumed_valences[i-1] = child_entry.consumed_valences[i];
		}

		// forget each bond which is incident to forgotten vertex
		for (; child_bond_index < child_bonds.size(); ++child_bond_index)
		{
			MolecularGraphTraits::VertexType source = boost::source(child_bonds[child_bond_index], *molecule_);
			MolecularGraphTraits::VertexType target = boost::target(child_bonds[child_bond_index], *molecule_);

			if (source == forgotten_vertex || target == forgotten_vertex)
			{
				// forget this bond, add its bondvalue to the consumed valence of the kept (not forgotten) 
				// atom and to the forgotten atom
				MolecularGraphTraits::VertexType kept_atom = (source == forgotten_vertex) ? target : source;

				BondOrder bond_value = child_entry.bond_assignments[child_bond_index] + 1;

				Size atom_index = distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), kept_atom));;
				Size new_valence = entry.consumed_valences[atom_index] + bond_value;
				forgotten_valence += bond_value;

				// if valence is greater than maximal possible valence (< 8, < number of neighbours * max bond value per neighbour)
				// then don't add this entry
				if ((forgotten_valence <= max_forgotten_valence) &&
						(new_valence <= std::min((Size)boost::out_degree(kept_atom, *molecule_) * (Size)(max_bond_order_), (Size)(max_valence_))))
				{
					entry.consumed_valences[atom_index] = new_valence;
				} 
				else
				{
					return FPTBondOrderStrategy::infinite_penalty;
				}
			} 
			else
			{
				// just copy bondvalue
				entry.bond_assignments[bond_index] = child_entry.bond_assignments[child_bond_index];
				++bond_index;
			}
		}
		if (forgotten_valence > max_forgotten_valence)
		{
			// shouldn't happen on this place
			return FPTBondOrderStrategy::infinite_penalty;
		}
		// add penalty of forgotten atom to the penalty of the entry
		return parent_->getPenaltyFor_(forgotten_vertex, forgotten_valence) + child_row.second;
	}

	void FPTBondOrderStrategy::FPTBondOrderAssignment_::computeForgetBag(TreeDecompositionBag& bag, DPTable_& child, 
																					                             AdditionalBagProperties_& property)
	{
		typedef TreeWidth<MolecularGraph>::TreeDecompositionContent TreeDecompositionContent;

		TreeDecomposition::children_iterator c_i, c_end;
		boost::tie(c_i, c_end) = children(bag, *ntd_);

		AdditionalBagProperties_& child_property = properties_[boost::get(boost::vertex_index, *ntd_, *c_i)];

		TreeDecompositionContent vertices = boost::get(boost::vertex_bag_content, *ntd_, *c_i);
		MolecularGraphTraits::VertexType forgotten_vertex = boost::get(boost::vertex_bag_special, *ntd_, bag);

		DPConfig_ new_entry(vertices.size()-1, property.bonds.size());

		Size forgotten_index = distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), forgotten_vertex));

		for (DPTable_::const_iterator entry_iterator = child.begin(); entry_iterator != child.end(); ++entry_iterator)
		{
			Penalty entry_penalty(forgetInnerVertexIn(bag, *entry_iterator, new_entry, child_property.bonds, forgotten_index));
			if (entry_penalty < upper_bound_)
			{
				property.table->insert(new_entry, entry_penalty);
			}
		}
	}

	void FPTBondOrderStrategy::FPTBondOrderAssignment_::computeRootBag(TreeDecompositionBag& bag, 
	                                                                   DPTable_& child, AdditionalBagProperties_& bag_properties)
	{
		DPConfig_ empty(0, 0);
		std::vector<Edge> empty_list;

		Penalty min_penalty = FPTBondOrderStrategy::infinite_penalty;
		for (DPTable_::const_iterator iter = child.begin(); iter != child.end(); ++iter)
		{
			min_penalty = std::min(min_penalty, forgetInnerVertexIn(bag, *iter, empty, empty_list, 0));

			if (Maths::isEqual(0, min_penalty))
			{
				break;
			}
		}

		bag_properties.table->insert(empty, min_penalty);
	}

	void FPTBondOrderStrategy::FPTBondOrderAssignment_::computeJoinBag(TreeDecompositionBag& bag,
                                  			DPTable_& left_child, DPTable_& right_child, AdditionalBagProperties_& property)
	{
		typedef TreeWidth<MolecularGraph>::TreeDecompositionContent TreeDecompositionContent;

		TreeDecompositionContent inner_vertices = boost::get(boost::vertex_bag_content, *ntd_, bag);
		std::vector<MolecularGraphTraits::VertexType> vertices(inner_vertices.begin(), inner_vertices.end());
		Size num_valences = vertices.size();

		DPTable_& table(*property.table);
		DPJoinMap_ map;

		// insert each entry of the left child into a DPJoinMap
		for (DPTable_::const_iterator left_iter = left_child.begin(); left_iter != left_child.end(); ++left_iter)
		{
			DPConstRow_ left_entry = *left_iter;
			map.insert(pair<DPConfig_ const*, Penalty> (&(left_entry.first), left_entry.second));
		}

		// find for each entry of the right child's table appropiate entries in the DPJoinMap (which have the same bondvalues)
		for (DPTable_::const_iterator r_iter = right_child.begin(); r_iter != right_child.end(); ++r_iter)
		{
			DPConstRow_ right_entry = *r_iter;
			DPConfig_ const* right_conf = &right_entry.first;

			pair<DPJoinMap_::const_iterator, DPJoinMap_::const_iterator> matching_range(map.equal_range(right_conf));

			for (DPJoinMap_::const_iterator match = matching_range.first; match != matching_range.second; ++match)
			{
				// merge both entries by summate the consumed valences and penalties
				DPConfig_ config = *match->first;

				bool do_insert = true;

				for (Size index = 0; index < num_valences; ++index)
				{
					config.consumed_valences[index] += right_entry.first.consumed_valences[index];
					{
						MolecularGraphTraits::VertexType vertex = vertices[index];
						if (config.consumed_valences[index] > (int) (std::min(static_cast<BondOrder>(max_valence_-1),
															max_bond_order_ * static_cast<BondOrder>(boost::out_degree(vertex, *molecule_)))))
						{
							do_insert = false;
							break;
						}
					}
				}

				Penalty penalty(right_entry.second + match->second);

				if (do_insert && penalty < FPTBondOrderStrategy::infinite_penalty)
				{
					table.insert(config, penalty);
				}
			}
		}
	}

}
