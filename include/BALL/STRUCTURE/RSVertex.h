// $Id: RSVertex.h,v 1.1 2000/10/10 14:24:55 oliver Exp $

#ifndef BALL_STRUCTURE_RSVERTEX_H
#define BALL_STRUCTURE_RSVERTEX_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#include <list>

namespace BALL
{
	

	template <typename T>
	class TRSFace;

	template <typename T>
	class TRSEdge;

	/** Generic RSVertex Class.
			\\
			{\bf Definition:} \URL{BALL/STRUCTURE/RSVertex.h}
			\\
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
			: atom_(-1), edges_(), faces_()//, index_(-1)
		{
		}

		/**	Copy constructor.
				Create a new RSVertex object from another.
				@param rsface the RSVertex object to be copied
				@param bool ignored - just for interface consistency
		*/
		TRSVertex(const TRSVertex<T>& rsvertex,bool /* deep */ = true)
			: atom_(rsvertex.atom_),
				edges_(rsvertex.edges_),
				faces_(rsvertex.faces_)
		{
		}

		/**	Detailled constructor.
				Create a new RSVertex object from a sphere.
				@param	sphere assigned to the atom
		*/
		TRSVertex(const Index a)
			: atom_(a), edges_(), faces_()//, index_(-1)
		{
		}

		/**	Detailled constructor.
				Create a new RSVertex object from a Index and two lists.
				@param	a assigned to the index of the atom
				@param	edge_list assigned to list of rsedges
				@param	face_list assigned to list of rsfaces
				@param	i assigned to the index of the rsvertex
		*/
		TRSVertex(const Index a, const list< TRSEdge<T>* >& edge_list,
							const list< TRSFace<T>* >& face_list)
			: atom_(a), edges_(edge_list), faces_(face_list)
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
//			index_ = rsvertex.getIndex();
		}

		/**	Assign to a sphere and two lists.
				@param	a assigned to the index of the atom
				@param	edge_list assigned to list of edges
				@param	face_list assigned to list of faces
		*/
		void set(const Index a, const list< TRSEdge<T>* >& edge_list,
						 const list< TRSFace<T>* >& face_list)//, const Index i)
		{
			atom_ = a; edges_ = edge_list; faces_ = face_list;// index_ = i;
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Change the index of the atom represented by the vertex.
				@param atom the new index
		*/
		void setAtom(Index atom)
		{
			atom_ = atom;
		}

		/** Return the index of the atom represented by the vertex.
				@return Index, the index of the atom represented by the vertex
		*/
		Index getAtom()
		{
			return atom_;
		}

		/** Change the list of rsedges the rsvertex belongs to.
				@param edges the new rsedge list
		*/
		void setEdges(list< TRSEdge<T>* > edges)
		{
			edges_ = edges;
		}

		/** Push a new edge into the list of rsedges the rsvertex belongs to.
				@param edge a pointer to the new rsedge
		*/
		void pushEdge(TRSEdge<T>* edge)
		{
			edges_.push_back(edge);
		}

		/** Return the list of rsedges the rsvertex belongs to.
				@return list< TRSEdge<T>* >, the list of rsedges the rsvertex belongs to
		*/
		list< TRSEdge<T>* > getEdges()
		{
			return edges_;
		}

		/** Change the list of rsfaces the rsvertex belongs to.
				@param faces the new rsface list
		*/
		void setFaces(list< TRSFace<T>* > faces)
		{
			faces_ = faces;
		}

		/** Push a new face into the list of rsfaces the rsvertex belongs to.
				@param face a pointer to the new rsface
		*/
		void pushFace(TRSFace<T>* face)
		{
			faces_.push_back(face);
		}

		/** Return the list of rsfaces the rsvertex belongs to.
				@return list< TRSFace<T>* >, the list of rsfaces the rsvertex belongs to
		*/
		list< TRSFace<T>* > getFaces()
		{
			return faces_;
		}

//		/** Change the index of the rsvertex.
//				@param index the new index
//		*/
//		void setIndex(Index index)
//		{
//			index_ = index;
//		}

//		/** Return the index of the rsvertex.
//				@return Index, the index of the rsvertex
//		*/
//		Index getIndex()
//		{
//			return index_;
//		}

		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if the atoms are equal, {\bf false} otherwise
		*/
		bool operator == (const TRSVertex& rsvertex) const
		{
			return (bool)(atom_ == rsvertex.getAtom());
		}

		/**	Inequality operator.
				@return bool, {\bf true} if the atoms differ, {\bf false} otherwise
		*/
		bool operator != (const TRSVertex& rsvertex) const
		{
			return (bool)(atom_ != rsvertex.getAtom());
		}

		/**	Test if the vertex is meber of a given rsface.
				@param	f	a pointer to the the face to be tested
				@return TRSFace<T>, a pointer to the face if it exists, otherwise {\bf NULL}
		*/
		TRSFace<T>* has(TRSFace<T>* f)
		{
			list<TRSFace<T>*>::iterator i;
			for (i = faces_.begin(); i != faces_.end(); i++)
			{
				if (*(*i) == *f)
				{
					return *i;
				}
			}
			return NULL;
		}

		/**	Test if the vertex is meber of a given rsedge.
				@param	e	a pointer to the edge to be tested
				@return bool,	{\bf true} if it exists, otherwise {\bf false}
		*/
		bool has(TRSEdge<T>* e)
		{
			list<TRSEdge<T>*>::iterator i;
			for (i = edges_.begin(); i != edges_.end(); i++)
				{
					if (*(*i) == *e)
						{
							return true;
						}
				}
			return false;
		}


		//@}

		protected:
		/**	@name	Attributes
		*/
		//@{

		/**	atom.
				The index of the atom represented by the vertex.
		*/
		Index atom_;

		/**	edges.
				A list of edges the vertex belongs to.
		*/
		list< TRSEdge<T>* > edges_;

		/**	faces.
				A list of faces the vertex belongs to.
		*/
		list< TRSFace<T>* > faces_;

		/**	index.
		*/
//		Index index_;

		//@}
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
			prints out six pointers and two TVector3s : vertex1, vertex2, vertex3, edge1, edge3, edge3,
			center, normasl
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TRSVertex<T>& rsvertex)
		{
			s << "RSVERTEX" << /*rsvertex.getIndex() <<*/ "(" << rsvertex.getAtom() << " [";
			list< TRSEdge<T>* > edges = rsvertex.getEdges();
			for (list<TRSEdge<T>*>::iterator i = edges.begin(); i != edges.end(); i++)
			{
				s << (*i)->getIndex() << ' ';
			}
			s << "] [";
			list< TRSFace<T>* > faces = rsvertex.getFaces();
			for (list<TRSFace<T>*>::iterator i = faces.begin(); i != faces.end(); i++)
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
