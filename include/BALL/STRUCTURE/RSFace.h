// $Id: RSFace.h,v 1.15 2001/06/28 19:12:36 amoll Exp $

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
			throw();

		/**	Copy constructor.
				Create a new RSFace object from another.
				@param	rsface	the RSFace object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TRSFace(const TRSFace<T>& rsface, bool = true)
			throw();

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
		TRSFace(TRSVertex<T>* vertex1,
				TRSVertex<T>* vertex2,
				TRSVertex<T>* vertex3,
				TRSEdge<T>* edge1,
				TRSEdge<T>* edge2,
				TRSEdge<T>* edge3,
				const TVector3<T>& center,
				const TVector3<T>& normal,
				bool singular,
				Index index)
			throw(Exception::DivisionByZero);

		/**	Destructor.
				Destructs the RSFace object.
		*/
		virtual ~TRSFace()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another RSFace.
				@param	rsface	the RSFace object to assign from
				@param	deep ignored
		*/
		void set(const TRSFace<T>& rsface, bool = true)
			throw();

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
		void set(TRSVertex<T>* vertex1,
				TRSVertex<T>* vertex2,
				TRSVertex<T>* vertex3,
				TRSEdge<T>* edge1,
				TRSEdge<T>* edge2,
				TRSEdge<T>* edge3,
				const TVector3<T>& center,
				const TVector3<T>& normal,
				bool singular,
				Index index)
			throw(Exception::DivisionByZero);

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool	{\bf true} if all vertices are similar modulo order	
											and the normals are equal, {\bf false} otherwise
		*/
		bool operator == (const TRSFace& rsface) const
			throw();

		/**	similar
				@return	bool	{\bf true} if all vertices are similar modulo order,	
											{\bf false} otherwise
		*/
		bool similar(const TRSFace& rsface) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false} if all vertices are similar modulo order	
											and the normals are equal, {\bf true} otherwise
		*/
		bool operator != (const TRSFace<T>& rsface) const
			throw();
		
		/** isSingular
				@return	bool	{\bf true} if the RSFace is singular,	
											{\bf false} otherwise
		*/
		bool isSingular() const
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set one of the three RSVertices of the RSFace.
				@param	i				the first vertex is changed if i = 0,	
												the second vertex is changed if i = 1,	
												the third vertex is changed otherwise
				@param	vertex	a pointer to the new vertex
		*/
		void setVertex(Position i, TRSVertex<T>* vertex)
			throw();

		/** Return one of the three RSVertices of the RSFace.
				@param	i
				@return	TRSVertex<T>*	the first RSVertex if i = 0,	
															the second RSVertex if i = 1,	
															the third RSVertex if otherwise
		*/
		TRSVertex<T>* getVertex(Position i) const
			throw();

		/** Set one of the three RSEdges of the RSFace.
				@param	i			the first edge is changed if i = 0,	
											the second edge is changed if i = 1,	
											the third edge is changed otherwise
				@param	edge	a pointer to the new edge
		*/
		void setEdge(Position i, TRSEdge<T>* edge)
			throw();

		/** Return of one of the three RSEdges of the rsface.
				@param	i
				@return	RSEdge*	the first RSEdge if i = 0,	
												the second RSEdge if i = 1,	
												the third RSEdge if otherwise
		*/
		TRSEdge<T>* getEdge(Position i) const
			throw();

		/** Set the center of the probe sphere defining the RSFace.
				@param	center	the new center
		*/
		void setCenter(const TVector3<T>& center)
			throw();

		/** Return the center of the probe sphere defining the RSFace.
				@return	TVector3<T>	the center of the probe sphere
		*/
		TVector3<T> getCenter() const
			throw();

		/** Set the vector orthogonal to the RSFace.
				@param	normal	the new normal
		*/
		void setNormal(const TVector3<T>& normal)
			throw(Exception::DivisionByZero);

		/** Return the vector orthogonal to the RSFace.
				@return	TVector3<T>	the vector orthogonal to the RSFace.
		*/
		TVector3<T> getNormal() const
			throw();

		/** Set singular
		*/
		void setSingular(bool singular)
			throw();

		/** Set the index of the RSFace.
				@param	index	the new index
		*/
		void setIndex(Index index)
			throw();

		/** Return the index of the RSFace.
				@return	Index	the index of the RSFace
		*/
		Index getIndex() const
			throw();

		/** Find the two RSEdges of this RSFace that belong to the given RSVertex
				@param	vertex	a pointer to the given vertex
				@param	edge1		a pointer to the first found edge
				@param	edge2		a pointer to the second found edge
				@return	bool		{\bf true} if the edges can be found,	
												{\bf false} otherwise
		*/
		bool getEdges(TRSVertex<T>* vertex, TRSEdge<T>*& edge1, TRSEdge<T>*& edge2)
			throw();
		
		/** Find the RSEdge of this RSFace that belongs to the two given RSVertices
				@param	vertex1	a pointer to the first given vertex
				@param	vertex2	a pointer to the second given vertex
				@param	edge		a pointer to the found edge
				@return	bool		{\bf true} if the edge can be found,	
												{\bf false} otherwise
		*/
		bool getEdge(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, TRSEdge<T>*& edge)
			throw();
		
		/** Return the relative index of a RSVertex in the RSFace.
				@return	Index	the relative index of the RSVertex
		*/
		Index getRelativeVertexIndex(TRSVertex<T>* vertex)
			throw();

		/** Return the third RSVertex of this RSFace.
				@param	v1						the first vertex
				@param	v2						the second vertex
				@return	TRSVertex<T>*	the third vertex
		*/
		TRSVertex<T>* thirdVertex(TRSVertex<T>* v1, TRSVertex<T>* v2)
			throw();

		/** Substitute a RSVertex by an other one.
				@param	old_vertex	the vertex that has to be substituted
				@param	new_vertex	the new vertex
				@return	bool				{\bf true}, if the vertex can be substituted,	
														{\bf false} otherwise
		*/
		bool substituteVertex(TRSVertex<T>* old_vertex, TRSVertex<T>* new_vertex)
			throw();

		//@}

		protected:

		/*_ The first RSVertex of the RSFace
		*/
		TRSVertex<T>* vertex0_;
		/*_ The second RSVertex of the RSFace
		*/
		TRSVertex<T>* vertex1_;
		/*_ The third RSVertex of the RSFace
		*/
		TRSVertex<T>* vertex2_;
		/*_ The first RSEdge of the RSFace
		*/
		TRSEdge<T>* edge0_;
		/*_ The second RSEdge of the RSFace
		*/
		TRSEdge<T>* edge1_;
		/*_ The third RSEdge of the RSFace
		*/
		TRSEdge<T>* edge2_;
		/*_ The center of the probe sphere defining the RSFace
		*/
		TVector3<T> center_;
		/*_ The vector orthogonal to the RSFace
		*/
		TVector3<T> normal_;
		/*_ singular
		*/
		bool singular_;
		/* The index of the RSFace
		*/
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



	template <typename T>
	TRSFace<T>::TRSFace()
		throw()
		: vertex0_(NULL),
			vertex1_(NULL),
			vertex2_(NULL),
			edge0_(NULL),
			edge1_(NULL),
			edge2_(NULL),
			center_(),
			normal_(),
			singular_(false),
			index_(-1)
	{
	}


	template <typename T>
	TRSFace<T>::TRSFace(const TRSFace<T>& rsface, bool)
		throw()
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


	template <typename T>
	TRSFace<T>::TRSFace(TRSVertex<T>* vertex1,
			TRSVertex<T>* vertex2,
			TRSVertex<T>* vertex3,
			TRSEdge<T>* edge1,
			TRSEdge<T>* edge2,
			TRSEdge<T>* edge3,
			const TVector3<T>& center,
			const TVector3<T>& normal,
			bool singular,
			Index index)
		throw(Exception::DivisionByZero)
		: vertex0_(vertex1),
			vertex1_(vertex2),
			vertex2_(vertex3),
			edge0_(edge1),
			edge1_(edge2),
			edge2_(edge3),
			center_(center),
			normal_(normal),
			singular_(singular),
			index_(index)
	{
		normal_.normalize();
	}


	template <typename T>
	TRSFace<T>::~TRSFace()
		throw()
	{
	}


	template <typename T>
	void TRSFace<T>::set(const TRSFace<T>& rsface, bool)
		throw()
	{
		vertex0_ = rsface.vertex0_;
		vertex1_ = rsface.vertex1_;
		vertex2_ = rsface.vertex2_;
		edge0_ = rsedge.edge0_;
		edge1_ = rsedge.edge1_;
		edge2_ = rsedge.edge2_;
		center_ = rsface.center_;
		normal_ = rsface.normal_;
		singular_ = rsface.singular_;
		index_ = rsface.index_;
	}


	template <typename T>
	void TRSFace<T>::set(TRSVertex<T>* vertex1,
			TRSVertex<T>* vertex2,
			TRSVertex<T>* vertex3,
			TRSEdge<T>* edge1,
			TRSEdge<T>* edge2,
			TRSEdge<T>* edge3,
			const TVector3<T>& center,
			const TVector3<T>& normal,
			bool singular,
			Index index)
		throw(Exception::DivisionByZero)
	{
		vertex0_ = vertex1;
		vertex1_ = vertex2;
		vertex2_ = vertex3;
		edge0_ = edge1;
		edge1_ = edge2;
		edge2_ = edge3;
		center_ = center;
		normal_ = normal;
		singular_ = singular;
		index_ = index;
		normal_.normalize();
	}


	template <typename T>
	bool TRSFace<T>::operator == (const TRSFace& rsface) const
		throw()
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


	template <typename T>
	bool TRSFace<T>::similar(const TRSFace& rsface) const
		throw()
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


	template <typename T>
	bool TRSFace<T>::operator != (const TRSFace<T>& rsface) const
		throw()
	{
		return (bool)(!(*this == rsface));
	}


	template <typename T>
	bool TRSFace<T>::isSingular() const
		throw()
	{
		return singular_;
	}


	template <typename T>
	void TRSFace<T>::setVertex(Position i, TRSVertex<T>* vertex)
		throw()
	{
		switch (i)
		{
			case 0  :	vertex0_ = vertex; break;
			case 1  :	vertex1_ = vertex; break;
			default :	vertex2_ = vertex; break;
		}
	}


	template <typename T>
	TRSVertex<T>* TRSFace<T>::getVertex(Position i) const
		throw()
	{
		switch (i)
		{
			case 0  :	return vertex0_;
			case 1  :	return vertex1_;
			default :	return vertex2_;
		}
	}


	template <typename T>
	void TRSFace<T>::setEdge(Position i, TRSEdge<T>* edge)
		throw()
	{
		switch (i)
		{
			case 0  :	edge0_ = edge; break;
			case 1  :	edge1_ = edge; break;
			default :	edge2_ = edge; break;
		}
	}


	template <typename T>
	TRSEdge<T>* TRSFace<T>::getEdge(Position i) const
		throw()
	{
		switch (i)
		{
			case 0  :	return edge0_;
			case 1  :	return edge1_;
			default :	return edge2_;
		}
	}


	template <typename T>
	void TRSFace<T>::setCenter(const TVector3<T>& center)
		throw()
	{
		center_ = center;
	}


	template <typename T>
	TVector3<T> TRSFace<T>::getCenter() const
		throw()
	{
		return center_;
	}


	template <typename T>
	void TRSFace<T>::setNormal(const TVector3<T>& normal)
		throw(Exception::DivisionByZero)
	{
		normal_ = normal;
		normal_.normalize();
	}


	template <typename T>
	TVector3<T> TRSFace<T>::getNormal() const
		throw()
	{
		return normal_;
	}


	template <typename T>
	void TRSFace<T>::setSingular(bool singular)
		throw()
	{
		singular_ = singular;
	}


	template <typename T>
	void TRSFace<T>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename T>
	Index TRSFace<T>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename T>
	bool TRSFace<T>::getEdges(TRSVertex<T>* vertex,
			TRSEdge<T>*& edge1,
			TRSEdge<T>*& edge2)
		throw()
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


	template <typename T>
	bool TRSFace<T>::getEdge(TRSVertex<T>* vertex1,
			TRSVertex<T>* vertex2,
			TRSEdge<T>*& edge)
		throw()
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


	template <typename T>
	Index TRSFace<T>::getRelativeVertexIndex(TRSVertex<T>* vertex)
		throw()
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


	template <typename T>
	TRSVertex<T>* TRSFace<T>::thirdVertex(TRSVertex<T>* v1, TRSVertex<T>* v2)
		throw()
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


	template <typename T>
	bool TRSFace<T>::substituteVertex(TRSVertex<T>* old_vertex,
			TRSVertex<T>* new_vertex)
		throw()
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

	
	

} // namespace BALL

#endif // BALL_STRUCTURE_RSFACE_H
