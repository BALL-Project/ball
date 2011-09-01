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

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <vector>

namespace BALL
{
	/**
	 *  This class combines a number of type definitions that depend on the graph type
	 */
	template <class UndirectedGraph>
	class TreeWidthTraits
		: public GRAPH::GraphTraits<UndirectedGraph>
	{
		public:
			/// The type of vertices in our graph
			typedef typename GRAPH::GraphTraits<UndirectedGraph>::VertexType             VertexType;
			typedef typename GRAPH::GraphTraits<UndirectedGraph>::VertexIterator         VertexIterator;

			typedef typename GRAPH::GraphTraits<UndirectedGraph>::NeighbourIterator      NeighbourIterator;

			typedef typename GRAPH::GraphTraits<UndirectedGraph>::EdgeType               EdgeType;

			/**
			 * An EliminationOrder is a permutation of vertices of a graph which can be used to build
			 * a Fill-In-Graph. The TreeDecomposition of such a graph is a minimal Tree-Decomposition of the source graph,
			 * if the elimination order is perfect.
			 * first is the permutation of vertices, second is the tree width of the Three-Decomposition of such a Fill-In graph
			 */
			typedef std::pair<std::vector<VertexType>, Size> EliminationOrder;

			template <class Decomposition> class TreeNodeList;
			template <class GraphType> class TreeDecompositionBag;
			template <class GraphType> class NiceTreeDecompositionBag;
			template <class GraphType, class Criterion> class GreedyX;
//			template <class GraphType> class FillInHeuristic;

			typedef TreeNodeList<TreeDecompositionBag<UndirectedGraph> > TreeDecomposition;

			/**
			 * An upperbound heuristic which computes an EliminationOrder,
			 * which can build a tree decomposition, and a treewidth of a given
			 * graph. The basic idea is to add as few as possible edges into the
			 * FillInGraph, because this should reduce the treewidth of the FillInGraph.
			 * Nevertheless, it's just a heuristic, so there is no guarantee, that
			 * the treewidth is minimal.
			 * @throw BALL::DPBondOrder::UnconnectedGraphException if called on unconnected graphs
			 */
//			typedef GreedyX<UndirectedGraph, FillInHeuristic<UndirectedGraph> > GreedyFillIn;

			/**
			 * The difference between a nice tree-decomposition and a tree-decomposition
			 * is that each node in a nice-treedecomposition either differs from its parent and it's single child
			 * in just one inner vertex, or has exact two childs, which have the same set of inner vertices as their
			 * parent. So a nice tree-decomposition is a binary tree.
			 */
			typedef TreeNodeList<NiceTreeDecompositionBag<UndirectedGraph> > NiceTreeDecomposition;
	};

	/**
	 * Is thrown if a function, which operates on connected graphs, is called with a unconnected graph.
	 */
	class UnconnectedGraphException 
		: public Exception::GeneralException
	{
		public:
			UnconnectedGraphException(const char * file, int line);
			UnconnectedGraphException(const char * file, int line, BALL::String computation);
	};

	/**
	 * @brief Generic lower bound algorithm on graphs.
	 * 
	 * This class defines a base that can be extended to many different lower bound algorithms that
	 * have the same procedure: Finde the best vertex by a special criterion, reduce
	 * the graph size by one, and repeat these two steps until the graph has only one vertex.
	 * @tparam UndirectedGraph the graph type
	 * @tparam Criterion the criterium which chooses the next vertex
	 * @tparam Reducer the reducer which removes a vertex from the graph to reduce it's size
	 */
	template<class UndirectedGraph, class Criterion, class Reducer>
	class GeneralLowerBoundAlgorithm 
		: public UnaryFunctor<UndirectedGraph, Size>
	{
		public:
			typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor VertexType;

			GeneralLowerBoundAlgorithm() {}

			virtual Size operator() (UndirectedGraph const& originalGraph);
	};

	/**
	 * Contract the edge of the vertex which fits the MinorMinWidthCriterion with
	 * a neighbour with minimum degree
	 * @tparam UndirectedGraph the graph type
	 * @throw BALL::DPBondOrder::UnconnectedGraphException if called on unconnected graphs
	 */
	template<class UndirectedGraph>
	class MinorMinWidthReducer
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;

			MinorMinWidthReducer(UndirectedGraph& graph);

			void operator() (VertexType& vertex);
			void contractEdge(VertexType& u, VertexType& v);

