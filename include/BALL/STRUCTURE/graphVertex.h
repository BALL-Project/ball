// $Id:

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#define BALL_STRUCTURE_GRAPHVERTEX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <list>

namespace BALL
{
	
	/** Generic GraphVertex Class.	
			{\bf Definition:} \URL{BALL/STRUCTURE/graphVertex.h}	
	*/
	template <typename Edge, typename Face>
	class GraphVertex
	{
		public:

		BALL_CREATE(GraphVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new GraphVertex object.
		*/
		GraphVertex()
			throw();

		/**	Copy constructor.
				Create a new GraphVertex object from another.
				@param	vertex	the GraphVertex object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											GraphVertex object is linked to the neighbours of the old GraphVertex object.
		*/
		GraphVertex(const GraphVertex<Edge,Face>& vertex, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new GraphVertex object from two lists and an Index.
				@param	edges	assigned to list of edges
				@param	faces	assigned to list of faces
				@param	i			assigned to the index of the GraphVertex
		*/
		GraphVertex
				(const std::list<Edge*>& edges,
				 const std::list<Face*>& faces,
				 Index i)
			throw();

		/**	Destructor.
				Destructs the GraphVertex object.
		*/
		virtual ~GraphVertex()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another GraphVertex.
				@param	vertex	the GraphVertex object to assign from
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the	
											GraphVertex object is linked to the neighbours of the GraphVertex object to be copied.
		*/
		virtual void set(const GraphVertex<Edge,Face>& vertex, bool deep = false)
			throw();

		/**	Assign to two lists and an Index.
				@param	edges	assigned to list of edges
				@param	faces	assigned to list of faces
				@param	i			asiigned to the index of the GraphVertex
		*/
		virtual void set
				(const std::list<Edge*>& edges,
				 const std::list<Face*>& faces,
				 Index i)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the list of edges the GraphVertex belongs to.
				@param	edges	the new list of edges
		*/
		void setEdges(std::list<Edge*> edges)
			throw();

		/** Insert a new edge into the list of edges the GraphVertex belongs to.
				@param	edge	a pointer to the new edge
		*/
		void pushEdge(Edge* edge)
			throw();

		/** Delete a edge from the list of edges the GraphVertex belongs to.
				@param	edge	a pointer to the edge to delete
		*/
		void deleteEdge(Edge* edge)
			throw();

		/** Return the list of edges the GraphVertex belongs to.
				@return	std::list<Edge*>	the list of edges the GraphVertex belongs to
		*/
		std::list<Edge*> getEdges() const
			throw();

		/** Set the list of faces the GraphVertex belongs to.
				@param	faces	the new list of faces
		*/
		void setFaces(std::list<Face*> faces)
			throw();

		/** Insert a new face into the list of faces the GraphVertex belongs to.
				@param	face	a pointer to the new face
		*/
		void pushFace(Face* face)
			throw();

		/** Delete a face from the list of faces the GraphVertex belongs to.
				@param	edge	a pointer to the face to delete
		*/
		void deleteFace(Face* face)
			throw();

		/** Return the list of faces the GraphVertex belongs to.
				@return	std::list<Face*>	the list of faces the GraphVertex belongs to
		*/
		std::list<Face*> getFaces() const
			throw();

		/** Set the index of the vertex.
				@param	index	the new index
		*/
		void setIndex(Index index)
			throw();

		/** Return the index of the vertex.
				@return	Index	the index of the vertex
		*/
		Index getIndex() const
			throw();


		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true} if the vertices are equal in all	
											components, {\bf false} otherwise
		*/
		virtual bool operator == (const GraphVertex<Edge,Face>&) const
			throw();// = 0;

		/**	Inequality operator.
				@return	bool	{\bf false} if the vertices are equal in all	
											components, {\bf true} otherwise
		*/
		virtual bool operator != (const GraphVertex<Edge,Face>&) const
			throw();// = 0;

		/**	Test if the vertex is meber of a given face.
				@param	face	a pointer to the the face to be tested
				@return	Face*	a pointer to the face if it exists, otherwise {\bf NULL}
		*/
		Face* has(Face* face) const
			throw();

		/**	Test if the vertex is meber of a given edge.
				@param	edge	a pointer to the edge to be tested
				@return	Edge*	a pointer to the edge if it exists, otherwise {\bf NULL}
		*/
		Edge* has(Edge* edge) const
			throw();

		bool hasEdges() const
			throw()
		{
			return !edges_.isEmpty();
		}

		//@}

		protected:

		/*_ The RSEdges the RSVetex belongs to
		*/
		std::list<Edge*> edges_;
		/*_ The RSFaces the RSVetex belongs to
		*/
		std::list<Face*> faces_;
		/*_ The index of the GraphVertex
		*/
		Index index_;

	};



