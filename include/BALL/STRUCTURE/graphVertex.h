// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: graphVertex.h,v 1.10 2003/03/14 11:50:01 sturm Exp $

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#define BALL_STRUCTURE_GRAPHVERTEX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

namespace BALL
{

	template <typename Vertex, typename Edge, typename Face>
	class GraphEdge;

	template <typename Vertex, typename Edge, typename Face>
	class GraphFace;

	template <typename Vertex, typename Edge, typename Face>
	class GraphTriangle;

	/** Generic GraphVertex Class.
			
	*/
	template <typename Vertex, typename Edge, typename Face>
	class GraphVertex
	{
		public:

		/** @name Class friends

					- class GraphEdge<Vertex,Edge,Face>;
					- class GraphFace<Vertex,Edge,Face>;
					- class GraphTriangle<Vertex,Edge,Face>;
				
		*/
		friend class GraphEdge<Vertex,Edge,Face>;
		friend class GraphFace<Vertex,Edge,Face>;
		friend class GraphTriangle<Vertex,Edge,Face>;

		BALL_CREATE(GraphVertex)

		/**	@name	Type definitions
		*/
		//@{

		typedef typename HashSet<Edge*>::Iterator EdgeIterator;
		typedef typename HashSet<Edge*>::ConstIterator ConstEdgeIterator;
		typedef typename HashSet<Face*>::Iterator FaceIterator;
		typedef typename HashSet<Face*>::ConstIterator ConstFaceIterator;

		//@}
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
				@param	deep	if deep = false, all pointers are set to NULL (default).
											Otherwise the new GraphVertex object is linked to the
											neighbours of the old GraphVertex object.
		*/
		GraphVertex(const GraphVertex<Vertex,Edge,Face>& vertex, bool deep = false)
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
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the GraphVertex object is linked	
												to the neighbours of the GraphVertex object to assign	
												from.
		*/
		void set(const GraphVertex<Vertex,Edge,Face>& vertex, bool deep = false)
			throw();

		/**	Assign from another GraphVertex.
				The GraphVertex object is linked to the neighbours of the GraphVertex	
				object to assign from.
				@param	vertex	the GraphVertex object to assign from
		*/
		GraphVertex<Vertex,Edge,Face>& operator =
				(const GraphVertex<Vertex,Edge,Face>& vertex)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Insert a new edge into the HashSet of edges the GraphVertex belongs to.
				@param	edge	a pointer to the new edge
		*/
		void insert(Edge* edge)
			throw();

		/** Insert a new face into the HashSet of faces the GraphVertex belongs to.
				@param	face	a pointer to the new face
		*/
		void insert(Face* face)
			throw();

		/** Remove an edge from the HashSet of edges the GraphVertex belongs to.
				@param	edge	a pointer to the edge to remove
		*/
		void remove(Edge* edge)
			throw();

		/** Remove a face from the HashSet of faces the GraphVertex belongs to.
				@param	edge	a pointer to the face to remove
		*/
		void remove(Face* face)
			throw();

		/** Return the number of edges the GraphVertex belongs to.
				@return	Position	the number of edges the GraphVertex belongs to
		*/
		Position numberOfEdges() const
			throw();

		/** Return the number of faces the GraphVertex belongs to.
				@return	Position	the number of faces the GraphVertex belongs to
		*/
		Position numberOfFaces() const
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

		/** Join two GraphVertices if they are similar.
				All edges and faces of the given Vertex are inserted.
				@param	vertex	the Vertex to join with
				@return	bool		<b>true</b> if the vertices can be joined,
												<b>false</b> otherwise
		*/
		bool join(const Vertex& vertex)
			throw();

		/** Subsitute the GraphVertex by a similar one.
				The GraphVertex is substituted in all his Edges and Faces.
				@param	vertex	a pointer to the vertex to substitute
				@return	bool		<b>true</b> if the vertex can be substituted,
												<b>false</b> otherwise
		*/
		bool substitute(Vertex* vertex)
			throw();


		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator == (const Vertex&) const
			throw();

