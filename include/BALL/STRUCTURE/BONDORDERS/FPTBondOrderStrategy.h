#ifndef BALL_STRUCTURE_BONDORDERS_FPTBONDORDERSTRATEGY_H
#define BALL_STRUCTURE_BONDORDERS_FPTBONDORDERSTRATEGY_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_MATHS_COMMON_H
# include <BALL/MATHS/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_GRAPH_H
# include <BALL/DATATYPE/GRAPH/molecularGraph.h>
#endif

#ifndef BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H
# include <BALL/DATATYPE/GRAPH/graphAlgorithms.h>
#endif

#ifndef BALL_DATATYPE_GRAPH_TREEWIDTH_H
# include <BALL/DATATYPE/GRAPH/treeWidth.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#endif

#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <iterator>
#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

namespace BALL
{
	//TODO: documentation is obsolete!
	/** FPT algorithm for bond order assignment.
	 *  
	 *  This class implements a fixed parameter tractability approach
	 *  for the bond order assignment problem that can be used by 
	 *  the \link AssignBondOrderProcessor AssignBondOrderProcessor \endlink.
	 *
	 * It handles the creation of the nice tree decomposition, the bond assignment computing and the backtracking. 
	 * This class is the only one in the dynamic programming algorithm which expects a molecule. 
	 * All other classes works with graphs (which doesn't have atoms or bonds, but just vertices and edges).
	 * So this class close the gap between the algorithmic bond order problem and it's practical meaning.
	 * The most classes of the bond order algorithm doesn't manage their dependencies, so it's important to delete
	 * them in the correct order. This class wraps about them and manages the memory of their instances itself.
	 * So it's the best to forget, that their are other classes than this: Just use this class if you want to assign
	 * bond orders by the dynamic programming algorithm.
	 *
	 * The class has a default constructor and can be copied and assigned. It saves it's dynamic programming table and the
	 * nice tree decomposition in an extra data structure which is accessed by shared pointers. So you can copy instances
	 * of this class without copying all the algorithm data! Nevertheless you have to copy the backtracking combiner and all
	 * it's remembered solutions.
	 *
	 * After setting the atom container and the penalty map, you have to call the #start method to start the computing.
	 * After that, the instance of this class behaves like an Ball iterator (although it doesn't inherited from it and
	 * doesn't provide all it's functions). You can iterate about each solution by calling ++, and you can check for
	 * further solutions by calling +.
	 *
	 * @code
	 *	FPTBondOrderStrategy algorithm;
	 *	algorithm.setMolecule(someAtomContainer);
	 *	algorithm.setPenaltyMap(somePenaltyMap);
	 *	algorithm.start();
	 *	for (; +algorithm; ++algorithm)
	 *	{
	 *		cout << algorithm->penalty;
	 *	}
	 * @endcode
	 *
	 * Dereferencing an algorithm instance means: return the result of the last computed solution. The result is
	 * an Assignment instance. So it doesn't give information about the concrete bonds, if you don't know their indizes.
	 * There are two ways to handle this: You cann call #getBonds to get a vector with pointers to the bonds. The order
	 * of the bonds in the vector is the same as the order of the bond values on the assignments.
	 * Another way is to call #getBondAssignmentHashMap. This returns a HashMap which maps each bond to it's bond value.
	 * This structure is nearly the same as AssignBondOrderProcessor is using, just with the different that the pointers
	 * references constant bonds.
	 *
	 * An important difference to the AssignBondOrderProcessor is that a bond value of 0 means "single bond", a bond
	 * value of 1 means "double bond" and a bond value of 2 "tripple bound". So you have to increase the bond values
	 * by 1 if you want to use them in AssignBondOrderProcessor or in the Bond class itself.
	 *
	 * Beside the iterator-like functions you can also call the same functions you know from the DPBackTracking and
	 * DPBackTrackingCombiner class. But other than DPBackTracking this class computes the best solution after
	 * calling #start without the need of calling #nextSolution.
	 *
	 * An DPBondAssignmentStrategy instance which hasn't started behaves like an iterator which isn't bound to a container.
	 * You can return the instance into this state by calling #reset. Also changing the molecule or penalty map also
	 * reset the instance. Calling other methods than #start, #reset, #setMolecule, #getMolecule, #setPenaltyMap,
	 * #getPenaltyMap, #setUpperbound, #getUpperbound, #setNumberOfSolutions, #getNumberOfSolutions will throw an
	 * InvalidIterator exception if you forget to call #start before.
	 *
	 * The DPBondAssignmentStrategy expect that the atom container and the penalty map aren't deleted during the
	 * instances' lifetime. If you want do delete one of them, reset the instance and set their atom container and penalty
	 * map to NULL.
	 *
	 */
	class BALL_EXPORT FPTBondOrderStrategy
		: public BondOrderAssignmentStrategy
	{
		public:
			typedef GRAPH::GraphTraits<MolecularGraph>::EdgeType Edge;
			typedef GRAPH::GraphTraits<MolecularGraph>::VertexType VertexType;

			typedef TreeWidth<MolecularGraph>::TreeDecomposition    TreeDecomposition;
			typedef TreeWidth<MolecularGraph>::TreeDecompositionBag TreeDecompositionBag;

			friend class FPTBondOrderAssignment_;

			/**
			 * Penalties are represented as floats
			 */
			typedef float Penalty;

			/**
			 * The valence-type for atoms is represented as integer, although it shouldn't exceed a small
			 * constant value (typical 8)
			 */
			typedef int Valence;

			/**
			 * The BondOrder type is represented as integer, although it shouldn't exceed a small constant
			 * value (typical 3)
			 */
			typedef int BondOrder;

			/** @name Constant Definitions
			*/
			static const Penalty infinite_penalty;
			static const Valence max_valence;

			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/** The maximum penalty for which this FPT strategy will compute solutions.
				 *  Setting this to a value different from infinite_penalty can significantly
				 *  speed up the computation.
				 */
				static String UPPER_PENALTY_BOUND;
			};

