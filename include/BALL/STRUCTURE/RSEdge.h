// $Id: RSEdge.h,v 1.16 2001/07/15 17:03:24 oliver Exp $

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

namespace BALL
{

	template <typename T>
	class TRSFace;

	template <typename T>
	class TRSVertex;

	/** Generic RSEdge Class.	
      {\bf Definition:} \URL{BALL/STRUCTURE/RSEdge.h} 
	*/
	template <class T>
	class TRSEdge
	{
		public:

		BALL_CREATE(TRSEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new RSEdge object. All components are
				initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TRSEdge()
			throw();

		/**	Copy constructor.
				Create a new RSEdge object from another.
				@param	rsface	the RSEdge object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TRSEdge(const TRSEdge<T>& rsedge, bool  = true)
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
				const bool singular,
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
				@param	deep		ignored
		*/
		void set(const TRSEdge& rsedge, bool = true)
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
		void set(TRSVertex<T>* vertex0,
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

		/** Set one of the two RSVertices of the RSEdge.
				@param	i				the first vertex is changed if i = 0, the second
												otherwise
				@param	vertex	a pointer to the new vertex
		*/
		void setVertex(Position i, TRSVertex<T>* vertex)
			throw();

		/** Return one of the two RSVertices of the RSEdge.
				@param	i
				@return	TRSVertex<T>*	a pointer to the first RSVertex if i = 0,
															a pointer to the second RSVertex otherwise
		*/
		TRSVertex<T>* getVertex(Position i)
			throw();

		/** Return one of the two RSFaces of the RSEdge.
				@param	i
				@return	TRSFace<T>*	a pointer to the first RSFace if i = 0,
														a pointer to the second RSFace otherwise
		*/
		TRSFace<T>* getFace(Position i)
			throw();

		/** Set one of the two RSFaces of the RSEdge.
				@param	i			change the first face, if i = 0, the second otherwise
				@param	face	a pointer to the new RSFace
		*/
		void setFace(Position i, TRSFace<T>* face)
			throw();

		/** Set the center of the torus.
				Set the center of the torus described by the probe sphere when it
				rolls over the RSEdge.
				@param	center	the new center
		*/
		void setCenterOfTorus(const TVector3<T>& center)
			throw();

		/** Return the center of the torus.
				Return the center of the torus described by the probe sphere when
				it rolls over the RSEdge.
				@return	TVector3<T>	the center of the torus
		*/
		TVector3<T> getCenterOfTorus()
			throw();

		/** Set the major radius of the torus.
				Set the major radius of the torus described by the probe sphere when
				it rolls over the RSEdge.
				@param	radius	the new radius
		*/
		void setMajorRadiusOfTorus(const T& radius)
			throw();

		/** Return the  major radius of the torus.
				Return the  major radius of the torus described by the probe sphere
				when it rolls over the RSEdge.
				@return	TVector3<T>	the major radius of the torus
		*/
		T getMajorRadiusOfTorus()
			throw();

		/** Set the rotation angle.
				Set the rotation angle between the starting and ending position of the
				probe sphere when it rolls over the RSEdge.
				@param	phi	the new rotation angle
		*/
		void setPhi(const TAngle<T>& phi)
			throw();

		/** Return the rotation angle.
				Return the rotation angle between the starting and ending position of
				the probe sphere when it rolls over the RSEdge.
				@return	TAngle<T>	the rotation angle
		*/
		TAngle<T> getPhi()
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
				Return one of the the contact circles of the probe sphere with the two
				RSVertices
				@param	i
				@return	TCircle3<T>	the contact circle with the first RSVertex
														if i = 0, the contact circle with the second
														RSVertex otherwise
		*/
		TCircle3<T> getContactCircle(Position i)
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
				with the RSEdge. If the RSEdge not is singular, an exception is thrown.
				@return	TVector3<T>	the intersection point near to the first RSVertex
														if i = 0, the intersection point near to the
														second RSVertex otherwise
		*/
		TVector3<T> getIntersectionPoint(Position i)
			throw(Exception::GeneralException);

		/** Set singular
		*/
		void setSingular(bool singular)
			throw();

		/** Set the index of the RSEdge.
				@param	index	the new index
		*/
		void setIndex(Index index)
			throw();

		/** Return the index of the RSEdge.
				@return	Index	the index of the RSEdge
		*/
		Index getIndex() const
			throw();
		
		/** Return a pointer to the other face of the RSEdge.
				If the given RSFace is not neighboured to the RSEdge, an exception is
				thrown.
				@param	TRSFace<T>*	one of the faces of the RSEdge
				@return	TRSFace<T>*	the other face
		*/
		TRSFace<T>* other(TRSFace<T>* face)
			throw(Exception::GeneralException);

		/** Substitute a RSVertex by an other one.
				@param	old_vertex	the vertex that has to be substituted
				@param	new_vertex	the new vertex
				@return	bool				{\bf true}, if the vertex can be substituted,
														{\bf false} otherwise
		*/
		bool substituteVertex(TRSVertex<T>* old_vertex,
				TRSVertex<T>* new_vertex)
			throw();
		//@}


		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all vertices are similar and all faces
											are equal modulo order, {\bf false} otherwise
		*/
		bool operator == (const TRSEdge& rsedge) const
			throw();

		/**	similar.
				@return	bool	{\bf true} if all vertices are similar modulo order,
											{\bf false} otherwise
		*/
		bool similar(const TRSEdge& rsedge) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false} if all vertices are similar and all faces
											are equal modulo order, {\bf true} otherwise
		*/
		bool operator != (const TRSEdge& rsedge) const
			throw();
		
		/** isSingular
				@return	bool	{\bf true} if the RSEdge is singular,
											{\bf false} otherwise
		*/
		bool isSingular()
			throw();
		
		/** isFree
				@return	bool	{\bf true} if the RSEdge is free, {\bf false} otherwise
		*/
		bool isFree()
			throw();

		//@}


		protected:

		/*_ The first RSVertex of the RSEdge
		*/
		TRSVertex<T>* vertex0_;
		/*_ The second RSVertex of the RSEdge
		*/
		TRSVertex<T>* vertex1_;
		/*_ The first RSFace of the RSEdge
		*/
		TRSFace<T>* face0_;
		/*_ The second RSFace of the RSEdge
		*/
		TRSFace<T>* face1_;
		/*_ The center of the torus described by the probe when ir rolls over
				the RSEdge
		*/
		TVector3<T> center_of_torus_;
		/*_ The major radius of the torus described by the probe when ir rolls over
				the RSEdge
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
		/*_ The index of the RSEdge
		*/
		Index index_;

	};

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
			@exception NotImplemented
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
				<< "([" << (rsedge.getVertex(0) == NULL ? -2 : rsedge.getVertex(0)->getIndex()) << ' '
				<<				 (rsedge.getVertex(1) == NULL ? -2 : rsedge.getVertex(1)->getIndex()) << "] "
				<< "[" << (rsedge.getFace(0) == NULL ? -2 : rsedge.getFace(0)->getIndex()) << ' '
				<<				(rsedge.getFace(1) == NULL ? -2 : rsedge.getFace(1)->getIndex()) << "] "
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
				s << TVector3<T>::getZero() << ' ' << TVector3<T>::getZero() << " false)";
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
		: vertex0_(NULL),
			vertex1_(NULL),
			face0_(NULL),
			face1_(NULL),
			center_of_torus_(),
			radius_of_torus_((T)0),
			phi_(),
			circle0_(),
			circle1_(),
			intersection_point0_(),
			intersection_point1_(),
			singular_(false),
			index_(-1)
	{
	}


