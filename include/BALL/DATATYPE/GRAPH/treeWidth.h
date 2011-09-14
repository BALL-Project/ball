// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_GRAPH_TREEWIDTH_H
#define BALL_DATATYPE_GRAPH_TREEWIDTH_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_CONCEPT_BASEFUNCTOR_H
# include <BALL/CONCEPT/baseFunctor.h>
#endif

#ifndef BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H
# include <BALL/DATATYPE/GRAPH/graphAlgorithms.h>
#endif

// TODO: replace by boost::graph_as_tree
#ifndef BALL_DATATYPE_GRAPH_TREETRAITS_H
# include <BALL/DATATYPE/GRAPH/treeTraits.h>
#endif

#ifndef BALL_DATATYPE_GRAPH_MOLECULARGRAPH_H
# include <BALL/DATATYPE/GRAPH/molecularGraph.h>
#endif

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <vector>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/copy.hpp>

namespace BALL
{
	template <class EditableGraph> class TreeWidthImplementation;
	/** This class computes a minimal tree decomposition for the given input graph.
	 *  
	 */
	template <class UndirectedGraph>
	class TreeWidth
	{
		public:
			typedef typename GRAPH::GraphTraits<UndirectedGraph>::EditableGraph EditableGraph;
		  typedef typename TreeWidthImplementation<EditableGraph>::NiceTreeDecomposition NiceTreeDecomposition;

			typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
			                              boost::property<boost::vertex_atom_ptr_list_t, std::list<const Atom*> >,
																	  boost::no_property> TreeDecompositionGraph;

			TreeWidth(UndirectedGraph const& input);

			std::vector<boost::shared_ptr<EditableGraph> >& getComponents() { return components_; }
			std::vector<NiceTreeDecomposition>& getDecompositions() { return decompositions_; }

			/// Converts the tree decomposition into a boost::graph
			TreeDecompositionGraph convertToGraph();
				
		protected:
			template <typename ComponentMap>
			class ComponentFilter_
			{
				public:
					ComponentFilter_(ComponentMap cm, Position i) 
						: cm_(&cm),
							component_(i)
					{ }

					template <typename Vertex>
					bool operator() (const Vertex& e) const 
					{
						return ((*cm_)[e] == component_);
					}

				protected:
					ComponentMap* cm_;
					Position      component_;	
			};

			MolecularGraph const* input_;
			std::vector<boost::shared_ptr<EditableGraph> > components_; 
		  std::vector<typename TreeWidthImplementation<EditableGraph>::NiceTreeDecomposition> decompositions_;
	};

	template <class UndirectedGraph>
	class TreeWidthImplementation
	{
		public:

		typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor  VertexType;
		typedef typename boost::graph_traits<UndirectedGraph>::adjacency_iterator NeighbourIterator;
		typedef typename boost::graph_traits<UndirectedGraph>::vertex_iterator    VertexIterator;

		/**
		 * An EliminationOrder is a permutation of vertices of a graph which can be used to build
		 * a Fill-In-Graph. The TreeDecomposition of such a graph is a minimal Tree-Decomposition of the source graph,
		 * if the elimination order is perfect.
		 * first is the permutation of vertices, second is the tree width of the Three-Decomposition of such a Fill-In graph
		 */
		typedef std::pair<std::vector<Size>, Size> EliminationOrder;

		/**
		 * @brief Generic lower bound algorithm on graphs.
		 * 
		 * This class defines a base that can be extended to many different lower bound algorithms that
		 * have the same procedure: Find the best vertex by a special criterion, reduce
		 * the graph size by one, and repeat these two steps until the graph has only one vertex.
		 * @tparam UndirectedGraph the graph type
		 * @tparam Criterion the criterium which chooses the next vertex
		 * @tparam Reducer the reducer which removes a vertex from the graph to reduce it's size
		 */
		template<class Criterion, class Reducer>
		class GeneralLowerBoundAlgorithm 
			: public UnaryFunctor<UndirectedGraph, Size>
		{
			public:
				GeneralLowerBoundAlgorithm() {}

				virtual Size operator() (UndirectedGraph const& originalGraph);
		};

