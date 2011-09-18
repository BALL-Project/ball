#include <BALL/STRUCTURE/BONDORDERS/FPTBondOrderStrategy.h>

namespace BALL 
{
	namespace DPBondOrder 
	{

		FPTBondOrderStrategy::FPTBondOrderStrategy(AtomContainer& molecule, PenaltyMap const& penaltyMap,
		                                                     Size numberOfSolutions, Penalty upperbound) 
			: numberOfSolutions(numberOfSolutions), 
			  upperbound(upperbound),
				atomContainer(&molecule), 
				penaltyMap(&penaltyMap), 
				computed(false), 
				hasSolution(false), 
				combiner(NULL), 
				computingData()
		{
		}

		FPTBondOrderStrategy::FPTBondOrderStrategy(FPTBondOrderStrategy const& copy) 
			: numberOfSolutions(copy.numberOfSolutions), 
			  upperbound(copy.upperbound),
				atomContainer(copy.atomContainer), 
				penaltyMap(copy.penaltyMap), 
				computed(copy.computed),
				hasSolution(copy.hasSolution), 
				combiner(NULL), 
				computingData(copy.computingData)
		{
			if (copy.computed)
			{
				combiner = new DPBackTrackingCombiner(*copy.combiner);
			}
		}

		FPTBondOrderStrategy& FPTBondOrderStrategy::operator = (FPTBondOrderStrategy const& copy)
		{
			if (this != &copy)
			{
				reset();

				numberOfSolutions = copy.numberOfSolutions;
				upperbound = copy.upperbound;
				atomContainer = copy.atomContainer;
				penaltyMap = copy.penaltyMap;
				computed = copy.computed;
				hasSolution = copy.hasSolution;
				computingData = copy.computingData;

				if (computed)
				{
					combiner = new DPBackTrackingCombiner(*copy.combiner);
				}
			}

			return *this;
		}

		FPTBondOrderStrategy::ComputingData::ComputingData() 
			: bondAssignments(), 
			  connectionComponents(),
				moleculeGraph(NULL), 
				bonds()
		{
		}

		FPTBondOrderStrategy::~FPTBondOrderStrategy()
		{
			reset();
		}

		FPTBondOrderStrategy::ComputingData::~ComputingData()
		{
			for (std::vector<FPTBondOrder*>::iterator iter = bondAssignments.begin(); iter != bondAssignments.end(); ++iter)
			{
				delete *iter;
			}

			delete moleculeGraph;
		}

		void FPTBondOrderStrategy::setMaximumNumberOfSolutions(Size solutionNumber_)
		{
			reset();

			numberOfSolutions = solutionNumber_;
		}

		Size FPTBondOrderStrategy::getMaximumNumberOfSolutions() const
		{
			return numberOfSolutions;
		}

		void FPTBondOrderStrategy::setUpperbound(Penalty upperbound_)
		{
			reset();
			upperbound = upperbound_;
		}

		Penalty FPTBondOrderStrategy::getUpperbound() const
		{
			return upperbound;
		}

		void FPTBondOrderStrategy::setMolecule(AtomContainer& molecule)
		{
			reset();
			atomContainer = &molecule;
		}

		AtomContainer& FPTBondOrderStrategy::getMolecule()
		{
			if (atomContainer != NULL)
			{
				return *atomContainer;
			} else
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		void FPTBondOrderStrategy::setPenaltyMap(PenaltyMap const& penMap)
		{
			reset();

			penaltyMap = &penMap;
		}

		PenaltyMap const& FPTBondOrderStrategy::getPenaltyMap()
		{
			if (penaltyMap != NULL)
			{
				return *penaltyMap;
			} 
			else
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		bool FPTBondOrderStrategy::isValidAssignment()
		{
			shouldHaveSolution();

			return (*this)->isValid(*computingData->moleculeGraph, getPenaltyMap());
		}

		bool FPTBondOrderStrategy::isValidAssignment(Assignment& ref)
		{
			shouldBeStarted();

			return ref.isValid(*computingData->moleculeGraph, getPenaltyMap());
		}

		void FPTBondOrderStrategy::nextSolution()
		{
			shouldBeStarted();
			if (hasNextSolution())
			{
				combiner->nextSolution();
			} 
			else
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}
		}

		bool FPTBondOrderStrategy::hasNextSolution() const
		{
			shouldBeStarted();

			return combiner->hasMoreSolutions();
		}

		Penalty FPTBondOrderStrategy::penaltyOfNextSolution() const
		{
			shouldBeStarted();

			return combiner->penaltyOfNextSolution();
		}

		FPTBondOrderStrategy& FPTBondOrderStrategy::operator++()
		{
			shouldHaveSolution();
			if (hasNextSolution())
			{
				nextSolution();
			} 
			else
			{
				hasSolution = false;
			}

			return *this;
		}

		bool FPTBondOrderStrategy::operator+() const
		{
			shouldBeStarted();

			return hasSolution;
		}

		Assignment& FPTBondOrderStrategy::operator * ()
		{
			shouldHaveSolution();

			return combiner->getSolution();
		}

		Assignment * FPTBondOrderStrategy::operator -> ()
		{
			shouldHaveSolution();

			return &(combiner->getSolution());
		}

		HashMap<Bond const*, int > FPTBondOrderStrategy::getBondAssignmentHashMap () const
		{
			shouldHaveSolution();
			HashMap<Bond const*, int> hashmap;
			Assignment const& assignment (combiner->getSolution());
			Size numOfBonds (assignment.getBondOrders().size());
			vector<Bond const *> bonds (computingData->bonds);
			for (Position i(0); i < numOfBonds; ++i)
			{
				hashmap.insert(HashMap<Bond const*, int>::value_type(bonds[i], assignment[i]));
			}
			return hashmap;
		}

		vector<Bond const *> const& FPTBondOrderStrategy::getBonds() const
		{
			shouldBeStarted();
			return computingData->bonds;
		}


		void FPTBondOrderStrategy::computeBondAssignment()
		{
		}

		void FPTBondOrderStrategy::shouldBeStarted() const
		{
			if (!computed)
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		}

		void FPTBondOrderStrategy::shouldHaveSolution() const
		{
			shouldBeStarted();
			if (!hasSolution)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}
		}

