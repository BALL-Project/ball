// $Id: triangulation.h,v 1.18 2001/02/25 18:33:44 strobel Exp $

#ifndef BALL_STRUCTURE_TRIANGULATION_H
#define BALL_STRUCTURE_TRIANGULATION_H

//#define debug_triangulation
#ifdef debug_triangulation
#	define with_bonds
#endif
//#define print_debug_info

#ifndef BALL_STRUCTURE_SESVERTEX_H
#	include <BALL/STRUCTURE/SESVertex.h>
#endif

#ifndef BALL_STRUCTURE_SESEDGE_H
#	include <BALL/STRUCTURE/SESEdge.h>
#endif

#ifndef BALL_STRUCTURE_SESFACE_H
#	include <BALL/STRUCTURE/SESFace.h>
#endif

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventExcludedSurface.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#include <vector>
#include <list>
#include <fstream>

#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/HINFile.h>
#include <string>

namespace BALL
{


	/**
	*/
	template <class T>
	TTriangulatedSurface<T>* Triangulate(TSolventExcludedSurface<T>* ses,
																			 TReducedSurface<T>* rs,
																			 const T& density)
	{
		TTriangulatedSurface<T>* surface = new TTriangulatedSurface<T>;
		T radius_of_probe(rs->getProbeRadius());

		T test0 = (4.0*density*Constants::PI*
							 radius_of_probe*radius_of_probe-12.0)/30.0;
		Position n;
		if (Maths::isLess(test0,(T)0))
		{
			n = 0;
		}
		else
		{
			n = 0;
			T test1;
			T test2 = 1.0;
			while (Maths::isLess(test2,test0))
			{
				test1 = test2;
				test2 *= 4.0;
				n++;
			}
			if (Maths::isLess(test2-test0,test0-test1))
			{
				n++;
			}
		}
		TTriangulatedSurface<T>* probe = new TTriangulatedSurface<T>;
		probe->icosaeder(false);
		probe->refineSphere(n,false);
		probe->blowUp(radius_of_probe);
		HashMap<T,TTriangulatedSurface<T>*> spheres;

		std::vector<TTrianglePoint<T>*> point(ses->vertices.size());
		std::vector< std::list<TTriangleEdge<T>*> >
				edge(ses->edges.size());

		std::vector<TTriangulatedSurface<T>*> toric_face(ses->toric_faces.size());
		std::vector<TTriangulatedSurface<T>*> contact_face;
		std::vector<TTriangulatedSurface<T>*> spheric_face;

		//std::vector< std::list<Index> > contact_contour;
		//std::vector< std::list<Index> > spheric_contour;
		std::vector< std::list<Index> > contact_border;
		std::vector< std::list<Index> > spheric_border;

		//std::vector< std::vector<TSESEdge<T>*> > contact_border;
		//std::vector< std::vector<TSESEdge<T>*> > spheric_border;

		for (Position i = 0;  i < ses->vertices.size(); i++)
		{
			point[i] = new TTrianglePoint<T>;
			point[i]->p = ses->vertices[i]->p;
			point[i]->n = ses->vertices[i]->n;
			surface->points.push_back(point[i]);
		}
				std::cerr << "  triangulate toric faces\n";
		T sqrt_density = sqrt(density);
		for (Position i = 0; i < ses->toric_faces.size(); i++)
		{
			if (ses->toric_faces[i] != NULL)
			{
				TriangulateToricFace(ses->toric_faces[i],radius_of_probe,sqrt_density,
														 toric_face[i],point,edge
														 #ifdef debug_triangulation
														 ,"hin/molecule/toricFace"+IndexToString(i,0)+".hin"
														 #endif
														 );
				for (Position k = 0; k < ses->toric_faces[i]->vertex.size(); k++)
				{
					toric_face[i]->points.remove(point[ses->toric_faces[i]->vertex[k]->index]);
				}
				*surface += (*toric_face[i]);
			}
		}
				std::cerr << "  treat singular edges\n";
		for (Position i = 0; i < ses->singular_edges.size(); i++)
		{
			TSESEdge<T>* singular_edge = ses->singular_edges[i];
			TAngle<T> phi;
			GetAngle(singular_edge->vertex1->p-singular_edge->circle.p,
							 singular_edge->vertex2->p-singular_edge->circle.p,
							 phi);
			Size number_of_segments
					= (Size)Maths::round(phi.toRadian()*
															 singular_edge->circle.radius*
															 sqrt_density);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<T> psi(phi.toRadian()/number_of_segments,true);
			vector< TVector3<T> > points;
			TVector3<T> normal((singular_edge->vertex1->p-singular_edge->circle.p)%
												 (singular_edge->vertex2->p-singular_edge->circle.p));
			singular_edge->circle.n = normal;
			points = PartitionOfCircle(singular_edge->circle,
																 singular_edge->vertex1->p,
																 psi,number_of_segments);
			points.pop_back();
			points.push_back(singular_edge->vertex2->p);
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = point[singular_edge->vertex1->index];
			for (Position k = 1; k < points.size()-1; k++)
			{
				p2 = new TTrianglePoint<T>;
				p2->p = points[k];
				p2->n = p2->p-singular_edge->circle.p;
				surface->points.push_back(p2);
				e = new TTriangleEdge<T>;
				e->point[0] = p1;
				e->point[1] = p2;
				e->triangle.clear();
				edge[singular_edge->index].push_back(e);
				p1->edge.push_back(e);
				p2->edge.push_back(e);
				p1 = p2;
			}
			p2 = point[singular_edge->vertex2->index];
			e = new TTriangleEdge<T>;
			e->point[0] = p1;
			e->point[1] = p2;
			e->triangle.clear();
			edge[singular_edge->index].push_back(e);
			p1->edge.push_back(e);
			p2->edge.push_back(e);
		}
				std::cerr << "  triangulate contact faces\n";
		for (Position i = 0; i < ses->contact_faces.size(); i++)
		{
			if (ses->contact_faces[i] != NULL)
			{
				TSphere3<T> sphere(rs->getSphere(ses->contact_faces[i]->rsvertex->getAtom()));
				TriangulateSphericFace(ses->contact_faces[i],sphere,false,spheres,probe,density,
															 contact_face,contact_border
															 #ifdef debug_triangulation
															 ,"hin/molecule/contactFace"+IndexToString(i,0)+".hin"
															 #endif
															 );
				*surface += (*contact_face[i]);
				surface->setIndices();
				double epsilon = Constants::EPSILON;
				Constants::EPSILON = 1e-8;
				SewFace(contact_border[i],contact_face[i]->points,edge,
								ses->edges,sphere,true,surface,i,(T)0);
				Constants::EPSILON = epsilon;
			}
		}
				std::cerr << "  triangulate spheric reentrant faces\n";
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
		{
			TSphere3<T> sphere(ses->spheric_faces[i]->rsface->getCenter(),radius_of_probe);
			TriangulateSphericFace(ses->spheric_faces[i],sphere,true,spheres,probe,density,
														 spheric_face,spheric_border
														 #ifdef debug_triangulation
														 "hin/molecule/sphericFace"+IndexToString(i,0)+".hin"
														 #endif
														 );
			*surface += (*spheric_face[i]);
			surface->setIndices();
			double epsilon = Constants::EPSILON;
			Constants::EPSILON = 1e-8;
			SewFace(spheric_border[i],spheric_face[i]->points,edge,
							ses->edges,sphere,false,surface,i+contact_face.size(),(T)0);
			Constants::EPSILON = epsilon;
		}
		return surface;
	}