		/**
		 * Contract the edge of the vertex which fits the MinorMinWidthCriterion with
		 * a neighbour with minimum degree
		 * @tparam UndirectedGraph the graph type
		 * @throw BALL::GRAPH::UnconnectedGraphException if called on unconnected graphs
		 */
		class MinorMinWidthReducer
		{
			public:
				MinorMinWidthReducer(UndirectedGraph& graph);

				void operator() (VertexType& vertex);
				void contractEdge(VertexType& u, VertexType& v);

			protected:
				UndirectedGraph& graph_;
		};

		/**
		 * search for a vertex with minimum degree
		 */
		class MinorMinWidthCriterion
		{
			public:
				MinorMinWidthCriterion(UndirectedGraph const& graph);

				Size operator() (VertexType& vertex) const;

			protected:
				UndirectedGraph const& graph_;
		};

		/**
		 * Minor-Min-Width is a lowerbound algorithm for computing the
		 * treewidth.
		 * It contracts in each step a vertex u, which has minimum degree in graph,
		 * with a vertex v, which has a minimum degree in u's neighbourhood. The
		 * maximum of the minimum degrees is the lowerbound for the treewidth.
		 *
		 * The basic idea behind this algorithm is:
		 * - the treewidth of a graph is never lower than the treewidth of its minor
		 * - the treewidth of a graph is never lower than the minimum degree of its vertices
		 * 	 (because you can always find an optimal tree decomposition which contains a leaf,
		 * 		which has at least one vertex v, which isn't contained in the parents vertex set.
		 * 		For each edge of this vertex v, there must be a vertex in the leaf. So degree(v)
		 * 		is a minimal treewidth of this tree decomposition!)
		 * @throw BALL::GRAPH::UnconnectedGraphException if called on unconnected graphs
		 */
		/*template <class UndirectedGraph>
		class MinorMinWidth
		  : public GeneralLowerBoundAlgorithm<UndirectedGraph, MinorMinWidthCriterion<UndirectedGraph>, 
			                                    MinorMinWidthReducer<UndirectedGraph> >
		{
		};
		*/
		typedef GeneralLowerBoundAlgorithm<MinorMinWidthCriterion, MinorMinWidthReducer> MinorMinWidth;


		/**
		 * Algorithm which can be extended to different upperbound heuristics which follow the
		 * same procedure: Finding a vertex v by a special criterium, insert this vertex
		 * in the elimination order and then eliminate it in the graph.
		 *
		 * The basic idea is to build a Fill-In-Graph. Such a Fill-In-Graph can be produced by
		 * eliminating the vertices of a graph and adding the edges, which appear by eliminating, into
		 * the Fill-In-Graph. If the vertices were eliminated in the correct order, the tree decomposition of the
		 * Fill-In-Graph is also the optimal tree decomposition of the source graph. Otherwise, it's a tree decomposition
		 * with higher treewidth which can be used as upperbound. The order of eliminating is called EliminationOrder.
		 * Each GreedyX algorithm returns the EliminationOrder with the vertex permuation and the treewidth. This
		 * vertex permutation can be used to build a tree decomposition of the source graph.
		 * @tparam Criterion the criterium which chooses the next vertex to eliminate
		 * @throw BALL::GRAPH::UnconnectedGraphException if called on unconnected graphs
		 */
		template<class Criterion>
		class GreedyX 
			: public UnaryFunctor<UndirectedGraph, typename std::pair<
				std::vector<boost::graph_traits<typename UndirectedGraph::vertex_descriptor> >, Size> >
		{
			public:
				EliminationOrder operator() (UndirectedGraph& original_graph);
		};

		/**
		 * A criterium for GreedyFillIn which search for a vertex with
		 * minimum number of additional edges after eliminating
		 */
		struct FillInHeuristic 
		{
			VertexType& operator() (UndirectedGraph& graph);

			Size edgeIncreaseByEliminating(VertexIterator vertex, UndirectedGraph& graph);
		};