		protected:
			UndirectedGraph& graph_;
	};

	/**
	 * search for a vertex with minimum degree
	 */
	template <class UndirectedGraph>
	class MinorMinWidthCriterion
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;

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
	 * @throw BALL::DPBondOrder::UnconnectedGraphException if called on unconnected graphs
	 */
	template <class UndirectedGraph>
	class MinorMinWidth
		: public GeneralLowerBoundAlgorithm<UndirectedGraph, MinorMinWidthCriterion<UndirectedGraph>, 
				                                MinorMinWidthReducer<UndirectedGraph> >
	{
	};

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
	 * @throw BALL::DPBondOrder::UnconnectedGraphException if called on unconnected graphs
	 */
	template<class UndirectedGraph, class Criterion>
	class GreedyX 
		: public UnaryFunctor<UndirectedGraph, typename std::pair<
													std::vector<boost::graph_traits<typename UndirectedGraph::vertex_descriptor> >, Size> >
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;
			typedef typename TreeWidthTraits<UndirectedGraph>::NeighbourIterator NeighbourIterator;
			typedef typename TreeWidthTraits<UndirectedGraph>::EliminationOrder EliminationOrder;

			EliminationOrder operator() (UndirectedGraph const& original_graph);
	};

	/**
	 * A criterium for GreedyFillIn which search for a vertex with
	 * minimum number of additional edges after eliminating
	 */
	template <class UndirectedGraph>
	struct FillInHeuristic 
	{
		typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;
		typedef typename TreeWidthTraits<UndirectedGraph>::VertexIterator VertexIterator;

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
	template<class UndirectedGraph, 
	         class Lowerbound=MinorMinWidth<UndirectedGraph>, 
					 class Upperbound=GreedyX<UndirectedGraph, FillInHeuristic<UndirectedGraph> > >
	class QuickBB
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;
			typedef typename TreeWidthTraits<UndirectedGraph>::EliminationOrder EliminationOrder;

			typedef GreedyX<UndirectedGraph, FillInHeuristic<UndirectedGraph> > GreedyFillIn;

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
				std::vector<VertexType> permutation;
			};

			typedef std::map<VertexType, bool> BitSet;
			typedef std::map<BitSet, Size> GraphMap;
			typedef std::pair<typename GraphMap::iterator, bool> MapPos;
			typedef std::pair<BitSet, Size> MapEntry;

			/**
			 * The graph for which the tree decomposition is built
			 */
			UndirectedGraph graph;

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

	template<class UndirectedGraph, class L, class U>
	QuickBB<UndirectedGraph, L, U>::QuickBB(UndirectedGraph const& graph) 
		: graph(graph), 
		  state(), 
			greedy_solution(), 
			own_solution(),
			visitedSubgraphs(), 
			upper_bound(0)
	{
	}


	template<class UndirectedGraph, class L, class U>
	typename QuickBB<UndirectedGraph, L, U>::SIMPLICIAL_TYPE QuickBB<UndirectedGraph, L, U>::isSimplicial(VertexType& vertex) const
	{
		if (out_degree(vertex, graph) <= 1)
			return IS_SIMPLICIAL;

		VertexType* n1 = NULL;
		VertexType* n2 = NULL;

		typename boost::graph_traits<UndirectedGraph>::adjacency_iterator a_i, a_j, ai_end;

		for (tie(a_i, ai_end) = adjacent_vertices(vertex, graph); a_i != ai_end; ++a_i)
		{
			for (a_j = a_i+1; a_j != ai_end; ++a_j)
			{
				VertexType& i = *a_i;
				VertexType& j = *a_j;

				if (!edge(i, j, graph).second)
				{
					if (!n1)
					{
						n1 = &i;
						n2 = &j;
					} else if (i == *n1 || i == *n2)
					{
						n1 = i;
						n2 = i;
					} else if (j == *n1 || j == *n2)
					{
						n1 = j;
						n2 = j;
					} else
					{
						return NOT_SIMPLICIAL;
					}
				}
			}
		}
		if (!n1)
		{
			return IS_SIMPLICIAL;
		} else {
			return ALMOST_SIMPLICIAL;
		}
	}

	template <class UndirectedGraph, class L, class U>
	void QuickBB<UndirectedGraph, L, U>::prune(QuickBB::QuickBBState& cstate)
	{
		Size nxt = cstate.permutation.size();

		VertexType current_vertex;
		BGL_FORALL_VERTICES_T(current_vertex, graph, UndirectedGraph)
		{
			SIMPLICIAL_TYPE simplicial(isSimplicial(current_vertex));
			if (simplicial == IS_SIMPLICIAL || simplicial == ALMOST_SIMPLICIAL)
			{
				cstate.permutation.push_back(current_vertex);
			}
		}

		for (typename std::vector<VertexType>::const_iterator st_iter = cstate.permutation.begin() + nxt; 
																													st_iter != cstate.permutation.end(); ++st_iter)
		{
			VertexType vertex = *st_iter;
			cstate.g = std::max(cstate.g, out_degree(vertex, graph));
			cstate.f = std::max(cstate.f, cstate.g);

			GRAPH::eliminateVertex(vertex);
		}
	}

	template<class UndirectedGraph, class L, class U>
	typename QuickBB<UndirectedGraph, L, U>::BitSet QuickBB<UndirectedGraph, L, U>::buildBitset() const
	{
		BitSet set;

		BGL_FORALL_VERTICES_T(v, graph, UndirectedGraph)
		{
			set[v] = true;
		}

		return set;
	}

	template<class UndirectedGraph, class L, class U>
	typename QuickBB<UndirectedGraph, L, U>::EliminationOrder QuickBB<UndirectedGraph, L, U>::compute()
	{
		Size lower_bound = L()(graph);
		greedy_solution = U()(graph);

		state.f = lower_bound;
		state.h = lower_bound;
		state.g = 0;

		upper_bound = greedy_solution.second;
		if (lower_bound < upper_bound)
		{
			prune(state);
			branchAndBound(state);

			own_solution = EliminationOrder(state.permutation, upper_bound);

		} 
		else
		{
			own_solution = greedy_solution;
		}
		return own_solution;
	}

	template <class UndirectedGraph, class L, class U>
	void QuickBB<UndirectedGraph, L, U>::branchAndBound(QuickBB::QuickBBState& nstate)
	{
		Size length = num_vertices(graph);

		if (length == 0)
		{
			upper_bound = nstate.f;
		} 
		else if (length == 1)
		{
			if (upper_bound >= nstate.f)
			{
				upper_bound = nstate.f;
				state.permutation = nstate.permutation;
				state.permutation.push_back(*(vertices(graph).first));
			}
		} 
		else
		{
			for (typename std::vector<VertexType>::const_iterator viter = greedy_solution.first.begin(); 
			     viter != greedy_solution.first.end(); ++viter)
			{
				VertexType const& vertex = *viter;
				Size degree = out_degree(vertex, graph);

				EliminationOrder elimination(graph, vertex);

				QuickBBState xstate(nstate);
				xstate.g = std::max(nstate.g, degree);
				xstate.permutation.push_back(vertex);

				BitSet bitset(buildBitset());

				Size lower_bound = L()(graph);
				xstate.h = lower_bound;
				xstate.f = max(xstate.g, lower_bound);

				if (xstate.f >= upper_bound)
				{
					elimination.undo();
					continue;
				}

				MapPos entry(visitedSubgraphs.insert(MapEntry(bitset, xstate.f)));
				if (entry.second)
				{
					branchAndBound(xstate);
				} 
				else if ((entry.first)->second > xstate.f)
				{
					entry.first->second = xstate.f;
					branchAndBound(xstate);
				}
				elimination.undo();
			}
		}
	}

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
	template <class UndirectedGraph>
	class TreeDecompositionBag
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;

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
			 * returns a const reference to inner vertices.
			 * Use setInnerVertices to change this set
			 */
			VertexSet& getInnerVertices();

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
			void addInnerVertex(VertexType&);

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

	template <class UndirectedGraph>
	TreeDecompositionBag<UndirectedGraph>::TreeDecompositionBag() 
		: vertices(), 
		  children()
	{
	}

	template <class UndirectedGraph>
	TreeDecompositionBag<UndirectedGraph>::TreeDecompositionBag(VertexSet vs) 
		: vertices(vs), 
		  children()
	{
	}

	template <class UndirectedGraph>
	TreeDecompositionBag<UndirectedGraph>::TreeDecompositionBag(TreeDecompositionBag const& other) 
		: vertices(other.vertices), 
		  children(other.children)
	{
	}

	template <class UndirectedGraph>
	TreeDecompositionBag<UndirectedGraph>::~TreeDecompositionBag() 
	{
	}

	template <class UndirectedGraph>
	typename TreeDecompositionBag<UndirectedGraph>::VertexSet& 
	TreeDecompositionBag<UndirectedGraph>::getInnerVertices()
	{
		return vertices;
	}

	template <class UndirectedGraph>
	void TreeDecompositionBag<UndirectedGraph>::addChild(TreeDecompositionBag* bag)
	{
		children.push_back(bag);
	}

	template <class UndirectedGraph>
	void TreeDecompositionBag<UndirectedGraph>::addInnerVertex(VertexType& id)
	{
		vertices.insert(id);
	}

	template <class UndirectedGraph>
	Size TreeDecompositionBag<UndirectedGraph>::size() const
	{
		return vertices.size();
	}

	template <class UndirectedGraph>
	Size TreeDecompositionBag<UndirectedGraph>::numberOfChildren() const
	{
		return children.size();
	}

	template <class UndirectedGraph>
	TreeDecompositionBag<UndirectedGraph>& TreeDecompositionBag<UndirectedGraph>::firstChild()
	{
		if (children.empty())
		{
			throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		} else {
			return *children.front();
		}
	}

	template <class UndirectedGraph>
	typename std::vector<TreeDecompositionBag<UndirectedGraph>*>::iterator 
	TreeDecompositionBag<UndirectedGraph>::beginChild()
	{
		return children.begin();
	}

	template <class UndirectedGraph>
	typename std::vector<TreeDecompositionBag<UndirectedGraph>*>::iterator 
	TreeDecompositionBag<UndirectedGraph>::endChild()
	{
		return children.end();
	}

	template <class UndirectedGraph>
	typename std::vector<TreeDecompositionBag<UndirectedGraph>*>::const_iterator 
	TreeDecompositionBag<UndirectedGraph>::beginChild() const
	{
		return children.begin();
	}

	template <class UndirectedGraph>
	typename std::vector<TreeDecompositionBag<UndirectedGraph>*>::const_iterator 
	TreeDecompositionBag<UndirectedGraph>::endChild() const
	{
		return children.end();
	}

	template <class UndirectedGraph>
	TreeDecompositionBag<UndirectedGraph>& TreeDecompositionBag<UndirectedGraph>::operator=(TreeDecompositionBag const& other)
	{
		if (&other != this)
		{
			vertices = other.vertices;
			children = other.children;
		}

		return *this;
	}

	/**
	 * TreeNodeList which handles allocating/deallocating of a tree-decomposition
	 */
	template <class UndirectedGraph>
	class TreeNodeListTraits
	{
	};


	/**
	 * Builds a tree decomposition by the given elimination order
	 * @param graph The source graph for which the tree decomposition is built
	 * @param permutation the elimination order which is used to build the tree
	 * @param treeDecomposition an empty TreeNodeList which is filled with instances of TreeDecompositionBag
	 */
	template <class UndirectedGraph, class TreeDecomposition>
	void eliminationOrderToTreeDecomposition(UndirectedGraph const& graph, 
	                                         typename TreeWidthTraits<UndirectedGraph>::EliminationOrder const& permutation,
																					 TreeDecomposition& treeDecomposition);

	template <class UndirectedGraph, class TreeDecomposition>
	void eliminationOrderToTreeDecomposition(UndirectedGraph& originalGraph, 
	                                         typename TreeWidthTraits<UndirectedGraph>::EliminationOrder& permutation,
																					 TreeDecomposition& treeDecomposition)
	{
		typedef typename TreeWidthTraits<UndirectedGraph>::VertexType        VertexType;
		typedef typename TreeWidthTraits<UndirectedGraph>::NeighbourIterator NeighbourIterator;

		if (num_vertices(originalGraph) == 0)
		{
			return;
		}

		UndirectedGraph graph(originalGraph);

		std::map<VertexType, int> permutation_order;
		Size last_vertex_id = permutation.first.size() - 1;

		std::vector<TreeDecompositionBag<UndirectedGraph>*> bags(last_vertex_id+1, NULL);
		std::vector<GRAPH::UndoEliminateOperation<UndirectedGraph> > eliminations(last_vertex_id, NULL);

		// first: eliminate each vertex and remember its permutation order
		for (Size i = 0; i <= last_vertex_id; ++i)
		{
			VertexType const& vertex = permutation.first[i];
			permutation_order[vertex] = i;

			if (i < last_vertex_id)
			{
				eliminations[i] = GRAPH::eliminateVertexUndoable(vertex, graph);
			}
		}

		// last vertex is root of tree decomposition
		treeDecomposition.getRoot()->addInnerVertex(permutation.first[last_vertex_id]);
		bags[last_vertex_id] = treeDecomposition.getRoot();

		// next: de-eliminate each vertex and add it and its neighbours to the tree decomposition
		for (Index i = last_vertex_id - 1; i >= 0; --i)
		{
			eliminations[i].undo();

			VertexType vertex = permutation.first[i];

			TreeDecompositionBag<UndirectedGraph>* bag(treeDecomposition.newVertex());
			bags[i] = bag;

			// add it to bag
			bag->addInnerVertex(vertex);

			// add its neighbours to bag and search the neighbour with minimal permutation order
			NeighbourIterator neighbour;
			Size min_permutation_order = last_vertex_id;
			BGL_FORALL_ADJ_T(vertex, neighbour, graph, UndirectedGraph)
			{
				bag->addInnerVertex(neighbour);

				min_permutation_order = std::min(min_permutation_order, permutation_order[neighbour]);
			}

			// the bag with the neighour with minimal permutation order becomes parent of the
			// new created bag
			bags[min_permutation_order]->addChild(bag);
		}
	}

	
	/**
	 * A nice tree decomposition bag is a bag of a nice tree decomposition. Such a nice tree decomposition
	 * is a binary tree with five vertex types:
	 * - Introduce-nodes, which have one child and one more inner vertex than their child
	 * - Forget-nodes, which have one child and one inner vertex less than their child
	 * - Join-nodes, which have two childs and the same inner vertices as their childs
	 * - Leaf-nodes, which have no childs and exactly one inner vertex
	 * - Root-nodes, which have one child and no inner vertices
	 */
	template <class UndirectedGraph>
	class NiceTreeDecompositionBag
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;

			typedef typename TreeDecompositionBag<UndirectedGraph>::BagType BagType;
			typedef typename TreeDecompositionBag<UndirectedGraph>::VertexSet VertexSet;

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

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>::NiceTreeDecompositionBag() 
		: children(), 
		  vertices(),
			specialVertex(), 
			bagType(TreeDecompositionBag<UndirectedGraph>::END_BAG), 
			index(0)
	{
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>::NiceTreeDecompositionBag(Size index) 
		: children(), 
			vertices(),
			specialVertex(), 
			bagType(TreeDecompositionBag<UndirectedGraph>::END_BAG), 
			index(index)
	{
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>::NiceTreeDecompositionBag(NiceTreeDecompositionBag const& copy) 
		: children(), 
		  vertices(copy.vertices), 
			specialVertex(copy.specialVertex), 
			bagType(copy.bagType), 
			index(copy.index)
	{
		children[0] = copy.children[0];
		children[1] = copy.children[1];
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>& 
	NiceTreeDecompositionBag<UndirectedGraph>::operator=(NiceTreeDecompositionBag const& copy)
	{
		if (this != &copy)
		{
			children[0] = copy.children[0];
			children[1] = copy.children[1];

			vertices = copy.vertices;
			specialVertex = copy.specialVertex;
			bagType = copy.bagType;
			index = copy.index;
		}

		return *this;
	}

	template <class UndirectedGraph>
	Size NiceTreeDecompositionBag<UndirectedGraph>::getIndex() const
	{
		return index;
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>& NiceTreeDecompositionBag<UndirectedGraph>::firstChild()
	{
		if (   bagType == TreeDecompositionBag<UndirectedGraph>::END_BAG 
			  || bagType == TreeDecompositionBag<UndirectedGraph>::LEAF_BAG)
		{
			throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		} 
		else
		{
			return *children[0];
		}
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph> const& NiceTreeDecompositionBag<UndirectedGraph>::firstChild() const
	{
		if (   bagType == TreeDecompositionBag<UndirectedGraph>::END_BAG 
			  || bagType == TreeDecompositionBag<UndirectedGraph>::LEAF_BAG)
		{
			throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		} 
		else
		{
			return *children[0];
		}
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>& NiceTreeDecompositionBag<UndirectedGraph>::secondChild()
	{
		if (bagType != TreeDecompositionBag<UndirectedGraph>::JOIN_BAG)
		{
			throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		} 
		else
		{
			return *children[1];
		}
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph> const& NiceTreeDecompositionBag<UndirectedGraph>::secondChild() const
	{
		if (bagType != TreeDecompositionBag<UndirectedGraph>::JOIN_BAG)
		{
			throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		} 
		else
		{
			return *children[1];
		}
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph> * const * NiceTreeDecompositionBag<UndirectedGraph>::beginChild()
	{
		return children;
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph> * const * NiceTreeDecompositionBag<UndirectedGraph>::endChild()
	{
		switch (bagType)
		{
			case TreeDecompositionBag<UndirectedGraph>::LEAF_BAG:
			case TreeDecompositionBag<UndirectedGraph>::END_BAG:
				return children;
			case TreeDecompositionBag<UndirectedGraph>::INTRODUCE_BAG:
			case TreeDecompositionBag<UndirectedGraph>::FORGET_BAG:
			case TreeDecompositionBag<UndirectedGraph>::ROOT_BAG:
				return &children[1];
			case TreeDecompositionBag<UndirectedGraph>::JOIN_BAG:
				return &children[2];
		}

		return NULL; // shouldn't happen
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph> const * const * NiceTreeDecompositionBag<UndirectedGraph>::beginChild() const
	{
		return children;
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph> const * const * NiceTreeDecompositionBag<UndirectedGraph>::endChild() const
	{
		switch (bagType)
		{
			case TreeDecompositionBag<UndirectedGraph>::LEAF_BAG:
			case TreeDecompositionBag<UndirectedGraph>::END_BAG:
				return children;
			case TreeDecompositionBag<UndirectedGraph>::INTRODUCE_BAG:
			case TreeDecompositionBag<UndirectedGraph>::FORGET_BAG:
			case TreeDecompositionBag<UndirectedGraph>::ROOT_BAG:
				return &children[1];
			case TreeDecompositionBag<UndirectedGraph>::JOIN_BAG:
				return &children[2];
		}

		return NULL; // shouldn't happen
	}

	template <class UndirectedGraph>
	typename NiceTreeDecompositionBag<UndirectedGraph>::VertexSet const& 
	NiceTreeDecompositionBag<UndirectedGraph>::getInnerVertices() const
	{
		return vertices;
	}

	template <class UndirectedGraph>
	void NiceTreeDecompositionBag<UndirectedGraph>::setAsLeaf(VertexType& vertex)
	{
		bagType = TreeDecompositionBag<UndirectedGraph>::LEAF_BAG;
		specialVertex = vertex;
		vertices.clear();
		vertices.insert(vertex);
	}

	template <class UndirectedGraph>
	void NiceTreeDecompositionBag<UndirectedGraph>::setAsIntroduce(NiceTreeDecompositionBag<UndirectedGraph>& child, 
	                                                               VertexType& introducedVertex)
	{
		if (   child.getBagType() == TreeDecompositionBag<UndirectedGraph>::END_BAG 
			  || child.getBagType() == TreeDecompositionBag<UndirectedGraph>::ROOT_BAG)
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "The child of a vertex can't be a root or an invalid vertex!");
		}

		bagType = TreeDecompositionBag<UndirectedGraph>::INTRODUCE_BAG;
		vertices = child.vertices;
		vertices.insert(introducedVertex);
		specialVertex = introducedVertex;
		children[0] = &child;
	}

	template <class UndirectedGraph>
	void NiceTreeDecompositionBag<UndirectedGraph>::setAsForget(NiceTreeDecompositionBag<UndirectedGraph>& child, 
	                                                            VertexType& forgotten)
	{
		if (   child.getBagType() == TreeDecompositionBag<UndirectedGraph>::END_BAG 
			  || child.getBagType() == TreeDecompositionBag<UndirectedGraph>::ROOT_BAG)
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "The child of a vertex can't be a root or an invalid vertex!");
		}

		bagType = TreeDecompositionBag<UndirectedGraph>::FORGET_BAG;
		vertices = child.vertices;
		if (vertices.erase(forgotten) == 0)
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Can't forget an inner vertex which doesn't exist!");
		}

		specialVertex = forgotten;
		children[0] = &child;
	}

	template <class UndirectedGraph>
	void NiceTreeDecompositionBag<UndirectedGraph>::setAsJoin(NiceTreeDecompositionBag<UndirectedGraph>& left, 
	                                                          NiceTreeDecompositionBag<UndirectedGraph>& right)
	{
		if (   left.getBagType()  == TreeDecompositionBag<UndirectedGraph>::END_BAG 
			  || left.getBagType()  == TreeDecompositionBag<UndirectedGraph>::ROOT_BAG 
				|| right.getBagType() == TreeDecompositionBag<UndirectedGraph>::END_BAG 
				|| right.getBagType() == TreeDecompositionBag<UndirectedGraph>::ROOT_BAG)
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "The child of a vertex can't be a root or an invalid vertex!");
		}

		bagType = TreeDecompositionBag<UndirectedGraph>::JOIN_BAG;
		vertices = left.vertices;

		if (left.vertices != right.vertices)
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Both children of a join node should have the same inner vertex set!");
		}

		specialVertex = VertexType();
		children[0] = &left;
		children[1] = &right;
	}

	template <class UndirectedGraph>
	void NiceTreeDecompositionBag<UndirectedGraph>::setAsRoot(NiceTreeDecompositionBag<UndirectedGraph>& child, 
	                                                          VertexType& forgotten)
	{
		if (   child.getBagType() == TreeDecompositionBag<UndirectedGraph>::END_BAG 
			  || child.getBagType() == TreeDecompositionBag<UndirectedGraph>::ROOT_BAG)
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "The child of a vertex can't be a root or an invalid vertex!");
		}

		bagType = TreeDecompositionBag<UndirectedGraph>::ROOT_BAG;

		vertices.clear();
		specialVertex = forgotten;
		children[0] = &child;
	}

	template <class UndirectedGraph>
	typename NiceTreeDecompositionBag<UndirectedGraph>::BagType NiceTreeDecompositionBag<UndirectedGraph>::getBagType() const
	{
		return bagType;
	}

	template <class UndirectedGraph>
	typename NiceTreeDecompositionBag<UndirectedGraph>::VertexType& 
	NiceTreeDecompositionBag<UndirectedGraph>::getIntroducedVertex()
	{
		return specialVertex;
	}

	template <class UndirectedGraph>
	typename NiceTreeDecompositionBag<UndirectedGraph>::VertexType& 
	NiceTreeDecompositionBag<UndirectedGraph>::getForgottenVertex()
	{
		return specialVertex;
	}

	/**
	 * Builds a NiceTreeDecomposition from a TreeDecomposition
	 */
	template <class UndirectedGraph>
	class NiceTreeDecompositionBuilder
	{
		public:
			typedef typename TreeWidthTraits<UndirectedGraph>::VertexType VertexType;
			typedef TreeNodeList<NiceTreeDecompositionBag<UndirectedGraph> > NiceTreeDecomposition;
			typedef std::set<VertexType> VertexSet;

			typedef typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator ConstBagIterator; 

			/**
			 * Constructs a new builder which inserts it's vertices into the given NiceTreeDecomposition
			 */
			NiceTreeDecompositionBuilder(NiceTreeDecomposition& ntd);

			/**
			 * Builds a NiceTreeDecomposition from the TreeDecomposition rooted in the given vertex
			 */
			void buildNiceTreeDecomposition(TreeDecompositionBag<UndirectedGraph> const& root);

			NiceTreeDecompositionBag<UndirectedGraph>* operator () (TreeDecompositionBag<UndirectedGraph> const& node, 
                                                    			    NodeType nodeType, ConstBagIterator begin, ConstBagIterator end);

		protected:
			/**
			 * Manages the allocation of new nice tree decomposition vertices
			 */
			typename TreeWidthTraits<UndirectedGraph>::NiceTreeDecomposition* ntd;

			/**
			 * First this function builds a leaf node which introduce one vertex from the given tree decomposition bag.
			 * After this, the function builds introduce nodes for each further inner vertex in the bag.
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* buildLeaf(TreeDecompositionBag<UndirectedGraph> const&);

			/**
			 * This function builds forget nodes until all inner vertices of the child node are forgotten. This last
			 * forget node which has no inner vertices is called "root".
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* buildRoot(NiceTreeDecompositionBag<UndirectedGraph>* child);

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
			NiceTreeDecompositionBag<UndirectedGraph>* buildJoin(TreeDecompositionBag<UndirectedGraph> const& node, 
                       NiceTreeDecompositionBag<UndirectedGraph>* left, NiceTreeDecompositionBag<UndirectedGraph>* right, 
											 bool doForget);

			/**
			 * This function is called for inner vertices. If the inner vertices have more than one child, this function
			 * builds join nodes to keep the binary property of the nice tree decomposition.
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* branch(TreeDecompositionBag<UndirectedGraph> const&, NodeType nodeType,
                                               					ConstBagIterator begin, ConstBagIterator end);

			/**
			 * Is called for inner vertices with just one child. Builds forget and introduce nodes by calling #buildLinkage
			 * (or #buildRoot for the root node).
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* buildSingle(TreeDecompositionBag<UndirectedGraph> const& node, 
			                                         NodeType nodeType, NiceTreeDecompositionBag<UndirectedGraph>* child);

			/**
			 * Builds introduce and forget nodes to introduce all vertices, which are contained in
			 * the given bag but not in the child bag, and to forget all vertices which are contained in the child bag but not
			 * in the tree decomposition bag.
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* buildLinkage(TreeDecompositionBag<UndirectedGraph> const& node, 
			                                                        NiceTreeDecompositionBag<UndirectedGraph>* child);

			/**
			 * Builds introduce nodes to insert vertices which are contained in the parent set but not in the child bag
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* linkWithIntroduceNodes(VertexSet const& parentSet, 
			                                                                  NiceTreeDecompositionBag<UndirectedGraph>* child);

			/**
			 * Builds forget nodes to remove vertices which are contained in the child bag but not in the parent set
			 */
			NiceTreeDecompositionBag<UndirectedGraph>* linkWithForgetNodes(VertexSet const& parentSet, 
			                                                               NiceTreeDecompositionBag<UndirectedGraph>* child);

	};

	template <class UndirectedGraph>
	NiceTreeDecompositionBuilder<UndirectedGraph>::NiceTreeDecompositionBuilder(NiceTreeDecomposition& ntd) 
		: ntd(&ntd) 
	{
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::operator () (TreeDecompositionBag<UndirectedGraph> const& node, NodeType nodeType, 
	                                          typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator begin, 
																						typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator end)
	{
		switch (nodeType)
		{
			case LEAF_VERTEX:
				return buildLeaf(node);
			case INNER_VERTEX:
			case ROOT_VERTEX:
				if (begin == end)
				{
					throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
				}
				return buildSingle(node, nodeType, branch(node, nodeType, begin, end));
			case END_VERTEX:
			default:
				throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		}
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::branch(TreeDecompositionBag<UndirectedGraph> const& node, NodeType nodeType,
                                           typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator begin, 
																		       typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator end)
	{
		Size length = distance(begin, end);
		if (length == 0)
		{
			throw Exception::IllegalTreeOperation(__FILE__, __LINE__);
		} 
		else if (length == 1)
		{
			return *begin;
		} 
		else if (length == 2)
		{
			typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator now(begin);
			++begin;
			return buildJoin(node, *now, *begin, true);
		} 
		else
		{
			typename std::vector<NiceTreeDecompositionBag<UndirectedGraph>*>::const_iterator middle(begin + length/2);
			return buildJoin(
						node,
						branch(node, nodeType, begin, middle),
						branch(node, nodeType, middle, end), length < 4
					);
		}
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::buildLinkage(TreeDecompositionBag<UndirectedGraph> const& node, 
	                                                            NiceTreeDecompositionBag<UndirectedGraph>* child)
	{
		return linkWithIntroduceNodes(node.getInnerVertices(), linkWithForgetNodes(node.getInnerVertices(), child));
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::linkWithIntroduceNodes(VertexSet const& parentSet, 
	                                                                      NiceTreeDecompositionBag<UndirectedGraph>* child)
	{
		VertexSet const& childSet = child->getInnerVertices();
		NiceTreeDecompositionBag<UndirectedGraph>* currentParent(child);
		// for each vertex, which exists in parent and doesn't exists in child
		for (typename VertexSet::const_iterator iter = parentSet.begin(); iter != parentSet.end(); ++iter)
		{
			VertexType vertex = *iter;
			if (childSet.count(vertex) == 0)
			{
				// insert introduce node
				NiceTreeDecompositionBag<UndirectedGraph>* newParent(ntd->newVertex(NiceTreeDecompositionBag<UndirectedGraph>(ntd->size())));
				newParent->setAsIntroduce(*currentParent, vertex);
				currentParent = newParent;
			}
		}

		return currentParent;
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::linkWithForgetNodes(VertexSet const& parentSet, 
	                                                                   NiceTreeDecompositionBag<UndirectedGraph>* child)
	{
		VertexSet const& childSet = child->getInnerVertices();
		NiceTreeDecompositionBag<UndirectedGraph>* currentParent(child);

		// for each vertex, which exists in child and doesn't exists in parent
		for (typename VertexSet::const_iterator iter = childSet.begin(); iter != childSet.end(); ++iter)
		{
			VertexType vertex = *iter;
			if (parentSet.count(vertex) == 0)
			{
				// insert forget node
				NiceTreeDecompositionBag<UndirectedGraph>* newParent (ntd->newVertex(NiceTreeDecompositionBag<UndirectedGraph>(ntd->size())));
				newParent->setAsForget(*currentParent, vertex);
				currentParent = newParent;
			}
		}

		return currentParent;
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::buildLeaf(TreeDecompositionBag<UndirectedGraph> const& node)
	{
		VertexSet vset(node.getInnerVertices());
		VertexType leafInnerVertex = *vset.begin();
		vset.erase(vset.begin());
		NiceTreeDecompositionBag<UndirectedGraph>* vertex(ntd->newVertex(NiceTreeDecompositionBag<UndirectedGraph>(ntd->size())));
		vertex->setAsLeaf(leafInnerVertex);
		return linkWithIntroduceNodes(vset, vertex);
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::buildJoin(TreeDecompositionBag<UndirectedGraph> const& node, 
	                                                         NiceTreeDecompositionBag<UndirectedGraph>* left, 
																													 NiceTreeDecompositionBag<UndirectedGraph>* right, bool doForget)
	{
		// first: both children should have identical inner vertex sets
		VertexSet const& parentSet = node.getInnerVertices();
		if (doForget)
		{
			left = linkWithForgetNodes(parentSet, left);
			right = linkWithForgetNodes(parentSet, right);
		}
		left = linkWithIntroduceNodes(right->getInnerVertices(), left);
		right = linkWithIntroduceNodes(left->getInnerVertices(), right);
		// now both children should be equal
		NiceTreeDecompositionBag<UndirectedGraph>* join(ntd->newVertex(NiceTreeDecompositionBag<UndirectedGraph>(ntd->size())));
		join->setAsJoin(*left, *right);
		// the join node isn't finished, because it doesn't contains the introduce nodes. So call
		// buildSingle(join) to complete the node
		return join;
	}

	template <class UndirectedGraph>
	NiceTreeDecompositionBag<UndirectedGraph>* 
	NiceTreeDecompositionBuilder<UndirectedGraph>::buildRoot(NiceTreeDecompositionBag<UndirectedGraph>* child)
	{
		VertexType lastVertex;
		if (child->getInnerVertices().size() > 1)
		{
			VertexSet vs = child->getInnerVertices();
			lastVertex = *vs.begin();
			vs.erase(vs.begin());
			child = linkWithForgetNodes(vs, child);
		} 
		else
		{
			lastVertex = *child->getInnerVertices().begin();
		}

		*ntd->getRoot() = NiceTreeDecompositionBag<UndirectedGraph>(0);
		ntd->getRoot()->setAsRoot(*child, lastVertex);

		return ntd->getRoot();
	}

	template <class UndirectedGraph>
	void NiceTreeDecompositionBuilder<UndirectedGraph>::buildNiceTreeDecomposition(TreeDecompositionBag<UndirectedGraph> const& root)
	{
		if (root.numberOfChildren() > 0)
		{
			postOrderFolding<TreeDecompositionBag<UndirectedGraph> const, 
			                 NiceTreeDecompositionBag<UndirectedGraph>*, 
											 NiceTreeDecompositionBuilder<UndirectedGraph> >(root, *this);
		} 
		else if (root.size() > 0) 
		{
			buildRoot(buildLeaf(root));
		}
	}

	template <class UndirectedGraph, class Criterion, class Reducer>
	Size GeneralLowerBoundAlgorithm<UndirectedGraph, Criterion, Reducer>::operator() (UndirectedGraph const& original_graph)
	{
		UndirectedGraph graph_copy(original_graph);

		Size maximum = num_vertices(graph_copy);
		Size lower_bound = 0;

		while (num_vertices(graph_copy) > 1)
		{
			Size minimum = maximum;

			Criterion criterion(graph_copy);
			Reducer   reducer(graph_copy);

			VertexType best_vertex = *(vertices(graph_copy).first);
			VertexType current_vertex;

			BGL_FORALL_VERTICES_T(current_vertex, graph_copy, UndirectedGraph)
			{
				// find vertex which fits the criterium
				Size criterion_value = criterion(current_vertex);

				if (criterion_value < minimum)
				{
					best_vertex = current_vertex;
					minimum     = criterion_value;
				}
			}

			lower_bound = std::max(lower_bound, minimum);

			// now reduce graph by the found vertex
			reducer(best_vertex);
		}

		return lower_bound;
	}

	template <class UndirectedGraph>
	MinorMinWidthReducer<UndirectedGraph>::MinorMinWidthReducer(UndirectedGraph& graph) 
		: graph_(graph) 
	{
	}

	template <class UndirectedGraph>
	void MinorMinWidthReducer<UndirectedGraph>::operator () (VertexType& vertex)
	{
		if (out_degree(vertex, graph_) == 0)
		{
			throw UnconnectedGraphException(__FILE__, __LINE__, String("MinorMinWidth"));
		}

		// iterate over the neighbourhood of the given vertex
		VertexType min_vertex = *(adjacent_vertices(vertex, graph_).first());
		VertexType neighbour;

		Size min_degree = out_degree(min_vertex, graph_);

		BGL_FORALL_ADJ_T(vertex, neighbour, graph_, UndirectedGraph)
		{
			Size current_degree = out_degree(neighbour, graph_);

			if (current_degree < min_degree)
			{
				min_vertex = neighbour;
				min_degree = current_degree;
			}
		}

		if (min_degree < graph_.degree(vertex))
		{
			contractEdge(vertex, min_vertex);
		} 
		else
		{
			contractEdge(min_vertex, vertex);
		}
	}

	template <class UndirectedGraph>
	void MinorMinWidthReducer<UndirectedGraph>::contractEdge(VertexType& u, VertexType& v)
	{
		// iterate over v's neighbours
		VertexType neighbour;

		BGL_FORALL_ADJ_T(v, neighbour, graph_, UndirectedGraph)
		{
			// connect the neighbour to u
			add_edge(u, neighbour, graph_);
		}

		// remove v from the graph
		clear_vertex(v, graph_);
		remove_vertex(v, graph_);
	}

	template <class UndirectedGraph>
	MinorMinWidthCriterion<UndirectedGraph>::MinorMinWidthCriterion(UndirectedGraph const& graph) 
		: graph_(graph)
	{
	}

	template <class UndirectedGraph>
	Size MinorMinWidthCriterion<UndirectedGraph>::operator () (VertexType& a) const
	{
		return out_degree(a, graph_);
	}

	template <class UndirectedGraph, class Criterion>
	typename GreedyX<UndirectedGraph, Criterion>::EliminationOrder 
	GreedyX<UndirectedGraph, Criterion>::operator() (UndirectedGraph const& original_graph)
	{
		UndirectedGraph graph(original_graph);

		Criterion criterion;

		Size length = num_vertices(graph);
		std::vector<VertexType> permutation(length); // UNDEFINED_VERTEX_ID);

		if (length == 0)
			return EliminationOrder(permutation, length);

		Size upper_bound = 0;
		for (Size index = 0; index < length; ++index)
		{
			VertexType& vertex = criterion(graph);

			Size degree = out_degree(vertex, graph);
			if (degree == 0 && index < length-1)
			{
				throw UnconnectedGraphException(__FILE__, __LINE__, "GreedyX");
			}

			permutation[index] = vertex;

			upper_bound = std::max(upper_bound, degree);

			GRAPH::eliminateVertex(vertex, graph);
		}
		return EliminationOrder(permutation, upper_bound);
	}

	template <class Decomposition>
	TreeNodeList<Decomposition>::TreeNodeList() 
		: vertices(1, NULL), 
			root(NULL)
	{
		vertices.at(0) = new Decomposition();
		root = vertices.front();
	}

	template <class Decomposition>
	TreeNodeList<Decomposition>::TreeNodeList(TreeNodeList const& other) 
		: vertices(), 
			root(NULL)
	{
		(*this) = other;
	}

	template <class Decomposition>
	TreeNodeList<Decomposition>::~TreeNodeList()
	{
		clear;
	}

	template <class Decomposition>
	TreeNodeList<Decomposition>& TreeNodeList<Decomposition>::operator=(TreeNodeList const& other) 
	{
		// avoid self-assignment
		if (this == &other) 
		{
			return *this;
		}

		// first free own vertices
		clear();

		// then copy each vertex in other's list
		vertices.reserve(other.vertices.size());
		for (typename std::vector<Decomposition*>::const_iterator iter = other.vertices.begin(); 
		     iter != other.vertices.end(); ++iter)
		{
			vertices.push_back(new Decomposition(**iter));
		}

		root = vertices.front();

		return *this;
	}

	template <class Decomposition>
	void TreeNodeList<Decomposition>::clear()
	{
		root = NULL;

		for (typename std::vector<Decomposition*>::iterator iter = vertices.begin(); 
		     iter != vertices.end(); 
				 ++iter)
		{
			delete *iter;
		}
	}

	template <class Decomposition>
	Decomposition* TreeNodeList<Decomposition>::newVertex(Decomposition const& vertex)
	{
		Decomposition* v (new Decomposition(vertex));
		vertices.push_back(v);

		return v;
	}

	template <class Decomposition>
	Decomposition* TreeNodeList<Decomposition>::newVertex()
	{
		Decomposition* vertex (new Decomposition());
		vertices.push_back(vertex);

		return vertex;
	}

	template <class UndirectedGraph>
	typename FillInHeuristic<UndirectedGraph>::VertexType& FillInHeuristic<UndirectedGraph>::operator() (UndirectedGraph& graph)
	{
		VertexIterator v_i, v_j, v_end;
		tie(v_i, v_end) = vertices(graph);

		VertexIterator v_min = v_i;
		Size min_value = edgeIncreaseByEliminating(v_min, graph);

		if (min_value > 0)
		{
			for (; v_i != v_end; ++v_i)
			{
				Size value = edgeIncreaseByEliminating(v_i, graph);

				if (value < min_value)
				{
					v_min = v_i;
					min_value = value;

					if (min_value == 0)
					{
						return v_min;
					}
				}
			}
		}

		return v_min;
	}

	template <class UndirectedGraph>
	Size FillInHeuristic<UndirectedGraph>::edgeIncreaseByEliminating(VertexIterator vertex, UndirectedGraph& graph)
	{
		Size result = 0;

		typename boost::graph_traits<UndirectedGraph>::adjacency_iterator a_i, a_j, ai_end;

		for (tie(a_i, ai_end) = adjacent_vertices(vertex, graph); a_i != ai_end; ++a_i)
		{
			for (a_j = a_j+1; a_j != ai_end; ++a_j)
			{
				VertexType& i = *a_i;
				VertexType& j = *a_j;

				if (!edge(i, j, graph).second)
				{
					++result;
				}
			}
		}

		return result;
	}
}

#endif // BALL_DATATYPE_GRAPH_TREEWIDTH_H
