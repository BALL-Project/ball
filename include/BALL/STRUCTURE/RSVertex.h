// $Id: RSVertex.h,v 1.14 2001/07/15 17:03:24 oliver Exp $

#ifndef BALL_STRUCTURE_RSVERTEX_H
#define BALL_STRUCTURE_RSVERTEX_H

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif


namespace BALL
{
	
	template <typename T>
	class TRSFace;

	template <typename T>
	class TRSEdge;

	/** Generic RSVertex Class.	
			{\bf Definition:} \URL{BALL/STRUCTURE/RSVertex.h}	
	*/
	template <class T>
	class TRSVertex
	{
		public:

		BALL_CREATE(TRSVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new RSVertex object.
		*/
		TRSVertex()
			throw();

		/**	Copy constructor.
				Create a new RSVertex object from another.
				@param	rsvertex	the RSVertex object to be copied
				@param	bool			ignored - just for interface consistency
		*/
		TRSVertex(const TRSVertex<T>& rsvertex, bool = true)
			throw();

		/**	Detailled constructor.
				Create a new RSVertex object from an index.
				@param	atom	assigned to the index of the atom
		*/
		TRSVertex(Index a)
			throw();

		/**	Detailled constructor.
				Create a new RSVertex object from a Index and two HashSets and another Index.
				@param	atom	assigned to the index of the atom
				@param	edges	assigned to HashSet of RSEdges
				@param	faces	assigned to HashSet of RSFaces
				@param	i			assigned to the index of the RSVertex
		*/
		TRSVertex(Index atom,
				const HashSet<TRSEdge<T>*>& edges,
				const HashSet<TRSFace<T>*>& faces,
				Index i)
			throw();

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~TRSVertex()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another RSVertex.
				@param rsvertex	the RSVertex object to assign from
				@param deep ignored
		*/
		void set(const TRSVertex<T>& rsvertex, bool = true)
			throw();

		/**	Assign to a Index, two HashSets amd another Index.
				@param	atom	assigned to the index of the atom
				@param	edges	assigned to list of edges
				@param	faces	assigned to list of faces
				@param	i			asiigned to the index of the RSVertex
		*/
		void set(Index atom,
				const HashSet<TRSEdge<T>*>& edges,
				const HashSet<TRSFace<T>*>& faces,
				Index i)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the index of the atom represented by the vertex.
				@param	atom	the new index
		*/
		void setAtom(Index atom)
			throw();

		/** Return the index of the atom represented by the vertex.
				@return	Index	the index of the atom represented by the vertex
		*/
		Index getAtom() const
			throw();

		/** Set the HashSet of RSEdges the RSVertex belongs to.
				@param	edges	the new RSEdge HashSet
		*/
		void setEdges(HashSet<TRSEdge<T>*> edges)
			throw();

		/** Insert a new edge into the HashSet of RSEdges the RSVertex belongs to.
				@param	edge	a pointer to the new RSEdge
		*/
		void pushEdge(TRSEdge<T>* edge)
			throw();

		/** Delete a edge from the HashSet of RSEdges the RSVertex belongs to.
				@param	edge	a pointer to the RSEdge to delete
		*/
		void deleteEdge(TRSEdge<T>* edge)
			throw();

		/** Return the HashSet of RSEdges the RSVertex belongs to.
				@return	HashSet<TRSEdge<T>*>	the HashSet of RSEdges the RSVertex	
								belongs to
		*/
		HashSet<TRSEdge<T>*> getEdges() const
			throw();

		/** Set the HashSet of RSFaces the RSVertex belongs to.
				@param	faces	the new RSFace HashSet
		*/
		void setFaces(HashSet<TRSFace<T>*> faces)
			throw();

		/** Insert a new face into the HashSet of RSFaces the RSVertex belongs to.
				@param	face	a pointer to the new RSFace
		*/
		void pushFace(TRSFace<T>* face)
			throw();

		/** Delete a face from the HashSet of RSFaces the RSVertex belongs to.
				@param	edge	a pointer to the RSFace to delete
		*/
		void deleteFace(TRSFace<T>* face)
			throw();

		/** Return the HashSet of RFSaces the RSVertex belongs to.
				@return	HashSet<TRSFace<T>*>	the HashSet of RSFaces the RSVertex	
								belongs to
		*/
		HashSet<TRSFace<T>*> getFaces() const
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

		/** Join two vertices if they are similar.
				All edges and faces of the given RSVertex are inserted.
				@param	rsvertex	the RSVertex to join with
				@return	bool			{\bf true} if the vertices can be joined,	
													{\bf false} otherwise
		*/
		bool join(const TRSVertex<T>& rsvertex)
			throw();


		/** Subsitute the vertex by a similar one.
				The RSVertex is substituted in all his RSEdges and RSFaces.
				@param	rsvertex	a pointer to the rsvertex to substitute
				@return	bool			{\bf true} if the vertex can be substituted,	
													{\bf false} otherwise
		*/
		bool substitute(TRSVertex<T>* rsvertex)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true} if the vertices are equal in all	
											components, {\bf false} otherwise
		*/
		bool operator == (const TRSVertex<T>& rsvertex) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false} if the vertices are equal in all	
											components, {\bf true} otherwise
		*/
		bool operator != (const TRSVertex<T>& rsvertex) const
			throw();

