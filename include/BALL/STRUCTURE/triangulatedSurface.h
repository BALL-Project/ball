// $Id: triangulatedSurface.h,v 1.23 2001/12/08 17:10:39 strobel Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#define BALL_STRUCTURE_TRIANGULATEDSURFACE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEPOINT_H
#	include <BALL/STRUCTURE/trianglePoint.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEEDGE_H
#	include <BALL/STRUCTURE/triangleEdge.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLE_H
#	include <BALL/STRUCTURE/triangle.h>
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
	template <class T>
	class TTriangulatedSurface
	{
		
		public:

		BALL_CREATE(TTriangulatedSurface)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSurface object.
		*/
		TTriangulatedSurface()
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSurface object from another.
				@param	surface	the TriangulatedSurface object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TTriangulatedSurface(const TTriangulatedSurface<T>& surface, bool = true)
			throw(Exception::GeneralException);

		/**	Destructor.
				Destructs the TriangulatedSurface object.
		*/
		virtual ~TTriangulatedSurface()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSurface.
				@param	surface	the TriangulatedSurface object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TTriangulatedSurface<T>& surface, bool = true)
			throw(Exception::GeneralException);

		/**	Assign from another TriangulatedSurface.
				@param	surface	the TriangulatedSurface object to assign from
		*/
		void operator=(const TTriangulatedSurface<T>& surface)
			throw(Exception::GeneralException);

		//@}

		/** @name Accessors
		*/
		//@{

		/** Push a new point to the TriangulatedSurface.
				@param	point	a pointer to the new point
		*/
		void pushPoint(TTrianglePoint<T>* point)
			throw();

		/** Get the list of points of the TriangulatedSurface.
				@param	points	the list of points
		*/
		void getPoints(std::list<TTrianglePoint<T>*>& points) const
			throw();

		/** Get the number of points of the TriangulatedSurface.
		*/
		Size numberOfPoints() const
			throw();

		/** Push a new edge to the TriangulatedSurface.
				@param	edge	a pointer to the new edge
		*/
		void pushEdge(TTriangleEdge<T>* edge)
			throw();

		/** Get the list of edges of the TriangulatedSurface.
				@param	points	the list of edges
		*/
		void getEdges(std::list<TTriangleEdge<T>*>& edges) const
			throw();

		/** Get the number of edges of the TriangulatedSurface.
		*/
		Size numberOfEdges() const
			throw();

		/** Push a new triangle to the TriangulatedSurface.
				@param	triangle	a pointer to the new triangle
		*/
		void pushTriangle(TTriangle<T>* triangle)
			throw();

		/** Get the list of triangles of the TriangulatedSurface.
				@param	points	the list of trianglers
		*/
		void getTriangles(std::list<TTriangle<T>*>& triangles) const
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
		void remove(TTrianglePoint<T>* point, bool deep = true)
			throw();

		/** Remove a point from the TriangulatedSurface.
				If deep is true (default) the incidence-structure will be updated.
				@param	p			an iterator into the list of points of the TriangulatedSurface which indicates	
											the point to delete
				@param	deep	look above
		*/
		void remove(typename std::list<TTrianglePoint<T>*>::iterator p, bool deep = true)
			throw();

		/** Remove an edge from the TriangulatedSurface.
				If deep is false (default) the incidence-structure will not be updated.
				@param	edge	a pointer to the edge to delete
				@param	deep	look above
		*/
		void remove(TTriangleEdge<T>* edge, bool deep = false)
			throw();

		/** Remove an edge from the TriangulatedSurface.
				If deep is false (default) the incidence-structure will not be updated.
				@param	e			an iterator into the list of edges of the TriangulatedSurface which indicates	
											the edge to delete
				@param	deep	look above
		*/
		void remove(typename std::list<TTriangleEdge<T>*>::iterator e, bool deep = false)
			throw();

		/** Remove a triangle from the TriangulatedSurface.
				If deep is false (default) the incidence-structure will not be updated.
				@param	triangle	a pointer to the triangle to delete
				@param	deep			look above
		*/
		void remove(TTriangle<T>* triangle, bool deep = false)
			throw();

		/** Remove a triangle from the TriangulatedSurface.
				If deep is false (default) the incidence-structure will not be updated.
				@param	p			an iterator into the list of triangles of the TriangulatedSurface which indicates	
											the triangle to delete
				@param	deep	look above
		*/
		void remove(typename std::list<TTriangle<T>*>::iterator t, bool deep = false)
			throw();

		/** Create a Surface objact from th TriangulatedeSurface.
				@param	surface	the created Surface object
		*/
		void exportSurface(TSurface<T>& surface)
			throw();

		/** Add a TriangulatedSurface object.
				The lists of points, edges and triangles of the given TriangulatedSurface objact are appended.	
				The given TriangulatedSurface objact will be unchanged.
				@param	surface	the TriangulatedSurface object to add
		*/
		TTriangulatedSurface<T>& operator+=(const TTriangulatedSurface<T>& surface)
			throw();

		/** Add a TriangulatedSurface object.
				The lists of points, edges and triangles of the given TriangulatedSurface objact are appended.	
				After this operation, the given TriangulatedSurface objact will be empty!
				@param	source	the TriangulatedSurface object to add
		*/
		void join(TTriangulatedSurface<T>& source);

		/** Make the TriangulatedSurface object an icosaeder (Ist das korrektes Englisch???).
				If the TriangulatedSurface was not empty it will be cleared first. The center of the icosaeder	
				will be the origin, the radius will be 1.
				@param	out	if out = true the normal vectors will be oriented outside, otherwise thy will	
										be oriented inside
		*/
		void icosaeder(bool out = true);

		/** Shift the TriangulatedSurface.
				All points are shifted by a vector c.
				@param	c	the shift vector
		*/
		void shift(const TVector3<T>& c);

		/**	Blow up th TriangulatedSurface.
				All points are multiplied by a scalar r.
				@param	r	the blow up factor
		*/
		void blowUp(const T& r);

		/** Refine a triangulated sphere.
				The center of the sphere must be the origin, the radius must be 1.
				@param	iteration	the number of refinement iterations
				@param	out				the orientation of the normal vectors
		*/
		void refineSphere(Position iterations, bool out = true);

		/** Set the indices of al points, edges and triangles.
		*/
		void setIndices();

		/** Cut the TriangulatedSurface on a plane.
				@param	plane	the plane to cut on
				@param	fuzzy
		*/
		void cut(const TPlane3<T>& plane, const T& fuzzy = 0);

		/** Get the border edges of the TriangulatedSurface.
				Border edges are the edges with only one triangle.
				@param	border	a list of the border edges
		*/
		void getBorder(std::list<TTriangleEdge<T>*>& border);

		protected:

		/*_ Copy a TriangulatedSurface object
		*/
		void copy(const TTriangulatedSurface<T>& surface)
			throw(Exception::GeneralException);

		/*_ Refine a triangulated sphere once.
		*/
		void refineSphere(bool out);

		/*_ Set the incidences of a refined triangulated sphere.
		*/
		void setIncidences();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		/*_ the points of the surface.
		*/
		std::list<TTrianglePoint<T>*> points_;

		/*_ the edges of the surface.
		*/
		std::list<TTriangleEdge<T>*> edges_;

		/*_ the triangles of the surface.
		*/
		std::list<TTriangle<T>*> triangles_;
    /*_ the number of points of the triangulated surface
    */
    Size number_of_points_;
    /*_ the number of edges of the triangulated surface
    */
    Size number_of_edges_;
    /*_ the number of triangles of the triangulated surface
    */
    Size number_of_triangles_;

		//@}

	};
	

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TTriangulatedSurface<T>& surface)
	{
		std::list<TTrianglePoint<T>*> points;
		surface.getPoints(points);
		typename std::list<TTrianglePoint<T>*>::const_iterator p;
		s << "Points: " << surface.numberOfPoints() << " = " << points.size() << "\n";
		for (p = points.begin(); p != points.end(); p++)
		{
			s << **p << "\n";
		}
		std::list<TTriangleEdge<T>*> edges;
		surface.getEdges(edges);
		typename std::list<TTriangleEdge<T>*>::const_iterator e;
		s << "Edges: " << surface.numberOfEdges() << " = " << edges.size() << "\n";
		for (e = edges.begin(); e != edges.end(); e++)
		{
			s << **e << "\n";
		}
		std::list<TTriangle<T>*> triangles;
		surface.getTriangles(triangles);
		typename std::list<TTriangle<T>*>::const_iterator t;
		s << "Triangles: " << surface.numberOfTriangles() << " = " << triangles.size() << "\n";
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			s << **t << "\n";
		}
		return s;
	}

	/**	The Default TriangulatedSurface type.
			If double precision is not needed, {\tt TTriangulatedSurface<float>} should
			be used. It is predefined as {\tt TriangulatedSurface} for convenience.
	*/
	typedef TTriangulatedSurface<float> TriangulatedSurface;



	template <class T>
	TTriangulatedSurface<T>::TTriangulatedSurface()
		throw()
		:	points_(),
			edges_(),
			triangles_(),
			number_of_points_(0),
			number_of_edges_(0),
			number_of_triangles_(0)
	{
	}


	template <class T>
	TTriangulatedSurface<T>::TTriangulatedSurface(const TTriangulatedSurface<T>& surface, bool)
		throw(Exception::GeneralException)
		:	points_(),
			edges_(),
			triangles_(),
			number_of_points_(surface.number_of_points_),
			number_of_edges_(surface.number_of_edges_),
			number_of_triangles_(surface.number_of_triangles_)
	{
		try
		{
			copy(surface);
		}
		catch (Exception::GeneralException)
		{
			typename std::list<TTrianglePoint<T>*>::iterator p;
			typename std::list<TTriangleEdge<T>*>::iterator e;
			typename std::list<TTriangle<T>*>::iterator t;
			for (p = points_.begin(); p != points_.end(); p++)
			{
				delete *p;
			}
			for (e = edges_.begin(); e != edges_.end(); e++)
			{
				delete *e;
			}
			for (t = triangles_.begin(); t != triangles_.end(); t++)
			{
				delete *t;
			}
			points_.clear();
			edges_.clear();
			triangles_.clear();
			number_of_points_ = 0;
			number_of_edges_ = 0;
			number_of_triangles_ = 0;
			throw;
		}
	}


	template <class T>
	TTriangulatedSurface<T>::~TTriangulatedSurface()
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			delete *p;
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			delete *e;
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			delete *t;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::set(const TTriangulatedSurface<T>& surface, bool)
		throw(Exception::GeneralException)
	{
		*this = surface;
	}


	template <class T>
	void TTriangulatedSurface<T>::operator=(const TTriangulatedSurface<T>& surface)
		throw(Exception::GeneralException)
	{
		number_of_points_ = surface.number_of_points_;
		number_of_edges_ = surface.number_of_edges_;
		number_of_triangles_ = surface.number_of_triangles_;
		try
		{
			copy(surface);
		}
		catch (Exception::GeneralException)
		{
			typename std::list<TTrianglePoint<T>*>::iterator p;
			typename std::list<TTriangleEdge<T>*>::iterator e;
			typename std::list<TTriangle<T>*>::iterator t;
			for (p = points_.begin(); p != points_.end(); p++)
			{
				delete *p;
			}
			for (e = edges_.begin(); e != edges_.end(); e++)
			{
				delete *e;
			}
			for (t = triangles_.begin(); t != triangles_.end(); t++)
			{
				delete *t;
			}
			points_.clear();
			edges_.clear();
			triangles_.clear();
			number_of_points_ = 0;
			number_of_edges_ = 0;
			number_of_triangles_ = 0;
			throw;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::pushPoint(TTrianglePoint<T>* p)
		throw()
	{
		points_.push_back(p);
		number_of_points_++;
	}


	template <class T>
	void TTriangulatedSurface<T>::getPoints(std::list<TTrianglePoint<T>*>& points) const
		throw()
	{
		points = points_;
	}


	template <class T>
	Size TTriangulatedSurface<T>::numberOfPoints() const
		throw()
	{
		return number_of_points_;
	}


	template <class T>
	void TTriangulatedSurface<T>::pushEdge(TTriangleEdge<T>* e)
		throw()
	{
		edges_.push_back(e);
		number_of_edges_++;
	}


	template <class T>
	void TTriangulatedSurface<T>::getEdges(std::list<TTriangleEdge<T>*>& edges) const
		throw()
	{
		edges = edges_;
	}


	template <class T>
	Size TTriangulatedSurface<T>::numberOfEdges() const
		throw()
	{
		return number_of_edges_;
	}


	template <class T>
	void TTriangulatedSurface<T>::pushTriangle(TTriangle<T>* t)
		throw()
	{
		triangles_.push_back(t);
		number_of_triangles_++;
	}


	template <class T>
	void TTriangulatedSurface<T>::getTriangles(std::list<TTriangle<T>*>& triangles) const
		throw()
	{
		triangles = triangles_;
	}


	template <class T>
	Size TTriangulatedSurface<T>::numberOfTriangles() const
		throw()
	{
		return number_of_triangles_;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(TTrianglePoint<T>* point, bool deep)
		throw()
	{
		if (deep)
		{
			std::list<TTriangle<T>*> delete_triangles = point->faces_;
			typename std::list<TTriangle<T>*>::iterator t;
			for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
			{
				(*t)->vertex_[0]->faces_.remove(*t);
				(*t)->vertex_[1]->faces_.remove(*t);
				(*t)->vertex_[2]->faces_.remove(*t);
				(*t)->edge_[0]->deleteFace(*t);
				(*t)->edge_[1]->deleteFace(*t);
				(*t)->edge_[2]->deleteFace(*t);
				//(*t)->getOppositeEdge(point)->deleteFace(*t);
				triangles_.remove(*t);
				delete *t;
				number_of_triangles_--;
			}
			std::list<TTriangleEdge<T>*> delete_edges = point->edges_;
			typename std::list<TTriangleEdge<T>*>::iterator e;
			for (e = delete_edges.begin(); e != delete_edges.end(); e++)
			{
				(*e)->vertex_[0]->edges_.remove(*e);
				(*e)->vertex_[1]->edges_.remove(*e);
				edges_.remove(*e);
				delete *e;
				number_of_edges_--;
			}
		}
		points_.remove(point);
		delete point;
		number_of_points_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(typename std::list<TTrianglePoint<T>*>::iterator point, bool deep)
		throw()
	{
		if (deep)
		{
			std::list<TTriangle<T>*> delete_triangles = (*point)->faces_;
			typename std::list<TTriangle<T>*>::iterator t;
			for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
			{
				(*t)->vertex_[0]->faces_.remove(*t);
				(*t)->vertex_[1]->faces_.remove(*t);
				(*t)->vertex_[2]->faces_.remove(*t);
				(*t)->edge_[0]->deleteFace(*t);
				(*t)->edge_[1]->deleteFace(*t);
				(*t)->edge_[2]->deleteFace(*t);
				//(*t)->getOppositeEdge(*point)->deleteFace(*t);
				triangles_.remove(*t);
				number_of_triangles_--;
				delete *t;
			}
			std::list<TTriangleEdge<T>*> delete_edges = (*point)->edges_;
			typename std::list<TTriangleEdge<T>*>::iterator e;
			for (e = delete_edges.begin(); e != delete_edges.end(); e++)
			{
				(*e)->vertex_[0]->edges_.remove(*e);
				(*e)->vertex_[1]->edges_.remove(*e);
				edges_.remove(*e);
				number_of_edges_--;
				delete *e;
			}
		}
		points_.erase(point);
		delete *point;
		number_of_points_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(TTriangleEdge<T>* edge, bool deep)
		throw()
	{
		if (deep)
		{
			edge->vertex_[0]->edges_.remove(edge);
			edge->vertex_[1]->edges_.remove(edge);
			edge->face_[0]->remove(edge);
			edge->face_[1]->remove(edge);
		}
		edges_.remove(edge);
		delete edge;
		number_of_edges_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(typename std::list<TTriangleEdge<T>*>::iterator e, bool deep)
		throw()
	{
		if (deep)
		{
			(*e)->vertex_[0]->edges_.remove(*e);
			(*e)->vertex_[1]->edges_.remove(*e);
			(*e)->face_[0]->remove(*e);
			(*e)->face_[1]->remove(*e);
		}
		delete *e;
		edges_.erase(e);
		number_of_edges_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(TTriangle<T>* triangle, bool deep)
		throw()
	{
		if (deep)
		{
			triangle->vertex_[0]->faces_.remove(triangle);
			triangle->vertex_[1]->faces_.remove(triangle);
			triangle->vertex_[2]->faces_.remove(triangle);
			triangle->edge[0]->deleteFace(triangle);
			triangle->edge[1]->deleteFace(triangle);
			triangle->edge[2]->deleteFace(triangle);
		}
		triangles_.remove(t);
		delete t;
		number_of_triangles_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(typename std::list<TTriangle<T>*>::iterator t, bool deep)
		throw()
	{
		if (deep)
		{
			(*t)->vertex_[0]->faces_.remove(*t);
			(*t)->vertex_[1]->faces_.remove(*t);
			(*t)->vertex_[2]->faces_.remove(*t);
			(*t)->edge[0]->deleteFace(*t);
			(*t)->edge[1]->deleteFace(*t);
			(*t)->edge[2]->deleteFace(*t);
		}
		triangles_.erase(t);
		delete t;
		number_of_triangles_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::exportSurface(TSurface<T>& surface)
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		Index i = 0;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			surface.vertex.push_back((*p)->point_);
			surface.normal.push_back((*p)->normal_);
			(*p)->index_ = i;
			i++;
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			TSurface<T>::Triangle triangle;
			triangle.v1 = (*t)->vertex_[0]->index_;
			triangle.v2 = (*t)->vertex_[1]->index_;
			triangle.v3 = (*t)->vertex_[2]->index_;
			surface.triangle.push_back(triangle);
		}
	}


	template <class T>
	TTriangulatedSurface<T>& TTriangulatedSurface<T>::operator+=(const TTriangulatedSurface<T>& surface)
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = surface.points_.begin(); p != surface.points_.end(); p++)
		{
			points_.push_back(*p);
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = surface.edges_.begin(); e != surface.edges_.end(); e++)
		{
			edges_.push_back(*e);
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = surface.triangles_.begin(); t != surface.triangles_.end(); t++)
		{
			triangles_.push_back(*t);
		}
		number_of_points_ += surface.number_of_points_;
		number_of_edges_ += surface.number_of_edges_;
		number_of_triangles_ += surface.number_of_triangles_;
		return *this;
	}


	template <class T>
	void TTriangulatedSurface<T>::join(TTriangulatedSurface<T>& source)
	{
		points_.splice(points_.end(),source.points_);
		edges_.splice(edges_.end(),source.edges_);
		triangles_.splice(triangles_.end(),source.triangles_);
		number_of_points_ += source.number_of_points_;
		number_of_edges_ += source.number_of_edges_;
		number_of_triangles_ += source.number_of_triangles_;
		source.number_of_points_ = 0;
		source.number_of_edges_ = 0;
		source.number_of_triangles_ = 0;
	}


	template <class T>
	void TTriangulatedSurface<T>::shift(const TVector3<T>& c)
	{
		typename std::list<TTrianglePoint<T>*>::iterator i;
		for (i = points_.begin(); i != points_.end(); i++)
		{
			(*i)->point_ += c;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::blowUp(const T& r)
	{
		typename std::list<TTrianglePoint<T>*>::iterator i;
		for (i = points_.begin(); i != points_.end(); i++)
		{
			(*i)->point_ *= r;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::refineSphere(Position iterations, bool out)
	{
		for (Position i = 0; i < iterations; i++)
		{
			refineSphere(out);
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			TVector3<T> norm( ((*t)->vertex_[1]->point_-(*t)->vertex_[0]->point_) %
												((*t)->vertex_[2]->point_-(*t)->vertex_[0]->point_)   );
			if (Maths::isGreater(norm*(*t)->vertex_[0]->point_,0) && (out == false))
			{
				TTrianglePoint<T>* temp = (*t)->vertex_[1];
				(*t)->vertex_[1] = (*t)->vertex_[2];
				(*t)->vertex_[2] = temp;
			}
			if (Maths::isLess(norm*(*t)->vertex_[0]->point_,0) && (out == true))
			{
				TTrianglePoint<T>* temp = (*t)->vertex_[1];
				(*t)->vertex_[1] = (*t)->vertex_[2];
				(*t)->vertex_[2] = temp;
			}
		}
		setIncidences();
	}


	template <class T>
	void TTriangulatedSurface<T>::refineSphere(bool out)
	{
		std::list<TTriangleEdge<T>*> new_edges;
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			TTrianglePoint<T>* point1 = (*e)->vertex_[0];
			TTrianglePoint<T>* point2 = (*e)->vertex_[1];
			TTrianglePoint<T>* new_point = new TTrianglePoint<T>;
			new_point->point_ = (point1->point_+point2->point_).normalize();
			if (out == true)
			{
				new_point->normal_ = new_point->point_;
			}
			else
			{
				new_point->normal_ = -new_point->point_;
			}
			(*e)->face_[0]->vertex_.push_back(new_point);
			(*e)->face_[1]->vertex_.push_back(new_point);
			points_.push_back(new_point);
			TTriangleEdge<T>* new_edge1 = new TTriangleEdge<T>;
			new_edge1->vertex_[0] = point1;
			new_edge1->vertex_[1] = new_point;
			(*e)->face_[0]->edge_.push_back(new_edge1);
			(*e)->face_[1]->edge_.push_back(new_edge1);
			new_edges.push_back(new_edge1);
			TTriangleEdge<T>* new_edge2 = new TTriangleEdge<T>;
			new_edge2->vertex_[0] = point2;
			new_edge2->vertex_[1] = new_point;
			(*e)->face_[0]->edge_.push_back(new_edge2);
			(*e)->face_[1]->edge_.push_back(new_edge2);
			new_edges.push_back(new_edge2);
		}
		std::list<TTriangle<T>*> new_triangles;
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			TTriangle<T>* current = *t;
			// create four new triangles
			std::vector<TTriangle<T>*> triangle(4);
			for (Position k = 0; k < 4; k++)
			{
				triangle[k] = new TTriangle<T>;
			}
			// create three new edges
			std::vector<TTriangleEdge<T>* > edge(3);
			for (Position k = 0; k < 3; k++)
			{
				edge[k] = new TTriangleEdge<T>;
			}
			// create a list of the edges created in the first for-loop that belong to current
			std::list<TTriangleEdge<T>*> edge_list;
			for (Position k = 3; k < 9; k++)
			{
				edge_list.push_back(current->edge_[k]);
			}
			for (Position k = 0; k < 3; k++)
			{
				// create a smaller triangle containing current->vertex_[k]
				TTriangleEdge<T>* first = NULL;
				TTriangleEdge<T>* second = NULL;
				TTrianglePoint<T>* p1 = NULL;
				TTrianglePoint<T>* p2 = NULL;
				TTrianglePoint<T>* p3 = current->vertex_[k];
				typename std::list<TTriangleEdge<T>*>::iterator l = edge_list.begin();
				while (first == NULL)
				{
					if ((*l)->vertex_[0]->point_ == p3->point_)
					{
						first = *l;
						p1 = (*l)->vertex_[1];
						edge_list.remove(*l);
					}
					else
					{
						if ((*l)->vertex_[1]->point_ == p3->point_)
						{
							first = *l;
							p1 = (*l)->vertex_[0];
							edge_list.remove(*l);
						}
					}
					l++;
				}
				l = edge_list.begin();
				while (second == NULL)
				{
					if ((*l)->vertex_[0]->point_ == p3->point_)
					{
						second = *l;
						p2 = (*l)->vertex_[1];
						edge_list.remove(*l);
					}
					else
					{
						if ((*l)->vertex_[1]->point_ == p3->point_)
						{
							second = *l;
							p2 = (*l)->vertex_[0];
							edge_list.remove(*l);
						}
					}
					l++;
				}
				triangle[k]->vertex_[0] = p1;
				triangle[k]->vertex_[1] = p2;
				triangle[k]->vertex_[2] = p3;
				triangle[k]->edge_[0] = first;
				triangle[k]->edge_[1] = second;
				triangle[k]->edge_[2] = edge[k];
				if (first->face_[0] == NULL)
				{
					first->face_[0] = triangle[k];
				}
				else
				{
					first->face_[1] = triangle[k];
				}
				if (second->face_[0] == NULL)
				{
					second->face_[0] = triangle[k];
				}
				else
				{
					second->face_[1] = triangle[k];
				}
				edge[k]->face_[0] = triangle[k];
				edge[k]->face_[1] = triangle[3];
				edge[k]->vertex_[0] = p1;
				edge[k]->vertex_[1] = p2;
			}
			triangle[3]->vertex_[0] = current->vertex_[3];
			triangle[3]->vertex_[1] = current->vertex_[4];
			triangle[3]->vertex_[2] = current->vertex_[5];
			triangle[3]->edge_[0] = edge[0];
			triangle[3]->edge_[1] = edge[1];
			triangle[3]->edge_[2] = edge[2];
			new_edges.push_back(edge[0]);
			new_edges.push_back(edge[1]);
			new_edges.push_back(edge[2]);
			new_triangles.push_back(triangle[0]);
			new_triangles.push_back(triangle[1]);
			new_triangles.push_back(triangle[2]);
			new_triangles.push_back(triangle[3]);
			delete current;
		}
		edges_ = new_edges;
		triangles_ = new_triangles;
		number_of_points_ += number_of_edges_;
		number_of_edges_ *= 4;
		number_of_triangles_ *= 4;
	}


	template <class T>
	void TTriangulatedSurface<T>::setIncidences()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			(*p)->edges_.clear();
			(*p)->faces_.clear();
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			(*e)->vertex_[0]->edges_.push_back(*e);
			(*e)->vertex_[0]->faces_.push_back((*e)->face_[0]);
			(*e)->vertex_[0]->faces_.push_back((*e)->face_[1]);
			(*e)->vertex_[1]->edges_.push_back(*e);
			(*e)->vertex_[1]->faces_.push_back((*e)->face_[0]);
			(*e)->vertex_[1]->faces_.push_back((*e)->face_[1]);
		}
		for (p = points_.begin(); p != points_.end(); p++)
		{
			(*p)->edges_.sort();
			(*p)->edges_.unique();
			(*p)->faces_.sort();
			(*p)->faces_.unique();
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::setIndices()
	{
		Index i = 0;
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			(*p)->index_ = i;
			i++;
		}
		i = 0;
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			(*e)->index_ = i;
			i++;
		}
		i = 0;
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			(*t)->index_ = i;
			i++;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::cut(const TPlane3<T>& plane, const T& fuzzy)
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		typename std::list<TTrianglePoint<T>*>::iterator next_point;
		T test_value;
		test_value = plane.n*plane.p+fuzzy;
		p = points_.begin();
		// delete all points on the wrong side of the plane
		while (p != points_.end())
		{
			if (Maths::isLessOrEqual(plane.n*(*p)->point_,test_value))
			{
				next_point = p;
				next_point++;
				if (next_point == points_.end())
				{
					remove(p);
					p = points_.end();
				}
				else
				{
					remove(p);
					p = next_point;
				}
			}
			else
			{
				p++;
			}
		}
		typename std::list<TTriangleEdge<T>*>::iterator e = edges_.begin();
		typename std::list<TTriangleEdge<T>*>::iterator next_edge;
		// delete all "isolated" edges (edges with no triangles)
		while (e != edges_.end())
		{
			if (((*e)->face_[0] == NULL) && ((*e)->face_[1] == NULL))
			{
				next_edge = e;
				next_edge++;
				(*e)->vertex_[0]->edges_.remove(*e);
				(*e)->vertex_[1]->edges_.remove(*e);
				delete *e;
				if (next_edge == edges_.end())
				{
					edges_.erase(e);
					e = edges_.end();
				}
				else
				{
					edges_.erase(e);
					e = next_edge;
				}
				number_of_edges_--;
			}
			else
			{
				e++;
			}
		}
		// delete all "isolated points" (points with no edge)
		p = points_.begin();
		while (p != points_.end())
		{
			if ((*p)->edges_.size() == 0)
			{
				next_point = p;
				next_point++;
				if (next_point == points_.end())
				{
					remove(p);
					p = points_.end();
				}
				else
				{
					remove(p);
					p = next_point;
				}
			}
			else
			{
				p++;
			}
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::getBorder(std::list<TTriangleEdge<T>*>& border)
	{
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (((*e)->face_[0] == NULL) || ((*e)->face_[1] == NULL))
			{
				border.push_back(*e);
			}
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::copy(const TTriangulatedSurface<T>& surface)
		throw(Exception::GeneralException)
	{
		std::vector<TTrianglePoint<T>*> point_vector(number_of_points_);
		Index i = 0;
		typename std::list<TTrianglePoint<T>*>::const_iterator p;
		for (p = surface.points_.begin(); p != surface.points_.end(); p++)
		{
			if (*p != NULL)
			{
				if ((*p)->index_ != i)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY TRIANGULATED SURFACE",
																						"ILLAGAL INDEX FOUND");
				}
				else
				{
					point_vector[i] = new TTrianglePoint<T>(**p,false);
					points_.push_back(point_vector[i]);
					i++;
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY TRIANGULATED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		std::vector<TTriangleEdge<T>*> edge_vector(number_of_edges_);
		i = 0;
		typename std::list<TTriangleEdge<T>*>::const_iterator e;
		for (e = surface.edges_.begin(); e != surface.edges_.end(); e++)
		{
			if (*e != NULL)
			{
				if ((*e)->index_ != i)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY TRIANGULATED SURFACE",
																						"ILLEGAL INDEX FOUND");
				}
				else
				{
					edge_vector[i] = new TTriangleEdge<T>(**e,false);
					edges_.push_back(edge_vector[i]);
					i++;
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY TRIANGULATED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		std::vector<TTriangle<T>*> triangle_vector(number_of_triangles_);
		i = 0;
		typename std::list<TTriangle<T>*>::const_iterator t;
		for (t = surface.triangles_.begin(); t != surface.triangles_.end(); t++)
		{
			if (*t != NULL)
			{
				if ((*t)->index_ != i)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY TRIANGULATED SURFACE",
																						"ILLEGAL INDEX FOUND");
				}
				else
				{
					triangle_vector[i] = new TTriangle<T>(**t,false);
					triangles_.push_back(triangle_vector[i]);
					i++;
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY TRIANGULATED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		i = 0;
		for (p = surface.points_.begin(); p != surface.points_.end(); p++)
		{
			for (e = (*p)->edges_.begin(); e != (*p)->edges_.end(); e++)
			{
				point_vector[i]->edges_.push_back(edge_vector[(*e)->index_]);
			}
			for (t = (*p)->faces_.begin(); t != (*p)->faces_.end(); t++)
			{
				point_vector[i]->faces_.push_back(triangle_vector[(*t)->index_]);
			}
			i++;
		}
		i = 0;
		for (e = surface.edges_.begin(); e != surface.edges_.end(); e++)
		{
			edge_vector[i]->vertex_[0] = point_vector[(*e)->vertex_[0]->index_];
			edge_vector[i]->vertex_[1] = point_vector[(*e)->vertex_[1]->index_];
			if ((*e)->face_[0] != NULL)
			{
				edge_vector[i]->face_[0] = triangle_vector[(*e)->face_[0]->index_];
			}
			if ((*e)->face_[1] != NULL)
			{
				edge_vector[i]->face_[1] = triangle_vector[(*e)->face_[1]->index_];
			}
			i++;
		}
		i = 0;
		for (t = surface.triangles_.begin(); t != surface.triangles_.end(); t++)
		{
			triangle_vector[i]->vertex_[0] = point_vector[(*t)->vertex_[0]->index_];
			triangle_vector[i]->vertex_[1] = point_vector[(*t)->vertex_[1]->index_];
			triangle_vector[i]->vertex_[2] = point_vector[(*t)->vertex_[2]->index_];
			triangle_vector[i]->edge_[0] = edge_vector[(*t)->edge_[0]->index_];
			triangle_vector[i]->edge_[1] = edge_vector[(*t)->edge_[1]->index_];
			triangle_vector[i]->edge_[2] = edge_vector[(*t)->edge_[2]->index_];
			i++;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::icosaeder(bool out)
	{
		TTrianglePoint<T>* point0  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point1  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point2  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point3  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point4  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point5  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point6  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point7  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point8  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point9  = new TTrianglePoint<T>;
		TTrianglePoint<T>* point10 = new TTrianglePoint<T>;
		TTrianglePoint<T>* point11 = new TTrianglePoint<T>;
		TTriangleEdge<T>* edge0  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge1  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge2  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge3  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge4  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge5  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge6  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge7  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge8  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge9  = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge10 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge11 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge12 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge13 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge14 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge15 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge16 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge17 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge18 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge19 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge20 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge21 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge22 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge23 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge24 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge25 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge26 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge27 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge28 = new  TTriangleEdge<T>;
		TTriangleEdge<T>* edge29 = new  TTriangleEdge<T>;
		TTriangle<T>* t0  = new TTriangle<T>;
		TTriangle<T>* t1  = new TTriangle<T>;
		TTriangle<T>* t2  = new TTriangle<T>;
		TTriangle<T>* t3  = new TTriangle<T>;
		TTriangle<T>* t4  = new TTriangle<T>;
		TTriangle<T>* t5  = new TTriangle<T>;
		TTriangle<T>* t6  = new TTriangle<T>;
		TTriangle<T>* t7  = new TTriangle<T>;
		TTriangle<T>* t8  = new TTriangle<T>;
		TTriangle<T>* t9  = new TTriangle<T>;
		TTriangle<T>* t10 = new TTriangle<T>;
		TTriangle<T>* t11 = new TTriangle<T>;
		TTriangle<T>* t12 = new TTriangle<T>;
		TTriangle<T>* t13 = new TTriangle<T>;
		TTriangle<T>* t14 = new TTriangle<T>;
		TTriangle<T>* t15 = new TTriangle<T>;
		TTriangle<T>* t16 = new TTriangle<T>;
		TTriangle<T>* t17 = new TTriangle<T>;
		TTriangle<T>* t18 = new TTriangle<T>;
		TTriangle<T>* t19 = new TTriangle<T>;
		number_of_points_ =12;
		number_of_edges_ = 30;
		number_of_triangles_ = 20;

		point0->point_  = TVector3<T>( 0.0     , 0.0     , 1.0      );
		point1->point_  = TVector3<T>( 0.894427, 0.0     , 0.4472135);
		point2->point_  = TVector3<T>( 0.276393, 0.850651, 0.4472135);
		point3->point_  = TVector3<T>(-0.723607, 0.525731, 0.4472135);
		point4->point_  = TVector3<T>(-0.723607,-0.525731, 0.4472135);
		point5->point_  = TVector3<T>( 0.276393,-0.850651, 0.4472135);
		point6->point_  = TVector3<T>( 0.723607, 0.525731,-0.4472135);
		point7->point_  = TVector3<T>(-0.276393, 0.850651,-0.4472135);
		point8->point_  = TVector3<T>(-0.894427, 0.0     ,-0.4472135);
		point9->point_  = TVector3<T>(-0.276393,-0.850651,-0.4472135);
		point10->point_ = TVector3<T>( 0.723607,-0.525731,-0.4472135);
		point11->point_ = TVector3<T>( 0.0     , 0.0     ,-1.0      );
		if (out == true)
		{
			point0->normal_  = point0->point_;
			point1->normal_  = point1->point_;
			point2->normal_  = point2->point_;
			point3->normal_  = point3->point_;
			point4->normal_  = point4->point_;
			point5->normal_  = point5->point_;
			point6->normal_  = point6->point_;
			point7->normal_  = point7->point_;
			point8->normal_  = point8->point_;
			point9->normal_  = point9->point_;
			point10->normal_ = point10->point_;
			point11->normal_ = point11->point_;
		}
		else
		{
			point0->normal_  = -point0->point_;
			point1->normal_  = -point1->point_;
			point2->normal_  = -point2->point_;
			point3->normal_  = -point3->point_;
			point4->normal_  = -point4->point_;
			point5->normal_  = -point5->point_;
			point6->normal_  = -point6->point_;
			point7->normal_  = -point7->point_;
			point8->normal_  = -point8->point_;
			point9->normal_  = -point9->point_;
			point10->normal_ = -point10->point_;
			point11->normal_ = -point11->point_;
		}
		point0->faces_.push_back(t0);   point0->faces_.push_back(t1);   point0->faces_.push_back(t2);
		point0->faces_.push_back(t3);   point0->faces_.push_back(t4);
		point1->faces_.push_back(t0);   point1->faces_.push_back(t4);   point1->faces_.push_back(t5);
		point1->faces_.push_back(t13);  point1->faces_.push_back(t14);
		point2->faces_.push_back(t0);   point2->faces_.push_back(t1);   point2->faces_.push_back(t6);
		point2->faces_.push_back(t7);   point2->faces_.push_back(t5);
		point3->faces_.push_back(t1);   point3->faces_.push_back(t2);   point3->faces_.push_back(t7);
		point3->faces_.push_back(t8);   point3->faces_.push_back(t9);
		point4->faces_.push_back(t2);   point4->faces_.push_back(t3);   point4->faces_.push_back(t9);
		point4->faces_.push_back(t10);  point4->faces_.push_back(t11);
		point5->faces_.push_back(t4);   point5->faces_.push_back(t11);  point5->faces_.push_back(t12);
		point5->faces_.push_back(t3);   point5->faces_.push_back(t14);
		point6->faces_.push_back(t5);   point6->faces_.push_back(t6);   point6->faces_.push_back(t13);
		point6->faces_.push_back(t15);  point6->faces_.push_back(t19);
		point7->faces_.push_back(t6);   point7->faces_.push_back(t7);   point7->faces_.push_back(t8);
		point7->faces_.push_back(t15);  point7->faces_.push_back(t16);
		point8->faces_.push_back(t8);   point8->faces_.push_back(t9);   point8->faces_.push_back(t10);
		point8->faces_.push_back(t16);  point8->faces_.push_back(t17);
		point9->faces_.push_back(t10);  point9->faces_.push_back(t11);  point9->faces_.push_back(t12);
		point9->faces_.push_back(t17);  point9->faces_.push_back(t18);
		point10->faces_.push_back(t12); point10->faces_.push_back(t13); point10->faces_.push_back(t14);
		point10->faces_.push_back(t18); point10->faces_.push_back(t19);
		point11->faces_.push_back(t15); point11->faces_.push_back(t16); point11->faces_.push_back(t17);
		point11->faces_.push_back(t18); point11->faces_.push_back(t19);
		point0->edges_.push_back(edge0);   point0->edges_.push_back(edge1);   point0->edges_.push_back(edge3);
		point0->edges_.push_back(edge5);   point0->edges_.push_back(edge7);
		point1->edges_.push_back(edge1);   point1->edges_.push_back(edge2);   point1->edges_.push_back(edge9);
		point1->edges_.push_back(edge10);  point1->edges_.push_back(edge23);
		point2->edges_.push_back(edge0);   point2->edges_.push_back(edge2);   point2->edges_.push_back(edge4);
		point2->edges_.push_back(edge11);  point2->edges_.push_back(edge12);
		point3->edges_.push_back(edge3);   point3->edges_.push_back(edge4);   point3->edges_.push_back(edge6);
		point3->edges_.push_back(edge14);  point3->edges_.push_back(edge15);
		point4->edges_.push_back(edge5);   point4->edges_.push_back(edge6);   point4->edges_.push_back(edge8);
		point4->edges_.push_back(edge17);  point4->edges_.push_back(edge18);
		point5->edges_.push_back(edge7);   point5->edges_.push_back(edge8);   point5->edges_.push_back(edge9);
		point5->edges_.push_back(edge20);  point5->edges_.push_back(edge21);
		point6->edges_.push_back(edge10);  point6->edges_.push_back(edge11);  point6->edges_.push_back(edge13);
		point6->edges_.push_back(edge24);  point6->edges_.push_back(edge25);
		point7->edges_.push_back(edge12);  point7->edges_.push_back(edge13);  point7->edges_.push_back(edge14);
		point7->edges_.push_back(edge16);  point7->edges_.push_back(edge26);
		point8->edges_.push_back(edge15);  point8->edges_.push_back(edge16);  point8->edges_.push_back(edge17);
		point8->edges_.push_back(edge19);  point8->edges_.push_back(edge27);
		point9->edges_.push_back(edge18);  point9->edges_.push_back(edge19);  point9->edges_.push_back(edge20);
		point9->edges_.push_back(edge22);  point9->edges_.push_back(edge28);
		point10->edges_.push_back(edge21); point10->edges_.push_back(edge22); point10->edges_.push_back(edge23);
		point10->edges_.push_back(edge24); point10->edges_.push_back(edge29);
		point11->edges_.push_back(edge25); point11->edges_.push_back(edge26); point11->edges_.push_back(edge27);
		point11->edges_.push_back(edge28); point11->edges_.push_back(edge29);
		points_.push_back(point0);
		points_.push_back(point1);
		points_.push_back(point2);
		points_.push_back(point3);
		points_.push_back(point4);
		points_.push_back(point5);
		points_.push_back(point6);
		points_.push_back(point7);
		points_.push_back(point8);
		points_.push_back(point9);
		points_.push_back(point10);
		points_.push_back(point11);

		edge0->vertex_[0]  = point2;  edge0->vertex_[1]  = point0;
		edge1->vertex_[0]  = point0;  edge1->vertex_[1]  = point1;
		edge2->vertex_[0]  = point1;  edge2->vertex_[1]  = point2;
		edge3->vertex_[0]  = point3;  edge3->vertex_[1]  = point0;
		edge4->vertex_[0]  = point2;  edge4->vertex_[1]  = point3;
		edge5->vertex_[0]  = point4;  edge5->vertex_[1]  = point0;
		edge6->vertex_[0]  = point3;  edge6->vertex_[1]  = point4;
		edge7->vertex_[0]  = point5;  edge7->vertex_[1]  = point0;
		edge8->vertex_[0]  = point4;  edge8->vertex_[1]  = point5;
		edge9->vertex_[0]  = point5;  edge9->vertex_[1]  = point1;
		edge10->vertex_[0] = point1;  edge10->vertex_[1] = point6;
		edge11->vertex_[0] = point6;  edge11->vertex_[1] = point2;
		edge12->vertex_[0] = point7;  edge12->vertex_[1] = point2;
		edge13->vertex_[0] = point6;  edge13->vertex_[1] = point7;
		edge14->vertex_[0] = point7;  edge14->vertex_[1] = point3;
		edge15->vertex_[0] = point8;  edge15->vertex_[1] = point3;
		edge16->vertex_[0] = point7;  edge16->vertex_[1] = point8;
		edge17->vertex_[0] = point8;  edge17->vertex_[1] = point4;
		edge18->vertex_[0] = point9;  edge18->vertex_[1] = point4;
		edge19->vertex_[0] = point8;  edge19->vertex_[1] = point9;
		edge20->vertex_[0] = point9;  edge20->vertex_[1] = point5;
		edge21->vertex_[0] = point10; edge21->vertex_[1] = point5;
		edge22->vertex_[0] = point9;  edge22->vertex_[1] = point10;
		edge23->vertex_[0] = point1;  edge23->vertex_[1] = point10;
		edge24->vertex_[0] = point10; edge24->vertex_[1] = point6;
		edge25->vertex_[0] = point6;  edge25->vertex_[1] = point11;
		edge26->vertex_[0] = point11; edge26->vertex_[1] = point7;
		edge27->vertex_[0] = point11; edge27->vertex_[1] = point8;
		edge28->vertex_[0] = point11; edge28->vertex_[1] = point9;
		edge29->vertex_[0] = point11; edge29->vertex_[1] = point10;
		edge0->face_[0]  = t0;  edge0->face_[1]  = t1;
		edge1->face_[0]  = t0;  edge1->face_[1]  = t4;
		edge2->face_[0]  = t0;  edge2->face_[1]  = t5;
		edge3->face_[0]  = t1;  edge3->face_[1]  = t2;
		edge4->face_[0]  = t1;  edge4->face_[1]  = t7;
		edge5->face_[0]  = t2;  edge5->face_[1]  = t3;
		edge6->face_[0]  = t2;  edge6->face_[1]  = t9;
		edge7->face_[0]  = t3;  edge7->face_[1]  = t4;
		edge8->face_[0]  = t3;  edge8->face_[1]  = t11;
		edge9->face_[0]  = t4;  edge9->face_[1]  = t14;
		edge10->face_[0] = t5;  edge10->face_[1] = t13;
		edge11->face_[0] = t5;  edge11->face_[1] = t6;
		edge12->face_[0] = t6;  edge12->face_[1] = t7;
		edge13->face_[0] = t6;  edge13->face_[1] = t15;
		edge14->face_[0] = t7;  edge14->face_[1] = t8;
		edge15->face_[0] = t8;  edge15->face_[1] = t9;
		edge16->face_[0] = t8;  edge16->face_[1] = t16;
		edge17->face_[0] = t9;  edge17->face_[1] = t10;
		edge18->face_[0] = t10; edge18->face_[1] = t11;
		edge19->face_[0] = t10; edge19->face_[1] = t17;
		edge20->face_[0] = t11; edge20->face_[1] = t12;
		edge21->face_[0] = t12; edge21->face_[1] = t14;
		edge22->face_[0] = t12; edge22->face_[1] = t18;
		edge23->face_[0] = t13; edge23->face_[1] = t14;
		edge24->face_[0] = t13; edge24->face_[1] = t19;
		edge25->face_[0] = t15; edge25->face_[1] = t19;
		edge26->face_[0] = t15; edge26->face_[1] = t16;
		edge27->face_[0] = t16; edge27->face_[1] = t17;
		edge28->face_[0] = t17; edge28->face_[1] = t18;
		edge29->face_[0] = t18; edge29->face_[1] = t19;
		edges_.push_back(edge0);
		edges_.push_back(edge1);
		edges_.push_back(edge2);
		edges_.push_back(edge3);
		edges_.push_back(edge4);
		edges_.push_back(edge5);
		edges_.push_back(edge6);
		edges_.push_back(edge7);
		edges_.push_back(edge8);
		edges_.push_back(edge9);
		edges_.push_back(edge10);
		edges_.push_back(edge11);
		edges_.push_back(edge12);
		edges_.push_back(edge13);
		edges_.push_back(edge14);
		edges_.push_back(edge15);
		edges_.push_back(edge16);
		edges_.push_back(edge17);
		edges_.push_back(edge18);
		edges_.push_back(edge19);
		edges_.push_back(edge20);
		edges_.push_back(edge21);
		edges_.push_back(edge22);
		edges_.push_back(edge23);
		edges_.push_back(edge24);
		edges_.push_back(edge25);
		edges_.push_back(edge26);
		edges_.push_back(edge27);
		edges_.push_back(edge28);
		edges_.push_back(edge29);

		t0->vertex_[0]  = point2;  t0->vertex_[1]  = point0;  t0->vertex_[2]  = point1;
		t1->vertex_[0]  = point3;  t1->vertex_[1]  = point0;  t1->vertex_[2]  = point2;
		t2->vertex_[0]  = point4;  t2->vertex_[1]  = point0;  t2->vertex_[2]  = point3;
		t3->vertex_[0]  = point5;  t3->vertex_[1]  = point0;  t3->vertex_[2]  = point4;
		t4->vertex_[0]  = point1;  t4->vertex_[1]  = point0;  t4->vertex_[2]  = point5;
		t5->vertex_[0]  = point2;  t5->vertex_[1]  = point1;  t5->vertex_[2]  = point6;
		t6->vertex_[0]  = point7;  t6->vertex_[1]  = point2;  t6->vertex_[2]  = point6;
		t7->vertex_[0]  = point3;  t7->vertex_[1]  = point2;  t7->vertex_[2]  = point7;
		t8->vertex_[0]  = point8;  t8->vertex_[1]  = point3;  t8->vertex_[2]  = point7;
		t9->vertex_[0]  = point4;  t9->vertex_[1]  = point3;  t9->vertex_[2]  = point8;
		t10->vertex_[0] = point9;  t10->vertex_[1] = point4;  t10->vertex_[2] = point8;
		t11->vertex_[0] = point5;  t11->vertex_[1] = point4;  t11->vertex_[2] = point9;
		t12->vertex_[0] = point10; t12->vertex_[1] = point5;  t12->vertex_[2] = point9;
		t13->vertex_[0] = point6;  t13->vertex_[1] = point1;  t13->vertex_[2] = point10;
		t14->vertex_[0] = point1;  t14->vertex_[1] = point5;  t14->vertex_[2] = point10;
		t15->vertex_[0] = point6;  t15->vertex_[1] = point11; t15->vertex_[2] = point7;
		t16->vertex_[0] = point7;  t16->vertex_[1] = point11; t16->vertex_[2] = point8;
		t17->vertex_[0] = point8;  t17->vertex_[1] = point11; t17->vertex_[2] = point9;
		t18->vertex_[0] = point9;  t18->vertex_[1] = point11; t18->vertex_[2] = point10;
		t19->vertex_[0] = point10; t19->vertex_[1] = point11; t19->vertex_[2] = point6;
		t0->edge_[0]  = edge0;  t0->edge_[1]  = edge1;  t0->edge_[2]  = edge2;
		t1->edge_[0]  = edge0;  t1->edge_[1]  = edge3;  t1->edge_[2]  = edge4;
		t2->edge_[0]  = edge3;  t2->edge_[1]  = edge5;  t2->edge_[2]  = edge6;
		t3->edge_[0]  = edge5;  t3->edge_[1]  = edge7;  t3->edge_[2]  = edge8;
		t4->edge_[0]  = edge1;  t4->edge_[1]  = edge7;  t4->edge_[2]  = edge9;
		t5->edge_[0]  = edge2;  t5->edge_[1]  = edge10; t5->edge_[2]  = edge11;
		t6->edge_[0]  = edge11; t6->edge_[1]  = edge12; t6->edge_[2]  = edge13;
		t7->edge_[0]  = edge4;  t7->edge_[1]  = edge12; t7->edge_[2]  = edge14;
		t8->edge_[0]  = edge14; t8->edge_[1]  = edge15; t8->edge_[2]  = edge16;
		t9->edge_[0]  = edge6;  t9->edge_[1]  = edge15; t9->edge_[2]  = edge17;
		t10->edge_[0] = edge17; t10->edge_[1] = edge18; t10->edge_[2] = edge19;
		t11->edge_[0] = edge8;  t11->edge_[1] = edge18; t11->edge_[2] = edge20;
		t12->edge_[0] = edge20; t12->edge_[1] = edge21; t12->edge_[2] = edge22;
		t13->edge_[0] = edge10; t13->edge_[1] = edge23; t13->edge_[2] = edge24;
		t14->edge_[0] = edge9;  t14->edge_[1] = edge21; t14->edge_[2] = edge23;
		t15->edge_[0] = edge13; t15->edge_[1] = edge25; t15->edge_[2] = edge26;
		t16->edge_[0] = edge16; t16->edge_[1] = edge26; t16->edge_[2] = edge27;
		t17->edge_[0] = edge19; t17->edge_[1] = edge27; t17->edge_[2] = edge28;
		t18->edge_[0] = edge22; t18->edge_[1] = edge28; t18->edge_[2] = edge29;
		t19->edge_[0] = edge24; t19->edge_[1] = edge25; t19->edge_[2] = edge29;
		triangles_.push_back(t0);
		triangles_.push_back(t1);
		triangles_.push_back(t2);
		triangles_.push_back(t3);
		triangles_.push_back(t4);
		triangles_.push_back(t5);
		triangles_.push_back(t6);
		triangles_.push_back(t7);
		triangles_.push_back(t8);
		triangles_.push_back(t9);
		triangles_.push_back(t10);
		triangles_.push_back(t11);
		triangles_.push_back(t12);
		triangles_.push_back(t13);
		triangles_.push_back(t14);
		triangles_.push_back(t15);
		triangles_.push_back(t16);
		triangles_.push_back(t17);
		triangles_.push_back(t18);
		triangles_.push_back(t19);
	}

}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSURFACE_H


