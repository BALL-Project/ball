// $Id: RSFace.h,v 1.2 2000/10/19 14:24:51 strobel Exp $

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
			\\
			{\bf Definition:} \URL{BALL/STRUCTURE/RSFace.h}
			\\
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
			: vertex0_(-1), vertex1_(-1), vertex2_(-1), edge0_(), edge1_(), edge2_(),
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
		TRSFace(const Index vertex1, const Index vertex2, const Index vertex3,
						TRSEdge<T>* edge1, TRSEdge<T>* edge2, TRSEdge<T>* edge3,
						const TVector3<T>& center, const TVector3<T>& normal, const bool singular, const Index index)
			: vertex0_(vertex1), vertex1_(vertex2), vertex2_(vertex3),
				edge0_(edge1), edge1_(edge2), edge2_(edge3),
				center_(center), normal_(normal), singular_(singular), index_(index)
		{
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
/*			vertex0_ = rsface.getVertex(0); vertex1_ = rsface.getVertex(1); vertex2_ = rsface.getVertex(2);
			edge0_ = rsedge.getEdge(0); edge1_ = rsedge.getEdge(1); edge2_ = rsedge.getEdge(2);
			center_ = rsface.getCenter(); normal_ = rsface.getNormal();
			singular_ = rsface.getSingular(); index_ = rsface.getIndex();*/
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
		void set(const Index vertex1, const Index vertex2, const Index vertex3,
						 TRSEdge<T>* edge1, TRSEdge<T>* edge2, TRSEdge<T>* edge3,
						 const TVector3<T>& center, const TVector3<T>& normal, const bool singular, const Index index)
		{
			vertex0_ = vertex1; vertex1_ = vertex2; vertex2_ = vertex3;
			edge0_ = edge1; edge1_ = edge2; edge2_ = edge3;
			center_ = center;	normal_ = normal; singular_ = singular; index_ = index;
		}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all vertices are equal modulo order and the centers are equal,
											{\bf false} otherwise
		*/
		bool operator == (TRSFace& rsface) const
		{
			if (center_ != rsface.getCenter())
				{
					return false;
				}
			if ((vertex0_ != rsface.getVertex(0)) &&
					(vertex0_ != rsface.getVertex(1)) &&
					(vertex0_ != rsface.getVertex(2)))
				{
					return false;
				}
			if ((vertex1_ != rsface.getVertex(0)) &&
					(vertex1_ != rsface.getVertex(1)) &&
					(vertex1_ != rsface.getVertex(2)))
				{
					return false;
				}
			if ((vertex2_ != rsface.getVertex(0)) &&
					(vertex2_ != rsface.getVertex(1)) &&
					(vertex2_ != rsface.getVertex(2)))
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
			if ((vertex0_ != rsface.getVertex(0)) &&
					(vertex0_ != rsface.getVertex(1)) &&
					(vertex0_ != rsface.getVertex(2)))
				{
					return false;
				}
			if ((vertex1_ != rsface.getVertex(0)) &&
					(vertex1_ != rsface.getVertex(1)) &&
					(vertex1_ != rsface.getVertex(2)))
				{
					return false;
				}
			if ((vertex2_ != rsface.getVertex(0)) &&
					(vertex2_ != rsface.getVertex(1)) &&
					(vertex2_ != rsface.getVertex(2)))
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
			return (bool)(!(this == rsface));
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

		/** Change the index of one of the three rsvertices of the rsface.
				@param i the first vertex is changed if i = 0, the second otherwise
				@param vertex the new index
		*/
		void  setVertex(const Position i, const Index vertex)
		{
			switch (i)
			{
				case 0  :	vertex0_ = vertex; break;
				case 1  :	vertex1_ = vertex; break;
				default :	vertex2_ = vertex; break;
			}
		}
		/** Return the index of one of the three rsvertices of the rsface.
				@return Index the index of the first rsvertex if i = 0,
											the index of the second rsvertex if i = 1
											the index of the third rsvertex if otherwse
		*/
		Index getVertex(const Position i)
		{
			switch (i)
			{
				case 0  :	return vertex0_;
				case 1  :	return vertex1_;
				default :	return vertex2_;
			}
		}

		/** Change the indices of the three rsedges of the rsface.
				@param i the first edge is changed if i = 0, the second otherwise
				@param edge a pointer to the new edge
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

		/** Return the index of one of the three rsedges of the rsface.
				@return Index the index of the first rsedge if i = 0,
											the index of the second rsedge if i = 1
											the index of the third rsedge if otherwse
		*/
		TRSEdge<T>* getEdge(const Position i)
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
				@param normal the new normal
		*/
		void setNormal(const TVector3<T>& normal)
		{
			normal_ = normal;
		}

		/** Return the vector orthogonal to the rsface.
				@return TVector3<T> the vector orthogonal to the rsface.
		*/
		TVector3<T> getNormal()
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
				@param vertex the index of the given vertex
				@param edge1 the index of the first found edge
				@param edge2 the index of the second found edge
				@return bool {\bf true} if the edges can be found, {\bf false} otherwise
		*/
		bool getEdges(const Index vertex, Index& edge1, Index& edge2)
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
					edge1 = edge1_->getIndex();
					edge2 = edge2_->getIndex();
				}
				else
				{
					edge1 = edge0_->getIndex();
					if ((edge1_->getVertex(0) == vertex) || (edge1_->getVertex(1) == vertex))
						{
							edge2 = edge1_->getIndex();
						}
						else
						{
							edge2 = edge2_->getIndex();
						}
				}
			return true;
		}
		
		/** Find the rsedge of this face that belong to the two given rsvertices
				@param vertex1 the index of the first given vertex
				@param vertex2 the index of the second given vertex
				@param edge the index of the first found edge
				@return bool {\bf true} if the edge can be found, {\bf false} otherwise
		*/
		bool getEdge(const Index vertex1, const Index vertex2, Index& edge)
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
				edge = edge0_->getIndex();
				return true;
			}
			if ((edge1_->getVertex(0) == vertex1) && (edge1_->getVertex(1) == vertex2) ||
			    (edge1_->getVertex(1) == vertex1) && (edge1_->getVertex(0) == vertex2))
			{
				edge = edge1_->getIndex();
				return true;
			}
			if ((edge2_->getVertex(0) == vertex1) && (edge2_->getVertex(1) == vertex2) ||
			    (edge2_->getVertex(1) == vertex1) && (edge2_->getVertex(0) == vertex2))
			{
				edge = edge2_->getIndex();
				return true;
			}
			return false;
		}
		
		/** Return the relative index of a rsvertex in thie rsface.
				@return Index, the relative index of the rsvertex
		*/
		Index getRelativeVertexIndex(const Index vertex)
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


		/** Return the index of the third rsvertex of this rsface.
				@param v1 the index of the first vertex
				@param v2 the index of the second vertex
				@return Index, the index of the third vertex
		*/
		Index thirdVertexIndex(const Index v1, const Index v2)
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

		//@}

		protected:

		Index vertex0_;
		Index vertex1_;
		Index vertex2_;
		TRSEdge<T>* edge0_;
		TRSEdge<T>* edge1_;
		TRSEdge<T>* edge2_;
		TVector3<T> center_;
		TVector3<T> normal_;
		bool singular_;
		int index_;

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
			return (s << "RSFACE" << rsface.getIndex()
								<< "([" << rsface.getVertex(0) << ' '
								<< rsface.getVertex(1) << ' '
								<< rsface.getVertex(2) << "] ["
								<< (rsface.getEdge(0) == NULL ? -2 : rsface.getEdge(0)->getIndex()) << ' '
								<< (rsface.getEdge(1) == NULL ? -2 : rsface.getEdge(1)->getIndex()) << ' '
								<< (rsface.getEdge(2) == NULL ? -2 : rsface.getEdge(2)->getIndex()) << "] "
								<< rsface.getCenter() << ' ' << rsface.getNormal()
								<< ' ' << (rsface.isSingular() ? "true" : "false")
								<< ")");
		}
	//@}


	/**	The Default RSFace Type.
			If double precision is not needed, {\tt RSFace<float>} should
			be used. It is predefined as {\tt RSFace} for convenience.
	*/
	typedef TRSFace<float> RSFace;

} // namespace BALL

#endif // BALL_STRUCTURE_RSFACE_H
