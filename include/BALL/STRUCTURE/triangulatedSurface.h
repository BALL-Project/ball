// $Id: triangulatedSurface.h,v 1.22 2001/09/19 22:21:55 amoll Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#define BALL_STRUCTURE_TRIANGULATEDSURFACE_H

//#define with_indices
//#define debug_triangulation
//#define debug_triangulation_with_planes
#ifdef debug_triangulation
#	define with_bonds
#endif
#ifdef with_indices
#	define print_debug_info
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#include <list>
#include <vector>
#include <fstream>

namespace BALL
{

	template <class T>
	class TTriangle;

	template <class T>
	class TTrianglePoint;

	template <class T>
	class TTriangleEdge
	{
		public:
		TTriangleEdge()
			throw();
		TTriangleEdge(const TTriangleEdge<T>& edge, bool deep = false)
			throw();
		virtual ~TTriangleEdge()
			throw();
		void del(TTriangle<T>* t)
			throw();
		bool operator == (const TTriangleEdge<T>& e)
			throw();
		//protected:
		vector<TTrianglePoint<T>*> point;
		vector<TTriangle<T>*> triangle;
		Index index;
	};

	template <class T>
	TTriangleEdge<T>::TTriangleEdge()
		throw()
		:	point(2),
			triangle(2),
			index(-1)
	{
	}
	template <class T>
	TTriangleEdge<T>::TTriangleEdge(const TTriangleEdge<T>& edge, bool deep)
		throw()
		:	point(edge.point.size()),
			triangle(2),
			index(edge.index)
	{
		if (deep)
		{
			if (edge.point.size() > 0)
			{
				point[0] = new TTrianglePoint<T>(*(edge.point[0]),false);
				if (edge.point.size() > 1)
				{
					point[1] = new TTrianglePoint<T>(*(edge.point[1]),false);
				}
			}
			triangle[0] = new TTriangle<T>(*(edge.triangle[0]),false);
			triangle[1] = new TTriangle<T>(*(edge.triangle[1]),false);
		}
		else
		{
			if (edge.point.size() > 0)
			{
				point[0] = NULL;
				if (edge.point.size() > 1)
				{
					point[1] = NULL;
				}
			}
			triangle[0] = NULL;
			triangle[1] = NULL;
		}
	}
	template <class T>
	TTriangleEdge<T>::~TTriangleEdge()
		throw()
	{
	}
	template <class T>
	void TTriangleEdge<T>::del(TTriangle<T>* t)
		throw()
	{
		typename std::vector<TTriangle<T>*>::iterator i = triangle.begin();
		while (i != triangle.end())
		{
			if (*i == t)
			{
				triangle.erase(i);
				i = triangle.end();
			}
			else
			{
				i++;
			}
		}
	}
	template <class T>
	bool TTriangleEdge<T>::operator == (const TTriangleEdge<T>& e)
		throw()
	{
		return ( ((point[0]->p == e.point[0]->p) &&
							(point[1]->p == e.point[1]->p))		||
						 ((point[0]->p == e.point[1]->p) &&
							(point[1]->p == e.point[0]->p))				);
	}
	template <class T>
	std::ostream& operator << (std::ostream& s, const TTriangleEdge<T>& edge)
	{
		s << "EDGE" << edge.index << "(";
		if (edge.point.size() == 0)
		{
			s << "--";
		}
		else
		{
			s << (edge.point[0] == NULL ? -2 : edge.point[0]->index);
			for (Position i = 1; i < edge.point.size(); i++)
			{
				s << "-" << (edge.point[i] == NULL ? -2 : edge.point[i]->index);
			}
		}
		s << " ";
		if (edge.triangle.size() == 0)
		{
			s << "--";
		}
		else
		{
			s << (edge.triangle[0] == NULL ? -2 : edge.triangle[0]->index);
			for (Position i = 1; i < edge.triangle.size(); i++)
			{
				s << "|" << (edge.triangle[i] == NULL ? -2 : edge.triangle[i]->index);
			}
		}
		s << ")";
		return s;
	}



	template <class T>
	class TTriangle
	{
		public:
		TTriangle()
			throw();
		TTriangle(const TTriangle<T>& triangle, bool deep = false)
			throw();
		virtual ~TTriangle()
			throw();
		void remove(TTriangleEdge<T>* e)
			throw();
		TTriangleEdge<T>* getEdge(TTrianglePoint<T>* p)
			throw();
		TTrianglePoint<T>* third(TTrianglePoint<T>* p1, TTrianglePoint<T>* p2)
			throw();
		Index relativePointIndex(TTrianglePoint<T>* p)
			throw();
		//protected:
		vector<TTrianglePoint<T>*> point;
		vector<TTriangleEdge<T>*> edge;
		Index index;
	};

	template <class T>
	TTriangle<T>::TTriangle()
		throw()
		:	point(3),
			edge(3),
			index(-1)
	{
	}
	template <class T>
	TTriangle<T>::TTriangle(const TTriangle<T>& triangle, bool deep)
		throw()
		:	point(3),
			edge(3),
			index(triangle.index)
	{
		if (deep)
		{
			point[0] = new TTrianglePoint<T>(*(triangle.point[0]),false);
			point[1] = new TTrianglePoint<T>(*(triangle.point[1]),false);
			point[2] = new TTrianglePoint<T>(*(triangle.point[2]),false);
			edge[0] = new TTriangleEdge<T>(*(triangle.edge[0]),false);
			edge[1] = new TTriangleEdge<T>(*(triangle.edge[1]),false);
			edge[2] = new TTriangleEdge<T>(*(triangle.edge[2]),false);
		}
		else
		{
			point[0] = NULL;
			point[1] = NULL;
			point[2] = NULL;
			edge[0] = NULL;
			edge[1] = NULL;
			edge[2] = NULL;
		}
	}
	template <class T>
	TTriangle<T>::~TTriangle()
		throw()
	{
	}
	template <class T>
	void TTriangle<T>::remove(TTriangleEdge<T>* e)
		throw()
	{
		for (Position i = 0; i < 3; i++)
		{
			if (edge[i] == e)
			{
				edge[i] = NULL;
			}
		}
	}
	template <class T>
	TTriangleEdge<T>* TTriangle<T>::getEdge(TTrianglePoint<T>* p)
		throw()
	{
		for (Position i = 0; i < edge.size(); i++)
		{
			if ((edge[i]->point[0] != p) && (edge[i]->point[1] != p))
			{
				return edge[i];
			}
		}
		return NULL;
	}
	template <class T>
	TTrianglePoint<T>* TTriangle<T>::third(TTrianglePoint<T>* p1, TTrianglePoint<T>* p2)
		throw()
	{
		if (((*p1 == *point[0]) && (*p2 == *point[1])) ||
				((*p1 == *point[1]) && (*p2 == *point[0]))   )
		{
				return point[2];
		}
		if (((*p1 == *point[0]) && (*p2 == *point[2])) ||
				((*p1 == *point[2]) && (*p2 == *point[0]))   )
		{
				return point[1];
		}
		return point[0];
	}
	template <class T>
	Index TTriangle<T>::relativePointIndex(TTrianglePoint<T>* p)
		throw()
	{
		for (Position i = 0; i < point.size(); i++)
		{
			if (point[i] == p)
			{
				return i;
			}
		}
		return -1;
	}
	template <class T>
	std::ostream& operator << (std::ostream& s, const TTriangle<T>& triangle)
	{
		s << "TRIANGLE" << triangle.index << "( ["
			<< triangle.point[0]->index << " "
			<< triangle.point[1]->index << " "
			<< triangle.point[2]->index << "] {"
			<< (triangle.edge[0] == NULL ? -2 : triangle.edge[0]->index) << " "
			<< (triangle.edge[1] == NULL ? -2 : triangle.edge[1]->index) << " "
			<< (triangle.edge[2] == NULL ? -2 : triangle.edge[2]->index) << "} )";
		return s;
	}



