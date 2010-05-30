// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#define BALL_STRUCTURE_TRIANGULATEDSURFACE_H

#ifndef BALL_STRUCTURE_TRIANGLE_H
#	include <BALL/STRUCTURE/triangle.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEEDGE_H
#	include <BALL/STRUCTURE/triangleEdge.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEPOINT_H
#	include <BALL/STRUCTURE/trianglePoint.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <list>
#include <vector>

namespace BALL
{
	/** Generic TriangulatedSurface Class.
			\ingroup Surface
	*/
	class BALL_EXPORT TriangulatedSurface
	{

		public:

		BALL_CREATE(TriangulatedSurface)

		/**	@name	Type definitions
		*/
		//@{

		typedef std::list<TrianglePoint*>::iterator
						PointIterator;
		typedef std::list<TrianglePoint*>::const_iterator
						ConstPointIterator;
		typedef std::list<TriangleEdge*>::iterator
						EdgeIterator;
		typedef std::list<TriangleEdge*>::const_iterator
						ConstEdgeIterator;
		typedef std::list<Triangle*>::iterator
						TriangleIterator;
		typedef std::list<Triangle*>::const_iterator
						ConstTriangleIterator;

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSurface object.
		*/
		TriangulatedSurface()
			;

		/**	Copy constructor.
				Create a new TriangulatedSurface object from another.
				@param	surface	the TriangulatedSurface object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSurface(const TriangulatedSurface& surface, bool = true)
			;

		/**	Destructor.
				Destructs the TriangulatedSurface object.
		*/
		virtual ~TriangulatedSurface()
			;
		//@}

		/** @name Static Members
		 */
		//@{

		/**
		 * A static function that constructs a triangulated tube. The tube is oriented along the
		 * z-axis, starts at z=0, ends at z=1 and has radius 1
		 *
		 * @param num_vertices The amount of vertices of which the base of the tube is composed
		 * @param subdiv Controls the amount of subdivisions along the z-axis. Default: 0
		 * @param closed Should the tube have opend or closed ends? Default: false
		 * @param out Controls whether the normals of the tube are pointing in or outwards. Default: true
		 */
		static TriangulatedSurface* createTube(unsigned int num_vertices, unsigned int subdiv = 0, bool closed = false, bool out = true);

		/**
		 * A static function that constructs a triangulated disk. The disk lies in the x-y plane
		 * and has radius 1.
		 *
		 * @param num_vertices The amount of vertices of which the border of the disk is composed
		 * @param out Controls whether the normals of the tube are pointing up or downwards. Default: true
		 */
		static TriangulatedSurface* createDisk(unsigned int num_vertices, bool out = true);
		//@}

		/**	@name	Assignment
		*/
		//@{

		/** Delete all points, edges and triangles.
		*/
		void clear()
			;

		/**	Assign from another TriangulatedSurface.
				@param	surface	the TriangulatedSurface object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSurface& surface, bool = true)
			;

		/**	Assign from another TriangulatedSurface.
				@param	surface	the TriangulatedSurface object to assign from
		*/
		TriangulatedSurface& operator = (const TriangulatedSurface& surface)
			;

		//@}

		/** @name Accessors
		*/
		//@{

		/** Insert a new point to the TriangulatedSurface.
				@param	point	a pointer to the new point
		*/
		void insert(TrianglePoint* point);

		/** Insert a new edge to the TriangulatedSurface.
				@param	edge	a pointer to the new edge
		*/
		void insert(TriangleEdge* edge);

		/** Insert a new triangle to the TriangulatedSurface.
				@param	triangle	a pointer to the new triangle
		*/
		void insert(Triangle* triangle);

		/** Get the number of points of the TriangulatedSurface.
		 * 	@deprecated Use TriangulatedSurface::getNumberOfPoints() instead
		*/
		BALL_DEPRECATED Size numberOfPoints() const;

		/** Get the number of points of the TriangulatedSurface.
		 * 	@deprecated Use TriangulatedSurface::getNumberOfPoints() instead
		*/
		Size getNumberOfPoints() const;