	template <class T>
	void TriangulateToricFace
				(TSESFace<T>* face, const T& radius_of_probe, const T& sqrt_density,
				 TTriangulatedSurface<T>*& surface,
				 std::vector<TTrianglePoint<T>*> points,
				 std::vector
				 		<
				 			std::list<TTriangleEdge<T>*>
				 		>& border
				 #ifdef debug_triangulation
				 , string filename
				 #endif
				 )
	{
					//cout << "#1\n";
		surface = new TTriangulatedSurface<T>;
		if (face->isFree())
		{
			TriangulateFreeToricFace(face,radius_of_probe,sqrt_density,
															 points,surface,border
															 #ifdef debug_triangulation
															 ,filename
															 #endif
															 );
			return;
		}
					//cout << "#2\n";
		if (face->rsedge->isSingular())
		{
			TriangulateSingularToricFace(face,radius_of_probe,sqrt_density,
																	 points,surface,border
																	 #ifdef debug_triangulation
																	 ,filename
																	 #endif
																	 );
			return;
		}
					//cout << "#3\n";
		face->normalize(false);
		TSESEdge<T>* edge0 = face->edge[0];
		TSESEdge<T>* edge1 = face->edge[1];
		TSESEdge<T>* edge2 = face->edge[2];
		TSESEdge<T>* edge3 = face->edge[3];
		TSESVertex<T>* p0 = face->vertex[0];
		TSESVertex<T>* p1 = face->vertex[1];
		TSESVertex<T>* p2 = face->vertex[2];
		TSESVertex<T>* p3 = face->vertex[3];
					//cout << "#4\n";
		Size number_of_segments =
			(Size)Maths::round(face->rsedge->getPhi().toRadian()*
												 edge3->circle.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
					//cout << "#5\n";
		TAngle<T> psi(face->rsedge->getPhi().toRadian()/number_of_segments,true);
		vector< TVector3<T> > edge3_segments;
		vector< TVector3<T> > edge1_segments;
		TVector3<T> normal((p0->p-edge3->circle.p)%(p3->p-edge3->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().toRadian(),Constants::PI))
		{
			normal.negate();
		}
					//cout << "#6\n";
		TCircle3<T> circle3(edge3->circle.p,normal,edge3->circle.radius);
		TCircle3<T> circle1(edge1->circle.p,normal,edge1->circle.radius);
		edge3_segments = PartitionOfCircle(circle3,p0->p,psi,number_of_segments);
		edge3_segments.pop_back();
		edge3_segments.push_back(p3->p);
		edge1_segments = PartitionOfCircle(circle1,p1->p,psi,number_of_segments);
		edge1_segments.pop_back();
		edge1_segments.push_back(p2->p);
		TCircle3<T> center_circle(face->rsedge->getCenterOfTorus(),
															normal,
															face->rsedge->getMajorRadiusOfTorus());
		vector< TVector3<T> > centers;
		centers = PartitionOfCircle(center_circle,edge0->circle.p,psi,number_of_segments);
		centers.pop_back();
		centers.push_back(edge2->circle.p);
					//cout << "#7\n";
		BuildTriangles(face,edge0,edge1,edge2,edge3,centers,edge1_segments,edge3_segments,
									 points,radius_of_probe,sqrt_density,surface,border
									 #ifdef debug_triangulation
									 ,filename
									 #endif
									 );
					//cout << "#8\n";
		TTriangle<T>* test_triangle = surface->triangles.front();
		TVector3<T> orth( (test_triangle->point[1]->p-test_triangle->point[0]->p) %
											(test_triangle->point[2]->p-test_triangle->point[0]->p));
					//cout << "#9\n";
		if (Maths::isGreater(orth*test_triangle->point[0]->p,orth*centers[0]))
		{
			std::list<TTriangle<T>*>::iterator t;
			for (t = surface->triangles.begin(); t != surface->triangles.end(); t++)
			{
				TTrianglePoint<T>* temp = (*t)->point[0];
				(*t)->point[0] = (*t)->point[1];
				(*t)->point[1] = temp;
			}
		}
					//cout << "#10\n";
	}


	template <class T>
	void TriangulateFreeToricFace
		(TSESFace<T>* face, const T& radius_of_probe, const T& sqrt_density,
		 std::vector<TTrianglePoint<T>*> points,
		 TTriangulatedSurface<T>* surface,
		 std::vector< list<TTriangleEdge<T>*> >& border
		 #ifdef debug_triangulation
		 , string filename
		 #endif
		 )
	{
		TVector3<T> normal(face->edge[0]->circle.n);
		TCircle3<T> circle1(face->edge[0]->circle);
		TCircle3<T> circle2(face->edge[1]->circle.p,normal,face->edge[1]->circle.radius);
		TCircle3<T> circle3(face->rsedge->getCenterOfTorus(),normal,face->rsedge->getMajorRadiusOfTorus());
		TVector3<T> orth(normal.y,-normal.x,(T)0);
		if (orth == TVector3<T>::getZero())
		{
			orth.set(normal.z,(T)0,-normal.x);
		}
		orth.normalize();
		TVector3<T> p1(circle1.p+(orth*circle1.radius));
		TVector3<T> p2(circle2.p+(orth*circle2.radius));
		TVector3<T> p3(circle3.p+(orth*circle3.radius));
		Size number_of_points
			= (Size)Maths::round(2*Constants::PI*circle1.radius*sqrt_density);
		if (number_of_points == 0)
		{
			number_of_points++;
		}
		TAngle<T> phi(2*Constants::PI/number_of_points,true);
		std::vector< TVector3<T> > points1;
		std::vector< TVector3<T> > points2;
		std::vector< TVector3<T> > centers;
		points1 = PartitionOfCircle(circle1,p1,phi,number_of_points);
		points2 = PartitionOfCircle(circle2,p2,phi,number_of_points);
		centers = PartitionOfCircle(circle3,p3,phi,number_of_points);
		points1.pop_back();
		points2.pop_back();
		centers.pop_back();
		TSESEdge<T>* dummy(NULL);
		BuildTriangles(face,dummy,face->edge[0],dummy,face->edge[1],centers,points1,points2,
									 points,radius_of_probe,sqrt_density,surface,border
									 #ifdef debug_triangulation
									 ,filename
									 #endif
									 );
	}


	template <class T>
	void TriangulateSingularToricFace
				(TSESFace<T>* face, const T& radius_of_probe, const T& sqrt_density,
				 std::vector< TTrianglePoint<T>* > points,
				 TTriangulatedSurface<T>* surface,
				 std::vector< std::list<TTriangleEdge<T>*> >& border
				 #ifdef debug_triangulation
				 , string filename
				 #endif
				 )
	{
					//cout << "$1\n";
		face->normalize(true);
		Size number_of_segments
				= (Size)Maths::round(face->rsedge->getPhi().toRadian()*
														 face->edge[1]->circle.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
					//cout << "$2\n";
		TAngle<T> psi(face->rsedge->getPhi().toRadian()/number_of_segments,true);
		TVector3<T> normal((face->vertex[0]->p-face->edge[0]->circle.p)%
											 (face->vertex[2]->p-face->edge[0]->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().toRadian(),Constants::PI))
		{
			normal.negate();
		}
		Position offset = 0;
		list<TTriangle<T>*>::iterator last = surface->triangles.end();
					//cout << "$3\n";
		for (Position counter = 0; counter < 2; counter++)
		{
			TCircle3<T> circle(face->edge[0+offset]->circle.p,
												 normal,
												 face->edge[0+offset]->circle.radius);
			vector< TVector3<T> > edge_segments;
			edge_segments = PartitionOfCircle(circle,face->vertex[0+offset]->p,psi,number_of_segments);
			edge_segments.pop_back();
			edge_segments.push_back(face->vertex[2+offset]->p);
			vector< TVector3<T> > point;
					//cout << "$4\n";
			for (Position i = 0; i < number_of_segments+1; i++)
			{
				point.push_back(face->vertex[1+offset]->p);
			}
			TCircle3<T> center_circle(face->rsedge->getCenterOfTorus(),
																normal,
																face->rsedge->getMajorRadiusOfTorus());
			vector< TVector3<T> > centers;
			centers = PartitionOfCircle(center_circle,face->edge[1+offset]->circle.p,psi,number_of_segments);
					//cout << "$5\n";
			centers.pop_back();
			centers.push_back(face->edge[2+offset]->circle.p);
			TSESEdge<T>* dummy(NULL);
					#ifdef debug_triangulation
					filename += ".";
					filename += IndexToString(counter,0);
					filename += ".hin";
					#endif
					//cout << "$6\n";
			BuildTriangles(face,face->edge[1+offset],face->edge[0+offset],face->edge[2+offset],dummy,
										 centers,edge_segments,point,points,radius_of_probe,sqrt_density,surface,border
										 #ifdef debug_triangulation
										 , filename
										 #endif
										 );
					//cout << "$7\n";
			offset += 3;
			TTriangle<T>* test_triangle;
			if (last == surface->triangles.end())
			{
				test_triangle = surface->triangles.front();
				last--;
			}
			else
			{
				last++;
				test_triangle = *last;
			}
			TVector3<T> orth( (test_triangle->point[1]->p-test_triangle->point[0]->p) %
												(test_triangle->point[2]->p-test_triangle->point[0]->p));
			if (Maths::isGreater(orth*test_triangle->point[0]->p,orth*centers[0]))
			{
				std::list<TTriangle<T>*>::iterator t;
				for (t = surface->triangles.begin(); t != surface->triangles.end(); t++)
				{
					TTrianglePoint<T>* temp = (*t)->point[0];
					(*t)->point[0] = (*t)->point[1];
					(*t)->point[1] = temp;
				}
			}
					//cout << "$8\n";
		}
					//cout << "$9\n";
	}


	template <class T>
	void TriangulateSphericFace
		(TSESFace<T>* face, const TSphere3<T>& sphere, bool probe,
		 HashMap<T,TTriangulatedSurface<T>*>& triangulated_spheres,
		 TTriangulatedSurface<T>* probe_sphere, T density,
		 std::vector<TTriangulatedSurface<T>*>& triangulated_faces,
		 std::vector< std::list<Index> >& border
				#ifdef debug_triangulation
				, string filename
				#endif
		 )
	{
		TTriangulatedSurface<T>* triangulated_face;
		if (probe == true)
		{
			triangulated_face = new TTriangulatedSurface<T>(*probe_sphere);
		}
		else
		{
			if (triangulated_spheres.has(sphere.radius) == false)
			{
				T test0 = (4.0*density*Constants::PI*
									 sphere.radius*sphere.radius-12.0)/30.0;
				Position n;
				if (Maths::isLess(test0,(T)0))
				{
					n = 0;
				}
				else
				{
					n = 0;
					T test1;
					T test2 = 1.0;
					while (Maths::isLess(test2,test0))
					{
						test1 = test2;
						test2 *= 4.0;
						n++;
					}
					if (Maths::isLess(test2-test0,test0-test1))
					{
						n++;
					}
				}
				TTriangulatedSurface<T>* template_sphere = new TTriangulatedSurface<T>;
				template_sphere->icosaeder(true);
				template_sphere->refineSphere(n,true);
				template_sphere->blowUp(sphere.radius);
				triangulated_spheres[sphere.radius] = template_sphere;
			}
			triangulated_face
				= new TTriangulatedSurface<T>(*triangulated_spheres[sphere.radius]);
		}
		triangulated_face->shift(sphere.p);
		std::list<Index> this_border;
		if (face->edge.size() > 0)
		{
			std::vector< TPlane3<T> > planes;
			for (Position i = 0; i < face->edge.size(); i++)
			{
				if (face->edge[i] != NULL)
				{
					TPlane3<T> plane(face->edge[i]->circle.p,face->edge[i]->circle.n);
					if (face->orientation[i] == 1)
					{
						plane.n.negate();
					}
					planes.push_back(plane);
					//this_border.push_back(face->edge[i]);
					this_border.push_back(face->edge[i]->index);
				}
			}
			triangulated_face->cut(planes);
		}
		triangulated_faces.push_back(triangulated_face);
		border.push_back(this_border);
				#ifdef debug_triangulation
				Molecule* molecule = new Molecule;
				std::list<TTrianglePoint<T>*>::iterator p;
				for (p = triangulated_face->points.begin();
						 p != triangulated_face->points.end(); p++)
				{
					Atom* atom = new Atom;
					atom->setPosition((*p)->p);
					molecule->insert(*atom);
				}
				System* system = new System;
				system->insert(*molecule);
				HINFile file(filename,ios::out);
				file << *system;
				file.close();
				delete system;
				#endif
	}


	template <class T>
	void SortContour
		(std::list<TTriangleEdge<T>*>& edges,
		 std::list<TTrianglePoint<T>*>& contour,
		 bool free, const T& /*Wozu braucht man das?*/)
	{
		if (edges.size() == 0)
		{
			return;
		}
		std::list<TTriangleEdge<T>*> sorted_edges;
		TTriangleEdge<T>* start_edge = edges.front();
		TTrianglePoint<T>* start_point = start_edge->point[0];
		edges.remove(start_edge);
		TTriangleEdge<T>* last_edge = start_edge;
		TTrianglePoint<T>* last_point = start_point;
		sorted_edges.push_back(last_edge);
		contour.push_back(last_point);
		std::list<TTriangleEdge<T>*>::iterator e;
		bool found = true;
		while ((edges.size() > 0) && found)
		{
			found = false;
			e = edges.begin();
			while (e != edges.end())
			{
				if ((*e)->point[0] == last_point)
				{
					last_edge = *e;
					last_point = last_edge->point[1];
					found = true;
					sorted_edges.push_back(last_edge);
					contour.push_back(last_point);
					edges.erase(e);
					e = edges.end();
				}
				else
				{
					if ((*e)->point[1] == last_point)
					{
						last_edge = *e;
						last_point = last_edge->point[0];
						found = true;
						sorted_edges.push_back(last_edge);
						contour.push_back(last_point);
						edges.erase(e);
						e = edges.end();
					}
					else
					{
						e++;
					}
				}
			}
		}
		if (free == false)
		{
			last_edge = start_edge;
			last_point = last_edge->point[1];
			contour.push_front(last_point);
			found = true;
			while ((edges.size() > 0) && found)
			{
				found = false;
				std::list<TTriangleEdge<T>*>::iterator e
						= edges.begin();
				while (e != edges.end())
				{
					if ((*e)->point[0] == last_point)
					{
						last_edge = *e;
						last_point = last_edge->point[1];
						found = true;
						sorted_edges.push_front(last_edge);
						contour.push_front(last_point);
						edges.erase(e);
						e = edges.end();
					}
					else
					{
						if ((*e)->point[1] == last_point)
						{
							last_edge = *e;
							last_point = last_edge->point[0];
							found = true;
							sorted_edges.push_front(last_edge);
							contour.push_front(last_point);
							edges.erase(e);
							e = edges.end();
						}
						else
						{
							e++;
						}
					}
				}
			}
		}
		edges = sorted_edges;
	}


	template <class T>
	vector< TVector3<T> > PartitionOfCircle
			(const TCircle3<T>& circle, const TVector3<T>& p0,
			 const TAngle<T>& phi, Size number_of_segments)
	{
		TVector4<T> p;
		if (circle.has(p0,true))
		{
			TVector3<T> p_(p0-circle.p);
			p.set(p_.x,p_.y,p_.z,(T)0);
		}
		else
		{
			p.set(circle.n.y,-circle.n.x,(T)0,(T)0);
			if (p == TVector4<T>::getZero())
			{
				p.set(circle.n.z,(T)0,-circle.n.x,(T)0);
			}
			p.normalize();
			p *= circle.radius;
		}
		TQuaternion<T> rotate(circle.n,phi);
		TMatrix4x4<T> rotation;
		rotate.getRotationMatrix(rotation);
		vector< TVector3<T> > back;
		back.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
		for (Size i = 0; i < number_of_segments; i++)
		{
			p = rotation*p;
			back.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
		}
		return back;
	}


	template <class T>
	void SewFace
		(std::list<Index> border,
		 std::list< TTrianglePoint<T>* > face_points,
		 std::vector
		 	< std::list
		 			<TTriangleEdge<T>*>
		 	> edge_contours,
		 const vector<TSESEdge<T>*>& sesedge,
		 const TSphere3<T>& sphere, bool convex,
		 TTriangulatedSurface<T>* surface,
		 Index nummer, const T& /*Wozu braucht man das?*/)
	{
		std::cout << "    ... " << nummer << "\n";
		if (border.size() == 0)
		{
			return;
		}
		std::list<TTriangleEdge<T>*> edges;
		HashSet<TTrianglePoint<T>*> points;
		std::list<Index>::iterator b;
		//bool empty = (face_points.size() == 0);
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "i: ";
				#endif
		std::list<TTrianglePoint<T>*>::iterator p_;
		for (p_ = face_points.begin(); p_ != face_points.end(); p_++)
		{
			(*p_)->state = 0;
			points.insert(*p_);
					#ifdef print_debug_info
					std::cout << (*p_)->index << " ";
					#endif
		}
				#ifdef print_debug_info
				std::cout << "\n";
				#endif
		for (b = border.begin(); b != border.end(); b++)
		{
			std::list<TTriangleEdge<T>*>::iterator e;
			for (e = edge_contours[*b].begin();
					 e != edge_contours[*b].end(); e++)
			{
				(*e)->point[0]->state = 0;
				(*e)->point[1]->state = 0;
			}
		}
		Index state = 1;
		for (b = border.begin(); b != border.end(); b++)
		{
			HashSet<TTrianglePoint<T>*> this_points;
			std::list<TTriangleEdge<T>*>::iterator e;
					#ifdef print_debug_info
					std::cout << pre << "a:";
					#endif
			for (e = edge_contours[*b].begin(); e != edge_contours[*b].end(); e++)
			{
				if (sesedge[*b]->type != 2)
				{
					edges.push_back(*e);
				}
				this_points.insert((*e)->point[0]);
				this_points.insert((*e)->point[1]);
						#ifdef print_debug_info
						std::cout << (*e)->point[0]->index << "-"
								 << (*e)->point[1]->index << " ";
					#endif
			}
					#ifdef print_debug_info
					std::cout << "\n";
					#endif
			HashSet<TTrianglePoint<T>*>::Iterator p;
			for (p = this_points.begin(); p != this_points.end(); p++)
			{
				if (sesedge[*b]->type != 2)
				{
					(*p)->state += state;
				}
				else
				{
					(*p)->state -= state;
				}
				points.insert(*p);
				if (Maths::isNotEqual(sphere.p.getDistance((*p)->p),
															sphere.radius))
				{
					TVector3<T> diff((*p)->p-sphere.p);
					diff.normalize();
					diff *= sphere.radius;
					(*p)->p = sphere.p+diff;
							#ifdef print_debug_info
							std::cout << (*p)->index << " put to sphere\n";
							#endif
				}
			}
			if (sesedge[*b]->type != 2)
			{
				state *= 2;
			}
		}
				#ifdef print_debug_info
				HashSet<TTrianglePoint<T>*>::Iterator h;
				for (h = points.begin(); h != points.end(); h++)
				{
					std::cout << pre << (*h)->index << "  " << (*h)->state << "\n";
				}
				#endif
		//if (empty)
		//{
		//	return;
		//}
				#ifdef debug_triangulation
				Molecule* molecule = new Molecule;
				#endif
		b = border.begin();
		state = 1;
				#ifdef print_debug_info
				std::cout << "new state: " << state << "\n";
				int halt = (((nummer == 31) || (nummer == 37)) ? 0 : -1);
				//int halt = 0;
				#endif
		while (edges.size() > 0)
		{
			TTriangleEdge<T>* edge = edges.front();
					#ifdef print_debug_info
					std::cout << pre << "derzeit " << edges.size() << " Edges";
					if (halt == 0) cin >> halt; else { std::cout << "\n"; halt--; }
			  	std::cout << pre << "gepopt\n";
			  	#endif
			edges.pop_front();
			if (b != border.end())
			{
				if (edge_contours[*b].size() == 0)
				{
					b++;
					bool found = false;
					while (found == false)
					{
						if (b == border.end())
						{
							found = true;
						}
						else
						{
							if (sesedge[*b]->type != 2)
							{
								found = true;
							}
							else
							{
								b++;
							}
						}
					}
					if (b != border.end())
					{
						state *= 2;
						edge_contours[*b].remove(edge);
					}
					else
					{
						state = 0;
					}
							#ifdef print_debug_info
							std::cout << pre << "######  new state: " << state << "\n";
							#endif
				}
				else
				{
					edge_contours[*b].remove(edge);
				}
			}
			TTrianglePoint<T>* third_point;
			if (edge->triangle.size() > 0)
			{
				third_point = edge->triangle[0]->third(edge->point[0],edge->point[1]);
			}
			else
			{
				third_point = edge->point[0];
			}
			std::list<TTrianglePoint<T>*> third;
					#ifdef print_debug_info
					std::cout << pre << "starte mit Edge "; edge->print();
					std::cout << " (" << edge << ")\n";
					std::cout << pre << "third point: " << third_point->index << " ";
					if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			HashSet<TTrianglePoint<T>*>::Iterator next = points.begin();
			HashSet<TTrianglePoint<T>*>::Iterator test;
			while (next != points.end())
			{
				if ((*next == edge->point[0]) || (*next == edge->point[1]) ||
						(*next == third_point))
				{
							#ifdef print_debug_info
							std::cout << pre << "dritter Punkt: " << (*next)->index << " --\n";
							#endif
					next++;
				}
				else
				{
					bool same_edge = false;
					if (state > 0)
					{
						if (((*next)->state & state) != 0)
						{
							same_edge = true;
						}
					}
					if (same_edge)
					{
								#ifdef print_debug_info
								std::cout << pre << "dritter Punkt: " << (*next)->index << " --\n";
								#endif
						next++;
					}
					else
					{
						TVector3<T> normal((edge->point[0]->p-edge->point[1]->p) %
															 (edge->point[0]->p-(*next)->p)		      );
						T test_value = normal*edge->point[0]->p;
						if (normal*sphere.p > test_value)
						{
							normal.negate();
							test_value = -test_value;
						}
								#ifdef print_debug_info
								std::cout << pre << "dritter Punkt: " << (*next)->index
										 << " (" << test_value << ") " << normal << " ";
								if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						bool is_convex = true;
						test = points.begin();
								#ifdef print_debug_info
								std::cout << pre << "teste mit ...\n";
								#endif
						while ((test != points.end()) && is_convex)
						{
									#ifdef print_debug_info
									std::cout << pre << "... " << (*test)->index << " :  ";
									#endif
							if (test == next)
							{
										#ifdef print_debug_info
										std::cout << "--\n";
										#endif
								//third.push_back(*test);
								//if (pushed == false)
								//{
								//	last = --third.end();
								//	pushed = true;
								//}
								test++;
							}
							else
							{
								T this_value = normal*(*test)->p;
								if (Maths::isLessOrEqual(this_value,test_value))
								{
									test++;
											#ifdef print_debug_info
											std::cout << "ok (" << this_value << ")\n";
											#endif
								}
								else
								{
									if (Maths::isGreater(this_value,test_value))
									{
										same_edge = false;
										if (state > 0)
										{
											if (((*test)->state & state) != 0)
											{
												same_edge = true;
											}
										}
										if (same_edge == false)
										{
													#ifdef print_debug_info
													std::cout << "not ok (" << this_value << ")\n";
													#endif
											is_convex = false;
											//if (pushed == true)
											//{
											//	third.erase(last,third.end());
											//	pushed = false;
											//}
										}
										else
										{
													#ifdef print_debug_info
													std::cout << "ok (gleiche SESEdge)\n";
													#endif
											test++;
										}
									}
									/*else
									{
										if (points.has(*test)									&&
												((*test)->p != third_point->p)		&&
												((*test)->p != edge->point[0]->p) &&
												((*test)->p != edge->point[1]->p)    )
										{
													#ifdef print_debug_info
													std::cout << "added (" << this_value << ")";
													#endif
											third.push_back(*test);
											if (pushed == false)
											{
												last = --third.end();
												pushed = true;
											}
										}
												#ifdef print_debug_info
												std::cout << "\n";
												#endif
										test++;
									}*/
								}
							}
						}
						if (is_convex)
						{
							third.push_back(*next);
									#ifdef print_debug_info
									std::cout << "added ()\n";
									#endif
						}
						next++;
					}
				}
			}
			switch (third.size())
			{
				case 0 :
									#ifdef print_debug_info
									std::cout << pre << "no convex triangle found\n";
									#endif
									break;
				case 1 :
									#ifdef debug_triangulation
									BuildUnambiguousTriangle(edge,third.front(),edges,border,
																					 edge_contours,sphere,convex,surface,
																					 molecule,halt);
									#else
									BuildUnambiguousTriangle(edge,third.front(),edges,border,
																					 edge_contours,sphere,convex,surface);
									#endif
									break;
				default :
									#ifdef debug_triangulation
									BuildAmbiguousTriangles(edge,third,edges,border,
																					edge_contours,sphere,convex,surface,
																					molecule,halt);
									#else
									BuildAmbiguousTriangles(edge,third,edges,border,
																					edge_contours,sphere,convex,surface);
									#endif
									break;
			}
		}
				#ifdef debug_triangulation
				System* system = new System;
				system->insert(*molecule);
				HINFile output("hin/molecule/border"+IndexToString(nummer,(T)0)+".hin",
											 ios::out);
				output << *system;
				output.close();
				delete system;
				#endif
	}
	
	
	template <class T>
	void BuildUnambiguousTriangle
		(TTriangleEdge<T>* edge,
		 TTrianglePoint<T>* point,
		 std::list<TTriangleEdge<T>*>& edges,
		 std::list<Index>& border,
		 std::vector
		 	< std::list
		 			<TTriangleEdge<T>*>
		 	>& edge_contours,
		 const TSphere3<T>& sphere,
		 bool convex,
		 TTriangulatedSurface<T>* surface
				#ifdef debug_triangulation
				, Molecule* molecule, int& halt
				#endif
		)
	{
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "eindeutiges Dreieck\n";
				#endif
		TTriangleEdge<T>* edge1;
		TTriangleEdge<T>* edge2;
		TTriangle<T>* triangle;
		bool old1, old2;
		#ifdef debug_triangulation
		CreateTriangleAndEdges(edge,point,sphere,convex,molecule,
													 edge1,old1,edge2,old2,triangle);
		#else
		CreateTriangleAndEdges(edge,point,sphere,convex,
													 edge1,old1,edge2,old2,triangle);
		#endif
		std::list<Index>::iterator b;
		if (old1 == true)
		{
			edges.remove(edge1);
					#ifdef print_debug_info
					std::cout << pre << "remove Edge "; edge1->print();
					std::cout << " (" << edge1 << ") from\n" << pre << "  edges\n";
					#endif
			for (b = border.begin(); b != border.end(); b++)
			{
				edge_contours[*b].remove(edge1);
						#ifdef print_debug_info
						std::cout << pre << "  edge_contours[" << *b << "]\n";
						#endif
			}
		}
		else
		{
			edge1->point[0]->edge.push_back(edge1);
			edge1->point[1]->edge.push_back(edge1);
			edge1->index = surface->edges.size();
			surface->edges.push_back(edge1);
			edges.push_back(edge1);
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge "; edge1->print();
					std::cout << " (" << edge1 << ") to\n" << pre << "  surface\n" << pre << "  edges\n";
					#endif
		}
		if (old2 == true)
		{
			edges.remove(edge2);
					#ifdef print_debug_info
					std::cout << pre << "remove Edge "; edge2->print();
					std::cout << " (" << edge2 << ") from\n" << pre << "  edges\n";
					#endif
			for (b = border.begin(); b != border.end(); b++)
			{
				edge_contours[*b].remove(edge2);
						#ifdef print_debug_info
						std::cout << pre << "  edge_contours[" << *b << "]\n";
						#endif
			}
		}
		else
		{
			edge2->point[0]->edge.push_back(edge2);
			edge2->point[1]->edge.push_back(edge2);
			edge2->index = surface->edges.size();
			surface->edges.push_back(edge2);
			edges.push_back(edge2);
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge "; edge2->print();
					std::cout << " (" << edge2 << ") to\n" << pre << "  surface\n" << pre << "  edges\n";
					#endif
		}
		edge->triangle.push_back(triangle);
		edge1->triangle.push_back(triangle);
		edge2->triangle.push_back(triangle);
		triangle->point[0]->triangle.push_back(triangle);
		triangle->point[1]->triangle.push_back(triangle);
		triangle->point[2]->triangle.push_back(triangle);
								#ifdef debug_triangulation
								Atom* atom1 = new Atom;
								Atom* atom2 = new Atom;
								Atom* atom3 = new Atom;
								atom1->setPosition(triangle->point[0]->p);
								atom2->setPosition(triangle->point[1]->p);
								atom3->setPosition(triangle->point[2]->p);
								molecule->insert(*atom1);
								molecule->insert(*atom2);
								molecule->insert(*atom3);
								#ifdef with_bonds
								atom1->createBond(*atom2);
								atom1->createBond(*atom3);
								atom2->createBond(*atom3);
								#endif
								#endif
		triangle->index = surface->edges.size();
		surface->triangles.push_back(triangle);
					#ifdef print_debug_info
					std::cout << pre << "geupdatet:\n";
					std::cout << pre << "  Edge "; edge->print(); std::cout << " (" << edge << ")\n";
					std::cout << pre << "  Edge "; edge1->print(); std::cout << " (" << edge1 << ")\n";
					std::cout << pre << "  Edge "; edge2->print(); std::cout << " (" << edge2 << ")\n";
					std::cout << pre << "  Point "; triangle->point[0]->print();
							std::cout << " (" << triangle->point[0] << ")\n";
					std::cout << pre << "  Point "; triangle->point[1]->print();
							std::cout << " (" << triangle->point[1] << ")\n";
					std::cout << pre << "  Point "; triangle->point[2]->print();
							std::cout << " (" << triangle->point[2] << ")\n";
					std::cout << pre << "  Triangle "; triangle->print(); std::cout << " (" << triangle << ")\n";
					std::cout << pre << "pushe Triangle "; triangle->print();
					std::cout << " (" << triangle << ") to\n" << pre << "  surface";
					if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
	}



	template <class T>
	void BuildAmbiguousTriangles
		(TTriangleEdge<T>* edge,
		 std::list<TTrianglePoint<T>*> points,
		 std::list<TTriangleEdge<T>*>& edges,
		 std::list<Index>& border,
		 std::vector
		 	< std::list
		 			<TTriangleEdge<T>*>
		 	>& edge_contours,
		 const TSphere3<T>& sphere,
		 bool convex,
		 TTriangulatedSurface<T>* surface
				#ifdef debug_triangulation
				, Molecule* molecule, int & halt
				#endif
		)
	{
		if (edge->triangle.size() > 0)
		{
			TPlane3<T> plane(edge->point[0]->p,edge->point[1]->p,sphere.p);
			T test_value = plane.p*plane.n;
			TVector3<T> third(edge->triangle[0]->third(edge->point[0],
																								 edge->point[1] )->p);
			std::list<TTrianglePoint<T>*>::iterator p;
			if (Maths::isGreater(plane.n*third,test_value))
			{
				p = points.begin();
				while (p != points.end())
				{
					std::list<TTrianglePoint<T>*>::iterator next = p;
					next++;
					if (Maths::isGreater(plane.n*(*p)->p,test_value))
					{
						points.erase(p);
					}
					p = next;
				}
			}
			else
			{
				p = points.begin();
				while (p != points.end())
				{
					std::list<TTrianglePoint<T>*>::iterator next = p;
					next++;
					if (Maths::isLess(plane.n*(*p)->p,test_value))
					{
						points.erase(p);
					}
					p = next;
				}
			}
		}
		/*TPlane3<T> plane(edge->point[0]->p,edge->point[1]->p,points.front()->p);
		std::list<TTrianglePoint<T>*>::iterator p;
		bool remove = false;
		for (p = ++points.begin(); p != points.end(); p++)
		{
			if (plane.has((*p)->p) == false)
			{
				remove = true;
			}
		}
		if (remove)
		{
			for (p = points.begin(); p != points.end(); p++)
			{
				std::list<TTrianglePoint<T>*>::iterator next = p;
				next++;
				if ((*p)->state != 0)
				{
					points.erase(p);
				}
				p = next;
			}
		}*/
		if (points.size() == 1)
		{
			#ifdef debug_triangulation
			BuildUnambiguousTriangle(edge,points.front(),edges,border,
															 edge_contours,sphere,convex,surface,
															 molecule,halt);
			#else
			BuildUnambiguousTriangle(edge,points.front(),edges,border,
															 edge_contours,sphere,convex,surface);
			#endif
			return;
		}
		points.push_back(edge->point[0]);
		points.push_back(edge->point[1]);
				#ifdef print_debug_info
				string pre = "        ";
				std::cout << pre << points.size() << " Punkte in einer Ebene ... ";
				if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		std::list<TTriangleEdge<T>*> planar_edges;
		planar_edges.push_back(edge);
		TTriangleEdge<T>* edge0;
		TTriangleEdge<T>* edge1;
		TTriangleEdge<T>* edge2;
		TTriangle<T>* triangle;
		bool old1, old2;
		while (planar_edges.size() > 0)
		{
			edge0 = planar_edges.front();
					#ifdef print_debug_info
					std::cout << pre << "suche nach Dreieck für Edge "; edge0->print(); std::cout << " (" << edge0 << ") ";
					if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			planar_edges.pop_front();
			std::list<TTrianglePoint<T>*>::iterator p;
			p = points.begin();
			bool built = false;
			while ((p != points.end()) && (built == false))
			{
				if ((*p == edge0->point[0]) || (*p == edge0->point[1]))
				{
					p++;
				}
				else
				{
							#ifdef print_debug_info
							std::cout << pre << "baue Dreieck mit Punkt "; (*p)->print(); std::cout << " (" << *p << ") ";
							if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					#ifdef debug_triangulation
					CreateTriangleAndEdges(edge0,*p,sphere,convex,molecule,
																 edge1,old1,edge2,old2,triangle);
					#else
					CreateTriangleAndEdges(edge0,*p,sphere,convex,
																 edge1,old1,edge2,old2,triangle);
					#endif
							#ifdef print_debug_info
							std::cout << pre << "teste dieses Dreieck: ";
							if (halt == 0) cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					if (edge0 == edge)
					{
						built = true;
					}
					else
					{
						TTriangle<T>* old_triangle
							= edge0->triangle[0];
						TTrianglePoint<T>* third
							= old_triangle->third(edge0->point[0],edge0->point[1]);
								#ifdef print_debug_info
								std::cout << pre << "  teste gegen Dreieck "; old_triangle->print();
								std::cout << " (" << old_triangle << ") ";
								if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						TVector3<T> orth1((edge0->point[0]->p-edge0->point[1]->p)%
															(edge0->point[0]->p-third->p));
						TVector3<T> orth2((edge0->point[0]->p-edge0->point[1]->p)%
															(edge0->point[0]->p-(*p)->p));
						orth1.normalize();
						orth2.normalize();
						TAngle<T> pi_2(Constants::PI/2.0,true);
						TAngle<T> phi;
								#ifdef print_debug_info
								std::cout << pre << orth1 << "\n" << pre << orth2 << "\n" << pre << phi << "\n";
								#endif
						phi = orth1.getAngle(orth2);
								#ifdef print_debug_info
								std::cout << pre << orth1 << "\n" << pre << orth2 << "\n" << pre << phi << " ";
								if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						if (phi > pi_2)
						{
							built = true;
						}
					}
					#ifdef print_debug_info	
					std::cout << pre << (built ? "ok " : "nok ") << edges.size() << " " << planar_edges.size() << " ";
					if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
					if (built)
					{
						edge0->triangle.push_back(triangle);
						edge1->triangle.push_back(triangle);
						edge2->triangle.push_back(triangle);
						triangle->point[0]->triangle.push_back(triangle);
						triangle->point[1]->triangle.push_back(triangle);
						triangle->point[2]->triangle.push_back(triangle);
								#ifdef debug_triangulation
								Atom* atom1 = new Atom;
								Atom* atom2 = new Atom;
								Atom* atom3 = new Atom;
								atom1->setPosition(triangle->point[0]->p);
								atom2->setPosition(triangle->point[1]->p);
								atom3->setPosition(triangle->point[2]->p);
								molecule->insert(*atom1);
								molecule->insert(*atom2);
								molecule->insert(*atom3);
								#ifdef with_bonds
								atom1->createBond(*atom2);
								atom1->createBond(*atom3);
								atom2->createBond(*atom3);
								#endif
								#endif
						std::list<Index>::const_iterator b;
						if (old1 == true)
						{
									#ifdef print_debug_info
									std::cout << pre << "remove Edge "; edge1->print();
									std::cout << " (" << edge1 << ") from\n" << pre << "  edges\n";
									#endif
							edges.remove(edge1);
							planar_edges.remove(edge1);
							for (b = border.begin(); b != border.end(); b++)
							{
								edge_contours[*b].remove(edge1);
										#ifdef print_debug_info
										std::cout << pre << "  edge_contours[" << *b << "]\n";
										#endif
							}
						}
						else
						{
							//edges.push_back(edge1);
							planar_edges.push_back(edge1);
							edge1->point[0]->edge.push_back(edge1);
							edge1->point[1]->edge.push_back(edge1);
							edge1->index = surface->edges.size();
							surface->edges.push_back(edge1);
									#ifdef print_debug_info
									std::cout << pre << "pushe Edge "; edge1->print();
									std::cout << " (" << edge1 << ") to\n" << pre << "  planar_edges\n" << pre << "  surface\n";
									#endif
						}
						if (old2 == true)
						{
									#ifdef print_debug_info
									std::cout << pre << "remove Edge "; edge2->print();
									std::cout << " (" << edge2 << ") from\n" << pre << "  edges\n";
									#endif
							edges.remove(edge2);
							planar_edges.remove(edge2);
							for (b = border.begin(); b != border.end(); b++)
							{
								edge_contours[*b].remove(edge2);
										#ifdef print_debug_info
										std::cout << pre << "  edge_contours[" << *b << "]\n";
										#endif
							}
						}
						else
						{
							//edges.push_back(edge2);
							planar_edges.push_back(edge2);
							edge2->point[0]->edge.push_back(edge2);
							edge2->point[1]->edge.push_back(edge2);
							edge2->index = surface->edges.size();
							surface->edges.push_back(edge2);
									#ifdef print_debug_info
									std::cout << pre << "pushe Edge "; edge2->print();
									std::cout << " (" << edge2 << ") to\n" << pre << "  planar_edges\n" << pre << "  surface\n";
									#endif
						}
						edges.remove(edge0);
						triangle->index = surface->triangles.size();
						surface->triangles.push_back(triangle);
								#ifdef print_debug_info
								std::cout << pre << "remove Edge"; edge0->print();
								std::cout << " (" << edge0 << ") from\n" << pre << "  edges\n";
								std::cout << pre << "pushe Triangle "; triangle->print();
								std::cout << " (" << triangle << ") to\n" << pre << "  surface";
								if (halt == 0)  cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
					}
					else
					{
						p++;
								#ifdef print_debug_info
								std::cout << pre << "zerstöre Triangle "; triangle->print(); std::cout << " (" << triangle << ")\n";
								#endif
						delete triangle;
						if (old1 == false)
						{
									#ifdef print_debug_info
									std::cout << pre << "zerstöre Edge "; edge1->print(); std::cout << " (" << edge1 << ")\n";
									#endif
							delete edge1;
						}
						if (old2 == false)
						{
									#ifdef print_debug_info
									std::cout << pre << "zerstöre Edge "; edge2->print(); std::cout << " (" << edge2 << ")\n";
									#endif
							delete edge2;
						}
					}
				}
			}
		}
	}



	template <class T>
	void CreateTriangleAndEdges
		(TTriangleEdge<T>* edge, TTrianglePoint<T>* point,
		 const TSphere3<T>& sphere, bool convex,
				#ifdef debug_triangulation
				Molecule* /*molecule*/,
				#endif
		 TTriangleEdge<T>*& edge1, bool& old1,
		 TTriangleEdge<T>*& edge2, bool& old2,
		 TTriangle<T>*& triangle
		)
	{
		// build two new edges
		edge1	= new TTriangleEdge<T>;
		edge1->point[0] = edge->point[0];
		edge1->point[1] = point;
		edge1->triangle.clear();
		edge2 = new TTriangleEdge<T>;
		edge2->point[0] = point;
		edge2->point[1] = edge->point[1];
		edge2->triangle.clear();
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "zwei Edges konstruiert\n"
						 << pre << "  "; edge1->print(); std::cout << " (" << edge1 << ")\n"
						 << pre << "  "; edge2->print(); std::cout << " (" << edge2 << ")\n";
				#endif
		// do the edges exist
		//std::cout << "############\n";
		//edge->print(); std::cout << "\n";
		//edge->point[0]->print(); std::cout << "\n";
		TTriangleEdge<T>* test = edge->point[0]->has(edge1);
		//std::cout << "************\n";
		if (test != NULL)
		{
					#ifdef print_debug_info
					std::cout << pre << "erste existiert schon: "; test->print(); std::cout << " (" << test << ")\n";
					std::cout << pre << "  zerstöre Edge " << edge1 << "\n";
					#endif
			delete edge1;
			edge1 = test;
			old1 = true;
		}
		else
		{
			old1 = false;
		}
		//std::cout << "############\n";
		//edge->print(); std::cout << "\n";
		//edge->point[1]->print(); std::cout << "\n";
		test = edge->point[1]->has(edge2);
		//std::cout << "************\n";
		if (test != NULL)
		{
					#ifdef print_debug_info
					std::cout << pre << "zweite existiert schon: "; test->print(); std::cout << " (" << test << ")\n";
					std::cout << pre << "  zerstöre Edge " << edge2 << "\n";
					#endif
			delete edge2;
			edge2 = test;
			old2 = true;
		}
		else
		{
			old2 = false;
		}
		//std::cout << "############\n";
		// build the new triangle
		triangle = new TTriangle<T>;
		triangle->point[0] = edge->point[0];
		triangle->point[1] = edge->point[1];
		triangle->point[2] = point;
		triangle->edge[0] = edge;
		triangle->edge[1] = edge1;
		triangle->edge[2] = edge2;
		// swap triangle if necessary
		TVector3<T> test_vector((triangle->point[1]->p-triangle->point[0]->p)%
														(triangle->point[2]->p-triangle->point[0]->p));
		if (convex)
		{
			if (Maths::isGreater(test_vector*sphere.p,
													 test_vector*triangle->point[0]->p))
			{
				TTrianglePoint<T>* temp = triangle->point[0];
				triangle->point[0] = triangle->point[1];
				triangle->point[1] = temp;
			}
		}
		else
		{
			if (Maths::isLess(test_vector*sphere.p,
												test_vector*triangle->point[0]->p))
			{
				TTrianglePoint<T>* temp = triangle->point[0];
				triangle->point[0] = triangle->point[1];
				triangle->point[1] = temp;
			}
		}
				#ifdef print_debug_info
				std::cout << pre << "Dreieck gebaut: "; triangle->print(); std::cout << " (" << triangle << ")\n";
				#endif
	}


	
	

			// debuging
			template <class T>
			string IndexToString(Index nr, const T& /*dummy*/)
			{
				if (nr == 0) return "0";
				string back = "";
				while (nr != 0)
				{
					Index rest = nr % 10;
					switch (rest)
					{
						case 0 :	back += "0"; break;
						case 1 :	back += "1"; break;
						case 2 :	back += "2"; break;
						case 3 :	back += "3"; break;
						case 4 :	back += "4"; break;
						case 5 :	back += "5"; break;
						case 6 :	back += "6"; break;
						case 7 :	back += "7"; break;
						case 8 :	back += "8"; break;
						case 9 :	back += "9"; break;
					}
					nr = nr / 10;
				}
				string number = "";
				for (Position i = back.size(); i > 0;)
				{
					number += back[--i];
				}
				return number;
			}



	template <class T>
	void SewEmptyContour
			(std::list<TTrianglePoint<T>*>& contour,
			 const TSphere3<T>& sphere, bool convex,
			 TTriangulatedSurface<T>* surface,
			 string filename)
	{
		TVector3<T> point(0,0,0);
		std::list<TTrianglePoint<T>*>::iterator c;
		for (c = contour.begin(); c != contour.end(); c++)
		{
			point += (*c)->p;
		}
				#ifdef debug_triangulation
				Molecule* molecule = new Molecule;
				Atom* atom = new Atom;
				atom->setPosition(point);
				atom->setElement(PTE[Element::O]);
				molecule->insert(*atom);
				atom = new Atom;
				atom->setPosition(sphere.p);
				atom->setElement(PTE[Element::P]);
				molecule->insert(*atom);
				#endif
		point -= (sphere.p*(T)contour.size());
		point.normalize();
		point *= sphere.radius;
		point += sphere.p;
		TTrianglePoint<T>* center
			= new TTrianglePoint<T>;
		center->p = point;
		center->n = (convex ? point-sphere.p : sphere.p-point);
		surface->points.push_back(center);
		c = contour.begin();
		c++;
		TTriangle<T>* t
				= new TTriangle<T>;
		t->point[0] = center;
		t->point[1] = contour.front();
		t->point[2] = *c;
				#ifdef debug_triangulation
				Atom* atom1 = new Atom;
				Atom* atom2 = new Atom;
				Atom* atom3 = new Atom;
				atom1->setPosition(center->p);
				atom2->setPosition(contour.front()->p);
				atom3->setPosition((*c)->p);
				#ifdef with_bonds
				atom1->createBond(*atom2);
				atom1->createBond(*atom3);
				atom3->createBond(*atom2);
				#endif
				molecule->insert(*atom1);
				molecule->insert(*atom2);
				molecule->insert(*atom3);
				#endif
		surface->triangles.push_back(t);
		while (c != --contour.end())
		{
			t = new TTriangle<T>;
			t->point[0] = center;
			t->point[1] = *c;
					#ifdef debug_triangulation
					Atom* atom1 = new Atom;
					Atom* atom2 = new Atom;
					Atom* atom3 = new Atom;
					atom1->setPosition(center->p);
					atom2->setPosition((*c)->p);
					#endif
			c++;
			t->point[2] = *c;
					#ifdef debug_triangulation
					atom3->setPosition((*c)->p);
					#ifdef with_bond
					atom1->createBond(*atom2);
					atom1->createBond(*atom3);
					atom3->createBond(*atom2);
					#endif
					molecule->insert(*atom1);
					molecule->insert(*atom2);
					molecule->insert(*atom3);
					#endif
			surface->triangles.push_back(t);
		}
				#ifdef debug_triangulation
				System* system = new System;
				system->insert(*molecule);
				HINFile output(filename,ios::out);
				output << *system;
				output.close();
				delete system;
				#endif
	}



	/********** BUILDTRIANGLES **************************************************/

	template <class T>
	bool BuildTriangles
		 (TSESFace<T>* /*face*/,
			TSESEdge<T>* edge0, TSESEdge<T>* edge1,
			TSESEdge<T>* edge2, TSESEdge<T>* edge3,
			const std::vector< TVector3<T> >& centers,
			const std::vector< TVector3<T> >& edge1_points,
			const std::vector< TVector3<T> >& edge3_points,
			std::vector<TTrianglePoint<T>*> existing_points,
			const T& radius_of_probe, const T& sqrt_density,
			TTriangulatedSurface<T>* surface,
			std::vector< std::list<TTriangleEdge<T>*> >& border
			#ifdef debug_triangulation
			, string filename
			#endif
			)
	{
				//cout << "%1\n";
		TAngle<T> pi(Constants::PI,true);
		TAngle<T> psi;
		psi = getOrientedAngle(edge1_points[0]-centers[0],
													 edge3_points[0]-centers[0],
													 (edge1_points[0]-centers[0])%
													 (edge3_points[0]-centers[0])) + pi;
		Size number_of_triangles
			= (Size)Maths::round(psi.toRadian()*radius_of_probe*sqrt_density);
		if (number_of_triangles == 0)
		{
			number_of_triangles++;
		}
		TAngle<T> phi(psi/number_of_triangles,true);
		Size number_of_segments = centers.size()-1;
		Size number_of_points
			= ((edge3 != NULL) ? (number_of_segments+1)*(number_of_triangles+1)
												 : (number_of_segments+1)* number_of_triangles +1 );
		std::vector<TTrianglePoint<T>*> points(number_of_points);
				#ifdef debug_triangulation
				Molecule* molecule = new Molecule;
				vector<Atom*> atom(number_of_points);
				#endif
				//cout << "%2\n";
		if (edge3 == NULL)
		{
			Index index;
			if (edge0->vertex1->p == edge3_points[0])
			{
				index = edge0->vertex1->index;
			}
			else
			{
				index = edge0->vertex2->index;
			}
			TTrianglePoint<T>* top
				= existing_points[index];
			top->n = centers[centers.size()-1]-top->p;
			top->n.normalize();
			top->index = (number_of_segments+1)*number_of_triangles;
			points[(number_of_segments+1)*number_of_triangles] = top;
					#ifdef debug_triangulation
					atom[(number_of_segments+1)*number_of_triangles] = new Atom;
					atom[(number_of_segments+1)*number_of_triangles]->setPosition(top->p);
					molecule->insert(*atom[(number_of_segments+1)*number_of_triangles]);
					#endif
		}
				//cout << "%3\n";
		Position end = ((edge3 != NULL) ? number_of_triangles+1 : number_of_triangles);
				//cout << "number_of_triangles: " << number_of_triangles << "\n";
				//cout << "number_of_segments:  " << number_of_segments << "\n";
				//cout << "end:                 " << end << "\n";
		for (Position i = 0; i < centers.size(); i++)
		{
				//cout << "%3a\n";
			TCircle3<T> circle
				(centers[i],
				 (edge1_points[i]-centers[i])%(edge3_points[i]-centers[i]),
				 radius_of_probe);
				//cout << "%3b\n";
			vector< TVector3<T> > line;
				//cout << "%3c\n";
			line = PartitionOfCircle(circle,edge1_points[i],phi,number_of_triangles);
				//cout << "%4\n";
			for (Position j = 0; j < end; j++)
			{
				//cout << "%4a\n";
				Index index = -1;
				//cout << "%4b\n";
				if (edge0 != NULL)
				{
				//cout << "%4c\n";
					if ((i == 0) && (j == 0))
					{
						index = ((line[0] == edge0->vertex1->p) ? edge0->vertex1->index
																										: edge0->vertex2->index);
					}
				//cout << "%4d\n";
					if ((i == 0) && (j == end-1) && (edge3 != NULL))
					{
						index
							= ((line[end-1] == edge0->vertex1->p) ? edge0->vertex1->index
																										: edge0->vertex2->index);
					}
				//cout << "%4e\n";
					if ((i == centers.size()-1) && (j == 0))
					{
						index = ((line[0] == edge2->vertex1->p) ? edge2->vertex1->index
																										: edge2->vertex2->index);
					}
				//cout << "%4f\n";
					if ((i == centers.size()-1) && (j == end-1) && (edge3 != NULL))
					{
						index
							= ((line[end-1] == edge2->vertex1->p) ? edge2->vertex1->index
																										: edge2->vertex2->index);
					}
				//cout << "%4g\n";
				}
				//cout << "%4h\n";
				TTrianglePoint<T>* point;
				if (index == -1)
				{
					point = new TTrianglePoint<T>;
					point->p = line[j];
					point->n = centers[i]-line[j];
				}
				else
				{
					point = existing_points[index];
				}
				//cout << "%4i\n";
				point->index = i*end+j;
				points[i*end+j] = point;
						#ifdef debug_triangulation
						atom[i*end+j] = new Atom;
						atom[i*end+j]->setPosition(point->p);
						atom[i*end+j]->setElement(PTE[Element::H]);
						molecule->insert(*atom[i*end+j]);
						#endif
				//cout << "%4j\n";
			}
				//cout << "%5\n";
		}
				#ifdef debug_triangulation
				if (edge3 == NULL)
				{
					atom[points.size()-1]->setElement(PTE[Element::O]);
				}
				#endif
				//cout << "%5a\n";
		Size edge_number
				= ((edge3 != NULL) ? 3*number_of_segments*number_of_triangles +
														 number_of_segments+number_of_triangles
													 : 3*number_of_segments*number_of_triangles-
													 	 2*number_of_segments+number_of_triangles-1	 );
		std::vector<TTriangleEdge<T>*> edges(edge_number);
		for (Position j = 0; j < number_of_segments+1; j++)
		{
			Position offset = j*end;
			for (Position i = 0; i < end-1; i++)
			{
				edges[offset+i-j] = new TTriangleEdge<T>;
				edges[offset+i-j]->point[0] = points[offset+i];
				edges[offset+i-j]->point[1] = points[offset+i+1];
				edges[offset+i-j]->point[0]->edge.push_back(edges[offset+i-j]);
				edges[offset+i-j]->point[1]->edge.push_back(edges[offset+i-j]);
				edges[offset+i-j]->triangle.erase(edges[offset+i-j]->triangle.begin(),
																					edges[offset+i-j]->triangle.end());
				if ((j == 0) && (edge0 != 0))
				{
					border[edge0->index].push_back(edges[offset+i-j]);
				}
				else
				{
					if ((j == number_of_segments) && (edge0 != 0))
					{
						border[edge2->index].push_back(edges[offset+i-j]);
					}
				}
			}
		}
				//cout << "%7\n";
		Position add1 = (end-1)*(number_of_segments+1);
		for (Position j = 0; j < number_of_segments; j++)
		{
			Position offset = j*end;
			for (Position i = 0; i < end; i++)
			{
				edges[offset+i+add1] = new TTriangleEdge<T>;
				edges[offset+i+add1]->point[0] = points[offset+i];
				edges[offset+i+add1]->point[1] = points[offset+i+end];
				edges[offset+i+add1]->point[0]->edge.push_back(edges[offset+i+add1]);
				edges[offset+i+add1]->point[1]->edge.push_back(edges[offset+i+add1]);
				edges[offset+i+add1]->triangle.erase(edges[offset+i+add1]->triangle.begin(),
																						 edges[offset+i+add1]->triangle.end());
				if ((i == 0) && (edge1 != 0))
				{
					border[edge1->index].push_back(edges[offset+i+add1]);
				}
				else
				{
					if ((i == end-1) && (edge3 != 0))
					{
						border[edge3->index].push_back(edges[offset+i+add1]);
					}
				}
			}
		}
				//cout << "%8\n";
		Position add2 = add1+end*number_of_segments;
		for (Position j = 0; j < number_of_segments; j++)
		{
			Position offset = j*end-j;
			for (Position i = 0; i < end-1; i++)
			{
				edges[offset+i+add2] = new TTriangleEdge<T>;
				edges[offset+i+add2]->point[0] = points[offset+i+j];
				edges[offset+i+add2]->point[1] = points[offset+i+j+end+1];
				edges[offset+i+add2]->point[0]->edge.push_back(edges[offset+i+add2]);
				edges[offset+i+add2]->point[1]->edge.push_back(edges[offset+i+add2]);
				edges[offset+i+add2]->triangle.erase(edges[offset+i+add2]->triangle.begin(),
																						 edges[offset+i+add2]->triangle.end());
			}
		}
				//cout << "%9\n";
		Size triangle_number
				= ((edge3 != NULL) ? 2*number_of_segments*number_of_triangles
													 : 2*number_of_segments*(number_of_triangles-1));
		std::vector<TTriangle<T>*>	triangles(triangle_number);
		for (Position i = 0; i < end-1; i++)
		{
				//cout << "%10\n";
			for (Position j = 0; j < number_of_segments; j++)
			{
				TTriangle<T>* t1 = new TTriangle<T>;
					t1->point[0] = points[  j  *end+ i ];
					t1->point[1] = points[  j  *end+i+1];
					t1->point[2] = points[(j+1)*end+i+1];
					t1->edge[0] = edges[i+j*(end-1)       ];
					t1->edge[1] = edges[i+j*  end  +add1+1];
					t1->edge[2] = edges[i+j*(end-1)+add2  ];
					points[  j  *end+ i ]->triangle.push_back(t1);
					points[  j  *end+i+1]->triangle.push_back(t1);
					points[(j+1)*end+i+1]->triangle.push_back(t1);
					edges[i+j*(end-1)       ]->triangle.push_back(t1);
					edges[i+j*  end  +add1+1]->triangle.push_back(t1);
					edges[i+j*(end-1)+add2  ]->triangle.push_back(t1);
							#ifdef with_bonds
							atom[  j  *end+ i ]->createBond(*atom[(j+1)*end+i+1]);
							atom[  j  *end+i+1]->createBond(*atom[(j+1)*end+i+1]);
							atom[  j  *end+ i ]->createBond(*atom[  j  *end+i+1]);
							#endif
					t1->index = 2*(number_of_segments*i+j);
					triangles[2*(number_of_segments*i+j)] = t1;
				TTriangle<T>* t2 = new TTriangle<T>;
					t2->point[0] = points[  j  *end+ i ];
					t2->point[1] = points[(j+1)*end+i+1];
					t2->point[2] = points[(j+1)*end+ i ];
					t2->edge[0] = edges[i+(j+1)*(end-1)     ];
					t2->edge[1] = edges[i+  j  *  end  +add1];
					t2->edge[2] = edges[i+  j  *(end-1)+add2];
					points[  j  *end+ i ]->triangle.push_back(t2);
					points[(j+1)*end+i+1]->triangle.push_back(t2);
					points[(j+1)*end+ i ]->triangle.push_back(t2);
					edges[i+(j+1)*(end-1)     ]->triangle.push_back(t2);
					edges[i+  j  *  end  +add1]->triangle.push_back(t2);
					edges[i+  j  *(end-1)+add2]->triangle.push_back(t2);
							#ifdef with_bonds
							atom[  j  *end+ i ]->createBond(*atom[(j+1)*end+i+1]);
							atom[(j+1)*end+ i ]->createBond(*atom[(j+1)*end+i+1]);
							atom[  j  *end+ i ]->createBond(*atom[(j+1)*end+ i ]);
							#endif
					t2->index = 2*(number_of_segments*i+j)+1;
					triangles[2*(number_of_segments*i+j)+1] = t2;
				//cout << "%13\n";
			}
		}
				//cout << "%14\n";
		if (edge0 == NULL)
		{
			Position offset = number_of_segments*end;
			TTriangleEdge<T>* edge = new TTriangleEdge<T>;
				edge->point[0] = points[offset];
				edge->point[1] = points[0];
				edge->triangle.erase(edge->triangle.begin(),edge->triangle.end());
				edges.push_back(edge);
				//cout << "%15\n";
			for (Position i = 0; i < number_of_triangles; i++)
			{
				TTriangle<T>* t1 = new TTriangle<T>;
					t1->point[0] = points[offset+ i ];
					t1->point[1] = points[       i+1];
					t1->point[2] = points[        i ];
					t1->edge[0] = edges[edges.size()-1];
					edge = new TTriangleEdge<T>;
						edge->point[0] = points[offset+i];
						edge->point[1] = points[i+1];
						edge->triangle.erase(edge->triangle.begin(),edge->triangle.end());
						edges.push_back(edge);
					t1->edge[1] = edge;
					t1->edge[2] = edges[i];
					points[offset+ i ]->triangle.push_back(t1);
					points[       i+1]->triangle.push_back(t1);
					points[        i ]->triangle.push_back(t1);
					t1->edge[0]->triangle.push_back(t1);
					t1->edge[1]->triangle.push_back(t1);
					t1->edge[2]->triangle.push_back(t1);
					t1->index = triangles.size();
							#ifdef with_bonds
							atom[offset+ i ]->createBond(*atom[offset+i+1]);
							atom[offset+ i ]->createBond(*atom[       i+1]);
							atom[offset+i+1]->createBond(*atom[       i+1]);
							#endif
					triangles.push_back(t1);
				TTriangle<T>* t2 = new TTriangle<T>;
					t2->point[0] = points[offset+ i ];
					t2->point[1] = points[offset+i+1];
					t2->point[2] = points[       i+1];
					t2->edge[0] = edge;
					t2->edge[1] = edges[offset+i-number_of_segments];
					edge = new TTriangleEdge<T>;
						edge->point[0] = points[offset+i+1];
						edge->point[1] = points[i+1];
						edge->triangle.erase(edge->triangle.begin(),edge->triangle.end());
						edges.push_back(edge);
					t2->edge[2] = edge;
					points[offset+ i ]->triangle.push_back(t2);
					points[offset+i+1]->triangle.push_back(t2);
					points[       i+1]->triangle.push_back(t2);
					t2->edge[0]->triangle.push_back(t2);
					t2->edge[1]->triangle.push_back(t2);
					t2->edge[2]->triangle.push_back(t2);
					t2->index = triangles.size();
							#ifdef with_bonds
							atom[offset+ i ]->createBond(*atom[i+1]);
							atom[offset+ i ]->createBond(*atom[ i ]);
							atom[       i+1]->createBond(*atom[ i ]);
							#endif
					triangles.push_back(t2);
			}
		}
		if (edge3 == NULL)
		{
			Position last = (number_of_segments+1)*number_of_triangles;
			Position offset = (end-1)*(number_of_segments+2);
			TTriangleEdge<T>* edge = new TTriangleEdge<T>;
				edge->point[0] = points[number_of_triangles-1];
				edge->point[1] = points[last];
				edge->triangle.erase(edge->triangle.begin(),edge->triangle.end());
				points[number_of_triangles-1]->edge.push_back(edge);
				points[last]->edge.push_back(edge);
				edges.push_back(edge);
				border[edge0->index].push_back(edge);
			for (Position i = 0; i < number_of_segments; i++)
			{
				TTriangle<T>* t = new TTriangle<T>;
					t->point[0] = points[number_of_triangles*(i+1)-1];
					t->point[1] = points[last];
					t->point[2] = points[number_of_triangles*(i+2)-1];
					t->edge[0] = edges[edges.size()-1];
					t->edge[1] = edges[offset+i*end];
					edge = new TTriangleEdge<T>;
						edge->point[0] = points[number_of_triangles*(i+2)-1];
						edge->point[1] = points[last];
						edge->triangle.erase(edge->triangle.begin(),edge->triangle.end());
						points[number_of_triangles*(i+2)-1]->edge.push_back(edge);
						points[last]->edge.push_back(edge);
						edges.push_back(edge);
						if (i == number_of_segments-1)
						{
							border[edge2->index].push_back(edge);
						}
					t->edge[2] = edge;
					points[number_of_triangles*(i+1)-1]->triangle.push_back(t);
					points[number_of_triangles*(i+2)-1]->triangle.push_back(t);
					points[last]->triangle.push_back(t);
					t->edge[0]->triangle.push_back(t);
					t->edge[1]->triangle.push_back(t);
					t->edge[2]->triangle.push_back(t);
							#ifdef with_bonds
							atom[number_of_triangles*(i+1)-1]->createBond(*atom[number_of_triangles*(i+2)-1]);
							atom[number_of_triangles*(i+1)-1]->createBond(*atom[last]);
							atom[number_of_triangles*(i+2)-1]->createBond(*atom[last]);
							#endif
					t->index = triangles.size();
					triangles.push_back(t);
				//cout << "%23\n";
			}
		}
				//cout << "%24\n";
				#ifdef debug_triangulation
				System* system = new System;
				system->insert(*molecule);
				HINFile output(filename,ios::out);
				output << *system;
				output.close();
				delete system;
				#endif
				//cout << "%25\n";
		for (Position i = 0; i < points.size(); i++)
		{
			surface->points.push_back(points[i]);
		}
		for (Position i = 0; i < edges.size(); i++)
		{
			surface->edges.push_back(edges[i]);
		}
		for (Position i = 0; i < triangles.size(); i++)
		{
			surface->triangles.push_back(triangles[i]);
		}
				//cout << "%26\n";
		return true;
	}






}	// namespace BALL

#endif	// BALL_STRUCTURE_SINGULARITIES_H