		void FPTBondOrderStrategy::reset()
		{
			if (computed)
			{
				computingData.reset();
				delete combiner;
				combiner = NULL;
				computed = false;
			}
		}

		/*
		 * PenaltyMap
		 */
		PenaltyMap::PenaltyMap(std::vector<int> const& penalties_, std::vector<Position> const& block_to_start_idx_,
				std::vector<Size> const& block_to_length_, std::vector<int> const& block_to_start_valence_,
				std::vector<std::vector<int> > const & atom_to_block_, int maxBondOrder_, int maxValence_) 
			: penalties_(&penalties_), 
			  block_to_start_idx_(&block_to_start_idx_), 
				block_to_length_(&block_to_length_),
				block_to_start_valence_(&block_to_start_valence_), 
				atom_to_block_(&atom_to_block_),
				maxBondOrder_(static_cast<BondOrder>(maxBondOrder_)),
				maxValence_(static_cast<Valence>(maxValence_))
		{

		}

		BondOrder PenaltyMap::maxBondOrder() const
		{
			return maxBondOrder_;
		}

		Valence PenaltyMap::maxValence() const
		{
			return maxValence_;
		}

		PenaltyMap::~PenaltyMap()
		{

		}

		/*
		class PenaltyMapForDebugging : public PenaltyMap
		{
			public:
				PenaltyMapForDebugging();
				PenaltyMapForDebugging(PenaltyMapForDebugging const& copy);
				PenaltyMapForDebugging(Penalty fillWith);
				void setPenalty(vertexId atom, Valence value, Penalty penalty);
				Penalty getPenaltyFor(vertexId vertex, Valence valence) const;
			protected:
				vector< vector<float> > penalties;
		};

		PenaltyMapForDebugging::PenaltyMapForDebugging(MolecularGraph const& graph) :
				penalties((graph.maxVertexId()+1) * VALENCE_SIZE, INFINITE_PENALTY)
		{
		}
		PenaltyMapForDebugging::PenaltyMapForDebugging(MolecularGraph const& graph, Penalty fillWith) :
				penalties((graph.maxVertexId()+1) * VALENCE_SIZE, fillWith)
		{
		}
		PenaltyMapForDebugging::PenaltyMapForDebugging(PenaltyMapForDebugging const& copy) :
				penalties(copy.penalties)
		{
		}
		PenaltyMapForDebugging::PenaltyMapForDebugging(vector<Penalty> const& penalties) : penalties(penalties) {}

		Penalty& PenaltyMapForDebugging::penalty(MolecularGraphTraits::VertexType atom, Valence value)
		{
			Size index (atom * VALENCE_SIZE + value);
			if (index < penalties.size())
			{
				return penalties[index];
			} else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), penalties.size());
			}
		}

		Penalty PenaltyMapForDebugging::getPenaltyFor(vertexId vertex, Valence valence) const
		{
			Size index (vertex * VALENCE_SIZE + valence);
			if (index < penalties.size())
			{
				return penalties[index];
			} else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), penalties.size());
			}
		}

		BondOrder PenaltyMapForDebugging::maxBondOrder() const
		{
			return BOND_SIZE;
		}

		Valence PenaltyMapForDebugging::maxValence() const
		{
			return VALENCE_SIZE;
		}
		*/

		/**
		 * DPJoinMapComparator
		 */
		int DPJoinMapComparator::compare(DPConfig const* leftp, DPConfig const* rightp) const
		{
			DPConfig const& left(*leftp);
			DPConfig const& right(*rightp);
			Size blength(right.numberOfBonds());
			if (blength != left.numberOfBonds())
			{
				throw Exception::InvalidArgument(__FILE__, __LINE__, "Can compare only entries from childs of the same join bag");
			}
			// compare just bond values
			for (Size i(0); i < blength; ++i)
			{
				BondOrder l(left.bondAssignments[i]);
				BondOrder r(right.bondAssignments[i]);
				if (l < r)
					return -1;
				if (l > r)
					return 1;
			}
			return 0;
		}

		bool DPJoinMapComparator::operator()(DPConfig const* leftp,
				DPConfig const* rightp) const
		{
			return compare(leftp, rightp) < 0;
		}

		/*
		 * Assignment
		 */

		Assignment::Assignment() :
			bonds(), penalty(INFINITE_PENALTY)
		{
		}

		Assignment::Assignment(Size numOfBonds) :
			bonds(numOfBonds, 0), penalty(INFINITE_PENALTY)
		{
		}

		Assignment::Assignment(Assignment const& copy) :
			bonds(copy.bonds), penalty(copy.penalty)
		{
		}

		Assignment::Assignment(vector<BondOrder> bonds, Penalty penalty) :
			bonds(bonds), penalty(penalty)
		{
		}

