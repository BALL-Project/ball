// $Id: RSVertex.h,v 1.6 2001/04/03 13:13:02 strobel Exp $

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

	/** Generic RSVertex Class.	\\
			{\bf Definition:} \URL{BALL/STRUCTURE/RSVertex.h}	\\
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
				This method creates a new RSVertex object. All components are
				initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TRSVertex()
			: atom_(-1), edges_(), faces_(), index_(-1)
		{
		}

		/**	Copy constructor.
				Create a new RSVertex object from another.
				@param rsface the RSVertex object to be copied
				@param bool ignored - just for interface consistency
		*/
		TRSVertex(const TRSVertex<T>& rsvertex, bool /* deep */ = true)
			: atom_(rsvertex.atom_),
				edges_(rsvertex.edges_),
				faces_(rsvertex.faces_),
				index_(rsvertex.index_)
		{
		}

		/**	Detailled constructor.
				Create a new RSVertex object from a sphere.
				@param	sphere assigned to the atom
		*/
		TRSVertex(const Index a)
			: atom_(a), edges_(), faces_(), index_(-1)
		{
		}

		/**	Detailled constructor.
				Create a new RSVertex object from a Index and two HashSets and another Index.
				@param	a assigned to the index of the atom
				@param	edge_list assigned to HashSet of rsedges
				@param	face_list assigned to HashSet of rsfaces
				@param	i assigned to the index of the rsvertex
		*/
		TRSVertex(Index a, const HashSet< TRSEdge<T>* >& edge_list,
							const HashSet< TRSFace<T>* >& face_list, Index i)
			: atom_(a), edges_(edge_list), faces_(face_list), index_(i)
		{
		}

		/**	Destructor.
				Destructs the RSVertex object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TRSVertex()
		{
		}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another RSVertex.
				@param rsvertex	the RSVertex object to assign from
				@param deep ignored
		*/
		void set(const TRSVertex& rsvertex, bool /* deep */ = true)
		{
			atom_ = rsvertex.getAtom();
			edges_ = rsvertex.getEdges();
			faces_ = rsvertex.getFaces();
			index_ = rsvertex.getIndex();
		}

		/**	Assign to a Index, two HashSets amd another Index.
				@param	a assigned to the index of the atom
				@param	edge_list assigned to list of edges
				@param	face_list assigned to list of faces
				@param	i	asiigned to the index of the rsvertex
		*/
		void set(Index a, const HashSet< TRSEdge<T>* >& edge_list,
						 const HashSet< TRSFace<T>* >& face_list, Index i)
		{
			atom_ = a; edges_ = edge_list; faces_ = face_list; index_ = i;
		}

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Change the index of the atom represented by the vertex.
				@param	atom	the new index
		*/
		void setAtom(Index atom)
		{
			atom_ = atom;
		}

		/** Return the index of the atom represented by the vertex.
				@return		Index,	the index of the atom represented by the vertex
		*/
		Index getAtom()
		{
			return atom_;
		}

		/** Change the HashSet of rsedges the rsvertex belongs to.
				@param edges the new rsedge HashSet
		*/
		void setEdges(HashSet< TRSEdge<T>* > edges)
		{
			edges_ = edges;
		}

		/** Insert a new edge into the HashSet of rsedges the rsvertex belongs to.
				@param edge a pointer to the new rsedge
		*/
		void pushEdge(TRSEdge<T>* edge)
		{
			edges_.insert(edge);
		}

		/** Delete a edge from the HashSet of rsedges the rsvertex belongs to.
				@param edge a pointer to the rsedge to delete
		*/
		void deleteEdge(TRSEdge<T>* edge)
		{
			edges_.erase(edge);
		}

		/** Return the HashSet of rsedges the rsvertex belongs to.
				@return HashSet< TRSEdge<T>* >, the HashSet of rsedges the rsvertex belongs to
		*/
		HashSet< TRSEdge<T>* > getEdges() const
		{
			return edges_;
		}

		/** Change the HashSet of rsfaces the rsvertex belongs to.
				@param faces the new rsface HashSet
		*/
		void setFaces(HashSet< TRSFace<T>* > faces)
		{
			faces_ = faces;
		}

		/** Insert a new face into the HashSet of rsfaces the rsvertex belongs to.
				@param face a pointer to the new rsface
		*/
		void pushFace(TRSFace<T>* face)
		{
			faces_.insert(face);
		}

		/** Return the HashSet of rsfaces the rsvertex belongs to.
				@return HashSet< TRSFace<T>* >, the HashSet of rsfaces the rsvertex belongs to
		*/
		HashSet< TRSFace<T>* > getFaces() const
		{
			return faces_;
		}

		/** Change the index of the vertex.
				@param index the new index
		*/
		void setIndex(Index index)
		{
			index_ = index;
		}

		/** Return the index of the vertex.
				@return Index, the index of the vertex
		*/
		Index getIndex()
		{
			return index_;
		}

		/** Join two vertices if they are similar.
		*/
		bool join(const TRSVertex<T>& rsvertex)
		{
			if (atom_ != rsvertex.atom_)
			{
				return false;
			}
			for (HashSet<TRSEdge<T>*>::Iterator e = rsvertex.edges_.begin();
					 e != rsvertex.edges_.end(); e++)
			{
				edges_.insert(*e);
			}
			for (HashSet<TRSFace<T>*>::Iterator f = rsvertex.faces_.begin();
					 f != rsvertex.faces_.end(); f++)
			{
				faces_.insert(*f);
			}
			return true;
		}


		/** Subsitute the vertex by an other.
		*/
		bool substitute(TRSVertex<T>* rsvertex)
		{
			if (atom_ != rsvertex->atom_)
			{
				return false;
			}
			for (HashSet<TRSEdge<T>*>::Iterator e = edges_.begin();
					 e != edges_.end(); e++)
			{
				(*e)->substituteVertex(this,rsvertex);
			}
			for (HashSet<TRSFace<T>*>::Iterator f = faces_.begin();
					 f != faces_.end(); f++)
			{
				(*f)->substituteVertex(this,rsvertex);
			}
			return true;
		}

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool,	{\bf true} if the vertices are equal in all components,
											{\bf false} otherwise
		*/
		bool operator == (const TRSVertex& rsvertex) const
		{
			if (atom_ != rsvertex.getAtom())
			{
				return false;
			}
			for (HashSet<TRSEdge<T>*>::Iterator e = edges_.begin();
					 e != edges_.end(); e++)
			{
				if (rsvertex.edges_.has(*e) == false)
				{
					return false;
				}
			}
			for (HashSet<TRSEdge<T>*>::Iterator e = rsvertex.edges_.begin();
					 e != rsvertex.edges_.end(); e++)
			{
				if (edges_.has(*e) == false)
				{
					return false;
				}
			}
			for (HashSet<TRSFace<T>*>::Iterator f = faces_.begin();
					 f != faces_.end(); f++)
			{
				if (rsvertex.faces_.has(*f) == false)
				{
					return false;
				}
			}
			for (HashSet<TRSFace<T>*>::Iterator f = rsvertex.faces_.begin();
					 f != rsvertex.faces_.end(); f++)
			{
				if (faces_.has(*f) == false)
				{
					return false;
				}
			}
			return true;
		}

		/**	Inequality operator.
				@return	bool,	{\bf true} if the vertices differ in one component,
											{\bf false} otherwise
		*/
		bool operator != (TRSVertex& rsvertex) const
		{
			return ( ! (*this == rsvertex) );
		}

		/**	Similar
				@return	bool,	{\bf true} if the atoms are equal,
											{\bf false} otherwise
		*/
		bool similar(/*const*/ TRSVertex<T>& rsvertex)
		{
			return (atom_ == rsvertex.atom_);
		}

		/**	Test if the vertex is meber of a given rsface.
				@param	face					a pointer to the the face to be tested
				@return	TRSFace<T>*,	a pointer to the face if it exists,
															otherwise {\bf NULL}
		*/
		TRSFace<T>* has(TRSFace<T>* face)
		{
			HashSet<TRSFace<T>*>::Iterator f;
			for (f = faces_.begin(); f != faces_.end(); f++)
			{
				if (*(*f) == *face)
				{
					return *f;
				}
			}
			return NULL;
		}

		/**	Test if the vertex is meber of a given rsedge.
				@param	edge	a pointer to the edge to be tested
				@return	bool,	{\bf true} if it exists, otherwise {\bf false}
		*/
		bool has(TRSEdge<T>* edge)
		{
			HashSet<TRSEdge<T>*>::Iterator e;
			for (e = edges_.begin(); e != edges_.end(); e++)
			{
				if (*(*e) == *edge)
				{
					return true;
				}
			}
			return false;
		}
		

		//@}

		protected:

		Index atom_;
		HashSet<TRSEdge<T>*> edges_;
		HashSet<TRSFace<T>*> faces_;
		Index index_;

	};

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
			reads in a rsvertex
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
			for (HashSet<TRSEdge<T>*>::Iterator i = edges.begin(); i != edges.end(); i++)
			{
				s << (*i)->getIndex() << ' ';
			}
			s << "] [";
			HashSet< TRSFace<T>* > faces = rsvertex.getFaces();
			for (HashSet<TRSFace<T>*>::Iterator i = faces.begin(); i != faces.end(); i++)
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




} // namespace BALL

#endif // BALL_STRUCTURE_RSVERTEX_H