	template <typename T>
	TRSEdge<T>::TRSEdge(const TRSEdge<T>& rsedge, bool)
		throw()
		: vertex0_(rsedge.vertex0_),
			vertex1_(rsedge.vertex1_),
			face0_(rsedge.face0_),
			face1_(rsedge.face1_),
			center_of_torus_(rsedge.center_of_torus_),
			radius_of_torus_(rsedge.radius_of_torus_),
			phi_(rsedge.phi_),
			circle0_(rsedge.circle0_),
			circle1_(rsedge.circle1_),
			intersection_point0_(rsedge.intersection_point0_),
			intersection_point1_(rsedge.intersection_point1_),
			singular_(rsedge.singular_),
			index_(rsedge.index_)
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
			const bool singular,
			Index index)
		throw()
		: vertex0_(vertex1),
			vertex1_(vertex2),
			face0_(face1),
			face1_(face2),
			center_of_torus_(center_of_torus),
			radius_of_torus_(radius_of_torus),
			phi_(phi),
			circle0_(circle1),
			circle1_(circle2),
			intersection_point0_(intersection_point1),
			intersection_point1_(intersection_point2),
			singular_(singular),
			index_(index)
	{
	}


	template <typename T>
	TRSEdge<T>::~TRSEdge()
		throw()
	{
	}