			/// Default option values
			struct BALL_EXPORT Default
			{
				/** The maximum penalty for which this FPT strategy will compute solutions.
				 *  Setting this to a value different from infinite_penalty can significantly
				 *  speed up the computation.
				 */
				static Penalty UPPER_PENALTY_BOUND;
			};

			//@}

			FPTBondOrderStrategy(AssignBondOrderProcessor* parent);

			/**
			 * Destructor. Deletes the backtracking. The nice tree decomposition and the dynamic programming tables are
			 * deleted if there are no other FPTBondOrderStrategy instances which access them.
			 */
			virtual ~FPTBondOrderStrategy();

			/**
			 * delete all previous computings and frees the nice tree decomposition and penalty map, if they are not
			 * referenced by another FPTBondOrderStrategy.
			 * After this method you can not access the solutions. Call #start again to compute the bond order.
			 */
			virtual void clear();

			/**
			 * Start the bond assignment computing. Computes the nice tree decomposition and the dynamic programming table.
			 * After calling this method, you can access the solutions
			 * @throw BALL::Exception::NullPointer if the atom container or penalty map aren't set
			 */
			virtual void init();

			virtual bool readOptions(const Options& options);
			virtual void setDefaultOptions();

			/**
			 * Backtracks the next best solution. Call #hasNextSolution first to avoid an OutOfRange exception
			 * @throw BALL::Exception::OutOfRange if there are no more solutions to backtrack
			 * @throw BALL::Exception::InvalidIterator if #start is not called
			 */
			virtual boost::shared_ptr<BondOrderAssignment> computeNextSolution();

		protected:
			/**
			 * A DPConfig_ is an entry in a dynamic programming table. It holds the current bond-order assignments,
			 * and the valences, which were consumed after forgetting some bonds in forget-nodes.
			 * Each DPConfig belongs to just one node in the nice tree decomposition. So it contains just the valences
			 * and bondvalues of the atoms and bonds in the bag.
			 * DPConfigs are computed in Leaf-nodes or by one (or two) ancestor tables by
			 * forgetting, introducing or merging their entries. So even if a DPConfig is empty (as in the root table), you can
			 * get the assignment of all ancestors by backtracking the computing of this DPConfig.
			 */
			class DPConfig_
			{
				public:
					/**
					 * Default constructor
					 */
					DPConfig_();

					/**
					 * Builds a new DPConfig with the given number of atoms and bonds. The valence and bond values
					 * are filled with 0.
					 */
					DPConfig_(Size atoms, Size bonds);

					/**
					 * Builds a new DPConfig with the given atom-valences and bond-values
					 */
					DPConfig_(std::vector<Valence> const& v, std::vector<BondOrder> const&  bo);

					/**
					 * Assignment operator
					 */
					DPConfig_& operator=(DPConfig_ const& copy);

					/**
					 * Builds a new DPConfig and initialize its valences and bond-values with the values which are iterated by the given iterators
					 */
					template<typename ValenceIterator, typename BondIterator>
					DPConfig_(ValenceIterator vit, ValenceIterator vend, BondIterator boit, BondIterator boend) 
						: consumed_valences(vit, vend), 
							bond_assignments(boit, boend)
					{
					}

					/**
					 * @see #compare
					 */
					bool operator < (DPConfig_ const& conf) const;

					/**
					 * @see #compare
					 */
					bool operator > (DPConfig_ const& conf) const;

					/**
					 * @see #compare
					 */
					bool operator <= (DPConfig_ const& conf) const;

					/**
					 * @see #compare
					 */
					bool operator >= (DPConfig_ const& conf) const;

					/**
					 * @see #compare
					 */
					bool operator == (DPConfig_ const& conf) const;

					/**
					 * Comparison is needed to insert a DPConfig into a tree-map. A DPConfig is
					 * equal to another DPConfig if it's valences and bond-values are equal.
					 * Other comparisons are done by comparing each valence and bond value seperately.
					 * You can compare only entries from the same table or from tables which belong to the same
					 * nice tree decomposition bag. The number of valences and bounds have to be equal.
					 * @param other another entry of the same table
					 * @return 1, if this is greather than other, 0, if both entries are equal, otherwise -1.
					 * @throws BALL::Exception::InvalidArgument if two entries of different tables are compared
					 */
					int compare(DPConfig_ const& other) const;

					/**
					 * returns the number of atoms of the bag this entry belongs to.
					 */
					Size numberOfAtoms() const;

					/**
					 * returns the number of bonds of the bag this entry belongs to.
					 */
					Size numberOfBonds() const;

					/**
					 * Every time a bond is forgotten, the consumed valences of its incident vertices are increased by its bond value + 1.
					 * If a atom is forgotten, it's consumed valences holds the final valence assignment of this atom.
					 */
					std::vector<Valence> consumed_valences;

					/**
					 * Holds the current assigned bond values. 
					 * Every time a new bond is visited in an introduce node, entries for each possible bond value of this bond
					 * are inserted into the table.
					 * Single bonds habe a bond value of 0, double bonds have the bond value 1 and tripple bonds have the bond value 2.
					 * So increasing the valences means adding the bond value + 1 (!) to the valence value.
					 */
					std::vector<BondOrder> bond_assignments;

			};

			/**
			 * A single row in a DPTable, which consists of the DPConfig (valences and bond values) and the penalty, which was
			 * computed for the DPConfig and it's ancestors.
			 */
			typedef std::pair<boost::reference_wrapper<DPConfig_>, Penalty> DPRow_;

			/**
			 * After computing a DPTable, we don't modify it's entries (because we need them for backtracking). So usually we
			 * work with const references to the table entries
			 */
			typedef std::pair<boost::reference_wrapper<DPConfig_ const>, Penalty> DPConstRow_;

			/**
			 * Is used to save a reference to a const DPConfig in an object (which isn't 
			 * possible with references, because they are constant and would prevent it's
			 * adding into collections)
			 */
			typedef std::pair<DPConfig_*, Penalty> DPPointerRow_;

			/**
			 * A map which gives fast access to the penalty of a given DPConfig. Is used to compare the penalty of two DPConfigs or to
			 * iterate above all table entries.
			 */
			typedef std::map<DPConfig_, Penalty> DPMap_;

