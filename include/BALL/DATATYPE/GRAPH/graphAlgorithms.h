#ifndef BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H
#define BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/tree_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>

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
		 *  This class combines a number of type definitions that depend on the graph type
		 */
		template <class UndirectedGraph>
		class GraphTraits
		{
			public:
				typedef typename boost::graph_traits<UndirectedGraph>::vertex_descriptor       VertexType;
				typedef typename boost::graph_traits<UndirectedGraph>::vertex_iterator         VertexIterator;
				typedef typename boost::graph_traits<UndirectedGraph>::adjacency_iterator NeighbourIterator;
				typedef typename boost::graph_traits<UndirectedGraph>::edge_descriptor EdgeType;
		};

		/** Eliminate the vertex from the graph and fill in the resulting hole.
		 *
		 *  This function first builds a clique of the neighbourhood of the given vertex
		 *  and removes it afterwards
		 */
		template <class UndirectedGraph>
		void eliminateVertex(typename GraphTraits<UndirectedGraph>::VertexType& vertex, UndirectedGraph& graph)
		{
			typename GraphTraits<UndirectedGraph>::NeighbourIterator ai, bi, ai_end;

			for (tie(ai, ai_end) = adjacent_vertices(vertex, graph); ai != ai_end; ++ai)
				for (bi = ai+1; bi != ai_end; ++bi)
					add_edge(*ai, *bi, graph);

			clear_vertex(vertex, graph);
			remove_vertex(vertex, graph);
		}

		/** Eliminate the vertex from the graph and fill in the resulting hole.
		 *
		 *  This function first builds a clique of the neighbourhood of the given vertex
		 *  and removes it afterwards. This function returns an undo object that can
		 *  be used to undo the operation.
		 */
		template <class UndirectedGraph>
		UndoEliminateOperation<UndirectedGraph> eliminateVertexUndoable(typename GraphTraits<UndirectedGraph>::VertexType& vertex, 
				                                                            UndirectedGraph& graph)
		{
			typename GraphTraits<UndirectedGraph>::NeighbourIterator ai, bi, ai_end;
			UndoEliminateOperation<UndirectedGraph> result(graph, vertex);

			for (tie(ai, ai_end) = adjacent_vertices(vertex, graph); ai != ai_end; ++ai)
			{
				result.getNeighbours().push_back(*ai);

				for (bi = ai+1; bi != ai_end; ++bi)
				{
					if (!edge(*ai, *bi).second)
					{
						result.getEdges().push_back(add_edge(*ai, *bi, graph));
					}
				}
			}

			clear_vertex(vertex, graph);
			remove_vertex(vertex, graph);

			return result;
		}

		/**
		 * Elimination means: remove a vertex v from graph and build a clique with
		 * the neighbourhood of v. Sometimes it's usefull to reverse this operation. This class
		 * remembers the changes of an elimination and provide an undo-operation.
		 * It's important, that you can only undo an elimination, if the graph wasn't changed after the
		 * elimination operation. Currently, there is no check if this condition is fulfilled.
		 */
		template <class UndirectedGraph>
		class UndoEliminateOperation
		{
			public:
				typedef typename GraphTraits<UndirectedGraph>::VertexType VertexType;
				typedef typename GraphTraits<UndirectedGraph>::EdgeType   EdgeType;
				typedef typename GraphTraits<UndirectedGraph>::NeighbourIterator NeighbourIterator;

				/**
				 * Eliminate vertex a in undirectedGraph and remember the changes.
				 */
				UndoEliminateOperation(UndirectedGraph& graph, VertexType& a);

				/**
				 * returns the eliminated vertex
				 */
				VertexType& getEliminatedVertex();

				/**
				 * reverse this operation. It's important that you can reverse this operation only if the
				 * graph is in the same state as immediately after the elimination. Furthermore you should call this
				 * method only one time! Currently there is no check if this conditions are fulfilled.
				 */
				void undo();

				std::vector<EdgeType>& getEdges() { return edges_; }
				std::vector<EdgeType>& getNeighbours() { return neighbours_; }


			protected:
				UndirectedGraph* graph;
				VertexType vertex;
				std::vector<EdgeType> edges_;
				std::vector<VertexType> neighbours_;
				bool applied;
		};

		template <class UndirectedGraph>
		UndoEliminateOperation<UndirectedGraph>::UndoEliminateOperation(UndirectedGraph& ugraph, VertexType& a) 
			: graph(&ugraph), 
				vertex(a),
				neighbours_(),
				edges_(), 
				applied(true)
		{
		}

		template <class UndirectedGraph>
		typename UndoEliminateOperation<UndirectedGraph>::VertexType& 
		UndoEliminateOperation<UndirectedGraph>::getEliminatedVertex()
		{
			return vertex;
		}

		template <class UndirectedGraph>
		void UndoEliminateOperation<UndirectedGraph>::undo()
		{
			if (!applied)
			{
				throw IllegalStateException(__FILE__, __LINE__, "Can't undo an elimination twice");
			}

			applied = false;

			add_vertex(vertex, graph);

			for (Position i=0; i<neighbours_.size(); ++i)
			{
				add_edge(vertex, neighbours_[i], graph);
			}

			for (Position i=0; i<edges_.size(); ++i)
			{
				remove_edge(edges_[i], graph);
			}
		}

	}
}

#endif // BALL_DATATYPE_GRAPH_GRAPHALGORITHMS_H


