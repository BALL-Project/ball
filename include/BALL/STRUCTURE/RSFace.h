// $Id: RSFace.h,v 1.20 2001/12/08 17:00:53 strobel Exp $

#ifndef BALL_STRUCTURE_RSFACE_H
#define BALL_STRUCTURE_RSFACE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHEDGE_H
#	include <BALL/STRUCTURE/graphEdge.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#	include <BALL/STRUCTURE/graphFace.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif

namespace BALL
{
	
	template <typename T>
	class TRSVertex;

	template <typename T>
	class TRSEdge;

	template <typename T>
	class TReducedSurface;

	template <typename T>
	class TSolventExcludedSurface;

	template <typename T>
	class TSESEdge;

	template <typename T>
	class TSESFace;

	template <typename T>
	class TSESVertex;

	template <typename T>
	class TTriangulatedSES;

	/** Generic RSFace Class.	
			{\bf Definition:} \URL{BALL/STRUCTURE/RSFace.h} 
	*/
	template <class T>
	class TRSFace	:	public GraphFace< TRSVertex<T>,TRSEdge<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TReducedSurface<T>
					\item class TRSEdge<T>
					\item class TRSVertex<T>
					\item class TSolventExcludedSurface<T>;
					\item class TSESFace<T>
					\item class TSESEdge<T>
					\item class TSESVertex<T>
					\item class TTriangulatedSES<T>
				\end{itemize}
		*/
		friend class GraphEdge< TRSVertex<T>,TRSFace<T> >;
		friend class GraphFace< TRSVertex<T>,TRSEdge<T> >;
		friend class GraphVertex< TRSEdge<T>,TRSFace<T> >;
		friend class TReducedSurface<T>;
		friend class TRSEdge<T>;
		friend class TRSVertex<T>;
		friend class TSolventExcludedSurface<T>;
		friend class TSESFace<T>;
		friend class TSESEdge<T>;
		friend class TSESVertex<T>;
		friend class TTriangulatedSES<T>;

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
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the new
												RSFace object is linked to the neighbours of the old RSFace object.
		*/
		TRSFace(const TRSFace<T>& rsface, bool deep = false)
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
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the
												RSFace object is linked to the neighbours of the RSFace object to be copied.
		*/
		virtual void set(const TRSFace<T>& rsface, bool deep = false)
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
		virtual void set(TRSVertex<T>* vertex1,
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
		virtual bool operator == (const TRSFace& rsface) const
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
		virtual bool operator != (const TRSFace<T>& rsface) const
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

		/** Return the third RSVertex of the RSFace.
				@param	v1						the first vertex
				@param	v2						the second vertex
				@return	TRSVertex<T>*	the third vertex
		*/
		TRSVertex<T>* thirdVertex(TRSVertex<T>* v1, TRSVertex<T>* v2)
			throw();

		/** Return the third RSEdge of the RSFace.
				@param	e1					the first edge
				@param	e2					the second edge
				@return	TRSEdge<T>*	the third edge
		*/
		TRSEdge<T>* thirdEdge(TRSEdge<T>* e1, TRSEdge<T>* e2)
			throw();

		void remove(HashSet<TRSFace<T>*>& faces)
		{
			vertex_[0]->deleteFace(this);
			vertex_[1]->deleteFace(this);
			vertex_[2]->deleteFace(this);
			TRSFace<T>* face;
			if (edge_[0] != NULL)
			{
				if ((edge_[0]->getFace(1) == NULL) && (edge_[0]->has() == false))
				{
					edge_[0]->remove();
				}
				else
				{
					face = edge_[0]->deleteFace(this);
					faces.insert(face);
				}
			}
			if (edge_[1] != NULL)
			{
				faces.insert(edge_[1]->deleteFace(this));
			}
			if (edge_[2] != NULL)
			{
				faces.insert(edge_[2]->deleteFace(this));
			}
		}

		//@}

		protected:

		/*_ The center of the probe sphere defining the RSFace
		*/
		TVector3<T> center_;
		/*_ The vector orthogonal to the RSFace
		*/
		TVector3<T> normal_;
		/*_ singular
		*/
		bool singular_;

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
								//<< " (" << (rsface.getVertex(0) == NULL ? -2 : rsface.getVertex(0)->getAtom()) << ") "
								<<				 (rsface.getVertex(1) == NULL ? -2 : rsface.getVertex(1)->getIndex()) << ' '
								//<< " (" << (rsface.getVertex(1) == NULL ? -2 : rsface.getVertex(1)->getAtom()) << ") "
								<<				 (rsface.getVertex(2) == NULL ? -2 : rsface.getVertex(2)->getIndex()) << "] "
								//<< " (" << (rsface.getVertex(2) == NULL ? -2 : rsface.getVertex(2)->getAtom()) << ") "
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
		: GraphFace< TRSVertex<T>,TRSEdge<T> >(),
			center_(),
			normal_(),
			singular_(false)
	{
		vertex_.push_back(NULL);
		vertex_.push_back(NULL);
		vertex_.push_back(NULL);
		number_of_vertices_ = 3;
		edge_.push_back(NULL);
		edge_.push_back(NULL);
		edge_.push_back(NULL);
		number_of_edges_ = 3;
	}


	template <typename T>
	TRSFace<T>::TRSFace(const TRSFace<T>& rsface, bool deep)
		throw()
		:	GraphFace< TRSVertex<T>,TRSEdge<T> >(rsface,deep),
			center_(rsface.center_),
			normal_(rsface.normal_),
			singular_(rsface.singular_)
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
		:	GraphFace< TRSVertex<T>,TRSEdge<T> >(),
			center_(center),
			normal_(normal),
			singular_(singular)
	{
		vertex_.push_back(vertex1);
		vertex_.push_back(vertex2);
		vertex_.push_back(vertex3);
		number_of_vertices_ = 3;
		edge_.push_back(edge1);
		edge_.push_back(edge2);
		edge_.push_back(edge3);
		number_of_edges_ = 3;
		index_ = index;
		normal_.normalize();
	}


	template <typename T>
	TRSFace<T>::~TRSFace()
		throw()
	{
	}


	template <typename T>
	void TRSFace<T>::set(const TRSFace<T>& rsface, bool deep)
		throw()
	{
		if (deep)
		{
			vertex_[0] = rsface.vertex_[0];
			vertex_[1] = rsface.vertex_[1];
			vertex_[2] = rsface.vertex_[2];
			edge_[0] = rsface.edge_[0];
			edge_[1] = rsface.edge_[1];
			edge_[2] = rsface.edge_[2];
		}
		else
		{
			vertex_[0] = NULL;
			vertex_[1] = NULL;
			vertex_[2] = NULL;
			edge_[0] = NULL;
			edge_[1] = NULL;
			edge_[2] = NULL;
		}
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
		vertex_[0] = vertex1;
		vertex_[1] = vertex2;
		vertex_[2] = vertex3;
		edge_[0] = edge1;
		edge_[1] = edge2;
		edge_[2] = edge3;
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
		//if (center_ != rsface.center_)
		{
			return false;
		}
		if ((vertex_[0]->atom_ != rsface.vertex_[0]->atom_) &&
				(vertex_[0]->atom_ != rsface.vertex_[1]->atom_) &&
				(vertex_[0]->atom_ != rsface.vertex_[2]->atom_)    )
		{
			return false;
		}
		if ((vertex_[1]->atom_ != rsface.vertex_[0]->atom_) &&
				(vertex_[1]->atom_ != rsface.vertex_[1]->atom_) &&
				(vertex_[1]->atom_ != rsface.vertex_[2]->atom_)    )
		{
			return false;
		}
		if ((vertex_[2]->atom_ != rsface.vertex_[0]->atom_) &&
				(vertex_[2]->atom_ != rsface.vertex_[1]->atom_) &&
				(vertex_[2]->atom_ != rsface.vertex_[2]->atom_)    )
		{
			return false;
		}
		return true;
	}


	template <typename T>
	bool TRSFace<T>::similar(const TRSFace& rsface) const
		throw()
	{
		if ((vertex_[0]->atom_ != rsface.vertex_[0]->atom_) &&
				(vertex_[0]->atom_ != rsface.vertex_[1]->atom_) &&
				(vertex_[0]->atom_ != rsface.vertex_[2]->atom_)    )
		{
			return false;
		}
		if ((vertex_[1]->atom_ != rsface.vertex_[0]->atom_) &&
				(vertex_[1]->atom_ != rsface.vertex_[1]->atom_) &&
				(vertex_[1]->atom_ != rsface.vertex_[2]->atom_)    )
		{
			return false;
		}
		if ((vertex_[2]->atom_ != rsface.vertex_[0]->atom_) &&
				(vertex_[2]->atom_ != rsface.vertex_[1]->atom_) &&
				(vertex_[2]->atom_ != rsface.vertex_[2]->atom_)    )
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
	TRSVertex<T>* TRSFace<T>::thirdVertex(TRSVertex<T>* v1, TRSVertex<T>* v2)
		throw()
	{
		if ((vertex_[0] == v1) || (vertex_[0] == v2))
		{
			if ((vertex_[1] == v1) || (vertex_[1] == v2))
			{
				return vertex_[2];
			}
			else
			{
				return vertex_[1];
			}
		}
		else
		{
			return vertex_[0];
		}
	}


	template <typename T>
	TRSEdge<T>* TRSFace<T>::thirdEdge(TRSEdge<T>* e1, TRSEdge<T>* e2)
		throw()
	{
		if ((edge_[0] == e1) || (edge_[0] == e2))
		{
			if ((edge_[1] == e1) || (edge_[1] == e2))
			{
				return edge_[2];
			}
			else
			{
				return edge_[1];
			}
		}
		else
		{
			return edge_[0];
		}
	}


	
	

} // namespace BALL

#endif // BALL_STRUCTURE_RSFACE_H
