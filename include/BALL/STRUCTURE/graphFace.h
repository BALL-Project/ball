// $Id:

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#define BALL_STRUCTURE_GRAPHFACE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>

namespace BALL
{
	
	/** Generic GraphFace Class.	
			{\bf Definition:} \URL{BALL/STRUCTURE/graphFace.h}
	*/
	template <typename Vertex, typename Edge>
	class GraphFace
	{
		public:

		BALL_CREATE(GraphFace)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new GraphFace object.
		*/
		GraphFace()
			throw();

		/**	Copy constructor.
				Create a new GraphFace object from another.
				@param	face	the GraphFace object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											GraphFace object is linked to the neighbours of the old GraphFace object.
		*/
		GraphFace(const GraphFace<Vertex,Edge>& face, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new GraphFace object from some nice objects
				@param	vertices	assigned to the vertices
				@param	edges			assigned to the edges
				@param	index			assigned to the index
		*/
		GraphFace(const std::vector<Vertex*>& vertices,
				const std::vector<Edge*>& edges,
				Index index)
			throw();

		/**	Destructor.
				Destructs the GraphFace object.
		*/
		virtual ~GraphFace()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another GraphFace.
				@param	face	the GraphFace object to assign from
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the	
											GraphFace object is linked to the neighbours of the GraphFace object to be copied.
		*/
		virtual void set(const GraphFace<Vertex,Edge>& face, bool deep = false)
			throw();

		/**	Assign to a lot of nice objects
				@param	vertices	assigned to the vertices
				@param	edges			assigned to the edges
				@param	index			assigned to the index
		*/
		virtual void set(const std::vector<Vertex*>& vertices, const std::vector<Edge*>& edges, Index index)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
		*/
		virtual bool operator == (const GraphFace&) const
			throw();// = 0;

		/**	Inequality operator.
		*/
		virtual bool operator != (const GraphFace&) const
			throw();// = 0;
		
		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set one of the vertices of the GraphFace.
				@param	i				the relative index of the vertex which should be set. If i is greater than the
												current number of vertices, an exception is thrown
				@param	vertex	a pointer to the new vertex
		*/
		void setVertex(Position i, Vertex* vertex)
			throw(Exception::IndexOverflow);

		/** Return one of the vertices of the GraphFace.
				@param	i				the relative index of the vertex which should be given back. If i is greater than
												the current number of vertices, an exception is thrown
				@return	Vertex*	a pointer to the asked vertex
		*/
		Vertex* getVertex(Position i) const
			throw(Exception::IndexOverflow);

		/** Push a new vertex to the GraphFace.
				@param	vertex	a pointer to the new vertex
		*/
		void pushVertex(Vertex* vertex)
			throw();

		/** Return the number of vertices of the GraphFace.
				@return	Position	the number of vertices of the GraphFace
		*/
		Position numberOfVertices() const
			throw();

		/** Set one of the edges of the GraphFace.
				@param	i			the relative index of the edge which should be set. If i is greater than the
											current number of edges, an exception is thrown
				@param	edge	a pointer to the new edge
		*/
		void setEdge(Position i, Edge* edge)
			throw(Exception::IndexOverflow);

		/** Return one of the edges of the GraphFace.
				@param	i			the relative index of the edge which should be given back. If i is greater than
											the current number of edges, an exception is thrown
				@return	Edge*	a pointer to the asked vertex
		*/
		Edge* getEdge(Position i) const
			throw(Exception::IndexOverflow);

		/** Push a new edge to the GraphFace.
				@param	edge	a pointer to the new edge
		*/
		void pushEdge(Edge* edge)
			throw();

		/** Return the number of edges of the GraphFace.
				@return	Position	the number of edges of the GraphFace
		*/
		Position numberOfEdges() const
			throw();

		/** Set the index of the GraphFace.
				@param	index	the new index
		*/
		void setIndex(Index index)
			throw();

		/** Return the index of the GraphFace.
				@return	Index	the index of the GraphFace
		*/
		Index getIndex() const
			throw();

		/** Find the two edges of the GraphFace that belong to the given vertex
				@param	vertex	a pointer to the given vertex
				@param	edge1		a pointer to the first found edge
				@param	edge2		a pointer to the second found edge
				@return	bool		{\bf true} if the edges can be found, {\bf false} otherwise
		*/
		bool getEdges(Vertex* vertex, Edge*& edge1, Edge*& edge2) const
			throw();
		
		/** Find the edge of the GraphFace that belongs to the two given vertices
				@param	vertex1	a pointer to the first given vertex
				@param	vertex2	a pointer to the second given vertex
				@param	edge		a pointer to the found edge
				@return	bool		{\bf true} if the edge can be found, {\bf false} otherwise
		*/
		bool getEdge(Vertex* vertex1, Vertex* vertex2, Edge*& edge) const
			throw();
		
		/** Return the relative index of a vertex in the GraphFace.
				@return	Index	the relative index of the vertex
		*/
		Index getRelativeVertexIndex(Vertex* vertex) const
			throw();
		
		/** Return the relative index of an edge in the GraphFace.
				@return	Index	the relative index of the edge
		*/
		Index getRelativeEdgeIndex(Edge* edge) const
			throw();

		/** Substitute a vertex by an other one.
				@param	old_vertex	the vertex that has to be substituted
				@param	new_vertex	the new vertex
				@return	bool				{\bf true}, if the vertex can be substituted, {\bf false} otherwise
		*/
		bool substituteVertex(Vertex* old_vertex, Vertex* new_vertex)
			throw();

		/** Substitute an edge by an other one.
				@param	old_edge	the edge that has to be substituted
				@param	new_edge	the new edge
				@return	bool			{\bf true}, if the edge can be substituted, {\bf false} otherwise
		*/
		bool substituteEdge(Edge* old_edge, Edge* new_edge)
			throw();

		/*void remove(HashSet<GraphFace<Vertex,Edge>*>& faces)
		{
			vertex0_->deleteFace(this);
			vertex1_->deleteFace(this);
			vertex2_->deleteFace(this);
			GraphFace<Vertex,Edge>* face;
			if (edge0_ != NULL)
			{
				if ((edge0_->getFace(1) == NULL) && (edge0_->has() == false))
				{
					edge0_->remove();
				}
				else
				{
					face = edge0_->deleteFace(this);
					faces.insert(face);
				}
			}
			if (edge1_ != NULL)
			{
				faces.insert(edge1_->deleteFace(this));
			}
			if (edge2_ != NULL)
			{
				faces.insert(edge2_->deleteFace(this));
			}
		}*/

		//@}

		protected:

		/*_ The number of vertices of the GrpahFace
		*/
		Position number_of_vertices_;
		/*_ The vertices of the GraphFace
		*/
		std::vector<Vertex*> vertex_;
		/*_ The number of edges of the GrpahFace
		*/
		Position number_of_edges_;
		/*_ The edges of the GraphFace
		*/
		std::vector<Edge*> edge_;
		/* The index of the GraphFace
		*/
		Index index_;

	};



	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::GraphFace()
		throw()
		: number_of_vertices_(0),
			vertex_(),
			number_of_edges_(0),
			edge_(),
			index_(-1)
	{
	}


	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::GraphFace(const GraphFace<Vertex,Edge>& face, bool deep)
		throw()
		: number_of_vertices_(face.number_of_vertices_),
			vertex_(number_of_vertices_,NULL),
			number_of_edges_(face.number_of_edges_),
			edge_(number_of_edges_,NULL),
			index_(face.index_)
	{
		if (deep)
		{
			vertex_ = face.vertex_;
			edge_ = face.edge_;
		}
	}


	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::GraphFace(const std::vector<Vertex*>& vertices,
			const std::vector<Edge*>& edges,
			Index index)
		throw()
		: number_of_vertices_(vertices.size()),
			vertex_(vertices),
			number_of_edges_(edges.size()),
			edge_(edges),
			index_(index)
	{
	}


	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::~GraphFace()
		throw()
	{
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::set(const GraphFace<Vertex,Edge>& face, bool deep)
		throw()
	{
		if (deep)
		{
			number_of_vertices_ = face.number_of_vertices_;
			vertex_ = face.vertex_;
			number_of_edges_ = face.number_of_edges_;
			edge_ = face.edge_;
			index_ = face.index_;
		}
		else
		{
			number_of_vertices_ = face.number_of_vertices_;
			for (Position i = 0; i < number_of_vertices_; i++)
			{
				vertex_[i] = NULL;
			}
			number_of_edges_ = face.number_of_edges_;
			for (Position i = 0; i < number_of_edges_; i++)
			{
				edge_[i] = NULL;
			}
			index_ = face.index_;
		}
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::set(const std::vector<Vertex*>& vertices,
			const std::vector<Edge*>& edges,
			Index index)
		throw()
	{
		vertex_ = vertices;
		edge_ = edges;
		number_of_vertices_ = vertex_.size();
		number_of_edges_ = edge_.size();
		index_ = index;
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::setVertex(Position i, Vertex* vertex)
		throw(Exception::IndexOverflow)
	{
		if (i >= number_of_vertices_)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,number_of_vertices_-1);
		}
		else
		{
			vertex_[i] = vertex;
		}
	}


	template <typename Vertex, typename Edge>
	Vertex* GraphFace<Vertex,Edge>::getVertex(Position i) const
		throw(Exception::IndexOverflow)
	{
		if (i >= number_of_vertices_)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,number_of_vertices_-1);
		}
		else
		{
			return vertex_[i];
		}
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::pushVertex(Vertex* vertex)
		throw()
	{
		vertex_.push_back(vertex);
		number_of_vertices++;
	}


