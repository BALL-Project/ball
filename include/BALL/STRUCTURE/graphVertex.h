// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#define BALL_STRUCTURE_GRAPHVERTEX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <BALL/DATATYPE/hashSet.h>

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
				Create a new GraphVertex object from two HashSets and an Index.
				@param	edges	assigned to HashSet of edges
				@param	faces	assigned to HashSet of faces
				@param	i			assigned to the index of the GraphVertex
		*/
		GraphVertex
				(const HashSet<Edge*>& edges,
				 const HashSet<Face*>& faces,
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
		void set(const GraphVertex<Edge,Face>& vertex, bool deep = false)
			throw();

		/**	Assign to two HashSets and an Index.
				@param	edges	assigned to HashSet of edges
				@param	faces	assigned to HashSet of faces
				@param	i			asiigned to the index of the GraphVertex
		*/
		void set
				(const HashSet<Edge*>& edges,
				 const HashSet<Face*>& faces,
				 Index i)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the HashSet of edges the GraphVertex belongs to.
				@param	edges	the new HashSet of edges
		*/
		void setEdges(HashSet<Edge*> edges)
			throw();

		/** Insert a new edge into the HashSet of edges the GraphVertex belongs to.
				@param	edge	a pointer to the new edge
		*/
		void pushEdge(Edge* edge)
			throw();

		/** Delete a edge from the HashSet of edges the GraphVertex belongs to.
				@param	edge	a pointer to the edge to delete
		*/
		void deleteEdge(Edge* edge)
			throw();

		/** Return the HashSet of edges the GraphVertex belongs to.
				@return	HashSet<Edge*>	the HashSet of edges the GraphVertex belongs to
		*/
		HashSet<Edge*> getEdges() const
			throw();

		/** Return the number of edges the GraphVertex belongs to.
				@return	Position	the number of edges the GraphVertex belongs to
		*/
		Position numberOfEdges() const
			throw();

		/** Set the HashSet of faces the GraphVertex belongs to.
				@param	faces	the new HashSet of faces
		*/
		void setFaces(HashSet<Face*> faces)
			throw();

		/** Insert a new face into the HashSet of faces the GraphVertex belongs to.
				@param	face	a pointer to the new face
		*/
		void pushFace(Face* face)
			throw();

		/** Delete a face from the HashSet of faces the GraphVertex belongs to.
				@param	edge	a pointer to the face to delete
		*/
		void deleteFace(Face* face)
			throw();

		/** Return the HashSet of faces the GraphVertex belongs to.
				@return	HashSet<Face*>	the HashSet of faces the GraphVertex belongs to
		*/
		HashSet<Face*> getFaces() const
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

		//virtual bool join(const GraphVertex<Edge,Face>& vertex)
		//	throw();


		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true} if the vertices are equal in all	
											components, {\bf false} otherwise
		*/
		bool operator == (const GraphVertex<Edge,Face>&) const
			throw();// = 0;

		/**	Inequality operator.
				@return	bool	{\bf false} if the vertices are equal in all	
											components, {\bf true} otherwise
		*/
		bool operator != (const GraphVertex<Edge,Face>&) const
			throw();// = 0;

		virtual bool operator *= (const GraphVertex<Edge,Face>&) const
			throw();

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

		//protected:

		/*_ The RSEdges the RSVetex belongs to
		*/
		HashSet<Edge*> edges_;
		/*_ The RSFaces the RSVetex belongs to
		*/
		HashSet<Face*> faces_;
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
			(const HashSet<Edge*>& edges,
			 const HashSet<Face*>& faces,
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
			(const HashSet<Edge*>& edges,
			 const HashSet<Face*>& faces,
			 Index i)
		throw()
	{
		edges_ = edges;
		faces_ = faces;
		index_ = i;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::setEdges(HashSet<Edge*> edges)
		throw()
	{
		edges_ = edges;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::pushEdge(Edge* edge)
		throw()
	{
		edges_.insert(edge);
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::deleteEdge(Edge* edge)
		throw()
	{
		edges_.erase(edge);
	}


	template <typename Edge, typename Face>
	HashSet<Edge*> GraphVertex<Edge,Face>::getEdges() const
		throw()
	{
		return edges_;
	}


	template <typename Edge, typename Face>
	Position GraphVertex<Edge,Face>::numberOfEdges() const
		throw()
	{
		return edges_.size();
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::setFaces(HashSet<Face*> faces)
		throw()
	{
		faces_ = faces;
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::pushFace(Face* face)
		throw()
	{
		faces_.insert(face);
	}


	template <typename Edge, typename Face>
	void GraphVertex<Edge,Face>::deleteFace(Face* face)
		throw()
	{
		faces_.erase(face);
	}


	template <typename Edge, typename Face>
	HashSet<Face*> GraphVertex<Edge,Face>::getFaces() const
		throw()
	{
		return faces_;
	}


	template <typename Edge, typename Face>
	Position GraphVertex<Edge,Face>::numberOfFaces() const
		throw()
	{
		return faces_.size();
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


	/*template <typename Edge, typename Face>
	bool GraphVertex<Edge,Face>::join(const GraphVertex<Edge,Face>& vertex)
		throw()
	{
cout << "join. rufe *= von ";
		if (*this *= vertex)
		{
cout << " auf\n";
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
cout << " auf\n";
			return false;
		}
	}*/


	template <typename Edge, typename Face>
	Face* GraphVertex<Edge,Face>::has(Face* face) const
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


	template <typename Edge, typename Face>
	Edge* GraphVertex<Edge,Face>::has(Edge* edge) const
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


	template <typename Edge, typename Face>
	bool GraphVertex<Edge,Face>::operator *= (const GraphVertex<Edge,Face>&) const
		throw()
	{
//cout << "GraphVertex";
		return false;
	}





} // namespace BALL

#endif // BALL_STRUCTURE_RSVERTEX_H