		/** Get the number of edges of the TriangulatedSurface.
		 * 	@deprecated Use TriangulatedSurface::getNumberOfEdges() instead
		*/
		BALL_DEPRECATED Size numberOfEdges() const;

		/** Get the number of edges of the TriangulatedSurface.
		*/
		Size getNumberOfEdges() const;

		/** Get the number of triangles of the TriangulatedSurface.
		 * 	@deprecated Use TriangulatedSurface::getNumberOfTriangles() instead
		*/
		BALL_DEPRECATED Size numberOfTriangles() const;

		/** Get the number of triangles of the TriangulatedSurface.
		 * 	Use TriangulatedSurface::getNumberOfTriangles() instead
		*/
		Size getNumberOfTriangles() const;

		/** Remove a point from the TriangulatedSurface.
				If deep is true (default) the incidence-structure will be updated.
				@param	point	a pointer to the point to delete
				@param	deep	look above
		*/
		void remove(TrianglePoint* point, bool deep = true);

		/** Remove a point from the TriangulatedSurface.
				If deep is true (default) the incidence-structure will be updated.
				@param	p			an iterator into the list of points of the
											TriangulatedSurface which indicates the point to delete
				@param	deep	look above
		*/
		void remove(PointIterator p, bool deep = true);

		/** Remove an edge from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	edge	a pointer to the edge to delete
				@param	deep	look above
		*/
		void remove(TriangleEdge* edge, bool deep = true);

		/** Remove an edge from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	e			an iterator into the list of edges of the
											TriangulatedSurface which indicates the edge to delete
				@param	deep	look above
		*/
		void remove(EdgeIterator e, bool deep = true);

		/** Remove a triangle from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	triangle	a pointer to the triangle to delete
				@param	deep			look above
		*/
		void remove(Triangle* triangle, bool deep = true);

		/** Remove a triangle from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	t			an iterator into the list of triangles of the
											TriangulatedSurface which indicates the triangle to
											delete
				@param	deep	look above
		*/
		void remove(TriangleIterator t, bool deep = true);

		/** Create a Surface object from the TriangulatedeSurface.
				@param	surface	the created Surface object
		*/
		void exportSurface(Surface& surface);

		/** Add a TriangulatedSurface object.
				The lists of points, edges and triangles of the given
				TriangulatedSurface objact are appended. The given TriangulatedSurface
				objact will be unchanged.
				@param	surface	the TriangulatedSurface object to add
		*/
		TriangulatedSurface& operator += (const TriangulatedSurface& surface);

		/** Add a TriangulatedSurface object.
				The lists of points, edges and triangles of the given
				TriangulatedSurface objact are appended. After this operation, the
				given TriangulatedSurface objact will be empty!
				@param	source	the TriangulatedSurface object to add
		*/
		void join(TriangulatedSurface& source);

		/** Shift the TriangulatedSurface.
				All points are shifted by a vector c.
				@param	c	the shift vector
		*/
		void shift(const TVector3<double>& c);

		/**	Blow up th TriangulatedSurface.
				All points are multiplied by a scalar r.
				@param	r	the blow up factor
		*/
		void blowUp(const double& r);

		/** Set the indices of all points, edges and triangles.
		*/
		void setIndices();

		/** Set the density used by triangulation algorithms.
		 */
		void setDensity(const double& density);
		
		/** Get the density used by triangulation algorithms.
		 */
		double getDensity() const;
		
		/** Cut the TriangulatedSurface on a plane.
				@param	plane	the plane to cut on
				@param	fuzzy
		*/
		void cut(const TPlane3<double>& plane, const double& fuzzy = 0);

		/** Delete all triangles on the border of the TriangulatedSurface
		*/
		void shrink();

		/** Delete all isolated edges of the TriangulatedSurface
		*/
		void deleteIsolatedEdges();

		/** Delete all isolated points of the TriangulatedSurface
		*/
		void deleteIsolatedPoints();

		/** Get the border edges of the TriangulatedSurface.
				Border edges are the edges with only one triangle.
				@param	border	a list of the border edges
		*/
		void getBorder(std::list<TriangleEdge*>& border);

		//@}
		/**	@name	External Iterators
		*/
		//@{

		PointIterator beginPoint();