			/**
			 * The dynamic programming table. 
			 * Each nice tree decomposition bag has it's own dynamic programming table. They are computed either from leaf-nodes, by
			 * forgetting or introducing entries from ancestor-tables in forget/introduce nodes or by merging two tables into one in 
			 * join-nodes. The rows in this table are different possible bond assignments for the molecule graph. 
			 * It's columns are the consumed valences of the bag's atoms and bond values of the bag's bonds.
			 */
			class DPTable_
			{
				protected:
					/**
					 * the table data is represented as a tree map, because this allow fast inserting of DPConfigs and removing
					 * duplicates with greater penalty.
					 */
					DPMap_ table;

				public:
					/**
					 * Default constructor
					 */
					DPTable_();

					/**
					 * Copy constructor
					 */
					DPTable_(DPTable_ const& table);

					/**
					 * iterator above the table entries
					 */
					typedef DPMap_::iterator iterator;

					/**
					 * constant iterator above the table entries
					 */
					typedef DPMap_::const_iterator const_iterator;

					/**
					 * returns the penalty of a given DPConfig
					 */
					Penalty operator[](DPConfig_ const& config) const;

					/**
					 * insert a new DPConfig with the given penalty. If the table already contains an equal DPConfig
					 * with a lower penalty, this function does nothing.
					 */
					bool insert(DPConfig_ const& config, Penalty penalty);

					/**
					 * returns the number of rows in this table
					 */
					Size size() const;

					/**
					 * returns the best penalty in this table or infinite_penalty, if the table is empty
					 */
					Penalty bestPenalty() const;

					/**
					 * returns the entry with best penalty. Because this function throws an exception if the table
					 * is empty, you should check this by calling #size() before calling this function.
					 * @throw BALL::Exception::IndexOverflow if table is empty
					 */
					DPConstRow_ bestEntry() const;

					/**
					 * returns an iterator to the first entry.
					 */
					iterator begin();

					/**
					 * returns an iterator to the end of the table
					 */
					iterator end();

					/**
					 * returns a constant iterator to the first entry
					 */
					const_iterator begin() const;

					/**
					 * returns a constant iterator to the end of the table
					 */
					const_iterator end() const;
			};

			/**
			 * Contains informations about a bag which are computed during the
			 * bond assignment algorithm. Later, this informations are used by the
			 * backtracking.
			 */
			class AdditionalBagProperties_
			{
				public:
					/**
					 * Default constructor
					 */
					AdditionalBagProperties_();

					/**
					 * Copy constructor
					 */
					AdditionalBagProperties_(AdditionalBagProperties_ const& copy);

					/**
					 * Assignment operator
					 */
					AdditionalBagProperties_& operator=(AdditionalBagProperties_ const& copy);

					/**
					 * Destructor
					 */
					~AdditionalBagProperties_();

					/**
					 * bonds in this bag
					 */
					std::vector<MolecularGraphTraits::EdgeType> bonds;

					/**
					 * The table of this bag. Is computed during the bond assignment
					 */
					DPTable_ * table;
			};

			class DPBackTracking_;
			class DPBackTrackingCombiner_;

			/**
			 * bond assignment algorithm. 
			 * Traverse in post-order the nice tree decomposition and computes the dynamic programming table for each vertex
			 * This class uses a pointer to a graph, to a penalty map and to a nice tree decomposition. So you should take care that
			 * all three objects aren't deleted during this algorithm, because this class neither uses shared pointers, nor make
			 * copies of the objects.
			 */
			class FPTBondOrderAssignment_
			{
				public:
					friend class DPBackTracking_;
					friend class DPBackTrackingCombiner_;
					friend class GRAPH::PostOrderFolding<TreeDecomposition, TreeDecompositionBag, DPTable_*, FPTBondOrderAssignment_>;

					/**
					 * Construct a new FPTBondOrder with the given molecule, a PenaltyMap and a built nice tree decomposition
					 * @param parent the bond order assignment algorithm
					 * @param ntd a nice tree decomposition of the graph
					 * @param upperbound the algorithm will only compute solutions which are better than the upperbound. By default
					 * 										the upperbound is infinity.
					 */
					FPTBondOrderAssignment_(FPTBondOrderStrategy& parent, boost::shared_ptr<TreeDecomposition>& ntd, 
					                        Penalty upper_bound = infinite_penalty);

					/**
					 * Destructor. Just delete the AdditionalBagProperties_, which were computed during this algorithm. So don't
					 * delete this instance before backtracking. Furthermore this destructor doesn't delete the nice tree decomposition
					 * and the penalty map. You have to take care for this yourself.
					 */
					~FPTBondOrderAssignment_();

					/**
					 * Call this to start the bond assignment computing. This function returns the optimal penalty for the
					 * graph. If you want to know the bond order assignment or the penalties of other solutions, you have to
					 * start the backtracking by DPBackTracking.
					 */
					Penalty compute();

				protected:
				  /**
					 *  A pointer to our parent
					 */
					FPTBondOrderStrategy* parent_;

					/**
					 * the graph with the bonds, which were assigned to a bond value during this algorithm
					 */
					MolecularGraph * molecule_;

					/**
					 * The nice tree decomposition which is built from the graph
					 */
					boost::shared_ptr<TreeDecomposition> ntd_;

					/**
					 * A vector of AdditionalBagProperties_. Contains the bonds and the dynamic programming tables for each vertex
					 * in the nice tree decomposition
					 */
					vector<AdditionalBagProperties_> properties_;

					/**
					 * The algorithm will just compute solutions which are better than the upperbound (they have to be BETTER,
					 * not equal, so "upperbound" is maybe not the best word for it).
					 * The upperbound is infinite_penalty by default, but you can change this if you know that your molecule
					 * has a better solution. This can improve the performance - but usually the algorithm is fast enough to
					 * compute without an upperbound.
					 */
					Penalty upper_bound_;

					/**
					 * The maximum value which can be assigned to a bond
					 */
					BondOrder max_bond_order_;

					/**
					 * The maximum valence which can be assigned to an atom
					 */
					Valence max_valence_;

