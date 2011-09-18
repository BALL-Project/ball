#ifndef BALL_STRUCTURE_BONDORDERS_DPBONDASSIGNMENTSTRATEGY_H
#define BALL_STRUCTURE_BONDORDERS_DPBONDASSIGNMENTSTRATEGY_H


namespace BALL
{
	class BALL_EXPORT FPTBondOrderStrategy
	{

		public:
			BALL_CREATE(FPTBondOrderStrategy)

				/**
				 * Default constructor
				 */
				FPTBondOrderStrategy();

			/**
			 * Builds a FPTBondOrderStrategy with the given molecule and penalty map
			 * @param molecule an atomContainer for which bonds the assignment should be done
			 * @param penaltyMap a penalty map which maps an atom with a specific valence to a penalty value
			 * @param numberOfSolutions the maximum number of solutions you want to backtrack. By default this class just
			 * 													backtracks the optimum solution
			 * @param upperbound Solutions which are worse than the upperbound will not be computed. This can improve the
			 * 										performance, but usually the algorithm should be fast enough. Is INFINITE_PENALTY by default.
			 */
			FPTBondOrderStrategy(AtomContainer& molecule, PenaltyMap const& penaltyMap,
					Size numberOfSolutions = MAX_SOLUTION_NUMBER, Penalty upperbound = INFINITE_PENALTY );

			/**
			 * Copy constructor
			 */
			FPTBondOrderStrategy(FPTBondOrderStrategy const& copy);

			/**
			 * Assignment operator
			 */
			FPTBondOrderStrategy& operator=(FPTBondOrderStrategy const& copy);

			virtual ~FPTBondOrderStrategy();

			void start();

			/**
			 * setter for the atom container. Calling this methods reset the FPTBondOrderStrategy. That means, each
			 * previous computing is deleted. This function doesn't check if the new atom container is really different
			 * than the previous one.
			 */
			void setMolecule(AtomContainer& molecule);

			/**
			 * set the upperbound for the computing. Changing this value during the algorithm will reset the
			 * FPTBondOrderStrategy and delete each previous computation.
			 */
			void setUpperbound(Penalty upperbound);

			/**
			 * returns the upperbound for the computing. This algorithm computes only solutions with
			 * better penalty than the upperbound.
			 */
			Penalty getUpperbound() const;

			/**
			 * set the maximum number of solutions for the computing. Changing this value during the algorithm will reset the
			 * FPTBondOrderStrategy and delete each previous computation.
			 */
			void setMaximumNumberOfSolutions(Size solutionNumber);

			/**
			 * returns the maximum number of solutions which can be backtracked.
			 * @see MAX_SOLUTION_NUMBER
			 */
			Size getMaximumNumberOfSolutions() const;

			/**
			 * returns a const reference to the atom container
			 * @throws BALL::Exception::NullPointer if molecule is NULL
			 */
			AtomContainer& getMolecule();

			/**
			 * setter for the penalty map. Calling this methods reset the FPTBondOrderStrategy. That means, each
			 * previous computing is deleted. This function doesn't check if the new penalty map is really different
			 * than the previous one.
			 */
			void setPenaltyMap(PenaltyMap const& penaltyMap);

			/**
			 * returns a const reference to the penalty map
			 * @throws BALL::Exception::NullPointer if penalty map is NULL
			 */
			PenaltyMap const& getPenaltyMap();

			void nextSolution();

			/**
			 * returns true if there is a further solution which can be backtracked
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			bool hasNextSolution() const;

			/**
			 * delete all previous computings and frees the nice tree decomposition and penalty map, if they are not
			 * referenced by another FPTBondOrderStrategy.
			 * After this method you can not access the solutions. Call #start again to compute the bond order.
			 */
			void reset();

