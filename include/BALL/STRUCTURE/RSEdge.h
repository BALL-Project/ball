// $Id: RSEdge.h,v 1.9 2001/04/03 13:16:40 strobel Exp $

#ifndef BALL_STRUCTURE_RSEDGE_H
#define BALL_STRUCTURE_RSEDGE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
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

	/** Generic RSEdge Class.	\\
      {\bf Definition:} \URL{BALL/STRUCTURE/RSEdge.h}  \\
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
			: vertex0_(NULL), vertex1_(NULL), face0_(NULL), face1_(NULL),
				center_of_torus_(), radius_of_torus_((T)0), phi_(),
				circle0_(), circle1_(), intersection_point0_(), intersection_point1_(),
				singular_(false), index_(-1)
		{
		}

		/**	Copy constructor.
				Create a new RSEdge object from another.
				@param rsface the RSEdge object to be copied
				@param bool ignored - just for interface consistency
		*/
		TRSEdge(const TRSEdge<T>& rsedge,bool /* deep */ = true)
			: vertex0_(rsedge.vertex0_), vertex1_(rsedge.vertex1_),
				face0_(rsedge.face0_), face1_(rsedge.face1_),
				center_of_torus_(rsedge.center_of_torus_),
				radius_of_torus_(rsedge.radius_of_torus_),
				phi_(rsedge.phi_),
				circle0_(rsedge.circle0_), circle1_(rsedge.circle1_),
				intersection_point0_(rsedge.intersection_point0_),
				intersection_point1_(rsedge.intersection_point1_),
				singular_(rsedge.singular_),
				index_(rsedge.index_)
		{
		}

		/**	Detailled constructor.
				Create a new RSEdge object from a lot of nice objects
				@param	vertex1	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face1	assigned to the first face
				@param	face2	assigned to the second face
				@param	center_of_torus assigned to the center of the torus
				@param	radius_of_torus assigned to the radius of the torus
				@param	phi assigned to th angle
				@param	circle1	assigned to the first contact circle
				@param	circle2	assigned to the second contact circle
				@param	intersection_point1 assigned to the first intersection point (in singular case)
				@param	intersection_point2 assigned to the second intersection point (in singular case)
				@param	singular
				@param	index assigned to the index
		*/
		TRSEdge(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, TRSFace<T>* face1, TRSFace<T>* face2,
						const TVector3<T>& center_of_torus, const T& radius_of_torus, const TAngle<T>& phi,
						const TCircle3<T>& circle1, const TCircle3<T>& circle2,
						const TVector3<T>& intersection_point1, const TVector3<T>& intersection_point2,
						const bool singular, Index index)
			: vertex0_(vertex1), vertex1_(vertex2), face0_(face1), face1_(face2),
				center_of_torus_(center_of_torus), radius_of_torus_(radius_of_torus),
				phi_(phi), circle0_(circle1), circle1_(circle2),
				intersection_point0_(intersection_point1), intersection_point1_(intersection_point2),
				singular_(singular), index_(index)
		{
		}

		/**	Destructor.
				Destructs the RSEdge object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TRSEdge()
		{
		}

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another RSEdge.
				@param rsedge	the RSEdge object to assign from
				@param deep ignored
		*/
		void set(const TRSEdge& rsedge, bool /* deep */ = true)
		{
			vertex0_ = rsedge.getVertex(0);
			vertex1_ = rsedge.getVertex(1);
			face0_ = rsedge.getFace(0);
			face1_ = rsedge.getFace(1);
			center_of_torus_ = rsedge.getCenterOfTorus();
			radius_of_torus_ = rsedge.getMajorRadiusOfTorus();
			phi_ = rsedge.getPhi();
			circle0_ = rsedge.getContactCircle(0);
			circle1_ = rsedge.getContactCircle(1);
			intersection_point0_ = rsedge.getIntersectionPoints(0);
			intersection_point1_ = rsedge.getIntersectionPoints(1);
			singular_ = rsedge.isSingular();
			index_ = rsedge.getIndex();
		}

		/**	Assign from a lot of nice objects
				@param	vertex1	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face1	assigned to the first face
				@param	face2	assigned to the second face
				@param	center_of_torus assigned to the center of the torus
				@param	radius_of_torus assigned to the radius of the torus
				@param	phi assigned to th angle
				@param	circle1	assigned to the first contact circle
				@param	circle2	assigned to the second contact circle
				@param	intersection_point1 assigned to the first intersection point (in singular case)
				@param	intersection_point2 assigned to the second intersection point (in singular case)
				@param	singular
				@param	index assigned to the index
		*/
		void set(TRSVertex<T>* vertex0, TRSVertex<T>* vertex1, TRSFace<T>* face0, TRSFace<T>* face1,
						 const TVector3<T>& center_of_torus, const T& radius_of_torus, const TAngle<T>& phi,
						 const TCircle3<T>& circle0, const TCircle3<T>& circle1,
						 const TVector3<T>& intersection_point0, const TVector3<T>& intersection_point1,
						 const bool singular, Index index)
		{
			vertex0_ = vertex0; vertex1_ = vertex1; face0_ = face0; face1_ = face1;
			center_of_torus_ = center_of_torus; radius_of_torus_ = radius_of_torus;
			phi_ = phi; circle0_ = circle0; circle1_ = circle1;
			intersection_point0_ = intersection_point0; intersection_point1_ = intersection_point1;
			singular_ = singular; index_ = index;
		}

		/**	@name	Accessors
		*/
		//@{

		/** Change the indices of the two rsvertices of the rsedge.
				@param i the first vertex is changed if i = 0, the second otherwise
				@param vertex the new vertex
		*/
		void setVertices(const Position i, TRSVertex<T>* vertex)
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

		/** Return one of the two rsvertices of the rsedge.
				@return RSVertex the first rsvertex if i = 0,
												 the second rsvertex otherwise
		*/
		TRSVertex<T>* getVertex(const Position i)
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

		/** Change one of the two rsfaces of the rsedge.
				@param i the first face is changed if i = 0, the second otherwise
				@param face a pointer to the the new face
		*/
		void  setFaces(const Position i, TRSFace<T>* face)
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

		/** Return one of the two rsfaces of the rsedge.
				@return Index the first rsface if i = 0, the second rsface otherwise
		*/
		TRSFace<T>* getFace(const Position i)
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

		/** Change the center of the torus described by the probe sphere when it rolls over the rsedge.
				@param center the new center
		*/
		void setCenterOfTorus(const TVector3<T>& center)
		{
			center_of_torus_ = center;
		}

		/** Return the center of the torus described by the probe sphere when it rolls over the rsedge.
				@return TVector3<T> the center of the torus
		*/
		TVector3<T> getCenterOfTorus()
		{
			return center_of_torus_;
		}

		/** Change the major radius of the torus described by the probe sphere when it rolls over the rsedge.
				@param radius the new radius
		*/
		void setMajorRadiusOfTorus(const T& radius)
		{
			radius_of_torus_ = radius;
		}

		/** Return the  major radius of the torus described by the probe sphere when it rolls over the rsedge.
				@return TVector3<T> the center of the torus
		*/
		T getMajorRadiusOfTorus()
		{
			return radius_of_torus_;
		}

		/** Change the rotation angle between the starting and ending position of the probe sphere when it rolls
							 over the rsedge.
				@param phi the new rotation angle
		*/
		void setPhi(const TAngle<T>& phi)
		{
			phi_ = phi;
		}

		/** Return the rotation angle between the starting and ending position of the probe sphere when it rolls
							 over the rsedge.
				@return TVector3<T> the center of the torus
		*/
		TAngle<T> getPhi()
		{
			return phi_;
		}

		/** Change one of the two contact circles of the probe sphere with the two rsvertices.
				@param i the first contact circle is changed if i = 0, the second otherwise
				@param circle the new contact circle
		*/
		void  setContactCircle(const Position i, const TCircle3<T>&  circle)
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

		/** Return the contact circle of the probe sphere with the one of two rsvertices
				@return TCircle3<T> the contact circle with the first rsvertex if i = 0,
														the contact circle with the second rsvertex otherwise
		*/
		TCircle3<T> getContactCircle(const Position i)
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

		/** Change one of the intersection points of the probe sphere with the rsedge.
				@param i the first intersection point is changed if i = 0, the second otherwise
				@param point the new intersection point
		*/
		void setIntersectionPointsFaces(const Position i, const TVector3<T>& point)
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

		/** Return one of the intersection points of the probe sphere with the rsedge.
				@return TVector3<T> the intersection point near to the first rsvertex if i = 0,
														the intersection point near to the second rsvertex otherwise
		*/
		TVector3<T> getIntersectionPoint(const Position i)
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

		/** Change singular
		*/
		void setSingular(const bool singular)
		{
			singular_ = singular;
		}

		/** Change the index of the rsedge.
				@param index the new index
		*/
		void setIndex(Index index)
		{
			index_ = index;
		}

		/** Return the index of the rsedge.
				@return Index, the index of the rsedge
		*/
		Index getIndex()
		{
			return index_;
		}
		
		/** Return a pointer to the other face of the rsedge.
				@param TRSFace<T>* one of the faces of the rsedge
				@return TRSFace<T>* the other face
		*/
		TRSFace<T>* other(TRSFace<T>* face)
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


		/** Substitute a rsvertex by an other one.
				@param old_vertex the vertex that has to be substituted
				@param new_vertex the new vertex
				@return bool, {\bf true}, if the vertex can be substituted, {\bf false} otherwise
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
			return false;
		}
		//@}


		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all vertices and all faces are equal modulo order,
											{\bf false} otherwise
		*/
		bool operator == (const TRSEdge& rsedge) const
		{
			if ((vertex0_->similar(*rsedge.getVertex(0)) == false) &&
					(vertex0_->similar(*rsedge.getVertex(1)) == false)    )
				{
					return false;
				}
			if ((vertex1_->similar(*rsedge.getVertex(0)) == false) &&
					(vertex1_->similar(*rsedge.getVertex(1)) == false)    )
				{
					return false;
				}
			if ((face0_ != rsedge.getFace(0)) && (face0_ != rsedge.getFace(1)))
				{
					return false;
				}
			if ((face1_ != rsedge.getFace(0)) && (face1_ != rsedge.getFace(1)))
				{
					return false;
				}
			return true;
		}

		/**	similar.
				@return bool, {\bf true} if all vertices are equal modulo order, {\bf false} otherwise
		*/
		bool similar(TRSEdge& rsedge) const
		{
			if ((vertex0_->similar(*rsedge.getVertex(0)) == false) &&
					(vertex0_->similar(*rsedge.getVertex(1)) == false)    )
				{
					return false;
				}
			if ((vertex1_->similar(*rsedge.getVertex(0)) == false) &&
					(vertex1_->similar(*rsedge.getVertex(1)) == false)    )
				{
					return false;
				}
			return true;
		}

		/**	Inequality operator.
				@return bool, {\bf true} if the atoms differ, {\bf false} otherwise
		*/
		bool operator != (const TRSEdge& rsedge) const
		{
			return (bool)(!(*this == rsedge));
		}
		
		/** isSingular
				@return bool, {\bf true} if the rsedge is singular, {\bf false} otherwise
		*/
		bool isSingular()
		{
			return singular_;
		}
		
		/** isFree
				@return bool, {\bf true} if the rsedge is free, {\bf false} otherwise
		*/
		bool isFree()
		{
			return (face0_ == NULL);
		}

		//@}


		protected:

		TRSVertex<T>* vertex0_;
		TRSVertex<T>* vertex1_;
		TRSFace<T>* face0_;
		TRSFace<T>* face1_;
		TVector3<T> center_of_torus_;
		T radius_of_torus_;
		TAngle<T> phi_;
		TCircle3<T> circle0_;
		TCircle3<T> circle1_;
		TVector3<T> intersection_point0_;
		TVector3<T> intersection_point1_;
		bool singular_;
		Index index_;

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

} // namespace BALL

#endif // BALL_STRUCTURE_RSEDGE_H
