#ifndef BALL_STRUCTURE_BONDORDERS_DPBONDASSIGNMENT_H
#define BALL_STRUCTURE_BONDORDERS_DPBONDASSIGNMENT_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_LIMITS_H
# include <BALL/COMMON/limits.h>
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

#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <iterator>
#include <queue>

#include <boost/shared_ptr.hpp>

namespace BALL
{
	namespace DPBondOrder
	{
		using boost::shared_ptr;

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
		 * own values in the constructor of DPBondAssignmentAlgorithm, DPBackTracking and DPBackTrackingCombiner.
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
		 * A PenaltyMap maps the valence of a specific atom to the penalty for this configuration.
		 * This is a virtual abstract class, so you can use different implementations.
		 * Impossible configurations should have the penalty INFINITE_PENALTY
		 */
		class PenaltyMap
		{
			public:
				virtual Penalty getPenaltyFor(MolecularGraphTraits::VertexType vertex, Valence valence) const = 0;
				virtual BondOrder maxBondOrder() const = 0;
				virtual Valence maxValence() const = 0;
				virtual ~PenaltyMap();
		};

		/**
		 * A very simple implementation which holds an array with (VALENCE_SIZE * number of Atoms) penalties.
		 * Access to the penalty for a given atom with a given valence is realised through simple index-arithmetic
		 */
//		class PenaltyMapForDebugging 
//			: public PenaltyMap
//		{
//			public:
//				/**
//				 * Build a PenaltyMap with the penalties given in the vector. Impossible configurations
//				 * should have the penalty INFINITE_PENALTY
//				 * @param penalties vector with penalties. Should have size of (VALENCE_SIZE * number of Atoms)
//				 */
//				PenaltyMapForDebugging(std::vector<Penalty> const& penalties);
//				/**
//				 * Build a PenaltyMap with penalties for each vertex in the given graph. The table is filled with
//				 * INFINITE_PENALTY values, so you should insert the correct penalties after construction.
//				 */
//				PenaltyMapForDebugging(MolecularGraph const& graph);
//				/**
//				 * Copy constructor
//				 */
//				PenaltyMapForDebugging(PenaltyMapForDebugging const& copy);
//				/**
//				 * Build a PenaltyMap with penalties for each vertex in the given graph. The table is filled with
//				 * the value in fillWith.
//				 */
//				PenaltyMapForDebugging(MolecularGraph const& graph, Penalty fillWith);
//				/**
//				 * Give access to the penalty for the given atom with the given valence. This operation returns a reference,
//				 * so you can change the penalty value.
//				 */
//				Penalty& penalty(MolecularGraphTraits::VertexType atom, Valence value);
//				/**
//				 * Const access to the penalty for the given atom with the given valence. Furthermore this function overwrites
//				 * the abstract definition in PenaltyMap, so you can use instances of this class as PenaltyMap.
//				 */
//				Penalty getPenaltyFor(MolecularGraphTraits::VertexType vertex, Valence valence) const;
//
//				/**
//				 * returns BOND_SIZE
//				 */
//				BondOrder maxBondOrder() const;
//
//				/**
//				 * returns VALENCE_SIZE
//				 */
//				Valence maxValence() const;
//
//			protected:
//				/**
//				 * vector of penalties. penalties[index * MAX_VALENCE] returns the valence 0 for the atom with index as vertexId.
//				 * Each atom has MAX_VALENCE penalty values in this array, even if many of them could have the value INFINITE_PENALTY.
//				 */
//				vector<Penalty> penalties;
//		};
//
		/**
		 * A more complex implementation of a PenaltyMap. This implementation is more memory efficient than PenaltyMapForDebugging,
		 * because it doesn't save MAX_VALENCE values for each atom and defines penalties for more than one atom.
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
		class BallPenaltyMap : public PenaltyMap
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
						std::map<MolecularGraphTraits::VertexType, vector<int> > & atom_to_block_, int maxBondOrder_, int maxValence_);

				/**
				 * @return penalty for the atom with index vertex and the given valence
				 */
				Penalty getPenaltyFor(MolecularGraphTraits::VertexType vertex, Valence valence) const;

				BondOrder maxBondOrder() const;

				Valence maxValence() const;