		/**
		 * This algorithm computes a perfect elimination order in a branch and bound approach.
		 * First it computes a greedy solution. Then it tries each vertex permutation and uses
		 * a lower bound algorithm to check, if this branch can be better than either the best found solution
		 * or a found permutation of the same vertices but in a different order.
		 * If not, the branch is bounded and the algorithm tries another permutation.
		 * @tparam Lowerbound the lowerbound algorithm which should be used by this branch and bound algorithm
		 * @tparam Upperbound the greedy algorithm which is used to compute a initial solution
		 */
		template <class Lowerbound=MinorMinWidth, class Upperbound=GreedyX<FillInHeuristic> >
		class QuickBB
		{
			public:
				/**
				 * A vertex IS simplicial, if its neighbourhood induces a clique.
				 * A vertex is ALMOST simplicial, it all but one of its neighbours
				 * induces a clique.
				 */
				enum SIMPLICIAL_TYPE
				{
					NOT_SIMPLICIAL, 
					ALMOST_SIMPLICIAL, 
					IS_SIMPLICIAL
				};

				/**
				 * Builds a new QuickBB algorithm for the given graph
				 */
				QuickBB(UndirectedGraph const& graph);

				/**
				 * computes the best elimination order
				 */
				EliminationOrder compute();

				SIMPLICIAL_TYPE isSimplicial(VertexType& vertex) const;

			protected:
				/**
				 * a single vertex in the search-tree
				 */
				struct QuickBBState
				{
					/**
					 * width of the permutation along the path from the root
					 */
					unsigned int g;

					/**
					 * lowerbound of g
					 */
					unsigned int h;

					/**
					 * best lowerbound for this state
					 */
					unsigned int f;

					/**
					 * current elimination order
					 */
					std::vector<Size> permutation;
				};

				typedef std::map<VertexType, bool> BitSet;
				typedef std::map<BitSet, Size> GraphMap;
				typedef std::pair<typename GraphMap::iterator, bool> MapPos;
				typedef std::pair<BitSet, Size> MapEntry;

				/**
				 * The graph for which the tree decomposition is built
				 */
				UndirectedGraph graph_;

				/**
				 * the current vertex in the search-tree
				 */
				QuickBBState state;

				/**
				 * An initial permutation which is computed by a greedy algorithm
				 */
				EliminationOrder greedy_solution;

				/**
				 * The permutation which is found by this algorithm
				 */
				EliminationOrder own_solution;

				/**
				 * Remembers the eliminated vertices of found branches during the algorithm. A branch is
				 * bound if it eliminates the same vertices but with a worse penalty.
				 */
				GraphMap visitedSubgraphs;

				/**
				 * the current upper bound. A branch is bound if it has a worse penalty than the upper bound.
				 * Each found solution gives a new upper bound. The greedy solution is the initial upper bound.
				 * The algorithm terminates if it's upper bound is equal to the lowerbound.
				 */
				Size upper_bound;

				/**
				 * Each vertex in the search tree is an elimination order. The root is an elimination order of length 0. It's children
				 * are elimination order of length 1 and so on. The leafs are elimination order of length n and define a
				 * tree decomposition. This algorithm searches the best solution (= permutation with minimal tree width) in this
				 * search tree. It computes the lowerbound for each vertex to get the mimimal tree width of the subtree, which is
				 * rooted in this vertex. Branches are bounded, if their lowerbound is higher than the tree width of the best
				 * found solution, or if there is another branch with a better tree width which eliminates the same vertices but
				 * in a different order. To speed up the computation, the algorithm uses a greedy solution as "template".
				 */
				void branchAndBound(QuickBBState& nstate);

				/**
				 * Vertices which are simplicial or almost simplicial can be eliminated instantly. So this function is called
				 * at the begin of the algorithm to reduce the number of vertices and the length of the searched permutation.
				 * You could call this function in each branch&bound step, but testing the simpliciality is expensive. So this is
				 * done only one time in the algorithm.
				 */
				void prune(QuickBBState&);

				/**
				 * The bitset remembers the eliminated vertices without an ordering.
				 */
				BitSet buildBitset() const;
		};

