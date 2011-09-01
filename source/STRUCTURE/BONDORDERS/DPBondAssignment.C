#include <BALL/STRUCTURE/BONDORDERS/DPBondAssignment.h>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/copy.hpp>

namespace BALL 
{
	namespace DPBondOrder 
	{

		/*
		 * DPBondAssignmentAlgorithm
		 */
		DPBondAssignmentAlgorithm::DPBondAssignmentAlgorithm() 
			: numberOfSolutions(MAX_SOLUTION_NUMBER), 
			  upperbound(INFINITE_PENALTY),
				atomContainer(NULL), 
				penaltyMap(NULL), 
				computed(false), 
				hasSolution(false), 
				combiner(NULL), 
				computingData()
		{
		}

		DPBondAssignmentAlgorithm::DPBondAssignmentAlgorithm(AtomContainer& molecule, PenaltyMap& penaltyMap,
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

		DPBondAssignmentAlgorithm::DPBondAssignmentAlgorithm(DPBondAssignmentAlgorithm const& copy) 
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

		DPBondAssignmentAlgorithm& DPBondAssignmentAlgorithm::operator = (DPBondAssignmentAlgorithm const& copy)
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

		DPBondAssignmentAlgorithm::ComputingData::ComputingData() 
			: bondAssignments(), 
			  connectionComponents(),
				moleculeGraph(NULL), 
				niceTreeDecompositions(), 
				bonds()
		{
		}

		DPBondAssignmentAlgorithm::~DPBondAssignmentAlgorithm()
		{
			reset();
		}

		DPBondAssignmentAlgorithm::ComputingData::~ComputingData()
		{
			for (std::vector<DPBondAssignment*>::iterator iter = bondAssignments.begin(); iter != bondAssignments.end(); ++iter)
			{
				delete *iter;
			}

			for (std::vector<DPBondAssignment::NiceTreeDecomposition*>::iterator 
				  iter = niceTreeDecompositions.begin();
					iter != niceTreeDecompositions.end(); ++iter)
			{
				delete *iter;
			}

			delete moleculeGraph;
		}

		void DPBondAssignmentAlgorithm::setMaximumNumberOfSolutions(Size solutionNumber_)
		{
			reset();

			numberOfSolutions = solutionNumber_;
		}

		Size DPBondAssignmentAlgorithm::getMaximumNumberOfSolutions() const
		{
			return numberOfSolutions;
		}

		void DPBondAssignmentAlgorithm::setUpperbound(Penalty upperbound_)
		{
			reset();
			upperbound = upperbound_;
		}

		Penalty DPBondAssignmentAlgorithm::getUpperbound() const
		{
			return upperbound;
		}

		void DPBondAssignmentAlgorithm::setMolecule(AtomContainer& molecule)
		{
			reset();
			atomContainer = &molecule;
		}

		AtomContainer& DPBondAssignmentAlgorithm::getMolecule()
		{
			if (atomContainer != NULL)
			{
				return *atomContainer;
			} else
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		void DPBondAssignmentAlgorithm::setPenaltyMap(PenaltyMap& penMap)
		{
			reset();

			penaltyMap = &penMap;
		}

		PenaltyMap& DPBondAssignmentAlgorithm::getPenaltyMap()
		{
			if (penaltyMap != NULL)
			{
				return *penaltyMap;
			} else
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		void DPBondAssignmentAlgorithm::start()
		{
			reset();

			// check if options are set
			if (penaltyMap == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			if (atomContainer == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			computingData.reset( new ComputingData() );

			buildGraph();

			// build NiceTreeDecomposition
			buildNiceTreeDecomposition();

			// compute BondAssignments
			computeBondAssignment();

			// initialize backtracking
			vector<DPBondAssignment*> & bondAssignments = computingData->bondAssignments;

			combiner = new DPBackTrackingCombiner(bondAssignments, numberOfSolutions, upperbound);
			if (combiner->hasMoreSolutions())
			{
				hasSolution = true;
				combiner->nextSolution();
			} 
			else
			{
				hasSolution = false;
			}

			// now this "iterator" becomes valid
			computed = true;
		}

		bool DPBondAssignmentAlgorithm::isValidAssignment()
		{
			shouldHaveSolution();

			return (*this)->isValid(*computingData->moleculeGraph, getPenaltyMap());
		}

		bool DPBondAssignmentAlgorithm::isValidAssignment(Assignment const& ref)
		{
			shouldBeStarted();

			return ref.isValid(*computingData->moleculeGraph, getPenaltyMap());
		}

		void DPBondAssignmentAlgorithm::nextSolution()
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

		bool DPBondAssignmentAlgorithm::hasNextSolution() const
		{
			shouldBeStarted();

			return combiner->hasMoreSolutions();
		}

		Penalty DPBondAssignmentAlgorithm::penaltyOfNextSolution() const
		{
			shouldBeStarted();

			return combiner->penaltyOfNextSolution();
		}

		DPBondAssignmentAlgorithm& DPBondAssignmentAlgorithm::operator++()
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

		bool DPBondAssignmentAlgorithm::operator+() const
		{
			shouldBeStarted();

			return hasSolution;
		}

		Assignment const& DPBondAssignmentAlgorithm::operator * () const
		{
			shouldHaveSolution();

			return combiner->getSolution();
		}

		Assignment const * DPBondAssignmentAlgorithm::operator -> () const
		{
			shouldHaveSolution();

			return &(combiner->getSolution());
		}

		HashMap<Bond const*, int > DPBondAssignmentAlgorithm::getBondAssignmentHashMap () const
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

		void DPBondAssignmentAlgorithm::buildGraph()
		{
			// build graph from molecule
			// TODO: do we really need to pointer this? let's at least use boost::shared_ptr
			MolecularGraph* mol_graph = new MolecularGraph(*const_cast<AtomContainer*>(atomContainer));
			computingData->moleculeGraph = mol_graph;

			// TODO: do we need to sort the bonds in a particular way?
		}

		void DPBondAssignmentAlgorithm::buildNiceTreeDecomposition()
		{
			MolecularGraph & mol_graph = *computingData->moleculeGraph;

			std::vector<int> components(num_vertices(mol_graph));
			int num_components = boost::connected_components(mol_graph, &components[0]);

			std::vector<NiceTreeDecomposition*> & ntds = computingData->niceTreeDecompositions;
			ntds.reserve(num_components);


			// build individual graphs out of the components
			std::vector<MolecularGraph> subgraphs;
			subgraphs.reserve(num_components);

			for (int i=0; i<num_components; ++i)
			{
				subgraphs[i] = MolecularGraph(mol_graph);

				std::list<VertexType> to_remove;
				boost::property_map<MolecularGraph, boost::vertex_index_t>::type vertex_index = boost::get(boost::vertex_index, subgraphs[i]);

				BGL_FORALL_VERTICES(current_vertex, subgraphs[i], MolecularGraph)
				{
					if (components[vertex_index[current_vertex]] != i)
						to_remove.push_back(current_vertex);
				}

				for (std::list<VertexType>::iterator li = to_remove.begin(); li != to_remove.end(); ++li)
				{
					boost::clear_vertex(*li, subgraphs[i]);
					boost::remove_vertex(*li, subgraphs[i]);
				}
			}

			for (int i=0; i<num_components; ++i)
			{
				MolecularGraph& component = subgraphs[i];

				TreeDecomposition treeDecomposition;

				// Quick-Bound-And-Branch algorithm to compute a perfect elimination order
				QuickBB<MolecularGraph> quickBB (component);

				TreeWidthTraits<MolecularGraph>::EliminationOrder permutation(quickBB.compute());
				// use this elimination order to build an optimal tree decomposition
				eliminationOrderToTreeDecomposition(mol_graph, permutation, treeDecomposition);
				// transform this tree decomposition into a nice tree decomposition
				ntds.push_back(new NiceTreeDecomposition());

				NiceTreeDecompositionBuilder<MolecularGraph> builder (*ntds[i]);
				builder.buildNiceTreeDecomposition(*treeDecomposition.getRoot());
			}
			// for each connection component in the molecule graph
		}

		void DPBondAssignmentAlgorithm::computeBondAssignment()
		{
			MolecularGraph const& mol_graph = *computingData->moleculeGraph;

			std::vector<NiceTreeDecomposition*> & ntds = computingData->niceTreeDecompositions;
			std::vector<DPBondAssignment*> & bondAssignments = computingData->bondAssignments;

			Size connectionComponents = computingData->connectionComponents.size();
			bondAssignments.reserve(connectionComponents);
			for (Position i(0); i < connectionComponents; ++i)
			{
				bondAssignments.push_back(new DPBondAssignment(mol_graph, *penaltyMap, *ntds[i], upperbound));
				bondAssignments[i]->compute();
			}
		}

		void DPBondAssignmentAlgorithm::shouldBeStarted() const
		{
			if (!computed)
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		}

		void DPBondAssignmentAlgorithm::shouldHaveSolution() const
		{
			shouldBeStarted();
			if (!hasSolution)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}
		}

		void DPBondAssignmentAlgorithm::reset()
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
		 * BallPenaltyMap
		 */
		BallPenaltyMap::BallPenaltyMap(vector<int> const& penalties_, vector<Position> const& block_to_start_idx_,
				vector<Size> const& block_to_length_, vector<int> const& block_to_start_valence_,
				std::map<MolecularGraphTraits::VertexType, std::vector<int> > & atom_to_block_, int maxBondOrder_, int maxValence_) :
							penalties_(&penalties_), block_to_start_idx_(&block_to_start_idx_), block_to_length_(&block_to_length_),
							block_to_start_valence_(&block_to_start_valence_), atom_to_block_(&atom_to_block_),
							maxBondOrder_(static_cast<BondOrder>(maxBondOrder_)),
							maxValence_(static_cast<Valence>(maxValence_))
		{

		}

		Penalty BallPenaltyMap::getPenaltyFor(MolecularGraphTraits::VertexType vertex, Valence valence) const
		{
			int atomBlock = (*atom_to_block_)[vertex].front();
			Size const blockLength (_safeReading(atomBlock, *block_to_length_));
			Position const startIndex (_safeReading(atomBlock, *block_to_start_idx_));
			int const startValence (_safeReading(atomBlock, *block_to_start_valence_));
			int const position (valence - startValence);
			if (position >= 0 && static_cast<Size>(position) < blockLength)
			{
				int const penalty (_safeReading(startIndex + position, *penalties_));
				return (penalty < 0) ? INFINITE_PENALTY : static_cast<float>(penalty);
			} else
			{
				return INFINITE_PENALTY;
			}
		}

		BondOrder BallPenaltyMap::maxBondOrder() const
		{
			return maxBondOrder_;
		}

		Valence BallPenaltyMap::maxValence() const
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

		/*
		 * DPConfig
		 */
		DPConfig::DPConfig() :
			consumedValences(0), bondAssignments(0)
		{

		}
		DPConfig::DPConfig(DPConfig const& copy) :
			consumedValences(copy.consumedValences),
					bondAssignments(copy.bondAssignments)
		{

		}
		DPConfig::DPConfig(Size atoms, Size bonds) :
			consumedValences(atoms, 0), bondAssignments(bonds, 0)
		{

		}
		DPConfig::DPConfig(vector<Valence> v, vector<BondOrder> bo) :
			consumedValences(v), bondAssignments(bo)
		{

		}

		int DPConfig::compare(DPConfig const& conf) const
		{
			Size blength(conf.numberOfBonds());
			Size alength(conf.numberOfAtoms());
			if (blength != numberOfBonds() || alength != numberOfAtoms())
			{
				throw Exception::InvalidArgument(__FILE__, __LINE__, "you can compare only DPConfig entries from the same table");
			}
			for (Size i = 0; i < blength; ++i)
			{
				BondOrder left(bondAssignments[i]);
				BondOrder right(conf.bondAssignments[i]);
				if (left < right)
					return -1;
				if (left > right)
					return 1;
			}
			for (Size i = 0; i < alength; ++i)
			{
				Valence left(consumedValences[i]);
				Valence right(conf.consumedValences[i]);
				if (left < right)
					return -1;
				if (left > right)
					return 1;
			}
			return 0;
		}

		Size DPConfig::numberOfAtoms() const
		{
			return consumedValences.size();
		}

		Size DPConfig::numberOfBonds() const
		{
			return bondAssignments.size();
		}

		bool DPConfig::operator<(DPConfig const& conf) const
		{
			return compare(conf) < 0;
		}
		bool DPConfig::operator>(DPConfig const& conf) const
		{
			return compare(conf) > 0;
		}
		bool DPConfig::operator==(DPConfig const& conf) const
		{
			return compare(conf) == 0;
		}
		bool DPConfig::operator<=(DPConfig const& conf) const
		{
			return compare(conf) <= 0;
		}
		bool DPConfig::operator>=(DPConfig const& conf) const
		{
			return compare(conf) >= 0;
		}

		DPConfig& DPConfig::operator=(DPConfig const& copy)
		{
			if (this != &copy)
			{
				consumedValences = copy.consumedValences;
				bondAssignments = copy.bondAssignments;
			}
			return *this;
		}

		/*
		 * DPTable
		 */
		DPTable::DPTable() : table()
		{

		}
		DPTable::DPTable(DPTable const& dptab) :
			table(dptab.table)
		{

		}

		DPTable::iterator DPTable::begin()
		{
			return table.begin();
		}
		DPTable::iterator DPTable::end()
		{
			return table.end();
		}
		DPTable::const_iterator DPTable::begin() const
		{
			return table.begin();
		}
		DPTable::const_iterator DPTable::end() const
		{
			return table.end();
		}

		Size DPTable::size() const
		{
			return table.size();
		}

		Penalty DPTable::bestPenalty() const
		{
			Penalty pen(INFINITE_PENALTY);
			for (const_iterator iter(begin()); iter != end(); ++iter)
			{
				pen = min(pen, (*iter).second);
			}
			return pen;
		}

		DPConstRow DPTable::bestEntry() const
		{
			if (table.empty())
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, 1, 0);
			}
			const_iterator iter(begin());
			const_iterator best(iter);
			for (++iter; iter != end(); ++iter)
			{
				if ((*iter).second < (*best).second)
				{
					best = iter;
				}
			}
			return *best;
		}

		Penalty DPTable::operator[](DPConfig const& config) const
		{
			DPMap::const_iterator iter(table.find(config));
			return (iter == table.end()) ? INFINITE_PENALTY : iter->second;
		}

		bool DPTable::insert(DPConfig const& config, Penalty penalty)
		{
			if (penalty == INFINITE_PENALTY)
				return false;
			pair<DPConfig, Penalty> entry(config, penalty);
			pair<DPMap::iterator, bool> insertion(table.insert(entry));
			if (insertion.second)
			{
				return true;
			} else
			{
				if (insertion.first->second > penalty)
				{
					(insertion.first)->second = penalty;
					return true;
				} else
				{
					return false;
				}
			}
		}

		AdditionalBagProperties::AdditionalBagProperties() : bonds(), table(new DPTable()) {}
		AdditionalBagProperties::AdditionalBagProperties(AdditionalBagProperties const& copy) : bonds(copy.bonds), table(new DPTable(*copy.table)) {}
		AdditionalBagProperties::~AdditionalBagProperties()
		{
			delete table;
		}
		AdditionalBagProperties& AdditionalBagProperties::operator=(AdditionalBagProperties const& copy)
		{
			if (&copy != this) {
				bonds = copy.bonds;
				table = new DPTable(*copy.table);
			}
			return *this;
		}

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

		/**
		 * DPBondAssignment
		 */
		DPBondAssignment::DPBondAssignment(MolecularGraph const& molecule,
				PenaltyMap const& penalties, NiceTreeDecomposition const& ntd, Penalty upperbound) :
			molecule(&molecule), penalties(&penalties), ntd(&ntd), properties(ntd.size()), upperbound(upperbound),
			maxBondOrder(penalties.maxBondOrder()), maxValence(penalties.maxValence())
		{

		}

		DPBondAssignment::~DPBondAssignment()
		{
		}

		Penalty DPBondAssignment::compute()
		{
			DPTable* table(postOrderFolding<NiceTreeDecompositionBag<MolecularGraph>, DPTable*, DPBondAssignment> (*ntd->getRoot(), *this));
			return table->bestPenalty();
		}

		DPTable* DPBondAssignment::operator()(NiceTreeDecompositionBag<MolecularGraph>& bag,
				NodeType, vector<DPTable*>::const_iterator begin, vector<DPTable*>::const_iterator end)
		{
			AdditionalBagProperties& bagProperties (properties[bag.getIndex()]);
			vector<DPTable*>::const_iterator first (begin);
			vector<DPTable*>::const_iterator second (begin);
			++second;
			// check the pointers
			switch (bag.getBagType())
			{
			case TreeDecompositionBag<MolecularGraph>::JOIN_BAG:
				if (first == end || second == end || *second == NULL)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
			case TreeDecompositionBag<MolecularGraph>::INTRODUCE_BAG:
			case TreeDecompositionBag<MolecularGraph>::FORGET_BAG:
			case TreeDecompositionBag<MolecularGraph>::ROOT_BAG:
				if (first == end || *first == NULL)
				{
					throw Exception::NullPointer(__FILE__, __LINE__);
				}
			case TreeDecompositionBag<MolecularGraph>::LEAF_BAG: break;
			case TreeDecompositionBag<MolecularGraph>::END_BAG:
				if (boost::num_vertices(*molecule) == 0 && (&bag == ntd->getRoot()))
				{
					// empty molecule -> empty table
					bagProperties.table->insert(DPConfig(), 0);
					return bagProperties.table;
				} else
				{
					// else nice tree decomposition is damaged
					throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
				}
			}
			// remember bonds in bag
			bagProperties.bonds = getBondsInBag(bag);
			// compute the table
			switch (bag.getBagType())
			{
				case TreeDecompositionBag<MolecularGraph>::INTRODUCE_BAG:
					computeIntroduceBag(bag, **first, bagProperties);
					break;
				case TreeDecompositionBag<MolecularGraph>::FORGET_BAG:
					computeForgetBag(bag, **first, bagProperties);
					break;
				case TreeDecompositionBag<MolecularGraph>::JOIN_BAG:
					computeJoinBag(bag, **first, **second, bagProperties);
					break;
				case TreeDecompositionBag<MolecularGraph>::LEAF_BAG:
					computeLeafIntroduceBag(bagProperties);
					break;
				case TreeDecompositionBag<MolecularGraph>::ROOT_BAG:
					computeRootBag(bag, **first, bagProperties);
					break;
				case TreeDecompositionBag<MolecularGraph>::END_BAG: ;
					// can't happen
			}
			//PRINT_TABLE(bagProperties, bag);
			return bagProperties.table;
		}

		void DPBondAssignment::computeLeafIntroduceBag(AdditionalBagProperties& bagProperties)
		{
			for (BondOrder i = 0; i < maxBondOrder; ++i)
			{
				bagProperties.table->insert(DPConfig(1, 0), 0);
			}
		}

		std::vector<MolecularGraphTraits::EdgeType> DPBondAssignment::getBondsInBag(
				NiceTreeDecompositionBag<MolecularGraph>& bag)
		{
			std::set<MolecularGraphTraits::VertexType>& vertices = bag.getInnerVertices();
			std::vector<MolecularGraphTraits::EdgeType> bonds;
			bonds.reserve((vertices.size()-1 * vertices.size())/2);
			for (std::set<MolecularGraphTraits::VertexType>::const_iterator iter(vertices.begin()); iter
					!= vertices.end(); ++iter)
			{
				set<MolecularGraphTraits::VertexType>::const_iterator iter2 (iter);
				for (++iter2; iter2 != vertices.end(); ++iter2)
				{
					if (boost::edge(*iter, *iter2, *molecule).second)
					{
						bonds.push_back(Edge(*iter, *iter2));
					}
				}
			}
			return bonds;
		}

		void DPBondAssignment::computeIntroduceBag(
				NiceTreeDecompositionBag<MolecularGraph>& bag, DPTable& child, AdditionalBagProperties& property)
		{
			DPTable& table(*property.table);
			MolecularGraphTraits::VertexType iv(bag.getIntroducedVertex());
			std::vector<MolecularGraphTraits::EdgeType>& bonds(property.bonds);
			Size numOfValences(bag.getInnerVertices().size());
			Size numOfBonds(property.bonds.size());
			std::set<MolecularGraphTraits::VertexType> vset(bag.getInnerVertices());
			// for each entry in child table, copy this entry 3^n times, where n is the number of introduced bonds. Then fill each
			// copy with n new bondvalue-columns with different values
			for (DPTable::const_iterator iter(child.begin()); iter != child.end(); ++iter)
			{
				DPConstRow childEntry(*iter);
				DPConfig conf(numOfValences, numOfBonds);
				Size vindex(0);
				Size cindex(0);
				// because in our new entry we have one more valence-column, each vertex after the introduced vertex
				// have to be copied one column next to it's previous position
				for (std::set<MolecularGraphTraits::VertexType>::const_iterator it(vset.begin()); it != vset.end(); ++it, ++vindex)
				{
					if (*it != iv)
					{
						conf.consumedValences[vindex]
								= childEntry.first.consumedValences[cindex++];
					}
				}
				// the same happens with the bonds. Copy each bond which exists also in the child into it's correct position
				vindex = 0;
				cindex = 0;
				vector<Size> indizes;
				for (vector<Edge>::const_iterator it(bonds.begin()); it
						!= bonds.end(); ++it, ++vindex)
				{
					Edge e(*it);
					if (e.incidence(iv))
					{
						// remember the indizes of the new introduced bonds
						indizes.push_back(vindex);
					} else
					{
						conf.bondAssignments[vindex]
								= childEntry.first.bondAssignments[cindex++];
					}
				}
				// if there are any introduced bonds we have to fill them with each possible value
				if (indizes.size() > 0 ) {
					vector<int> bondValues (indizes.size());
					Size currentIndex (0);
					while (currentIndex < bondValues.size())
					{
						currentIndex = 0;
						// insert entry
						for (Size k(0); k < bondValues.size(); ++k)
						{
							conf.bondAssignments[indizes[k]] = bondValues[k];
						}
						table.insert(conf, childEntry.second);
						// increase value in index
						while (currentIndex < bondValues.size() && ++bondValues[currentIndex] >= maxBondOrder)
						{
							bondValues[currentIndex] = 0;
							++currentIndex;
						}
					}
				} else
				{
					table.insert(conf, childEntry.second);
				}
			}
		}

		Penalty DPBondAssignment::forgetInnerVertexIn(NiceTreeDecompositionBag<MolecularGraph> & bag,
				DPConstRow childRow, DPConfig& entry, vector<Edge>& childBonds, Size forgottenIndex)
		{
			DPConfig& childEntry (childRow.first);
			MolecularGraphTraits::VertexType forgottenVertex (bag.getForgottenVertex());
			std::set<MolecularGraphTraits::VertexType>& vertices (bag.getInnerVertices());
			Size bondIndex (0);
			Size childBondIndex(0);
			Size forgottenValence (childEntry.consumedValences[forgottenIndex]);
			Size maxForgottenValence (min(static_cast<BondOrder>(maxValence-1),
					static_cast<BondOrder>(molecule->degree(forgottenVertex)) * maxBondOrder));
			// copy valence values in child entry into new entry
			for (Size i (0); i < forgottenIndex; ++i)
			{
				entry.consumedValences[i] = childEntry.consumedValences[i];
			}
			for (Size i (forgottenIndex+1); i < childEntry.consumedValences.size(); ++i)
			{
				entry.consumedValences[i-1] = childEntry.consumedValences[i];
			}
			// forget each bond which is incident to forgotten vertex
			for (; childBondIndex < childBonds.size(); ++childBondIndex)
			{
				if (childBonds[childBondIndex].incidence(forgottenVertex))
				{
					// forget this bond, add its bondvalue to the consumed valence of the kept (not forgotten) atom and to the forgotten atom
					MolecularGraphTraits::VertexType keptAtom = childBonds[childBondIndex].partnerOf(forgottenVertex);
					BondOrder bondValue (childEntry.bondAssignments[childBondIndex] + 1);
					Size atomIndex (distance(vertices.begin(), find(vertices.begin(), vertices.end(), keptAtom)));;
					Size newValence (entry.consumedValences[atomIndex] + bondValue);
					forgottenValence += bondValue;
					// if valence is greater than maximal possible valence (< 8, < number of neighbours * max bond value per neighbour)
					// then don't add this entry
					if ((forgottenValence <= maxForgottenValence) &&
							(newValence <= min(molecule->degree(keptAtom) * static_cast<Size>(maxBondOrder),
									static_cast<Size>(maxValence))))
					{
						entry.consumedValences[atomIndex] = newValence;
					} else
					{
						return INFINITE_PENALTY;
					}
				} else
				{
					// just copy bondvalue
					entry.bondAssignments[bondIndex] = childEntry.bondAssignments[childBondIndex];
					++bondIndex;
				}
			}
			if (forgottenValence > maxForgottenValence)
			{
				// shouldn't happen on this place
				return INFINITE_PENALTY;
			}
			// add penalty of forgotten atom to the penalty of the entry
			return penalties->getPenaltyFor(forgottenVertex, forgottenValence) + childRow.second;
		}

		void DPBondAssignment::computeForgetBag(
				NiceTreeDecompositionBag<MolecularGraph> const& bag, DPTable const& child, AdditionalBagProperties& property)
		{
			AdditionalBagProperties& childProperty (properties[bag.firstChild().getIndex()]);
			vertexSet const& vset (bag.firstChild().getInnerVertices());
			DPConfig newEntry (vset.size()-1, property.bonds.size());
			Size forgottenIndex (distance(vset.begin(), find(vset.begin(), vset.end(), bag.getForgottenVertex())));
			for (DPTable::const_iterator entryIterator (child.begin()); entryIterator != child.end(); ++entryIterator)
			{
				Penalty entryPenalty (forgetInnerVertexIn(bag, *entryIterator, newEntry, childProperty.bonds, forgottenIndex));
				if (entryPenalty < upperbound)
				{
					property.table->insert(newEntry, entryPenalty);
				}
			}
		}

		void DPBondAssignment::computeRootBag(NiceTreeDecompositionBag<MolecularGraph> const& bag, DPTable const& child, AdditionalBagProperties& bagProperties)
		{
			DPConfig empty (0, 0);
			vector<Edge> emptyList (0);
			Penalty minPenalty (INFINITE_PENALTY);
			for (DPTable::const_iterator iter (child.begin()); iter != child.end(); ++iter)
			{
				minPenalty = min(minPenalty, forgetInnerVertexIn(bag, *iter, empty, emptyList, 0));
				if (Maths::isEqual(0, minPenalty))
				{
					break;
				}
			}
			bagProperties.table->insert(empty, minPenalty);
		}

		void DPBondAssignment::computeJoinBag(NiceTreeDecompositionBag<MolecularGraph> const& bag,
				DPTable const& leftChild, DPTable const& rightChild, AdditionalBagProperties& property)
		{
			Size const numOfValences(bag.getInnerVertices().size());
			vertexSet const& innerVertices(bag.getInnerVertices());
			idList vertices(innerVertices.begin(), innerVertices.end());
			DPTable& table(*property.table);
			DPJoinMap map;
			// insert each entry of the left child into a DPJoinMap
			for (DPTable::const_iterator leftIter(leftChild.begin()); leftIter
					!= leftChild.end(); ++leftIter)
			{
				DPConstRow leftEntry(*leftIter);
				map.insert(pair<DPConfig const*, Penalty> (&(leftEntry.first),
						leftEntry.second));
			}
			// find for each entry of the right child's table appropiate entries in the DPJoinMap (which have the same bondvalues)
			for (DPTable::const_iterator rIter(rightChild.begin()); rIter
					!= rightChild.end(); ++rIter)
			{
				DPConstRow rightEntry(*rIter);
				DPConfig const* rightConf(&rightEntry.first);
				pair<DPJoinMap::const_iterator, DPJoinMap::const_iterator> matchingRange(
						map.equal_range(rightConf));

				for (DPJoinMap::const_iterator match(matchingRange.first); match
						!= matchingRange.second; ++match)
				{
					// merge both entries by summate the consumed valences and penalties
					DPConfig config(*(*match).first);
					bool doInsert(true);
					for (Size index(0); index < numOfValences; ++index)
					{
						config.consumedValences[index]
								+= rightEntry.first.consumedValences[index];
						{
							vertexId vertex(vertices[index]);
							if (config.consumedValences[index] > (int) (min(static_cast<BondOrder>(maxValence-1),
									maxBondOrder * static_cast<BondOrder>(molecule->degree(vertex)))))
							{
								doInsert = false;
								break;
							}
						}
					}
					Penalty penalty(rightEntry.second + (*match).second);
					if (doInsert && penalty < INFINITE_PENALTY)
					{
						table.insert(config, penalty);
					}
				}
			}
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
		DPBackTracking::DPBackTracking(DPBondAssignment const& bondAssignment, Size maxNumberOfSolutions, Penalty upperbound) :
				bondAssignment(&bondAssignment), currentState(NULL), queue(), maxNumberOfSolutions(maxNumberOfSolutions), bonds(),
				bags(), maxHeapSize(maxNumberOfSolutions), upperbound(upperbound)
		{
			// build a ordered list of bonds
			if (bondAssignment.molecule == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
			UndirectedGraph const& graph (*bondAssignment.molecule);
			bonds.reserve(graph.numberOfEdges());
			for (EdgeIterator iter (graph.firstEdge()); iter != graph.endEdge(); ++iter)
			{
				Edge e (*iter);
				if (e.left < e.right)
				{
					bonds.push_back(e);
				}
			}
			// sort bonds - the second vertex could be in false order
			sort(bonds.begin(), bonds.end());
			// order bags in preorder
			if (bondAssignment.ntd == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
			{
				bags.reserve(bondAssignment.ntd->size());
				PreOrderTraversal<NiceTreeDecompositionBag<MolecularGraph> const> begin (*bondAssignment.ntd->getRoot());
				PreOrderTraversal<NiceTreeDecompositionBag<MolecularGraph> const> const end;
				for (; begin != end; ++begin)
				{
					bags.push_back(&(*begin));
				}
			}
			// init state
			DPTable const& table (getTable(bondAssignment.ntd->getRoot()->getIndex()));
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
		DPBackTracking::DPBackTracking(DPBackTracking const& copy) :
				bondAssignment(copy.bondAssignment), currentState(NULL), queue(),
				maxNumberOfSolutions(copy.maxNumberOfSolutions), bonds(copy.bonds), bags(copy.bags), maxHeapSize(copy.maxHeapSize),
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
				this->~DPBackTracking();
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


		DPTable const& DPBackTracking::getTable(Size order) const
		{
			DPTable * table (getProperties(order).table);
			if (table == NULL)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
			return *table;
		}

		AdditionalBagProperties const& DPBackTracking::getProperties(Size order) const
		{
			vector<AdditionalBagProperties> const& properties (bondAssignment->properties);
			if (order < properties.size())
			{
				return properties[order];
			} else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(order), properties.size());
			}
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
			multiset<BackTrackingState*>::iterator first (queue.begin());
			currentState = *first;
			queue.erase(first);
			--maxHeapSize;
			for (Size index(currentState->index); index < bags.size(); ++index)
			{
				NiceTreeDecompositionBag<MolecularGraph> const& bag(*bags[index]);
				switch (bag.getBagType())
				{
				case ROOT_BAG:
					visitForget(*currentState, bag, getTable(bag.firstChild().getIndex()));
					break;
				case INTRODUCE_BAG:
					visitIntroduce(*currentState, bag, getTable(bag.firstChild().getIndex()));
					break;
				case LEAF_BAG:
					visitLeaf(*currentState);
					break;
				case JOIN_BAG:
				{
					visitJoin(*currentState, bag, getTable(bag.firstChild().getIndex()), getTable(bag.secondChild().getIndex()));
					break;
				}
				case FORGET_BAG:
					visitForget(*currentState, bag, getTable(bag.firstChild().getIndex()));
					break;
				case END_BAG:
					if (bondAssignment->molecule->numberOfVertices() == 0)
					{
						// empty molecule -> you are finished
						return;
					} else
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
		void DPBackTracking::visitIntroduce(BackTrackingState& state,
				NiceTreeDecompositionBag<MolecularGraph> const& bag, DPTable const& table)
		{

			vertexSet const& bagVertices(bag.getInnerVertices());
			vector<Edge> const& bagBonds(getProperties(bag.getIndex()).bonds);
			vertexId const iv(bag.getIntroducedVertex());

			// entry of the given table
			DPConfig& successor(state.config);
			// the entry of the previous table. We don't know it and want do find it
			DPConfig antecessor((*table.begin()).first);

			// copy each bond value of bonds which were not introduced in this vertex
			Size j(0);
			for (Size i(0); i < bagBonds.size(); ++i)
			{
				if (!bagBonds[i].incidence(iv))
				{
					antecessor.bondAssignments[j++] = successor.bondAssignments[i];
				}
			}
			// copy each consumed valences of vertices which were not introduced in this vertex
			j = 0;
			Size k(0);
			for (set<vertexId>::const_iterator iter(bagVertices.begin()); iter
					!= bagVertices.end(); ++iter, ++k)
			{
				if ((*iter) != iv)
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
				NiceTreeDecompositionBag<MolecularGraph> const& bag, DPTable const& leftTable,
				DPTable const& rightTable)
		{

			vector<DPPairIt> possibleAntecessors;
			vector<DPTable::const_iterator> leftEntries;
			vector<DPTable::const_iterator> rightEntries;
			DPJoinMapComparator comp;
			DPConfig const& successor(state.config);
			NiceTreeDecompositionBag<MolecularGraph> const& rightChild(bag.secondChild());
			// insert possible antecessors in vectors
			for (DPTable::const_iterator iter(leftTable.begin()); iter != leftTable.end(); ++iter)
			{
				DPConstRow antecessor(*iter);
				if (comp.compare(&successor, &(antecessor.first)) == 0)
				{
					leftEntries.push_back(iter);
				}
			}
			for (DPTable::const_iterator iter(rightTable.begin()); iter
					!= rightTable.end(); ++iter)
			{
				DPConstRow antecessor(*iter);
				if (comp.compare(&successor, &(antecessor.first)) == 0)
				{
					rightEntries.push_back(iter);
				}
			}
			// now search possible pairs of antecessors from vectors
			Size n(successor.consumedValences.size());
			for (vector<DPTable::const_iterator>::const_iterator
					left(leftEntries.begin()); left != leftEntries.end(); ++left)
			{
				DPConstRow leftEntry(**left);
				for (vector<DPTable::const_iterator>::const_iterator right(
						rightEntries.begin()); right != rightEntries.end(); ++right)
				{
					DPConstRow rightEntry(**right);
					// check sum of valences
					bool correctValences(true);
					for (Size i(0); i < n; ++i)
					{
						if (successor.consumedValences[i]
								!= (leftEntry.first.consumedValences[i]
										+ rightEntry.first.consumedValences[i]))
						{
							correctValences = false;
							break;
						}
					}
					if (correctValences)
					{
						possibleAntecessors.push_back(DPPairIt(*left, *right));
					}
				}
			}
			sort(possibleAntecessors.begin(), possibleAntecessors.end(),
					compareJoinTablePairs);
			vector<DPPairIt>::const_iterator iter(possibleAntecessors.begin());
			// follow best left entry, add the right entry into join branch stack
			DPPairIt best(*iter);
			Penalty bestPenalty(best.first->second + best.second->second);
			{
				DPConstRow left(*best.first);
				DPConstRow right(*best.second);
				extendState(state, left.first, 0);
				++state.index;

				// add the other solutions into queue
				for (++iter; iter != possibleAntecessors.end(); ++iter)
				{
					DPPairIt entry(*iter);
					DPConstRow left(*entry.first);
					DPConstRow right(*entry.second);
					Penalty addPenalty((left.second + right.second) - bestPenalty);
					if (isSolutionNeeded(state.assignment.penalty + addPenalty))
					{
						BackTrackingState newState(state);
						extendState(newState, left.first, addPenalty);
						branchState(newState, rightChild, right.first);
						remember(newState);
					}
				}

				// branch current state

				branchState(state, rightChild, right.first);
			}
		}

#include <iostream>

		/**
		 * geg: state mit Eintrag aus Forget-Tabelle
		 * bag: Forget Bag
		 * table: Tabelle des Kindknotens der ForgetBag
		 * gesucht: Alle Einträge in der Kindtabelle, die den Forget-Eintrag erzeugt haben können
		 * -> schon schwieriger. Man muss alle Einträge der Kindtabelle durchgehen, ihre Forget-Nodes
		 * -> löschen und prüfen, ob sie dann den gesuchten Eintrag ergeben.
		 */
		void DPBackTracking::visitForget(BackTrackingState& state,
				NiceTreeDecompositionBag<MolecularGraph> const& bag, DPTable const& table)
		{
			vector<DPPointerRow> possibleAntecessors;
			NiceTreeDecompositionBag<MolecularGraph> const& child(bag.firstChild());
			vertexSet const& childVertices(child.getInnerVertices());
			vector<Edge> const& childBonds(getProperties(child.getIndex()).bonds);
			vertexId const fv(bag.getForgottenVertex());
			Size const forgottenIndex(distance(childVertices.begin(), find(
					childVertices.begin(), childVertices.end(), fv)));
			DPConfig const& successor(state.config);
			DPConfig testEntry (successor);
			// check for each row entry: is it a possible anteccessor?
			for (DPTable::const_iterator iter(table.begin()); iter != table.end(); ++iter)
			{
				Penalty pen (bondAssignment->forgetInnerVertexIn(bag, *iter, testEntry, childBonds, forgottenIndex));
				if (pen < INFINITE_PENALTY && testEntry == successor)
				{
					possibleAntecessors.push_back(DPPointerRow(&(iter->first), pen));
				}
			}
			// remember each found antecessors
			sort(possibleAntecessors.begin(), possibleAntecessors.end(),
					compareTablePointerEntries);
			vector<DPPointerRow>::const_iterator iter(possibleAntecessors.begin());
			// follow this antecessor;

			Penalty bestPenalty((*iter).second);
			{
				DPConfig const& antecessor(*(*iter).first);
				extendState(state, antecessor, 0);
				++state.index;
				setStateAssignment(state, child, antecessor, fv);
			}

			// add each other antecessors into the queue
			for (++iter; iter != possibleAntecessors.end(); ++iter)
			{
				DPConfig const& antecessor(*(*iter).first);
				Penalty const addPenalty((*iter).second - bestPenalty);
				if (isSolutionNeeded(state.assignment.penalty + addPenalty))
				{
					BackTrackingState newState(state);
					extendState(newState, antecessor, addPenalty);
					setStateAssignment(newState, child, antecessor, fv);
					remember(newState);
				} else
				{
					return;
				}
			}
		}

		Size DPBackTracking::bondIndexFor(Edge bond) const
		{
			Size d (distance(bonds.begin(), lower_bound(bonds.begin(), bonds.end(), bond)));
			if (d < bonds.size())
			{
				return d;
			} else
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
				NiceTreeDecompositionBag<MolecularGraph> const& bag, DPConfig const& antecessor, vertexId forgottenVertex)
		{
			vector<Edge> bonds(getProperties(bag.getIndex()).bonds);
			vector<Edge>::const_iterator begin (bonds.begin());
			{
				for (vector<Edge>::const_iterator iter(begin); iter
						!= bonds.end(); ++iter)
				{
					Edge bond(*iter);
					if (bond.incidence(forgottenVertex))
					{
						Size bondIndex(bondIndexFor(bond));
						state.assignment[bondIndex] = antecessor.bondAssignments[std::distance(begin, iter)];
					}
				}
			}
		}

		void DPBackTracking::extendState(BackTrackingState& state,
				DPConfig const& antecessor, Penalty additionalPenalty)
		{
			state.config = antecessor;
			state.assignment.penalty += additionalPenalty;
		}

		void DPBackTracking::branchState(BackTrackingState& state,
				NiceTreeDecompositionBag<MolecularGraph> const& child, DPConfig const& antecessor)
		{
			pair<DPConfig, Size> rightState;
			rightState.first = antecessor;
			rightState.second = distance(bags.begin(), find(bags.begin() + state.index,
							bags.end(), &child));
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

		DPBackTrackingCombiner::DPBackTrackingCombiner(
				DPBackTrackingCombiner const& copy) :
		backtrackers(copy.deepCopyOfBacktrackers()), priorityQueue(copy.priorityQueue),
		componentSolutions(copy.componentSolutions), assignment(copy.assignment),
		solutionNumber(copy.solutionNumber), optimum(copy.optimum), upperbound(copy.upperbound)
		{

		}
		DPBackTrackingCombiner::DPBackTrackingCombiner(vector<DPBondAssignment*> const& bondAssignments, Size solutionNumber, Penalty upperbound) :
		backtrackers(), priorityQueue(), componentSolutions(bondAssignments.size()),
		assignment(), solutionNumber(solutionNumber), optimum(INFINITE_PENALTY), upperbound(upperbound)
		{
			if (bondAssignments.empty())
			{
				throw Exception::SizeUnderflow(__FILE__, __LINE__, 0);
			}
			backtrackers.reserve(bondAssignments.size());
			for (vector<DPBondAssignment*>::const_iterator iter(bondAssignments.begin()); iter
					!= bondAssignments.end(); ++iter)
			{
				backtrackers.push_back(new DPBackTracking(**iter, solutionNumber, upperbound));
			}
			initialize();
		}
		DPBackTrackingCombiner::DPBackTrackingCombiner(
				vector<DPBondAssignment> const& bondAssignments, Size solutionNumber, Penalty upperbound) :
		backtrackers(), priorityQueue(), componentSolutions(bondAssignments.size()),
		assignment(), solutionNumber(solutionNumber), optimum(INFINITE_PENALTY), upperbound(upperbound)
		{
			backtrackers.reserve(bondAssignments.size());
			for (vector<DPBondAssignment>::const_iterator iter(bondAssignments.begin()); iter
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

		bool Assignment::isValid(UndirectedGraph const& graph, PenaltyMap const& penalties) const
		{
			float cpen (0);
			Valence const maxValence (penalties.maxValence());
			BondOrder const maxBondOrder (penalties.maxBondOrder());
			vector<Valence> valences (graph.maxVertexId()+1, 0);
			vector<vertexId> sortedNeighbours;
			sortedNeighbours.reserve(10);
			vector<vertexId> const& vertices (graph.getVertices());
			vector<BondOrder>::const_iterator bondIterator (bonds.begin());
			for (vector<vertexId>::const_iterator iter(vertices.begin()); iter != vertices.end(); ++iter)
			{
				vertexId left (*iter);
				vector<vertexId> const& neighbours (graph.getNeighbourhood(left));
				sortedNeighbours = neighbours; //.assign(neighbours.begin(), neighbours.end());
				sort(sortedNeighbours.begin(), sortedNeighbours.end());
				for (vector<vertexId>::const_iterator iter2(sortedNeighbours.begin()); iter2 != sortedNeighbours.end(); ++iter2)
				{
					vertexId right (*iter2);
					if (left < right)
					{
						if (bondIterator == bonds.end())
						{
							throw Exception::NullPointer(__FILE__, __LINE__);
						}
						BondOrder value (*bondIterator + 1);
						if (value > maxBondOrder)
						{
							return false;
						}
						valences.at(left) += value;
						valences.at(right) += value;
						++bondIterator;
					}
				}
			}
			Size vertexId (0);
			for (vector<Valence>::const_iterator iter (valences.begin()); iter != valences.end(); ++iter, ++vertexId)
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