	template <typename T>
	void TRSEdge<T>::set(const TRSEdge& rsedge, bool)
		throw()
	{
		vertex0_ = rsedge.vertex0_;
		vertex1_ = rsedge.vertex1_;
		face0_ = rsedge.face0_;
		face1_ = rsedge.face1_;
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
		vertex0_ = vertex0;
		vertex1_ = vertex1;
		face0_ = face0;
		face1_ = face1;
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
	void TRSEdge<T>::setVertex(Position i, TRSVertex<T>* vertex)
		throw()
	{
		if (i == 0)
		{
			vertex0_ = vertex;
		}
		else
		{
			vertex1_ = vertex;
		}
	}


	template <typename T>
	TRSVertex<T>* TRSEdge<T>::getVertex(Position i)
		throw()
	{
		if (i == 0)
		{
			return vertex0_;
		}
		else
		{
			return vertex1_;
		}
	}


	template <typename T>
	TRSFace<T>* TRSEdge<T>::getFace(Position i)
		throw()
	{
		if (i == 0)
		{
			return face0_;
		}
		else
		{
			return face1_;
		}
	}


	template <typename T>
	void TRSEdge<T>::setFace(Position i, TRSFace<T>* face)
		throw()
	{
		if (i == 0)
		{
			face0_ = face;
		}
		else
		{
			face1_ = face;
		}
	}


	template <typename T>
	void TRSEdge<T>::setCenterOfTorus(const TVector3<T>& center)
		throw()
	{
		center_of_torus_ = center;
	}


	template <typename T>
	TVector3<T> TRSEdge<T>::getCenterOfTorus()
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
	T TRSEdge<T>::getMajorRadiusOfTorus()
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
	TAngle<T> TRSEdge<T>::getPhi()
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
	TCircle3<T> TRSEdge<T>::getContactCircle(Position i)
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
	TVector3<T> TRSEdge<T>::getIntersectionPoint(Position i)
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
	void TRSEdge<T>::setIndex(Index index)
		throw()
	{
		index_ = index;
	}


	template <typename T>
	Index TRSEdge<T>::getIndex() const
		throw()
	{
		return index_;
	}


	template <typename T>
	TRSFace<T>* TRSEdge<T>::other(TRSFace<T>* face)
		throw(Exception::GeneralException)
	{
		if (face0_ == face)
		{
			return face1_;
		}
		else
		{
			if (face1_ == face)
			{
				return face0_;
			}
			else
			{
				throw Exception::GeneralException(__FILE__, __LINE__);
			}
		}
	}


	template <typename T>
	bool TRSEdge<T>::substituteVertex(TRSVertex<T>* old_vertex,
			 TRSVertex<T>* new_vertex)
		throw()
	{
		if (vertex0_ == old_vertex)
		{
			vertex0_ = new_vertex;
		}
		else
		{
			if (vertex1_ == old_vertex)
			{
				vertex1_ = new_vertex;
			}
			else
			{
				return false;
			}
		}
		return true;
	}


	template <typename T>
	bool TRSEdge<T>::operator == (const TRSEdge& rsedge) const
		throw()
	{
		if ((vertex0_->similar(*rsedge.vertex0_) == false) &&
				(vertex0_->similar(*rsedge.vertex1_) == false)    )
		{
			return false;
		}
		if ((vertex1_->similar(*rsedge.vertex0_) == false) &&
				(vertex1_->similar(*rsedge.vertex1_) == false)    )
		{
			return false;
		}
		if ((face0_ != rsedge.face0_) && (face0_ != rsedge.face1_))
		{
			return false;
		}
		if ((face1_ != rsedge.face0_) && (face1_ != rsedge.face1_))
		{
			return false;
		}
		return true;
	}


	template <typename T>
	bool TRSEdge<T>::similar(const TRSEdge& rsedge) const
		throw()
	{
		if ((vertex0_->similar(*rsedge.vertex0_) == false) &&
				(vertex0_->similar(*rsedge.vertex1_) == false)    )
		{
			return false;
		}
		if ((vertex1_->similar(*rsedge.vertex0_) == false) &&
				(vertex1_->similar(*rsedge.vertex1_) == false)    )
		{
			return false;
		}
		return true;
	}


	template <typename T>
	bool TRSEdge<T>::operator != (const TRSEdge& rsedge) const
		throw()
	{
		return (bool)(!(*this == rsedge));
	}

	template <typename T>
	bool TRSEdge<T>::isSingular()
		throw()
	{
		return singular_;
	}


	template <typename T>
	bool TRSEdge<T>::isFree()
		throw()
	{
		return (face0_ == NULL);
	}




} // namespace BALL

#endif // BALL_STRUCTURE_RSEDGE_H
