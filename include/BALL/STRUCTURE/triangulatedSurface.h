// $Id: triangulatedSurface.h,v 1.9 2000/12/13 15:14:29 strobel Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#define BALL_STRUCTURE_TRIANGULATEDSURFACE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#include <list>
#include <vector>
#include <fstream>

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

		/**	@name	Type Definitions
		*/
		//@{

		/**
		*/
		class Point;
		class Edge;
		class Triangle
		{
			public:
			Triangle() :  point(3), edge(3)/*, triangle(3)*/, index(-1)
			{
			}
			~Triangle()
			{
			}
			Edge* other(Point* p)
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
			vector<Point*> point;
			vector<Edge*> edge;
//			vector<Triangle*> triangle;
			long int index;
		};
		
		class Edge
		{
			public:
			Edge() :  point(2), triangle(2), index(-1)
			{
			}
			~Edge()
			{
			}
			void del(Triangle* p)
			{
				for (Position i = 0; i < triangle.size(); i++)
					{
						if (triangle[i] == p)
							{
								triangle[i] = NULL;
							}
					}
			}
			bool operator == (const Edge& e)
				{
					return ( ((point[0]->p == e->point[0]->p) && (point[1]->p == e->point[1]->p)) ||
									 ((point[0]->p == e->point[1]->p) && (point[1]->p == e->point[0]->p))    );
				}
			vector<Point*> point;
			vector<Triangle*> triangle;
			long int index;
		};

		class Point
		{
			public:
			Point() :  p(), n(), edge(), triangle(), index(-1)
			{
			}
			~Point()
			{
			}
			TVector3<T> p;
			TVector3<T> n;
			std::list<Edge*> edge;
			std::list<Triangle*> triangle;
			Index state;
			long int index;
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSurface object.
		*/
		TTriangulatedSurface()
			: points(), edges(), triangles()
		{
		}

		/**	Copy constructor.
				Create a new RSVertex object from another.
				@param triangulatedSurface the TriangulatedSurface object to be copied
				@param bool ignored - just for interface consistency
		*/
		TTriangulatedSurface(const TTriangulatedSurface<T>& /*surface*/, bool /* deep */ = true)
		{
/*			std::list<Point*>::iterator p;
			for (p = surface.points.begin(); p != surface.points.end(); p++)
			{
				Point* point = new Point(*(*p));
				points.push_back(point);
			}
			std::list<Edge*>::iterator e;
			for (e = surface.edges.begin(); e != surface.edges.end(); e++)
			{
				Edge* edge = new Edge(*(*e));
				edges.push_back(edge);
			}
			std::list<Triangle*>::iterator t;
			for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
			{
				Triangle* triangle = new Triangle(*(*t));
				triangles.push_back(triangle);
			}*/
		}

		/**	Destructor.
				Destructs the TriangulatedSurface object.
		*/
		virtual ~TTriangulatedSurface()
		{
			for (list<Point*>::iterator i = points.begin(); i != points.end(); i++)
			{
				delete *i;
			}
			for (list<Edge*>::iterator i = edges.begin(); i != edges.end(); i++)
			{
				delete *i;
			}
			for (list<Triangle*>::iterator i = triangles.begin(); i != triangles.end(); i++)
			{
				delete *i;
			}
		}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSurface.
				@param triangulatedSurface	the TriangulatedSurface object to assign from
				@param deep ignored
		*/
		void set(const TTriangulatedSurface<T>& triangulatedSurface, bool /* deep */ = true)
		{
			points = triangulatedSurface.points;
			edges = triangulatedSurface.edges;
			triangles = triangulatedSurface.triangles;
		}

		/**	Assign from three lists.
				@param	v assigned to list of vertices
				@param	e assigned to list of edges
				@param	f assigned to list of faces
		*/
		void set(const std::list< Point* >& p, const std::list< Edge* >& e, const std::list< Triangle* >& t)
		{
			points = p; edges = e; triangles = t;
		}
		//@}

		/**	@name	Predicates
		*/
		//@{
		//@}

		/** @name Accessors
		*/
		//@{

		void exportSurface(TSurface<T>& surface);

		TTriangulatedSurface<T>& operator += (TTriangulatedSurface<T>& surface);

		void icosaeder(const bool out);

		void shift(const TVector3<T>& c);

		void blow_up(const T& r);

		void refineSphere(Position iterations, bool out = true);

		void refineSphere(bool out);

		void setIncidences();

		void setIndices();

		void cut(const std::vector< TPlane3<T> >& plane,
						 std::vector< list<TTriangulatedSurface<T>::Edge*> >& border);

		//@}

		/**	@name	Attributes
		*/
		//@{

		/** points.
				The points of the surface.
		*/
		std::list<Point*> points;

		/** edges.
				The edges of the surface.
		*/
		std::list<Edge*> edges;

		/** triangles.
				The triangles of the surface.
		*/
		std::list<Triangle*> triangles;

		//@}
	};

	/**	Output- Operator
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TTriangulatedSurface<T>& surface)
		{
			surface.setIndices();
			std::list<TTriangulatedSurface<T>::Point*>::iterator p;
			std::list<TTriangulatedSurface<T>::Edge*>::iterator e;
			std::list<TTriangulatedSurface<T>::Triangle*>::iterator t;
			s << "Points: " << surface.points.size() << "\n";
			for (p = surface.points.begin(); p != surface.points.end(); p++)
				{
					s << (*p)->index << ": " << (*p)->p << " {";
					for (e = (*p)->edge.begin(); e != (*p)->edge.end(); e++)
						{
							s << (*e)->index << " ";
						}
					s << "} [";
					for (t = (*p)->triangle.begin(); t != (*p)->triangle.end(); t++)
						{
							s << (*t)->index << " ";
						}
					s << "]\n";
				}
			s << "Edges: " << surface.edges.size() << "\n";
			for (e = surface.edges.begin(); e != surface.edges.end(); e++)
				{
					s << (*e)->index << ": (" << (*e)->point[0]->index << " " << (*e)->point[1]->index << ")\t"
						<< "[" << ((*e)->triangle[0] == NULL ? -2 : (*e)->triangle[0]->index)
						<< " " << ((*e)->triangle[1] == NULL ? -2 : (*e)->triangle[1]->index) << "]\n";
				}
			s << "Triangles: " << surface.triangles.size() << "\n";
			for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
				{
					s << (*t)->index << ": ("
						<< (*t)->point[0]->index << " "
						<< (*t)->point[1]->index << " "
						<< (*t)->point[2]->index << ")\t{"
						<< ((*t)->edge[0] == NULL ? -2 : (*t)->edge[0]->index) << " "
						<< ((*t)->edge[1] == NULL ? -2 : (*t)->edge[1]->index) << " "
						<< ((*t)->edge[2] == NULL ? -2 : (*t)->edge[2]->index) //<< "}\n";
						<< "\t" << surface.triangles.size() << "\n";
				}
			return s;
		}

	/**	The Default TriangulatedSurface type.
			If double precision is not needed, {\tt TTriangulatedSurface<float>} should
			be used. It is predefined as {\tt TriangulatedSurface} for convenience.
	*/
	typedef TTriangulatedSurface<float> TriangulatedSurface;



	template <class T>
	void TTriangulatedSurface<T>::exportSurface(TSurface<T>& surface)
	{
		setIndices();
		std::vector< TVector3<T> > surface_points;
		std::vector< TSurface<T>::Triangle > surface_triangles;
		std::vector< TVector3<T> > surface_normals;
		std::list<TTriangulatedSurface<T>::Point*>::iterator p;
		for (p = points.begin(); p != points.end(); p++)
		{
			surface_points.push_back((*p)->p);
			surface_normals.push_back((*p)->n);
		}
		std::list<TTriangulatedSurface<T>::Triangle*>::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			TSurface<T>::Triangle triangle;
			triangle.v1 = (*t)->point[0]->index;
			triangle.v2 = (*t)->point[1]->index;
			triangle.v3 = (*t)->point[2]->index;
			surface_triangles.push_back(triangle);
		}
		surface.vertex = surface_points;
		surface.triangle = surface_triangles;
		surface.normal = surface_normals;
	}


	template <class T>
	TTriangulatedSurface<T>& TTriangulatedSurface<T>::operator += (TTriangulatedSurface<T>& surface)
	{
		std::list<TTriangulatedSurface<T>::Point*>::iterator p;
		for (p = surface.points.begin(); p != surface.points.end(); p++)
		{
			points.push_back(*p);
		}
		std::list<TTriangulatedSurface<T>::Edge*>::iterator e;
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
		{
			edges.push_back(*e);
		}
		std::list<TTriangulatedSurface<T>::Triangle*>::iterator t;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
		{
			triangles.push_back(*t);
		}
		return *this;
	}


	template <class T>
	void TTriangulatedSurface<T>::shift(const TVector3<T>& c)
	{
		std::list<TTriangulatedSurface<T>::Point*>::iterator i;
		for (i = points.begin(); i != points.end(); i++)
		{
			(*i)->p += c;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::blow_up(const T& r)
	{
		std::list<TTriangulatedSurface<T>::Point*>::iterator i;
		for (i = points.begin(); i != points.end(); i++)
		{
			(*i)->p *= r;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::refineSphere(Position iterations, bool out = true)
	{
		for (Position i = 0; i < iterations; i++)
		{
			refineSphere(out);
		}
		for (std::list<Triangle*>::iterator t = triangles.begin(); t != triangles.end(); t++)
		{
			TVector3<T> norm( ((*t)->point[1]->p-(*t)->point[0]->p) %
												((*t)->point[2]->p-(*t)->point[0]->p)   );
			if (Maths::isGreater(norm*(*t)->point[0]->p,0) && (out == false))
			{
				Point* temp = (*t)->point[1];
				(*t)->point[1] = (*t)->point[2];
				(*t)->point[2] = temp;
			}
			if (Maths::isLess(norm*(*t)->point[0]->p,0) && (out == true))
			{
				Point* temp = (*t)->point[1];
				(*t)->point[1] = (*t)->point[2];
				(*t)->point[2] = temp;
			}
		}
		setIncidences();
	}


	template <class T>
	void TTriangulatedSurface<T>::refineSphere(bool out)
	{
		std::list<Edge*> new_edges;
		for (std::list<Edge*>::iterator i = edges.begin(); i != edges.end(); i++)
		{
			Point* point1 = (*i)->point[0];
			Point* point2 = (*i)->point[1];
			Point* new_point = new Point;
			new_point->p = (point1->p+point2->p).normalize();
			if (out == true)
			{
				new_point->n = new_point->p;
			}
			else
			{
				new_point->n = -new_point->p;
			}
			(*i)->triangle[0]->point.push_back(new_point);
			(*i)->triangle[1]->point.push_back(new_point);
			points.push_back(new_point);
			Edge* new_edge1 = new Edge;
			new_edge1->point[0] = point1;
			new_edge1->point[1] = new_point;
			(*i)->triangle[0]->edge.push_back(new_edge1);
			(*i)->triangle[1]->edge.push_back(new_edge1);
			new_edges.push_back(new_edge1);
			Edge* new_edge2 = new Edge;
			new_edge2->point[0] = point2;
			new_edge2->point[1] = new_point;
			(*i)->triangle[0]->edge.push_back(new_edge2);
			(*i)->triangle[1]->edge.push_back(new_edge2);
			new_edges.push_back(new_edge2);
		}
		std::list<Triangle*> new_triangles;
		for (std::list<Triangle*>::iterator i = triangles.begin(); i != triangles.end(); i++)
		{
			Triangle current = *(*i);
			vector< Triangle* > t(4);
			for (int k = 0; k < 4; k++)				// create four new triangles
			{
				t[k] = new Triangle;
			}
			vector<Edge* > e(3);
			for (int k = 0; k < 3; k++)				// create three new edges
			{
				e[k] = new Edge;
			}
			list<Edge*> edge_list;
			for (int k = 3; k < 9; k++)				// list of edges created in the first for-loop
			{																//  that belong to current
				edge_list.push_back(current.edge[k]);
			}
			for (int k = 0; k < 3; k++)				// create a smaller triangle containing current->point[k]
			{
				Edge* first = NULL;
				Edge* second = NULL;
				Point* p1 = NULL;
				Point* p2 = NULL;
				Point* p3 = current.point[k];
				list<Edge*>::iterator l = edge_list.begin();
				while (first == NULL)
				{
					if ((*l)->point[0]->p == p3->p)
					{
						first = *l;
						p1 = (*l)->point[1];
						edge_list.remove(*l);
					}
					if ((*l)->point[1]->p == p3->p)
					{
						first = *l;
						p1 = (*l)->point[0];
						edge_list.remove(*l);
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
					if ((*l)->point[1]->p == p3->p)
					{
						second = *l;
						p2 = (*l)->point[0];
						edge_list.remove(*l);
					}
					l++;
				}
				t[k]->point[0] = p1;
				t[k]->point[1] = p2;
				t[k]->point[2] = p3;
				t[k]->edge[0] = first;
				t[k]->edge[1] = second;
				t[k]->edge[2] = e[k];
				if (first->triangle[0] == NULL)
				{
					first->triangle[0] = t[k];
				}
				else
				{
					first->triangle[1] = t[k];
				}
				if (second->triangle[0] == NULL)
				{
					second->triangle[0] = t[k];
				}
				else
				{
					second->triangle[1] = t[k];
				}
				e[k]->triangle[0] = t[k];
				e[k]->triangle[1] = t[4];
				e[k]->point[0] = p1;
				e[k]->point[1] = p2;
			}
			t[3]->point[0] = current.point[3];
			t[3]->point[1] = current.point[4];
			t[3]->point[2] = current.point[5];
			t[3]->edge[0] = e[0];
			t[3]->edge[1] = e[1];
			t[3]->edge[2] = e[2];
			e[0]->triangle[1] = t[3];
			e[1]->triangle[1] = t[3];
			e[2]->triangle[1] = t[3];
			new_edges.push_back(e[0]);
			new_edges.push_back(e[1]);
			new_edges.push_back(e[2]);
			new_triangles.push_back(t[0]);
			new_triangles.push_back(t[1]);
			new_triangles.push_back(t[2]);
			new_triangles.push_back(t[3]);
			delete *i;
		}
		edges.erase(edges.begin(),edges.end());
		edges = new_edges;
		triangles.erase(triangles.begin(),triangles.end());
		triangles = new_triangles;
	}


	template <class T>
	void TTriangulatedSurface<T>::setIncidences()
	{
		std::list<TTriangulatedSurface<T>::Point*>::iterator p;
		for (p = points.begin(); p != points.end(); p++)
		{
			(*p)->edge.clear();
			(*p)->triangle.clear();
		}
		std::list<TTriangulatedSurface<T>::Edge*>::iterator e;
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
		std::list<TTriangulatedSurface<T>::Point*>::iterator p;
		for (p = points.begin(); p != points.end(); p++)
		{
			(*p)->index = i;
			i++;
		}
		i = 0;
		std::list<TTriangulatedSurface<T>::Edge*>::iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			(*e)->index = i;
			i++;
		}
		i = 0;
		std::list<TTriangulatedSurface<T>::Triangle*>::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			(*t)->index = i;
			i++;
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::cut
		(const std::vector< TPlane3<T> >& plane,
		 std::vector< std::list<TTriangulatedSurface<T>::Edge*> >& border)
	{
		setIndices();
		std::vector<Position> cutting_plane(edges.size());
		std::list<TTriangulatedSurface<T>::Edge*> empty;
		for (Position i = 0;  i < plane.size(); i++)
		{
			border.push_back(empty);
			std::list< TTriangulatedSurface<T>::Point* >::iterator p;
			for (p = points.begin(); p != points.end(); p++)
			{
				(*p)->state = 0;
			}
			T test_value = plane[i].n*plane[i].p;
			std::list< TTriangulatedSurface<T>::Triangle* >::iterator t;
			t = triangles.begin();
			while (t != triangles.end())
			{
				TTriangulatedSurface<T>::Triangle* triangle = *t;
				t++;
				bool del = false;
				for (Position j = 0; j < 3; j++)
				{
					switch (triangle->point[j]->state)
					{
						case 0 :	if (Maths::isLess(plane[i].n*triangle->point[j]->p,test_value))
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
		HashSet< TTriangulatedSurface<T>::Edge* > existing_edges;
		HashSet< TTriangulatedSurface<T>::Point* > existing_points;
		std::list< TTriangulatedSurface<T>::Triangle* >::iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			for (Position k = 0; k < 3; k++)
			{
				TTriangulatedSurface<T>::Edge* edge = (*t)->edge[k];
				existing_edges.insert(edge);
				edges.remove(edge);
				if ((edge->triangle[0] == NULL) || (edge->triangle[1] == NULL))
				{
					border[cutting_plane[edge->index]].push_back(edge);
				}
				TTriangulatedSurface<T>::Point* point = (*t)->point[k];
				existing_points.insert(point);
				points.remove(point);
			}
		}
		while (edges.size() > 0)
		{
			TTriangulatedSurface<T>::Edge* edge = edges.front();
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
		HashSet< TTriangulatedSurface<T>::Edge* >::Iterator e;
		for (e = existing_edges.begin(); e != existing_edges.end(); e++)
		{
			edges.push_back(*e);
		}
		HashSet< TTriangulatedSurface<T>::Point* >::Iterator point;
		for (point = existing_points.begin(); point != existing_points.end(); point++)
		{
			points.push_back(*point);
		}
	}


	template <class T>
	void TTriangulatedSurface<T>::icosaeder(const bool out)
	{
		TTriangulatedSurface<T>::Point* point0  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point1  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point2  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point3  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point4  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point5  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point6  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point7  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point8  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point9  = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point10 = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Point* point11 = new TTriangulatedSurface<T>::Point;
		TTriangulatedSurface<T>::Edge* edge0  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge1  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge2  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge3  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge4  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge5  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge6  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge7  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge8  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge9  = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge10 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge11 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge12 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge13 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge14 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge15 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge16 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge17 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge18 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge19 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge20 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge21 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge22 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge23 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge24 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge25 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge26 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge27 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge28 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Edge* edge29 = new  TTriangulatedSurface<T>::Edge;
		TTriangulatedSurface<T>::Triangle* t0  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t1  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t2  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t3  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t4  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t5  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t6  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t7  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t8  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t9  = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t10 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t11 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t12 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t13 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t14 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t15 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t16 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t17 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t18 = new TTriangulatedSurface<T>::Triangle;
		TTriangulatedSurface<T>::Triangle* t19 = new TTriangulatedSurface<T>::Triangle;

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
		blow_up(0.5);
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


