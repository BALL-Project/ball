// $Id: triangulatedSurface.h,v 1.2 2000/10/11 09:40:57 oliver Exp $

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
			Triangle() :  point(3), edge(3), triangle(3), index(-1)
			{
			}
			~Triangle()
			{
			}
			vector<Point*> point;
			vector<Edge*> edge;
			vector<Triangle*> triangle;
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
			list<Edge*> edge;
			list<Triangle*> triangle;
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
		TTriangulatedSurface(const TTriangulatedSurface<T>& triangulatedSurface, bool /* deep */ = true)
			: points(triangulatedSurface.points),
				edges(triangulatedSurface.edges),
				triangles(triangulatedSurface.triangles)
		{
		}

		/**	Destructor.
				Destructs the TriangulatedSurface object.
		*/
		virtual ~TTriangulatedSurface()
		{
			for (list< Point<T>* >::iterator i = points.begin(); i != points.end(); i++)
				{
					delete *i;
				}
			for (list< Edge<T>* >::iterator i = edges.begin(); i != edges.end(); i++)
				{
					delete *i;
				}
			for (list< Triangle<T>* >::iterator i = triangles.begin(); i != triangles.end(); i++)
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
		void set(const list< Point* >& p, const list< Edge* >& e, const list< Triangle* >& t)
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

		TSurface<T> exportSurface()
		{
			setIndices();
			std::vector<TVector3<T> > surface_points;
			std::vector<TSurface<T>::Triangle> surface_triangles;
			std::vector<TVector3<T> > surface_normals;
			list<Point*>::iterator p;
			for (p = points.begin(); p != points.end(); p++)
			{
				surface_points.push_back((*p)->p);
				surface_normals.push_back((*p)->n);
			}
			list<Triangle*>::iterator t;
			for (t = triangles.begin(); t != triangles.end(); t++)
			{
				TSurface<T>::Triangle triangle;
				triangle.v1 = (*t)->point[0]->index;
				triangle.v2 = (*t)->point[1]->index;
				triangle.v3 = (*t)->point[2]->index;
				surface_triangles.push_back(triangle);
			}
			
			TSurface<T> surface;
			surface.vertex = surface_points;
			surface.triangle = surface_triangles;
			surface.normal = surface_normals;

			return surface;
		}


		TTriangulatedSurface<T>& operator += (TTriangulatedSurface<T>& surface)
		{
			for (list<Point*>::iterator p = surface.points.begin(); p != surface.points.end(); p++)
				{
					points.push_back(*p);
				}
			for (list<Edge*>::iterator e = surface.edges.begin(); e != surface.edges.end(); e++)
				{
					edges.push_back(*e);
				}
			for (list<Triangle*>::iterator t = surface.triangles.begin(); t != surface.triangles.end(); t++)
				{
					triangles.push_back(*t);
				}
			return *this;
		}


		void isokaeder(const T& radius)
		{
			TTriangulatedSurface<T>::Point* point1 = new TTriangulatedSurface<T>::Point();
			TTriangulatedSurface<T>::Point* point2 = new TTriangulatedSurface<T>::Point();
			TTriangulatedSurface<T>::Point* point3 = new TTriangulatedSurface<T>::Point();
			TTriangulatedSurface<T>::Point* point4 = new TTriangulatedSurface<T>::Point();
			TTriangulatedSurface<T>::Edge* edge1 = new  TTriangulatedSurface<T>::Edge();
			TTriangulatedSurface<T>::Edge* edge2 = new  TTriangulatedSurface<T>::Edge();
			TTriangulatedSurface<T>::Edge* edge3 = new  TTriangulatedSurface<T>::Edge();
			TTriangulatedSurface<T>::Edge* edge4 = new  TTriangulatedSurface<T>::Edge();
			TTriangulatedSurface<T>::Edge* edge5 = new  TTriangulatedSurface<T>::Edge();
			TTriangulatedSurface<T>::Edge* edge6 = new  TTriangulatedSurface<T>::Edge();
			TTriangulatedSurface<T>::Triangle* t1 = new  TTriangulatedSurface<T>::Triangle();
			TTriangulatedSurface<T>::Triangle* t2 = new  TTriangulatedSurface<T>::Triangle();
			TTriangulatedSurface<T>::Triangle* t3 = new  TTriangulatedSurface<T>::Triangle();
			TTriangulatedSurface<T>::Triangle* t4 = new  TTriangulatedSurface<T>::Triangle();

			point1->p = TVector3<T>(-sqrt(6)/3*radius,-sqrt(2)/3*radius,-radius/3);
			point1->edge.push_back(edge1); point1->edge.push_back(edge2); point1->edge.push_back(edge6);
			point1->triangle.push_back(t1); point1->triangle.push_back(t3); point1->triangle.push_back(t4);
			point1->n = point1->p;
			points.push_back(point1);
			point2->p = TVector3<T>(sqrt(6)/3*radius,-sqrt(2)/3*radius,-radius/3);
			point2->edge.push_back(edge2); point2->edge.push_back(edge3); point2->edge.push_back(edge5);
			point2->triangle.push_back(t1); point2->triangle.push_back(t2); point2->triangle.push_back(t4);
			point2->n = point2->p;
			points.push_back(point2);
			point3->p = TVector3<T>(0,2*sqrt(2)/3*radius,-radius/3);
			point3->edge.push_back(edge4); point3->edge.push_back(edge5); point3->edge.push_back(edge6);
			point3->triangle.push_back(t2); point3->triangle.push_back(t3); point3->triangle.push_back(t4);
			point3->n = point3->p;
			points.push_back(point3);
			point4->p = TVector3<T>(0,0,radius);
			point4->edge.push_back(edge1); point4->edge.push_back(edge3); point4->edge.push_back(edge4);
			point4->triangle.push_back(t1); point4->triangle.push_back(t2); point4->triangle.push_back(t3);
			point4->n = point4->p;
			points.push_back(point4);

			edge1->point[0] = point1; edge1->point[1] = point4;
			edge1->triangle[0] = t1; edge1->triangle[1] = t3;
			edges.push_back(edge1);
			edge2->point[0] = point1; edge2->point[1] = point2;
			edge2->triangle[0] = t1; edge2->triangle[1] = t4;
			edges.push_back(edge2);
			edge3->point[0] = point2; edge3->point[1] = point4;
			edge3->triangle[0] = t1; edge3->triangle[1] = t2;
			edges.push_back(edge3);
			edge4->point[0] = point3; edge4->point[1] = point4;
			edge4->triangle[0] = t2; edge4->triangle[1] = t3;
			edges.push_back(edge4);
			edge5->point[0] = point2; edge5->point[1] = point3;
			edge5->triangle[0] = t2; edge5->triangle[1] = t4;
			edges.push_back(edge5);
			edge6->point[0] = point1; edge6->point[1] = point3;
			edge6->triangle[0] = t4; edge6->triangle[1] = t3;
			edges.push_back(edge6);

			t1->point[0] = point1; t1->point[1] = point2; t1->point[2] = point4;
			t1->edge[0] = edge1; t1->edge[1] = edge2; t1->edge[2] = edge3;
			t1->triangle[0] = t2; t1->triangle[1] = t3; t1->triangle[2] = t4;
			triangles.push_back(t1);
			t2->point[0] = point3; t2->point[1] = point2; t2->point[2] = point4;
			t2->edge[0] = edge3; t2->edge[1] = edge4; t2->edge[2] = edge5;
			t2->triangle[0] = t1; t2->triangle[1] = t3; t2->triangle[2] = t4;
			triangles.push_back(t2);
			t3->point[0] = point1; t3->point[1] = point3; t3->point[2] = point4;
			t3->edge[0] = edge1; t3->edge[1] = edge4; t3->edge[2] = edge6;
			t3->triangle[0] = t2; t3->triangle[1] = t1; t3->triangle[2] = t4;
			triangles.push_back(t3);
			t4->point[0] = point1; t4->point[1] = point2; t4->point[2] = point3;
			t4->edge[0] = edge2; t4->edge[1] = edge5; t4->edge[2] = edge6;
			t4->triangle[0] = t2; t4->triangle[1] = t3; t4->triangle[2] = t1;
			triangles.push_back(t4);
		}


		void shift(const TVector3<T>& c)
		{
			for (list<Point*>::iterator i = points.begin(); i != points.end(); i++)
				{
					(*i)->p += c;
				}
		}


		void refineSphere(const T& radius, const int& iterations)
		{
			for (int i = 0; i < iterations; i++)
				{
					refineSphere(radius);
				}
			setIncidences();
		}

		void refineSphere(const T& radius)
		{
			list<Edge<T>*> new_edges;
			for (list<Edge*>::iterator i = edges.begin(); i != edges.end(); i++)
				{
					Point<T>* point1 = (*i)->point[0];
					Point<T>* point2 = (*i)->point[1];
					Point<T>* new_point = new Point<T>();
					new_point->p = (point1->p+point2->p).normalize();
					new_point->p *= radius;
					new_point->n = new_point->p;
					(*i)->triangle[0]->point.push_back(new_point);
					(*i)->triangle[1]->point.push_back(new_point);
					points.push_back(new_point);
					Edge<T>* new_edge1 = new Edge<T>();
					new_edge1->point[0] = point1;
					new_edge1->point[1] = new_point;
					(*i)->triangle[0]->edge.push_back(new_edge1);
					(*i)->triangle[1]->edge.push_back(new_edge1);
					new_edges.push_back(new_edge1);
					Edge<T>* new_edge2 = new Edge<T>();
					new_edge2->point[0] = point2;
					new_edge2->point[1] = new_point;
					(*i)->triangle[0]->edge.push_back(new_edge2);
					(*i)->triangle[1]->edge.push_back(new_edge2);
					new_edges.push_back(new_edge2);
				}
			list<Triangle*> new_triangles;
			for (list<Triangle*>::iterator i = triangles.begin(); i != triangles.end(); i++)
				{
					Triangle current = *(*i);
					vector< Triangle<T>* > t(4);
					for (int k = 0; k < 4; k++)				// create four new triangles
						{
							t[k] = new Triangle<T>();
						}
					vector< Edge<T>* > e(3);
					for (int k = 0; k < 3; k++)				// create three new edges
						{
							e[k] = new Edge<T>();
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


		void setIncidences()
		{
			for (list<Point*>::iterator p = points.begin(); p != points.end(); p++)
				{
					(*p)->edge.erase((*p)->edge.begin(),(*p)->edge.end());
					(*p)->triangle.erase((*p)->triangle.begin(),(*p)->triangle.end());
				}
			for (list<Edge*>::iterator e = edges.begin(); e != edges.end(); e++)
				{
					(*e)->point[0]->edge.push_back(*e);
					(*e)->point[0]->triangle.push_back((*e)->triangle[0]);
					(*e)->point[0]->triangle.push_back((*e)->triangle[1]);
					(*e)->point[1]->edge.push_back(*e);
					(*e)->point[1]->triangle.push_back((*e)->triangle[0]);
					(*e)->point[1]->triangle.push_back((*e)->triangle[1]);
				}
			for (list<Point*>::iterator p = points.begin(); p != points.end(); p++)
				{
					(*p)->edge.sort();
					(*p)->edge.unique();
					(*p)->triangle.sort();
					(*p)->triangle.unique();
				}
		}


		void setIndices()
		{
			long int i = 0;
			for (list<Point*>::iterator p = points.begin(); p != points.end(); p++)
				{
					(*p)->index = i;
					i++;
				}
			i = 0;
			for (list<Edge*>::iterator e = edges.begin(); e != edges.end(); e++)
				{
					(*e)->index = i;
					i++;
				}
			i = 0;
			for (list<Triangle*>::iterator t = triangles.begin(); t != triangles.end(); t++)
				{
					(*t)->index = i;
					i++;
				}
		}

		//@}

		/**	@name	Attributes
		*/
		//@{

		/** points.
				The points of the surface.
		*/
		list<Point*> points;

		/** edges.
				The edges of the surface.
		*/
		list<Edge*> edges;

		/** triangles.
				The triangles of the surface.
		*/
		list<Triangle*> triangles;

		//@}
	};

	/**	Output- Operator
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TTriangulatedSurface<T>& surface)
		{
			surface.setIndices();
			list<TTriangulatedSurface<T>::Point*>::iterator p;
			list<TTriangulatedSurface<T>::Edge*>::iterator e;
			list<TTriangulatedSurface<T>::Triangle*>::iterator t;
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
//					s << (*e)->index << ": (" << (*e)->point[0]->index << " " << (*e)->point[1]->index << ")\n";
//						<< "[" << (*e)->triangle[0]->index << " " << (*e)->triangle[1]->index << "]\n";
				}
			s << "Triangles: " << surface.triangles.size() << "\n";
			for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
				{
					s << (*t)->index << ": ("
						<< (*t)->point[0]->index<<" " << (*t)->point[1]->index << " "	<< (*t)->point[2]->index << ")\t{"
						<< (*t)->edge[0]->index << " " << (*t)->edge[1]->index << " "	<< (*t)->edge[2]->index << "}\n";
				}
			return s;
		}

	/**	The Default TriangulatedSurface type.
			If double precision is not needed, {\tt TTriangulatedSurface<float>} should
			be used. It is predefined as {\tt TriangulatedSurface} for convenience.
	*/
	typedef TTriangulatedSurface<float> TriangulatedSurface;

}	// namespace BALL

#endif	// BALL_STRUCTURE_TRIANGULATEDSURFACE_H


