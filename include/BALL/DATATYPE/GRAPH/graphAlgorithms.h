#ifndef BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H
#define BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/tree_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/copy.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

namespace boost
{
	enum vertex_atom_ptr_t { vertex_atom_ptr };
	enum vertex_orig_ptr_t { vertex_orig_ptr };

	enum edge_bond_ptr_t { edge_bond_ptr };
	enum edge_orig_ptr_t { edge_orig_ptr   };

	BOOST_INSTALL_PROPERTY(vertex, atom_ptr);
	BOOST_INSTALL_PROPERTY(vertex, orig_ptr);

	BOOST_INSTALL_PROPERTY(edge, bond_ptr);
	BOOST_INSTALL_PROPERTY(edge,   orig_ptr);
}

namespace BALL
{
	namespace GRAPH
	{
		template <class UndirectedGraph> class UndoEliminateOperation;

		/**
		 * is thrown if a operation is called while an object is not in the right state to
		 * handle it. For example, if you have to set some options or call some other methods
		 * before calling the operation.
		 */
		class BALL_EXPORT IllegalStateException 
			: public Exception::GeneralException
		{
			public:
				IllegalStateException(const char* file, int line, String message);
		};

		/**
		 * Is thrown if a function, which operates on connected graphs, is called with a unconnected graph.
		 */
		class BALL_EXPORT UnconnectedGraphException 
			: public Exception::GeneralException
		{
			public:
				UnconnectedGraphException(const char * file, int line);
				UnconnectedGraphException(const char * file, int line, BALL::String computation);
		};

		/**
		 *  This class combines a number of type definitions that depend on the graph type
		 */
		template <class Graph>
		class GraphTraits
		{
			public:
				typedef typename boost::graph_traits<Graph>::vertex_descriptor       VertexType;
				typedef typename boost::graph_traits<Graph>::vertex_iterator         VertexIterator;
				typedef typename boost::graph_traits<Graph>::adjacency_iterator      NeighbourIterator;
				typedef typename boost::graph_traits<Graph>::edge_descriptor         EdgeType;

				// this defines an editable version of the graph, i.e., a graph with list-based storage types
				// that has property maps on the edges and vertices pointing to edges and vertices of an instance
				// of the original graph type
				typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS,
								                      boost::property<boost::vertex_orig_ptr_t, VertexType,
																			                boost::property<boost::vertex_index_t, int> >,
																			boost::property<boost::edge_orig_ptr_t, EdgeType> > EditableGraph;
																
		};

		template <typename Graph1, typename Graph2>
		struct EditableEdgeCopier 
		{
			EditableEdgeCopier(const Graph1& /*g1*/, Graph2& g2) 
				: edge_orig_map(get(boost::edge_orig_ptr, g2))
			{}

			template <typename Edge1, typename Edge2>
			void operator()(const Edge1& e1, Edge2& e2) const 
			{
				put(edge_orig_map, e2, e1);
			}   

			mutable typename boost::property_map<Graph2, boost::edge_orig_ptr_t>::type edge_orig_map;
		};  

		template <typename Graph1, typename Graph2>
		inline EditableEdgeCopier<Graph1,Graph2>
		makeEditableEdgeCopier(const Graph1& g1, Graph2& g2)
		{
			return EditableEdgeCopier<Graph1,Graph2>(g1, g2);
		}

		template <typename Graph1, typename Graph2>
		struct EditableVertexCopier 
		{
			EditableVertexCopier(const Graph1& g1_, Graph2& g2_)
				: vertex_orig_map(get(boost::vertex_orig_ptr, g2_)),
				  g1(g1_),
				  g2(g2_)
			{}

			template <typename Vertex1, typename Vertex2>
			void operator()(const Vertex1& v1, Vertex2& v2) const 
			{
				boost::put(vertex_orig_map, v2, v1);
				boost::put(boost::vertex_index, g2, v2, boost::get(boost::vertex_index, g1, v1));
			}   

			mutable typename boost::property_map<Graph2, boost::vertex_orig_ptr_t>::type vertex_orig_map;
			Graph1 const& g1;
			Graph2& g2;
		};

		template <typename Graph1, typename Graph2>
		inline EditableVertexCopier<Graph1,Graph2>
		makeEditableVertexCopier(const Graph1& g1, Graph2& g2)
		{
			return EditableVertexCopier<Graph1,Graph2>(g1, g2);
		}
		