			/**
			 * Returns the penalty of the solution you get if you call ++ or nextSolution. Returns INFINITE_PENALTY
			 * if there is no more solution.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			Penalty penaltyOfNextSolution() const;

			/**
			 * Computes the next solution. Behaves like the iterator function. This function differs from nextSolution, because
			 * it allows to "move the iterator" a position behind the last element. So even if there is no more solution you can
			 * call the ++ operator a last time. Nevertheless you can not access a solution if the iterator is behind the last element.
			 * So call hasMoreSolutions() before calling nextSolution, and operator + before dereferencing a solution or moving
			 * the iterator forward.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			FPTBondOrderStrategy& operator++();

			/**
			 * returns true if the "iterator" points on a solution. If this method returns false, you cannot access the solutions,
			 * because the iterator points behind the last solution.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			bool operator + () const;

			/**
			 * Give access to the last computed solution. This solution is returned as Assignment instance. So you can
			 * access the penalty of the solution and the bond order.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			Assignment& operator * ();

			/**
			 * Give access to the last computed solution. This solution is returned as Assignment instance. So you can
			 * access the penalty of the solution and the bond order.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			Assignment * operator -> ();

			/**
			 * Validates the assignment
			 * Returns true if the assignment is valid with the molecular graph and penalty map. This function should
			 * always returns true, otherwise the algorithm has a bug.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			bool isValidAssignment();

			bool isValidAssignment(Assignment& ref);

			/**
			 * Give access to the last computed solution. This function returns a HashMap which maps each bond in the
			 * atom container to the computed bond value. The HashMap is not built during backtracking, but by calling this
			 * function. So each calling of this method builds a new HashMap.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			HashMap<Bond const *, int > getBondAssignmentHashMap() const;

			/**
			 * returns a vector with pointers to the bonds of the atom container. The order of this bonds in the vector
			 * is the same as the order of the bond values in the assignments.
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			vector<Bond const *> const& getBonds() const;

			typedef HashMap<Atom const *, Position> AtomToIndex;

			/**
			 * strict ordering of bonds depending of the atom indizes of their incident atoms
			 */
			struct sortBondInNormalizedOrder
			{
				FPTBondOrderStrategy const& obj;
				bool operator()(Bond const * left, Bond const * right) const;
			};

		protected:
			/**
			 * number of solutions this algorithm can backtrack
			 */
			Size numberOfSolutions;

			/**
			 * this algorithm will just compute solutions which have a better penalty than this upperbound
			 */
			Penalty upperbound;

			/**
			 * the atom container for which the bond assignment is computed
			 */
			AtomContainer * atomContainer;

			/**
			 * the penalty map which maps each atom with a specific valence to a penalty
			 */
			PenaltyMap const* penaltyMap;

			/**
			 * is true when the #start method is called and the computing is done. Is set to false after calling #reset
			 */
			bool computed;

			/**
			 * is true if the "iterator" points to a solution and false, if the iterator is behind the solutions
			 */
			bool hasSolution;

			/**
			 * The backtracker
			 */
			DPBackTrackingCombiner *combiner;


			/**
			 * builds a undirected graph from the given atom container
			 */
			void buildGraph();

			/**
			 * compute the dynamic programming tables for each bag of the tree decomposition
			 */
			void computeBondAssignment();

			/**
			 * check if the computing was started before accessing the solutions
			 */
			void shouldBeStarted() const;

			/**
			 * check if there is a solution before access it
			 */
			void shouldHaveSolution() const;
	};

	namespace DPBondOrder
	{
		using boost::shared_ptr;

		/**
		 * Each atom can have a Valence of (0, VALENCE_SIZE]. This is just the default option. The algorithm uses the
		 * option which is given by the penalty map
		 */
		#define VALENCE_SIZE 8

		/**
		 * Each bond can have a BondOrder of (0, BOND_SIZE]. This is just the default option. The algorithm uses the
		 * option which is given by the penalty map
		 */
    #define BOND_SIZE 3

