// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: solventExcludedSurface.h,v 1.33 2003/02/21 16:06:57 anhi Exp $

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#define BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H

#ifndef BALL_STRUCTURE_SESEDGE_H
#	include <BALL/STRUCTURE/SESEdge.h>
#endif

#ifndef BALL_STRUCTURE_SESFACE_H
#	include <BALL/STRUCTURE/SESFace.h>
#endif

#ifndef BALL_STRUCTURE_SESVERTEX_H
#	include <BALL/STRUCTURE/SESVertex.h>
#endif

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#	include <BALL/STRUCTURE/reducedSurface.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_VECTOR4_H
#	include <BALL/MATHS/vector4.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#	include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#include <vector>
#include <list>


namespace BALL
{

	class SESComputer;
	class SESSingularityCleaner;
	class TriangulatedSES;
	class SESTriangulator;

	/** Generic SolventExcludedSurface Class.
			<b>Definition:</b> BALL/STRUCTURE/solventExcludedSurface.h
	*/
	class SolventExcludedSurface
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class SESComputer
					\item class SESSingularityCleaner
					\item class TriangulatedSES
					\item class SESTriangulator
				\end{itemize}
		*/
		friend class SESComputer;
		friend class SESSingularityCleaner;
		friend class TriangulatedSES;
		friend class SESTriangulator;

		BALL_CREATE(SolventExcludedSurface)

  	/**	@name	Type definitions
		*/
		//@{

		typedef std::vector<SESVertex*>::iterator
						VertexIterator;
		typedef std::vector<SESVertex*>::const_iterator
						ConstVertexIterator;
		typedef std::vector<SESEdge*>::iterator
						EdgeIterator;
		typedef std::vector<SESEdge*>::const_iterator
						ConstEdgeIterator;
		typedef std::list<SESEdge*>::iterator
						SingularEdgeIterator;
		typedef std::list<SESEdge*>::const_iterator
						ConstSingularEdgeIterator;
		typedef std::vector<SESFace*>::iterator
						ContactFaceIterator;
		typedef std::vector<SESFace*>::const_iterator
						ConstContactFaceIterator;
		typedef std::vector<SESFace*>::iterator
						SphericFaceIterator;
		typedef std::vector<SESFace*>::const_iterator
						ConstSphericFaceIterator;
		typedef std::vector<SESFace*>::iterator
						ToricFaceIterator;
		typedef std::vector<SESFace*>::const_iterator
						ConstToricFaceIterator;

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SolventExcludedSurface object.
		*/
		SolventExcludedSurface()
			throw();

		/**	Copy constructor.
				Create a new SolventExcludedSurface object from another.
				@param	ses		the SolventExcludedSurface object to be copied
				@param	bool	ignored - just for interface consistency
		*/
		SolventExcludedSurface
				(const SolventExcludedSurface& ses, bool = false)
			throw();

		/** Detailed constructor
		*/
		SolventExcludedSurface(ReducedSurface* reduced_surface)
			throw();

		/**	Destructor.
				As there are no dynamic	data structures, nothing happens.
		*/
		virtual ~SolventExcludedSurface()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		void clear();

		void clean(const double& density);

		/**	Computes the solvent excluded surface from a ReducedSurface object
		*/
		void compute()
			throw(Exception::GeneralException);

		void splitSphericFaces()
			throw();

		bool check()
			throw();

		//@}
		/**	@name	External Iterators
		*/
		//@{

		VertexIterator beginVertex()
			throw();
		ConstVertexIterator beginVertex() const
			throw();
		VertexIterator endVertex()
			throw();
		ConstVertexIterator endVertex() const
			throw();

		EdgeIterator beginEdge()
			throw();
		ConstEdgeIterator beginEdge() const
			throw();
		EdgeIterator endEdge()
			throw();
		ConstEdgeIterator endEdge() const
			throw();

		SingularEdgeIterator beginSingularEdge()
			throw();
		ConstSingularEdgeIterator beginSingularEdge() const
			throw();
		SingularEdgeIterator endSingularEdge()
			throw();
		ConstSingularEdgeIterator endSingularEdge() const
			throw();

		ContactFaceIterator beginContactFace()
			throw();
		ConstContactFaceIterator beginContactFace() const
			throw();
		ContactFaceIterator endContactFace()
			throw();
		ConstContactFaceIterator endContactFace() const
			throw();

		SphericFaceIterator beginSphericFace()
			throw();
		ConstSphericFaceIterator beginSphericFace() const
			throw();
		SphericFaceIterator endSphericFace()
			throw();
		ConstSphericFaceIterator endSphericFace() const
			throw();