		Assignment& Assignment::operator=(Assignment const& copy)
		{
			if (this != &copy)
			{
				bonds = copy.bonds;
				penalty = copy.penalty;
			}
			return *this;
		}
		BondOrder Assignment::operator [](Size index) const
		{
			if (index < bonds.size())
			{
				return bonds[index];
			} else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), static_cast<Size>(bonds.size()));
			}
		}
		BondOrder& Assignment::operator [](Size index)
		{
			if (index < bonds.size())
			{
				return bonds[index];
			} else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), static_cast<Size>(bonds.size()));
			}
		}
		vector<BondOrder> const& Assignment::getBondOrders() const
		{
			return bonds;
		}

		void Assignment::combine(Assignment const& other)
		{
			if(bonds.size() != other.bonds.size())
			{
				throw Exception::InvalidArgument(__FILE__, __LINE__, "bond size differs");
			}
			for (Size i(0); i < bonds.size(); ++i)
			{
				bonds[i] = max(bonds[i], other.bonds[i]);
			}
			penalty += other.penalty;
		}

		int Assignment::compare(Assignment const& s) const
		{
			int comp (Maths::compare(penalty, s.penalty));
			if (comp == 0)
			{
				if (bonds.size() == s.bonds.size())
				{
					for (Size i(0); i < bonds.size(); ++i)
					{
						if (bonds[i] < s.bonds[i]) {
							return -1;
						} else if (bonds[i] > s.bonds[i])
						{
							return 1;
						}
					}
					return 0;
				} else
					return (bonds.size() > s.bonds.size()) ? 1 : (bonds.size() == s.bonds.size() ? -1 : 0);
			} else
			{
				return comp;
			}
		}

		bool Assignment::operator < (Assignment const& a) const
		{
			return compare(a) < 0;
		}
		bool Assignment::operator > (Assignment const& a) const
		{
			return compare(a) > 0;
		}
		bool Assignment::operator <= (Assignment const& a) const
		{
			return compare(a) <= 0;
		}
		bool Assignment::operator >= (Assignment const& a) const
		{
			return compare(a) >= 0;
		}
		bool Assignment::operator == (Assignment const& a) const
		{
			return compare(a) == 0;
		}

		BackTrackingState::BackTrackingState() : assignment(), config(), joinBranchs(), index()
		{
		}
		BackTrackingState::BackTrackingState(Size bonds) : assignment(bonds), config(), joinBranchs(), index()
		{
		}
		BackTrackingState::BackTrackingState(BackTrackingState const& other) :
				assignment(other.assignment), config(other.config), joinBranchs(other.joinBranchs), index(other.index)
		{
		}
		BackTrackingState& BackTrackingState::operator=(BackTrackingState const& other)
		{
			if (this != &other)
			{
				assignment = other.assignment;
				config = other.config;
				joinBranchs = other.joinBranchs;
				index = other.index;
			}
			return *this;
		}

		int BackTrackingState::compare(BackTrackingState const& other) const
		{
			return Maths::compare(assignment.penalty, other.assignment.penalty);
		}
		bool BackTrackingState::operator < (BackTrackingState const& o) const
		{
			return compare(o) < 0;
		}
		bool BackTrackingState::operator > (BackTrackingState const& o) const
		{
			return compare(o) > 0;
		}
		bool BackTrackingState::operator <= (BackTrackingState const& o) const
		{
			return compare(o) <= 0;
		}
		bool BackTrackingState::operator >= (BackTrackingState const& o) const
		{
			return compare(o) >= 0;
		}
		bool BackTrackingState::operator == (BackTrackingState const& o) const
		{
			return compare(o) == 0;
		}

		bool compareJoinTablePairs(DPPairIt const& left, DPPairIt const& right)
		{
			return (left.first->second + left.second->second) < (right.first->second
					+ right.second->second);
		}

		bool compareTablePointerEntries(DPPointerRow const& left,
				DPPointerRow const& right)
		{
			return left.second < right.second;
		}

		/*
		 * DPBackTracking
		 */
		DPBackTracking::DPBackTracking(FPTBondOrder& bondAssignment, Size maxNumberOfSolutions, Penalty upperbound) 
			: bondAssignment(&bondAssignment), 
			  currentState(NULL), 
				queue(), 
				maxNumberOfSolutions(maxNumberOfSolutions), 
				bonds(),
				bags(), 
				maxHeapSize(maxNumberOfSolutions), 
				upperbound(upperbound)
		{
			// build a ordered list of bonds
			if (bondAssignment.molecule == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			MolecularGraph& graph = *bondAssignment.molecule;
			bonds.reserve(boost::num_edges(graph));

			BGL_FORALL_EDGES(edge_it, graph, MolecularGraph)
			{
				if (boost::source(edge_it, graph) < boost::target(edge_it, graph))
					bonds.push_back(edge_it);
			}

			// sort bonds - the second vertex could be in false order
			sort(bonds.begin(), bonds.end());
			// order bags in preorder
			if (bondAssignment.ntd == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			} 
			else
			{
				bags.reserve(boost::num_vertices(bondAssignment.ntd->_g));
				boost::traverse_tree(root(*bondAssignment.ntd), *bondAssignment.ntd, *this);
			}

			// init state
			DPTable const& table = getTable(boost::get(boost::vertex_index, *bondAssignment.ntd, root(*bondAssignment.ntd)));
			if (table.bestPenalty() < INFINITE_PENALTY)
			{
				DPConstRow row(table.bestEntry());
				BackTrackingState * state = new BackTrackingState(bonds.size());
				state->index = 0;
				state->config = row.first;
				state->assignment.penalty = row.second;
				queue.insert(state);
			}
		}

		DPBackTracking::DPBackTracking(DPBackTracking const& copy) 
		 : bondAssignment(copy.bondAssignment), 
		   currentState(NULL), 
			 queue(),
			 maxNumberOfSolutions(copy.maxNumberOfSolutions), 
			 bonds(copy.bonds), 
			 bags(copy.bags), 
			 maxHeapSize(copy.maxHeapSize),
			 upperbound(copy.upperbound)
		{
			if (copy.currentState != NULL)
			{
				currentState = new BackTrackingState(*copy.currentState);
			}
			for (multiset<BackTrackingState*>::const_iterator iter (copy.queue.begin()); iter != copy.queue.end(); ++iter)
			{
				queue.insert(new BackTrackingState(**iter));
			}
		}

		DPBackTracking& DPBackTracking::operator= (DPBackTracking const& copy)
		{
			if (this != &copy)
			{
				clear();
				bondAssignment = copy.bondAssignment;
				currentState = copy.currentState;
				queue.clear();
				for (multiset<BackTrackingState*>::const_iterator iter (copy.queue.begin()); iter != copy.queue.end(); ++iter)
				{
					queue.insert(new BackTrackingState(**iter));
				}
				maxNumberOfSolutions = copy.maxNumberOfSolutions;
				bonds = copy.bonds;
				bags = copy.bags;
				maxHeapSize = copy.maxHeapSize;
				upperbound = copy.upperbound;
			}
			return *this;
		}

		DPBackTracking::~DPBackTracking()
		{
			clear();
		}

		void DPBackTracking::clear()
		{
			if (currentState != NULL)
			{
				delete currentState;
			}
			vector<BackTrackingState*> copy (queue.begin(), queue.end());
			queue.clear();
			for (vector<BackTrackingState*>::iterator iter(copy.begin()); iter != copy.end(); ++iter)
			{
				delete *iter;
			}
		}

		bool DPBackTracking::StateComparator::operator()(BackTrackingState const * left, BackTrackingState const * right) const
		{
			return *left < *right;
		}

		DPTable& DPBackTracking::getTable(Size order)
		{
			DPTable * table (getProperties(order).table);
			if (table == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
			return *table;
		}

		AdditionalBagProperties& DPBackTracking::getProperties(Size order)
		{
			vector<AdditionalBagProperties>& properties = bondAssignment->properties;

			if (order < properties.size())
			{
				return properties[order];
			} else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(order), properties.size());
			}
		}

		Assignment& DPBackTracking::getSolution()
		{
			if (currentState == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
			return currentState->assignment;
		}

		Assignment const& DPBackTracking::getSolution() const
		{
			if (currentState == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
			return currentState->assignment;
		}

		bool DPBackTracking::hasMoreSolutions() const
		{
			return !queue.empty() && maxHeapSize > 0;
		}

		void DPBackTracking::nextSolution()
		{
			if (queue.empty() || maxHeapSize == 0)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}

			if (currentState != NULL)
			{
				delete currentState;
			}

			multiset<BackTrackingState*>::iterator first = queue.begin();
			currentState = *first;
			queue.erase(first);
			--maxHeapSize;

			TreeDecomposition& tree = *bondAssignment->ntd;

			for (Size index = currentState->index; index < bags.size(); ++index)
			{
				TreeDecompositionBag& bag = bags[index];
				TreeDecomposition::children_iterator c_i, c_end;
				boost::tie(c_i, c_end) = children(bag, tree);

				switch (boost::get(boost::vertex_bag_type, tree, bag))
				{
					case TreeWidth<MolecularGraph>::ROOT_BAG:
						visitForget(*currentState, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)));
						break;
					case TreeWidth<MolecularGraph>::INTRODUCE_BAG:
						visitIntroduce(*currentState, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)));
						break;
					case TreeWidth<MolecularGraph>::LEAF_BAG:
						visitLeaf(*currentState);
						break;
					case TreeWidth<MolecularGraph>::JOIN_BAG:
					{
						TreeDecomposition::children_iterator c_2 = c_i;
						++c_2;
						visitJoin(*currentState, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)), 
						                              getTable(boost::get(boost::vertex_index, tree, *c_2)));
						break;
					}
					case TreeWidth<MolecularGraph>::FORGET_BAG:
						visitForget(*currentState, bag, getTable(boost::get(boost::vertex_index, tree, *c_i)));
						break;
					case TreeWidth<MolecularGraph>::END_BAG:
						if (boost::num_vertices(*(bondAssignment->molecule)) == 0)
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
		 * geg: state mit Eintrag aus Introduce-Tabelle
		 * bag: Introduce Bag
		 * table: Tabelle des Kindknotens der IntroduceBag
		 * gesucht: Alle Einträge in der Kindtabelle, die den Introduce-Eintrag erzeugt haben können
		 * -> ist hier einfach, da Introduce-Nodes aus jedem Kind-Eintrag drei Introduce-Einträge machen
		 * -> Andersrum heißt das: Man kann aus einem beliebigen Eintrag der Introduce-Tabelle durch
		 * -> Löschen der neuen Spalten wieder den Eintrag der Kindtabelle bekommen
		 */
		void DPBackTracking::visitIntroduce(BackTrackingState& state, TreeDecompositionBag& bag, DPTable& table)
		{
			TreeDecomposition tree = *bondAssignment->ntd;

			TreeDecompositionContent content = boost::get(boost::vertex_bag_content, tree, bag);
			std::vector<MolecularGraphTraits::EdgeType>& bag_bonds = 
			                    getProperties(boost::get(boost::vertex_index, tree, bag)).bonds;

			MolecularGraphTraits::VertexType iv = boost::get(boost::vertex_bag_special, tree, bag);

			// entry of the given table
			DPConfig& successor = state.config;
			// the entry of the previous table. We don't know it and want do find it
			DPConfig antecessor = table.begin()->first;

			// copy each bond value of bonds which were not introduced in this vertex
			Size j = 0;
			for (Size i = 0; i < bag_bonds.size(); ++i)
			{
				MolecularGraphTraits::VertexType source = boost::source(bag_bonds[i], *(bondAssignment->molecule));
				MolecularGraphTraits::VertexType target = boost::target(bag_bonds[i], *(bondAssignment->molecule));

				if (!(source == iv || target == iv))
				{
					antecessor.bondAssignments[j++] = successor.bondAssignments[i];
				}
			}

			// copy each consumed valences of vertices which were not introduced in this vertex
			j = 0;
			Size k = 0;
			for (TreeDecompositionContent::const_iterator iter = content.begin(); iter != content.end(); ++iter, ++k)
			{
				if (*iter != iv)
				{
					antecessor.consumedValences[j++] = successor.consumedValences[k];
				}
			}
			// the state is now the state of the previous table
			state.config = antecessor;
			++state.index;
		}

		/**
		 * geg. state mit Eintrag aus Leaf-Node
		 * -> da Leafs keine Kinder haben, gibt es keinen Antecessor
		 * -> Im inorder-Reihenfolge müsste der nächste zu iterierende Knoten
		 * 		das zweite Kind eines Join-Knotens sein
		 * -> JoinBranch popen
		 */
		void DPBackTracking::visitLeaf(BackTrackingState& state)
		{
			if (state.joinBranchs.empty())
			{
				// perfect, we are at the end of the backtracking
			} else
			{
				{
					pair<DPConfig, Size> const& rightChild(state.joinBranchs.top());
					state.config = rightChild.first;
					state.index = rightChild.second;
				}
				state.joinBranchs.pop();
			}
		}

		/**
		 * geg: state mit Eintrag aus Join-Tabelle
		 * bag: Join-Bag
		 * leftTable, rightTable: Tabellen der Kinder des Joinknotens
		 * gesucht: Alle Paare von Kindknoten, die diesen Eintrag erzeugt haben können
		 */
		void DPBackTracking::visitJoin(BackTrackingState& state,
				TreeDecompositionBag& bag, DPTable& leftTable, DPTable& rightTable)
		{
			TreeDecomposition tree = *bondAssignment->ntd;

			vector<DPPairIt> possibleAntecessors;
			vector<DPTable::const_iterator> leftEntries;
			vector<DPTable::const_iterator> rightEntries;

			DPJoinMapComparator comp;
			DPConfig const& successor(state.config);

			TreeDecomposition::children_iterator c_first, c_second, c_end;
			boost::tie(c_first, c_end) = children(bag, tree);
			c_second = ++c_first;

			TreeDecompositionBag const& right_child = *c_second;

			// insert possible antecessors in vectors
			for (DPTable::const_iterator iter = leftTable.begin(); iter != leftTable.end(); ++iter)
			{
				DPConstRow antecessor = *iter;
				if (comp.compare(&successor, &(antecessor.first)) == 0)
				{
					leftEntries.push_back(iter);
				}
			}

			for (DPTable::const_iterator iter = rightTable.begin(); iter != rightTable.end(); ++iter)
			{
				DPConstRow antecessor = *iter;
				if (comp.compare(&successor, &(antecessor.first)) == 0)
				{
					rightEntries.push_back(iter);
				}
			}

			// now search possible pairs of antecessors from vectors
			Size n = successor.consumedValences.size();

			for (vector<DPTable::const_iterator>::const_iterator left = leftEntries.begin(); left != leftEntries.end(); ++left)
			{
				DPConstRow left_entry = **left;
				for (vector<DPTable::const_iterator>::const_iterator right = rightEntries.begin(); right != rightEntries.end(); ++right)
				{
					DPConstRow right_entry = **right;

					// check sum of valences
					bool correct_valences = true;
					for (Size i = 0; i < n; ++i)
					{
						if (successor.consumedValences[i]
								!= (left_entry.first.consumedValences[i]
										+ right_entry.first.consumedValences[i]))
						{
							correct_valences = false;
							break;
						}
					}

					if (correct_valences)
					{
						possibleAntecessors.push_back(DPPairIt(*left, *right));
					}
				}
			}

			sort(possibleAntecessors.begin(), possibleAntecessors.end(), compareJoinTablePairs);

			vector<DPPairIt>::const_iterator iter = possibleAntecessors.begin();

			// follow best left entry, add the right entry into join branch stack
			DPPairIt best = *iter;

			Penalty bestPenalty = best.first->second + best.second->second;

			DPConstRow left  = *best.first;
			DPConstRow right = *best.second;

			extendState(state, left.first, 0);
			++state.index;

			// add the other solutions into queue
			for (++iter; iter != possibleAntecessors.end(); ++iter)
			{
				DPPairIt entry = *iter;

				DPConstRow left  = *entry.first;
				DPConstRow right = *entry.second;

				Penalty add_penalty = (left.second + right.second) - bestPenalty;
				if (isSolutionNeeded(state.assignment.penalty + add_penalty))
				{
					BackTrackingState newState = state;
					extendState(newState, left.first, add_penalty);
					branchState(newState, right_child, right.first);
					remember(newState);
				}
			}

			// branch current state

			branchState(state, right_child, right.first);
		}

		/**
		 * geg: state mit Eintrag aus Forget-Tabelle
		 * bag: Forget Bag
		 * table: Tabelle des Kindknotens der ForgetBag
		 * gesucht: Alle Einträge in der Kindtabelle, die den Forget-Eintrag erzeugt haben können
		 * -> schon schwieriger. Man muss alle Einträge der Kindtabelle durchgehen, ihre Forget-Nodes
		 * -> löschen und prüfen, ob sie dann den gesuchten Eintrag ergeben.
		 */
		void DPBackTracking::visitForget(BackTrackingState& state, TreeDecompositionBag& bag, DPTable& table)
		{
			TreeDecomposition tree = *bondAssignment->ntd;

			vector<DPPointerRow> possibleAntecessors;

			TreeDecomposition::children_iterator c_i, c_end;
			boost::tie(c_i, c_end) = children(bag, tree);

			TreeDecompositionBag child = *c_i;

			TreeDecompositionContent child_vertices = boost::get(boost::vertex_bag_content, tree, bag); 

			std::vector<MolecularGraphTraits::EdgeType>& childBonds = getProperties(boost::get(boost::vertex_index, tree, child)).bonds;

			MolecularGraphTraits::VertexType fv = boost::get(boost::vertex_bag_special, tree, bag);

			Size forgotten_index = distance(child_vertices.begin(), std::find(child_vertices.begin(), child_vertices.end(), fv));
			DPConfig & successor = state.config;
			DPConfig testEntry = successor;

			// check for each row entry: is it a possible anteccessor?
			for (DPTable::iterator iter = table.begin(); iter != table.end(); ++iter)
			{
				Penalty pen  = bondAssignment->forgetInnerVertexIn(bag, *iter, testEntry, childBonds, forgotten_index);

				if (pen < INFINITE_PENALTY && testEntry == successor)
				{
					possibleAntecessors.push_back(DPPointerRow(const_cast<DPConfig*>(&(iter->first)), pen));
				}
			}

			// remember each found antecessors
			sort(possibleAntecessors.begin(), possibleAntecessors.end(), compareTablePointerEntries);
			vector<DPPointerRow>::iterator iter = possibleAntecessors.begin();

			// follow this antecessor
			Penalty bestPenalty = iter->second;
			{
				DPConfig& antecessor = *iter->first;

				extendState(state, antecessor, 0);
				++state.index;

				setStateAssignment(state, child, antecessor, fv);
			}

			// add each other antecessors into the queue
			for (++iter; iter != possibleAntecessors.end(); ++iter)
			{
				DPConfig& antecessor = *iter->first;

				Penalty add_penalty = iter->second - bestPenalty;
				if (isSolutionNeeded(state.assignment.penalty + add_penalty))
				{
					BackTrackingState newState = state;
					extendState(newState, antecessor, add_penalty);
					setStateAssignment(newState, child, antecessor, fv);
					remember(newState);
				} 
				else
				{
					return;
				}
			}
		}

		Size DPBackTracking::bondIndexFor(MolecularGraphTraits::EdgeType bond) const
		{
			Size d  = distance(bonds.begin(), lower_bound(bonds.begin(), bonds.end(), bond));
			if (d < bonds.size())
			{
				return d;
			} 
			else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(d), bonds.size());
			}
		}

		void DPBackTracking::remember(BackTrackingState& state)
		{
			queue.insert(new BackTrackingState(state));
			if (queue.size() >= maxHeapSize)
			{

				while (queue.size() > maxHeapSize)
				{
					multiset<BackTrackingState*, StateComparator>::iterator pos (queue.end());
					--pos;
					delete *pos;
					queue.erase(pos);
				}

				upperbound = Maths::min(upperbound, (*queue.rbegin())->assignment.penalty);
			}
		}

		bool DPBackTracking::isSolutionNeeded(Penalty penalty)
		{
			if (maxHeapSize == 0) {return false;}
			if (queue.size() >= maxHeapSize) {return Maths::isLess(penalty, upperbound);}
			return Maths::isLessOrEqual(penalty, upperbound);
		}

		void DPBackTracking::setStateAssignment(BackTrackingState& state,
				TreeDecompositionBag& bag, DPConfig& antecessor, MolecularGraphTraits::VertexType forgottenVertex)
		{
			TreeDecomposition tree = *bondAssignment->ntd;

			std::vector<MolecularGraphTraits::EdgeType> bonds = getProperties(boost::get(boost::vertex_index, tree, bag)).bonds;
			std::vector<MolecularGraphTraits::EdgeType>::iterator begin =bonds.begin();

			for (std::vector<MolecularGraphTraits::EdgeType>::iterator iter = begin; iter != bonds.end(); ++iter)
			{
				MolecularGraphTraits::EdgeType bond = *iter;

				MolecularGraphTraits::VertexType source = boost::source(*iter, *(bondAssignment->molecule));
				MolecularGraphTraits::VertexType target = boost::target(*iter, *(bondAssignment->molecule));

				if (source == forgottenVertex || target == forgottenVertex)
				{
					Size bondIndex(bondIndexFor(bond));
					state.assignment[bondIndex] = antecessor.bondAssignments[std::distance(begin, iter)];
				}
			}
		}

		void DPBackTracking::extendState(BackTrackingState& state, DPConfig const& antecessor, Penalty additionalPenalty)
		{
			state.config = antecessor;
			state.assignment.penalty += additionalPenalty;
		}

		void DPBackTracking::branchState(BackTrackingState& state,
				TreeDecompositionBag const& child, DPConfig const& antecessor)
		{
			pair<DPConfig, Size> rightState;
			rightState.first = antecessor;
			rightState.second = distance(bags.begin(), std::find(bags.begin() + state.index, bags.end(), child));
			state.joinBranchs.push(rightState);
		}

		Penalty DPBackTracking::penaltyOfNextSolution() const
		{
			if (queue.empty() || maxHeapSize == 0)
			{
				return INFINITE_PENALTY;
			} else
			{
				return (*queue.begin())->assignment.penalty;
			}
		}

		DPBackTrackingCombiner::DPBackTrackingCombiner(DPBackTrackingCombiner const& copy) 
			: backtrackers(copy.deepCopyOfBacktrackers()), 
			  priorityQueue(copy.priorityQueue),
				componentSolutions(copy.componentSolutions), 
				assignment(copy.assignment),
				solutionNumber(copy.solutionNumber), 
				optimum(copy.optimum), 
				upperbound(copy.upperbound)
		{
		}

		DPBackTrackingCombiner::DPBackTrackingCombiner(vector<FPTBondOrder*>& bondAssignments, 
		                                               Size solutionNumber, Penalty upperbound) 
			: backtrackers(), 
				priorityQueue(), 
				componentSolutions(bondAssignments.size()),
				assignment(), 
				solutionNumber(solutionNumber), 
				optimum(INFINITE_PENALTY), 
				upperbound(upperbound)
		{
			if (bondAssignments.empty())
			{
				throw Exception::SizeUnderflow(__FILE__, __LINE__, 0);
			}
			backtrackers.reserve(bondAssignments.size());
			for (vector<FPTBondOrder*>::const_iterator iter(bondAssignments.begin()); iter
					!= bondAssignments.end(); ++iter)
			{
				backtrackers.push_back(new DPBackTracking(**iter, solutionNumber, upperbound));
			}
			initialize();
		}

		DPBackTrackingCombiner::DPBackTrackingCombiner(vector<FPTBondOrder>& bondAssignments, 
		                                               Size solutionNumber, Penalty upperbound) 
			: backtrackers(), 
			  priorityQueue(), 
				componentSolutions(bondAssignments.size()),
				assignment(), 
				solutionNumber(solutionNumber), 
				optimum(INFINITE_PENALTY), 
				upperbound(upperbound)
		{
			backtrackers.reserve(bondAssignments.size());
			for (vector<FPTBondOrder>::iterator iter(bondAssignments.begin()); iter
					!= bondAssignments.end(); ++iter)
			{
				backtrackers.push_back(new DPBackTracking(*iter, solutionNumber, upperbound));
			}
			initialize();
		}

		DPBackTrackingCombiner::~DPBackTrackingCombiner()
		{
			for(vector<DPBackTracking*>::iterator iter(backtrackers.begin()); iter != backtrackers.end(); ++iter)
			{
				delete *iter;
			}
		}

		DPBackTrackingCombiner& DPBackTrackingCombiner::operator=(
				DPBackTrackingCombiner const& copy)
		{
			if (this != &copy)
			{
				this->~DPBackTrackingCombiner();
				backtrackers = copy.deepCopyOfBacktrackers();
				assignment = copy.assignment;
				priorityQueue = copy.priorityQueue;
				componentSolutions = copy.componentSolutions;
				solutionNumber = copy.solutionNumber;
				optimum = copy.optimum;
				upperbound = copy.upperbound;
			}
			return *this;
		}

		pair<Size, Penalty> DPBackTrackingCombiner::getNextMinimumBackTracker() const
		{
			if (backtrackers.size() == 1)
			{
				return pair<Size, Penalty>(0, backtrackers.front()->penaltyOfNextSolution());
			}
			typedef vector<DPBackTracking*>::const_iterator iterator;
			iterator min(backtrackers.begin());
			Penalty minDiff(INFINITE_PENALTY);
			for (iterator iter(min); iter != backtrackers.end(); ++iter)
			{
				DPBackTracking const& current(**iter);
				if (!current.hasMoreSolutions())
				{
					continue;
				}
				Size index (distance(backtrackers.begin(), iter));
				Penalty currentDiff(current.penaltyOfNextSolution()
						- componentSolutions[index].front().penalty + optimum);
				if (currentDiff < minDiff)
				{
					minDiff = currentDiff;
					min = iter;
				}
			}
			return pair<Size, Penalty>(distance(backtrackers.begin(), min), minDiff);
		}

		void DPBackTrackingCombiner::applyAssignment(Size backtracker_index,
				Size solutionIndex)
		{
			Assignment const& ass(componentSolutions.at(backtracker_index).at(solutionIndex));
			assignment.combine(ass);
		}

		bool DPBackTrackingCombiner::hasMoreSolutions() const
		{
			return !priorityQueue.empty() || getNextMinimumBackTracker().second < upperbound;
		}

		void DPBackTrackingCombiner::nextSolution()
		{
			if (backtrackers.size() == 1)
			{
				// if there is just one backtracker, it would be memory expensive to remember each solution.
				// so don't use the componentSolutions array and the priority queue. Instead return the solution directly.
				backtrackers.front()->nextSolution();
				Assignment const& ass (backtrackers.front()->getSolution());
				if (ass.penalty < upperbound)
				{
					assignment = ass;
					return;
				} else
				{
					throw Exception::OutOfRange(__FILE__, __LINE__);
				}
			}
			// compute next minimal Solution
			pair<Size, Penalty> nextMin (getNextMinimumBackTracker());
			if (nextMin.second < upperbound && (priorityQueue.empty() || nextMin.second < priorityQueue.top().penalty))
			{
				DPBackTracking& minBackTracker (*backtrackers[nextMin.first]);
				minBackTracker.nextSolution();
				componentSolutions[nextMin.first].push_back(minBackTracker.getSolution());
				// combine this new solutions with each other solutions
				combineEachSolution(nextMin.first);
			}
			if (!priorityQueue.empty())
			{
				assignment = priorityQueue.top();
				priorityQueue.pop();
			} else
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}
		}

		void DPBackTrackingCombiner::combineEachSolution(Size mindex)
		{
			vector<Size> indizes(backtrackers.size() - 1, 0);
			DPBackTracking& min(*backtrackers[mindex]);
			Size i(0);
			do
			{
				assignment = min.getSolution();
				for (Size j(0); j < indizes.size(); ++j)
				{
					Size k(j < mindex ? j : j + 1);
					applyAssignment(k, indizes[j]);
				}
				if (assignment.penalty < upperbound)
				{
					priorityQueue.push(assignment);
				}
				i = 0;
				while (i < indizes.size() && ++indizes[i] >= componentSolutions[(i < mindex ? i
								: i + 1)].size())
				{
					indizes[i] = 0;
					++i;
				}
			} while (i < indizes.size());
		}

		void DPBackTrackingCombiner::initialize()
		{
			if (backtrackers.size() <= 1)
			{
				return;
			}
			if (!backtrackers.at(0)->hasMoreSolutions())
			{
				return;
			}
			backtrackers.at(0)->nextSolution();
			componentSolutions[0].push_back(backtrackers[0]->getSolution());
			assignment = backtrackers[0]->getSolution();
			for (Size i(1); i < backtrackers.size(); ++i)
			{
				if (!backtrackers[i]->hasMoreSolutions())
				{
					return;
				}
				backtrackers[i]->nextSolution();
				componentSolutions[i].push_back(backtrackers[i]->getSolution());
				assignment.combine(backtrackers[i]->getSolution());
			}
			priorityQueue.push(assignment);
			optimum = assignment.penalty;
		}

		Penalty DPBackTrackingCombiner::penaltyOfNextSolution() const
		{
			Penalty nextPen (getNextMinimumBackTracker().second);
			return (priorityQueue.empty() ? nextPen : Maths::min(nextPen, priorityQueue.top().penalty));
		}

		Assignment& DPBackTrackingCombiner::getSolution()
		{
			return assignment;
		}

		Assignment const& DPBackTrackingCombiner::getSolution() const
		{
			return assignment;
		}

		vector<DPBackTracking*> DPBackTrackingCombiner::deepCopyOfBacktrackers() const
		{
			vector<DPBackTracking*> ts;
			ts.reserve(backtrackers.size());
			for (vector<DPBackTracking*>::const_iterator iter(backtrackers.begin());
					iter != backtrackers.end(); ++iter)
			{
				ts.push_back(new DPBackTracking(**iter));
			}
			return ts;
		}

		bool Assignment::isValid(MolecularGraph& graph, PenaltyMap const& penalties)
		{
			float cpen = 0;

			Valence maxValence (penalties.maxValence());
			BondOrder maxBondOrder (penalties.maxBondOrder());
			vector<Valence> valences (boost::num_vertices(graph)+1, 0);
			vector<MolecularGraphTraits::VertexType> sortedNeighbours;
			sortedNeighbours.reserve(10);

			vector<BondOrder>::const_iterator bondIterator = bonds.begin();
			BGL_FORALL_VERTICES(left, graph, MolecularGraph)
			{
				vector<MolecularGraphTraits::VertexType> neighbours;

				BGL_FORALL_ADJ(left, neighbour, graph, MolecularGraph)
				{
					neighbours.push_back(neighbour);
				}

				std::sort(neighbours.begin(), neighbours.end());

				for (vector<MolecularGraphTraits::VertexType>::const_iterator iter2  = neighbours.begin(); 
						                                                          iter2 != neighbours.end();   ++iter2)
				{
					MolecularGraphTraits::VertexType right = *iter2;

					if (left < right)
					{
						if (bondIterator == bonds.end())
						{
							throw Exception::NullPointer(__FILE__, __LINE__);
						}

						BondOrder value = *bondIterator + 1;
						if (value > maxBondOrder)
						{
							return false;
						}

						valences[left]  += value;
						valences[right] += value;

						++bondIterator;
					}
				}
			}

			Size vertexId = 0;
			for (vector<Valence>::const_iterator iter = valences.begin(); iter != valences.end(); ++iter, ++vertexId)
			{
				Valence valence (*iter);

				if (valence < maxValence) {
					cpen += penalties.getPenaltyFor(vertexId, valence);
					if (cpen >= INFINITE_PENALTY)
					{
						cpen = INFINITE_PENALTY;
						break;
					}
				} else
				{
					cpen = INFINITE_PENALTY;
					break;
				}
			}
			return Maths::compare(cpen, penalty) == 0;
		}


	}
}