		/**
		 * The maximum number of solutions which should be backtracked. This is just the default option. You can specify
		 * own values in the constructor of FPTBondOrderStrategy, DPBackTracking and DPBackTrackingCombiner.
		 * This value doesn't mean that this number of solution is backtracked after starting the algorithm. The algorithm
		 * works like an iterator and backtracks a solution if it is requested. So a high MAX_SOLUTION_NUMBER don't have
		 * to influence the performance of the algorithm, even if you want only a few solutions. Nevertheless, in each
		 * backtracking step the algorithm remembers all found solutions until it has enough solutions, so that it can
		 * forget the bad ones. A high MAX_SOLUTION_NUMBER means that the algorithm have to remember more solutions and
		 * so the algorithm needs more memory during the backtracking.
		 * If you want just good solutions you can set MAX_SOLUTION_NUMBER to a very high value and decrease the upperbound
		 * instead. The backtracker just remembers solutions which are better than the upperbound, so a low upperbound can
		 * improve the memory efficience and compensate the high MAX_SOLUTION_NUMBER.
		 *
		 * By default this number is 100 and the upperbound is INFINITE_PENALTY
		 */
		 #define MAX_SOLUTION_NUMBER 100

		/**
		 * The infinite_penalty is the maximum upperbound of the Bond-Assignment computing. If a bond-assignment has this
		 * penalty, then there is no solution for this bond-assignment problem.
		 * The infinite penalty is defined in BALL/STRUCTURE/assignBondOrderProcessor.h
		 */
		#ifndef INFINITE_PENALTY
		# define INFINITE_PENALTY 1e5
		#endif

		/**
		 * This is a memory efficient implementation of PenaltyMap.
		 * It doesn't save MAX_VALENCE values for each atom and defines penalties for more than one atom.
		 * Often an atom has just a few possible valence configurations,
		 * for example (3, 4, 5). Then the first and the last configurations are INFINITE_PENALTY and will not be saved.
		 * Furthermore you can reuse penalty-entries for more than one atom (because many atoms of the same type have the same
		 * penalty entries).
		 * To realize this, you need a vector with the offsets and with the lengths of the penalties for an atom. Furthermore you
		 * need a vector with the first valence value.
		 * This implementation is used in AssignBondOrderProcessor so you can use the penalties, which were computed in
		 * this processor, to fill this table.
		 * The different vectors, which are used by this implementation, are referenced by pointers. So this class don't
		 * allocate additional memory for the vectors. On the other side you have to care that the vectors are not deleted or
		 * garbage collected during the existence of the instance of this class.
		 */
		class PenaltyMap 
		{
			public:

				/**
				 * Build a new BallPenaltyMap with the given vectors. The vectors aren't copied. Instead this Class uses
				 * pointers to access the values in this vectors. So you have to care that the vectors are not deleted or
				 * garbage collected during the existence of this instance.
				 * @param penalties_ a vector with the penalty values for each atom. The penalties have not the type Penalty, because
				 * 									 AssignBondOrderProcessor use int penalties instead.
				 * @param block_to_start_idx_ contains for each block-index the position in the penalties_ array where this block starts
				 * @param block_to_length_ contains for each block_index the number of saved penalties for the atoms in the block
				 * @param block_to_start_valence_ contains the valence of the first entry of the given block
				 * @param atom_to_block_ contains the block index for the given atom
				 * @param maxBondOrder bond values which are greater or equal to this value can not be assigned to bonds
				 * @param maxValence valences which are greater or equal to this value can not be assigned to atoms
				 */
				BallPenaltyMap(std::vector<int> const& penalties_, std::vector<Position> const& block_to_start_idx_,
						std::vector<Size> const& block_to_length_, std::vector<int> const& block_to_start_valence_, 
						std::vector<std::vector<int> > const& atom_to_block_, int maxBondOrder_, int maxValence_);

				/**
				 * @return penalty for the atom with index vertex and the given valence
				 */
				Penalty getPenaltyFor(MolecularGraphTraits::VertexType vertex, Valence valence) const;

				BondOrder maxBondOrder() const;

				Valence maxValence() const;

