// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSES.h,v 1.15 2003/03/14 11:50:05 sturm Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSES_H
#define BALL_STRUCTURE_TRIANGULATEDSES_H

#ifndef BALL_TRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventExcludedSurface.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLE_H
#	include <BALL/STRUCTURE/triangle.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEEDGE_H
#	include <BALL/STRUCTURE/triangleEdge.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEPOINT_H
#	include <BALL/STRUCTURE/trianglePoint.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
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

#include <list>
#include <vector>

namespace BALL
{
    /**  \addtogroup  Surface
     *  @{
     */
	class SESTriangulator;

	/** Generic TriangulatedSES Class.
			 \par
			
			 \par
	*/
	class TriangulatedSES	:	public TriangulatedSurface
	{
		
		public:

		/** @name Class friends

					- class SESTriangulator
				
		*/
		friend class SESTriangulator;

		BALL_CREATE(TriangulatedSES)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSES object.
		*/
		TriangulatedSES()
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSES object from another.
				@param	surface	the TriangulatedSES object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSES(const TriangulatedSES& surface, bool = true)
			throw();

		/** Detailed constructor.
				Create a new empty TriangulatedSES object and set its SES.
				@param	ses			a pointer to the corresponding SES
				@param	density	the density to use by triangulation
		*/
		TriangulatedSES(SolventExcludedSurface* ses, const double& density)
			throw();

		/**	Destructor.
				Destructs the TriangulatedSES object.
		*/
		virtual ~TriangulatedSES()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSES.
				@param	surface	the TriangulatedSES object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSES& surface, bool = true)
			throw();

		/**	Assign from another TriangulatedSES.
				@param	surface	the TriangulatedSES object to assign from
		*/
		TriangulatedSES& operator = (const TriangulatedSES& surface)
			throw();

		/** Set the density used by triangulation.
		*/
		void setDensity(const double& density)
			throw();

		/** Get the density used by triangulation.
		*/
		double getDensity() const
			throw();

		//@}

		/** @name Accessors
		*/
		//@{

		void compute()
			throw(Exception::GeneralException,Exception::DivisionByZero);

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		SolventExcludedSurface* ses_;

		double density_;

		//@}

	};



	/** Generic SESTriangulator Class.
			 \par
			
			 \par
	*/
	class SESTriangulator
	{
		
		public:

		BALL_CREATE(SESTriangulator)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESTriangulator object.
		*/
		SESTriangulator()
			throw();

		/** Detailed constructor.
				@param	tses			a pointer to the TriangulatedSES to compute
		*/
		SESTriangulator(TriangulatedSES* tses)
			throw();

		/**	Destructor.
				Destructs the SESTriangulator object.
		*/
		virtual ~SESTriangulator()
			throw();
		//@}

		/** @name Accessors
		*/
		//@{

		void run()
			throw(Exception::GeneralException,Exception::DivisionByZero);

		private:

		void preProcessing()
			throw();

		void triangulateToricFaces()
			throw();

		void partitionSingularEdges()
			throw();

		void triangulateContactFaces()
			throw();

		void triangulateSphericFaces()
			throw();

		void triangulateToricFace
				(SESFace*	face,
				 const double&			probe_radius)
			throw();

		void triangulateFreeToricFace
				(SESFace*	face,
				 const double& probe_radius)
			throw();

		void triangulateSingularToricFace
				(SESFace*	face,
				 const double& probe_radius)
			throw();

		void triangulateNonSingularToricFace
				(SESFace*	face,
				 const double& probe_radius)
			throw();

		void partitionSingularEdge(SESEdge* singular_edge)
			throw();

		void partitionFreeSingularEdge(SESEdge* singular_edge)
			throw();

		void partitionNonFreeSingularEdge(SESEdge* singular_edge)
			throw();

		void triangulateContactFace
				(SESFace*				face,
				 const TSphere3<double>& sphere)
			throw();

		bool triangulateSphericFace
				(SESFace* 			face,
				 const TSphere3<double>&	sphere)
			throw();

		bool buildSphericTriangles
				(SESFace*					face,
				 TriangulatedSES&	part,
				 const TSphere3<double>&		sphere,
			 	 bool							convex = true)
			throw();

		bool buildFirstTriangle
				(SESFace*												face,
				 TriangulatedSES&								part,
				 std::list<TriangleEdge*>&			border,
				 const TSphere3<double>&									sphere,
				 const HashSet<TrianglePoint*>&	points,
			 	 bool														convex)
			throw();

		SESEdge* firstSESEdge(const std::list<SESEdge*>& sesedge)
			throw();

		void buildUnambiguousTriangle
				(TriangleEdge*						 edge,
				 TrianglePoint*						 point,
				 std::list<TriangleEdge*>& border,
				 const TSphere3<double>&						 sphere,
				 TriangulatedSES&					 part,
			 	 bool													 convex
				)
			throw();

		void buildAmbiguousTriangles
				(TriangleEdge*						 edge,
				 std::list<TrianglePoint*> points,
				 std::list<TriangleEdge*>& border,
				 const TSphere3<double>&						 sphere,
				 TriangulatedSES&					 part,
			 	 bool													 convex
				)
			throw();

		void createTriangleAndEdges
				(TriangleEdge*	edge,
				 TrianglePoint* point,
				 const TSphere3<double>& sphere,
				 TriangleEdge*& edge1,
				 bool&							old1,
				 TriangleEdge*& edge2,
				 bool&							old2,
				 Triangle*&			triangle,
			 	 bool								convex
				)
			throw();

		void partitionOfCircle
				(const TCircle3<double>&			circle,
				 const TVector3<double>&			p0,
				 const TAngle<double>&				phi,
				 Size								number_of_segments,
				 vector< TVector3<double> >&	partition,
				 bool								on_surface = true)
			throw();

		Size numberOfRefinements(const double& density, const double& radius)
			throw();

		void buildTemplateSpheres()
			throw();

		void buildTriangles
			 (SESEdge*											edge0,
				SESEdge*											edge1,
				SESEdge*											edge2,
				SESEdge*											edge3,
				const std::vector< TVector3<double> >&	centers,
				const std::vector< TVector3<double> >&	edge1_points,
				const std::vector< TVector3<double> >& edge3_points,
				const double&									probe_radius)
			throw();

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		TriangulatedSES* tses_;
		std::vector<TrianglePoint*> point_;
		std::vector< std::list<TriangleEdge*> > edge_;
		HashMap< Size,std::list<TrianglePoint*> > template_spheres_;
		double sqrt_density_;

		//@}

	};

   /** @} */
}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSES_H
