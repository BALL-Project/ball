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

#ifndef BALL_DATATYPE_GRAPH_MOLECULARGRAPH_H
# include <BALL/DATATYPE/GRAPH/molecularGraph.h>
#endif

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <vector>
#include <iostream>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_as_tree.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>

namespace boost
{
	enum vertex_bag_content_t { vertex_bag_content };
	enum vertex_bag_special_t { vertex_bag_special };
	enum vertex_bag_type_t    { vertex_bag_type    };

	BOOST_INSTALL_PROPERTY(vertex, bag_content);
	BOOST_INSTALL_PROPERTY(vertex, bag_special);
	BOOST_INSTALL_PROPERTY(vertex, bag_type);
}

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
				 * Inner bags are any kind of inner node in the tree, i.e., JOIN, INTRODUCE, or FORGET nodes
				 */
				INNER_BAG,
				/**
				 * End bags aren't defined, so you can use them as null-value or as placeholder
				 */
				END_BAG
			};

			typedef typename GRAPH::GraphTraits<UndirectedGraph>::EditableGraph      EditableGraph;
			typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor OriginalVertexType;

			typedef std::set<OriginalVertexType> TreeDecompositionContent;

			typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
																		  boost::property<boost::vertex_bag_content_t, std::set<OriginalVertexType>,
																			boost::property<boost::vertex_bag_special_t, OriginalVertexType,
																			boost::property<boost::vertex_bag_type_t, int> > >,
																	  boost::no_property> TreeDecompositionGraph;

			typedef typename boost::graph_traits<TreeDecompositionGraph>::vertex_descriptor TreeDecompositionBag;

			typedef boost::iterator_property_map<typename std::vector<TreeDecompositionBag>::iterator,
							                             typename boost::property_map<TreeDecompositionGraph, boost::vertex_index_t>::type>
																					 TreeDecompositionParentMap;
			typedef boost::graph_as_tree<TreeDecompositionGraph, TreeDecompositionParentMap> TreeDecomposition;

			TreeWidth(UndirectedGraph const& input);

			/** Compute the tree width of a given tree decomposition.
			 *  This function iterates over all nodes in the graph to determine the tree width,
			 *  i.e., the (maximum number of vertices over all bags) - 1 
			 */
			static Size computeTreeWidth(TreeDecomposition const& td);

			/** Write a tree decomposition in graphviz format.
			 */
			void writeGraphvizFile(std::ostream& out, TreeDecomposition const& td);

			std::vector<boost::shared_ptr<EditableGraph> >& getComponents() { return components_; }
			std::vector<boost::shared_ptr<TreeDecomposition> >& getNiceTreeDecompositions()   { return nice_tree_decompositions_; }

		protected:
			template <typename ComponentMap>
			class ComponentFilter_
			{
				public:
					ComponentFilter_(ComponentMap cm, Position i)
						: cm_(cm),
							component_(i)
					{ }

					template <typename Vertex>
					bool operator() (const Vertex& e) const 
					{
						return ((cm_)[e] == component_);
					}

				protected:
					ComponentMap  cm_;
					Position      component_;
			};

			/** PropertyWriter for graphviz output.
			 */
			class BagContentWriter 
			{
				public:
					BagContentWriter(TreeDecomposition const* td, UndirectedGraph const* original_graph)
					  : td_(td),
						  original_graph_(original_graph)
					{ }

					void operator() (std::ostream& out, const TreeDecompositionBag& v) const;

				protected:
					TreeDecomposition const* td_;
					UndirectedGraph   const* original_graph_;
			};

			// TODO: would UndirectedGraph suffice here?
			MolecularGraph const* input_;

			std::vector<boost::shared_ptr<EditableGraph> > components_;

			std::vector<boost::shared_ptr<TreeDecomposition> >      nice_tree_decompositions_;
			std::vector<boost::shared_ptr<TreeDecompositionGraph> > nice_tree_decomposition_graphs_;
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

			protected:
				std::map<int, VertexType> index_to_vertex_;
		};

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

		template <class OriginalGraphType>
		class TreeDecompositionBuilder
	  {
			public:
				typedef typename TreeWidth<OriginalGraphType>::TreeDecomposition    TreeDecomposition;
				typedef typename TreeWidth<OriginalGraphType>::TreeDecompositionBag TreeDecompositionBag;
				typedef typename TreeWidth<OriginalGraphType>::TreeDecompositionGraph TreeDecompositionGraph;

				typedef typename TreeWidth<OriginalGraphType>::OriginalVertexType OriginalVertexType;

				typedef std::set<OriginalVertexType> TreeDecompositionContent;

				/**
				 * Builds a tree decomposition by the given elimination order
				 * @param graph The source graph for which the tree decomposition is built
				 * @param permutation the elimination order which is used to build the tree
				 * @return tree_decomposition an empty TreeNodeList which is filled with instances of TreeDecompositionBag
				 */
				boost::shared_ptr<TreeDecomposition> operator() (UndirectedGraph const& graph, EliminationOrder const& permutation);

				/**
				 *  Converts the TreeDecomposition into a NiceTreeDecomposition
				 *   A nice tree decomposition is a binary tree with five vertex types:
				 *    - Introduce-nodes, which have one child and one more inner vertex than their child
				 *    - Forget-nodes, which have one child and one inner vertex less than their child
				 *    - Join-nodes, which have two childs and the same inner vertices as their childs
				 *    - Leaf-nodes, which have no childs and exactly one inner vertex
				 *    - Root-nodes, which have one child and no inner vertices
				 */
				boost::shared_ptr<TreeDecomposition> makeNice(boost::shared_ptr<TreeDecompositionGraph>& nice_tree);

				TreeDecompositionBag operator() (TreeDecompositionBag n,
					typename std::vector<TreeDecompositionBag>::iterator c_i, typename std::vector<TreeDecompositionBag>::iterator c_end);

			protected:
				TreeDecompositionBag buildRoot_(TreeDecompositionBag child);
				TreeDecompositionBag buildLeaf_(TreeDecompositionBag child);
				TreeDecompositionBag buildJoin_(TreeDecompositionBag node, TreeDecompositionBag left,
				                                  TreeDecompositionBag right, bool do_forget);

				TreeDecompositionBag buildSingle_(TreeDecompositionBag node, int node_type,
				                                    TreeDecompositionBag child);

				TreeDecompositionBag buildLinkage_(TreeDecompositionBag node, TreeDecompositionBag child);

				TreeDecompositionBag linkWithIntroduceNodes_(TreeDecompositionContent parent_set, TreeDecompositionBag child);
				TreeDecompositionBag linkWithForgetNodes_   (TreeDecompositionContent parent_set, TreeDecompositionBag child);

				TreeDecompositionBag branch_(TreeDecompositionBag node, int node_type,
				                               typename std::vector<TreeDecompositionBag>::iterator begin,
																			 typename std::vector<TreeDecompositionBag>::iterator end);

				boost::shared_ptr<TreeDecomposition> tree_;
				boost::shared_ptr<TreeDecompositionGraph> tree_graph_;
				boost::shared_ptr<TreeDecompositionGraph> nice_tree_;

				TreeDecompositionBag root_;
		};

	};
}

#include <BALL/DATATYPE/GRAPH/treeWidth.iC>

#endif // BALL_DATATYPE_GRAPH_TREEWIDTH_H