		/**
		 * The TreeNodeList handles allocating and deleting of tree nodes. Tree nodes
		 * will survive until the TreeNodeList is freed. So you don't have to worry about
		 * freeing/allocating single vertices until you keep the TreeNodeList.
		 * The TreeNodeList expect that their vertices implements an empty constructor,
		 * a copy constructor and an assignment operator.
		 */
		template<class Decomposition>
		class TreeNodeList
		{
			public:
				/**
				 * Default-Constructor
				 */
				TreeNodeList();

				/**
				 * Save Copy-Constructor
				 */
				TreeNodeList(TreeNodeList<Decomposition> const& other);

				/// Destructor;
				virtual ~TreeNodeList();

				/**
				 * Save Assignment
				 */
				TreeNodeList<Decomposition>& operator=(TreeNodeList<Decomposition> const& other);

				/**
				 * Frees each vertex in the list.
				 */
				void clear();

				/**
				 * Adds a copy of the given vertex into the vertex-list
				 */
				Decomposition* newVertex(Decomposition const& vertex);

				/**
				 * Adds a new vertex into the vertex list by default constructor
				 */
				Decomposition* newVertex();

				/**
				 * returns the root of the tree
				 */
				Decomposition* getRoot()
				{
					return root;
				}

				/**
				 * const-getter for the root of the tree
				 */
				Decomposition const* getRoot() const
				{
					return root;
				}

				/**
				 * returns the number of vertices in the vertex list. This don't have to be
				 * the number of vertices in the tree!
				 */
				Size size() const
				{
					return vertices.size();
				}

			protected:
				/**
				 * vector with pointers of the vertices of the tree
				 */
				std::vector<Decomposition*> vertices;

				/**
				 * pointer to the root element of the tree
				 */
				Decomposition* root;
		};

		/**
		 * A TreeDecompositionBag is a vertex in a tree-decomposition. It consists of a set of vertices and
		 * pointers to child elements.
		 * TreeDecompositionBag implements the tree_traits, so you can use it in generic tree algorithms.
		 */
		class TreeDecompositionBag
		{
			public:
				/**
				 * Set of vertices which is used as label for tree-decomposition vertices
				 */
				typedef std::set<VertexType> VertexSet;

				/**
				 * The type of this bag
				 */
				enum BagType {
					/**
					 * Introduce bags differs from their childs in exactly one new vertex
					 */
					INTRODUCE_BAG,
					/**
					 * Leaf bags contains just one vertex and have no childs
					 */
					LEAF_BAG,
					/**
					 * Forget bags contain one vertex less than their children
					 */
					FORGET_BAG,
					/**
					 * Root bags have an empty vertex set
					 */
					ROOT_BAG,
					/**
					 * Join bags have two children, which have both the same inner vertices as their parent
					 */
					JOIN_BAG,
					/**
					 * End bags aren't defined, so you can use them as null-value or as placeholder
					 */
					END_BAG
				};

			public:

				/**
				 * iterator type which is used by tree_traits
				 */
				typedef typename std::vector<TreeDecompositionBag*>::iterator child_iterator;
				/**
				 * const iterator type which is used by tree_traits
				 */
				typedef typename std::vector<TreeDecompositionBag*>::const_iterator const_child_iterator;

				/**
				 * Default-Constructor which builds a new
				 * TreeDecompositionBag with empty inner vertices and
				 * no children
				 */
				TreeDecompositionBag();

				/**
				 * Copy-Constructor
				 */
				TreeDecompositionBag(TreeDecompositionBag const& other);

				/**
				 * Builds a new TreeDecompositionBag with the given
				 * inner vertices
				 */
				TreeDecompositionBag(VertexSet);

				/**
				 * Frees the bag, but doesn't free the children!
				 */
				~TreeDecompositionBag();

				/**
				 * returns a reference to inner vertices.
				 */
				VertexSet& getInnerVertices();

				/**
				 * returns a reference to inner vertices, const version
				 * Use setInnerVertices to change this set
				 */
				VertexSet const& getInnerVertices() const;