					/**
					 * Compute the dynamic programming table for a given bag. On this time, the dynamic programming tables of all
					 * vertices of the subtree, rooted by this bag, should are computed.
					 * This function shouldn't be called directly. Instead use the #compute function.
					 * @param bag the bag which is visited during the tree traversal
					 * @param type the type of this bag. Isn't used because the bag gives also access to it's bag type
					 * @param begin an iterator to the first computed child-table of this bag. For join-nodes there are two childs,
					 * 							leaf nodes have no childs. All other vertices have one child.
					 * @param end an iterator the end of the computed child-tables of this bag
					 * @throws BALL::Exception::IllegalTreeOperation if the nice tree decomposition is damaged
					 * @return the dynamic programming table for this bag
					 */
					DPTable_* operator() (TreeDecompositionBag& bag, 
							std::vector<DPTable_*>::const_iterator begin, std::vector<DPTable_*>::const_iterator end);

					/**
					 * Remember which bond belonging to the given bag. 
					 * Because this information is reused in backtracking, the bonds
					 * are saved in the AdditionalBagProperties_ object.
					 * @param bag the bag of the nice tree decomposition
					 * @return a vector of edges which are only incident to vertices in this bag
					 */
					std::vector<MolecularGraphTraits::EdgeType> getBondsInBag(TreeDecompositionBag& bag);

					/**
					 * computes the dynamic programming table for a given leaf node.
					 * This is done by filling an entry with just one valence-entry (with value 0) into the table.
					 * So the complexity of this operation is constant.
					 * @param bagProperties contains the dynamic programming table
					 */
					void computeLeafIntroduceBag(AdditionalBagProperties_& bag_properties);

					/**
					 * computes the dynamic programming table for a given introduce node.
					 * This is done by insert a new column for the introduced vertex in each entry, filled with the value 0.
					 * If the bag introduces new bonds, each entry is duplicated for each possible bond value. Then new bond order
					 * columns are inserted into the entries, filled with the different possible combinations of bond values.
					 * So at the end of this function, the bag contains each possible bond order combination of the introduced bonds. This
					 * could be very heavy, but usually we have never more than two or three bonds in a bag. So the complexity of this
					 * operation is O([number of bond values]^[number of introduced bonds] * [number of table entries] * insertion-costs),
					 * where insertion-costs are logarithmic, because the table is implemented as search tree.
					 */
					void computeIntroduceBag(TreeDecompositionBag& bag, 
																	 DPTable_& child, AdditionalBagProperties_& bag_properties);

					/**
					 * computes the dynamic programming table for a given forget node.
					 * This is done by removing the columns of each bond which is incident to the forget node.
					 * It's bond value (+1) is added to the consumed valence of the both incident atoms. After this,
					 * the forget-atom is removed itself and the penalty for its valences is added to the penalty of the table entry.
					 * If multiple entries have the same bond and valence values, just the entry with the lowest penalty is kept.
					 * So this operation removes many entries from the table. This is important, because introduce nodes insert many
					 * entries in the table. So during the algorithm the tables doesn't grow very much, because there is a sucession
					 * of forgetting table entries and introducing new table entries.
					 * The complexity of forgetting a vertex is linear in O(number of bonds + number of atoms + PenaltyMap lookup).
					 * This is done for each table entry. Furthermore there comes a lookup cost for finding duplicate entries in the table
					 * which is done in O(log(number of entries) * (number of bonds + number of atoms)),
					 * because the table is implemented as search tree.
					 */
					void computeForgetBag(TreeDecompositionBag& bag,
																DPTable_& child, AdditionalBagProperties_& property);

					/**
					 * computes the dynamic programming table for a given join node.
					 * The child bags of a join node contains the same vertices and bonds as the join bag itself. So the table
					 * of the join bag is computed by merging the tables of the both childs.
					 * This is done by finding each pair of table entries from first and second child, which have the same
					 * bond values but - maybe - different penalties and valence values. The valence values and penalties of this pairs
					 * are added and the merged entry is inserted in the table of the join node. Again, entries with the same
					 * values but greater penalty than other one are removed from the table.
					 * To avoid quadratic complexity by finding pairs, the table entries are inserted into a search tree which
					 * compares just the bond values of its keys. So just the entries with the same bond values are combined, which is
					 * quadratic in worst case, too. But usually the number of entries in the table is not so huge, because the forget bags
					 * remove many entries which can not have better solutions than the kept one.
					 */
					void computeJoinBag(TreeDecompositionBag& bag,
							DPTable_& leftChild, DPTable_& rightChild, AdditionalBagProperties_& bag_properties);

					/**
					 * computes the dynamic programming table for a given root node.
					 * A root bag is nothing more than a forget bag, so this operations does the same as the #computeForgetBag
					 * function.
					 */
					void computeRootBag(TreeDecompositionBag& bag, DPTable_& child, AdditionalBagProperties_& bag_properties);

					/**
					 * delete the column of the forgotten vertex and it's incident bonds in a given table entry. Sum the bond values of
					 * the forgotten bonds to the valence values of their incident non-forgotten vertices.
					 * This function is reused in DPBackTracking and is called in the #computeForgetBag function.
					 */
					Penalty forgetInnerVertexIn(TreeDecompositionBag& bag, DPConstRow_ child_row, DPConfig_& entry, 
																			std::vector<MolecularGraphTraits::EdgeType>& child_bonds, Size forgotten_index);

			};

			/**
			 * This data structure contains all algorithm data which is generated during the #start function
			 * and which can be shared above more than one FPTBondOrderStrategy "iterator", if they compute all
			 * the same molecule with the same penalty table.
			 */
			class ComputingData_
			{
				public:
					/**
					 * Default constructor
					 */
					ComputingData_();

					/**
					 * Deletes the dynamic programming table, the molecule graph and the nice tree decomposition
					 */
					~ComputingData_();

					/**
					 * The bond assignments for each connection component of this molecule
					 */
					vector<FPTBondOrderAssignment_*> bond_assignments;

					/**
					 * the molecule graph
					 */
					MolecularGraph *molecule_graph;

					/**
					 * The nice tree decompositions for each connection component
					 */
					boost::shared_ptr<TreeWidth<MolecularGraph> > tw;

