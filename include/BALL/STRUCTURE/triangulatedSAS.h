// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_TRIANGULATEDSAS_H
#define BALL_STRUCTURE_TRIANGULATEDSAS_H

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

#ifndef BALL_DATATYPE_HASHGRID_H
#	include <BALL/DATATYPE/hashGrid.h>
#endif

#include <list>
#include <vector>

namespace BALL
{
	class SASTriangulator;

	/** Generic TriangulatedSAS Class.
			\ingroup Surface	
	*/
	class BALL_EXPORT TriangulatedSAS	:	public TriangulatedSurface
	{
		
		public:

		/** @name Class friends

					- class SASTriangulator
				
		*/
		friend class SASTriangulator;

		BALL_CREATE(TriangulatedSAS)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSAS object.
		*/
		TriangulatedSAS();

		/**	Copy constructor.
				Create a new TriangulatedSAS object from another.
				@param	surface	the TriangulatedSAS object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSAS(const TriangulatedSAS& surface, bool = true);

		/** Detailed constructor.
				Create a new empty TriangulatedSAS object and set its SAS.
				@param	sas			a pointer to the corresponding SAS
				@param	density	the density to use by triangulation
		*/
		TriangulatedSAS(SolventAccessibleSurface* sas, const double& density);

		/**	Destructor.
				Destructs the TriangulatedSAS object.
		*/
		virtual ~TriangulatedSAS();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSAS.
				@param	surface	the TriangulatedSAS object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSAS& surface, bool = true);

		/**	Assign from another TriangulatedSAS.
				@param	surface	the TriangulatedSAS object to assign from
		*/
		TriangulatedSAS& operator = (const TriangulatedSAS& surface);

		//@}

		/** @name Accessors
		*/
		//@{

		/** Set the density used by triangulation.
		*/
		void setDensity(const double& density);

		/** Get the density used by triangulation.
		*/
		double getDensity() const;

		/** Compute the solvent-accessible surface
		*/
		void compute();

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
			 \par
			
			 \par
	\ingroup Surface	
	*/
	class BALL_EXPORT SASTriangulator
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
		SASTriangulator();

		/** Detailed constructor.
				Create a new empty SASTriangulator object and set its SAS.
				@param	tsas			a pointer to the corresponding SAS
		*/
		SASTriangulator(TriangulatedSAS* tsas);

		/**	Destructor.
				Destructs the SASTriangulator object.
		*/
		virtual ~SASTriangulator();
		//@}

		/** @name Accessors
		*/
		//@{

		void run();

		private:

		void triangulateFace(SASFace* face);

		void createPlanes(SASFace* face,
		                  std::list< std::pair<TPlane3<double>,double> >& planes);

		void tagPoints(TriangulatedSurface& part,
		               const std::list< std::pair<TPlane3<double>,double> >& planes);

		void removeInsideTriangles(TriangulatedSurface& part);

		HashGrid3<TrianglePoint*> createHashGrid(const TriangulatedSurface& part);

		void createPoints(TriangulatedSurface& part,
		                  const std::list< std::pair<TPlane3<double>,double> >& planes,
		                  HashGrid3<TrianglePoint*>& grid);

		void createNewTriangles(TriangulatedSurface& part, HashGrid3<TrianglePoint*>& grid);

		void onePointOutside(Index outside, Triangle* t,
		                     TriangulatedSurface& part, HashGrid3<TrianglePoint*>& grid);

		void twoPointsOutside(Position outside1, Position outside2,
		                      Triangle* t, TriangulatedSurface& part, HashGrid3<TrianglePoint*>& grid);

		TrianglePoint* vertexExists(const TVector3<double>& point, HashGrid3<TrianglePoint*>& grid);

		Size numberOfRefinements(const double& density, const double& radius);

		void buildTemplateSpheres();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		TriangulatedSAS* tsas_;

		double sqrt_density_;

		std::vector< std::list< TVector3<double> > > edge_;

		HashMap<Size,TriangulatedSurface> template_spheres_;

		//@}

	};

}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSAS_H