			protected:
				/**
				 * contains for each block-index the position in the penalties_ array where this block starts
				 */
				std::vector<int> const * penalties_;
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
				std::map< MolecularGraphTraits::VertexType, vector<int> > * atom_to_block_;

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
						} else
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
						} else if (static_cast<Size>(index) < vectorObj.size())
						{
							return vectorObj[index];
						} else
						{
							throw Exception::IndexOverflow(__FILE__, __LINE__, static_cast<Index>(index), vectorObj.size());
						}
					}
		};

		/**
		 * A DPConfig is an entry in a dynamic programming table. It holds the current bond-order assignments,
		 * and the valences, which were consumed after forgetting some bonds in forget-nodes.
		 * Each DPConfig belongs to just one node in the nice tree decomposition. So it contains just the valences
		 * and bondvalues of the atoms and bonds in the bag.
		 * DPConfigs are computed in Leaf-nodes or by one (or two) ancestor tables by
		 * forgetting, introducing or merging their entries. So even if a DPConfig is empty (as in the root table), you can
		 * get the assignment of all ancestors by backtracking the computing of this DPConfig.
		 */
		class DPConfig
		{
			public:
				/**
				 * Every time a bond is forgotten, the consumed valences of its incident vertices are increased by its bond value + 1.
				 * If a atom is forgotten, it's consumed valences holds the final valence assignment of this atom.
				 */
				vector<Valence> consumedValences;

				/**
				 * Holds the current assigned bond values. Every time a new bond is visited in an introduce node, entries for each possible bond value of this bond
				 * are inserted into the table.
				 * Single bonds habe a bond value of 0, double bonds have the bond value 1 and tripple bonds have the bond value 2.
				 * So increasing the valences means adding the bond value + 1 (!) to the valence value.
				 */
				vector<BondOrder> bondAssignments;

				/**
				 * Default constructor
				 */
				DPConfig();

				/**
				 * Copy constructor
				 */
				DPConfig(DPConfig const& copy);

				/**
				 * Builds a new DPConfig with the given number of atoms and bonds. The valence and bond values
				 * are filled with 0.
				 */
				DPConfig(Size atoms, Size bonds);

				/**
				 * Builds a new DPConfig with the given atom-valences and bond-values
				 */
				DPConfig(vector<Valence> v, vector<BondOrder> bo);

				/**
				 * Assignment operator
				 */
				DPConfig& operator=(DPConfig const& copy);

				/**
				 * Builds a new DPConfig and initialize its valences and bond-values with the values which are iterated by the given iterators
				 */
				template<typename ValenceIterator, typename BondIterator>
					DPConfig(ValenceIterator vit, ValenceIterator vend, BondIterator boit,
							BondIterator boend) :
						consumedValences(vit, vend), bondAssignments(boit, boend)
			{
			}
				/**
				 * @see #compare
				 */
				bool operator<(DPConfig const& conf) const;
				/**
				 * @see #compare
				 */
				bool operator>(DPConfig const& conf) const;
				/**
				 * @see #compare
				 */
				bool operator<=(DPConfig const& conf) const;
				/**
				 * @see #compare
				 */
				bool operator>=(DPConfig const& conf) const;
				/**
				 * @see #compare
				 */
				bool operator==(DPConfig const& conf) const;
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
				int compare(DPConfig const& other) const;

				/**
				 * returns the number of atoms of the bag this entry belongs to.
				 */
				Size numberOfAtoms() const;

				/**
				 * returns the number of bonds of the bag this entry belongs to.
				 */
				Size numberOfBonds() const;
		};

		/**
		 * A single row in a DPTable, which consists of the DPConfig (valences and bond values) and the penalty, which was
		 * computed for the DPConfig and it's ancestors.
		 */
		typedef pair<DPConfig&, Penalty> DPRow;

		/**
		 * After computing a DPTable, we don't modify it's entries (because we need them for backtracking). So usually we
		 * work with const references to the table entries
		 */
		typedef pair<DPConfig const&, Penalty> DPConstRow;
		/**
		 * Is used to save a reference to a const DPConfig in an object (which isn't possible with references, because they are constant and would prevent it's
		 * adding into collections)
		 */
		typedef pair<DPConfig*, Penalty> DPPointerRow;

		/**
		 * A map which gives fast access to the penalty of a given DPConfig. Is used to compare the penalty of two DPConfigs or to
		 * iterate above all table entries.
		 */
		typedef map<DPConfig, Penalty> DPMap;

		/**
		 * The dynamic programming table. Each nice tree decomposition bag has it's own dynamic programming table. They are computed either from leaf-nodes, by
		 * forgetting or introducing entries from ancestor-tables in forget/introduce nodes or by merging two tables into one in join-nodes. The rows in this table
		 * are different possible bond assignments for the molecule graph. It's columns are the consumed valences of the bag's atoms and bond values of the bag's bonds.
		 */
		class DPTable
		{
			protected:
				/**
				 * the table data is represented as a tree map, because this allow fast inserting of DPConfigs and removing
				 * duplicates with greater penalty.
				 */
				DPMap table;
			public:
				/**
				 * Default constructor
				 */
				DPTable();
				/**
				 * Copy constructor
				 */
				DPTable(DPTable const& table);

				/**
				 * iterator above the table entries
				 */
				typedef DPMap::iterator iterator;
				/**
				 * constant iterator above the table entries
				 */
				typedef DPMap::const_iterator const_iterator;

				/**
				 * returns the penalty of a given DPConfig
				 */
				Penalty operator[](DPConfig const& config) const;

				/**
				 * insert a new DPConfig with the given penalty. If the table already contains an equal DPConfig
				 * with a lower penalty, this function does nothing.
				 */
				bool insert(DPConfig const& config, Penalty penalty);

				/**
				 * returns the number of rows in this table
				 */
				Size size() const;

				/**
				 * returns the best penalty in this table or INFINITE_PENALTY, if the table is empty
				 */
				Penalty bestPenalty() const;

				/**
				 * returns the entry with best penalty. Because this function throws an exception if the table
				 * is empty, you should check this by calling #size() before calling this function.
				 * @throw BALL::Exception::IndexOverflow if table is empty
				 */
				DPConstRow bestEntry() const;

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
		class AdditionalBagProperties
		{
			public:
				/**
				 * Default constructor
				 */
				AdditionalBagProperties();
				/**
				 * Copy constructor
				 */
				AdditionalBagProperties(AdditionalBagProperties const& copy);
				/**
				 * Assignment operator
				 */
				AdditionalBagProperties& operator=(AdditionalBagProperties const& copy);
				/**
				 * Destructor
				 */
				~AdditionalBagProperties();

				/**
				 * bonds in this bag
				 */
				std::vector<MolecularGraphTraits::EdgeType> bonds;

				/**
				 * The table of this bag. Is computed during the bond assignment
				 */
				DPTable * table;

		};

		/**
		 * A comparator, which searches DPConfigs in a DPJoinMap. Two DPConfigs are
		 * equal, if they have the same bond values, even if they have different
		 * consumed valences.
		 */
		struct DPJoinMapComparator
		{
			/**
			 * Less-operator. Returns true if leftp is smaller than rightp.
			 * @throws BALL::Exception::InvalidArgument if the both DPConfig entries can not be compared
			 */
			bool operator()(DPConfig const* leftp, DPConfig const* rightp) const;

			/**
			 * returns 1, if leftp is greather than rightp, 0 if both are equal and -1 otherwise.
			 * You can compare only childs from the same join bag.
			 * @throws BALL::Exception::InvalidArgument if the both DPConfig entries can not be compared
			 */
			int compare(DPConfig const* leftp, DPConfig const* rightp) const;
		};

		/**
		 * A map which remember pointers to DPConfigs of a child of a join-node. It uses a DPJoinMapComparator to find
		 * entries with equal bond-values very fast.
		 */
		typedef multimap<DPConfig const*, Penalty, DPJoinMapComparator> DPJoinMap;

		class DPBackTracking;

		/**
		 * bond assignment algorithm. Traverse in post-order the nice tree decomposition and computes the dynamic programming table for each vertex
		 * This class uses a pointer to a graph, to a penalty map and to a nice tree decomposition. So you should take care that
		 * all three objects aren't deleted during this algorithm, because this class neither uses shared pointers, nor make
		 * copies of the objects.
		 */
		class DPBondAssignment
		{
			public:
				typedef GRAPH::GraphTraits<MolecularGraph>::EdgeType Edge;
				typedef TreeWidthImplementation<MolecularGraph>::NiceTreeDecomposition    NiceTreeDecomposition;
				typedef TreeWidthImplementation<MolecularGraph>::TreeDecompositionBag     TreeDecompositionBag;
				typedef TreeWidthImplementation<MolecularGraph>::NiceTreeDecompositionBag NiceTreeDecompositionBag;

				friend class DPBackTracking;
				friend DPTable* postOrderFolding<NiceTreeDecompositionBag, DPTable*, DPBondAssignment>(NiceTreeDecompositionBag&, 	
				                                                                                       DPBondAssignment &);

			/**
			 * Construct a new DPBondAssignment with the given molecule, a PenaltyMap and a built nice tree decomposition
			 * @param molecule the graph
			 * @param penalties a penalty map
			 * @param ntd a nice tree decomposition of the graph
			 * @param upperbound the algorithm will only compute solutions which are better than the upperbound. By default
			 * 										the upperbound is infinity.
			 */
			DPBondAssignment(MolecularGraph& molecule,
					PenaltyMap& penalties,
					NiceTreeDecomposition& ntd, Penalty upperbound = INFINITE_PENALTY);
			/**
			 * Destructor. Just delete the AdditionalBagProperties, which were computed during this algorithm. So don't
			 * delete this instance before backtracking. Furthermore this destructor doesn't delete the nice tree decomposition
			 * and the penalty map. You have to take care for this yourself.
			 */
			~DPBondAssignment();

			/**
			 * Call this to start the bond assignment computing. This function returns the optimal penalty for the
			 * graph. If you want to know the bond order assignment or the penalties of other solutions, you have to
			 * start the backtracking by DPBackTracking.
			 */
			Penalty compute();

			protected:

			/**
			 * the graph with the bonds, which were assigned to a bond value during this algorithm
			 */
			MolecularGraph * molecule;

			/**
			 * A PenaltyMap implementation which maps the valence of a specific atom to the penalty for this configuration
			 */
			PenaltyMap * penalties;

			/**
			 * The nice tree decomposition which is built from the graph
			 */
			NiceTreeDecomposition * ntd;

			/**
			 * A vector of AdditionalBagProperties. Contains the bonds and the dynamic programming tables for each vertex
			 * in the nice tree decomposition
			 */
			vector<AdditionalBagProperties> properties;

			/**
			 * The algorithm will just compute solutions which are better than the upperbound (they have to be BETTER,
			 * not equal, so "upperbound" is maybe not the best word for it).
			 * The upperbound is INFINITY_PENALTY by default, but you can change this if you know that your molecule
			 * has a better solution. This can improve the performance - but usually the algorithm is fast enough to
			 * compute without an upperbound.
			 */
			Penalty upperbound;

			/**
			 * The maximum value which can be assigned to a bond
			 */
			BondOrder maxBondOrder;

			/**
			 * The maximum valence which can be assigned to an atom
			 */
			Valence maxValence;

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
			DPTable* operator() (NiceTreeDecompositionBag& bag, NodeType type,
					std::vector<DPTable*>::const_iterator begin, std::vector<DPTable*>::const_iterator end);

			/**
			 * Remember which bond belonging to the given bag. Because this information is reused in backtracking, the bonds
			 * are saved in the AdditionalBagProperties object.
			 * @param bag the bag of the nice tree decomposition
			 * @return a vector of edges which are only incident to vertices in this bag
			 */
			std::vector<MolecularGraphTraits::EdgeType> getBondsInBag(NiceTreeDecompositionBag& bag);

			/**
			 * computes the dynamic programming table for a given leaf node.
			 * This is done by filling an entry with just one valence-entry (with value 0) into the table.
			 * So the complexity of this operation is constant.
			 * @param bagProperties contains the dynamic programming table
			 */
			void computeLeafIntroduceBag(AdditionalBagProperties& bagProperties);

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
			void computeIntroduceBag(NiceTreeDecompositionBag& bag, 
			                         DPTable& child, AdditionalBagProperties& bagProperties);

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
			void computeForgetBag(NiceTreeDecompositionBag& bag,
														DPTable& child, AdditionalBagProperties& property);

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
			void computeJoinBag(NiceTreeDecompositionBag& bag,
					DPTable& leftChild, DPTable& rightChild, AdditionalBagProperties& bagProperties);

			/**
			 * computes the dynamic programming table for a given root node.
			 * A root bag is nothing more than a forget bag, so this operations does the same as the #computeForgetBag
			 * function.
			 */
			void computeRootBag(NiceTreeDecompositionBag& bag,
					DPTable& child, AdditionalBagProperties& bagProperties);

			/**
			 * delete the column of the forgotten vertex and it's incident bonds in a given table entry. Sum the bond values of
			 * the forgotten bonds to the valence values of their incident non-forgotten vertices.
			 * This function is reused in DPBackTracking and is called in the #computeForgetBag function.
			 */
			Penalty forgetInnerVertexIn(NiceTreeDecompositionBag& bag, DPConstRow childRow, 
			                            DPConfig& entry, std::vector<MolecularGraphTraits::EdgeType>& childBonds, 
																	Size forgottenIndex);

		};

		/**
		 * An assignment is the solution of the bond assignment problem. It consists of the penalty of this solution,
		 * and a vector of bond values for each bond. The bond order is just a vector of bond values. If you want a HashMap which
		 * maps the concrete bonds to their bond value, you should use the DPBondAssignmentAlgorithm class instead, which can
		 * construct such a HashMap from an assignment.
		 * Nevertheless the index of the bonds has a strict order: Each bond which points to atoms with indizes u and v can be
		 * represented as edge (u < v). An edge is less than another edge, is either it's u is less, or if it's u is
		 * equal to the other's u and it's v is less than the other's v. The indizes of the atoms is given by the graph structure.
		 * The first atom has index 0.
		 * We call an assignment valid to a given UndirectedGraph and PenaltyMap, if it's penalty is equal to the sum of penalties
		 * of it's atoms in the given penalty map. Assignments which was computed by the DPBondAssignment algorithm should
		 * be valid to the same graph and penalty map which was used by the algorithm.
		 */
		class Assignment
		{
			protected:
				/**
				 * A vector with bond values. 0 is single bond, 1 is double bond, 2 is tripple bond
				 */
				std::vector<BondOrder> bonds;
			public:
				/**
				 * The penalty of this assignment
				 */
				Penalty penalty;

				/**
				 * build a new, empty assignment
				 */
				Assignment();

				/**
				 * build an assignment with the given number of bonds. The bond values are filled with 0, the penalty
				 * is initialized with INFINITE_PENALTY. Such a bond assignment don't have to be valid!
				 */
				Assignment(Size numOfBonds);

				/**
				 * Copy constructor
				 */
				Assignment(Assignment const& copy);

				/**
				 * Build an assignment from a given bond value vector and penalty. Such a bond assignment don't have to
				 * be valid!
				 */
				Assignment(std::vector<BondOrder> bonds, Penalty penalty);

				/**
				 * Assignment operator
				 */
				Assignment& operator=(Assignment const& copy);

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
				 * graph. This approach is important, because the DPBondAssignment algorithm is just defined for connected graphs.
				 * @throw BALL::Exception::InvalidArgument if the two assignments have a different number of bonds
				 */
				void combine(Assignment const& other);

				/**
				 * returns a const reference to the bond values
				 */
				std::vector<BondOrder> const& getBondOrders() const;

				/**
				 * Comparison is needed to insert such an assignment into a priority queue or a search tree.
				 * An assignment x is less than an assignment y, if the penalty of x is less than the penalty of y.
				 * If two assignments have the same penalty, the comparison is done by comparing each bond value separately.
				 */
				int compare(Assignment const& a) const;

				/**
				 * @see #compare
				 */
				bool operator < (Assignment const& a) const;

				/**
				 * @see #compare
				 */
				bool operator > (Assignment const& a) const;

				/**
				 * @see #compare
				 */
				bool operator <= (Assignment const& a) const;

				/**
				 * @see #compare
				 */
				bool operator >= (Assignment const& a) const;

				/**
				 * @see #compare
				 */
				bool operator == (Assignment const& a) const;

				/**
				 * Returns true if the assignment is valid to the given UndirectedGraph and PenaltyMap. This happens if the
				 * sum of penalties of each atom with the valence as sum of the bond values of each incident bond is equal to the
				 * penalty of this assignment.
				 * @param molecule should be the same graph which was used to compute this assignment
				 * @param PenaltyMap should be the same penalty map which was used to compute this assignment
				 * @return true if the assignment is valid, false otherwise
				 */
				bool isValid(MolecularGraph& molecule, PenaltyMap& penalties);
		};

		/**
		 * A BackTrackingState is a position in the tree during the backtracking. It contains an assignment, which
		 * contains all bond values of the bonds which were forgotten during the backtracking. Furthermore it contains the
		 * table entry of the dynamic programming table
		 */
		class BackTrackingState
		{
			public:
				/**
				 * Assignment which contains all bond values of the bonds which were forgotten during the backtracking.
				 * Such an assignment is valid, if the BackTrackingState is pointing to the end of the backtracking
				 */
				Assignment assignment;

				/**
				 * the table entry, which this backtracking state follows. This is the entry of the bag, this BackTrackingState is
				 * pointing to.
				 */
				DPConfig config;

				/**
				 * If the BackTrackingState reachs a join node, it can just follow one of it's both children. So it have to remember
				 * which entry of the table of the second child it choosed. This is done in this stack. It contains the table entry
				 * of the right child and the index of this child in pre-order.
				 */
				stack<pair<DPConfig, Size> > joinBranchs;

				/**
				 * pre-order index of this bag. If you traverse the tree in pre-order and give each vertex a number from 0 to n-1,
				 * this number is the pre-order index.
				 */
				Size index;

				/**
				 * Default constructor
				 */
				BackTrackingState();

				/**
				 * build a BackTrackingState for a bag with the given number of bonds
				 */
				BackTrackingState(Size bonds);

				/**
				 * Copy constructor
				 */
				BackTrackingState(BackTrackingState const& other);

				/**
				 * Assignment operator
				 */
				BackTrackingState& operator=(BackTrackingState const& other);

				/**
				 * Comparison is needed to insert BackTrackingState entries into a search tree. Comparison is done
				 * by compare the assignments of the states.
				 */
				int compare(BackTrackingState const& other) const;

				/**
				 * @see #compare
				 */
				bool operator < (BackTrackingState const&) const;

				/**
				 * @see #compare
				 */
				bool operator > (BackTrackingState const&) const;

				/**
				 * @see #compare
				 */
				bool operator <= (BackTrackingState const&) const;

				/**
				 * @see #compare
				 */
				bool operator >= (BackTrackingState const&) const;

				/**
				 * @see #compare
				 */
				bool operator == (BackTrackingState const&) const;

		};

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
		class DPBackTracking
		{
			public:
				typedef TreeWidthImplementation<MolecularGraph>::NiceTreeDecomposition    NiceTreeDecomposition;
				typedef TreeWidthImplementation<MolecularGraph>::TreeDecompositionBag     TreeDecompositionBag;
				typedef TreeWidthImplementation<MolecularGraph>::NiceTreeDecompositionBag NiceTreeDecompositionBag;

				/**
				 * Construct a new DPBackTracking for a given DPBondAssignment algorithm, which backtracks not more than
				 * maxNumberOfSolutions. By default, the backtracking backtracks only the optimal solution.
				 * You have to call the DPBondAssignment#compute method before constructing the DPBackTracking.
				 * Furthermore you should take care to delete the DPBackTracking before the DPBondAssignment, because this
				 * class operates on a pointer to the bond assignment algorithm, not on a copy.
				 * @param bondAssignment a reference to a DPBondAssignment which is already computed
				 * @param maxNumberOfSolutions the number of solutions you want to backtrack. Is by default 1. The size of the
				 * 															priority queue can never be greater than maxNumberOfSolutions
				 */
				DPBackTracking(DPBondAssignment& bondAssignment, Size maxNumberOfSolutions = MAX_SOLUTION_NUMBER, Penalty upperbound = INFINITE_PENALTY);

				/**
				 * Copy constructor
				 */
				DPBackTracking(DPBackTracking const& copy);

				/**
				 * Destructor. Removes just the BackTrackingStates, not the bond assignment algorithm instance.
				 */
				~DPBackTracking();

				/**
				 * Assignment operator
				 */
				DPBackTracking& operator= (DPBackTracking const& copy);

				/**
				 * returns the current solution. Remark that after constructing the backtracking, there is no solution computed. So
				 * you have to call #nextSolution first.
				 * @throw BALL::Exception::NullPointer if you forgot to call #nextSolution
				 */
				Assignment& getSolution();

				/**
				 * returns the current solution, const version. Remark that after constructing the backtracking, there is no solution computed. So
				 * you have to call #nextSolution first.
				 * @throw BALL::Exception::NullPointer if you forgot to call #nextSolution
				 */
				Assignment const& getSolution() const;

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
				 * this function returns INFINITE_PENALTY.
				 */
				Penalty penaltyOfNextSolution() const;

			protected:

				/**
				 * Defines the Min-order for BackTrackingState-pointer instances in the queue
				 */
				struct StateComparator
				{
					/**
					 * Comparison is done by dereferencing the pointer
					 */
					bool operator () (BackTrackingState const * left, BackTrackingState const * right) const;
				};

				/**
				 * The instance of the DPBondAssignment algorithm, which gives access to the computed tables and
				 * the nice tree decomposition. This class doesn't make a copy of the bondAssignment, so take care
				 * that the bondAssignment isn't deleted before the instance of this class.
				 */
				DPBondAssignment * bondAssignment;

				/**
				 * The current state of the backtracking. Each other state is in the priority queue
				 */
				BackTrackingState * currentState;

				/**
				 * priority queue for backtracking states. It is implemented as search tree, because we need also
				 * access to the worst element (to limit the queues size).
				 */
				multiset<BackTrackingState*, StateComparator> queue;

				/**
				 * the maximum number of solutions we want do backtrack.
				 */
				Size maxNumberOfSolutions;

				/**
				 * A sorted vector of the edges of the graph. The bond values in the assignments are in the same order
				 * as the edges in this vector.
				 */
				std::vector<MolecularGraphTraits::EdgeType> bonds;

				/**
				 * The nice tree decomposition bags in pre-order
				 */
				std::vector<NiceTreeDecompositionBag*> bags;

				/**
				 * maxHeapSize is the maxNumberOfSolutions - the number of backtracked solutions. So this attribute
				 * contains the current number of solutions we want to backtrack.
				 */
				Size maxHeapSize;

				/**
				 * current upperbound. This algorithm will just iterate solutions which are better than this upperbound;
				 */
				Penalty upperbound;

				typedef vector<NiceTreeDecompositionBag const *> BagVector;

				/**
				 * returns the dynamic programming table of the bag with the given pre-order index
				 */
				DPTable& getTable(Size order);

				/**
				 * returns the bag properties of the bag with the given pre-order index
				 */
				AdditionalBagProperties& getProperties(Size order);

				/**
				 * Leaf-Nodes have no antecessors. So either the computing is finished,
				 * or there is an unfinished join node in join branch stack which has to
				 * be computed next.
				 */
				void visitLeaf(BackTrackingState& state);

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
				void visitJoin(BackTrackingState& state, NiceTreeDecompositionBag& bag, 
				               DPTable& leftTable, DPTable& rightTable);

				/**
				 * search the antecessor of this forget node. This is the entry which is equal to
				 * the successor after forgetting the forget-vertex and it's incident bonds.
				 * @param state the current backtracking state
				 * @param bag the forget bag with the successor entry
				 * @param table the child's table with the antecessor entry
				 */
				void visitForget(BackTrackingState& state,
						NiceTreeDecompositionBag& bag, DPTable& table);

				/**
				 * search antecessor of this introduce node. This is the same entry
				 * as the successor, but without the introduced columns
				 * @param state the current backtracking state
				 * @param bag the introduce bag with the successor entry
				 * @param table the child's table with the antecessor entry
				 */
				void visitIntroduce(BackTrackingState& state,
						NiceTreeDecompositionBag& bag, DPTable& table);

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
				void remember(BackTrackingState& state);

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
				void setStateAssignment(BackTrackingState& state, NiceTreeDecompositionBag& bag, 
				                        DPConfig& antecessor, MolecularGraphTraits::VertexType forgottenVertex);

				/**
				 * Make the antecessor entry to the new successor entry of the given state and
				 * adding the penalty
				 * @param state the backtracking state
				 * @param antecessor the choosed entry which becomes the new successor
				 * @param additionalPenalty the penalty which is added to the best previous solution for choosing this antecessor
				 */
				void extendState(BackTrackingState& state, DPConfig const& antecessor,
						Penalty additionalPenalty);

				/**
				 * Remember the choosed entry of the right child's table by adding it into the joinBranch stack.
				 * @param state the backtracking state
				 * @param child the right child of the join bag
				 * @param antecessor the choosed entry in the right child's table
				 */
				void branchState(BackTrackingState& state,
						NiceTreeDecompositionBag const& child, DPConfig const& antecessor);

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
				 * Construct a DPBackTrackingCombiner with the given DPBondAssignment and the number of solutions
				 * @param bondAssignments vector with pointers to the bond assignments. Call #compute before constructing
				 * @param solutionNumber the maximum number of solutions you want to backtrack
				 */
				DPBackTrackingCombiner(vector<DPBondAssignment*>& bondAssignments,
						Size solutionNumber, Penalty upperbound = INFINITE_PENALTY);

				/**
				 * Construct a DPBackTrackingCombiner with the given DPBondAssignment and the number of solutions
				 * @param bondAssignments vector with the bond assignments. Call #compute before constructing
				 * @param solutionNumber the maximum number of solutions you want to backtrack
				 */
				DPBackTrackingCombiner(vector<DPBondAssignment>& bondAssignments,
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

		/**
		 * DPBondAssignmentAlgorithm provides an easy interface over the bond assignment algorithm. It handles
		 * the creation of the nice tree decomposition, the bond assignment computing and the backtracking. So you can compute
		 * the bond orders of a molecule without using all this classes like DPBondAssignment, NiceTreeDecompositionBuilder,
		 * DPBackTracking and so on directly. Furthermore this class is the only one in the dynamic programming algorithm which
		 * expect a molecule! All other classes works with graphs (which doesn't have atoms or bonds, but just vertices and edges).
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
		 *	DPBondAssignmentAlgorithm algorithm;
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
		 * An DPBondAssignmentAlgorithm instance which hasn't started behaves like an iterator which isn't bound to a container.
		 * You can return the instance into this state by calling #reset. Also changing the molecule or penalty map also
		 * reset the instance. Calling other methods than #start, #reset, #setMolecule, #getMolecule, #setPenaltyMap,
		 * #getPenaltyMap, #setUpperbound, #getUpperbound, #setNumberOfSolutions, #getNumberOfSolutions will throw an
		 * InvalidIterator exception if you forget to call #start before.
		 *
		 * The DPBondAssignmentAlgorithm expect that the atom container and the penalty map aren't deleted during the
		 * instances' lifetime. If you want do delete one of them, reset the instance and set their atom container and penalty
		 * map to NULL.
		 *
		 */
		class BALL_EXPORT DPBondAssignmentAlgorithm
		{

			public:
				typedef GRAPH::GraphTraits<MolecularGraph>::EdgeType Edge;
				typedef GRAPH::GraphTraits<MolecularGraph>::VertexType VertexType;
				typedef TreeWidthImplementation<MolecularGraph>::NiceTreeDecomposition NiceTreeDecomposition;
				typedef TreeWidthImplementation<MolecularGraph>::TreeDecomposition TreeDecomposition;

				BALL_CREATE(DPBondAssignmentAlgorithm)

					/**
					 * Default constructor
					 */
					DPBondAssignmentAlgorithm();

				/**
				 * Builds a DPBondAssignmentAlgorithm with the given molecule and penalty map
				 * @param molecule an atomContainer for which bonds the assignment should be done
				 * @param penaltyMap a penalty map which maps an atom with a specific valence to a penalty value
				 * @param numberOfSolutions the maximum number of solutions you want to backtrack. By default this class just
				 * 													backtracks the optimum solution
				 * @param upperbound Solutions which are worse than the upperbound will not be computed. This can improve the
				 * 										performance, but usually the algorithm should be fast enough. Is INFINITE_PENALTY by default.
				 */
				DPBondAssignmentAlgorithm(AtomContainer& molecule, PenaltyMap& penaltyMap,
						Size numberOfSolutions = MAX_SOLUTION_NUMBER, Penalty upperbound = INFINITE_PENALTY );

				/**
				 * Copy constructor
				 */
				DPBondAssignmentAlgorithm(DPBondAssignmentAlgorithm const& copy);

				/**
				 * Assignment operator
				 */
				DPBondAssignmentAlgorithm& operator=(DPBondAssignmentAlgorithm const& copy);

				/**
				 * Destructor. Deletes the backtracking. The nice tree decomposition and the dynamic programming tables are
				 * deleted if there are no other DPBondAssignmentAlgorithm instances which access them.
				 */
				virtual ~DPBondAssignmentAlgorithm();

				/**
				 * Start the bond assignment computing. Computes the nice tree decomposition and the dynamic programming table.
				 * After calling this method, you can access the solutions
				 * @throw BALL::Exception::NullPointer if the atom container or penalty map aren't set
				 */
				void start();

				/**
				 * setter for the atom container. Calling this methods reset the DPBondAssignmentAlgorithm. That means, each
				 * previous computing is deleted. This function doesn't check if the new atom container is really different
				 * than the previous one.
				 */
				void setMolecule(AtomContainer& molecule);

				/**
				 * set the upperbound for the computing. Changing this value during the algorithm will reset the
				 * DPBondAssignmentAlgorithm and delete each previous computation.
				 */
				void setUpperbound(Penalty upperbound);

				/**
				 * returns the upperbound for the computing. This algorithm computes only solutions with
				 * better penalty than the upperbound.
				 */
				Penalty getUpperbound() const;

				/**
				 * set the maximum number of solutions for the computing. Changing this value during the algorithm will reset the
				 * DPBondAssignmentAlgorithm and delete each previous computation.
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
				 * setter for the penalty map. Calling this methods reset the DPBondAssignmentAlgorithm. That means, each
				 * previous computing is deleted. This function doesn't check if the new penalty map is really different
				 * than the previous one.
				 */
				void setPenaltyMap(PenaltyMap& penaltyMap);

				/**
				 * returns a const reference to the penalty map
				 * @throws BALL::Exception::NullPointer if penalty map is NULL
				 */
				PenaltyMap& getPenaltyMap();

				/**
				 * Backtracks the next best solution. Call #hasNextSolution first to avoid an OutOfRange exception
				 * @throw BALL::Exception::OutOfRange if there are no more solutions to backtrack
				 * @throw BALL::Exception::InvalidIterator if #start is not called
				 */
				void nextSolution();

				/**
				 * returns true if there is a further solution which can be backtracked
				 * @throw BALL::Exception::InvalidIterator if #start is not called
				 */
				bool hasNextSolution() const;

				/**
				 * delete all previous computings and frees the nice tree decomposition and penalty map, if they are not
				 * referenced by another DPBondAssignmentAlgorithm.
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
				DPBondAssignmentAlgorithm& operator++();

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
				 * This data structure contains all algorithm data which is generated during the #start function
				 * and which can be shared above more than one DPBondAssignmentAlgorithm "iterator", if they compute all
				 * the same molecule with the same penalty table.
				 */
				class ComputingData
				{
					public:
						/**
						 * Default constructor
						 */
						ComputingData();

						/**
						 * Deletes the dynamic programming table, the molecule graph and the nice tree decomposition
						 */
						~ComputingData();

						/**
						 * The bond assignments for each connection component of this molecule
						 */
						vector<DPBondAssignment*> bondAssignments;

						/**
						 * the connection components of this molecule
						 */
						std::vector<std::vector<MolecularGraphTraits::VertexType> > connectionComponents;

						/**
						 * the molecule graph
						 */
						MolecularGraph *moleculeGraph;

						/**
						 * The nice tree decompositions for each connection component
						 */
						boost::shared_ptr<TreeWidth<MolecularGraph> > tw_;

						/**
						 * a vector with pointers to the bonds of the atom container. The order of this bonds in the vector
						 * is the same as the order of the bond values in the assignments.
						 */
						vector<Bond const *> bonds;
				};

				/**
				 * strict ordering of bonds depending of the atom indizes of their incident atoms
				 */
				struct sortBondInNormalizedOrder
				{
					DPBondAssignmentAlgorithm const& obj;
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
				PenaltyMap * penaltyMap;

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
				 * A shared pointer to the computing data, so that you can copy this instance without copying
				 * all the computing data
				 */
				shared_ptr<ComputingData> computingData;

				/**
				 * builds a undirected graph from the given atom container
				 */
				void buildGraph();

				/**
				 * builds an optimal nice tree decomposition
				 */
				void buildNiceTreeDecomposition();

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

#define DEBUG

#ifdef DEBUG

		/**
		 * returns the name of a bag. Just for debugging
		 */
		string getBagName(TreeWidthImplementation<MolecularGraph>::NiceTreeDecompositionBag const& bag);

		/**
		 * prints a table on standard output. Just for debugging
		 */
		void PRINT_TABLE(AdditionalBagProperties properties, 
		                 TreeWidthImplementation<MolecularGraph>::NiceTreeDecompositionBag const& bag, MolecularGraph const& graph);
		/**
		 * prints a table entry on standard output. Just for debugging
		 */
		void PRINT_ENTRY(DPConstRow const& row);
		/**
		 * prints an assignment on standard output. Just for debugging
		 */
		void PRINT_ASSIGNMENT(Assignment assignment);

		inline string getBagName(TreeWidthImplementation<MolecularGraph>::NiceTreeDecompositionBag const& bag)
		{
			typedef TreeWidthImplementation<MolecularGraph>::TreeDecompositionBag TreeDecompositionBag;
			string tab;
			switch (bag.getBagType())
			{
				case TreeDecompositionBag::INTRODUCE_BAG: tab = "introduce"; break;
				case TreeDecompositionBag::FORGET_BAG: tab = "forget"; break;
				case TreeDecompositionBag::LEAF_BAG: tab = "leaf"; break;
				case TreeDecompositionBag::ROOT_BAG: tab = "root"; break;
				case TreeDecompositionBag::JOIN_BAG: tab = "join"; break;
				default: tab = "UNDEFINED"; break;
			}
			return tab;
		}

#include <iostream>
		inline void PRINT_TABLE(AdditionalBagProperties properties, 
		                        TreeWidthImplementation<MolecularGraph>::NiceTreeDecompositionBag& bag, MolecularGraph const& graph)
		{
			typedef TreeWidthImplementation<MolecularGraph>::TreeDecompositionBag TreeDecompositionBag;

			DPTable const& table (*properties.table);
			cout << "<table " << getBagName(bag);
			if (bag.getBagType() != TreeDecompositionBag::JOIN_BAG)
			{
				cout << "  <-  " << bag.getForgottenVertex();
			}
			cout << "\n\t";

			for (TreeDecompositionBag::VertexSet::const_iterator iter = bag.getInnerVertices().begin(); 
			     iter != bag.getInnerVertices().end(); ++iter)
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

#endif