		ToricFaceIterator beginToricFace()
			throw();
		ConstToricFaceIterator beginToricFace() const
			throw();
		ToricFaceIterator endToricFace()
			throw();
		ConstToricFaceIterator endToricFace() const
			throw();

		//@}

		private:

		void splitSphericFace(Position i)
			throw();

		void deleteSmallToricFace(SESFace* face)
			throw();

		void deleteSmallSingularToricFace(SESFace* face)
			throw();

		bool cleanToricFace(SESFace* face, const double& sqrt_density)
			throw();

		bool cleanSingularToricFace(SESFace* face, const double& sqrt_density)
			throw();

		void cleanVertices()
			throw();

		void cleanEdges()
			throw();

		void cleanContactFaces()
			throw();

		void cleanToricFaces()
			throw();

		void cleanSphericFaces()
			throw();


		protected:

    /*_ the number of vertices of the solvent exluded surface
    */
		Position number_of_vertices_;
    /*_ the vertices of the solvent exluded surface
    */
		::std::vector<SESVertex*> vertices_;
    /*_ the number of edges of the solvent exluded surface
    */
		Position number_of_edges_;
    /*_ the edges of the solvent exluded surface
    */
		::std::vector<SESEdge*> edges_;
    /*_ the number of singular edges of the solvent exluded surface
    */
		Position number_of_singular_edges_;
    /*_ the singular edges of the solvent exluded surface
    */
		::std::list<SESEdge*> singular_edges_;
    /*_ the number of contact faces of the solvent exluded surface
    */
		Position number_of_contact_faces_;
    /*_ the contact faces of the solvent exluded surface
    */
		::std::vector<SESFace*> contact_faces_;
    /*_ the number of toric reentrant faces of the solvent exluded surface
    */
		Position number_of_toric_faces_;
    /*_ the toric reentrant faces of the solvent exluded surface
    */
		::std::vector<SESFace*> toric_faces_;
    /*_ the number of spheric reentrant faces of the solvent exluded surface
    */
		Position number_of_spheric_faces_;
    /*_ the spheric reentrant faces of the solvent exluded surface
    */
		::std::vector<SESFace*> spheric_faces_;
		/*_ the corresponding reduced surface
		*/
		ReducedSurface* reduced_surface_;

	};


	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator <<
			(std::ostream& s, const SolventExcludedSurface& ses);

	//@}



	/** Generic SESComputer Class.
			<b>Definition:</b> BALL/STRUCTURE/solventExcludedSurface.h
	*/
	class SESComputer
	{
		public:

		BALL_CREATE(SESComputer)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESComputer object.
		*/
		SESComputer()
			throw();

		/** Detailed constructor
		*/
		SESComputer(SolventExcludedSurface* ses)
			throw();

		/**	Destructor.
				As there are no dynamic	data structures, nothing happens.
		*/
		virtual ~SESComputer()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Computes the solvent excluded surface
		*/
		void run()
			throw(Exception::GeneralException);

		//@}

		private:

		/*_	@name	SES computation (private)
		*/
		//@{

		void preProcessing()
			throw();

		void get()
			throw();

		void createSphericFace(Position j)
			throw();

		SESVertex* createVertex
			(const TVector3<double>& probe_center,
			 Index index)
			throw();

		void pushVertex
			(SESFace* face,
			 const TSphere3<double>& probe,
			 RSVertex* rsvertex)
			throw();

		SESEdge* createConcaveEdge
			(SESFace* spheric_face,
			 Position p1,
			 Position p2,
			 Index index,
			 const double& radius_of_probe)
			throw();

		void pushConcaveEdge
			(SESFace* face,
			 Position p1,
			 Position p2,
			 const double& radius_of_probe)
			throw();

		SESEdge* createConvexEdge
			(SESFace* toric_face,
			 RSVertex* rsvertex)
			throw();

		void createToricFace(Position i)
			throw();

		void treatSingularToricFace(Position i)
			throw();

		void createFreeToricFace(Position i)
			throw();

		SESVertex* createSingularVertex
				(Position ip,
				 const TVector3<double>& dir,
				 SESFace* face0,
				 SESFace* face1,
				 SESFace* face2,
				 SESEdge* edge0,
				 SESEdge* edge1,
				 SESEdge* edge2)
			throw();

		void updateEdge
				(SESEdge*		edge,
				 SESVertex*	vertex1,
				 SESVertex*	vertex2,
				 bool						is_new)
			throw();

		void getPoint
			(const TVector3<double>& p1,
			 const TVector3<double>& p2,
			 const double&						dist,
			 TVector3<double>&				result)
			throw();

		Index vertexExists(const TVector3<double>& point)
			throw();

		//@}


		protected:

		/*_ a pointer to the solvent excluded surface to compute
		*/
		SolventExcludedSurface* ses_;
		/*_ a HashGrid to store and find the vertices in an efficient way
		*/
		HashGrid3<Index> vertex_grid_;

	};




	/** Generic SESComputer Class.
			<b>Definition:</b> BALL/STRUCTURE/solventExcludedSurface.h
	*/
	class SESSingularityCleaner
	{
		public:

		BALL_CREATE(SESSingularityCleaner)

		typedef std::pair< std::pair<TAngle<double>,Index>,TVector3<double> > Intersection;

		struct ProbeIntersection
		{
			TVector3<double> point[2];
		};

		friend class SESComputer;

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESComputer object.
		*/
		SESSingularityCleaner()
			throw();

		/** Detailed constructor
		*/
		SESSingularityCleaner
				(SolventExcludedSurface*	ses,
				 HashGrid3<Index>*				vertex_grid_)
			throw();

		/**	Destructor.
				As there are no dynamic	data structures, nothing happens.
		*/
		virtual ~SESSingularityCleaner()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Solves the singularities
		*/
		bool run()
			throw(Exception::GeneralException);

		//@}

		private:

		/**	@name	Treatment of singularities (private)
		*/
		//@{

		Index vertexExists(TVector3<double> point)
			throw();

		void treatSingularities()
			throw();

		void getSingularFaces(std::list<SESFace*>& faces)
			throw();

		bool treatFirstCategory()
			throw();

		void treatSecondCategory()
			throw();

		void getFirstCategoryFaces(std::list<SESFace*>& first_category_faces)
			throw();

		void noCut(SESFace* face1, SESFace* face2)
			throw();

		void twoCuts(SESFace* face1, SESFace* face2)
			throw();

		void treatSingularEdge
			(SESEdge*								edge,
			 HashGrid3<Position>&		grid,
			 ::std::list<SESEdge*>&	deletable_edges)
			throw();

		void getIntersectionsOfSingularEdge
			(SESEdge*									edge,
			 const TAngle<double>&							phi,
			 HashGrid3<Position>&			grid,
			 std::list<Intersection>& intersections)
			throw();

		bool getIntersectionPointsAndAngles
			(const TCircle3<double>&	circle,
			 const TVector3<double>&	point,
			 Position				index1,
			 Position				index2,
			 Position				probe_index,
			 TAngle<double>&					phi1,
			 TVector3<double>&				point1,
			 TAngle<double>&					phi2,
			 TVector3<double>&				point2)
			throw();

		bool isIntersection
			 (const TAngle<double>&	 min_phi,
				const TAngle<double>&	 max_phi,
				const TAngle<double>&	 phi,
				const TVector3<double>& middle,
				const TSphere3<double>& probe)
			throw();

		void buildEndEdges
			(SESEdge*												edge,
			 const std::list<Intersection>&	min,
			 const std::list<Intersection>&	max,
			 SESVertex*&										vertex1,
			 SESVertex*&										vertex2,
			 Index&													actual_min,
			 Index&													actual_max)
			throw();

		void buildEdge
			(SESEdge*								edge,
			 Index									face1,
			 Index&									face2,
			 Index									end,
			 SESVertex*&						vertex,
			 const HashSet<Index>&	indices,
			 bool										minimum)
			throw();

		void getExtrema
				(const std::list<Intersection>& intersections,
				 std::list<Intersection>& min,
				 std::list<Intersection>& max)
			throw();

		void buildEndEdge
			(SESEdge*												edge,
			 const std::list<Intersection>&	extrema,
			 SESVertex*&										vertex,
			 Index&													actual_extremum,
			 bool														min)
			throw();

		bool probeIntersection
			 (Index		 face1,
				Index		 face2,
				Index		 face3,
				TVector3<double>& point1,
				TVector3<double>& point2)
			throw();

		void sort
				(SESFace* face1,
				 SESFace* face2,
				 std::vector<SESEdge*>& sesedge1,
				 std::vector<SESEdge*>& sesedge2,
				 std::vector<SESVertex*>& sesvertex1,
				 std::vector<SESVertex*>& sesvertex2)
			throw();

		void sort
			 (Index		u1, Index		u2, Index		u3,
				Index&	s1,	Index&	s2, Index&	s3)
			throw();

		//@}


		protected:

		/*_ a pointer to the solvent excluded surface to compute
		*/
		SolventExcludedSurface* ses_;
		/*_ a pointer to a HashGrid to store and find the vertices
				in an efficient way
		*/
		HashGrid3<Index>* vertex_grid_;
		/*_ for each triple of probe spheres its intersections
		*/
		HashMap< Position,
						 HashMap< Position,
											HashMap< Position,
															 ProbeIntersection*
														 >
										>
					 > probe_intersections_;

	};


} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