		/**	Inequality operator.
				@return	bool	<b>false</b>
		*/
		virtual bool operator != (const Vertex&) const
			throw();

		/**	Similarity operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator *= (const Vertex&) const
			throw();

		/**	Test whether the vertex is meber of a given face.
				@param	face	a pointer to the the face to be tested
				@return	Face*	a pointer to the face if it exists, otherwise <b>NULL</b>
		*/
		Face* has(Face* face) const
			throw();

		/**	Test whether the vertex is meber of a given edge.
				@param	edge	a pointer to the edge to be tested
				@return	Edge*	a pointer to the edge if it exists, otherwise <b>NULL</b>
		*/
		Edge* has(Edge* edge) const
			throw();

		/**	Test whether the vertex has edges.
		*/
		bool hasEdges() const
			throw();

		/**	Test whether the vertex has faces.
		*/
		bool hasFaces() const
			throw();

		//@}
		/**	@name	External Iterators
		*/
		//@{

		EdgeIterator beginEdge()
			throw();
		ConstEdgeIterator beginEdge() const
			throw();
		EdgeIterator endEdge()
			throw();
		ConstEdgeIterator endEdge() const
			throw();
		FaceIterator beginFace()
			throw();
		ConstFaceIterator beginFace() const
			throw();
		FaceIterator endFace()
			throw();
		ConstFaceIterator endFace() const
			throw();

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		/*_ The RSEdges the RSVetex belongs to
		*/
		HashSet<Edge*> edges_;
		/*_ The RSFaces the RSVetex belongs to
		*/
		HashSet<Face*> faces_;
		/*_ The index of the GraphVertex
		*/
		Index index_;

		//@}

	};



	template <typename Vertex, typename Edge, typename Face>
	GraphVertex<Vertex,Edge,Face>::GraphVertex()
		throw()
		: edges_(),
			faces_(),
			index_(-1)
	{
	}


	template <typename Vertex, typename Edge, typename Face>
	GraphVertex<Vertex,Edge,Face>::GraphVertex
			(const GraphVertex<Vertex,Edge,Face>& vertex, bool deep)
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


	template <typename Vertex, typename Edge, typename Face>
	GraphVertex<Vertex,Edge,Face>::~GraphVertex()
		throw()
	{
	}


	template <typename Vertex, typename Edge, typename Face>
	void GraphVertex<Vertex,Edge,Face>::set
			(const GraphVertex<Vertex,Edge,Face>& vertex, bool deep)
		throw()
	{
		if (this != &vertex)
		{
			if (deep)
			{
				edges_ = vertex.edges_;
				faces_ = vertex.faces_;
			}
			index_ = vertex.index_;
		}
	}


	template <typename Vertex, typename Edge, typename Face>
	GraphVertex<Vertex,Edge,Face>& GraphVertex<Vertex,Edge,Face>::operator =
			(const GraphVertex<Vertex,Edge,Face>& vertex)
		throw()
	{
		if (this != &vertex)
		{
			edges_ = vertex.edges_;
			faces_ = vertex.faces_;
			index_ = vertex.index_;
		}
		return *this;
	}


	template <typename Vertex, typename Edge, typename Face>
	void GraphVertex<Vertex,Edge,Face>::insert(Edge* edge)
		throw()
	{
		edges_.insert(edge);
	}


	template <typename Vertex, typename Edge, typename Face>
	void GraphVertex<Vertex,Edge,Face>::insert(Face* face)
		throw()
	{
		faces_.insert(face);
	}


	template <typename Vertex, typename Edge, typename Face>
	void GraphVertex<Vertex,Edge,Face>::remove(Edge* edge)
		throw()
	{
		edges_.erase(edge);
	}


	template <typename Vertex, typename Edge, typename Face>
	void GraphVertex<Vertex,Edge,Face>::remove(Face* face)
		throw()
	{
		faces_.erase(face);
	}


	template <typename Vertex, typename Edge, typename Face>
	Position GraphVertex<Vertex,Edge,Face>::numberOfEdges() const
		throw()
	{
		return edges_.size();
	}


