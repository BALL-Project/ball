// $Id: RSEdge.h,v 1.20 2001/12/08 17:00:14 strobel Exp $

#ifndef BALL_STRUCTURE_RSEDGE_H
#define BALL_STRUCTURE_RSEDGE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
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
	class TReducedSurface;

	template <typename T>
	class TRSFace;

	template <typename T>
	class TRSVertex;

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

	/** Generic RSEdge Class.	
      {\bf Definition:} \URL{BALL/STRUCTURE/RSEdge.h} 
	*/
	template <class T>
	class TRSEdge	:	public GraphEdge< TRSVertex<T>,TRSFace<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TReducedSurface<T>
					\item class TRSFace<T>
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
		friend class TRSFace<T>;
		friend class TRSVertex<T>;
		friend class TSolventExcludedSurface<T>;
		friend class TSESFace<T>;
		friend class TSESEdge<T>;
		friend class TSESVertex<T>;
		friend class TTriangulatedSES<T>;

		BALL_CREATE(TRSEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new RSEdge object.
		*/
		TRSEdge()
			throw();

		/**	Copy constructor.
				Create a new RSEdge object from another.
				@param	rsface	the RSEdge object to be copied
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the new
												RSEdge object is linked to the neighbours of the old RSEdge object.
		*/
		TRSEdge(const TRSEdge<T>& rsedge, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new RSEdge object from a lot of nice objects
				@param	vertex1							assigned to the first vertex
				@param	vertex1							assigned to the second vertex
				@param	face1								assigned to the first face
				@param	face2								assigned to the second face
				@param	center_of_torus			assigned to the center of the torus
				@param	radius_of_torus			assigned to the radius of the torus
				@param	phi									assigned to th angle
				@param	circle1							assigned to the first contact circle
				@param	circle2							assigned to the second contact circle
				@param	intersection_point1	assigned to the first intersection point	
																		(in singular case)
				@param	intersection_point2	assigned to the second intersection point	
																		(in singular case)
				@param	singular
				@param	index								assigned to the index
		*/
		TRSEdge(TRSVertex<T>* vertex1,
				TRSVertex<T>* vertex2,
				TRSFace<T>* face1,
				TRSFace<T>* face2,
				const TVector3<T>& center_of_torus,
				const T& radius_of_torus,
				const TAngle<T>& phi,
				const TCircle3<T>& circle1,
				const TCircle3<T>& circle2,
				const TVector3<T>& intersection_point1,
				const TVector3<T>& intersection_point2,
				bool singular,
				Index index)
			throw();

		/**	Destructor.
				Destructs the RSEdge object.
				The neighboured faces and vertices are {/bf not} destructed.
		*/
		virtual ~TRSEdge()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another RSEdge.
				@param	rsedge	the RSEdge object to assign from
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the
												RSEdge object is linked to the neighbours of the RSEdge object to be copied.
		*/
		virtual void set(const TRSEdge& rsedge, bool deep = false)
			throw();

		/**	Assign from a lot of nice objects
				@param	vertex1							assigned to the first vertex
				@param	vertex1							assigned to the second vertex
				@param	face1								assigned to the first face
				@param	face2								assigned to the second face
				@param	center_of_torus			assigned to the center of the torus
				@param	radius_of_torus			assigned to the radius of the torus
				@param	phi									assigned to th angle
				@param	circle1							assigned to the first contact circle
				@param	circle2							assigned to the second contact circle
				@param	intersection_point1	assigned to the first intersection point	
																		(in singular case)
				@param	intersection_point2	assigned to the second intersection point	
																		(in singular case)
				@param	singular
				@param	index								assigned to the index
		*/
		virtual void set(TRSVertex<T>* vertex0,
				TRSVertex<T>* vertex1,
				TRSFace<T>* face0,
				TRSFace<T>* face1,
				const TVector3<T>& center_of_torus,
				const T& radius_of_torus,
				const TAngle<T>& phi,
				const TCircle3<T>& circle0,
				const TCircle3<T>& circle1,
				const TVector3<T>& intersection_point0,
				const TVector3<T>& intersection_point1,
				bool singular,
				Index index)
			throw();
		
		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the center of the torus.
				Set the center of the torus described by the probe sphere when it	
				rolls over the RSEdge.
				@param	center	the new center
		*/
		void setCenterOfTorus(const TVector3<T>& center)
			throw();

		/** Return the center of the torus.
				Return the center of the torus described by the probe sphere	
				when it rolls over the RSEdge.
				@return	TVector3<T>	the center of the torus
		*/
		TVector3<T> getCenterOfTorus() const
			throw();

		/** Set the major radius of the torus.
				Set the major radius of the torus described by the probe sphere	
				when it rolls over the RSEdge.
				@param	radius	the new radius
		*/
		void setMajorRadiusOfTorus(const T& radius)
			throw();

		/** Return the  major radius of the torus.
				Return the  major radius of the torus described by the probe sphere	
				when it rolls over the RSEdge.
				@return	TVector3<T>	the major radius of the torus
		*/
		T getMajorRadiusOfTorus() const
			throw();

		/** Set the rotation angle.
				Set the rotation angle between the starting and ending position of	
				the probe sphere when it rolls over the RSEdge.
				@param	phi	the new rotation angle
		*/
		void setPhi(const TAngle<T>& phi)
			throw();

		/** Return the rotation angle.
				Return the rotation angle between the starting and ending position	
				of the probe sphere when it rolls over the RSEdge.
				@return	TAngle<T>	the rotation angle
		*/
		TAngle<T> getPhi() const
			throw();

		/** Set one of the two contact circles.
				Set one of the two contact circles of the probe sphere with the two	
				RSVertices.
				@param	i				the first contact circle is changed if i = 0,	
												the second otherwise
				@param	circle	the new contact circle
		*/
		void  setContactCircle(Position i, const TCircle3<T>&  circle)
			throw();

		/** Return one of the the contact circles.
				Return one of the the contact circles of the probe sphere with the	
				two RSVertices
				@param	i
				@return	TCircle3<T>	the contact circle with the first RSVertex	
														if i = 0, the contact circle with the second	
														RSVertex otherwise
		*/
		TCircle3<T> getContactCircle(Position i) const
			throw();

		/** Set one of the intersection points.
				Set one of the intersection points of the probe sphere with the RSEdge.
				@param	i			the first intersection point is changed if i = 0,	
											the second otherwise
				@param	point	the new intersection point
		*/
		void setIntersectionPoint(Position i, const TVector3<T>& point)
			throw();

		/** Return one of the intersection points.
				Return one of the intersection points of the probe sphere	
				with the RSEdge. If the RSEdge not is singular, an exception	
				is thrown.
				@return	TVector3<T>	the intersection point near to the first	
														RSVertex if i = 0, the intersection point near	
														to the second RSVertex otherwise
		*/
		TVector3<T> getIntersectionPoint(Position i) const
			throw(Exception::GeneralException);

		/** Set singular
		*/
		void setSingular(bool singular)
			throw();


		void remove()
		{	
			vertex_[0]->edges_.erase(this);
			vertex_[1]->edges_.erase(this);
			//if (face0_ != NULL)
			//{
			//	face0_->deleteEdge(this);
			//}
			//if (face1_ != NULL)
			//{
			//	face1_->deleteEdge(this);
			//}
		}

		//@}


		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all vertices are similar and all faces	
											are equal modulo order, {\bf false} otherwise
		*/
		virtual bool operator == (const TRSEdge<T>& rsedge) const
			throw();

		/**	similar.
				@return	bool	{\bf true} if all vertices are similar modulo order,	
											{\bf false} otherwise
		*/
		bool similar(const TRSEdge<T>& rsedge) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false} if all vertices are similar and all faces	
											are equal modulo order, {\bf true} otherwise
		*/
		virtual bool operator != (const TRSEdge<T>& rsedge) const
			throw();
		
		/** isSingular
				@return	bool	{\bf true} if the RSEdge is singular,	
											{\bf false} otherwise
		*/
		bool isSingular() const
			throw();

		/** isFree
				@return	bool	{\bf true} if the RSEdge is free, {\bf false} otherwise
		*/
		bool isFree() const
			throw();

		//@}


		protected:

		/*_ The center of the torus described by the probe when ir rolls over	
				the RSEdge
		*/
		TVector3<T> center_of_torus_;
		/*_ The major radius of the torus described by the probe when ir rolls	
				over the RSEdge
		*/
		T radius_of_torus_;
		/*_ The rotation angle between the starting and ending position of the	
				probe sphere when it rolls over the RSEdge
		*/
		TAngle<T> phi_;
		/*_ The contact circle of the probe sphere with the first RSVertex
		*/
		TCircle3<T> circle0_;
		/*_ The contact circle of the probe sphere with the second RSVertex
		*/
		TCircle3<T> circle1_;
		/*_ The intersection point of the probe sphere with the RSEdge near to	
				the first RSVertex (in singular case).
		*/
		TVector3<T> intersection_point0_;
		/*_ The intersection point of the probe sphere with the RSEdge near to	
				the second RSVertex (in singular case).
		*/
		TVector3<T> intersection_point1_;
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
	std::istream& operator >> (std::istream& s, TRSEdge<T>& rsedge)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


	/**	Output- Operator
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TRSEdge<T>& rsedge)
		{
			s << "RSEDGE" << rsedge.getIndex()
				<< "([" << (rsedge.getVertex(0) == NULL ?
												-2 : rsedge.getVertex(0)->getIndex()) << ' '
				<<				 (rsedge.getVertex(1) == NULL ?
												-2 : rsedge.getVertex(1)->getIndex()) << "] "
				<< "[" << (rsedge.getFace(0) == NULL ?
												-2 : rsedge.getFace(0)->getIndex()) << ' '
				<<				(rsedge.getFace(1) == NULL ?
												-2 : rsedge.getFace(1)->getIndex()) << "] "
				<< rsedge.getCenterOfTorus() << ' '
				<< rsedge.getMajorRadiusOfTorus() << ' ' << rsedge.getPhi() << ' '
				<< rsedge.getContactCircle(0) << ' '
				<< rsedge.getContactCircle(1) << ' ';
			bool singular(rsedge.isSingular());
			if (singular)
			{
				s << rsedge.getIntersectionPoint(0) << ' '
					<< rsedge.getIntersectionPoint(1) << " true)";
			}
			else
			{
				s << TVector3<T>::getZero() << ' '
					<< TVector3<T>::getZero() << " false)";
			}
			return s;
		}

	//@}


	/**	The Default RSEdge Type.
			If double precision is not needed, {\tt RSEdge<float>} should
			be used. It is predefined as {\tt RSEdge} for convenience.
	*/
	typedef TRSEdge<float> RSEdge;



	template <typename T>
	TRSEdge<T>::TRSEdge()
		throw()
		: GraphEdge< TRSVertex<T>,TRSFace<T> >(),
			center_of_torus_(),
			radius_of_torus_((T)0),
			phi_(),
			circle0_(),
			circle1_(),
			intersection_point0_(),
			intersection_point1_(),
			singular_(false)
	{
	}


	template <typename T>
	TRSEdge<T>::TRSEdge(const TRSEdge<T>& rsedge, bool deep)
		throw()
		: GraphEdge< TRSVertex<T>,TRSFace<T> >(rsedge,deep),
			center_of_torus_(rsedge.center_of_torus_),
			radius_of_torus_(rsedge.radius_of_torus_),
			phi_(rsedge.phi_),
			circle0_(rsedge.circle0_),
			circle1_(rsedge.circle1_),
			intersection_point0_(rsedge.intersection_point0_),
			intersection_point1_(rsedge.intersection_point1_),
			singular_(rsedge.singular_)
	{
	}


	template <typename T>
	TRSEdge<T>::TRSEdge(TRSVertex<T>* vertex1,
			TRSVertex<T>* vertex2,
			TRSFace<T>* face1,
			TRSFace<T>* face2,
			const TVector3<T>& center_of_torus,
			const T& radius_of_torus,
			const TAngle<T>& phi,
			const TCircle3<T>& circle1,
			const TCircle3<T>& circle2,
			const TVector3<T>& intersection_point1,
			const TVector3<T>& intersection_point2,
			bool singular,
			Index index)
		throw()
		: GraphEdge< TRSVertex<T>,TRSFace<T> >(vertex1,vertex2,face1,face2,index),
			center_of_torus_(center_of_torus),
			radius_of_torus_(radius_of_torus),
			phi_(phi),
			circle0_(circle1),
			circle1_(circle2),
			intersection_point0_(intersection_point1),
			intersection_point1_(intersection_point2),
			singular_(singular)
	{
	}


	template <typename T>
	TRSEdge<T>::~TRSEdge()
		throw()
	{
	}


	template <typename T>
	void TRSEdge<T>::set(const TRSEdge<T>& rsedge, bool deep)
		throw()
	{
		if (deep)
		{
			vertex_[0] = rsedge.vertex_[0];
			vertex_[1] = rsedge.vertex_[1];
			face_[0] = rsedge.face_[0];
			face_[1] = rsedge.face_[1];
		}
		else
		{
			vertex_[0] = NULL;
			vertex_[1] = NULL;
			face_[0] = NULL;
			face_[1] = NULL;
		}
		center_of_torus_ = rsedge.center_of_torus_;
		radius_of_torus_ = rsedge.radius_of_torus_;
		phi_ = rsedge.phi_;
		circle0_ = rsedge.circle0_;
		circle1_ = rsedge.circle1_;
		intersection_point0_ = rsedge.intersection_point0_;
		intersection_point1_ = rsedge.intersection_point1_;
		singular_ = rsedge.singular_;
		index_ = rsedge.index_;
	}


	template <typename T>
	void TRSEdge<T>::set(TRSVertex<T>* vertex0,
			TRSVertex<T>* vertex1,
			TRSFace<T>* face0,
			TRSFace<T>* face1,
			const TVector3<T>& center_of_torus,
			const T& radius_of_torus,
			const TAngle<T>& phi,
			const TCircle3<T>& circle0,
			const TCircle3<T>& circle1,
			const TVector3<T>& intersection_point0,
			const TVector3<T>& intersection_point1,
			bool singular,
			Index index)
		throw()
	{
		vertex_[0] = vertex0;
		vertex_[1] = vertex1;
		face_[0] = face0;
		face_[1] = face1;
		center_of_torus_ = center_of_torus;
		radius_of_torus_ = radius_of_torus;
		phi_ = phi;
		circle0_ = circle0;
		circle1_ = circle1;
		intersection_point0_ = intersection_point0;
		intersection_point1_ = intersection_point1;
		singular_ = singular;
		index_ = index;
	}


	template <typename T>
	void TRSEdge<T>::setCenterOfTorus(const TVector3<T>& center)
		throw()
	{
		center_of_torus_ = center;
	}


	template <typename T>
	TVector3<T> TRSEdge<T>::getCenterOfTorus() const
		throw()
	{
		return center_of_torus_;
	}


	template <typename T>
	void TRSEdge<T>::setMajorRadiusOfTorus(const T& radius)
		throw()
	{
		radius_of_torus_ = radius;
	}


	template <typename T>
	T TRSEdge<T>::getMajorRadiusOfTorus() const
		throw()
	{
		return radius_of_torus_;
	}


	template <typename T>
	void TRSEdge<T>::setPhi(const TAngle<T>& phi)
		throw()
	{
		phi_ = phi;
	}


	template <typename T>
	TAngle<T> TRSEdge<T>::getPhi() const
		throw()
	{
		return phi_;
	}


	template <typename T>
	void  TRSEdge<T>::setContactCircle(Position i, const TCircle3<T>&  circle)
		throw()
	{
		if (i == 0)
		{
			circle0_ = circle;
		}
		else
		{
			circle1_ = circle;
		}
	}


	template <typename T>
	TCircle3<T> TRSEdge<T>::getContactCircle(Position i) const
		throw()
	{
		if (i == 0)
		{
			return circle0_;
		}
		else
		{
			return circle1_;
		}
	}


	template <typename T>
	void TRSEdge<T>::setIntersectionPoint(Position i, const TVector3<T>& point)
		throw()
	{
		if (i == 0)
		{
			intersection_point0_ = point;
		}
		else
		{
			intersection_point1_ = point;
		}
	}


	template <typename T>
	TVector3<T> TRSEdge<T>::getIntersectionPoint(Position i) const
		throw(Exception::GeneralException)
	{
		if (singular_ == false)
		{
			throw Exception::GeneralException(__FILE__, __LINE__);
		}
		if (i == 0)
		{
			return intersection_point0_;
		}
		else
		{
			return intersection_point1_;
		}
	}


	template <typename T>
	void TRSEdge<T>::setSingular(bool singular)
		throw()
	{
		singular_ = singular;
	}


	template <typename T>
	bool TRSEdge<T>::operator == (const TRSEdge<T>& rsedge) const
		throw()
	{
		if ((vertex_[0]->atom_ != rsedge.vertex_[0]->atom_) &&
				(vertex_[0]->atom_ != rsedge.vertex_[1]->atom_)    )
		{
			return false;
		}
		if ((vertex_[1]->atom_ != rsedge.vertex_[0]->atom_) &&
				(vertex_[1]->atom_ != rsedge.vertex_[1]->atom_)    )
		{
			return false;
		}
		if ((face_[0] != rsedge.face_[0]) && (face_[0] != rsedge.face_[1]))
		{
			return false;
		}
		if ((face_[1] != rsedge.face_[0]) && (face_[1] != rsedge.face_[1]))
		{
			return false;
		}
		return true;
	}


	template <typename T>
	bool TRSEdge<T>::similar(const TRSEdge<T>& rsedge) const
		throw()
	{
		if ((vertex_[0]->atom_ != rsedge.vertex_[0]->atom_) &&
				(vertex_[0]->atom_ != rsedge.vertex_[1]->atom_)    )
		{
			return false;
		}
		if ((vertex_[1]->atom_ != rsedge.vertex_[0]->atom_) &&
				(vertex_[1]->atom_ != rsedge.vertex_[1]->atom_)    )
		{
			return false;
		}
		return true;
	}


	template <typename T>
	bool TRSEdge<T>::operator != (const TRSEdge<T>& rsedge) const
		throw()
	{
		return (bool)(!(*this == rsedge));
	}


	template <typename T>
	bool TRSEdge<T>::isSingular() const
		throw()
	{
		return singular_;
	}


	template <typename T>
	bool TRSEdge<T>::isFree() const
		throw()
	{
		return (face_[0] == NULL);
	}




} // namespace BALL

#endif // BALL_STRUCTURE_RSEDGE_H