		ConstPointIterator beginPoint() const;

		PointIterator endPoint();

		ConstPointIterator endPoint() const;

		EdgeIterator beginEdge();

		ConstEdgeIterator beginEdge() const;

		EdgeIterator endEdge();

		ConstEdgeIterator endEdge() const;

		TriangleIterator beginTriangle();

		ConstTriangleIterator beginTriangle() const;

		TriangleIterator endTriangle();

		ConstTriangleIterator endTriangle() const;

		//@}
		/*_	@name	protected help functions
		*/
		//@{

		protected:

		/*_ Test whether a TriangulatedSurface object can be copied
		*/
		bool canBeCopied() const;

		/*_ Copy a TriangulatedSurface object
		*/
		void copy(const TriangulatedSurface& surface);

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

    /*_ the number of points of the triangulated surface
    */
    Size number_of_points_;
		/*_ the points of the surface.
		*/
		std::list<TrianglePoint*> points_;
    /*_ the number of edges of the triangulated surface
    */
    Size number_of_edges_;
		/*_ the edges of the surface.
		*/
		std::list<TriangleEdge*> edges_;
    /*_ the number of triangles of the triangulated surface
    */
    Size number_of_triangles_;
		/*_ the triangles of the surface.
		*/
		std::list<Triangle*> triangles_;

		/*_ the density used by triangulation algorithms
		*/
		double density_;

		//@}

	};

	/**	@name	Storers
	\ingroup Surface
	*/
	//@{

	/**	Output- Operator
	*/
	BALL_EXPORT std::ostream& operator << (std::ostream& s,
																				 const TriangulatedSurface& surface);

	//@}




	/** Generic TriangulatedSphere Class.
			 \par
			
			 \par
	\ingroup Surface
	*/
	class BALL_EXPORT TriangulatedSphere	:	public TriangulatedSurface
	{

		public:

		BALL_CREATE(TriangulatedSphere)

		struct Face
		{
			TrianglePoint* p[6];
			TriangleEdge* e[9];
			Position pcount;
			Position ecount;
		};

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSphere object.
		*/
		TriangulatedSphere()
			;

		/**	Copy constructor.
				Create a new TriangulatedSphere object from another.
				@param	sphere	the TriangulatedSphere object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSphere(const TriangulatedSphere& sphere, bool = true)
			;

		/**	Destructor.
				Destructs the TriangulatedSphere object.
		*/
		virtual ~TriangulatedSphere()
			;
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSphere.
				@param	sphere	the TriangulatedSphere object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSphere& sphere, bool = true)
			;

		/**	Assign from another TriangulatedSphere.
				@param	sphere	the TriangulatedSphere object to assign from
		*/
		TriangulatedSphere& operator = (const TriangulatedSphere& sphere)
			;

		//@}

		/** @name Accessors
		*/
		//@{

		/** Build an icosaeder.
				If the TriangulatedSphere is not empty, it will be cleared first.
				The center of the icosaeder will be the origin, the radius will be 1.
				@param	out	if out = true the normal vectors will be oriented outside,
										otherwise they will be oriented inside
		*/
		void icosaeder(bool out = true);

		/** Build a pentakis dodecaeder.
				If the TriangulatedSphere is not empty, it will be cleared first.
				The center of the dodecaeder will be the origin, the radius will be 1.
				@param	out	if out = true the normal vectors will be oriented outside,
										otherwise they will be oriented inside
		*/
		void pentakisDodecaeder(bool out = true);

		/** Refine a triangulated sphere.
				The center of the sphere must be the origin, the radius must be 1.
				@param	iterations	the number of refinement iterations
				@param	out				the orientation of the normal vectors
		*/
		void refine(Position iterations, bool out = true);


		private:

		/*_ Refine a triangulated sphere once.
		*/
		void refine(bool out);

		void buildFourTriangles
				(Face face,
				 Triangle* face0,
				 Triangle* face1,
				 Triangle* face2,
				 Triangle* face3)
			;

		/*_ Set the incidences of a refined triangulated sphere.
		*/
		void setIncidences();

		//@}

	};
   
}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSURFACE_H