	template <typename Edge, typename Face>
	GraphVertex<Edge,Face>::GraphVertex()
		throw()
		: edges_(),
			faces_(),
			index_(-1)
	{
	}


	template <typename Edge, typename Face>
	GraphVertex<Edge,Face>::GraphVertex(const GraphVertex<Edge,Face>& vertex, bool deep)
		throw()
		: edges_(),
			faces_(),
			index_(vertex.index_)
	{
		if (deep)
		{
			edges_ = vertex.edges_;
			faces_ = vertex.faces_;
		}
	}


	template <typename Edge, typename Face>
	GraphVertex<Edge,Face>::GraphVertex
			(const std::list<Edge*>& edges,
			 const std::list<Face*>& faces,
			 Index i)
		throw()
		: edges_(edges),
			faces_(faces),
			index_(i)
	{
	}


	template <typename Edge, typename Face>
	GraphVertex<Edge,Face>::~GraphVertex()
		throw()
	{
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::set(const GraphVertex<Edge,Face>& vertex, bool deep)
		throw()
	{
		if (deep)
		{
			edges_ = vertex.edges_;
			faces_ = vertex.faces_;
		}
		index_ = vertex.index_;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::set
			(const std::list<Edge*>& edges,
			 const std::list<Face*>& faces,
			 Index i)
		throw()
	{
		edges_ = edges;
		faces_ = faces;
		index_ = i;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::setEdges(std::list<Edge*> edges)
		throw()
	{
		edges_ = edges;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::pushEdge(Edge* edge)
		throw()
	{
		edges_.push_back(edge);
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::deleteEdge(Edge* edge)
		throw()
	{
		edges_.remove(edge);
	}


	template <typename Edge, typename Face>
	std::list<Edge*> GraphVertex<Edge,Face>::getEdges() const
		throw()
	{
		return edges_;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::setFaces(std::list<Face*> faces)
		throw()
	{
		faces_ = faces;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::pushFace(Face* face)
		throw()
	{
		faces_.push_back(face);
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::deleteFace(Face* face)
		throw()
	{
		faces_.remove(face);
	}


	template <typename Edge, typename Face>
	std::list<Face*> GraphVertex<Edge,Face>::getFaces() const
		throw()
	{
		return faces_;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename Edge, typename Face>
	Index GraphVertex<Edge,Face>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename Edge, typename Face>
	Face* GraphVertex<Edge,Face>::has(Face* face) const
		throw()
	{
		typename std::list<Face*>::const_iterator f;
		for (f = faces_.begin(); f != faces_.end(); f++)
		{
			if (*(*f) == *face)
			{
				return *f;
			}
		}
		return NULL;
	}


	template <typename Edge, typename Face>
	Edge* GraphVertex<Edge,Face>::has(Edge* edge) const
		throw()
	{
		typename std::list<Edge*>::const_iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (*(*e) == *edge)
			{
				return *e;
			}
		}
		return NULL;
	}


	template <typename Edge, typename Face>
	bool GraphVertex<Edge,Face>::operator == (const GraphVertex<Edge,Face>&) const
		throw()
	{
		return false;
	}


	template <typename Edge, typename Face>
	bool GraphVertex<Edge,Face>::operator != (const GraphVertex<Edge,Face>&) const
		throw()
	{
		return true;
	}





} // namespace BALL

#endif // BALL_STRUCTURE_RSVERTEX_H