					/**
					 * a vector with pointers to the bonds of the atom container. The order of this bonds in the vector
					 * is the same as the order of the bond values in the assignments.
					 */
					vector<Bond const *> bonds;
			};

			/**
			 * An assignment is the solution of the bond assignment problem. It consists of the penalty of this solution,
			 * and a vector of bond values for each bond. The bond order is just a vector of bond values. If you want a HashMap which
			 * maps the concrete bonds to their bond value, you should use the FPTBondOrderStrategy class instead, which can
			 * construct such a HashMap from an assignment.
			 * Nevertheless the index of the bonds has a strict order: Each bond which points to atoms with indizes u and v can be
			 * represented as edge (u < v). An edge is less than another edge, is either it's u is less, or if it's u is
			 * equal to the other's u and it's v is less than the other's v. The indizes of the atoms is given by the graph structure.
			 * The first atom has index 0.
			 * We call an assignment valid to a given UndirectedGraph and PenaltyMap, if it's penalty is equal to the sum of penalties
			 * of it's atoms in the given penalty map. Assignments which was computed by the FPTBondOrder algorithm should
			 * be valid to the same graph and penalty map which was used by the algorithm.
			 */
			class Assignment_
			{
				public:
					/**
					 * build a new, empty assignment
					 */
					Assignment_();

					/**
					 * build an assignment with the given number of bonds. The bond values are filled with 0, the penalty
					 * is initialized with infinite_penalty. Such a bond assignment don't have to be valid!
					 */
					Assignment_(Size num_bonds);

					/**
					 * Copy constructor
					 */
					Assignment_(Assignment_ const& copy);

					/**
					 * Build an assignment from a given bond value vector and penalty. Such a bond assignment don't have to
					 * be valid!
					 */
					Assignment_(std::vector<BondOrder> const& bonds, Penalty penalty);

					/**
					 * Assignment operator
					 */
					Assignment_& operator=(Assignment_ const& copy);

					/**
					 * Returns the value for the bond with the given index.
					 * @throw BALL::Exception::IndexOverflow unless index is less than the number of bonds in this assignment
					 */
					BondOrder operator [](Size index) const;

					/**
					 * Gives access to the bond value of the bond with the given index.
					 * @throw BALL::Exception::IndexOverflow unless index is less than the number of bonds in this assignment
					 */
					BondOrder& operator [](Size index);

					/**
					 * Combines two disjoint assignments. Two assignments are disjoint, if for each bond index i there is a 0 value in
					 * at least one of the both assignments. Combining such disjoint assignments means: Insert for each index i the
					 * not-zero bond value of the both assignments.
					 * Such disjoint assignments exist in separated computing of connection components of the same graph. Each computing
					 * just assign the bond values of the bonds, which were connected to the connection component. The other bond values
					 * are 0. The combining of assignments of all connection components of a graph creates an assignment of the whole
					 * graph. This approach is important, because the FPTBondOrder algorithm is just defined for connected graphs.
					 * @throw BALL::Exception::InvalidArgument if the two assignments have a different number of bonds
					 */
					void combine(Assignment_ const& other);

					/**
					 * returns a const reference to the bond values
					 */
					std::vector<BondOrder> const& getBondOrders() const;

					/**
					 * Comparison is needed to insert such an assignment into a priority queue or a search tree.
					 * An assignment x is less than an assignment y, if the penalty of x is less than the penalty of y.
					 * If two assignments have the same penalty, the comparison is done by comparing each bond value separately.
					 */
					int compare(Assignment_ const& a) const;

					/**
					 * @see #compare
					 */
					bool operator < (Assignment_ const& a) const;

					/**
					 * @see #compare
					 */
					bool operator > (Assignment_ const& a) const;

					/**
					 * @see #compare
					 */
					bool operator <= (Assignment_ const& a) const;

					/**
					 * @see #compare
					 */
					bool operator >= (Assignment_ const& a) const;

					/**
					 * @see #compare
					 */
					bool operator == (Assignment_ const& a) const;

					/**
					 * Returns true if the assignment is valid to the given UndirectedGraph and PenaltyMap. This happens if the
					 * sum of penalties of each atom with the valence as sum of the bond values of each incident bond is equal to the
					 * penalty of this assignment.
					 * @param molecule should be the same graph which was used to compute this assignment
					 * @param PenaltyMap should be the same penalty map which was used to compute this assignment
					 * @return true if the assignment is valid, false otherwise
					 */
					bool isValid(MolecularGraph& molecule, FPTBondOrderStrategy& parent);

					/**
					 * The penalty of this assignment
					 */
					Penalty penalty;

				protected:
					/**
					 * A vector with bond values. 0 is single bond, 1 is double bond, 2 is tripple bond
					 */
					std::vector<BondOrder> bonds_;

			};

			/**
			 * A comparator, which searches DPConfigs in a DPJoinMap. Two DPConfigs are
			 * equal, if they have the same bond values, even if they have different
			 * consumed valences.
			 */
			struct DPJoinMapComparator_
			{
				/**
				 * Less-operator. Returns true if leftp is smaller than rightp.
				 * @throws BALL::Exception::InvalidArgument if the both DPConfig entries can not be compared
				 */
				bool operator() (DPConfig_ const* leftp, DPConfig_ const* rightp) const;

				/**
				 * returns 1, if leftp is greather than rightp, 0 if both are equal and -1 otherwise.
				 * You can compare only childs from the same join bag.
				 * @throws BALL::Exception::InvalidArgument if the both DPConfig entries can not be compared
				 */
				int compare(DPConfig_ const* leftp, DPConfig_ const* rightp) const;
			};

			/** A comparator for edges in the molecular graph.
			 */
			class EdgeComparator_
			{
				public:
					typedef GRAPH::GraphTraits<MolecularGraph>::EdgeType Edge;

					EdgeComparator_(MolecularGraph* graph)
						: graph_(graph)
					{ }

					bool operator() (Edge const& e1, Edge const& e2);

				protected:
					MolecularGraph* graph_;
			};

