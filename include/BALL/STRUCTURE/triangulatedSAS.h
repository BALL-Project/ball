// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSAS.h,v 1.3 2002/12/17 14:13:14 anker Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSAS_H
#define BALL_STRUCTURE_TRIANGULATEDSAS_H

//#define with_indices
//#define debug_triangulation
#ifdef debug_triangulation
#	define with_bonds
//#	define with_normals
#endif
#ifdef with_indices
#	define print_debug_info
#endif

#ifndef BALL_STRUCTURE_SASEDGE_H
#	include <BALL/STRUCTURE/SASEdge.h>
#endif

#ifndef BALL_STRUCTURE_SASFACE_H
#	include <BALL/STRUCTURE/SASFace.h>
#endif

#ifndef BALL_TRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventAccessibleSurface.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
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

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <list>
#include <vector>

namespace BALL
{

	class SASTriangulator;

	/** Generic TriangulatedSAS Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangulatedSAS.h}
			\\
	*/
	class TriangulatedSAS	:	public TriangulatedSurface
	{
		
		public:

		#ifdef debug_triangulation
		void printToHINFile(string filename);
		#endif

		/** @name Class friends
				\begin{itemize}
					\item class SASTriangulator
				\end{itemize}
		*/
		friend class SASTriangulator;

		BALL_CREATE(TriangulatedSAS)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSAS object.
		*/
		TriangulatedSAS()
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSAS object from another.
				@param	surface	the TriangulatedSAS object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSAS(const TriangulatedSAS& surface, bool = true)
			throw();

		/** Detailed constructor.
				Create a new empty TriangulatedSAS object and set its SAS.
				@param	ses			a pointer to the corresponding SAS
				@param	density	the density to use by triangulation
		*/
		TriangulatedSAS(SolventAccessibleSurface* sas, const double& density)
			throw();

		/**	Destructor.
				Destructs the TriangulatedSAS object.
		*/
		virtual ~TriangulatedSAS()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSAS.
				@param	surface	the TriangulatedSAS object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSAS& surface, bool = true)
			throw();

		/**	Assign from another TriangulatedSAS.
				@param	surface	the TriangulatedSAS object to assign from
		*/
		TriangulatedSAS& operator = (const TriangulatedSAS& surface)
			throw();

		//@}

		/** @name Accessors
		*/
		//@{

		/** Set the density used by triangulation.
		*/
		void setDensity(const double& density)
			throw();

		/** Get the density used by triangulation.
		*/
		double getDensity() const
			throw();

		/** Compute the solvent-accessible surface
		*/
		void compute()
			throw();

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		SolventAccessibleSurface* sas_;

		double density_;

		//@}

	};



	/** Generic SASTriangulator Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangulatedSAS.h}
			\\
	*/
	class SASTriangulator
	{
		
		public:

		#ifdef debug_triangulation
		void printToHINFile(string filename);
		void Contour2HIN(const std::list<TriangleEdge*>& contour, const string& file);
		void SASEdge2HIN(SASEdge* edge, const string& file);
		#endif

		BALL_CREATE(SASTriangulator)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASTriangulator object.
		*/
		SASTriangulator()
			throw();

		/** Detailed constructor.
				Create a new empty SASTriangulator object and set its SAS.
				@param	ses			a pointer to the corresponding SAS
				@param	density	the density to use by triangulation
		*/
		SASTriangulator(TriangulatedSAS* tsas)
			throw();

		/**	Destructor.
				Destructs the SASTriangulator object.
		*/
		virtual ~SASTriangulator()
			throw();
		//@}

		/** @name Accessors
		*/
		//@{

		void run()
			throw();

		private:

		void partitionEdge(SASEdge* edge)
			throw();

		void triangulateFace(SASFace* face)
			throw();

		bool buildSphericTriangles
				(TriangulatedSAS&	part,
				 const TSphere3<double>&		sphere
			 			#ifdef print_debug_info
				 		, int halt
			 			#endif
			 	)
			throw();

		bool buildFirstTriangle
				(TriangulatedSAS&						part,
				 const TSphere3<double>&							sphere,
				 std::list<TriangleEdge*>&	border
							#ifdef print_debug_info
							, int& halt
							#endif
			 	)
			throw();

		void buildUnambiguousTriangle
				(TriangleEdge*						 edge,
				 TrianglePoint*						 point,
				 std::list<TriangleEdge*>& border,
				 const TSphere3<double>&						 sphere,
				 TriangulatedSAS&					 part
							#ifdef print_debug_info
							, int& halt
							#endif
				)
			throw();

		void buildAmbiguousTriangles
				(TriangleEdge*						 edge,
				 std::list<TrianglePoint*> points,
				 std::list<TriangleEdge*>& border,
				 const TSphere3<double>&						 sphere,
				 TriangulatedSAS&					 part
							#ifdef print_debug_info
							, int& halt
							#endif
				)
			throw();

		void createTriangleAndEdges
				(TriangleEdge*	edge,
				 TrianglePoint* point,
				 const TSphere3<double>& sphere,
				 TriangleEdge*& edge1,
				 bool&					old1,
				 TriangleEdge*& edge2,
				 bool&					old2,
				 Triangle*&			triangle
				)
			throw();

		void partitionOfCircle
				(const TCircle3<double>&					circle,
				 const TVector3<double>&					p0,
				 const TAngle<double>&						phi,
				 Size										number_of_segments,
				 std::list< TVector3<double> >&	partition,
				 bool										on_surface = true)
			throw();

		Size numberOfRefinements(const double& density, const double& radius)
			throw();

		void buildTemplateSpheres()
			throw();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		TriangulatedSAS* tsas_;
		double sqrt_density_;
		std::vector< std::list< TVector3<double> > > edge_;
		HashMap< Size,std::list< TVector3<double> > > template_spheres_;

		//@}

	};


}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSAS_H