	template <typename Vertex, typename Edge>
	Position GraphFace<Vertex,Edge>::numberOfVertices() const
		throw()
	{
		return number_of_vertices_;
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::setEdge(Position i, Edge* edge)
		throw(Exception::IndexOverflow)
	{
		if (i >= number_of_edges_)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,number_of_vertices_-1);
		}
		else
		{
			edge_[i] = edge;
		}
	}


	template <typename Vertex, typename Edge>
	Edge* GraphFace<Vertex,Edge>::getEdge(Position i) const
		throw(Exception::IndexOverflow)
	{
		if (i >= number_of_edges_)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,number_of_vertices_-1);
		}
		else
		{
			return edge_[i];
		}
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::pushEdge(Edge* edge)
		throw()
	{
		edge_.push_back(edge);
		number_of_edges++;
	}


	template <typename Vertex, typename Edge>
	Position GraphFace<Vertex,Edge>::numberOfEdges() const
		throw()
	{
		return number_of_edges_;
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename Vertex, typename Edge>
	Index GraphFace<Vertex,Edge>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>::getEdges(Vertex* vertex,
			Edge*& edge1,
			Edge*& edge2) const
		throw()
	{
		Position i = 0;
		bool found1 = false;
		bool found2 = false;
		while ((!found1) && (i < number_of_edges_))
		{
			if ((edge_[i]->vertex_[0] == vertex) || (edge_[i]->vertex_[1] == vertex))
			{
				edge1 = edge_[i];
				found1 = true;
			}
			i++;
		}
		if (found1)
		{
			while ((!found2) && (i < number_of_edges_))
			{
				if ((edge_[i]->vertex_[0] == vertex) || (edge_[i]->vertex_[1] == vertex))
				{
					edge2 = edge_[i];
					found2 = true;
				}
				i++;
			}
		}
		return (found1 && found2);
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>::getEdge(Vertex* vertex1,
			Vertex* vertex2,
			Edge*& edge) const
		throw()
	{
		Position i = 0;
		bool found = false;
		while ((!found) && (i < number_of_edges_))
		{
			if (((edge_[i]->vertex_[0] == vertex1) && (edge_[i]->vertex_[1] == vertex2)) ||
					((edge_[i]->vertex_[0] == vertex2) && (edge_[i]->vertex_[1] == vertex1))		)
			{
				edge = edge_[i];
				found = true;
			}
			i++;
		}
		return found;
	}


	template <typename Vertex, typename Edge>
	Index GraphFace<Vertex,Edge>::getRelativeVertexIndex(Vertex* vertex) const
		throw()
	{
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			if (vertex_[i] == vertex)
			{
				return i;
			}
		}
		return -1;
	}


	template <typename Vertex, typename Edge>
	Index GraphFace<Vertex,Edge>::getRelativeEdgeIndex(Edge* edge) const
		throw()
	{
		for (Position i = 0; i < number_of_edges_; i++)
		{
			if (edge_[i] == edge)
			{
				return i;
			}
		}
		return -1;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>::substituteVertex(Vertex* old_vertex,
			Vertex* new_vertex)
		throw()
	{
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			if (vertex_[i] == old_vertex)
			{
				vertex_[i] = new_vertex;
				return true;
			}
		}
		return false;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>::substituteEdge(Edge* old_edge,
			Edge* new_edge)
		throw()
	{
		for (Position i = 0; i < number_of_edges_; i++)
		{
			if (edge_[i] == old_edge)
			{
				edge_[i] = new_edge;
				return true;
			}
		}
		return false;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>:: operator == (const GraphFace&) const
		throw()
	{
		return false;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>:: operator != (const GraphFace&) const
		throw()
	{
		return true;
	}

	
	

} // namespace BALL

#endif // BALL_STRUCTURE_RSFACE_H