			/**
			 * A BackTrackingState_ is a position in the tree during the backtracking. 
			 * It contains an assignment, which
			 * contains all bond values of the bonds which were forgotten during the backtracking. 
			 * Furthermore it contains the table entry of the dynamic programming table
			 */
			class BackTrackingState_
			{
				public:
					/**
					 * Default constructor
					 */
					BackTrackingState_();

					/**
					 * build a BackTrackingState for a bag with the given number of bonds
					 */
					BackTrackingState_(Size bonds);

					/**
					 * Copy constructor
					 */
					BackTrackingState_(BackTrackingState_ const& other);

					/**
					 * Assignment operator
					 */
					BackTrackingState_& operator=(BackTrackingState_ const& other);

					/**
					 * Comparison is needed to insert BackTrackingState_ entries into a search tree. 
					 * Comparison is done by compare the assignments of the states.
					 */
					int compare(BackTrackingState_ const& other) const;

					/**
					 * @see #compare
					 */
					bool operator < (BackTrackingState_ const&) const;

					/**
					 * @see #compare
					 */
					bool operator > (BackTrackingState_ const&) const;

					/**
					 * @see #compare
					 */
					bool operator <= (BackTrackingState_ const&) const;

					/**
					 * @see #compare
					 */
					bool operator >= (BackTrackingState_ const&) const;

					/**
					 * @see #compare
					 */
					bool operator == (BackTrackingState_ const&) const;

					/**
					 * Assignment which contains all bond values of the bonds which were forgotten during the backtracking.
					 * Such an assignment is valid, if the BackTrackingState is pointing to the end of the backtracking
					 */
					Assignment_ assignment;

					/**
					 * the table entry, which this backtracking state follows. This is the entry of the bag, this BackTrackingState is
					 * pointing to.
					 */
					DPConfig_ config;

					/**
					 * If the BackTrackingState reachs a join node, it can just follow one of it's both children. So it have to remember
					 * which entry of the table of the second child it choosed. This is done in this stack. It contains the table entry
					 * of the right child and the index of this child in pre-order.
					 */
					std::stack<std::pair<DPConfig_, Size> > join_branches;

					/**
					 * pre-order index of this bag. If you traverse the tree in pre-order and give each vertex a number from 0 to n-1,
					 * this number is the pre-order index.
					 */
					Size index;


			};

			/**
			 * is used to remember the pair of table entries of the children of a join node without
			 * copying their configuration
			 */
			typedef std::pair<DPTable_::const_iterator, DPTable_::const_iterator> DPPairIt_;

			/**
			 * compare two join-table antecessor pairs by comparing their penalties
			 */
			static bool compareJoinTablePairs_(DPPairIt_ const& left, DPPairIt_ const& right);

			/**
			 * Compare pointers of entries of introduce or forget table antecessors by comparing their penalties
			 */
			static bool compareTablePointerEntries_(DPPointerRow_ const& left, DPPointerRow_ const& right);

			/**
			 * A map which remember pointers to DPConfigs of a child of a join-node. It uses a DPJoinMapComparator to find
			 * entries with equal bond-values very fast.
			 */
			typedef std::multimap<DPConfig_ const*, Penalty, DPJoinMapComparator_> DPJoinMap_;

			/**
			 * The backtracking algorithm. It traverses the nice tree decomposition in pre-order and chooses from the next table
			 * the entry, which was used to computed the previous one.
			 * We call a table entry successor, if we choosed it in the previous backtracking step. And we call a table entry
			 * antecessor, if it was used in the bond assignment algorithm to compute the successor.
			 * The backtracking starts in the root vertex which has just one table entry which becomes the first successor.
			 * The antecessor entry is gotten by finding the entry of the root's child bag, which can be used to compute the table
			 * entry in the root bag. This entry is used as successor for the next table. Often more than one table entry can be used
			 * to compute the successor entry. If we don't look at the penalty, there are even more possible antecessor entries.
			 * In each step the algorithm uses any antecessor, which computes the successor with the correct penalty. The other possible
			 * antecessors are inserted into a priority queue. After finding the successor of the last vertex in the tree,
			 * we can get the assignment by insert all bond values of the forgotten bonds in the choosed antecessor entries
			 * of visited forgot bags. Furthermore we can backtrack the next best solution by picking up the best
			 * antecessor in our priority queue and continue the backtracking with this entry as new successor.
			 * Remark that it's easy to get the penalty of the next solution (because we just have to look into the penalty of the
			 * antecessor entry). Just the computing of the bond order requires to traverse the whole tree.
			 * Furthermore if we don't specify an upperbound, this backtracking algorithm can iterate about EACH possible solution
			 * of this bond order problem.
			 */
			class DPBackTracking_
			{
				public:
					typedef TreeWidth<MolecularGraph>::TreeDecomposition        TreeDecomposition;
					typedef TreeWidth<MolecularGraph>::TreeDecompositionBag     TreeDecompositionBag;
					typedef TreeWidth<MolecularGraph>::TreeDecompositionContent TreeDecompositionContent;

					/**
					 * Construct a new DPBackTracking_ for a given FPTBondOrder algorithm, which backtracks not more than
					 * maxNumberOfSolutions. By default, the backtracking backtracks only the optimal solution.
					 * You have to call the FPTBondOrder#compute method before constructing the DPBackTracking_.
					 * Furthermore you should take care to delete the DPBackTracking_ before the FPTBondOrder, because this
					 * class operates on a pointer to the bond assignment algorithm, not on a copy.
					 * @param bondAssignment a reference to a FPTBondOrder which is already computed
					 * @param maxNumberOfSolutions the number of solutions you want to backtrack. Is by default 1. The size of the
					 * 															priority queue can never be greater than maxNumberOfSolutions
					 */
					DPBackTracking_(FPTBondOrderAssignment_& bond_assignment, Size max_number_of_solutions,
					                std::vector<MolecularGraphTraits::EdgeType> const& bonds, Penalty upperbound = infinite_penalty);

					/**
					 * Copy constructor
					 */
					DPBackTracking_(DPBackTracking_ const& copy);

					/**
					 * Destructor. Removes just the BackTrackingStates, not the bond assignment algorithm instance.
					 */
					~DPBackTracking_();

