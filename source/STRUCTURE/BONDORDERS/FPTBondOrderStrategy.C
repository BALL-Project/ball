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
		return true;
	}

	void FPTBondOrderStrategy::setDefaultOptions()
	{
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
		std::vector<boost::shared_ptr<TreeDecomposition> > & ntds = computing_data_->tw->getNiceTreeDecompositions();
		std::vector<FPTBondOrderAssignment_*>& bond_assignments   = computing_data_->bond_assignments;

		int max_penalty = abop->max_penalty_;
		if (max_penalty == -1)
			max_penalty = infinite_penalty;
		else
			max_penalty += 1;

		bond_assignments.reserve(ntds.size());
		for (Position i = 0; i < ntds.size(); ++i)
		{
			bond_assignments.push_back(new FPTBondOrderAssignment_(*this, ntds[i], max_penalty));
		  Penalty result = bond_assignments[i]->compute();
		}

		// initialize backtracking
		combiner_ = boost::shared_ptr<DPBackTrackingCombiner_>(
		                   new DPBackTrackingCombiner_(bond_assignments, abop->max_number_of_solutions_, max_penalty));
	}

	boost::shared_ptr<BondOrderAssignment> FPTBondOrderStrategy::computeNextSolution()
	{
		// TODO: this is not the most sensible way to do this...
		boost::shared_ptr<BondOrderAssignment> result;

		if (combiner_->hasMoreSolutions())
		{
			result = boost::shared_ptr<BondOrderAssignment>(new BondOrderAssignment(abop));
			result->ac = abop->ac_;

			result->valid = true;

			combiner_->nextSolution();
			Assignment_ const& solution = combiner_->getSolution();

			result->estimated_atom_type_penalty = solution.penalty;

			//TODO: virtual hydrogens? charges?
			for (Position current_bond_index=0; current_bond_index < combiner_->sorted_edges.size(); ++current_bond_index)
			{
				Bond* current_bond = const_cast<Bond*>(boost::get(boost::edge_bond_ptr, *computing_data_->molecule_graph,
				                                                  combiner_->sorted_edges[current_bond_index]));
				result->bond_order_map[current_bond] = solution[current_bond_index]+1;
			}

			AtomIterator at_it;
			Atom::BondIterator bond_it;
			BALL_FOREACH_BOND(*abop->getAtomContainer(), at_it, bond_it)
			{
				result->bond_orders.push_back(result->bond_order_map[&*bond_it]);
			}
		}
		else
		{
			// since we return a pointer, nothing to do here
			/*	Log.info() << "AssignBondOrderProcessor: strategy FPT does not support computeNextSolution(). " << endl
								 << "Please use the options Option::MAX_NUMBER_OF_SOLUTIONS or Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS " << endl
								 << "to compute additional solutions." << endl; */
		}

		return result;
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

		return DPConstRow_(boost::reference_wrapper<DPConfig_ const>(best->first), best->second);
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


		//PRINT_TABLE(bag_properties, bag, *ntd_, *molecule_);
		/*
#define DEBUG
#ifdef DEBUG
		{
			typedef TreeWidth<MolecularGraph>::TreeDecompositionBag TreeDecompositionBag;

			string tab;
			switch (boost::get(boost::vertex_bag_type, *ntd_, bag))
			{
				case TreeWidth<MolecularGraph>::INTRODUCE_BAG: tab = "introduce"; break;
				case TreeWidth<MolecularGraph>::FORGET_BAG: tab = "forget"; break;
				case TreeWidth<MolecularGraph>::LEAF_BAG: tab = "leaf"; break;
				case TreeWidth<MolecularGraph>::ROOT_BAG: tab = "root"; break;
				case TreeWidth<MolecularGraph>::JOIN_BAG: tab = "join"; break;
				default: tab = "UNDEFINED"; break;
			}

			DPTable_ const& table (*bag_properties.table);
			cout << "<table " << tab;

			if (boost::get(boost::vertex_bag_type, *ntd_, bag) != TreeWidth<MolecularGraph>::JOIN_BAG)
			{
				TreeWidth<MolecularGraph>::OriginalVertexType ov = boost::get(boost::vertex_bag_special, *ntd_, bag);

				Atom const* atom = boost::get(boost::vertex_atom_ptr, *molecule_, ov);
				cout << "  <-  " << ov << " " << atom->getFullName();
			}
			cout << "\n\t";

			TreeWidth<MolecularGraph>::TreeDecompositionContent content = boost::get(boost::vertex_bag_content, *ntd_, bag);
			for (TreeWidth<MolecularGraph>::TreeDecompositionContent::const_iterator iter  = content.begin(); 
			                                                                         iter != content.end(); ++iter)
			{
				TreeWidth<MolecularGraph>::OriginalVertexType ov = *iter;
				Atom const* atom = boost::get(boost::vertex_atom_ptr, *molecule_, ov);

				cout << ov << " " << atom->getFullName() << "\t\t";
			}
			cout << "\t|\t";
			std::vector<MolecularGraphTraits::EdgeType> bonds (bag_properties.bonds);
			for (std::vector<MolecularGraphTraits::EdgeType>::const_iterator iter = bonds.begin(); iter != bonds.end(); ++iter)
			{
				MolecularGraphTraits::EdgeType e = *iter;

				TreeWidth<MolecularGraph>::OriginalVertexType ov1 = boost::source(e, *molecule_);
				TreeWidth<MolecularGraph>::OriginalVertexType ov2 = boost::target(e, *molecule_);

				Atom const* atom1 = boost::get(boost::vertex_atom_ptr, *molecule_, ov1);
				Atom const* atom2 = boost::get(boost::vertex_atom_ptr, *molecule_, ov2);

				cout << atom1->getFullName() << "~" << atom2->getFullName() << "\t\t";
			}
			cout << "|\t\tPenalty\n";
			for (DPTable_::const_iterator citer = table.begin(); citer != table.end(); ++citer)
			{
				cout << "\t";
				DPConstRow_ row (*citer);
				DPConfig_ const& conf (row.first);
				for (vector<Valence>::const_iterator iter(conf.consumed_valences.begin()); iter !=
						conf.consumed_valences.end(); ++iter)
				{
					cout << *iter << "\t\t";
				}
				cout << "\t|\t";
				for (vector<BondOrder>::const_iterator iter(conf.bond_assignments.begin()); iter !=
						conf.bond_assignments.end(); ++iter)
				{
					cout << *iter << "\t\t";
				}
				cout << "|\t\t" << row.second << "\n";
			}
			cout << ">\n";
		}
#endif // DEBUG
		*/

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
	
#define	convertToCRow(tmp)\
		std::make_pair(boost::reference_wrapper<const DPConfig_>((tmp).first), (tmp).second)

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
			DPConstRow_ child_entry = convertToCRow(*iter);
			DPConfig_ conf(num_valences, num_bonds);

			Size vindex = 0;
			Size cindex = 0;

			// because in our new entry we have one more valence-column, each vertex after the introduced vertex
			// have to be copied one column next to it's previous position
			for (TreeDecompositionContent::const_iterator it = vertices.begin(); it != vertices.end(); ++it, ++vindex)
			{
				if (*it != iv)
				{
					conf.consumed_valences[vindex] = child_entry.first.get().consumed_valences[cindex++];
				}
			}

			// the same happens with the bonds. Copy each bond which exists also in the child into it's correct position
			vindex = 0;
			cindex = 0;
			vector<Size> indices;

			for (vector<Edge>::const_iterator it = bonds.begin(); it != bonds.end(); ++it, ++vindex)
			{
				Edge e = *it;

				MolecularGraphTraits::VertexType source = boost::source(e, *molecule_);
				MolecularGraphTraits::VertexType target = boost::target(e, *molecule_);

				if (iv == source || iv == target)
				{
					// remember the indizes of the new introduced bonds
					indices.push_back(vindex);
				}
				else
				{
					conf.bond_assignments[vindex] = child_entry.first.get().bond_assignments[cindex++];
				}
			}

			// if there are any introduced bonds we have to fill them with each possible value
			if (!indices.empty())
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

				Size atom_index = distance(vertices.begin(), std::find(vertices.begin(), vertices.end(), kept_atom));
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
			Penalty entry_penalty(forgetInnerVertexIn(bag, convertToCRow(*entry_iterator), new_entry, child_property.bonds, forgotten_index));
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
			min_penalty = std::min(min_penalty, forgetInnerVertexIn(bag, convertToCRow(*iter), empty, empty_list, 0));

			if (Maths::isEqual(0, min_penalty))
			{
				break;
			}
		}

		bag_properties.table->insert(empty, min_penalty);
	}

	void FPTBondOrderStrategy::FPTBondOrderAssignment_::computeJoinBag(TreeDecompositionBag& bag,
			                                                               DPTable_& left_child, DPTable_& right_child, 
																																		 AdditionalBagProperties_& property)
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
			DPConstRow_ left_entry = convertToCRow(*left_iter);
			map.insert(std::pair<DPConfig_ const*, Penalty> (left_entry.first.get_pointer(), left_entry.second));
		}

		// find for each entry of the right child's table appropiate entries in the DPJoinMap (which have the same bondvalues)
		for (DPTable_::const_iterator r_iter = right_child.begin(); r_iter != right_child.end(); ++r_iter)
		{
			DPConstRow_ right_entry = convertToCRow(*r_iter);
			DPConfig_ const* right_conf = right_entry.first.get_pointer();

			std::pair<DPJoinMap_::const_iterator, DPJoinMap_::const_iterator> matching_range(map.equal_range(right_conf));

			for (DPJoinMap_::const_iterator match  = matching_range.first;
					                            match != matching_range.second; ++match)
			{
				// merge both entries by summate the consumed valences and penalties
				DPConfig_ config = *match->first;

				bool do_insert = true;

				for (Size index = 0; index < num_valences; ++index)
				{
					config.consumed_valences[index] += right_entry.first.get().consumed_valences[index];
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

  //*****************************************************************************************************
	//*           ComputingData_                                                                          *
	//*****************************************************************************************************

	FPTBondOrderStrategy::ComputingData_::ComputingData_()
		: bond_assignments(), 
			molecule_graph(NULL), 
			bonds()
	{
	}

	FPTBondOrderStrategy::ComputingData_::~ComputingData_()
	{
		for (std::vector<FPTBondOrderAssignment_*>::iterator iter  = bond_assignments.begin(); 
		                                                     iter != bond_assignments.end(); ++iter)
		{
			delete *iter;
		}

		delete molecule_graph;
	}

  //*****************************************************************************************************
	//*           Assignment_                                                                             *
	//*****************************************************************************************************

	FPTBondOrderStrategy::Assignment_::Assignment_()
		: penalty(FPTBondOrderStrategy::infinite_penalty),
		  bonds_()
	{
	}

	FPTBondOrderStrategy::Assignment_::Assignment_(Size num_bonds)
		: penalty(FPTBondOrderStrategy::infinite_penalty),
		  bonds_(num_bonds, 0)
	{
	}

	FPTBondOrderStrategy::Assignment_::Assignment_(Assignment_ const& copy)
		: penalty(copy.penalty),
		  bonds_(copy.bonds_)
	{
	}

	FPTBondOrderStrategy::Assignment_::Assignment_(std::vector<BondOrder> const& bonds, Penalty penalty)
		: penalty(penalty),
		  bonds_(bonds)
	{
	}

	FPTBondOrderStrategy::Assignment_& FPTBondOrderStrategy::Assignment_::operator = (Assignment_ const& copy)
	{
		if (this != &copy)
		{
			bonds_  = copy.bonds_;
			penalty = copy.penalty;
		}

		return *this;
	}

	FPTBondOrderStrategy::BondOrder FPTBondOrderStrategy::Assignment_::operator [] (Size index) const
	{
		if (index < bonds_.size())
		{
			return bonds_[index];
		} 
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), static_cast<Size>(bonds_.size()));
		}
	}

	FPTBondOrderStrategy::BondOrder& FPTBondOrderStrategy::Assignment_::operator [] (Size index)
	{
		if (index < bonds_.size())
		{
			return bonds_[index];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), static_cast<Size>(bonds_.size()));
		}
	}

	std::vector<FPTBondOrderStrategy::BondOrder> const& FPTBondOrderStrategy::Assignment_::getBondOrders() const
	{
		return bonds_;
	}

	void FPTBondOrderStrategy::Assignment_::combine(Assignment_ const& other)
	{
		if (bonds_.size() != other.bonds_.size())
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "bond size differs");
		}

		for (Size i = 0; i < bonds_.size(); ++i)
		{
			bonds_[i] = std::max(bonds_[i], other.bonds_[i]);
		}

		penalty += other.penalty;
	}

	int FPTBondOrderStrategy::Assignment_::compare(Assignment_ const& s) const
	{
		int comp = Maths::compare(penalty, s.penalty);

		if (comp == 0)
		{
			if (bonds_.size() == s.bonds_.size())
			{
				for (Size i = 0; i < bonds_.size(); ++i)
				{
					if (bonds_[i] < s.bonds_[i])
					{
						return -1;
					} 
					else if (bonds_[i] > s.bonds_[i])
					{
						return 1;
					}
				}

				return 0;
			} 
			else
			{
				return (bonds_.size() > s.bonds_.size()) ? 1 : (bonds_.size() == s.bonds_.size() ? -1 : 0);
			}
		} 
		else
		{
			return comp;
		}
	}

	bool FPTBondOrderStrategy::Assignment_::operator < (Assignment_ const& a) const
	{
		return compare(a) < 0;
	}

	bool FPTBondOrderStrategy::Assignment_::operator > (Assignment_ const& a) const
	{
		return compare(a) > 0;
	}

	bool FPTBondOrderStrategy::Assignment_::operator <= (Assignment_ const& a) const
	{
		return compare(a) <= 0;
	}

	bool FPTBondOrderStrategy::Assignment_::operator >= (Assignment_ const& a) const
	{
		return compare(a) >= 0;
	}

	bool FPTBondOrderStrategy::Assignment_::operator == (Assignment_ const& a) const
	{
		return compare(a) == 0;
	}

	bool FPTBondOrderStrategy::Assignment_::isValid(MolecularGraph& graph, FPTBondOrderStrategy& parent)
	{
		float cpen = 0;

		Valence max_valence      = parent.max_valence;
		BondOrder max_bond_order = parent.abop->max_bond_order_;

		std::vector<Valence> valences(boost::num_vertices(graph)+1, 0);
		std::vector<MolecularGraphTraits::VertexType> sorted_neighbours;
		sorted_neighbours.reserve(10);

		std::vector<BondOrder>::const_iterator bond_iterator = bonds_.begin();
		BGL_FORALL_VERTICES(left, graph, MolecularGraph)
		{
			std::vector<MolecularGraphTraits::VertexType> neighbours;

			BGL_FORALL_ADJ(left, neighbour, graph, MolecularGraph)
			{
				neighbours.push_back(neighbour);
			}

			std::sort(neighbours.begin(), neighbours.end());

			for (std::vector<MolecularGraphTraits::VertexType>::const_iterator iter2  = neighbours.begin(); 
					                                                               iter2 != neighbours.end();   ++iter2)
			{
				MolecularGraphTraits::VertexType right = *iter2;

				if (left < right)
				{
					if (bond_iterator == bonds_.end())
					{
						throw Exception::NullPointer(__FILE__, __LINE__);
					}

					BondOrder value = *bond_iterator + 1;

					if (value > max_bond_order)
					{
						return false;
					}

					valences[left]  += value;
					valences[right] += value;

					++bond_iterator;
				}
			}
		}

		Size vertex_id = 0;
		for (std::vector<Valence>::const_iterator iter = valences.begin(); iter != valences.end(); ++iter, ++vertex_id)
		{
			Valence valence = *iter;

			if (valence < max_valence) 
			{
				cpen += parent.getPenaltyFor_(vertex_id, valence);

				if (cpen >= parent.infinite_penalty)
				{
					cpen = parent.infinite_penalty;

					break;
				}
			} 
			else
			{
				cpen = parent.infinite_penalty;

				break;
			}
		}

		return Maths::compare(cpen, penalty) == 0;
	}

  //*****************************************************************************************************
	//*           DPJoinMapComparator_                                                                    *
	//*****************************************************************************************************

	int FPTBondOrderStrategy::DPJoinMapComparator_::compare(DPConfig_ const* leftp, DPConfig_ const* rightp) const
	{
		DPConfig_ const& left  = *leftp;
		DPConfig_ const& right = *rightp;

		Size b_length = right.numberOfBonds();

		if (b_length != left.numberOfBonds())
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Can compare only entries from childs of the same join bag");
		}

		// compare just bond values
		for (Size i = 0; i < b_length; ++i)
		{
			BondOrder l =  left.bond_assignments[i];
			BondOrder r = right.bond_assignments[i];

			if (l < r)
				return -1;
			if (l > r)
				return 1;
		}

		return 0;
	}

	bool FPTBondOrderStrategy::DPJoinMapComparator_::operator() (DPConfig_ const* leftp,
	                                                             DPConfig_ const* rightp) const
	{
		return compare(leftp, rightp) < 0;
	}

  //*****************************************************************************************************
	//*           EdgeComparator_                                                                         *
	//*****************************************************************************************************

	bool FPTBondOrderStrategy::EdgeComparator_::operator() (FPTBondOrderStrategy::EdgeComparator_::Edge const& e1,
			                                                    FPTBondOrderStrategy::EdgeComparator_::Edge const& e2)
	{
		typedef FPTBondOrderStrategy::VertexType VertexType;

		VertexType left        = boost::source(e1, *graph_);
		VertexType right       = boost::target(e1, *graph_);

		if (left > right) std::swap(left, right);

		VertexType other_left  = boost::source(e2, *graph_);
		VertexType other_right = boost::target(e2, *graph_);

		if (other_left > other_right) std::swap(other_left, other_right);

		if (left < other_left)
			return true;
		if (left > other_left)
			return false;
		if (right < other_right)
			return true;
		if (right > other_right)
			return false;

		return false;
	}

  //*****************************************************************************************************
	//*           BackTrackingState_                                                                      *
	//*****************************************************************************************************

	FPTBondOrderStrategy::BackTrackingState_::BackTrackingState_() 
	  : assignment(), 
		  config(), 
			join_branches(), 
			index()
	{
	}

	FPTBondOrderStrategy::BackTrackingState_::BackTrackingState_(Size bonds) 
	  : assignment(bonds), 
		  config(), 
			join_branches(),
			index()
	{
	}

	FPTBondOrderStrategy::BackTrackingState_::BackTrackingState_(BackTrackingState_ const& other)
		: assignment(other.assignment), 
		  config(other.config), 
			join_branches(other.join_branches), 
			index(other.index)
	{
	}

	FPTBondOrderStrategy::BackTrackingState_& 
	FPTBondOrderStrategy::BackTrackingState_::operator=(BackTrackingState_ const& other)
	{
		if (this != &other)
		{
			assignment = other.assignment;
			config = other.config;
			join_branches = other.join_branches;
			index = other.index;
		}

		return *this;
	}

	int FPTBondOrderStrategy::BackTrackingState_::compare(BackTrackingState_ const& other) const
	{
		return Maths::compare(assignment.penalty, other.assignment.penalty);
	}

	bool FPTBondOrderStrategy::BackTrackingState_::operator < (BackTrackingState_ const& o) const
	{
		return compare(o) < 0;
	}

	bool FPTBondOrderStrategy::BackTrackingState_::operator > (BackTrackingState_ const& o) const
	{
		return compare(o) > 0;
	}

	bool FPTBondOrderStrategy::BackTrackingState_::operator <= (BackTrackingState_ const& o) const
	{
		return compare(o) <= 0;
	}

	bool FPTBondOrderStrategy::BackTrackingState_::operator >= (BackTrackingState_ const& o) const
	{
		return compare(o) >= 0;
	}

	bool FPTBondOrderStrategy::BackTrackingState_::operator == (BackTrackingState_ const& o) const
	{
		return compare(o) == 0;
	}

	bool FPTBondOrderStrategy::compareJoinTablePairs_(DPPairIt_ const& left, DPPairIt_ const& right)
	{
		return (left.first->second + left.second->second) < (right.first->second + right.second->second);
	}

	bool FPTBondOrderStrategy::compareTablePointerEntries_(DPPointerRow_ const& left, DPPointerRow_ const& right)
	{
		return left.second < right.second;
	}

  //*****************************************************************************************************
	//*           DPBackTracking_                                                                        *
	//*****************************************************************************************************

	FPTBondOrderStrategy::DPBackTracking_::DPBackTracking_(FPTBondOrderAssignment_& bond_assignment, Size max_num_solutions,
                                                         std::vector<MolecularGraphTraits::EdgeType> const& bonds, Penalty upper_bound)
		: bond_assignment_(&bond_assignment),
			current_state_(NULL),
			queue_(),
			max_num_solutions_(max_num_solutions),
			bonds_(&bonds),
			bags_(boost::shared_ptr<std::vector<TreeDecompositionBag> >(new std::vector<TreeDecompositionBag>)),
			max_heap_size_(1e6),
			num_computed_solutions_(0),
			upper_bound_(upper_bound)
	{
		// order bags in preorder
		if (bond_assignment.ntd_ == NULL)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		else
		{
			bags_->reserve(boost::num_vertices(bond_assignment.ntd_->_g));
			boost::traverse_tree(root(*bond_assignment.ntd_), *bond_assignment.ntd_, *this);
		}

		// init state
		DPTable_ const& table = getTable(boost::get(boost::vertex_index, *bond_assignment.ntd_, root(*bond_assignment.ntd_)));
		if (table.bestPenalty() < FPTBondOrderStrategy::infinite_penalty)
		{
			DPConstRow_ row = table.bestEntry();
			BackTrackingState_* state = new BackTrackingState_(bonds_->size());
			state->index = 0;
			state->config = row.first;
			state->assignment.penalty = row.second;
			queue_.insert(state);
		}
	}

	FPTBondOrderStrategy::DPBackTracking_::DPBackTracking_(DPBackTracking_ const& copy)
	 : bond_assignment_(copy.bond_assignment_),
		 current_state_(NULL),
		 queue_(),
		 max_num_solutions_(copy.max_num_solutions_),
		 bonds_(copy.bonds_),
		 bags_(copy.bags_),
		 max_heap_size_(copy.max_heap_size_),
		 num_computed_solutions_(copy.num_computed_solutions_),
		 upper_bound_(copy.upper_bound_)
	{
		if (copy.current_state_ != NULL)
		{
			current_state_ = new BackTrackingState_(*copy.current_state_);
		}

		std::multiset<BackTrackingState_*, StateComparator_>::const_iterator iter = copy.queue_.begin();
		for (; iter != copy.queue_.end(); ++iter)
		{
			queue_.insert(new BackTrackingState_(**iter));
		}
	}

	FPTBondOrderStrategy::DPBackTracking_& FPTBondOrderStrategy::DPBackTracking_::operator = (DPBackTracking_ const& copy)
	{
		if (this != &copy)
		{
			clear();

			bond_assignment_ = copy.bond_assignment_;
			current_state_   = copy.current_state_;

			queue_.clear();
			std::multiset<BackTrackingState_*, StateComparator_>::const_iterator iter = copy.queue_.begin();
			for (; iter != copy.queue_.end(); ++iter)
			{
				queue_.insert(new BackTrackingState_(**iter));
			}

			max_num_solutions_ = copy.max_num_solutions_;
			bonds_ = copy.bonds_;
			bags_ = copy.bags_;
			max_heap_size_ = copy.max_heap_size_;
			num_computed_solutions_ = copy.num_computed_solutions_;
			upper_bound_ = copy.upper_bound_;
		}

		return *this;
	}

	FPTBondOrderStrategy::DPBackTracking_::~DPBackTracking_()
	{
		clear();
	}

	void FPTBondOrderStrategy::DPBackTracking_::clear()
	{
		if (current_state_ != NULL)
		{
			delete current_state_;
		}

		std::vector<BackTrackingState_*> copy(queue_.begin(), queue_.end());
		queue_.clear();
		num_computed_solutions_ = 0;

		for (std::vector<BackTrackingState_*>::iterator iter = copy.begin(); iter != copy.end(); ++iter)
		{
			delete *iter;
		}
	}

	bool FPTBondOrderStrategy::DPBackTracking_::StateComparator_::operator() (BackTrackingState_ const * left,
	                                                                          BackTrackingState_ const * right) const
	{
		return *left < *right;
	}

	FPTBondOrderStrategy::DPTable_& FPTBondOrderStrategy::DPBackTracking_::getTable(Size order)
	{
		DPTable_* table = getProperties(order).table;

		if (table == NULL)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		return *table;
	}

	FPTBondOrderStrategy::AdditionalBagProperties_& FPTBondOrderStrategy::DPBackTracking_::getProperties(Size order)
	{
		std::vector<AdditionalBagProperties_>& properties = bond_assignment_->properties_;

		if (order < properties.size())
		{
			return properties[order];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(order), properties.size());
		}
	}

	FPTBondOrderStrategy::Assignment_& FPTBondOrderStrategy::DPBackTracking_::getSolution()
	{
		if (current_state_ == NULL)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		return current_state_->assignment;
	}

	FPTBondOrderStrategy::Assignment_ const& FPTBondOrderStrategy::DPBackTracking_::getSolution() const
	{
		if (current_state_ == NULL)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		return current_state_->assignment;
	}

	bool FPTBondOrderStrategy::DPBackTracking_::hasMoreSolutions() const
	{
		return (!queue_.empty() && (!max_num_solutions_ || (num_computed_solutions_ < max_num_solutions_)));
	}

	void FPTBondOrderStrategy::DPBackTracking_::nextSolution()
	{
		if (queue_.empty() || (max_heap_size_ == 0) || ((max_num_solutions_ > 0) && (num_computed_solutions_ > max_num_solutions_)))
		{
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}

		if (current_state_ != NULL)
		{
			delete current_state_;
		}

		std::multiset<BackTrackingState_*, StateComparator_>::iterator first = queue_.begin();
		current_state_ = *first;
		queue_.erase(first);
		++num_computed_solutions_;

		TreeDecomposition& tree = *bond_assignment_->ntd_;

		for (Size index = current_state_->index; index < bags_->size(); ++index)
		{
			TreeDecompositionBag& bag = (*bags_)[index];
			TreeDecomposition::children_iterator c_i, c_end;
			boost::tie(c_i, c_end) = children(bag, tree);

			switch (boost::get(boost::vertex_bag_type, tree, bag))
			{
				case TreeWidth<MolecularGraph>::ROOT_BAG:
					visitForget(*current_state_, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)));
					break;
				case TreeWidth<MolecularGraph>::INTRODUCE_BAG:
					visitIntroduce(*current_state_, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)));
					break;
				case TreeWidth<MolecularGraph>::LEAF_BAG:
					visitLeaf(*current_state_);
					break;
				case TreeWidth<MolecularGraph>::JOIN_BAG:
				{
					TreeDecomposition::children_iterator c_2 = c_i;
					++c_2;
					visitJoin(*current_state_, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)),
																				getTable(boost::get(boost::vertex_index, tree, *c_2)));
					break;
				}
				case TreeWidth<MolecularGraph>::FORGET_BAG:
					visitForget(*current_state_, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)));
					break;
				case TreeWidth<MolecularGraph>::END_BAG:
					if (boost::num_vertices(*(bond_assignment_->molecule_)) == 0)
					{
						// empty molecule -> you are finished
						return;
					} 
					else
					{
						// nice tree decomposition is damaged
						throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
					}
			}
		}
	}

	/**
	 * input: state with entry from introduce table
	 * bag: Introduce Bag
	 * table: table of child of the introduce bag
	 * wanted: all entries in the child table that could have generated the introduce entry
	 * -> simple in this case, since introduce nodes produce three introduce entries per child
	 * -> vice versa, this means that from any entry of the introduce table, we can get the child table entry
	 *    bag by erasing the new columns
	 */
	void FPTBondOrderStrategy::DPBackTracking_::visitIntroduce(BackTrackingState_& state, 
	                                                           TreeDecompositionBag& bag, DPTable_& table)
	{
		TreeDecomposition tree = *bond_assignment_->ntd_;

		TreeDecompositionContent content = boost::get(boost::vertex_bag_content, tree, bag);
		std::vector<MolecularGraphTraits::EdgeType>& bag_bonds = 
												getProperties(boost::get(boost::vertex_index, tree, bag)).bonds;

		MolecularGraphTraits::VertexType iv = boost::get(boost::vertex_bag_special, tree, bag);

		// entry of the given table
		DPConfig_& successor = state.config;
		// the entry of the previous table. We don't know it and want do find it
		DPConfig_ antecessor = table.begin()->first;

		// copy each bond value of bonds which were not introduced in this vertex
		Size j = 0;
		for (Size i = 0; i < bag_bonds.size(); ++i)
		{
			MolecularGraphTraits::VertexType source = boost::source(bag_bonds[i], *(bond_assignment_->molecule_));
			MolecularGraphTraits::VertexType target = boost::target(bag_bonds[i], *(bond_assignment_->molecule_));

			if (!(source == iv || target == iv))
			{
				antecessor.bond_assignments[j++] = successor.bond_assignments[i];
			}
		}

		// copy each consumed valences of vertices which were not introduced in this vertex
		j = 0;
		Size k = 0;
		for (TreeDecompositionContent::const_iterator iter = content.begin(); iter != content.end(); ++iter, ++k)
		{
			if (*iter != iv)
			{
				antecessor.consumed_valences[j++] = successor.consumed_valences[k];
			}
		}

		// the state is now the state of the previous table
		state.config = antecessor;
		++state.index;
	}

	/**
	 * input: state with entry from leaf node
	 * -> since leaves have no child, there is no antecessor
	 * -> in inorder-traversal, the next visited node must be the second child of a join node
	 * -> pop join_branch
	 */
	void FPTBondOrderStrategy::DPBackTracking_::visitLeaf(BackTrackingState_& state)
	{
		if (state.join_branches.empty())
		{
			// perfect, we are at the end of the backtracking
		} 
		else
		{
			{
				std::pair<DPConfig_, Size> const& right_child = state.join_branches.top();

				state.config = right_child.first;
				state.index  = right_child.second;
			}

			state.join_branches.pop();
		}
	}

	/**
	 * input: state with entry from join table
	 * bag: Join-Bag
	 * left_table, right_table: tables of children of the join node
	 * wanted: all pairs of child nodes that could have generated this entry
	 */
	void FPTBondOrderStrategy::DPBackTracking_::visitJoin(BackTrackingState_& state,
			                       TreeDecompositionBag& bag, DPTable_& left_table, DPTable_& right_table)
	{
		TreeDecomposition tree = *bond_assignment_->ntd_;

		std::vector<DPPairIt_> possible_antecessors;

		std::vector<DPTable_::const_iterator> left_entries;
		std::vector<DPTable_::const_iterator> right_entries;

		DPJoinMapComparator_ comp;
		DPConfig_ const& successor = state.config;

		TreeDecomposition::children_iterator c_first, c_second, c_end;
		boost::tie(c_first, c_end) = children(bag, tree);
		c_second = ++c_first;

		TreeDecompositionBag const& right_child = *c_second;

		// insert possible antecessors in vectors
		for (DPTable_::const_iterator iter = left_table.begin(); iter != left_table.end(); ++iter)
		{
			DPConstRow_ antecessor = convertToCRow(*iter);
			if (comp.compare(&successor, antecessor.first.get_pointer()) == 0)
			{
				left_entries.push_back(iter);
			}
		}

		for (DPTable_::const_iterator iter = right_table.begin(); iter != right_table.end(); ++iter)
		{
			DPConstRow_ antecessor = convertToCRow(*iter);
			if (comp.compare(&successor, antecessor.first.get_pointer()) == 0)
			{
				right_entries.push_back(iter);
			}
		}

		// now search possible pairs of antecessors from vectors
		Size n = successor.consumed_valences.size();

		for (std::vector<DPTable_::const_iterator>::const_iterator left = left_entries.begin(); left != left_entries.end(); ++left)
		{
			DPConstRow_ left_entry = convertToCRow(**left);
			for (std::vector<DPTable_::const_iterator>::const_iterator right = right_entries.begin(); right != right_entries.end(); ++right)
			{
				DPConstRow_ right_entry = convertToCRow(**right);

				// check sum of valences
				bool correct_valences = true;
				for (Size i = 0; i < n; ++i)
				{
					if (successor.consumed_valences[i]
							!= (left_entry.first.get().consumed_valences[i]
									+ right_entry.first.get().consumed_valences[i]))
					{
						correct_valences = false;
						break;
					}
				}

				if (correct_valences)
				{
					possible_antecessors.push_back(DPPairIt_(*left, *right));
				}
			}
		}

		sort(possible_antecessors.begin(), possible_antecessors.end(), FPTBondOrderStrategy::compareJoinTablePairs_);

		std::vector<DPPairIt_>::const_iterator iter = possible_antecessors.begin();

		// follow best left entry, add the right entry into join branch stack
		DPPairIt_ best = *iter;

		Penalty best_penalty = best.first->second + best.second->second;

		DPConstRow_ left  = convertToCRow(*best.first);
		DPConstRow_ right = convertToCRow(*best.second);

		extendState(state, left.first, 0);
		++state.index;

		// add the other solutions into queue
		for (++iter; iter != possible_antecessors.end(); ++iter)
		{
			DPPairIt_ entry = *iter;

			DPConstRow_ left  = convertToCRow(*entry.first);
			DPConstRow_ right = convertToCRow(*entry.second);

			Penalty add_penalty = (left.second + right.second) - best_penalty;
			if (isSolutionNeeded(state.assignment.penalty + add_penalty))
			{
				BackTrackingState_ newState = state;

				extendState(newState, left.first, add_penalty);
				branchState(newState, right_child, right.first);

				remember(newState);
			}
		}

		// branch current state

		branchState(state, right_child, right.first);
	}

	/**
	 * input: state with entry of forget table
	 * bag: Forget Bag
	 * table: table of child node of forget bag
	 * wanted: all entries in the child table, that could have generated the forget entry
	 * -> more difficulst. requires iteration over all entries of the child table and removing their forget nodes
	 * -> remove and test, whether this yields the wanted entry
	 */
	void FPTBondOrderStrategy::DPBackTracking_::visitForget(BackTrackingState_& state, TreeDecompositionBag& bag, DPTable_& table)
	{
		TreeDecomposition tree = *bond_assignment_->ntd_;

		std::vector<DPPointerRow_> possible_antecessors;

		TreeDecomposition::children_iterator c_i, c_end;
		boost::tie(c_i, c_end) = children(bag, tree);

		TreeDecompositionBag child = *c_i;

		TreeDecompositionContent child_vertices = boost::get(boost::vertex_bag_content, tree, child); 

		std::vector<MolecularGraphTraits::EdgeType>& child_bonds = getProperties(boost::get(boost::vertex_index, tree, child)).bonds;

		MolecularGraphTraits::VertexType fv = boost::get(boost::vertex_bag_special, tree, bag);

		Size forgotten_index = distance(child_vertices.begin(), std::find(child_vertices.begin(), child_vertices.end(), fv));
		DPConfig_ &successor = state.config;
		DPConfig_ test_entry = successor;

		// check for each row entry: is it a possible anteccessor?
		for (DPTable_::iterator iter = table.begin(); iter != table.end(); ++iter)
		{
			Penalty pen  = bond_assignment_->forgetInnerVertexIn(bag, convertToCRow(*iter), test_entry, child_bonds, forgotten_index);

			if (pen < FPTBondOrderStrategy::infinite_penalty && test_entry == successor)
			{
				possible_antecessors.push_back(DPPointerRow_(const_cast<DPConfig_*>(&(iter->first)), pen));
			}
		}

		// remember each found antecessors
		std::sort(possible_antecessors.begin(), possible_antecessors.end(), compareTablePointerEntries_);
		std::vector<DPPointerRow_>::iterator iter = possible_antecessors.begin();

		// follow this antecessor
		Penalty best_penalty = iter->second;
		{
			DPConfig_& antecessor = *iter->first;

			extendState(state, antecessor, 0);
			++state.index;

			setStateAssignment(state, child, antecessor, fv);
		}

		// add each other antecessors into the queue
		for (++iter; iter != possible_antecessors.end(); ++iter)
		{
			DPConfig_& antecessor = *iter->first;

			Penalty add_penalty = iter->second - best_penalty;
			if (isSolutionNeeded(state.assignment.penalty + add_penalty))
			{
				BackTrackingState_ new_state = state;

				extendState(new_state, antecessor, add_penalty);
				setStateAssignment(new_state, child, antecessor, fv);

				remember(new_state);
			} 
			else
			{
				return;
			}
		}
	}

	Size FPTBondOrderStrategy::DPBackTracking_::bondIndexFor(MolecularGraphTraits::EdgeType bond) const
	{
		FPTBondOrderStrategy::EdgeComparator_ ec(bond_assignment_->molecule_);
		Size d  = distance(bonds_->begin(), lower_bound(bonds_->begin(), bonds_->end(), bond, ec));

		if (d < bonds_->size())
		{
			return d;
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(d), bonds_->size());
		}
	}

	void FPTBondOrderStrategy::DPBackTracking_::remember(BackTrackingState_& state)
	{
		queue_.insert(new BackTrackingState_(state));
		if (queue_.size() >= max_heap_size_)
		{
			while (queue_.size() > max_heap_size_)
			{
				std::multiset<BackTrackingState_*, StateComparator_>::iterator pos = queue_.end();
				--pos;
				delete *pos;
				queue_.erase(pos);
			}

			upper_bound_ = Maths::min(upper_bound_, (*queue_.rbegin())->assignment.penalty);
		}
	}

	bool FPTBondOrderStrategy::DPBackTracking_::isSolutionNeeded(Penalty penalty)
	{
		if (max_heap_size_ == 0 || ((max_num_solutions_ > 0) && (num_computed_solutions_ > max_num_solutions_))) {return false;}
		if (queue_.size() >= max_heap_size_) {return Maths::isLess(penalty, upper_bound_);}
		return Maths::isLessOrEqual(penalty, upper_bound_);
	}

	void FPTBondOrderStrategy::DPBackTracking_::setStateAssignment(BackTrackingState_& state,
			TreeDecompositionBag& bag, DPConfig_& antecessor, MolecularGraphTraits::VertexType forgotten_vertex)
	{
		TreeDecomposition tree = *bond_assignment_->ntd_;

		std::vector<MolecularGraphTraits::EdgeType> bonds = getProperties(boost::get(boost::vertex_index, tree, bag)).bonds;
		std::vector<MolecularGraphTraits::EdgeType>::iterator begin = bonds.begin();

		for (std::vector<MolecularGraphTraits::EdgeType>::iterator iter = begin; iter != bonds.end(); ++iter)
		{
			MolecularGraphTraits::EdgeType bond = *iter;

			MolecularGraphTraits::VertexType source = boost::source(*iter, *(bond_assignment_->molecule_));
			MolecularGraphTraits::VertexType target = boost::target(*iter, *(bond_assignment_->molecule_));

			if (source == forgotten_vertex || target == forgotten_vertex)
			{
				Size bondIndex(bondIndexFor(bond));
				state.assignment[bondIndex] = antecessor.bond_assignments[std::distance(begin, iter)];
			}
		}
	}

	void FPTBondOrderStrategy::DPBackTracking_::extendState(BackTrackingState_& state, DPConfig_ const& antecessor, 
	                                                        Penalty additional_penalty)
	{
		state.config = antecessor;
		state.assignment.penalty += additional_penalty;
	}

	void FPTBondOrderStrategy::DPBackTracking_::branchState(BackTrackingState_& state, TreeDecompositionBag const& child, 
	                                                        DPConfig_ const& antecessor)
	{
		std::pair<DPConfig_, Size> right_state;

		right_state.first  = antecessor;
		right_state.second = distance(bags_->begin(), std::find(bags_->begin() + state.index, bags_->end(), child));

		state.join_branches.push(right_state);
	}

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::DPBackTracking_::penaltyOfNextSolution() const
	{
		if (queue_.empty() || max_heap_size_ == 0)
		{
			return FPTBondOrderStrategy::infinite_penalty;
		}
		else
		{
			return (*queue_.begin())->assignment.penalty;
		}
	}

  //*****************************************************************************************************
	//*           DPBackTrackingCombiner_                                                                 *
	//*****************************************************************************************************

	FPTBondOrderStrategy::DPBackTrackingCombiner_::DPBackTrackingCombiner_(DPBackTrackingCombiner_ const& copy)
		: backtrackers_(copy.deepCopyOfBacktrackers_()),
		  priority_queue_(copy.priority_queue_),
			component_solutions_(copy.component_solutions_),
			assignment_(copy.assignment_),
			solution_number_(copy.solution_number_),
			optimum_(copy.optimum_),
			upper_bound_(copy.upper_bound_)
	{
	}

	FPTBondOrderStrategy::DPBackTrackingCombiner_::DPBackTrackingCombiner_(std::vector<FPTBondOrderAssignment_*>& bond_assignments,
		                                                                     Size solution_number, Penalty upper_bound)
		: backtrackers_(),
		  priority_queue_(),
			component_solutions_(bond_assignments.size()),
			assignment_(),
			solution_number_(solution_number),
			optimum_(FPTBondOrderStrategy::infinite_penalty),
			upper_bound_(upper_bound)
	{
		if (bond_assignments.empty())
		{
			throw Exception::SizeUnderflow(__FILE__, __LINE__, 0);
		}

		backtrackers_.reserve(bond_assignments.size());

		MolecularGraph& graph = *bond_assignments[0]->molecule_;
		sorted_edges.reserve(boost::num_edges(graph));

		BGL_FORALL_EDGES(edge_it, graph, MolecularGraph)
		{
			sorted_edges.push_back(edge_it);
		}

		// sort bonds - the second vertex could be in false order
		FPTBondOrderStrategy::EdgeComparator_ ec(&graph);
		std::sort(sorted_edges.begin(), sorted_edges.end(), ec);

		for (std::vector<FPTBondOrderAssignment_*>::const_iterator iter  = bond_assignments.begin();
		                                                           iter != bond_assignments.end();   ++iter)
		{
			backtrackers_.push_back(new DPBackTracking_(**iter, solution_number_, sorted_edges, upper_bound_));
		}

		initialize_();
	}

	FPTBondOrderStrategy::DPBackTrackingCombiner_::~DPBackTrackingCombiner_()
	{
		clear();
	}

	void FPTBondOrderStrategy::DPBackTrackingCombiner_::clear()
	{
		for (std::vector<DPBackTracking_*>::iterator iter  = backtrackers_.begin();
		                                             iter != backtrackers_.end();   ++iter)
		{
			delete *iter;
		}
	}

	FPTBondOrderStrategy::DPBackTrackingCombiner_&
	FPTBondOrderStrategy::DPBackTrackingCombiner_::operator = (DPBackTrackingCombiner_ const& copy)
	{
		if (this != &copy)
		{
			clear();

			backtrackers_        = copy.deepCopyOfBacktrackers_();
			assignment_          = copy.assignment_;
			priority_queue_      = copy.priority_queue_;
			component_solutions_ = copy.component_solutions_;
			solution_number_     = copy.solution_number_;
			optimum_             = copy.optimum_;
			upper_bound_         = copy.upper_bound_;
		}

		return *this;
	}

	std::pair<Size, FPTBondOrderStrategy::Penalty>
	FPTBondOrderStrategy::DPBackTrackingCombiner_::getNextMinimumBackTracker_() const
	{
		if (backtrackers_.size() == 1)
		{
			return std::pair<Size, Penalty>(0, backtrackers_.front()->penaltyOfNextSolution());
		}

		typedef std::vector<DPBackTracking_*>::const_iterator iterator;

		iterator min = backtrackers_.begin();

		Penalty min_diff = FPTBondOrderStrategy::infinite_penalty;

		for (iterator iter = min; iter != backtrackers_.end(); ++iter)
		{
			DPBackTracking_ const& current = **iter;

			if (!current.hasMoreSolutions())
			{
				continue;
			}

			Size index = distance(backtrackers_.begin(), iter);

			Penalty current_diff = current.penaltyOfNextSolution() - component_solutions_[index].front().penalty + optimum_;

			if (current_diff < min_diff)
			{
				min_diff = current_diff;
				min = iter;
			}
		}

		return std::pair<Size, Penalty>(distance(backtrackers_.begin(), min), min_diff);
	}

	void FPTBondOrderStrategy::DPBackTrackingCombiner_::applyAssignment_(Size backtracker_index, Size solution_index)
	{
		Assignment_ const& ass = component_solutions_[backtracker_index][solution_index];
		assignment_.combine(ass);
	}

	bool FPTBondOrderStrategy::DPBackTrackingCombiner_::hasMoreSolutions() const
	{
		std::pair<Size, Penalty> next_min = getNextMinimumBackTracker_();

		return (backtrackers_[next_min.first]->hasMoreSolutions() && (!priority_queue_.empty() || (next_min.second) < upper_bound_));
	}

	void FPTBondOrderStrategy::DPBackTrackingCombiner_::nextSolution()
	{
		if (backtrackers_.size() == 1)
		{
			// if there is just one backtracker, it would be memory expensive to remember each solution.
			// so don't use the componentSolutions array and the priority queue. Instead return the solution directly.
			backtrackers_.front()->nextSolution();
			Assignment_ const& ass = backtrackers_.front()->getSolution();

			if (ass.penalty < upper_bound_)
			{
				assignment_ = ass;

				return;
			}
			else
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}
		}

		// compute next minimal Solution
		std::pair<Size, Penalty> next_min = getNextMinimumBackTracker_();

		if ((next_min.second < upper_bound_) && (priority_queue_.empty() || (next_min.second < priority_queue_.top().penalty)))
		{
			DPBackTracking_& min_back_tracker = *backtrackers_[next_min.first];

			min_back_tracker.nextSolution();

			component_solutions_[next_min.first].push_back(min_back_tracker.getSolution());

			// combine this new solutions with each other solutions
			combineEachSolution_(next_min.first);
		}

		if (!priority_queue_.empty())
		{
			assignment_ = priority_queue_.top();

			priority_queue_.pop();
		}
		else
		{
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}
	}

	void FPTBondOrderStrategy::DPBackTrackingCombiner_::combineEachSolution_(Size mindex)
	{
		std::vector<Size> indices(backtrackers_.size() - 1, 0);

		DPBackTracking_& min = *backtrackers_[mindex];

		Size i = 0;

		do
		{
			assignment_ = min.getSolution();

			for (Size j = 0; j < indices.size(); ++j)
			{
				Size k = j < mindex ? j : j + 1;

				applyAssignment_(k, indices[j]);
			}

			if (assignment_.penalty < upper_bound_)
			{
				priority_queue_.push(assignment_);
			}

			i = 0;
			while (i < indices.size() && ++indices[i] >= component_solutions_[(i < mindex ? i : i + 1)].size())
			{
				indices[i] = 0;
				++i;
			}

		} while (i < indices.size());
	}

	void FPTBondOrderStrategy::DPBackTrackingCombiner_::initialize_()
	{
		if (backtrackers_.size() <= 1)
		{
			return;
		}

		if (!backtrackers_[0]->hasMoreSolutions())
		{
			return;
		}

		backtrackers_[0]->nextSolution();
		component_solutions_[0].push_back(backtrackers_[0]->getSolution());
		assignment_ = backtrackers_[0]->getSolution();

		for (Size i = 1; i < backtrackers_.size(); ++i)
		{
			if (!backtrackers_[i]->hasMoreSolutions())
			{
				return;
			}

			backtrackers_[i]->nextSolution();
			component_solutions_[i].push_back(backtrackers_[i]->getSolution());
			assignment_.combine(backtrackers_[i]->getSolution());
		}

		priority_queue_.push(assignment_);
		optimum_ = assignment_.penalty;
	}

	FPTBondOrderStrategy::Penalty FPTBondOrderStrategy::DPBackTrackingCombiner_::penaltyOfNextSolution() const
	{
		Penalty next_pen = getNextMinimumBackTracker_().second;

		return (priority_queue_.empty() ? next_pen : Maths::min(next_pen, priority_queue_.top().penalty));
	}

	FPTBondOrderStrategy::Assignment_& FPTBondOrderStrategy::DPBackTrackingCombiner_::getSolution()
	{
		return assignment_;
	}

	FPTBondOrderStrategy::Assignment_ const& FPTBondOrderStrategy::DPBackTrackingCombiner_::getSolution() const
	{
		return assignment_;
	}

	std::vector<FPTBondOrderStrategy::DPBackTracking_*>
	FPTBondOrderStrategy::DPBackTrackingCombiner_::deepCopyOfBacktrackers_() const
	{
		std::vector<DPBackTracking_*> ts;
		ts.reserve(backtrackers_.size());

		for (std::vector<DPBackTracking_*>::const_iterator iter  = backtrackers_.begin();
				                                               iter != backtrackers_.end();   ++iter)
		{
			ts.push_back(new DPBackTracking_(**iter));
		}

		return ts;
	}

}
