// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSurface.h,v 1.25.2.1 2003/01/07 13:19:15 anker Exp $

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
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangulatedSurface.h}
			\\
	*/
	class TriangulatedSurface
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
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSurface object from another.
				@param	surface	the TriangulatedSurface object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSurface(const TriangulatedSurface& surface, bool = true)
			throw();

		/**	Destructor.
				Destructs the TriangulatedSurface object.
		*/
		virtual ~TriangulatedSurface()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/** Delete all points, edges and triangles.
		*/
		void clear()
			throw();

		/**	Assign from another TriangulatedSurface.
				@param	surface	the TriangulatedSurface object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSurface& surface, bool = true)
			throw();

		/**	Assign from another TriangulatedSurface.
				@param	surface	the TriangulatedSurface object to assign from
		*/
		TriangulatedSurface& operator = (const TriangulatedSurface& surface)
			throw();

		//@}

		/** @name Accessors
		*/
		//@{

		/** Insert a new point to the TriangulatedSurface.
				@param	point	a pointer to the new point
		*/
		void insert(TrianglePoint* point)
			throw();

		/** Insert a new edge to the TriangulatedSurface.
				@param	edge	a pointer to the new edge
		*/
		void insert(TriangleEdge* edge)
			throw();

		/** Insert a new triangle to the TriangulatedSurface.
				@param	triangle	a pointer to the new triangle
		*/
		void insert(Triangle* triangle)
			throw();

		/** Get the number of points of the TriangulatedSurface.
		*/
		Size numberOfPoints() const
			throw();

		/** Get the number of edges of the TriangulatedSurface.
		*/
		Size numberOfEdges() const
			throw();

		/** Get the number of triangles of the TriangulatedSurface.
		*/
		Size numberOfTriangles() const
			throw();

		/** Remove a point from the TriangulatedSurface.
				If deep is true (default) the incidence-structure will be updated.
				@param	point	a pointer to the point to delete
				@param	deep	look above
		*/
		void remove(TrianglePoint* point, bool deep = true)
			throw();

		/** Remove a point from the TriangulatedSurface.
				If deep is true (default) the incidence-structure will be updated.
				@param	p			an iterator into the list of points of the
											TriangulatedSurface which indicates the point to delete
				@param	deep	look above
		*/
		void remove(PointIterator p, bool deep = true)
			throw();

		/** Remove an edge from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	edge	a pointer to the edge to delete
				@param	deep	look above
		*/
		void remove(TriangleEdge* edge, bool deep = true)
			throw();

		/** Remove an edge from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	e			an iterator into the list of edges of the
											TriangulatedSurface which indicates the edge to delete
				@param	deep	look above
		*/
		void remove(EdgeIterator e, bool deep = true)
			throw();

		/** Remove a triangle from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	triangle	a pointer to the triangle to delete
				@param	deep			look above
		*/
		void remove(Triangle* triangle, bool deep = true)
			throw();

		/** Remove a triangle from the TriangulatedSurface.
				If deep is true (not default) the incidence-structure will be updated.
				@param	p			an iterator into the list of triangles of the
											TriangulatedSurface which indicates the triangle to
											delete
				@param	deep	look above
		*/
		void remove(TriangleIterator t, bool deep = true)
			throw();

		/** Create a Surface object from the TriangulatedeSurface.
				@param	surface	the created Surface object
		*/
		void exportSurface(Surface& surface)
			throw();

		/** Add a TriangulatedSurface object.
				The lists of points, edges and triangles of the given
				TriangulatedSurface objact are appended. The given TriangulatedSurface
				objact will be unchanged.
				@param	surface	the TriangulatedSurface object to add
		*/
		TriangulatedSurface& operator += (const TriangulatedSurface& surface)
			throw();

		/** Add a TriangulatedSurface object.
				The lists of points, edges and triangles of the given
				TriangulatedSurface objact are appended. After this operation, the
				given TriangulatedSurface objact will be empty!
				@param	source	the TriangulatedSurface object to add
		*/
		void join(TriangulatedSurface& source)
			throw();

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

		/** Set the indices of al points, edges and triangles.
		*/
		void setIndices();

		/** Cut the TriangulatedSurface on a plane.
				@param	plane	the plane to cut on
				@param	fuzzy
		*/
		void cut(const TPlane3<double>& plane, const double& fuzzy = 0);

		/** Delete all triangles on the border of the TriangulatedSurface
		*/
		void shrink();

		/** Delete all isolated points of the TriangulatedSurface
		*/
		void deleteIsolatedPoints()
			throw();

		/** Get the border edges of the TriangulatedSurface.
				Border edges are the edges with only one triangle.
				@param	border	a list of the border edges
		*/
		void getBorder(std::list<TriangleEdge*>& border);

		//@}
		/**	@name	External Iterators
		*/
		//@{

		PointIterator beginPoint()
			throw();
		ConstPointIterator beginPoint() const
			throw();
		PointIterator endPoint()
			throw();
		ConstPointIterator endPoint() const
			throw();

		EdgeIterator beginEdge()
			throw();
		ConstEdgeIterator beginEdge() const
			throw();
		EdgeIterator endEdge()
			throw();
		ConstEdgeIterator endEdge() const
			throw();

		TriangleIterator beginTriangle()
			throw();
		ConstTriangleIterator beginTriangle() const
			throw();
		TriangleIterator endTriangle()
			throw();
		ConstTriangleIterator endTriangle() const
			throw();

		//@}
		/*_	@name	protected help functions
		*/
		//@{

		protected:

		/*_ Test whether a TriangulatedSurface object can be copied
		*/
		bool canBeCopied() const
			throw();

		/*_ Copy a TriangulatedSurface object
		*/
		void copy(const TriangulatedSurface& surface)
			throw();

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

		//@}

	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator << (std::ostream& s,
														 const TriangulatedSurface& surface);

	//@}




	/** Generic TriangulatedSphere Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangulatedSurface.h}
			\\
	*/
	class TriangulatedSphere	:	public TriangulatedSurface
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
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSphere object from another.
				@param	sphere	the TriangulatedSphere object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TriangulatedSphere(const TriangulatedSphere& sphere, bool = true)
			throw();

		/**	Destructor.
				Destructs the TriangulatedSphere object.
		*/
		virtual ~TriangulatedSphere()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSphere.
				@param	sphere	the TriangulatedSphere object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TriangulatedSphere& sphere, bool = true)
			throw();

		/**	Assign from another TriangulatedSphere.
				@param	sphere	the TriangulatedSphere object to assign from
		*/
		TriangulatedSphere& operator = (const TriangulatedSphere& sphere)
			throw();

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

		/** Refine a triangulated sphere.
				The center of the sphere must be the origin, the radius must be 1.
				@param	iteration	the number of refinement iterations
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
			throw();

		/*_ Set the incidences of a refined triangulated sphere.
		*/
		void setIncidences();

		//@}

	};

}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSURFACE_H