				/**
				 * Assignment
				 */
				TreeDecompositionBag& operator=(TreeDecompositionBag const& other);

				/**
				 * Add new child. Don't allocate this child yourself, use
				 * the TreeNodeList instead, because this class will not free
				 * the child itself.
				 */
				void addChild(TreeDecompositionBag*);

				/**
				 * Adds a new vertex into the innerVertices
				 */
				void addInnerVertex(VertexType const&);

				/**
				 * Adds new vertices into the inner vertices by the given input-iterators
				 */
				template<class Iterator>
				void addInnerVertices(Iterator first, Iterator last) 
				{
					vertices.insert(first, last);
				}

				/**
				 * Replace the inner vertices by the given input-iterators
				 */
				template<class Iterator>
				void setInnerVertices(Iterator first, Iterator last) 
				{
					vertices.clear();
					vertices.insert(first, last);
				}

				/**
				 * returns the number of inner vertices
				 */
				Size size() const;

				/**
				 * returns the number of children
				 */
				Size numberOfChildren() const;

				/**
				 * returns the first child of this bag
				 */
				TreeDecompositionBag& firstChild();

				/**
				 * returns the first child of this bag
				 */
				TreeDecompositionBag const& firstChild() const;

				/**
				 * returns an iterator to the first child of this bag
				 */
				typename std::vector<TreeDecompositionBag*>::iterator beginChild();

				/**
				 * returns an iterator to the end of the childs of this bag
				 */
				typename std::vector<TreeDecompositionBag*>::iterator endChild();

				/**
				 * returns an iterator to the first child of this bag
				 */
				typename std::vector<TreeDecompositionBag*>::const_iterator beginChild() const;

				/**
				 * returns an iterator to the end of childs of this bag
				 */
				typename std::vector<TreeDecompositionBag*>::const_iterator endChild() const;

			protected:
				/**
				 * vertices of the source graph in this tree-decomposition bag
				 */
				VertexSet vertices;

				/**
				 * pointers to child elements
				 */
				std::vector<TreeDecompositionBag*> children;
		};

		typedef TreeNodeList<TreeDecompositionBag> TreeDecomposition;

		/**
		 * An upperbound heuristic which computes an EliminationOrder,
		 * which can build a tree decomposition, and a treewidth of a given
		 * graph. The basic idea is to add as few as possible edges into the
		 * FillInGraph, because this should reduce the treewidth of the FillInGraph.
		 * Nevertheless, it's just a heuristic, so there is no guarantee, that
		 * the treewidth is minimal.
		 * @throw BALL::GRAPH::UnconnectedGraphException if called on unconnected graphs
		 */
		typedef GreedyX<FillInHeuristic> GreedyFillIn;

		/**
		 * TreeNodeList which handles allocating/deallocating of a tree-decomposition
		 */
		class TreeNodeListTraits
		{
		};

		/**
		 * Builds a tree decomposition by the given elimination order
		 * @param graph The source graph for which the tree decomposition is built
		 * @param permutation the elimination order which is used to build the tree
		 * @param treeDecomposition an empty TreeNodeList which is filled with instances of TreeDecompositionBag
		 */
		template <class TreeDecompositionType>
		static void eliminationOrderToTreeDecomposition(UndirectedGraph const& graph, 
					                                          EliminationOrder const& permutation,
																										TreeDecompositionType& treeDecomposition);

		/**
		 * A nice tree decomposition bag is a bag of a nice tree decomposition. Such a nice tree decomposition
		 * is a binary tree with five vertex types:
		 * - Introduce-nodes, which have one child and one more inner vertex than their child
		 * - Forget-nodes, which have one child and one inner vertex less than their child
		 * - Join-nodes, which have two childs and the same inner vertices as their childs
		 * - Leaf-nodes, which have no childs and exactly one inner vertex
		 * - Root-nodes, which have one child and no inner vertices
		 */
		class NiceTreeDecompositionBag
		{
			public:
				typedef typename TreeDecompositionBag::BagType BagType;
				typedef typename TreeDecompositionBag::VertexSet VertexSet;