			protected:
				/**
				 * bond values which are greater or equal to this value can not be assigned to bonds
				 */
				BondOrder maxBondOrder_;

				/**
				 * valences which are greater or equal to this value can not be assigned to atoms
				 */
				Valence maxValence_;

			private:
				/**
				 * Read an element in the given vector with index checking.
				 * @param index an unsigned integer, which points to an entry in the vector
				 * @param vectorObj a vector instance
				 * @throws BALL::Exception::IndexOverflow If the given index exceeds the size of the vector
				 */
				template<class VectorClass>
				typename VectorClass::value_type _safeReading(Position index, VectorClass const& vectorObj) const
				{
					if (index < vectorObj.size())
					{
						return vectorObj[index];
					} 
					else
					{
						throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), vectorObj.size());
					}
				}

				/**
				 * Read an element in the given vector with index checking.
				 * @param index an signed integer, which points to an entry in the vector
				 * @param vectorObj a vector instance
				 * @throws BALL::Exception::IndexOverflow If the given index exceeds the size of the vector
				 * @throws BALL::Exception::IndexUnderflow If the given index is negative
				 */
				template<class VectorClass>
				typename VectorClass::value_type _safeReading(Index index, VectorClass const& vectorObj) const
				{
					if (index < 0)
					{
						throw Exception::IndexUnderflow(__FILE__, __LINE__, index, 0);
					} 
					else if (static_cast<Size>(index) < vectorObj.size())
					{
						return vectorObj[index];
					} 
					else
					{
						throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), vectorObj.size());
					}
				}
		};

		class DPBackTracking;

		/**
		 * is used to remember the pair of table entries of the children of a join node without
		 * copying their configuration
		 */
		typedef pair<DPTable::const_iterator, DPTable::const_iterator> DPPairIt;

		/**
		 * compare two join-table antecessor pairs by comparing their penalties
		 */
		bool compareJoinTablePairs(DPPairIt const& left, DPPairIt const& right);

		/**
		 * Compare pointers of entries of introduce or forget table antecessors by comparing their penalties
		 */
		bool compareTablePointerEntries(DPPointerRow const& left, DPPointerRow const& right);

		/**
		 * Combines backtracked solutions from other DPBackTrackers.
		 * Is used to combine the solutions of computed connection components
		 * of a graph to a solution for the whole graph. Because the bond orders of the
		 * connection components are disjoint, the solutions of the backtrackers can be combined
		 * independently.
		 * This class provides the same public functions as DPBackTracking.
		 * Remarks that this class will start the backtracking of the best solution after constructing,
		 * while DPBackTracking will start only after calling DPBackTracking#nextSolution. Nevertheless
		 * you have to call #nextSolution before you can access the optimal solution.
		 */
		class DPBackTrackingCombiner
		{
			protected:
				/**
				 * The backtrackers. They are managed by this class, so you don't have to care about deleting them.
				 */
				vector<DPBackTracking*> backtrackers;

				/**
				 * The priority queue for the assignments. Because each new backtracked assignment of a connection
				 * component can be combined with each other found assignment of the other connection components,
				 * you get many new solutions in each backtracking step. They are combined inserted into this queue.
				 */
				priority_queue<Assignment, vector<Assignment>, greater<Assignment> > priorityQueue;

				/**
				 * The backtracked solutions of the connection components. They can be combined to build the
				 * solution of the whole graph.
				 */
				vector<vector<Assignment> > componentSolutions;

				/**
				 * the last backtracked and combined solution
				 */
				Assignment assignment;

				/**
				 * maximum number of solutions you want to backtrack
				 */
				Size solutionNumber;

				/**
				 * The penalty of the best solution
				 */
				Penalty optimum;

				/**
				 * This backtracker returns only solutions which have a better penalty than the given upperbound
				 */
				Penalty upperbound;

				/**
				 * Searches for the backtracker which would compute the best next solution (after combining).
				 * Returns it index and the penalty of it's solution.
				 */
				pair<Size, Penalty> getNextMinimumBackTracker() const;

				/**
				 * Combines the component assignment with the whole graph assignment
				 * @param backtracker_index index of the backtracker
				 * @param solutionIndex the number of the assignment of this backtracker, which will be combined
				 */
				void applyAssignment(Size backtracker_index, Size solutionIndex);

				/**
				 * Lets each backtracker backtrack a solution and initialize the combiner.
				 */
				void initialize();

				/**
				 * Combines the given new assignment with each previous found assignments
				 * @param mindex index of the backtracker which found the next best assignment
				 */
				void combineEachSolution(Size mindex);

				/**
				 * copy each DPBackTracking
				 */
				vector<DPBackTracking*> deepCopyOfBacktrackers() const;

			public:
				/**
				 * Construct a DPBackTrackingCombiner with the given FPTBondOrder and the number of solutions
				 * @param bondAssignments vector with pointers to the bond assignments. Call #compute before constructing
				 * @param solutionNumber the maximum number of solutions you want to backtrack
				 */
				DPBackTrackingCombiner(vector<FPTBondOrder*>& bondAssignments,
						Size solutionNumber, Penalty upperbound = INFINITE_PENALTY);

				/**
				 * Construct a DPBackTrackingCombiner with the given FPTBondOrder and the number of solutions
				 * @param bondAssignments vector with the bond assignments. Call #compute before constructing
				 * @param solutionNumber the maximum number of solutions you want to backtrack
				 */
				DPBackTrackingCombiner(vector<FPTBondOrder>& bondAssignments,
						Size solutionNumber, Penalty upperbound = INFINITE_PENALTY);

				/**
				 * Copy constructor
				 */
				DPBackTrackingCombiner(DPBackTrackingCombiner const& copy);

				/**
				 * Destructor. Deletes all backtrackers but not the bond assignment algorithms
				 */
				~DPBackTrackingCombiner();

				/**
				 * Assignment operator
				 */
				DPBackTrackingCombiner& operator=(DPBackTrackingCombiner const& copy);

				/**
				 * return true if there are more solutions to backtrack
				 */
				bool hasMoreSolutions() const;

				/**
				 * computes the next solution. Call #hasMoreSolutions before to avoid an OutOfRange exception.
				 * @throw BALL::Exception::OutOfRange if there is no more solution to backtrack
				 */
				void nextSolution();

				/**
				 * returns the last computed solution
				 */
				Assignment& getSolution();

				/**
				 * returns the last computed solution, const version
				 */
				Assignment const& getSolution() const;

				/**
				 * returns the penalty of the solution which can be backtracked next or INFINITE_PENALTY, if there
				 * is no more solution.
				 */
				Penalty penaltyOfNextSolution() const;
		};