	template <typename Vertex, typename Edge, typename Face>
	Position GraphVertex<Vertex,Edge,Face>::numberOfFaces() const
		throw()
	{
		return faces_.size();
	}


	template <typename Vertex, typename Edge, typename Face>
	void GraphVertex<Vertex,Edge,Face>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename Vertex, typename Edge, typename Face>
	Index GraphVertex<Vertex,Edge,Face>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::substitute(Vertex* vertex)
		throw()
	{
		if (*this *= *vertex)
		{
			typename HashSet<Edge*>::Iterator e;
			for (e = edges_.begin(); e != edges_.end(); e++)
			{
				(*e)->substitute((Vertex*)this,vertex);
			}
			typename HashSet<Face*>::Iterator f;
			for (f = faces_.begin(); f != faces_.end(); f++)
			{
				(*f)->substitute((Vertex*)this,vertex);
			}
			return true;
		}
		return false;
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::join(const Vertex& vertex)
		throw()
	{
		if (*this *= vertex)
		{
			typename HashSet<Edge*>::ConstIterator e;
			for (e = vertex.edges_.begin(); e != vertex.edges_.end(); e++)
			{
				edges_.insert(*e);
			}
			typename HashSet<Face*>::ConstIterator f;
			for (f = vertex.faces_.begin(); f != vertex.faces_.end(); f++)
			{
				faces_.insert(*f);
			}
			return true;
		}
		else
		{
			return false;
		}
	}


	template <typename Vertex, typename Edge, typename Face>
	Face* GraphVertex<Vertex,Edge,Face>::has(Face* face) const
		throw()
	{
		typename HashSet<Face*>::ConstIterator f;
		for (f = faces_.begin(); f != faces_.end(); f++)
		{
			if (*(*f) == *face)
			{
				return *f;
			}
		}
		return NULL;
	}


	template <typename Vertex, typename Edge, typename Face>
	Edge* GraphVertex<Vertex,Edge,Face>::has(Edge* edge) const
		throw()
	{
		typename HashSet<Edge*>::ConstIterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (*(*e) == *edge)
			{
				return *e;
			}
		}
		return NULL;
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::hasEdges() const
		throw()
	{
		return !edges_.isEmpty();
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::hasFaces() const
		throw()
	{
		return !faces_.isEmpty();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::EdgeIterator
			GraphVertex<Vertex,Edge,Face>::beginEdge()
		throw()
	{
		return edges_.begin();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::ConstEdgeIterator
			GraphVertex<Vertex,Edge,Face>::beginEdge() const
		throw()
	{
		return edges_.begin();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::EdgeIterator
			GraphVertex<Vertex,Edge,Face>::endEdge()
		throw()
	{
		return edges_.end();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::ConstEdgeIterator
			GraphVertex<Vertex,Edge,Face>::endEdge() const
		throw()
	{
		return edges_.end();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::FaceIterator
			GraphVertex<Vertex,Edge,Face>::beginFace()
		throw()
	{
		return faces_.begin();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::ConstFaceIterator
			GraphVertex<Vertex,Edge,Face>::beginFace() const
		throw()
	{
		return faces_.begin();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::FaceIterator
			GraphVertex<Vertex,Edge,Face>::endFace()
		throw()
	{
		return faces_.end();
	}


	template <typename Vertex, typename Edge, typename Face>
	typename GraphVertex<Vertex,Edge,Face>::ConstFaceIterator
			GraphVertex<Vertex,Edge,Face>::endFace() const
		throw()
	{
		return faces_.end();
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::operator == (const Vertex&) const
		throw()
	{
		return true;
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::operator != (const Vertex&) const
		throw()
	{
		return false;
	}


	template <typename Vertex, typename Edge, typename Face>
	bool GraphVertex<Vertex,Edge,Face>::operator *= (const Vertex&) const
		throw()
	{
		return true;
	}



} // namespace BALL

#endif // BALL_STRUCTURE_GRAPHVERTEX_H
