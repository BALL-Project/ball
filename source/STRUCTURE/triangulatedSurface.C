// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSurface.C,v 1.4 2002/12/17 14:13:21 anker Exp $

#include <BALL/STRUCTURE/triangulatedSurface.h>

namespace BALL
{

	TriangulatedSurface::TriangulatedSurface()
		throw()
		:	number_of_points_(0),
			points_(),
			number_of_edges_(0),
			edges_(),
			number_of_triangles_(0),
			triangles_()
	{
	}


	TriangulatedSurface::TriangulatedSurface
			(const TriangulatedSurface& surface, bool)
		throw()
		:	number_of_points_(0),
			points_(),
			number_of_edges_(0),
			edges_(),
			number_of_triangles_(0),
			triangles_()
	{
		copy(surface);
	}


	TriangulatedSurface::~TriangulatedSurface()
		throw()
	{
		clear();
	}


	void TriangulatedSurface::clear()
		throw()
	{
		std::list<TrianglePoint*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			delete *p;
		}
		std::list<TriangleEdge*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			delete *e;
		}
		std::list<Triangle*>::iterator t;
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
	}


	void TriangulatedSurface::set
			(const TriangulatedSurface& surface, bool)
		throw()
	{
		if (this != &surface)
		{
			copy(surface);
		}
	}


	TriangulatedSurface& TriangulatedSurface::operator =
			(const TriangulatedSurface& surface)
		throw()
	{
		if (this != &surface)
		{
			copy(surface);
		}
		return *this;
	}


	void TriangulatedSurface::insert(TrianglePoint* p)
		throw()
	{
		points_.push_back(p);
		number_of_points_++;
	}


	void TriangulatedSurface::insert(TriangleEdge* e)
		throw()
	{
		edges_.push_back(e);
		number_of_edges_++;
	}


	void TriangulatedSurface::insert(Triangle* t)
		throw()
	{
		triangles_.push_back(t);
		number_of_triangles_++;
	}


	Size TriangulatedSurface::numberOfPoints() const
		throw()
	{
		return number_of_points_;
	}


	Size TriangulatedSurface::numberOfEdges() const
		throw()
	{
		return number_of_edges_;
	}


	Size TriangulatedSurface::numberOfTriangles() const
		throw()
	{
		return number_of_triangles_;
	}


	void TriangulatedSurface::remove(TrianglePoint* point, bool deep)
		throw()
	{
		if (deep)
		{
			HashSet<Triangle*> delete_triangles = point->faces_;
			HashSet<Triangle*>::Iterator t;
			for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
			{
				(*t)->vertex_[0]->faces_.erase(*t);
				(*t)->vertex_[1]->faces_.erase(*t);
				(*t)->vertex_[2]->faces_.erase(*t);
				(*t)->edge_[0]->remove(*t);
				(*t)->edge_[1]->remove(*t);
				(*t)->edge_[2]->remove(*t);
				triangles_.remove(*t);
				number_of_triangles_--;
				delete *t;
			}
			HashSet<TriangleEdge*> delete_edges = point->edges_;
			HashSet<TriangleEdge*>::Iterator e;
			for (e = delete_edges.begin(); e != delete_edges.end(); e++)
			{
				(*e)->vertex_[0]->edges_.erase(*e);
				(*e)->vertex_[1]->edges_.erase(*e);
				edges_.remove(*e);
				number_of_edges_--;
				delete *e;
			}
		}
		points_.remove(point);
		number_of_points_--;
		delete point;
	}


	void TriangulatedSurface::remove(PointIterator point, bool deep)
		throw()
	{
		if (deep)
		{
			HashSet<Triangle*> delete_triangles = (*point)->faces_;
			HashSet<Triangle*>::Iterator t;
			for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
			{
				(*t)->vertex_[0]->faces_.erase(*t);
				(*t)->vertex_[1]->faces_.erase(*t);
				(*t)->vertex_[2]->faces_.erase(*t);
				(*t)->edge_[0]->remove(*t);
				(*t)->edge_[1]->remove(*t);
				(*t)->edge_[2]->remove(*t);
				triangles_.remove(*t);
				number_of_triangles_--;
				delete *t;
			}
			HashSet<TriangleEdge*> delete_edges = (*point)->edges_;
			HashSet<TriangleEdge*>::Iterator e;
			for (e = delete_edges.begin(); e != delete_edges.end(); e++)
			{
				(*e)->vertex_[0]->edges_.erase(*e);
				(*e)->vertex_[1]->edges_.erase(*e);
				edges_.remove(*e);
				number_of_edges_--;
				delete *e;
			}
		}
		points_.erase(point);
		number_of_points_--;
		delete *point;
	}


	void TriangulatedSurface::remove(TriangleEdge* edge, bool deep)
		throw()
	{
		if (deep)
		{
			remove(edge->face_[0],true);
			remove(edge->face_[0],true);
			edge->vertex_[0]->edges_.erase(edge);
			edge->vertex_[1]->edges_.erase(edge);
		}
		edges_.remove(edge);
		number_of_edges_--;
		delete edge;
	}


	void TriangulatedSurface::remove(EdgeIterator e, bool deep)
		throw()
	{
		if (deep)
		{
			remove((*e)->face_[0],true);
			remove((*e)->face_[0],true);
			(*e)->vertex_[0]->edges_.erase(*e);
			(*e)->vertex_[1]->edges_.erase(*e);
		}
		edges_.erase(e);
		number_of_edges_--;
		delete *e;
	}


	void TriangulatedSurface::remove(Triangle* triangle, bool deep)
		throw()
	{
		if (deep)
		{
			triangle->vertex_[0]->faces_.erase(triangle);
			triangle->vertex_[1]->faces_.erase(triangle);
			triangle->vertex_[2]->faces_.erase(triangle);
			triangle->edge_[0]->remove(triangle);
			triangle->edge_[1]->remove(triangle);
			triangle->edge_[2]->remove(triangle);
		}
		triangles_.remove(triangle);
		number_of_triangles_--;
		delete triangle;
	}


	void TriangulatedSurface::remove(TriangleIterator t, bool deep)
		throw()
	{
		if (deep)
		{
			(*t)->vertex_[0]->faces_.erase(*t);
			(*t)->vertex_[1]->faces_.erase(*t);
			(*t)->vertex_[2]->faces_.erase(*t);
			(*t)->edge_[0]->remove(*t);
			(*t)->edge_[1]->remove(*t);
			(*t)->edge_[2]->remove(*t);
		}
		triangles_.erase(t);
		number_of_triangles_--;
		delete *t;
	}


	void TriangulatedSurface::exportSurface(Surface& surface)
		throw()
	{
		std::list<TrianglePoint*>::iterator p;
		Index i = 0;
		Vector3 point;
		Vector3 normal;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			point.set((float)(*p)->point_.x,
										 (float)(*p)->point_.y,
										 (float)(*p)->point_.z);
			normal.set((float)(*p)->normal_.x,
										 (float)(*p)->normal_.y,
										 (float)(*p)->normal_.z);
			surface.vertex.push_back(point);
			surface.normal.push_back(normal);
			(*p)->index_ = i;
			i++;
		}
		std::list<Triangle*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			Surface::Triangle triangle;
			triangle.v1 = (*t)->vertex_[0]->index_;
			triangle.v2 = (*t)->vertex_[1]->index_;
			triangle.v3 = (*t)->vertex_[2]->index_;
			surface.triangle.push_back(triangle);
		}
	}


	TriangulatedSurface& TriangulatedSurface::operator +=
			(const TriangulatedSurface& surface)
		throw()
	{
		std::list<TrianglePoint*>::const_iterator p;
		for (p = surface.points_.begin(); p != surface.points_.end(); p++)
		{
			points_.push_back(*p);
		}
		std::list<TriangleEdge*>::const_iterator e;
		for (e = surface.edges_.begin(); e != surface.edges_.end(); e++)
		{
			edges_.push_back(*e);
		}
		std::list<Triangle*>::const_iterator t;
		for (t = surface.triangles_.begin(); t != surface.triangles_.end(); t++)
		{
			triangles_.push_back(*t);
		}
		number_of_points_ += surface.number_of_points_;
		number_of_edges_ += surface.number_of_edges_;
		number_of_triangles_ += surface.number_of_triangles_;
		return *this;
	}


	void TriangulatedSurface::join(TriangulatedSurface& source)
		throw()
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


	void TriangulatedSurface::shift(const TVector3<double>& c)
	{
		std::list<TrianglePoint*>::iterator i;
		for (i = points_.begin(); i != points_.end(); i++)
		{
			(*i)->point_ += c;
		}
	}


	void TriangulatedSurface::blowUp(const double& r)
	{
		std::list<TrianglePoint*>::iterator i;
		for (i = points_.begin(); i != points_.end(); i++)
		{
			(*i)->point_ *= r;
		}
	}


	void TriangulatedSurface::setIndices()
	{
		Index i = 0;
		std::list<TrianglePoint*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			(*p)->index_ = i;
			i++;
		}
		i = 0;
		std::list<TriangleEdge*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			(*e)->index_ = i;
			i++;
		}
		i = 0;
		std::list<Triangle*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			(*t)->index_ = i;
			i++;
		}
	}


	void TriangulatedSurface::cut(const TPlane3<double>& plane, const double& fuzzy)
	{
		// delete all points on the wrong side of the plane
		std::list<TrianglePoint*>::iterator p;
		std::list<TrianglePoint*>::iterator next_point;
		double test_value;
		test_value = plane.n*plane.p+fuzzy;
		p = points_.begin();
		while (p != points_.end())
		{
			if (Maths::isLessOrEqual(plane.n*(*p)->point_,test_value))
			{
				next_point = p;
				next_point++;
				delete *p;
				if (next_point == points_.end())
				{
					points_.erase(p);
					p = points_.end();
				}
				else
				{
					points_.erase(p);
					p = next_point;
				}
				number_of_points_--;
			}
			else
			{
				p++;
			}
		}
	}


	void TriangulatedSurface::shrink()
	{
		// delete all border triangles
		std::list<Triangle*> delete_triangles;
		std::list<Triangle*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			if (((*t)->edge_[0]->face_[0] == NULL) || ((*t)->edge_[0]->face_[1] == NULL) ||
					((*t)->edge_[1]->face_[0] == NULL) || ((*t)->edge_[1]->face_[1] == NULL) ||
					((*t)->edge_[2]->face_[0] == NULL) || ((*t)->edge_[2]->face_[1] == NULL)		)
			{
				delete_triangles.push_back(*t);
			}
		}
		for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
		{
			remove(*t,true);
		}
		// delete all "isolated" edges (edges with no triangles)
		std::list<TriangleEdge*>::iterator e = edges_.begin();
		std::list<TriangleEdge*>::iterator next_edge;
		while (e != edges_.end())
		{
			if (((*e)->face_[0] == NULL) && ((*e)->face_[1] == NULL))
			{
				next_edge = e;
				next_edge++;
				(*e)->vertex_[0]->edges_.erase(*e);
				(*e)->vertex_[1]->edges_.erase(*e);
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
	}


	void TriangulatedSurface::deleteIsolatedPoints()
		throw()
	{
		std::list<TrianglePoint*>::iterator p1;
		std::list<TrianglePoint*>::iterator p2;
		p1 = points_.begin();
		while (p1 != points_.end())
		{
			if ((*p1)->faces_.size() == 0)
			{
				p2 = p1;
				p2++;
				if (p2 == points_.end())
				{
					points_.erase(p1);
					p1 = points_.end();
				}
				else
				{
					points_.erase(p1);
					p1 = p2;
				}
				number_of_points_--;
			}
			else
			{
				p1++;
			}
		}
	}


	void TriangulatedSurface::getBorder(std::list<TriangleEdge*>& border)
	{
		std::list<TriangleEdge*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (((*e)->face_[0] == NULL) || ((*e)->face_[1] == NULL))
			{
				border.push_back(*e);
			}
		}
	}


	TriangulatedSurface::PointIterator
			TriangulatedSurface::beginPoint()
		throw()
	{
		return points_.begin();
	}


	TriangulatedSurface::ConstPointIterator
			TriangulatedSurface::beginPoint() const
		throw()
	{
		return points_.begin();
	}


	TriangulatedSurface::PointIterator
			TriangulatedSurface::endPoint()
		throw()
	{
		return points_.end();
	}


	TriangulatedSurface::ConstPointIterator
			TriangulatedSurface::endPoint() const
		throw()
	{
		return points_.end();
	}


	TriangulatedSurface::EdgeIterator
			TriangulatedSurface::beginEdge()
		throw()
	{
		return edges_.begin();
	}


	TriangulatedSurface::ConstEdgeIterator
			TriangulatedSurface::beginEdge() const
		throw()
	{
		return edges_.begin();
	}


	TriangulatedSurface::EdgeIterator
			TriangulatedSurface::endEdge()
		throw()
	{
		return edges_.end();
	}


	TriangulatedSurface::ConstEdgeIterator
			TriangulatedSurface::endEdge() const
		throw()
	{
		return edges_.end();
	}


	TriangulatedSurface::TriangleIterator
			TriangulatedSurface::beginTriangle()
		throw()
	{
		return triangles_.begin();
	}


	TriangulatedSurface::ConstTriangleIterator
			TriangulatedSurface::beginTriangle() const
		throw()
	{
		return triangles_.begin();
	}


	TriangulatedSurface::TriangleIterator
			TriangulatedSurface::endTriangle()
		throw()
	{
		return triangles_.end();
	}


	TriangulatedSurface::ConstTriangleIterator
			TriangulatedSurface::endTriangle() const
		throw()
	{
		return triangles_.end();
	}


	bool TriangulatedSurface::canBeCopied() const
		throw()
	{
		std::list<TrianglePoint*>::const_iterator p;
		Index i = 0;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			if (*p == NULL)
			{
				return false;
			}
			if ((*p)->index_ != i)
			{
				return false;
			}
			i++;
		}
		std::list<TriangleEdge*>::const_iterator e;
		i = 0;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			if (*e == NULL)
			{
				return false;
			}
			if ((*e)->index_ != i)
			{
				return false;
			}
			i++;
		}
		std::list<Triangle*>::const_iterator t;
		i = 0;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			if (*t == NULL)
			{
				return false;
			}
			if ((*t)->index_ != i)
			{
				return false;
			}
			i++;
		}
		return true;
	}


	void TriangulatedSurface::copy(const TriangulatedSurface& surface)
		throw()
	{
		if (surface.canBeCopied())
		{
			number_of_points_ = surface.number_of_points_;
			number_of_edges_ = surface.number_of_edges_;
			number_of_triangles_ = surface.number_of_triangles_;
			std::vector<TrianglePoint*> point_vector(number_of_points_);
			std::list<TrianglePoint*>::const_iterator p;
			Position i = 0;
			for (p = surface.points_.begin(); p != surface.points_.end(); p++)
			{
				point_vector[i] = new TrianglePoint(**p,false);
				points_.push_back(point_vector[i]);
				i++;
			}
			std::vector<TriangleEdge*> edge_vector(number_of_edges_);
			std::list<TriangleEdge*>::const_iterator e;
			i = 0;
			for (e = surface.edges_.begin(); e != surface.edges_.end(); e++)
			{
				edge_vector[i] = new TriangleEdge(**e,false);
				edges_.push_back(edge_vector[i]);
				i++;
			}
			std::vector<Triangle*> triangle_vector(number_of_triangles_);
			std::list<Triangle*>::const_iterator t;
			i = 0;
			for (t = surface.triangles_.begin(); t != surface.triangles_.end(); t++)
			{
				triangle_vector[i] = new Triangle(**t,false);
				triangles_.push_back(triangle_vector[i]);
				i++;
			}
			HashSet<TriangleEdge*>::ConstIterator he;
			HashSet<Triangle*>::ConstIterator ht;
			i = 0;
			for (p = surface.points_.begin(); p != surface.points_.end(); p++)
			{
				for (he = (*p)->edges_.begin(); he != (*p)->edges_.end(); he++)
				{
					point_vector[i]->edges_.insert(edge_vector[(*he)->index_]);
				}
				for (ht = (*p)->faces_.begin(); ht != (*p)->faces_.end(); ht++)
				{
					point_vector[i]->faces_.insert(triangle_vector[(*ht)->index_]);
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
	}

///////////////////////////////////////////////////////////////////////////////


	TriangulatedSphere::TriangulatedSphere()
		throw()
		:	TriangulatedSurface()
	{
	}


	TriangulatedSphere::TriangulatedSphere
			(const TriangulatedSphere& sphere, bool)
		throw()
		:	TriangulatedSurface(sphere)
	{
	}


	TriangulatedSphere::~TriangulatedSphere()
		throw()
	{
	}


	void TriangulatedSphere::set(const TriangulatedSphere& sphere, bool)
		throw()
	{
		if (this != &sphere)
		{
			copy(sphere);
		}
	}


	TriangulatedSphere& TriangulatedSphere::operator =
			(const TriangulatedSphere& sphere)
		throw()
	{
		if (this != &sphere)
		{
			copy(sphere);
		}
		return *this;
	}


	void TriangulatedSphere::refine(Position iterations, bool out)
	{
		for (Position i = 0; i < iterations; i++)
		{
			refine(out);
		}
		std::list<Triangle*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			TVector3<double> norm(((*t)->vertex_[1]->point_-(*t)->vertex_[0]->point_) %
											 ((*t)->vertex_[2]->point_-(*t)->vertex_[0]->point_)	);
			if ((Maths::isGreater(norm*(*t)->vertex_[0]->point_,0) &&
					 (out == false)																				) ||
					(Maths::isLess(norm*(*t)->vertex_[0]->point_,0) &&
					 (out == true)																				)		)
			{
				TrianglePoint* temp = (*t)->vertex_[1];
				(*t)->vertex_[1] = (*t)->vertex_[2];
				(*t)->vertex_[2] = temp;
			}
		}
		setIncidences();
	}


	void TriangulatedSphere::refine(bool out)
	{
		std::vector<Face> faces(number_of_triangles_);
		std::list<Triangle*>::iterator t;
		Position i = 0;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			(*t)->index_ = i;
			faces[i].p[0] = (*t)->vertex_[0];
			faces[i].p[1] = (*t)->vertex_[1];
			faces[i].p[2] = (*t)->vertex_[2];
			faces[i].pcount = 3;
			faces[i].ecount = 0;
			i++;
		}
		std::list<TriangleEdge*> new_edges;
		std::list<TriangleEdge*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			TrianglePoint* point1 = (*e)->vertex_[0];
			TrianglePoint* point2 = (*e)->vertex_[1];
			TrianglePoint* new_point = new TrianglePoint;
			new_point->point_ = (point1->point_+point2->point_).normalize();
			if (out == true)
			{
				new_point->normal_ = new_point->point_;
			}
			else
			{
				new_point->normal_ = -new_point->point_;
			}
			TriangleEdge* new_edge1 = *e;
			new_edge1->vertex_[0] = point1;
			new_edge1->vertex_[1] = new_point;
			TriangleEdge* new_edge2 = new TriangleEdge;
			new_edge2->vertex_[0] = point2;
			new_edge2->vertex_[1] = new_point;
			i = (*e)->face_[0]->index_;
			faces[i].p[faces[i].pcount] = new_point;
			faces[i].pcount++;
			faces[i].e[faces[i].ecount] = new_edge1;
			faces[i].e[faces[i].ecount+1] = new_edge2;
			faces[i].ecount += 2;
			i = (*e)->face_[1]->index_;
			faces[i].p[faces[i].pcount] = new_point;
			faces[i].pcount++;
			faces[i].e[faces[i].ecount] = new_edge1;
			faces[i].e[faces[i].ecount+1] = new_edge2;
			faces[i].ecount += 2;
			new_edge1->face_[0] = NULL;
			new_edge1->face_[1] = NULL;
			new_edge2->face_[0] = NULL;
			new_edge2->face_[1] = NULL;
			points_.push_back(new_point);
			new_edges.push_back(new_edge2);
		}
		edges_.splice(edges_.end(),new_edges);
		i = 0;
		std::list<Triangle*> new_triangles;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			// create four new triangles
			Triangle* triangle[3];
			for (Position k = 0; k < 3; k++)
			{
				triangle[k] = new Triangle;
			}
			// create three new edges
			for (Position k = 6; k < 9; k++)
			{
				faces[i].e[k] = new TriangleEdge;
				faces[i].e[k]->vertex_[0] = faces[i].p[k-3];
				faces[i].e[k]->vertex_[1] = faces[i].p[(k-5)%3+3];
				faces[i].e[k]->face_[0] = NULL;
				faces[i].e[k]->face_[1] = NULL;
				edges_.push_back(faces[i].e[k]);
			}
			// build the four triangles replacing the old one
			buildFourTriangles(faces[i],
												 triangle[0],triangle[1],
												 triangle[2],*t);
			new_triangles.push_back(triangle[0]);
			new_triangles.push_back(triangle[1]);
			new_triangles.push_back(triangle[2]);
			i++;
		}
		triangles_.splice(triangles_.end(),new_triangles);
		number_of_points_ += number_of_edges_;
		number_of_edges_ *= 4;
		number_of_triangles_ *= 4;
	}


	void TriangulatedSphere::buildFourTriangles
			(Face face,
			 Triangle* face0,
			 Triangle* face1,
			 Triangle* face2,
			 Triangle* face3)
		throw()
	{
		Triangle* triangle[3];
		triangle[0] = face0;
		triangle[1] = face1;
		triangle[2] = face2;
		TriangleEdge* edge[3];
		edge[0] = NULL;
		edge[1] = NULL;
		edge[2] = NULL;
		for (Position k = 0; k < 3; k++)
		{
			// create a smaller triangle containing face.p[k]
			TriangleEdge* first = NULL;
			TriangleEdge* second = NULL;
			TrianglePoint* p1 = NULL;
			TrianglePoint* p2 = NULL;
			TrianglePoint* p3 = face.p[k];
			Position i = 0;
			while (first == NULL)
			{
				if (face.e[i]->vertex_[0] == p3)
				{
					first = face.e[i];
					p1 = face.e[i]->vertex_[1];
				}
				else
				{
					if (face.e[i]->vertex_[1] == p3)
					{
						first = face.e[i];
						p1 = face.e[i]->vertex_[0];
					}
				}
				i++;
			}
			while (second == NULL)
			{
				if (face.e[i]->vertex_[0] == p3)
				{
					second = face.e[i];
					p2 = face.e[i]->vertex_[1];
				}
				else
				{
					if (face.e[i]->vertex_[1] == p3)
					{
						second = face.e[i];
						p2 = face.e[i]->vertex_[0];
					}
				}
				i++;
			}
			i = 6;
			while (edge[k] == NULL)
			{
				if (((face.e[i]->vertex_[0] == p1) &&
						 (face.e[i]->vertex_[1] == p2)		) ||
						((face.e[i]->vertex_[0] == p2) &&
						 (face.e[i]->vertex_[1] == p1)		)		)
				{
					edge[k] = face.e[i];
				}
				i++;
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
			edge[k]->face_[1] = face3;
		}
		face3->vertex_[0] = face.p[3];
		face3->vertex_[1] = face.p[4];
		face3->vertex_[2] = face.p[5];
		face3->edge_[0] = edge[0];
		face3->edge_[1] = edge[1];
		face3->edge_[2] = edge[2];
	}


	void TriangulatedSphere::setIncidences()
	{
		std::list<TrianglePoint*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			(*p)->edges_.clear();
			(*p)->faces_.clear();
		}
		std::list<TriangleEdge*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
			(*e)->vertex_[0]->edges_.insert(*e);
			(*e)->vertex_[0]->faces_.insert((*e)->face_[0]);
			(*e)->vertex_[0]->faces_.insert((*e)->face_[1]);
			(*e)->vertex_[1]->edges_.insert(*e);
			(*e)->vertex_[1]->faces_.insert((*e)->face_[0]);
			(*e)->vertex_[1]->faces_.insert((*e)->face_[1]);
		}
	}


	void TriangulatedSphere::icosaeder(bool out)
	{
		clear();
		TrianglePoint* point0  = new TrianglePoint;
		TrianglePoint* point1  = new TrianglePoint;
		TrianglePoint* point2  = new TrianglePoint;
		TrianglePoint* point3  = new TrianglePoint;
		TrianglePoint* point4  = new TrianglePoint;
		TrianglePoint* point5  = new TrianglePoint;
		TrianglePoint* point6  = new TrianglePoint;
		TrianglePoint* point7  = new TrianglePoint;
		TrianglePoint* point8  = new TrianglePoint;
		TrianglePoint* point9  = new TrianglePoint;
		TrianglePoint* point10 = new TrianglePoint;
		TrianglePoint* point11 = new TrianglePoint;
		TriangleEdge* edge0  = new  TriangleEdge;
		TriangleEdge* edge1  = new  TriangleEdge;
		TriangleEdge* edge2  = new  TriangleEdge;
		TriangleEdge* edge3  = new  TriangleEdge;
		TriangleEdge* edge4  = new  TriangleEdge;
		TriangleEdge* edge5  = new  TriangleEdge;
		TriangleEdge* edge6  = new  TriangleEdge;
		TriangleEdge* edge7  = new  TriangleEdge;
		TriangleEdge* edge8  = new  TriangleEdge;
		TriangleEdge* edge9  = new  TriangleEdge;
		TriangleEdge* edge10 = new  TriangleEdge;
		TriangleEdge* edge11 = new  TriangleEdge;
		TriangleEdge* edge12 = new  TriangleEdge;
		TriangleEdge* edge13 = new  TriangleEdge;
		TriangleEdge* edge14 = new  TriangleEdge;
		TriangleEdge* edge15 = new  TriangleEdge;
		TriangleEdge* edge16 = new  TriangleEdge;
		TriangleEdge* edge17 = new  TriangleEdge;
		TriangleEdge* edge18 = new  TriangleEdge;
		TriangleEdge* edge19 = new  TriangleEdge;
		TriangleEdge* edge20 = new  TriangleEdge;
		TriangleEdge* edge21 = new  TriangleEdge;
		TriangleEdge* edge22 = new  TriangleEdge;
		TriangleEdge* edge23 = new  TriangleEdge;
		TriangleEdge* edge24 = new  TriangleEdge;
		TriangleEdge* edge25 = new  TriangleEdge;
		TriangleEdge* edge26 = new  TriangleEdge;
		TriangleEdge* edge27 = new  TriangleEdge;
		TriangleEdge* edge28 = new  TriangleEdge;
		TriangleEdge* edge29 = new  TriangleEdge;
		Triangle* t0  = new Triangle;
		Triangle* t1  = new Triangle;
		Triangle* t2  = new Triangle;
		Triangle* t3  = new Triangle;
		Triangle* t4  = new Triangle;
		Triangle* t5  = new Triangle;
		Triangle* t6  = new Triangle;
		Triangle* t7  = new Triangle;
		Triangle* t8  = new Triangle;
		Triangle* t9  = new Triangle;
		Triangle* t10 = new Triangle;
		Triangle* t11 = new Triangle;
		Triangle* t12 = new Triangle;
		Triangle* t13 = new Triangle;
		Triangle* t14 = new Triangle;
		Triangle* t15 = new Triangle;
		Triangle* t16 = new Triangle;
		Triangle* t17 = new Triangle;
		Triangle* t18 = new Triangle;
		Triangle* t19 = new Triangle;
		number_of_points_ =12;
		number_of_edges_ = 30;
		number_of_triangles_ = 20;

		point0->point_  = TVector3<double>( 0.0     , 0.0     , 1.0      );
		point1->point_  = TVector3<double>( 0.894427, 0.0     , 0.4472135);
		point2->point_  = TVector3<double>( 0.276393, 0.850651, 0.4472135);
		point3->point_  = TVector3<double>(-0.723607, 0.525731, 0.4472135);
		point4->point_  = TVector3<double>(-0.723607,-0.525731, 0.4472135);
		point5->point_  = TVector3<double>( 0.276393,-0.850651, 0.4472135);
		point6->point_  = TVector3<double>( 0.723607, 0.525731,-0.4472135);
		point7->point_  = TVector3<double>(-0.276393, 0.850651,-0.4472135);
		point8->point_  = TVector3<double>(-0.894427, 0.0     ,-0.4472135);
		point9->point_  = TVector3<double>(-0.276393,-0.850651,-0.4472135);
		point10->point_ = TVector3<double>( 0.723607,-0.525731,-0.4472135);
		point11->point_ = TVector3<double>( 0.0     , 0.0     ,-1.0      );
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
		point0->faces_.insert(t0);   point0->faces_.insert(t1);   point0->faces_.insert(t2);
		point0->faces_.insert(t3);   point0->faces_.insert(t4);
		point1->faces_.insert(t0);   point1->faces_.insert(t4);   point1->faces_.insert(t5);
		point1->faces_.insert(t13);  point1->faces_.insert(t14);
		point2->faces_.insert(t0);   point2->faces_.insert(t1);   point2->faces_.insert(t6);
		point2->faces_.insert(t7);   point2->faces_.insert(t5);
		point3->faces_.insert(t1);   point3->faces_.insert(t2);   point3->faces_.insert(t7);
		point3->faces_.insert(t8);   point3->faces_.insert(t9);
		point4->faces_.insert(t2);   point4->faces_.insert(t3);   point4->faces_.insert(t9);
		point4->faces_.insert(t10);  point4->faces_.insert(t11);
		point5->faces_.insert(t4);   point5->faces_.insert(t11);  point5->faces_.insert(t12);
		point5->faces_.insert(t3);   point5->faces_.insert(t14);
		point6->faces_.insert(t5);   point6->faces_.insert(t6);   point6->faces_.insert(t13);
		point6->faces_.insert(t15);  point6->faces_.insert(t19);
		point7->faces_.insert(t6);   point7->faces_.insert(t7);   point7->faces_.insert(t8);
		point7->faces_.insert(t15);  point7->faces_.insert(t16);
		point8->faces_.insert(t8);   point8->faces_.insert(t9);   point8->faces_.insert(t10);
		point8->faces_.insert(t16);  point8->faces_.insert(t17);
		point9->faces_.insert(t10);  point9->faces_.insert(t11);  point9->faces_.insert(t12);
		point9->faces_.insert(t17);  point9->faces_.insert(t18);
		point10->faces_.insert(t12); point10->faces_.insert(t13); point10->faces_.insert(t14);
		point10->faces_.insert(t18); point10->faces_.insert(t19);
		point11->faces_.insert(t15); point11->faces_.insert(t16); point11->faces_.insert(t17);
		point11->faces_.insert(t18); point11->faces_.insert(t19);
		point0->edges_.insert(edge0);   point0->edges_.insert(edge1);   point0->edges_.insert(edge3);
		point0->edges_.insert(edge5);   point0->edges_.insert(edge7);
		point1->edges_.insert(edge1);   point1->edges_.insert(edge2);   point1->edges_.insert(edge9);
		point1->edges_.insert(edge10);  point1->edges_.insert(edge23);
		point2->edges_.insert(edge0);   point2->edges_.insert(edge2);   point2->edges_.insert(edge4);
		point2->edges_.insert(edge11);  point2->edges_.insert(edge12);
		point3->edges_.insert(edge3);   point3->edges_.insert(edge4);   point3->edges_.insert(edge6);
		point3->edges_.insert(edge14);  point3->edges_.insert(edge15);
		point4->edges_.insert(edge5);   point4->edges_.insert(edge6);   point4->edges_.insert(edge8);
		point4->edges_.insert(edge17);  point4->edges_.insert(edge18);
		point5->edges_.insert(edge7);   point5->edges_.insert(edge8);   point5->edges_.insert(edge9);
		point5->edges_.insert(edge20);  point5->edges_.insert(edge21);
		point6->edges_.insert(edge10);  point6->edges_.insert(edge11);  point6->edges_.insert(edge13);
		point6->edges_.insert(edge24);  point6->edges_.insert(edge25);
		point7->edges_.insert(edge12);  point7->edges_.insert(edge13);  point7->edges_.insert(edge14);
		point7->edges_.insert(edge16);  point7->edges_.insert(edge26);
		point8->edges_.insert(edge15);  point8->edges_.insert(edge16);  point8->edges_.insert(edge17);
		point8->edges_.insert(edge19);  point8->edges_.insert(edge27);
		point9->edges_.insert(edge18);  point9->edges_.insert(edge19);  point9->edges_.insert(edge20);
		point9->edges_.insert(edge22);  point9->edges_.insert(edge28);
		point10->edges_.insert(edge21); point10->edges_.insert(edge22); point10->edges_.insert(edge23);
		point10->edges_.insert(edge24); point10->edges_.insert(edge29);
		point11->edges_.insert(edge25); point11->edges_.insert(edge26); point11->edges_.insert(edge27);
		point11->edges_.insert(edge28); point11->edges_.insert(edge29);
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


	std::ostream& operator << (std::ostream& s,
														 const TriangulatedSurface& surface)
	{
		s << "Points: " << surface.numberOfPoints() << "\n";
		TriangulatedSurface::ConstPointIterator p;
		for (p = surface.beginPoint(); p != surface.endPoint(); p++)
		{
			s << **p << "\n";
		}
		s << "Edges: " << surface.numberOfEdges() << "\n";
		TriangulatedSurface::ConstEdgeIterator e;
		for (e = surface.beginEdge(); e != surface.endEdge(); e++)
		{
			s << **e << "\n";
		}
		s << "Triangles: " << surface.numberOfTriangles() << "\n";
		TriangulatedSurface::ConstTriangleIterator t;
		for (t = surface.beginTriangle(); t != surface.endTriangle(); t++)
		{
			s << **t << "\n";
		}
		return s;
	}


}	// namespace BALL
