// $Id: RSFace.h,v 1.13 2001/06/21 23:22:25 amoll Exp $

#ifndef BALL_STRUCTURE_RSFACE_H
#define BALL_STRUCTURE_RSFACE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{
	
	template <typename T>
	class TRSVertex;

	template <typename T>
	class TRSEdge;

	/** Generic RSFace Class.	
			{\bf Definition:} \URL{BALL/STRUCTURE/RSFace.h} 
	*/
	template <class T>
	class TRSFace
	{
		public:

		BALL_CREATE(TRSFace)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new RSFace object.
		*/
		TRSFace()
			: vertex0_(NULL), vertex1_(NULL), vertex2_(NULL),
				edge0_(NULL), edge1_(NULL), edge2_(NULL),
				center_(), normal_(), singular_(false), index_(-1)
		{
		}

		/**	Copy constructor.
				Create a new RSFace object from another.
				@param rsface the RSFace object to be copied
				@param bool ignored - just for interface consistency
		*/
		TRSFace(const TRSFace<T>& rsface,bool /* deep */ = true)
			: vertex0_(rsface.vertex0_),
				vertex1_(rsface.vertex1_),
				vertex2_(rsface.vertex2_),
				edge0_(rsface.edge0_),
				edge1_(rsface.edge1_),
				edge2_(rsface.edge2_),
				center_(rsface.center_),
				normal_(rsface.normal_),
				singular_(rsface.singular_),
				index_(rsface.index_)
		{
		}

		/**	Detailled constructor.
				Create a new RSFace object from a lot of nice objects
				@param	vertex1		assigned to the first vertex
				@param	vertex2		assigned to the second vertex
				@param	vertex3		assigned to the third vertex
				@param	edge1			assigned to the first edge
				@param	edge2			assigned to the second edge
				@param	edge3			assigned to the third edge
				@param	center		assigned to the center
				@param	normal		assigned to the normal vector
				@param	singular
				@param	index			assigned to the index
		*/
		TRSFace(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, TRSVertex<T>* vertex3,
						TRSEdge<T>* edge1, TRSEdge<T>* edge2, TRSEdge<T>* edge3,
						const TVector3<T>& center, const TVector3<T>& normal,
						bool singular, Index index)
			: vertex0_(vertex1), vertex1_(vertex2), vertex2_(vertex3),
				edge0_(edge1), edge1_(edge2), edge2_(edge3),
				center_(center), normal_(normal), singular_(singular), index_(index)
		{
			normal_.normalize();
		}

		/**	Destructor.
				Destructs the RSFace object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TRSFace()
		{
		}

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another rsface.
				@param	rsface	the rsface object to assign from
				@param	deep ignored
		*/
		void set(const TRSFace<T>& rsface, bool /* deep */ = true)
		{
			vertex0_ = rsface.vertex0_; vertex1_ = rsface.vertex1_; vertex2_ = rsface.vertex2_;
			edge0_ = rsedge.edge0_; edge1_ = rsedge.edge1_; edge2_ = rsedge.edge2_;
			center_ = rsface.Center_; normal_ = rsface.normal_;
			singular_ = rsface.singular_; index_ = rsface.index_;
		}

		/**	Assign to a lot of nice objects
				@param	vertex1		assigned to the first vertex
				@param	vertex2		assigned to the second vertex
				@param	edge1			assigned to the first edge
				@param	edge2			assigned to the second edge
				@param	center		assigned to the center
				@param	normal		assigned to the normal vector
				@param	singular
				@param	index			assigned to the index
		*/
		void set(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, TRSVertex<T>* vertex3,
						 TRSEdge<T>* edge1, TRSEdge<T>* edge2, TRSEdge<T>* edge3,
						 const TVector3<T>& center, const TVector3<T>& normal,
						 bool singular, Index index)
		{
			vertex0_ = vertex1; vertex1_ = vertex2; vertex2_ = vertex3;
			edge0_ = edge1; edge1_ = edge2; edge2_ = edge3;
			center_ = center;	normal_ = normal; singular_ = singular;
			index_ = index;
			normal_.normalize();
		}

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all vertices are equal modulo order and the centers are equal,
											{\bf false} otherwise
		*/
		bool operator == (const TRSFace& rsface) const
		{
			if (normal_ != rsface.normal_)
			{
				return false;
			}
			if ((vertex0_->similar(*rsface.vertex0_) == false) &&
					(vertex0_->similar(*rsface.vertex1_) == false) &&
					(vertex0_->similar(*rsface.vertex2_) == false)    )
			{
				return false;
			}
			if ((vertex1_->similar(*rsface.vertex0_) == false) &&
					(vertex1_->similar(*rsface.vertex1_) == false) &&
					(vertex1_->similar(*rsface.vertex2_) == false)    )
			{
				return false;
			}
			if ((vertex2_->similar(*rsface.vertex0_) == false) &&
					(vertex2_->similar(*rsface.vertex1_) == false) &&
					(vertex2_->similar(*rsface.vertex2_) == false)    )
			{
				return false;
			}
			return true;
		}

		/**	similar
				@return bool, {\bf true} if all vertices are equal modulo order,{\bf false} otherwise
		*/
		bool similar(TRSFace& rsface) const
		{
			if ((vertex0_->similar(*rsface.vertex0_) == false) &&
					(vertex0_->similar(*rsface.vertex1_) == false) &&
					(vertex0_->similar(*rsface.vertex2_) == false)    )
			{
				return false;
			}
			if ((vertex1_->similar(*rsface.vertex0_) == false) &&
					(vertex1_->similar(*rsface.vertex1_) == false) &&
					(vertex1_->similar(*rsface.vertex2_) == false)    )
			{
				return false;
			}
			if ((vertex2_->similar(*rsface.vertex0_) == false) &&
					(vertex2_->similar(*rsface.vertex1_) == false) &&
					(vertex2_->similar(*rsface.vertex2_) == false)    )
			{
				return false;
			}
			return true;
		}

		/**	Inequality operator.
				@return bool, {\bf true} if the two rsface differ in at least on component modulo order,
											{\bf false} otherwise
		*/
		bool operator != (const TRSFace<T>& rsface) const
		{
			return (bool)(!(*this == rsface));
		}
		
		/** isSingular
				@return bool, {\bf true} is the rsface is singular, {\bf false} otherwise
		*/
		bool isSingular()
		{
			return singular_;
		}

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Change one of the three rsvertices of the rsface.
				@param	i				the first vertex is changed if i = 0,
												the second vertex is changed if i = 1,
												the third vertex is changed otherwise
				@param	vertex	a pointer to the new vertex
		*/
		void  setVertex(const Position i, TRSVertex<T>* vertex)
		{
			switch (i)
			{
				case 0  :	vertex0_ = vertex; break;
				case 1  :	vertex1_ = vertex; break;
				default :	vertex2_ = vertex; break;
			}
		}

		/** Return one of the three rsvertices of the rsface.
				@return	RSVertex*	the first rsvertex if i = 0,
													the second rsvertex if i = 1,
													the third rsvertex if otherwse
		*/
		TRSVertex<T>* getVertex(const Position i) const
		{
			switch (i)
			{
				case 0  :	return vertex0_;
				case 1  :	return vertex1_;
				default :	return vertex2_;
			}
		}

		/** Change the three rsedges of the rsface.
				@param	i			the first edge is changed if i = 0,
											the second edge is changed if i = 1,
											the third edge is changed otherwise
				@param	edge	a pointer to the new edge
		*/
		void  setEdge(const Position i, TRSEdge<T>* edge)
		{
			switch (i)
			{
				case 0  :	edge0_ = edge; break;
				case 1  :	edge1_ = edge; break;
				default :	edge2_ = edge; break;
			}
		}

		/** Return of one of the three rsedges of the rsface.
				@return	RSEdge*	the first rsedge if i = 0,
												the second rsedge if i = 1
												the third rsedge if otherwise
		*/
		TRSEdge<T>* getEdge(const Position i) const
		{
			switch (i)
			{
				case 0  :	return edge0_;
				case 1  :	return edge1_;
				default :	return edge2_;
			}
		}

		/** Change the center of the probe sphere defining the rsface.
				@param center the new center
		*/
		void setCenter(const TVector3<T>& center)
		{
			center_ = center;
		}

		/** Return the center of the probe sphere defining the rsface.
				@return TVector3<T> the center of the probe sphere
		*/
		TVector3<T> getCenter()
		{
			return center_;
		}

		/** Change the vector orthogonal to the rsface.
				@param	normal	the new normal
		*/
		void setNormal(const TVector3<T>& normal)
		{
			normal_ = normal;
			normal_.normalize();
		}

		/** Return the vector orthogonal to the rsface.
				@return	TVector3<T>	the vector orthogonal to the rsface.
		*/
		TVector3<T> getNormal() const
		{
			return normal_;
		}

		/** Change singular
		*/
		void setSingular(const bool singular)
		{
			singular_ = singular;
		}

		/** Change the index of the rsface.
				@param index the new index
		*/
		void setIndex(Index index)
		{
			index_ = index;
		}

		/** Return the index of the rsface.
				@return Index, the index of the rsface
		*/
		Index getIndex()
		{
			return index_;
		}

		/** Find the two rsedges of this face that belong to the given rsvertex
				@param	vertex	a pointer to the given vertex
				@param	edge1		a pointer to the first found edge
				@param	edge2		a pointer to the second found edge
				@return	bool		{\bf true} if the edges can be found,
												{\bf false} otherwise
		*/
		bool getEdges(TRSVertex<T>* vertex, TRSEdge<T>*& edge1, TRSEdge<T>*& edge2)
		{
			if ((vertex0_ != vertex) && (vertex1_ != vertex) && (vertex2_ != vertex))
			{
				return false;
			}
			if ((edge0_ == NULL) || (edge1_ == NULL) || (edge2_ == NULL))
			{
				return false;
			}
			if ((edge0_->getVertex(0) != vertex) && (edge0_->getVertex(1) != vertex))
			{
				edge1 = edge1_;
				edge2 = edge2_;
			}
			else
			{
				edge1 = edge0_;
				if ((edge1_->getVertex(0) == vertex) || (edge1_->getVertex(1) == vertex))
				{
					edge2 = edge1_;
				}
				else
				{
					edge2 = edge2_;
				}
			}
			return true;
		}
		
		/** Find the rsedge of this face that belong to the two given rsvertices
				@param	vertex1 a pointer to the first given vertex
				@param	vertex2 a pointer to the second given vertex
				@param	edge		a pointer to the first found edge
				@return	bool		{\bf true} if the edge can be found,
												{\bf false} otherwise
		*/
		bool getEdge(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, TRSEdge<T>*& edge)
		{
			if ((vertex0_ != vertex1) && (vertex1_ != vertex1) && (vertex2_ != vertex1))
			{
				return false;
			}
			if ((vertex0_ != vertex2) && (vertex1_ != vertex2) && (vertex2_ != vertex2))
			{
				return false;
			}
			if ((edge0_ == NULL) || (edge1_ == NULL) || (edge2_ == NULL))
			{
				return false;
			}
			if ((edge0_->getVertex(0) == vertex1) && (edge0_->getVertex(1) == vertex2) ||
			    (edge0_->getVertex(1) == vertex1) && (edge0_->getVertex(0) == vertex2))
			{
				edge = edge0_;
				return true;
			}
			if ((edge1_->getVertex(0) == vertex1) && (edge1_->getVertex(1) == vertex2) ||
			    (edge1_->getVertex(1) == vertex1) && (edge1_->getVertex(0) == vertex2))
			{
				edge = edge1_;
				return true;
			}
			if ((edge2_->getVertex(0) == vertex1) && (edge2_->getVertex(1) == vertex2) ||
			    (edge2_->getVertex(1) == vertex1) && (edge2_->getVertex(0) == vertex2))
			{
				edge = edge2_;
				return true;
			}
			return false;
		}
		
		/** Return the relative index of a rsvertex in the rsface.
				@return	Index	the relative index of the rsvertex
		*/
		Index getRelativeVertexIndex(TRSVertex<T>* vertex)
		{
			if (vertex0_ == vertex)
			{
				return 0;
			}
			if (vertex1_ == vertex)
			{
				return 1;
			}
			if (vertex2_ == vertex)
			{
				return 2;
			}
			return -1;
		}

		/** Return the third rsvertex of this rsface.
				@param	v1						the first vertex
				@param	v2						the second vertex
				@return	TRSVertex<T>*	the third vertex
		*/
		TRSVertex<T>* thirdVertex(TRSVertex<T>* v1, TRSVertex<T>* v2)
		{
			if ((vertex0_ == v1) || (vertex0_ == v2))
			{
				if ((vertex1_ == v1) || (vertex1_ == v2))
				{
					return vertex2_;
				}
				else
				{
					return vertex1_;
				}
			}
			else
			{
				return vertex0_;
			}
		}

		/** Substitute a rsvertex by an other one.
				@param	old_vertex	the vertex that has to be substituted
				@param	new_vertex	the new vertex
				@return	bool				{\bf true}, if the vertex can be substituted,
														{\bf false} otherwise
		*/
		bool substituteVertex(TRSVertex<T>* old_vertex, TRSVertex<T>* new_vertex)
		{
			if (vertex0_ == old_vertex)
			{
				vertex0_ = new_vertex;
				return true;
			}
			if (vertex1_ == old_vertex)
			{
				vertex1_ = new_vertex;
				return true;
			}
			if (vertex2_ == old_vertex)
			{
				vertex2_ = new_vertex;
				return true;
			}
			return false;
		}

		//@}

		protected:

		TRSVertex<T>* vertex0_;
		TRSVertex<T>* vertex1_;
		TRSVertex<T>* vertex2_;
		TRSEdge<T>* edge0_;
		TRSEdge<T>* edge1_;
		TRSEdge<T>* edge2_;
		TVector3<T> center_;
		TVector3<T> normal_;
		bool singular_;
		Index index_;

	};

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
	*/

	template <typename T>
	std::istream& operator >> (std::istream& s, TRSFace<T>& rsface)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


	/**	Output- Operator
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TRSFace<T>& rsface)
		{
			return (s << "RSFACE"
								<< rsface.getIndex()
								<< "([" << (rsface.getVertex(0) == NULL ? -2 : rsface.getVertex(0)->getIndex()) << ' '
								<<				 (rsface.getVertex(1) == NULL ? -2 : rsface.getVertex(1)->getIndex()) << ' '
								<<				 (rsface.getVertex(2) == NULL ? -2 : rsface.getVertex(2)->getIndex()) << "] "
								<< "[" << (rsface.getEdge(0) == NULL ? -2 : rsface.getEdge(0)->getIndex()) << ' '
								<<				(rsface.getEdge(1) == NULL ? -2 : rsface.getEdge(1)->getIndex()) << ' '
								<<				(rsface.getEdge(2) == NULL ? -2 : rsface.getEdge(2)->getIndex()) << "] "
								<< rsface.getCenter() << ' ' << rsface.getNormal() << ' '
								<< (rsface.isSingular() ? "true" : "false") << ")");
		}
	//@}


	/**	The Default RSFace Type.
			If double precision is not needed, {\tt RSFace<float>} should
			be used. It is predefined as {\tt RSFace} for convenience.
	*/
	typedef TRSFace<float> RSFace;
	
	

} // namespace BALL

#endif // BALL_STRUCTURE_RSFACE_H