					/**
					 * Assignment operator
					 */
					DPBackTracking_& operator= (DPBackTracking_ const& copy);

					/**
					 * returns the current solution. Remark that after constructing the backtracking, there is no solution computed. So
					 * you have to call #nextSolution first.
					 * @throw BALL::Exception::NullPointer if you forgot to call #nextSolution
					 */
					Assignment_& getSolution();

					/**
					 * returns the current solution, const version. 
					 * Remark that after constructing the backtracking, there is no solution computed. So
					 * you have to call #nextSolution first.
					 * @throw BALL::Exception::NullPointer if you forgot to call #nextSolution
					 */
					Assignment_ const& getSolution() const;

					/**
					 * returns true if there is another solution. Call this method before calling
					 * #nextSolution.
					 */
					bool hasMoreSolutions() const;

					/**
					 * Computes the next best solution. You can access it by calling #getSolution.
					 * @throw BALL::Exception::OutOfRange if there are no more solutions
					 */
					void nextSolution();

					/**
					 * returns the penalty of the next best solution. If there are no more solutions,
					 * this function returns infinite_penalty.
					 */
					Penalty penaltyOfNextSolution() const;

					void clear();

					void preorder(TreeDecompositionBag node, TreeDecomposition&)
					{
						bags_->push_back(node);
					}

					void inorder(TreeDecompositionBag, TreeDecomposition&)
					{
					}

					void postorder(TreeDecompositionBag, TreeDecomposition&)
					{
					}
					
				protected:

					/**
					 * Defines the Min-order for BackTrackingState-pointer instances in the queue
					 */
					struct StateComparator_
					{
						/**
						 * Comparison is done by dereferencing the pointer
						 */
						bool operator () (BackTrackingState_ const * left, BackTrackingState_ const * right) const;
					};

					/**
					 * The instance of the FPTBondOrder algorithm, which gives access to the computed tables and
					 * the nice tree decomposition. This class doesn't make a copy of the bondAssignment, so take care
					 * that the bondAssignment isn't deleted before the instance of this class.
					 */
					FPTBondOrderAssignment_* bond_assignment_;

					/**
					 * The current state of the backtracking. Each other state is in the priority queue
					 */
					BackTrackingState_* current_state_;

					/**
					 * priority queue for backtracking states. It is implemented as search tree, because we need also
					 * access to the worst element (to limit the queues size).
					 */
					std::multiset<BackTrackingState_*, StateComparator_> queue_;

					/**
					 * the maximum number of solutions we want do backtrack.
					 */
					Size max_num_solutions_;

					/**
					 * A sorted vector of the edges of the graph. The bond values in the assignments are in the same order
					 * as the edges in this vector.
					 */
					std::vector<MolecularGraphTraits::EdgeType> const* bonds_;

					/**
					 * The nice tree decomposition bags in pre-order
					 */
					boost::shared_ptr<std::vector<TreeDecompositionBag> > bags_;

					/**
					 * maxHeapSize is the maxNumberOfSolutions - the number of backtracked solutions. So this attribute
					 * contains the current number of solutions we want to backtrack.
					 */
					Size max_heap_size_;

					/**
					 * The number of solutions produced so far.
					 */
					Size num_computed_solutions_;

					/**
					 * current upperbound. This algorithm will just iterate solutions which are better than this upperbound;
					 */
					Penalty upper_bound_;

					typedef vector<TreeDecompositionBag> BagVector;

					/**
					 * returns the dynamic programming table of the bag with the given pre-order index
					 */
					DPTable_& getTable(Size order);

					/**
					 * returns the bag properties of the bag with the given pre-order index
					 */
					AdditionalBagProperties_& getProperties(Size order);

					/**
					 * Leaf-Nodes have no antecessors. So either the computing is finished,
					 * or there is an unfinished join node in join branch stack which has to
					 * be computed next.
					 */
					void visitLeaf(BackTrackingState_& state);

					/**
					 * search the both antecessors of this join node. This is the pair of entries,
					 * which bond values are the same as the successor's bond values and which
					 * sum of consumed valences are the same as the successor's consumed valences.
					 * The best left entry becomes the new current state. The right entry is pushed
					 * on top of the state's join branch stack. Other possible pairs of antecessors
					 * are inserted into the priority queue
					 * @param state the current backtracking state
					 * @param bag the join bag with the successor entry
					 * @param leftTable the table of the first child
					 * @param rightTable the table of the second child
					 */
					void visitJoin(BackTrackingState_& state, TreeDecompositionBag& bag, 
												 DPTable_& leftTable, DPTable_& rightTable);

					/**
					 * search the antecessor of this forget node. This is the entry which is equal to
					 * the successor after forgetting the forget-vertex and it's incident bonds.
					 * @param state the current backtracking state
					 * @param bag the forget bag with the successor entry
					 * @param table the child's table with the antecessor entry
					 */
					void visitForget(BackTrackingState_& state, TreeDecompositionBag& bag, DPTable_& table);

					/**
					 * search antecessor of this introduce node. This is the same entry
					 * as the successor, but without the introduced columns
					 * @param state the current backtracking state
					 * @param bag the introduce bag with the successor entry
					 * @param table the child's table with the antecessor entry
					 */
					void visitIntroduce(BackTrackingState_& state, TreeDecompositionBag& bag, DPTable_& table);

					/**
					 * searchs the index of this bond in the assignment array.
					 * Because there is a strict ordering of bonds, this search
					 * is computed as binary search in logarithmic time.
					 */
					Size bondIndexFor(MolecularGraphTraits::EdgeType bond) const;

					/**
					 * remembers the given state as another possible solution with higher penalty.
					 * The state is inserted in the queue.
					 * If the algorithm found enough solutions, it updates the upperbound to the worst solution in the
					 * queue. So just solutions with better penalty are inserted into the queue.
					 * @param state an alternative antecessor which has a greater or equal penalty than the choosed one
					 */
					void remember(BackTrackingState_& state);

					/**
					 * Checks if the penalty of this solution is good enough for backtracking.
					 * This happens if the penalty is better than the upperbound
					 */
					bool isSolutionNeeded(Penalty penalty);