	template <class T>
	class TTrianglePoint
	{
		public:
		TTrianglePoint()
			throw();
		TTrianglePoint(const TTrianglePoint<T>& point, bool deep = false)
			throw();
		virtual ~TTrianglePoint()
			throw();
		TTriangleEdge<T>* has(TTriangleEdge<T>* test)
			throw();
		bool operator == (const TTrianglePoint& point)
			throw();
		//protected:
		TVector3<T> p;
		TVector3<T> n;
		std::list<TTriangleEdge<T>*> edge;
		std::list<TTriangle<T>*> triangle;
		Index state;
		Index index;
	};

	template <class T>
	TTrianglePoint<T>::TTrianglePoint()
		throw()
		:	p(),
			n(),
			edge(),
			triangle(),
			index(-1)
	{
	}
	template <class T>
	TTrianglePoint<T>::TTrianglePoint(const TTrianglePoint<T>& point, bool deep)
		throw()
		:	p(point.p),
			n(point.n),
			edge(),
			triangle(),
			index(point.index)
	{
		if (deep)
		{
			typename std::list<TTriangleEdge<T>*>::const_iterator e;
			TTriangleEdge<T>* new_edge;
			for (e = point.edge.begin(); e != point.edge.end(); e++)
			{
				new_edge = new TTriangleEdge<T>(**e,false);
				edge.push_back(new_edge);
			}
			typename std::list<TTriangle<T>*>::const_iterator t;
			TTriangle<T>* new_triangle;
			for (t = point.triangle.begin(); t != point.triangle.end(); t++)
			{
				new_triangle = new TTriangle<T>(**t,false);
				triangle.push_back(new_triangle);
			}
		}
	}
	template <class T>
	TTrianglePoint<T>::~TTrianglePoint()
		throw()
	{
	}
	template <class T>
	TTriangleEdge<T>* TTrianglePoint<T>::has(TTriangleEdge<T>* test)
		throw()
	{
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edge.begin(); e != edge.end(); e++)
		{
			if (*(*e) == *test)
			{
				return *e;
			}
		}
		return NULL;
	}
	template <class T>
	bool TTrianglePoint<T>::operator == (const TTrianglePoint<T>& point)
		throw()
	{
		return (p == point.p);
	}
	template <class T>
	std::ostream& operator << (std::ostream& s, const TTrianglePoint<T>& point)
	{
		typename std::list<TTriangleEdge<T>*>::const_iterator e;
		typename std::list<TTriangle<T>*>::const_iterator t;
		s << "POINT";
		s << point.index;
		s << "( " << point.p << " " << point.n << " {";
		for (e = point.edge.begin(); e != point.edge.end(); e++)
		{
		s << (*e)->index << " ";
		}
		s << "} [";
		for (t = point.triangle.begin(); t != point.triangle.end(); t++)
		{
			s << (*t)->index << " ";
		}
		s << "] )";
		return s;
	}



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

		/**	@name	Predicates
		*/
		//@{
		//@}

		/** @name Accessors
		*/
		//@{

		void pushPoint(TTrianglePoint<T>* p)
			throw();

		void getPoints(std::list<TTrianglePoint<T>*> p)
			throw();

		void pushEdge(TTriangleEdge<T>* e)
			throw();

		void getEdges(std::list<TTriangleEdge<T>*> e)
			throw();

		void pushTriangle(TTriangle<T>* t)
			throw();

		void getTriangles(std::list<TTriangle<T>*> t)
			throw();

		void remove(TTrianglePoint<T>* point, bool deep = true)
			throw();

		void remove(typename std::list<TTrianglePoint<T>*>::iterator point, bool deep = true)
			throw();

		void remove(TTriangleEdge<T>* e, bool deep = false)
			throw();

		void remove(typename std::list<TTriangleEdge<T>*>::iterator e, bool deep = false)
			throw();

		void remove(TTriangle<T>* t, bool deep = false)
			throw();

		void remove(typename std::list<TTriangle<T>*>::iterator t, bool deep = false)
			throw();

		void exportSurface(TSurface<T>& surface)
			throw();

		TTriangulatedSurface<T>& operator+=(const TTriangulatedSurface<T>& surface)
			throw();

		void join(TTriangulatedSurface<T>& source);

		void icosaeder(const bool out);

		void shift(const TVector3<T>& c);

		void blowUp(const T& r);

		void refineSphere(Position iterations, bool out = true);

		void refineSphere(bool out);

		void setIncidences();

		void setIndices();

		void cut(const std::vector< TPlane3<T> >& plane);

		void cut(const std::vector< TPlane3<T> >& plane,
						 list<TTriangleEdge<T>*>& border);

		void cut(const TPlane3<T>& plane, const T& fuzzy = 0);

		void getBorder(std::list<TTriangleEdge<T>*>& border);

		void copy(const TTriangulatedSurface<T>& surface)
			throw(Exception::GeneralException);

		//@}

		/**	@name	Attributes
		*/
		//@{

		/*_ the points of the surface.
		*/
		std::list<TTrianglePoint<T>*> points;

		/*_ the edges of the surface.
		*/
		std::list<TTriangleEdge<T>*> edges;

		/*_ the triangles of the surface.
		*/
		std::list<TTriangle<T>*> triangles;
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
		typename std::list<TTrianglePoint<T>*>::const_iterator p;
		typename std::list<TTriangleEdge<T>*>::const_iterator e;
		typename std::list<TTriangle<T>*>::const_iterator t;
		s << "Points: " << surface.points.size() << " = " << surface.number_of_points_ << "\n";
		for (p = surface.points.begin(); p != surface.points.end(); p++)
		{
			s << **p << "\n";
		}
		s << "Edges: " << surface.edges.size() << " = " << surface.number_of_edges_ << "\n";
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
		{
			s << **e << "\n";
		}
		s << "Triangles: " << surface.triangles.size() << " = " << surface.number_of_triangles_ << "\n";
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
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
		:	points(),
			edges(),
			triangles()
	{
	}


	template <class T>
	TTriangulatedSurface<T>::TTriangulatedSurface(const TTriangulatedSurface<T>& surface, bool)
		throw(Exception::GeneralException)
		:	points(),
			edges(),
			triangles(),
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
			for (p = points.begin(); p != points.end(); p++)
			{
				delete *p;
			}
			for (e = edges.begin(); e != edges.end(); e++)
			{
				delete *e;
			}
			for (t = triangles.begin(); t != triangles.end(); t++)
			{
				delete *t;
			}
			points.clear();
			edges.clear();
			triangles.clear();
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
		for (p = points.begin(); p != points.end(); p++)
		{
			delete *p;
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			delete *e;
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
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
			for (p = points.begin(); p != points.end(); p++)
			{
				delete *p;
			}
			for (e = edges.begin(); e != edges.end(); e++)
			{
				delete *e;
			}
			for (t = triangles.begin(); t != triangles.end(); t++)
			{
				delete *t;
			}
			points.clear();
			edges.clear();
			triangles.clear();
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
		points.push_back(p);
		number_of_points_++;
	}


	template <class T>
	void TTriangulatedSurface<T>::getPoints(std::list<TTrianglePoint<T>*> p)
		throw()
	{
		return points;
	}


	template <class T>
	void TTriangulatedSurface<T>::pushEdge(TTriangleEdge<T>* e)
		throw()
	{
		edges.push_back(e);
		number_of_edges_++;
	}


	template <class T>
	void TTriangulatedSurface<T>::getEdges(std::list<TTriangleEdge<T>*> e)
		throw()
	{
		return edges;
	}


	template <class T>
	void TTriangulatedSurface<T>::pushTriangle(TTriangle<T>* t)
		throw()
	{
		triangles.push_back(t);
		number_of_triangles_;
	}


	template <class T>
	void TTriangulatedSurface<T>::getTriangles(std::list<TTriangle<T>*> t)
		throw()
	{
		return triangles;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(TTrianglePoint<T>* point, bool deep)
		throw()
	{
		if (deep)
		{
			typename std::list<TTriangleEdge<T>*>::iterator e;
			for (e = point->edges.begin(); e != point->edges.end(); e++)
			{
				if ((*e)->point[0] == point)
				{
					(*e)->point[1]->edges.remove(*e);
				}
				else
				{
					(*e)->point[0]->edges.remove(*e);
				}
				edges.remove(*e);
				delete *e;
				number_of_edges_--;
			}
			typename std::list<TTriangle<T>*> delete_triangles = point->triangles;
			typename std::list<TTriangle<T>*>::iterator t;
			for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
			{
				(*t)->point[0]->triangles.remove(*t);
				(*t)->point[1]->triangles.remove(*t);
				(*t)->point[2]->triangles.remove(*t);
				(*t)->getEdge(point)->del(*t);
				triangles.remove(*t);
				delete *t;
				number_of_triangles_--;
			}
		}
		delete point;
		points.remove(point);
		number_of_points_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(typename std::list<TTrianglePoint<T>*>::iterator point, bool deep)
		throw()
	{
		if (deep)
		{
			typename std::list<TTriangleEdge<T>*>::iterator e;
			for (e = (*point)->edge.begin(); e != (*point)->edge.end(); e++)
			{
				if ((*e)->point[0] == *point)
				{
					(*e)->point[1]->edge.remove(*e);
				}
				else
				{
					(*e)->point[0]->edge.remove(*e);
				}
				Position triangle_number = (*e)->triangle.size();
				for (Position i = 0; i < triangle_number; i++)
				{
					(*e)->triangle[i]->remove(*e);
				}
				edges.remove(*e);
				delete *e;
				number_of_edges_--;
			}
			std::list<TTriangle<T>*> delete_triangles = (*point)->triangle;
			typename std::list<TTriangle<T>*>::iterator t;
			TTriangleEdge<T>* border_edge;
			for (t = delete_triangles.begin(); t != delete_triangles.end(); t++)
			{
				(*t)->point[0]->triangle.remove(*t);
				(*t)->point[1]->triangle.remove(*t);
				(*t)->point[2]->triangle.remove(*t);
				for (Position i = 0; i < 3; i++)
				{
					if ((*t)->edge[i] != NULL)
					{
						border_edge = (*t)->edge[i];
					}
				}
				border_edge->del(*t);
				triangles.remove(*t);
				delete *t;
				number_of_triangles_--;
			}
		}
		delete *point;
		points.erase(point);
		number_of_points_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(TTriangleEdge<T>* e, bool deep)
		throw()
	{
		if (deep)
		{
			e->point[0]->edges.remove(e);
			e->point[1]->edges.remove(e);
			e->triangle[0]->del(e);
			e->triangle[1]->del(e);
		}
		delete e;
		edges.remove(e);
		number_of_edges_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(typename std::list<TTriangleEdge<T>*>::iterator e, bool deep)
		throw()
	{
		if (deep)
		{
			(*e)->point[0]->edges.remove(*e);
			(*e)->point[1]->edges.remove(*e);
			(*e)->triangle[0]->del(*e);
			(*e)->triangle[1]->del(*e);
		}
		delete *e;
		edges.erase(e);
		number_of_edges_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(TTriangle<T>* t, bool deep)
		throw()
	{
		if (deep)
		{
			t->point[0]->triangles.remove(t);
			t->point[1]->triangles.remove(t);
			t->point[2]->triangles.remove(t);
			t->edge[0]->del(t);
			t->edge[1]->del(t);
			t->edge[2]->del(t);
		}
		delete t;
		triangles.remove(t);
		number_of_triangles_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::remove(typename std::list<TTriangle<T>*>::iterator t, bool deep)
		throw()
	{
		if (deep)
		{
			(*t)->point[0]->triangles.remove(*t);
			(*t)->point[1]->triangles.remove(*t);
			(*t)->point[2]->triangles.remove(*t);
			(*t)->edge[0]->del(*t);
			(*t)->edge[1]->del(*t);
			(*t)->edge[2]->del(*t);
		}
		delete t;
		triangles.erase(t);
		number_of_triangles_--;
	}


	template <class T>
	void TTriangulatedSurface<T>::exportSurface(TSurface<T>& surface)
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		Index i = 0;
		for (p = points.begin(); p != points.end(); p++)
		{
			surface.vertex.push_back((*p)->p);
			surface.normal.push_back((*p)->n);
			(*p)->index = i;
			i++;
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			TSurface<T>::Triangle triangle;
			triangle.v1 = (*t)->point[0]->index;
			triangle.v2 = (*t)->point[1]->index;
			triangle.v3 = (*t)->point[2]->index;
			surface.triangle.push_back(triangle);
		}
	}


	template <class T>
	TTriangulatedSurface<T>& TTriangulatedSurface<T>::operator+=(const TTriangulatedSurface<T>& surface)
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = surface.points.begin(); p != surface.points.end(); p++)
		{
			points.push_back(*p);
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
		{
			edges.push_back(*e);
		}
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
		{
			triangles.push_back(*t);
		}
		number_of_points_ += surface.number_of_points_;
		number_of_edges_ += surface.number_of_edges_;
		number_of_triangles_ += surface.number_of_triangles_;
		return *this;
	}


	template <class T>
	void TTriangulatedSurface<T>::join(TTriangulatedSurface<T>& source)
	{
		points.splice(points.end(),source.points);
		edges.splice(edges.end(),source.edges);
		triangles.splice(triangles.end(),source.triangles);
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
		for (i = points.begin(); i != points.end(); i++)
		{
			(*i)->p += c;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::blowUp(const T& r)
	{
		typename std::list<TTrianglePoint<T>*>::iterator i;
		for (i = points.begin(); i != points.end(); i++)
		{
			(*i)->p *= r;
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
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			TVector3<T> norm( ((*t)->point[1]->p-(*t)->point[0]->p) %
												((*t)->point[2]->p-(*t)->point[0]->p)   );
			if (Maths::isGreater(norm*(*t)->point[0]->p,0) && (out == false))
			{
				TTrianglePoint<T>* temp = (*t)->point[1];
				(*t)->point[1] = (*t)->point[2];
				(*t)->point[2] = temp;
			}
			if (Maths::isLess(norm*(*t)->point[0]->p,0) && (out == true))
			{
				TTrianglePoint<T>* temp = (*t)->point[1];
				(*t)->point[1] = (*t)->point[2];
				(*t)->point[2] = temp;
			}
		}
		setIncidences();
	}


	template <class T>
	void TTriangulatedSurface<T>::refineSphere(bool out)
	{
		std::list<TTriangleEdge<T>*> new_edges;
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			TTrianglePoint<T>* point1 = (*e)->point[0];
			TTrianglePoint<T>* point2 = (*e)->point[1];
			TTrianglePoint<T>* new_point = new TTrianglePoint<T>;
			new_point->p = (point1->p+point2->p).normalize();
			if (out == true)
			{
				new_point->n = new_point->p;
			}
			else
			{
				new_point->n = -new_point->p;
			}
			(*e)->triangle[0]->point.push_back(new_point);
			(*e)->triangle[1]->point.push_back(new_point);
			points.push_back(new_point);
			TTriangleEdge<T>* new_edge1 = new TTriangleEdge<T>;
			new_edge1->point[0] = point1;
			new_edge1->point[1] = new_point;
			(*e)->triangle[0]->edge.push_back(new_edge1);
			(*e)->triangle[1]->edge.push_back(new_edge1);
			new_edges.push_back(new_edge1);
			TTriangleEdge<T>* new_edge2 = new TTriangleEdge<T>;
			new_edge2->point[0] = point2;
			new_edge2->point[1] = new_point;
			(*e)->triangle[0]->edge.push_back(new_edge2);
			(*e)->triangle[1]->edge.push_back(new_edge2);
			new_edges.push_back(new_edge2);
		}
		std::list<TTriangle<T>*> new_triangles;
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			TTriangle<T>* current = *t;
			vector<TTriangle<T>*> triangle(4);
			for (Position k = 0; k < 4; k++)	// create four new triangles
			{
				triangle[k] = new TTriangle<T>;
			}
			vector<TTriangleEdge<T>* > edge(3);
			for (Position k = 0; k < 3; k++)	// create three new edges
			{
				edge[k] = new TTriangleEdge<T>;
			}
			list<TTriangleEdge<T>*> edge_list;
			for (Position k = 3; k < 9; k++)	// list of edges created in the first for-loop
			{																	//  that belong to current
				edge_list.push_back(current->edge[k]);
			}
			for (Position k = 0; k < 3; k++)	// create a smaller triangle containing current->point[k]
			{
				TTriangleEdge<T>* first = NULL;
				TTriangleEdge<T>* second = NULL;
				TTrianglePoint<T>* p1 = NULL;
				TTrianglePoint<T>* p2 = NULL;
				TTrianglePoint<T>* p3 = current->point[k];
				typename list<TTriangleEdge<T>*>::iterator l = edge_list.begin();
				while (first == NULL)
				{
					if ((*l)->point[0]->p == p3->p)
					{
						first = *l;
						p1 = (*l)->point[1];
						edge_list.remove(*l);
					}
					else
					{
						if ((*l)->point[1]->p == p3->p)
						{
							first = *l;
							p1 = (*l)->point[0];
							edge_list.remove(*l);
						}
					}
					l++;
				}
				l = edge_list.begin();
				while (second == NULL)
				{
					if ((*l)->point[0]->p == p3->p)
					{
						second = *l;
						p2 = (*l)->point[1];
						edge_list.remove(*l);
					}
					else
					{
						if ((*l)->point[1]->p == p3->p)
						{
							second = *l;
							p2 = (*l)->point[0];
							edge_list.remove(*l);
						}
					}
					l++;
				}
				triangle[k]->point[0] = p1;
				triangle[k]->point[1] = p2;
				triangle[k]->point[2] = p3;
				triangle[k]->edge[0] = first;
				triangle[k]->edge[1] = second;
				triangle[k]->edge[2] = edge[k];
				if (first->triangle[0] == NULL)
				{
					first->triangle[0] = triangle[k];
				}
				else
				{
					first->triangle[1] = triangle[k];
				}
				if (second->triangle[0] == NULL)
				{
					second->triangle[0] = triangle[k];
				}
				else
				{
					second->triangle[1] = triangle[k];
				}
				edge[k]->triangle[0] = triangle[k];
				edge[k]->triangle[1] = triangle[4];
				edge[k]->point[0] = p1;
				edge[k]->point[1] = p2;
			}
			triangle[3]->point[0] = current->point[3];
			triangle[3]->point[1] = current->point[4];
			triangle[3]->point[2] = current->point[5];
			triangle[3]->edge[0] = edge[0];
			triangle[3]->edge[1] = edge[1];
			triangle[3]->edge[2] = edge[2];
			edge[0]->triangle[1] = triangle[3];
			edge[1]->triangle[1] = triangle[3];
			edge[2]->triangle[1] = triangle[3];
			new_edges.push_back(edge[0]);
			new_edges.push_back(edge[1]);
			new_edges.push_back(edge[2]);
			new_triangles.push_back(triangle[0]);
			new_triangles.push_back(triangle[1]);
			new_triangles.push_back(triangle[2]);
			new_triangles.push_back(triangle[3]);
			delete *t;
		}
		edges = new_edges;
		triangles = new_triangles;
		number_of_points_ += number_of_edges_;
		number_of_edges_ *= 4;
		number_of_triangles_ *= 4;
	}


	template <class T>
	void TTriangulatedSurface<T>::setIncidences()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = points.begin(); p != points.end(); p++)
		{
			(*p)->edge.clear();
			(*p)->triangle.clear();
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			(*e)->point[0]->edge.push_back(*e);
			(*e)->point[0]->triangle.push_back((*e)->triangle[0]);
			(*e)->point[0]->triangle.push_back((*e)->triangle[1]);
			(*e)->point[1]->edge.push_back(*e);
			(*e)->point[1]->triangle.push_back((*e)->triangle[0]);
			(*e)->point[1]->triangle.push_back((*e)->triangle[1]);
		}
		for (p = points.begin(); p != points.end(); p++)
		{
			(*p)->edge.sort();
			(*p)->edge.unique();
			(*p)->triangle.sort();
			(*p)->triangle.unique();
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::setIndices()
	{
		Index i = 0;
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = points.begin(); p != points.end(); p++)
		{
			(*p)->index = i;
			i++;
		}
		i = 0;
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			(*e)->index = i;
			i++;
		}
		i = 0;
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			(*t)->index = i;
			i++;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::cut(const std::vector< TPlane3<T> >& plane)
	{
		typename std::list<TTriangle<T>*>::iterator t	= triangles.begin();
		while (t != triangles.end())
		{
			delete *t;
			t++;
		}
		triangles.clear();
		typename std::list<TTriangleEdge<T>*>::iterator e	= edges.begin();
		while (e != edges.end())
		{
			delete *e;
			e++;
		}
		edges.clear();
		for (Position i = 0;  i < plane.size(); i++)
		{
			T test_value = plane[i].n*plane[i].p;
			typename std::list<TTrianglePoint<T>*>::iterator p = points.begin();
			while (p != points.end())
			{
				typename std::list<TTrianglePoint<T>*>::iterator next = p;
				next++;
				if (Maths::isLessOrEqual(plane[i].n*(*p)->p,test_value))
				{
					delete *p;
					points.erase(p);
				}
				else
				{
					(*p)->edge.clear();
					(*p)->triangle.clear();
				}
				p = next;
			}
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::cut
		(const std::vector< TPlane3<T> >& plane,
		 std::list<TTriangleEdge<T>*>& border)
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		typename std::list<TTrianglePoint<T>*>::iterator next;
		T test_value;
		for (Position i = 0; i < plane.size(); i++)
		{
			test_value = plane[i].n*plane[i].p;
			p = points.begin();
			while (p != points.end())
			{
				next = p;
				next++;
				if (Maths::isLessOrEqual(plane[i].n*(*p)->p,test_value))
				{
					remove(p);
				}
				p = next;
			}
		}
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			if ((*e)->triangle.size() < 2)
			{
				border.push_back(*e);
			}
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::cut(const TPlane3<T>& plane, const T& fuzzy)
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
		typename std::list<TTrianglePoint<T>*>::iterator next_point;
		T test_value;
		test_value = plane.n*plane.p+fuzzy;
		p = points.begin();
		while (p != points.end())
		{
			if (Maths::isLessOrEqual(plane.n*(*p)->p,test_value))
			{
				next_point = p;
				next_point++;
				if (next_point == points.end())
				{
					remove(p);
					p = points.end();
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
		typename std::list<TTriangleEdge<T>*>::iterator e = edges.begin();
		typename std::list<TTriangleEdge<T>*>::iterator next_edge;
		while (e != edges.end())
		{
			if ((*e)->triangle.size() == 0)
			{
				next_edge = e;
				next_edge++;
				(*e)->point[0]->edge.remove(*e);
				(*e)->point[1]->edge.remove(*e);
				delete *e;
				if (next_edge == edges.end())
				{
					edges.erase(e);
					e = edges.end();
				}
				else
				{
					edges.erase(e);
					e = next_edge;
				}
				number_of_edges_--;
			}
			else
			{
				e++;
			}
		}
		p = points.begin();
		while (p != points.end())
		{
			if ((*p)->edge.size() == 0)
			{
				next_point = p;
				next_point++;
				if (next_point == points.end())
				{
					remove(p);
					p = points.end();
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
		for (e = edges.begin(); e != edges.end(); e++)
		{
			if ((*e)->triangle.size() < 2)
			{
				border.push_back(*e);
			}
		}
	}
		/*setIndices();
		std::vector<Position> cutting_plane(edges.size());
		std::list<TTriangleEdge<T>*> empty;
		for (Position i = 0;  i < plane.size(); i++)
		{
			border.push_back(empty);
			std::list< TTrianglePoint<T>* >::iterator p;
			for (p = points.begin(); p != points.end(); p++)
			{
				(*p)->state = 0;
			}
			T test_value = plane[i].n*plane[i].p;
			std::list< TTriangle<T>* >::iterator t;
			t = triangles.begin();
			while (t != triangles.end())
			{
				TTriangle<T>* triangle = *t;
				t++;
				bool del = false;
				for (Position j = 0; j < 3; j++)
				{
					switch (triangle->point[j]->state)
					{
						case 0 :	if (Maths::isLessOrEqual(plane[i].n*triangle->point[j]->p,test_value))
											{
												triangle->point[j]->state = 1;
												del = true;
											}
											else
											{
												triangle->point[j]->state = 2;
											}
											break;
						case 1 :	del = true;
											break;
						case 2 :	break;
					}
				}
				if (del)
				{
					triangles.remove(triangle);
					for (Position k = 0; k < 3; k++)
					{
						triangle->edge[k]->del(triangle);
						triangle->point[k]->triangle.remove(triangle);
						cutting_plane[triangle->edge[k]->index] = i;
					}
					delete triangle;
				}
			}
		}
		HashSet< TTriangleEdge<T>* > existing_edges;
		HashSet< TTrianglePoint<T>* > existing_points;
		std::list< TTriangle<T>* >::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			for (Position k = 0; k < 3; k++)
			{
				TTriangleEdge<T>* edge = (*t)->edge[k];
				existing_edges.insert(edge);
				edges.remove(edge);
				if (edge->triangle.size() < 2)
				{
					border[cutting_plane[edge->index]].push_back(edge);
				}
				TTrianglePoint<T>* point = (*t)->point[k];
				existing_points.insert(point);
				points.remove(point);
			}
		}
		while (edges.size() > 0)
		{
			TTriangleEdge<T>* edge = edges.front();
			edge->point[0]->edge.remove(edge);
			edge->point[1]->edge.remove(edge);
			delete edge;
			edges.pop_front();
		}
		while (points.size() > 0)
		{
			delete points.front();
			points.pop_front();
		}
		HashSet< TTriangleEdge<T>* >::Iterator e;
		for (e = existing_edges.begin(); e != existing_edges.end(); e++)
		{
			edges.push_back(*e);
		}
		HashSet< TTrianglePoint<T>* >::Iterator point;
		for (point = existing_points.begin(); point != existing_points.end(); point++)
		{
			points.push_back(*point);
		}
	}*/


	template <class T>
	void TTriangulatedSurface<T>::copy(const TTriangulatedSurface<T>& surface)
		throw(Exception::GeneralException)
	{
		std::vector<TTrianglePoint<T>*> point_vector(number_of_points_);
		Index i = 0;
		typename std::list<TTrianglePoint<T>*>::const_iterator p;
		for (p = surface.points.begin(); p != surface.points.end(); p++)
		{
			if (*p != NULL)
			{
				if ((*p)->index != i)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY TRIANGULATED SURFACE",
																						"ILLAGAL INDEX FOUND");
				}
				else
				{
					point_vector[i] = new TTrianglePoint<T>(**p,false);
					points.push_back(point_vector[i]);
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
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
		{
			if (*e != NULL)
			{
				if ((*e)->index != i)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY TRIANGULATED SURFACE",
																						"ILLEGAL INDEX FOUND");
				}
				else
				{
					edge_vector[i] = new TTriangleEdge<T>(**e,false);
					edge_vector[i]->triangle.clear();
					edges.push_back(edge_vector[i]);
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
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
		{
			if (*t != NULL)
			{
				if ((*t)->index != i)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY TRIANGULATED SURFACE",
																						"ILLEGAL INDEX FOUND");
				}
				else
				{
					triangle_vector[i] = new TTriangle<T>(**t,false);
					triangles.push_back(triangle_vector[i]);
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
		for (p = surface.points.begin(); p != surface.points.end(); p++)
		{
			for (e = (*p)->edge.begin(); e != (*p)->edge.end(); e++)
			{
				point_vector[i]->edge.push_back(edge_vector[(*e)->index]);
			}
			for (t = (*p)->triangle.begin(); t != (*p)->triangle.end(); t++)
			{
				point_vector[i]->triangle.push_back(triangle_vector[(*t)->index]);
			}
			i++;
		}
		i = 0;
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
		{
			edge_vector[i]->point[0] = point_vector[(*e)->point[0]->index];
			edge_vector[i]->point[1] = point_vector[(*e)->point[1]->index];
			if ((*e)->triangle.size() > 0)
			{
				edge_vector[i]->triangle.push_back(triangle_vector[(*e)->triangle[0]->index]);
				if ((*e)->triangle.size() > 1)
				{
					edge_vector[i]->triangle.push_back(triangle_vector[(*e)->triangle[1]->index]);
				}
			}
			i++;
		}
		i = 0;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
		{
			triangle_vector[i]->point[0] = point_vector[(*t)->point[0]->index];
			triangle_vector[i]->point[1] = point_vector[(*t)->point[1]->index];
			triangle_vector[i]->point[2] = point_vector[(*t)->point[2]->index];
			triangle_vector[i]->edge[0] = edge_vector[(*t)->edge[0]->index];
			triangle_vector[i]->edge[1] = edge_vector[(*t)->edge[1]->index];
			triangle_vector[i]->edge[2] = edge_vector[(*t)->edge[2]->index];
			i++;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::icosaeder(const bool out)
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

		point0->p  = TVector3<T>( 0.0     , 0.0     , 1.0      );
		point1->p  = TVector3<T>( 0.894427, 0.0     , 0.4472135);
		point2->p  = TVector3<T>( 0.276393, 0.850651, 0.4472135);
		point3->p  = TVector3<T>(-0.723607, 0.525731, 0.4472135);
		point4->p  = TVector3<T>(-0.723607,-0.525731, 0.4472135);
		point5->p  = TVector3<T>( 0.276393,-0.850651, 0.4472135);
		point6->p  = TVector3<T>( 0.723607, 0.525731,-0.4472135);
		point7->p  = TVector3<T>(-0.276393, 0.850651,-0.4472135);
		point8->p  = TVector3<T>(-0.894427, 0.0     ,-0.4472135);
		point9->p  = TVector3<T>(-0.276393,-0.850651,-0.4472135);
		point10->p = TVector3<T>( 0.723607,-0.525731,-0.4472135);
		point11->p = TVector3<T>( 0.0     , 0.0     ,-1.0      );
		if (out == true)
		{
			point0->n  = point0->p;
			point1->n  = point1->p;
			point2->n  = point2->p;
			point3->n  = point3->p;
			point4->n  = point4->p;
			point5->n  = point5->p;
			point6->n  = point6->p;
			point7->n  = point7->p;
			point8->n  = point8->p;
			point9->n  = point9->p;
			point10->n = point10->p;
			point11->n = point11->p;
		}
		else
		{
			point0->n  = -point0->p;
			point1->n  = -point1->p;
			point2->n  = -point2->p;
			point3->n  = -point3->p;
			point4->n  = -point4->p;
			point5->n  = -point5->p;
			point6->n  = -point6->p;
			point7->n  = -point7->p;
			point8->n  = -point8->p;
			point9->n  = -point9->p;
			point10->n = -point10->p;
			point11->n = -point11->p;
		}
		point0->triangle.push_back(t0);   point0->triangle.push_back(t1);   point0->triangle.push_back(t2);
		point0->triangle.push_back(t3);   point0->triangle.push_back(t4);
		point1->triangle.push_back(t0);   point1->triangle.push_back(t4);   point1->triangle.push_back(t5);
		point1->triangle.push_back(t13);  point1->triangle.push_back(t14);
		point2->triangle.push_back(t0);   point2->triangle.push_back(t1);   point2->triangle.push_back(t6);
		point2->triangle.push_back(t7);   point2->triangle.push_back(t5);
		point3->triangle.push_back(t1);   point3->triangle.push_back(t2);   point3->triangle.push_back(t7);
		point3->triangle.push_back(t8);   point3->triangle.push_back(t9);
		point4->triangle.push_back(t2);   point4->triangle.push_back(t3);   point4->triangle.push_back(t9);
		point4->triangle.push_back(t10);  point4->triangle.push_back(t11);
		point5->triangle.push_back(t4);   point5->triangle.push_back(t11);  point5->triangle.push_back(t12);
		point5->triangle.push_back(t3);   point5->triangle.push_back(t14);
		point6->triangle.push_back(t5);   point6->triangle.push_back(t6);   point6->triangle.push_back(t13);
		point6->triangle.push_back(t15);  point6->triangle.push_back(t19);
		point7->triangle.push_back(t6);   point7->triangle.push_back(t7);   point7->triangle.push_back(t8);
		point7->triangle.push_back(t15);  point7->triangle.push_back(t16);
		point8->triangle.push_back(t8);   point8->triangle.push_back(t9);   point8->triangle.push_back(t10);
		point8->triangle.push_back(t16);  point8->triangle.push_back(t17);
		point9->triangle.push_back(t10);  point9->triangle.push_back(t11);  point9->triangle.push_back(t12);
		point9->triangle.push_back(t17);  point9->triangle.push_back(t18);
		point10->triangle.push_back(t12); point10->triangle.push_back(t13); point10->triangle.push_back(t14);
		point10->triangle.push_back(t18); point10->triangle.push_back(t19);
		point11->triangle.push_back(t15); point11->triangle.push_back(t16); point11->triangle.push_back(t17);
		point11->triangle.push_back(t18); point11->triangle.push_back(t19);
		point0->edge.push_back(edge0);   point0->edge.push_back(edge1);   point0->edge.push_back(edge3);
		point0->edge.push_back(edge5);   point0->edge.push_back(edge7);
		point1->edge.push_back(edge1);   point1->edge.push_back(edge2);   point1->edge.push_back(edge9);
		point1->edge.push_back(edge10);  point1->edge.push_back(edge23);
		point2->edge.push_back(edge0);   point2->edge.push_back(edge2);   point2->edge.push_back(edge4);
		point2->edge.push_back(edge11);  point2->edge.push_back(edge12);
		point3->edge.push_back(edge3);   point3->edge.push_back(edge4);   point3->edge.push_back(edge6);
		point3->edge.push_back(edge14);  point3->edge.push_back(edge15);
		point4->edge.push_back(edge5);   point4->edge.push_back(edge6);   point4->edge.push_back(edge8);
		point4->edge.push_back(edge17);  point4->edge.push_back(edge18);
		point5->edge.push_back(edge7);   point5->edge.push_back(edge8);   point5->edge.push_back(edge9);
		point5->edge.push_back(edge20);  point5->edge.push_back(edge21);
		point6->edge.push_back(edge10);  point6->edge.push_back(edge11);  point6->edge.push_back(edge13);
		point6->edge.push_back(edge24);  point6->edge.push_back(edge25);
		point7->edge.push_back(edge12);  point7->edge.push_back(edge13);  point7->edge.push_back(edge14);
		point7->edge.push_back(edge16);  point7->edge.push_back(edge26);
		point8->edge.push_back(edge15);  point8->edge.push_back(edge16);  point8->edge.push_back(edge17);
		point8->edge.push_back(edge19);  point8->edge.push_back(edge27);
		point9->edge.push_back(edge18);  point9->edge.push_back(edge19);  point9->edge.push_back(edge20);
		point9->edge.push_back(edge22);  point9->edge.push_back(edge28);
		point10->edge.push_back(edge21); point10->edge.push_back(edge22); point10->edge.push_back(edge23);
		point10->edge.push_back(edge24); point10->edge.push_back(edge29);
		point11->edge.push_back(edge25); point11->edge.push_back(edge26); point11->edge.push_back(edge27);
		point11->edge.push_back(edge28); point11->edge.push_back(edge29);
		points.push_back(point0);
		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
		points.push_back(point4);
		points.push_back(point5);
		points.push_back(point6);
		points.push_back(point7);
		points.push_back(point8);
		points.push_back(point9);
		points.push_back(point10);
		points.push_back(point11);

		edge0->point[0]  = point2;  edge0->point[1]  = point0;
		edge1->point[0]  = point0;  edge1->point[1]  = point1;
		edge2->point[0]  = point1;  edge2->point[1]  = point2;
		edge3->point[0]  = point3;  edge3->point[1]  = point0;
		edge4->point[0]  = point2;  edge4->point[1]  = point3;
		edge5->point[0]  = point4;  edge5->point[1]  = point0;
		edge6->point[0]  = point3;  edge6->point[1]  = point4;
		edge7->point[0]  = point5;  edge7->point[1]  = point0;
		edge8->point[0]  = point4;  edge8->point[1]  = point5;
		edge9->point[0]  = point5;  edge9->point[1]  = point1;
		edge10->point[0] = point1;  edge10->point[1] = point6;
		edge11->point[0] = point6;  edge11->point[1] = point2;
		edge12->point[0] = point7;  edge12->point[1] = point2;
		edge13->point[0] = point6;  edge13->point[1] = point7;
		edge14->point[0] = point7;  edge14->point[1] = point3;
		edge15->point[0] = point8;  edge15->point[1] = point3;
		edge16->point[0] = point7;  edge16->point[1] = point8;
		edge17->point[0] = point8;  edge17->point[1] = point4;
		edge18->point[0] = point9;  edge18->point[1] = point4;
		edge19->point[0] = point8;  edge19->point[1] = point9;
		edge20->point[0] = point9;  edge20->point[1] = point5;
		edge21->point[0] = point10; edge21->point[1] = point5;
		edge22->point[0] = point9;  edge22->point[1] = point10;
		edge23->point[0] = point1;  edge23->point[1] = point10;
		edge24->point[0] = point10; edge24->point[1] = point6;
		edge25->point[0] = point6;  edge25->point[1] = point11;
		edge26->point[0] = point11; edge26->point[1] = point7;
		edge27->point[0] = point11; edge27->point[1] = point8;
		edge28->point[0] = point11; edge28->point[1] = point9;
		edge29->point[0] = point11; edge29->point[1] = point10;
		edge0->triangle[0]  = t0;  edge0->triangle[1]  = t1;
		edge1->triangle[0]  = t0;  edge1->triangle[1]  = t4;
		edge2->triangle[0]  = t0;  edge2->triangle[1]  = t5;
		edge3->triangle[0]  = t1;  edge3->triangle[1]  = t2;
		edge4->triangle[0]  = t1;  edge4->triangle[1]  = t7;
		edge5->triangle[0]  = t2;  edge5->triangle[1]  = t3;
		edge6->triangle[0]  = t2;  edge6->triangle[1]  = t9;
		edge7->triangle[0]  = t3;  edge7->triangle[1]  = t4;
		edge8->triangle[0]  = t3;  edge8->triangle[1]  = t11;
		edge9->triangle[0]  = t4;  edge9->triangle[1]  = t14;
		edge10->triangle[0] = t5;  edge10->triangle[1] = t13;
		edge11->triangle[0] = t5;  edge11->triangle[1] = t6;
		edge12->triangle[0] = t6;  edge12->triangle[1] = t7;
		edge13->triangle[0] = t6;  edge13->triangle[1] = t15;
		edge14->triangle[0] = t7;  edge14->triangle[1] = t8;
		edge15->triangle[0] = t8;  edge15->triangle[1] = t9;
		edge16->triangle[0] = t8;  edge16->triangle[1] = t16;
		edge17->triangle[0] = t9;  edge17->triangle[1] = t10;
		edge18->triangle[0] = t10; edge18->triangle[1] = t11;
		edge19->triangle[0] = t10; edge19->triangle[1] = t17;
		edge20->triangle[0] = t11; edge20->triangle[1] = t12;
		edge21->triangle[0] = t12; edge21->triangle[1] = t14;
		edge22->triangle[0] = t12; edge22->triangle[1] = t18;
		edge23->triangle[0] = t13; edge23->triangle[1] = t14;
		edge24->triangle[0] = t13; edge24->triangle[1] = t19;
		edge25->triangle[0] = t15; edge25->triangle[1] = t19;
		edge26->triangle[0] = t15; edge26->triangle[1] = t16;
		edge27->triangle[0] = t16; edge27->triangle[1] = t17;
		edge28->triangle[0] = t17; edge28->triangle[1] = t18;
		edge29->triangle[0] = t18; edge29->triangle[1] = t19;
		edges.push_back(edge0);
		edges.push_back(edge1);
		edges.push_back(edge2);
		edges.push_back(edge3);
		edges.push_back(edge4);
		edges.push_back(edge5);
		edges.push_back(edge6);
		edges.push_back(edge7);
		edges.push_back(edge8);
		edges.push_back(edge9);
		edges.push_back(edge10);
		edges.push_back(edge11);
		edges.push_back(edge12);
		edges.push_back(edge13);
		edges.push_back(edge14);
		edges.push_back(edge15);
		edges.push_back(edge16);
		edges.push_back(edge17);
		edges.push_back(edge18);
		edges.push_back(edge19);
		edges.push_back(edge20);
		edges.push_back(edge21);
		edges.push_back(edge22);
		edges.push_back(edge23);
		edges.push_back(edge24);
		edges.push_back(edge25);
		edges.push_back(edge26);
		edges.push_back(edge27);
		edges.push_back(edge28);
		edges.push_back(edge29);

		t0->point[0]  = point2;  t0->point[1]  = point0;  t0->point[2]  = point1;
		t1->point[0]  = point3;  t1->point[1]  = point0;  t1->point[2]  = point2;
		t2->point[0]  = point4;  t2->point[1]  = point0;  t2->point[2]  = point3;
		t3->point[0]  = point5;  t3->point[1]  = point0;  t3->point[2]  = point4;
		t4->point[0]  = point1;  t4->point[1]  = point0;  t4->point[2]  = point5;
		t5->point[0]  = point2;  t5->point[1]  = point1;  t5->point[2]  = point6;
		t6->point[0]  = point7;  t6->point[1]  = point2;  t6->point[2]  = point6;
		t7->point[0]  = point3;  t7->point[1]  = point2;  t7->point[2]  = point7;
		t8->point[0]  = point8;  t8->point[1]  = point3;  t8->point[2]  = point7;
		t9->point[0]  = point4;  t9->point[1]  = point3;  t9->point[2]  = point8;
		t10->point[0] = point9;  t10->point[1] = point4;  t10->point[2] = point8;
		t11->point[0] = point5;  t11->point[1] = point4;  t11->point[2] = point9;
		t12->point[0] = point10; t12->point[1] = point5;  t12->point[2] = point9;
		t13->point[0] = point6;  t13->point[1] = point1;  t13->point[2] = point10;
		t14->point[0] = point1;  t14->point[1] = point5;  t14->point[2] = point10;
		t15->point[0] = point6;  t15->point[1] = point11; t15->point[2] = point7;
		t16->point[0] = point7;  t16->point[1] = point11; t16->point[2] = point8;
		t17->point[0] = point8;  t17->point[1] = point11; t17->point[2] = point9;
		t18->point[0] = point9;  t18->point[1] = point11; t18->point[2] = point10;
		t19->point[0] = point10; t19->point[1] = point11; t19->point[2] = point6;
		t0->edge[0]  = edge0;  t0->edge[1]  = edge1;  t0->edge[2]  = edge2;
		t1->edge[0]  = edge0;  t1->edge[1]  = edge3;  t1->edge[2]  = edge4;
		t2->edge[0]  = edge3;  t2->edge[1]  = edge5;  t2->edge[2]  = edge6;
		t3->edge[0]  = edge5;  t3->edge[1]  = edge7;  t3->edge[2]  = edge8;
		t4->edge[0]  = edge1;  t4->edge[1]  = edge7;  t4->edge[2]  = edge9;
		t5->edge[0]  = edge2;  t5->edge[1]  = edge10; t5->edge[2]  = edge11;
		t6->edge[0]  = edge11; t6->edge[1]  = edge12; t6->edge[2]  = edge13;
		t7->edge[0]  = edge4;  t7->edge[1]  = edge12; t7->edge[2]  = edge14;
		t8->edge[0]  = edge14; t8->edge[1]  = edge15; t8->edge[2]  = edge16;
		t9->edge[0]  = edge6;  t9->edge[1]  = edge15; t9->edge[2]  = edge17;
		t10->edge[0] = edge17; t10->edge[1] = edge18; t10->edge[2] = edge19;
		t11->edge[0] = edge8;  t11->edge[1] = edge18; t11->edge[2] = edge20;
		t12->edge[0] = edge20; t12->edge[1] = edge21; t12->edge[2] = edge22;
		t13->edge[0] = edge10; t13->edge[1] = edge23; t13->edge[2] = edge24;
		t14->edge[0] = edge9;  t14->edge[1] = edge21; t14->edge[2] = edge23;
		t15->edge[0] = edge13; t15->edge[1] = edge25; t15->edge[2] = edge26;
		t16->edge[0] = edge16; t16->edge[1] = edge26; t16->edge[2] = edge27;
		t17->edge[0] = edge19; t17->edge[1] = edge27; t17->edge[2] = edge28;
		t18->edge[0] = edge22; t18->edge[1] = edge28; t18->edge[2] = edge29;
		t19->edge[0] = edge24; t19->edge[1] = edge25; t19->edge[2] = edge29;
		triangles.push_back(t0);
		triangles.push_back(t1);
		triangles.push_back(t2);
		triangles.push_back(t3);
		triangles.push_back(t4);
		triangles.push_back(t5);
		triangles.push_back(t6);
		triangles.push_back(t7);
		triangles.push_back(t8);
		triangles.push_back(t9);
		triangles.push_back(t10);
		triangles.push_back(t11);
		triangles.push_back(t12);
		triangles.push_back(t13);
		triangles.push_back(t14);
		triangles.push_back(t15);
		triangles.push_back(t16);
		triangles.push_back(t17);
		triangles.push_back(t18);
		triangles.push_back(t19);
	}

}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSURFACE_H


