// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#define BALL_STRUCTURE_GRAPHFACE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <list>
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
		GraphFace(const std::list<Vertex*>& vertices,
				const std::list<Edge*>& edges,
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
		void set(const GraphFace<Vertex,Edge>& face, bool deep = false)
			throw();

		/**	Assign to a lot of nice objects
				@param	vertices	assigned to the vertices
				@param	edges			assigned to the edges
				@param	index			assigned to the index
		*/
		void set(const std::list<Vertex*>& vertices, const std::list<Edge*>& edges, Index index)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
		*/
		bool operator == (const GraphFace&) const
			throw();

		/**	Inequality operator.
		*/
		bool operator != (const GraphFace&) const
			throw();
		
		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Push a new vertex to the GraphFace.
				@param	vertex	a pointer to the new vertex
		*/
		void pushVertex(Vertex* vertex)
			throw();

		/** Delete a vertex from the GraphFace.
				@param	vertex	a pointer to the vertex to delete
		*/
		void deleteVertex(Vertex* vertex)
			throw();

		/** Get the vertices of the GraphFace.
				@return	std::list<Vertex*>	the vertices of the GraphFace
		*/
		std::list<Vertex*> getVertices() const
			throw();

		/** Return the number of vertices of the GraphFace.
				@return	Position	the number of vertices of the GraphFace
		*/
		Position numberOfVertices() const
			throw();

		/** Push a new edge to the GraphFace.
				@param	edge	a pointer to the new edge
		*/
		void pushEdge(Edge* edge)
			throw();

		/** Delete an edge from the GraphFace.
				@param	edge	a pointer to the edge to delete
		*/
		void deleteEdge(Edge* edge)
			throw();

		/** Return the edges of the GraphFace.
				@return	std::list<Edge*>	the edges of the GraphFace
		*/
		std::list<Edge*> getEdges() const
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

		//@}

		protected:

		/*_ The vertices of the GraphFace
		*/
		std::list<Vertex*> vertex_;
		/*_ The edges of the GraphFace
		*/
		std::list<Edge*> edge_;
		/* The index of the GraphFace
		*/
		Index index_;

	};



	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::GraphFace()
		throw()
		: vertex_(),
			edge_(),
			index_(-1)
	{
	}


	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::GraphFace(const GraphFace<Vertex,Edge>& face, bool deep)
		throw()
		: vertex_(),
			edge_(),
			index_(face.index_)
	{
		if (deep)
		{
			vertex_ = face.vertex_;
			edge_ = face.edge_;
		}
	}


	template <typename Vertex, typename Edge>
	GraphFace<Vertex,Edge>::GraphFace(const std::list<Vertex*>& vertices,
			const std::list<Edge*>& edges,
			Index index)
		throw()
		: vertex_(vertices),
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
			vertex_ = face.vertex_;
			edge_ = face.edge_;
			index_ = face.index_;
		}
		else
		{
			vertex_.clear();
			edge_.clear();
			index_ = face.index_;
		}
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::set(const std::list<Vertex*>& vertices,
			const std::list<Edge*>& edges,
			Index index)
		throw()
	{
		vertex_ = vertices;
		edge_ = edges;
		index_ = index;
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::pushVertex(Vertex* vertex)
		throw()
	{
		typename std::list<Vertex*>::iterator v = vertex_.begin();
		bool found = false;
		while ((!found) && (v != vertex_.end()))
		{
			found = (*v == vertex);
			v++;
		}
		if (!found)
		{
			vertex_.push_back(vertex);
		}
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::deleteVertex(Vertex* vertex)
		throw()
	{
		vertex_.remove(vertex);
	}


	template <typename Vertex, typename Edge>
	std::list<Vertex*> GraphFace<Vertex,Edge>::getVertices() const
		throw()
	{
		return vertex_;
	}


	template <typename Vertex, typename Edge>
	Position GraphFace<Vertex,Edge>::numberOfVertices() const
		throw()
	{
		return vertex_.size();
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::pushEdge(Edge* edge)
		throw()
	{
		typename std::list<Edge*>::iterator e = edge_.begin();
		bool found = false;
		while ((!found) && (e != edge_.end()))
		{
			found = (*e == edge);
			e++;
		}
		if (!found)
		{
			edge_.push_back(edge);
		}
	}


	template <typename Vertex, typename Edge>
	void GraphFace<Vertex,Edge>::deleteEdge(Edge* edge)
		throw()
	{
		edge_.remove(edge);
	}


	template <typename Vertex, typename Edge>
	std::list<Edge*> GraphFace<Vertex,Edge>::getEdges() const
		throw()
	{
		return edge_;
	}


	template <typename Vertex, typename Edge>
	Position GraphFace<Vertex,Edge>::numberOfEdges() const
		throw()
	{
		return edge_.size();
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
		bool found1 = false;
		bool found2 = false;
		typename std::list<Edge*>::const_iterator e = edge_.begin();
		while ((!found1) && (e != edge_.end()))
		{
			if (((*e)->vertex_[0] == vertex) || ((*e)->vertex_[1] == vertex))
			{
				edge1 = *e;
				found1 = true;
			}
			e++;
		}
		if (found1)
		{
			while ((!found2) && (e != edge_.end()))
			{
				if (((*e)->vertex_[0] == vertex) || ((*e)->vertex_[1] == vertex))
				{
					edge2 = *e;
					found2 = true;
				}
				e++;
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
		typename std::list<Edge*>::const_iterator e = edge_.begin();
		bool found = false;
		while ((!found) && (e != edge_.end()))
		{
			if ((((*e)->vertex_[0] == vertex1) && ((*e)->vertex_[1] == vertex2)) ||
					(((*e)->vertex_[0] == vertex2) && ((*e)->vertex_[1] == vertex1))		)
			{
				edge = *e;
				found = true;
			}
			e++;
		}
		return found;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>::substituteVertex(Vertex* old_vertex, Vertex* new_vertex)
		throw()
	{
		vertex_.remove(old_vertex);
		vertex_.push_back(new_vertex);
		return true;
	}


	template <typename Vertex, typename Edge>
	bool GraphFace<Vertex,Edge>::substituteEdge(Edge* old_edge,
			Edge* new_edge)
		throw()
	{
		edge_.remove(old_edge);
		edge_.push_back(new_edge);
		return true;
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

	
	
	/** Generic GraphTriangle Class.	
			{\bf Definition:} \URL{BALL/STRUCTURE/graphFace.h}
	*/
	template <typename Vertex, typename Edge>
	class GraphTriangle
	{
		public:

		BALL_CREATE(GraphTriangle)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new GraphTriangle object.
		*/
		GraphTriangle()
			throw();

		/**	Copy constructor.
				Create a new GraphTriangle object from another.
				@param	face	the GraphTriangle object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											GraphTriangle object is linked to the neighbours of the old GraphTriangle object.
		*/
		GraphTriangle(const GraphTriangle<Vertex,Edge>& face, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new GraphTriangle object from some nice objects
				@param	vertex1	assigned to the first vertex
				@param	vertex2	assigned to the second vertex
				@param	vertex3	assigned to the third vertex
				@param	edge1		assigned to the first edge
				@param	edge2		assigned to the second edge
				@param	edge3		assigned to the third edge
				@param	index			assigned to the index
		*/
		GraphTriangle(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3,
				Edge* edge1, Edge* edge2, Edge* edge3,
				Index index)
			throw();

		/**	Destructor.
				Destructs the GraphTriangle object.
		*/
		virtual ~GraphTriangle()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another GraphTriangle.
				@param	face	the GraphTriangle object to assign from
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the	
											GraphTriangle object is linked to the neighbours of the GraphTriangle	
											object to be copied.
		*/
		void set(const GraphTriangle<Vertex,Edge>& face, bool deep = false)
			throw();

		/**	Assign to a lot of nice objects
				@param	vertex1	assigned to the first vertex
				@param	vertex2	assigned to the second vertex
				@param	vertex3	assigned to the third vertex
				@param	edge1		assigned to the first edge
				@param	edge2		assigned to the second edge
				@param	edge3		assigned to the third edge
				@param	index		assigned to the index
		*/
		void set(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3,
				Edge* edge1, Edge* edge2, Edge* edge3,
				Index index)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
		*/
		bool operator == (const GraphTriangle&) const
			throw();

		/**	Inequality operator.
		*/
		bool operator != (const GraphTriangle&) const
			throw();
		
		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set one of the vertices of the GraphTriangle.
				@param	i				the relative index of the vertex which should be set. If i is greater three,	
												an exception is thrown
				@param	vertex	a pointer to the new vertex
		*/
		void setVertex(Position i, Vertex* vertex)
			throw(Exception::IndexOverflow);

		/** Return one of the vertices of the GraphTriangle.
				@param	i				the relative index of the vertex which should be given back. If i is greater than
												three, an exception is thrown
				@return	Vertex*	a pointer to the asked vertex
		*/
		Vertex* getVertex(Position i) const
			throw(Exception::IndexOverflow);

		/** Set one of the edges of the GraphTriangle.
				@param	i			the relative index of the edge which should be set. If i is greater than three,	
											an exception is thrown
				@param	edge	a pointer to the new edge
		*/
		void setEdge(Position i, Edge* edge)
			throw(Exception::IndexOverflow);

		/** Return one of the edges of the GraphTriangle.
				@param	i			the relative index of the edge which should be given back. If i is greater than	
											three, an exception is thrown
				@return	Edge*	a pointer to the asked vertex
		*/
		Edge* getEdge(Position i) const
			throw(Exception::IndexOverflow);

		/** Set the index of the GraphTriangle.
				@param	index	the new index
		*/
		void setIndex(Index index)
			throw();

		/** Return the index of the GraphTriangle.
				@return	Index	the index of the GraphTriangle
		*/
		Index getIndex() const
			throw();

		/** Find the two edges of the GraphTriangle that belong to the given vertex
				@param	vertex	a pointer to the given vertex
				@param	edge1		a pointer to the first found edge
				@param	edge2		a pointer to the second found edge
				@return	bool		{\bf true} if the edges can be found, {\bf false} otherwise
		*/
		bool getEdges(Vertex* vertex, Edge*& edge1, Edge*& edge2) const
			throw();
		
		/** Find the edge of the GraphTriangle that belongs to the two given vertices
				@param	vertex1	a pointer to the first given vertex
				@param	vertex2	a pointer to the second given vertex
				@param	edge		a pointer to the found edge
				@return	bool		{\bf true} if the edge can be found, {\bf false} otherwise
		*/
		bool getEdge(Vertex* vertex1, Vertex* vertex2, Edge*& edge) const
			throw();
		
		/** Return the relative index of a vertex in the GraphTriangle.
				@return	Index	the relative index of the vertex
		*/
		Index getRelativeVertexIndex(Vertex* vertex) const
			throw();
		
		/** Return the relative index of an edge in the GraphTriangle.
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

		//@}

		protected:

		/*_ The vertices of the GraphTriangle
		*/
		std::vector<Vertex*> vertex_;
		/*_ The edges of the GraphTriangle
		*/
		std::vector<Edge*> edge_;
		/* The index of the GraphTriangle
		*/
		Index index_;

	};



	template <typename Vertex, typename Edge>
	GraphTriangle<Vertex,Edge>::GraphTriangle()
		throw()
		: vertex_(3),
			edge_(3),
			index_(-1)
	{
	}


	template <typename Vertex, typename Edge>
	GraphTriangle<Vertex,Edge>::GraphTriangle(const GraphTriangle<Vertex,Edge>& face, bool deep)
		throw()
		: vertex_(3),
			edge_(3),
			index_(face.index_)
	{
		if (deep)
		{
			vertex_ = face.vertex_;
			edge_ = face.edge_;
		}
	}


	template <typename Vertex, typename Edge>
	GraphTriangle<Vertex,Edge>::GraphTriangle(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3,
			Edge* edge1, Edge* edge2, Edge* edge3,
			Index index)
		throw()
		: vertex_(3),
			edge_(3),
			index_(index)
	{
		vertex_[0] = vertex1;
		vertex_[1] = vertex2;
		vertex_[2] = vertex3;
		edge_[0] = edge1;
		edge_[1] = edge2;
		edge_[2] = edge3;
	}


	template <typename Vertex, typename Edge>
	GraphTriangle<Vertex,Edge>::~GraphTriangle()
		throw()
	{
	}


	template <typename Vertex, typename Edge>
	void GraphTriangle<Vertex,Edge>::set(const GraphTriangle<Vertex,Edge>& face, bool deep)
		throw()
	{
		if (deep)
		{
			vertex_ = face.vertex_;
			edge_ = face.edge_;
			index_ = face.index_;
		}
		else
		{
			vertex_[0] = NULL;
			vertex_[1] = NULL;
			vertex_[2] = NULL;
			edge_[0] = NULL;
			edge_[1] = NULL;
			edge_[2] = NULL;
			index_ = face.index_;
		}
	}


	template <typename Vertex, typename Edge>
	void GraphTriangle<Vertex,Edge>::set(Vertex* vertex1, Vertex* vertex2, Vertex* vertex3,
			Edge* edge1, Edge* edge2, Edge* edge3,
			Index index)
		throw()
	{
		vertex_[0] = vertex1;
		vertex_[1] = vertex2;
		vertex_[2] = vertex3;
		edge_[0] = edge1;
		edge_[1] = edge2;
		edge_[2] = edge3;
		index_ = index;
	}


	template <typename Vertex, typename Edge>
	void GraphTriangle<Vertex,Edge>::setVertex(Position i, Vertex* vertex)
		throw(Exception::IndexOverflow)
	{
		if (i > 2)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
		else
		{
			vertex_[i] = vertex;
		}
	}


	template <typename Vertex, typename Edge>
	Vertex* GraphTriangle<Vertex,Edge>::getVertex(Position i) const
		throw(Exception::IndexOverflow)
	{
		if (i > 2)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
		else
		{
			return vertex_[i];
		}
	}


	template <typename Vertex, typename Edge>
	void GraphTriangle<Vertex,Edge>::setEdge(Position i, Edge* edge)
		throw(Exception::IndexOverflow)
	{
		if (i > 2)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
		else
		{
			edge_[i] = edge;
		}
	}


	template <typename Vertex, typename Edge>
	Edge* GraphTriangle<Vertex,Edge>::getEdge(Position i) const
		throw(Exception::IndexOverflow)
	{
		if (i > 2)
		{
			throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
		else
		{
			return edge_[i];
		}
	}


	template <typename Vertex, typename Edge>
	void GraphTriangle<Vertex,Edge>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename Vertex, typename Edge>
	Index GraphTriangle<Vertex,Edge>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename Vertex, typename Edge>
	bool GraphTriangle<Vertex,Edge>::getEdges(Vertex* vertex,
			Edge*& edge1,
			Edge*& edge2) const
		throw()
	{
		Position i = 0;
		bool found1 = false;
		bool found2 = false;
		while ((!found1) && (i < 3))
		{
			if (edge_[i] != NULL)
			{
				if ((edge_[i]->vertex_[0] == vertex) || (edge_[i]->vertex_[1] == vertex))
				{
					edge1 = edge_[i];
					found1 = true;
				}
			}
			i++;
		}
		if (found1)
		{
			while ((!found2) && (i < 3))
			{
				if (edge_[i] != NULL)
				{
					if ((edge_[i]->vertex_[0] == vertex) || (edge_[i]->vertex_[1] == vertex))
					{
						edge2 = edge_[i];
						found2 = true;
					}
				}
				i++;
			}
		}
		return (found1 && found2);
	}


	template <typename Vertex, typename Edge>
	bool GraphTriangle<Vertex,Edge>::getEdge(Vertex* vertex1,
			Vertex* vertex2,
			Edge*& edge) const
		throw()
	{
		Position i = 0;
		bool found = false;
		while ((!found) && (i < 3))
		{
			if (edge_[i] != NULL)
			{
				if (((edge_[i]->vertex_[0] == vertex1) && (edge_[i]->vertex_[1] == vertex2)) ||
						((edge_[i]->vertex_[0] == vertex2) && (edge_[i]->vertex_[1] == vertex1))		)
				{
					edge = edge_[i];
					found = true;
				}
			}
			i++;
		}
		return found;
	}


	template <typename Vertex, typename Edge>
	Index GraphTriangle<Vertex,Edge>::getRelativeVertexIndex(Vertex* vertex) const
		throw()
	{
		for (Position i = 0; i < 3; i++)
		{
			if (vertex_[i] == vertex)
			{
				return i;
			}
		}
		return -1;
	}


	template <typename Vertex, typename Edge>
	Index GraphTriangle<Vertex,Edge>::getRelativeEdgeIndex(Edge* edge) const
		throw()
	{
		for (Position i = 0; i < 3; i++)
		{
			if (edge_[i] == edge)
			{
				return i;
			}
		}
		return -1;
	}


	template <typename Vertex, typename Edge>
	bool GraphTriangle<Vertex,Edge>::substituteVertex(Vertex* old_vertex, Vertex* new_vertex)
		throw()
	{
		for (Position i = 0; i < 3; i++)
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
	bool GraphTriangle<Vertex,Edge>::substituteEdge(Edge* old_edge,
			Edge* new_edge)
		throw()
	{
		for (Position i = 0; i < 3; i++)
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
	bool GraphTriangle<Vertex,Edge>:: operator == (const GraphTriangle&) const
		throw()
	{
		return false;
	}


	template <typename Vertex, typename Edge>
	bool GraphTriangle<Vertex,Edge>:: operator != (const GraphTriangle&) const
		throw()
	{
		return true;
	}

	
	

} // namespace BALL

#endif // BALL_STRUCTURE_RSFACE_H