		/**	Similar
				@return	bool	{\bf true} if the atoms are equal,	
											{\bf false} otherwise
		*/
		bool similar(const TRSVertex<T>& rsvertex) const
			throw();

		/**	Test if the vertex is meber of a given RSFace.
				@param	face				a pointer to the the face to be tested
				@return	TRSFace<T>*	a pointer to the face if it exists,	
														otherwise {\bf NULL}
		*/
		TRSFace<T>* has(TRSFace<T>* face) const
			throw();

		/**	Test if the vertex is meber of a given RSEdge.
				@param	edge	a pointer to the edge to be tested
				@return	bool	{\bf true} if it exists, otherwise {\bf false}
		*/
		bool has(TRSEdge<T>* edge) const
			throw();
		

		//@}

		protected:

		/*_ The index of the atom represented by the RSVertex
		*/
		Index atom_;
		/*_ The RSEdges the RSVetex belongs to
		*/
		HashSet<TRSEdge<T>*> edges_;
		/*_ The RSFaces the RSVetex belongs to
		*/
		HashSet<TRSFace<T>*> faces_;
		/*_ The index of the RSVertex
		*/
		Index index_;

	};

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
			reads in a rsvertex
			@exception NotImplemented
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TRSVertex<T>& rsvertex)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	/**	Output- Operator
			prints out a rsvertex
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TRSVertex<T>& rsvertex)
		{
			s << "RSVERTEX" << rsvertex.getIndex() << "(" << rsvertex.getAtom() << " [";
			HashSet< TRSEdge<T>* > edges = rsvertex.getEdges();
			for (typename HashSet<TRSEdge<T>*>::Iterator i = edges.begin(); i != edges.end(); i++)
			{
				s << (*i)->getIndex() << ' ';
			}
			s << "] [";
			HashSet< TRSFace<T>* > faces = rsvertex.getFaces();
			for (typename HashSet<TRSFace<T>*>::Iterator i = faces.begin(); i != faces.end(); i++)
			{
				s << (*i)->getIndex() << ' ';
			}
			s << "])";
			return s;
		}

	//@}

	/**	The Default RSVertex Type.
			If double precision is not needed, {\tt RSVertex<float>} should
			be used. It is predefined as {\tt RSVertex} for convenience.
	*/
	typedef TRSVertex<float> RSVertex;



	template <typename T>
	TRSVertex<T>::TRSVertex()
		throw()
		: atom_(-1),
			edges_(),
			faces_(),
			index_(-1)
	{
	}


	template <typename T>
	TRSVertex<T>::TRSVertex(const TRSVertex<T>& rsvertex, bool)
		throw()
		: atom_(rsvertex.atom_),
			edges_(rsvertex.edges_),
			faces_(rsvertex.faces_),
			index_(rsvertex.index_)
	{
	}


	template <typename T>
	TRSVertex<T>::TRSVertex(Index a)
		throw()
		: atom_(a),
			edges_(),
			faces_(),
			index_(-1)
	{
	}


	template <typename T>
	TRSVertex<T>::TRSVertex(Index atom,
			const HashSet<TRSEdge<T>*>& edges,
			const HashSet<TRSFace<T>*>& faces,
			Index i)
		throw()
		: atom_(a),
			edges_(edges),
			faces_(faces),
			index_(i)
	{
	}


	template <typename T>
	TRSVertex<T>::~TRSVertex()
		throw()
	{
	}


	template <typename T>
	void TRSVertex<T>::set(const TRSVertex<T>& rsvertex, bool)
		throw()
	{
		atom_ = rsvertex.atom_;
		edges_ = rsvertex.edges_;
		faces_ = rsvertex.faces_;
		index_ = rsvertex.index_;
	}


	template <typename T>
	void TRSVertex<T>::set(Index atom,
			const HashSet<TRSEdge<T>*>& edges,
			const HashSet<TRSFace<T>*>& faces,
			Index i)
		throw()
	{
		atom_ = atom;
		edges_ = edges;
		faces_ = faces;
		index_ = i;
	}


	template <typename T>
	void TRSVertex<T>::setAtom(Index atom)
		throw()
	{
		atom_ = atom;
	}


	template <typename T>
	Index TRSVertex<T>::getAtom() const
		throw()
	{
		return atom_;
	}


	template <typename T>
	void TRSVertex<T>::setEdges(HashSet<TRSEdge<T>*> edges)
		throw()
	{
		edges_ = edges;
	}


	template <typename T>
	void TRSVertex<T>::pushEdge(TRSEdge<T>* edge)
		throw()
	{
		edges_.insert(edge);
	}


	template <typename T>
	void TRSVertex<T>::deleteEdge(TRSEdge<T>* edge)
		throw()
	{
		edges_.erase(edge);
	}


	template <typename T>
	HashSet<TRSEdge<T>*> TRSVertex<T>::getEdges() const
		throw()
	{
		return edges_;
	}


	template <typename T>
	void TRSVertex<T>::setFaces(HashSet<TRSFace<T>*> faces)
		throw()
	{
		faces_ = faces;
	}


	template <typename T>
	void TRSVertex<T>::pushFace(TRSFace<T>* face)
		throw()
	{
		faces_.insert(face);
	}


	template <typename T>
	void TRSVertex<T>::deleteFace(TRSFace<T>* face)
		throw()
	{
		faces_.erase(face);
	}


	template <typename T>
	HashSet<TRSFace<T>*> TRSVertex<T>::getFaces() const
		throw()
	{
		return faces_;
	}


	template <typename T>
	void TRSVertex<T>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename T>
	Index TRSVertex<T>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename T>
	bool TRSVertex<T>::join(const TRSVertex<T>& rsvertex)
		throw()
	{
		if (atom_ != rsvertex.atom_)
		{
			return false;
		}
		typename HashSet<TRSEdge<T>*>::ConstIterator e;
		for (e = rsvertex.edges_.begin(); e != rsvertex.edges_.end(); e++)
		{
			edges_.insert(*e);
		}
		typename HashSet<TRSFace<T>*>::ConstIterator f;
		for (f = rsvertex.faces_.begin(); f != rsvertex.faces_.end(); f++)
		{
			faces_.insert(*f);
		}
		return true;
	}


	template <typename T>
	bool TRSVertex<T>::substitute(TRSVertex<T>* rsvertex)
		throw()
	{
		if (atom_ != rsvertex->atom_)
		{
			return false;
		}
		typename HashSet<TRSEdge<T>*>::Iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			(*e)->substituteVertex(this,rsvertex);
		}
		typename HashSet<TRSFace<T>*>::Iterator f;
		for (f = faces_.begin(); f != faces_.end(); f++)
		{
			(*f)->substituteVertex(this,rsvertex);
		}
		return true;
	}


	template <typename T>
	bool TRSVertex<T>::operator == (const TRSVertex<T>& rsvertex) const
		throw()
	{
		if (atom_ != rsvertex.atom_)
		{
			return false;
		}
		typename HashSet<TRSEdge<T>*>::ConstIterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (rsvertex.edges_.has(*e) == false)
			{
				return false;
			}
		}
		for (e = rsvertex.edges_.begin(); e != rsvertex.edges_.end(); e++)
		{
			if (edges_.has(*e) == false)
			{
				return false;
			}
		}
		typename HashSet<TRSFace<T>*>::ConstIterator f;
		for (f = faces_.begin(); f != faces_.end(); f++)
		{
			if (rsvertex.faces_.has(*f) == false)
			{
				return false;
			}
		}
		for (f = rsvertex.faces_.begin(); f != rsvertex.faces_.end(); f++)
		{
			if (faces_.has(*f) == false)
			{
				return false;
			}
		}
		return true;
	}


	template <typename T>
	bool TRSVertex<T>::operator != (const TRSVertex<T>& rsvertex) const
		throw()
	{
		return ( ! (*this == rsvertex) );
	}


	template <typename T>
	bool TRSVertex<T>::similar(const TRSVertex<T>& rsvertex) const
		throw()
	{
		return (atom_ == rsvertex.atom_);
	}

	
	template <typename T>
	TRSFace<T>* TRSVertex<T>::has(TRSFace<T>* face) const
		throw()
	{
		typename HashSet<TRSFace<T>*>::ConstIterator f;
		for (f = faces_.begin(); f != faces_.end(); f++)
		{
			if (*(*f) == *face)
			{
				return *f;
			}
		}
		return NULL;
	}


	template <typename T>
	bool TRSVertex<T>::has(TRSEdge<T>* edge) const
		throw()
	{
		typename HashSet<TRSEdge<T>*>::ConstIterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (*(*e) == *edge)
			{
				return true;
			}
		}
		return false;
	}





} // namespace BALL

#endif // BALL_STRUCTURE_RSVERTEX_H