		/** Eliminate the vertex from the graph and fill in the resulting hole.
		 *
		 *  This function first builds a clique of the neighbourhood of the given vertex
		 *  and removes it afterwards
		 */
		template <class UndirectedGraph>
		void eliminateVertex(typename GraphTraits<UndirectedGraph>::VertexType& vertex, UndirectedGraph& graph)
		{
			typename GraphTraits<UndirectedGraph>::NeighbourIterator ai, bi, ai_end;

			for (boost::tie(ai, ai_end) = adjacent_vertices(vertex, graph); ai != ai_end; ++ai)
			{
				bi = ai; ++bi;
				for (; bi != ai_end; ++bi)
					if (*bi != *ai && !boost::edge(*ai, *bi, graph).second)
						boost::add_edge(*ai, *bi, graph);
			}

			boost::clear_vertex(vertex, graph);
			boost::remove_vertex(vertex, graph);
		}

		/** Eliminate the vertex from the graph and fill in the resulting hole.
		 *
		 *  This function first builds a clique of the neighbourhood of the given vertex
		 *  and removes it afterwards. This function returns an undo object that can
		 *  be used to undo the operation.
		 *  @param vertex The vertex to eliminate
		 *  @param graph  The graph containing the vertex
		 */
		template <class UndirectedGraph>
		UndoEliminateOperation<UndirectedGraph> eliminateVertexUndoable(typename GraphTraits<UndirectedGraph>::VertexType const& vertex, 
		   		                                                          UndirectedGraph& graph) 
		{
			typename GraphTraits<UndirectedGraph>::NeighbourIterator ai, bi, ai_end;
			UndoEliminateOperation<UndirectedGraph> result(graph, vertex);

			for (boost::tie(ai, ai_end) = adjacent_vertices(vertex, graph); ai != ai_end; ++ai)
			{
				result.getNeighbours().push_back(boost::get(boost::vertex_index, graph, *ai));

				result.getEdgeProperties().push_back(boost::get(boost::edge_all_t(), graph, boost::edge(vertex, *ai, graph).first));

				bi = ai; ++bi;
				for (; bi != ai_end; ++bi)
				{
					if (!boost::edge(*ai, *bi, graph).second)
					{
						boost::add_edge(*ai, *bi, graph);
						result.getEdges().push_back(std::make_pair(boost::get(boost::vertex_index, graph, *ai),
						                                                      boost::get(boost::vertex_index, graph, *bi)));
					}
				}
			}

			boost::clear_vertex(vertex, graph);
			boost::remove_vertex(vertex, graph);

			return result;
		}

		/**
		 * Elimination means: remove a vertex v from graph and build a clique with
		 * the neighbourhood of v. Sometimes it's usefull to reverse this operation. This class
		 * remembers the changes of an elimination and provide an undo-operation.
		 * It's important, that you can only undo an elimination, if the graph wasn't changed after the
		 * elimination operation. Currently, there is no check if this condition is fulfilled.
		 * Since undo may change pointers into the graph (depending on the underlying graph structure),
		 * the user has to hand a pointer to a map from old vertex pointers to new vertex pointers.
		 */
		template <class UndirectedGraph>
		class UndoEliminateOperation
		{
			public:
				typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor  VertexType;
				typedef typename boost::graph_traits<UndirectedGraph>::edge_descriptor    EdgeType;
				typedef typename boost::graph_traits<UndirectedGraph>::adjacency_iterator NeighbourIterator;

				typedef typename boost::property_traits<typename boost::property_map<UndirectedGraph, 
				                                                 boost::vertex_all_t>::type>::value_type VertexProperties;
				typedef typename boost::property_traits<typename boost::property_map<UndirectedGraph, 
				                                                 boost::edge_all_t>::type>::value_type EdgeProperties;

				/**
				 * Eliminate vertex a in undirectedGraph and remember the changes.
				 */
				UndoEliminateOperation(UndirectedGraph& graph, VertexType const& a);

				/**
				 * returns the eliminated vertex
				 */
				VertexType& getEliminatedVertex();

				/**
				 * reverse this operation. It's important that you can reverse this operation only if the
				 * graph is in the same state as immediately after the elimination. Furthermore you should call this
				 * method only one time! Currently there is no check if this conditions are fulfilled.
				 */
				VertexType undo();

				std::vector<std::pair<int, int> >& getEdges() { return edges_; }
				std::vector<EdgeProperties>& getEdgeProperties() { return edge_properties_; }
				std::vector<int>& getNeighbours() { return neighbours_; }