					/**
					 * Is called by visitForget. It writes the values of all forgotten bonds into
					 * the state's assignment.
					 * @param state the current state
					 * @param bag the forget bag
					 * @param antecessor the choosed entry in the bag's child bag
					 * @param forgottenVertex the vertex which is forgotten in the forget bag
					 */
					void setStateAssignment(BackTrackingState_& state, TreeDecompositionBag& bag, 
																	DPConfig_& antecessor, MolecularGraphTraits::VertexType forgotten_vertex);

					/**
					 * Make the antecessor entry to the new successor entry of the given state and
					 * adding the penalty
					 * @param state the backtracking state
					 * @param antecessor the choosed entry which becomes the new successor
					 * @param additionalPenalty the penalty which is added to the best previous solution for choosing this antecessor
					 */
					void extendState(BackTrackingState_& state, DPConfig_ const& antecessor, Penalty additional_penalty);

					/**
					 * Remember the choosed entry of the right child's table by adding it into the joinBranch stack.
					 * @param state the backtracking state
					 * @param child the right child of the join bag
					 * @param antecessor the choosed entry in the right child's table
					 */
					void branchState(BackTrackingState_& state, TreeDecompositionBag const& child, DPConfig_ const& antecessor);

			};

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
			class DPBackTrackingCombiner_
			{
				public:
					/**
					 * Construct a DPBackTrackingCombiner with the given FPTBondOrder and the number of solutions
					 * @param bondAssignments vector with pointers to the bond assignments. Call #compute before constructing
					 * @param solutionNumber the maximum number of solutions you want to backtrack
					 */
					DPBackTrackingCombiner_(std::vector<FPTBondOrderAssignment_*>& bond_assignments,
							                    Size solution_number, Penalty upper_bound = infinite_penalty);

					/**
					 * Copy constructor
					 */
					DPBackTrackingCombiner_(DPBackTrackingCombiner_ const& copy);

					/**
					 * Destructor. Deletes all backtrackers but not the bond assignment algorithms
					 */
					~DPBackTrackingCombiner_();

					void clear();

					/**
					 * Assignment operator
					 */
					DPBackTrackingCombiner_& operator = (DPBackTrackingCombiner_ const& copy);

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
					Assignment_& getSolution();

					/**
					 * returns the last computed solution, const version
					 */
					Assignment_ const& getSolution() const;

					/**
					 * returns the penalty of the solution which can be backtracked next or INFINITE_PENALTY, if there
					 * is no more solution.
					 */
					Penalty penaltyOfNextSolution() const;

					/**
					 * A sorted vector of the edges of the graph. The bond values in the assignments are in the same order
					 * as the edges in this vector.
					 */
					std::vector<MolecularGraphTraits::EdgeType> sorted_edges;

				protected:
					/**
					 * The backtrackers. They are managed by this class, so you don't have to care about deleting them.
					 */
					std::vector<DPBackTracking_*> backtrackers_;

					/**
					 * The priority queue for the assignments. Because each new backtracked assignment of a connection
					 * component can be combined with each other found assignment of the other connection components,
					 * you get many new solutions in each backtracking step. They are combined inserted into this queue.
					 */
					std::priority_queue<Assignment_, std::vector<Assignment_>, std::greater<Assignment_> > priority_queue_;

					/**
					 * The backtracked solutions of the connection components. They can be combined to build the
					 * solution of the whole graph.
					 */
					std::vector<std::vector<Assignment_> > component_solutions_;

					/**
					 * the last backtracked and combined solution
					 */
					Assignment_ assignment_;

					/**
					 * maximum number of solutions you want to backtrack
					 */
					Size solution_number_;

					/**
					 * The penalty of the best solution
					 */
					Penalty optimum_;

					/**
					 * This backtracker returns only solutions which have a better penalty than the given upperbound
					 */
					Penalty upper_bound_;

					/**
					 * Searches for the backtracker which would compute the best next solution (after combining).
					 * Returns it index and the penalty of it's solution.
					 */
					std::pair<Size, Penalty> getNextMinimumBackTracker_() const;

					/**
					 * Combines the component assignment with the whole graph assignment
					 * @param backtracker_index index of the backtracker
					 * @param solutionIndex the number of the assignment of this backtracker, which will be combined
					 */
					void applyAssignment_(Size backtracker_index, Size solution_index);

					/**
					 * Lets each backtracker backtrack a solution and initialize the combiner.
					 */
					void initialize_();

					/**
					 * Combines the given new assignment with each previous found assignments
					 * @param mindex index of the backtracker which found the next best assignment
					 */
					void combineEachSolution_(Size mindex);

					/**
					 * copy each DPBackTracking
					 */
					std::vector<DPBackTracking_*> deepCopyOfBacktrackers_() const;

			};


			/// Initialize pointers to penalty data
			void initPenaltyData_();

			/// Return penalty value for given vertex and valence
	    Penalty getPenaltyFor_(MolecularGraphTraits::VertexType vertex, Valence valence) const;

			/**
			 * contains for each block-index the position in the penalties_ array where this block starts
			 */
			std::vector<int> const* penalties_;

			/**
			 * contains for each block_index the number of saved penalties for the atoms in the block
			 */
			std::vector<Position> const * block_to_start_idx_;

			/**
			 * contains for each block_index the number of saved penalties for the atoms in the block
			 */
			std::vector<Size> const * block_to_length_;

			/**
			 * contains the valence of the first entry of the given block
			 */
			std::vector<int> const * block_to_start_valence_;

			/**
			 * contains the block index for the given atom
			 */
			std::vector<std::vector<int> > const* atom_to_block_;

			/**
			 * A shared pointer to the computing data, so that you can copy this instance without copying
			 * all the computing data
			 */
			boost::shared_ptr<ComputingData_> computing_data_;

			/**
			 * The backtracking combiner_
			 */
			boost::shared_ptr<DPBackTrackingCombiner_> combiner_;
	};
}
#endif // BALL_STRUCTURE_BONDORDERS_FPTBONDORDERSTRATEGY_H