				typedef NiceTreeDecompositionBag * const * child_iterator;
				typedef NiceTreeDecompositionBag const * const * const_child_iterator;

				/**
				 * Default constructor
				 */
				NiceTreeDecompositionBag();

				/**
				 * Set the index
				 */
				NiceTreeDecompositionBag(Size index);

				/**
				 * Shallow copy constructor.
				 */
				NiceTreeDecompositionBag(NiceTreeDecompositionBag const& copy);

				/**
				 * shallow assignment operator
				 */
				NiceTreeDecompositionBag& operator=(NiceTreeDecompositionBag const& copy);

				/**
				 * returns the first child of this bag
				 * @throw IllegalTreeOperation if bagType is END_BAG or LEAF_BAG
				 */
				NiceTreeDecompositionBag& firstChild();
				NiceTreeDecompositionBag const& firstChild() const;

				/**
				 * returns the second child of this bag
				 * @throw IllegalTreeOperation if bagType isn't JOIN_BAG
				 */
				NiceTreeDecompositionBag& secondChild();
				NiceTreeDecompositionBag const& secondChild() const;

				/**
				 * returns an iterator to the first child
				 */
				NiceTreeDecompositionBag * const * beginChild();
				/**
				 * returns an iterator to the end of the children
				 */
				NiceTreeDecompositionBag * const * endChild();
				NiceTreeDecompositionBag const * const * beginChild() const;
				NiceTreeDecompositionBag const * const * endChild() const;

				BagType getBagType() const;

				Size getIndex() const;

				/**
				 * returns a reference to the inner vertices
				 */
				VertexSet& getInnerVertices();

				/**
				 * returns a reference to the inner vertices, const version
				 */
				VertexSet const& getInnerVertices() const;

				/**
				 * returns the introduced vertex. This function should be called only when bag is
				 * an introduce node. Nevertheless, this condition isn't checked.
				 */
				VertexType& getIntroducedVertex();

				/**
				 * returns the forgotten vertex. This function should be called only when bag is
				 * a forget node. Nevertheless, this condition isn't checked.
				 */
				VertexType& getForgottenVertex();

				/**
				 * The bag becomes a leaf-bag, introducing the given vertex
				 */
				void setAsLeaf(VertexType& vertex);

				/**
				 * The bag becomes an introduce-bag, introducing the given vertex and being a parent of the given child-node
				 */
				void setAsIntroduce(NiceTreeDecompositionBag& child, VertexType& introducedVertex);

				/**
				 * The bag becomes a forget-bag, forgetting the given vertex and being a parent of the given child-node
				 */
				void setAsForget(NiceTreeDecompositionBag& child, VertexType& forgotten);

				/**
				 * The bag becomes a join-bag, branching the tree by the given two childs
				 */
				void setAsJoin(NiceTreeDecompositionBag& left, NiceTreeDecompositionBag& right);

				/**
				 * The bag becomes a root-bag, forgetting the given vertex and being a parent of the given child-node
				 */
				void setAsRoot(NiceTreeDecompositionBag& child, VertexType& forgotten);

			protected:
				/**
				 * pointer to the children. Only join-nodes have to children, leafs have
				 * no childs and each other vertex has one child
				 */
				NiceTreeDecompositionBag* children[2];

				/**
				 * set of inner vertices
				 */
				VertexSet vertices;

				/**
				 * the special inner vertex is either an introduced vertex in an introduce/leaf-node,
				 * a forget vertex in a forget/root-node or undefined in a join-node
				 */
				VertexType specialVertex;

				/**
				 * the type of the bag. Either introduce, forget, join, root, leaf or undefined
				 */
				BagType bagType;

				/**
				 * index of the bag
				 */
				Size index;
		};

		/**
		 * The difference between a nice tree-decomposition and a tree-decomposition
		 * is that each node in a nice-treedecomposition either differs from its parent and it's single child
		 * in just one inner vertex, or has exact two childs, which have the same set of inner vertices as their
		 * parent. So a nice tree-decomposition is a binary tree.
		 */
		typedef TreeNodeList<NiceTreeDecompositionBag> NiceTreeDecomposition;