			protected:
				UndirectedGraph* graph;
				VertexType vertex;
				VertexProperties vertex_properties_;
				std::vector<std::pair<int, int> > edges_;
				std::vector<EdgeProperties> edge_properties_;
				std::vector<int> neighbours_;
				bool applied;
		};

		template <class UndirectedGraph>
		UndoEliminateOperation<UndirectedGraph>::UndoEliminateOperation(UndirectedGraph& ugraph, VertexType const& a)
			: graph(&ugraph), 
				vertex(a),
				edges_(), 
				neighbours_(),
				applied(true)
		{
			vertex_properties_ = boost::get(boost::vertex_all_t(), ugraph, a);
		}

		template <class UndirectedGraph>
		typename UndoEliminateOperation<UndirectedGraph>::VertexType& 
		UndoEliminateOperation<UndirectedGraph>::getEliminatedVertex()
		{
			return vertex;
		}

		template <class UndirectedGraph>
		typename UndoEliminateOperation<UndirectedGraph>::VertexType UndoEliminateOperation<UndirectedGraph>::undo()
		{
			if (!applied)
			{
				throw IllegalStateException(__FILE__, __LINE__, "Can't undo an elimination twice");
			}

			applied = false;

			VertexType v = boost::add_vertex(vertex_properties_, *graph);

			std::map<int, VertexType> index_to_vertex;
			BGL_FORALL_VERTICES_T(v, *graph, UndirectedGraph)
			{
				index_to_vertex[boost::get(boost::vertex_index, *graph, v)] = v;
			}

			for (Position i=0; i<neighbours_.size(); ++i)
			{
				boost::add_edge(v, index_to_vertex[neighbours_[i]], edge_properties_[i], *graph);
			}

			for (Position i=0; i<edges_.size(); ++i)
			{
				boost::remove_edge(index_to_vertex[edges_[i].first], index_to_vertex[edges_[i].second], *graph);
			}

			return v;
		}

		template <class UndirectedGraph>
		void deepCopy(const UndirectedGraph& src, UndirectedGraph& target)
		{
			typedef std::map<typename boost::graph_traits<UndirectedGraph>::vertex_descriptor,int> VertexIndexMap;
			typedef boost::associative_property_map<VertexIndexMap> VertexIndexPropertyMap;

			VertexIndexMap vertex_map;
			VertexIndexPropertyMap vertex_property_map(vertex_map);

			typename boost::graph_traits<UndirectedGraph>::vertices_size_type count = 0;

			typename boost::graph_traits<UndirectedGraph>::vertex_iterator vi, vend;
			for (boost::tie(vi, vend) = boost::vertices(src); vi != vend; ++vi)
				vertex_map[*vi] = count++;

			boost::copy_graph(src, target, vertex_index_map(vertex_property_map));
		}

		template <class Tree, class From, class To, class Functor>
		class PostOrderFolding
		{
			public:
				typedef typename Tree::children_iterator ChildrenIterator;

				typedef typename std::vector<To>::iterator        ArgumentIterator;

				PostOrderFolding(Tree& tree, Functor& f)
				  :	return_stack_(boost::shared_ptr<std::vector<To> >(new std::vector<To>())),
					  tree_(&tree),
					  f_(&f)
				{
					boost::traverse_tree(root(*tree_), *tree_, *this);
				}

				To getResult()
				{ 
					return *return_stack_->begin(); 
				}

				template <class Node>
				void preorder(Node, Tree&)
				{
				}

				template <class Node>
				void inorder(Node, Tree&)
				{
				}

				template <class Node>
				void postorder(Node n, Tree& t)
				{
					ChildrenIterator c_i, c_end;
					boost::tie(c_i, c_end) = children(n, t);

					bool is_leaf = (c_i == c_end);

					ArgumentIterator begin_arg = return_stack_->end();
					ArgumentIterator end_arg   = return_stack_->end();

					if (!is_leaf)
					{
						for (; c_i != c_end; ++c_i)
						{
							--begin_arg;
						}
					}

					To value = (*f_)(n, begin_arg, end_arg);

					if (begin_arg != end_arg)
					{
						return_stack_->erase(begin_arg, end_arg);
					}

					return_stack_->push_back(value);
				}

			protected:
				boost::shared_ptr<std::vector<To> > return_stack_;

				Tree*    tree_;
				Functor* f_;
		};
		
	}
}

#endif // BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H