#define DEBUG

#ifdef DEBUG

		/**
		 * returns the name of a bag. Just for debugging
		 */
		String getBagName(TreeWidth<MolecularGraph>::TreeDecompositionBag const& bag, 
		                  TreeWidth<MolecularGraph>::TreeDecomposition graph);

		/**
		 * prints a table on standard output. Just for debugging
		 */
		void PRINT_TABLE(AdditionalBagProperties properties, TreeWidth<MolecularGraph>::TreeDecompositionBag const& bag, 
		                 TreeWidth<MolecularGraph>::TreeDecomposition decomp, MolecularGraph const& graph);
		/**
		 * prints a table entry on standard output. Just for debugging
		 */
		void PRINT_ENTRY(DPConstRow const& row);
		/**
		 * prints an assignment on standard output. Just for debugging
		 */
		void PRINT_ASSIGNMENT(Assignment assignment);

		inline String getBagName(TreeWidth<MolecularGraph>::TreeDecompositionBag const& bag, 
		                         TreeWidth<MolecularGraph>::TreeDecomposition graph)
		{
			typedef TreeWidth<MolecularGraph>::TreeDecompositionBag TreeDecompositionBag;
			String tab;
			switch (boost::get(boost::vertex_bag_type, graph, bag))
			{
				case TreeWidth<MolecularGraph>::INTRODUCE_BAG: tab = "introduce"; break;
				case TreeWidth<MolecularGraph>::FORGET_BAG: tab = "forget"; break;
				case TreeWidth<MolecularGraph>::LEAF_BAG: tab = "leaf"; break;
				case TreeWidth<MolecularGraph>::ROOT_BAG: tab = "root"; break;
				case TreeWidth<MolecularGraph>::JOIN_BAG: tab = "join"; break;
				default: tab = "UNDEFINED"; break;
			}
			return tab;
		}

		inline void PRINT_TABLE(AdditionalBagProperties properties, TreeWidth<MolecularGraph>::TreeDecompositionBag& bag, 
		                        TreeWidth<MolecularGraph>::TreeDecomposition decomp, MolecularGraph const& graph)
		{
			typedef TreeWidth<MolecularGraph>::TreeDecompositionBag TreeDecompositionBag;

			DPTable const& table (*properties.table);
			cout << "<table " << getBagName(bag, decomp);

			if (boost::get(boost::vertex_bag_type, decomp, bag) != TreeWidth<MolecularGraph>::JOIN_BAG)
			{
				cout << "  <-  " << boost::get(boost::vertex_bag_special, decomp, bag);
			}
			cout << "\n\t";

			TreeWidth<MolecularGraph>::TreeDecompositionContent content = boost::get(boost::vertex_bag_content, decomp, bag);
			for (TreeWidth<MolecularGraph>::TreeDecompositionContent::const_iterator iter  = content.begin(); 
			                                                                         iter != content.end(); ++iter)
			{
				cout << *iter << "\t\t";
			}
			cout << "\t|\t";
			std::vector<MolecularGraphTraits::EdgeType> bonds (properties.bonds);
			for (std::vector<MolecularGraphTraits::EdgeType>::const_iterator iter = bonds.begin(); iter != bonds.end(); ++iter)
			{
				MolecularGraphTraits::EdgeType e = *iter;
				cout << source(e, graph) << "~" << target(e, graph) << "\t\t";
			}
			cout << "|\t\tPenalty\n";
			for (DPTable::const_iterator citer = table.begin(); citer != table.end(); ++citer)
			{
				cout << "\t";
				DPConstRow row (*citer);
				DPConfig const& conf (row.first);
				for (vector<Valence>::const_iterator iter(conf.consumedValences.begin()); iter !=
						conf.consumedValences.end(); ++iter)
				{
					cout << *iter << "\t\t";
				}
				cout << "\t|\t";
				for (vector<BondOrder>::const_iterator iter(conf.bondAssignments.begin()); iter !=
						conf.bondAssignments.end(); ++iter)
				{
					cout << *iter << "\t\t";
				}
				cout << "|\t\t" << row.second << "\n";
			}
			cout << ">\n";
		}

		inline  void PRINT_ENTRY(DPConstRow const& row)
		{
			vector<Valence> const& cvs (row.first.consumedValences);
			vector<BondOrder> const& bvs (row.first.bondAssignments);
			for (vector<Valence>::const_iterator iter(cvs.begin()); iter != cvs.end(); ++iter)
			{
				cout << *iter << " ";
			}
			cout << "\t || \t";
			for (vector<Valence>::const_iterator iter(bvs.begin()); iter != bvs.end(); ++iter)
			{
				cout << *iter << " ";
			}
			cout << "\t => \t " << row.second << "\n";
		}

		inline void PRINT_ASSIGNMENT(Assignment assignment)
		{
			cout << assignment.penalty << "\t::\t";
			for (vector<BondOrder>::const_iterator iter(assignment.getBondOrders().begin()); iter != assignment.getBondOrders().end(); ++iter)
			{
				cout << *iter << " ";
			}
			cout << "\n";
		}

#endif

	}
}

#endif // BALL_STRUCTURE_BONDORDERS_DPBONDASSIGNMENTSTRATEGY_H