		/**
		 * Builds a NiceTreeDecomposition from a TreeDecomposition
		 */
		class NiceTreeDecompositionBuilder
		{
			public:
				typedef std::set<VertexType> VertexSet;

				typedef typename std::vector<NiceTreeDecompositionBag*>::const_iterator ConstBagIterator; 

				/**
				 * Constructs a new builder which inserts it's vertices into the given NiceTreeDecomposition
				 */
				NiceTreeDecompositionBuilder(NiceTreeDecomposition& ntd);

				/**
				 * Builds a NiceTreeDecomposition from the TreeDecomposition rooted in the given vertex
				 */
				void buildNiceTreeDecomposition(TreeDecompositionBag const& root);

				NiceTreeDecompositionBag* operator () (TreeDecompositionBag const& node, 
				                                       NodeType nodeType, ConstBagIterator begin, ConstBagIterator end);

			protected:
				/**
				 * Manages the allocation of new nice tree decomposition vertices
				 */
				TreeNodeList<NiceTreeDecompositionBag>* ntd;

				/**
				 * First this function builds a leaf node which introduce one vertex from the given tree decomposition bag.
				 * After this, the function builds introduce nodes for each further inner vertex in the bag.
				 */
				NiceTreeDecompositionBag* buildLeaf(TreeDecompositionBag const&);

				/**
				 * This function builds forget nodes until all inner vertices of the child node are forgotten. This last
				 * forget node which has no inner vertices is called "root".
				 */
				NiceTreeDecompositionBag* buildRoot(NiceTreeDecompositionBag* child);

				/**
				 * First this function forgets all vertices which are in the child nodes but not in the parent bag.
				 * After this, the function introduce vertices so that the children are equal. Then it builds a join node
				 * which branches the tree into the both children.
				 * After calling this function the join node isn't finished, because there are missing introduce nodes for vertices
				 * which are contained in the tree decomposition bag but not in the join node. This makes sense because if the
				 * tree decomposition bag has more than two children, the algorithm have to build also more than two join nodes.
				 * So it's the best to introduce the missing vertices in the last join node. This happens in the #branch method
				 * by calling #buildSingle.
				 */
				NiceTreeDecompositionBag* buildJoin(TreeDecompositionBag const& node, NiceTreeDecompositionBag* left, 
				                                    NiceTreeDecompositionBag* right, bool doForget);

				/**
				 * This function is called for inner vertices. If the inner vertices have more than one child, this function
				 * builds join nodes to keep the binary property of the nice tree decomposition.
				 */
				NiceTreeDecompositionBag* branch(TreeDecompositionBag const&, NodeType nodeType,
				                                 ConstBagIterator begin, ConstBagIterator end);

				/**
				 * Is called for inner vertices with just one child. Builds forget and introduce nodes by calling #buildLinkage
				 * (or #buildRoot for the root node).
				 */
				NiceTreeDecompositionBag* buildSingle(TreeDecompositionBag const& node, NodeType nodeType, 
				                                      NiceTreeDecompositionBag* child);

				/**
				 * Builds introduce and forget nodes to introduce all vertices, which are contained in
				 * the given bag but not in the child bag, and to forget all vertices which are contained in the child bag but not
				 * in the tree decomposition bag.
				 */
				NiceTreeDecompositionBag* buildLinkage(TreeDecompositionBag const& node, NiceTreeDecompositionBag* child);

				/**
				 * Builds introduce nodes to insert vertices which are contained in the parent set but not in the child bag
				 */
				NiceTreeDecompositionBag* linkWithIntroduceNodes(VertexSet const& parentSet, NiceTreeDecompositionBag* child);

				/**
				 * Builds forget nodes to remove vertices which are contained in the child bag but not in the parent set
				 */
				NiceTreeDecompositionBag* linkWithForgetNodes(VertexSet const& parentSet, NiceTreeDecompositionBag* child);

		};

	};
}

#include <BALL/DATATYPE/GRAPH/treeWidth.iC>

#endif // BALL_DATATYPE_GRAPH_TREEWIDTH_H
