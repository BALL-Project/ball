// $Id: triangulation.h,v 1.24 2001/09/19 17:37:30 strobel Exp $

#ifndef BALL_STRUCTURE_TRIANGULATION_H
#define BALL_STRUCTURE_TRIANGULATION_H

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
#include <BALL/FORMAT/PDBFile.h>
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

		HashMap<Size,TTriangulatedSurface<T>*> spheres;
		BuildSpheres(spheres);
		TTriangulatedSurface<T> probe;
		probe.icosaeder(false);
		probe.refineSphere(numberOfRefinements(density,radius_of_probe),false);
		probe.setIndices();

		std::vector<TTrianglePoint<T>*> point(ses->vertices.size());
		std::vector< std::list<TTriangleEdge<T>*> > edge(ses->edges.size());

		for (Position i = 0;  i < ses->vertices.size(); i++)
		{
			point[i] = new TTrianglePoint<T>;
			point[i]->p = ses->vertices[i]->p;
			point[i]->n = ses->vertices[i]->n;
			surface->points.push_back(point[i]);
		}

		T sqrt_density = sqrt(density);
		TTriangulatedSurface<T>* face_part = new TTriangulatedSurface<T>;
				#ifdef debug_surface_processor
				std::cerr << "  triangulate toric faces\n";
				#endif
		for (Position i = 0; i < ses->toric_faces.size(); i++)
		{
			TriangulateToricFace(ses->toric_faces[i],radius_of_probe,sqrt_density,
													 face_part,point,edge);
						#ifdef debug_triangulation
						TTS2HIN(*face_part,"DATA/MOLECULE/toricFace"+IndexToString(i,0));
						#endif
			for (Position k = 0; k < ses->toric_faces[i]->vertex.size(); k++)
			{
				face_part->points.remove(point[ses->toric_faces[i]->vertex[k]->index]);
			}
			surface->join(*face_part);
		}
				#ifdef debug_surface_processor
				std::cerr << "  treat singular edges\n";
				#endif
		typename std::list<TSESEdge<T>*>::iterator se;
		for (se = ses->singular_edges.begin(); se != ses->singular_edges.end(); se++)
		{
			PartitionSingularEdge(*se,sqrt_density,point,edge,surface);
		}
				#ifdef debug_surface_processor
				std::cerr << "  triangulate contact faces\n";
				#endif
		//double epsilon = Constants::EPSILON;
		TSphere3<T> sphere;
				#ifdef with_indices
				surface->setIndices();
				#endif
		for (Position i = 0; i < ses->contact_faces.size(); i++)
		{
			sphere = rs->getSphere(ses->contact_faces[i]->rsvertex->getAtom());
			TriangulateContactFace(ses->contact_faces[i],sphere,density,radius_of_probe,spheres,edge,surface);
		}
				#ifdef debug_surface_processor
				std::cerr << "  triangulate spheric reentrant faces\n";
				#endif
		sphere.radius = radius_of_probe;
		std::list<Index> spheric_border;
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
		{
			//sphere. p = ses->spheric_faces[i]->rsface->getCenter();
			//GetSpherePoints(probe,sphere,ses->spheric_faces[i],
			//								face_points,spheric_border);
			//Constants::EPSILON = 1e-8;
			//BuildSphericTriangles(face_points,spheric_border,edge,ses->edges,
			//											sphere,false,
			//											face_part,i+ses->contact_faces.size());
			//Constants::EPSILON = epsilon;
			//			#ifdef debug_triangulation
			//			TTS2HIN(*face_part,"DATA/MOLECULE/sphericFace"+IndexToString(i,0));
			//			#endif
			//surface->join(*face_part);
			//face_points.clear();
			//spheric_border.clear();
						#ifdef debug_surface_processor_verbose
						std::cout << "    ... " << i << "\n";
						#endif
			std::list<Position> ecken;
			ecken.push_back(0);
			ecken.push_back(1);
			ecken.push_back(2);
			if (ses->spheric_faces[i]->edge[0] == NULL)
			{
				ecken.remove(0);
				ecken.remove(1);
				ecken.push_back(3);
				ecken.push_back(4);
				if (ses->spheric_faces[i]->edge[1] == NULL)
				{
					ecken.remove(2);
					ecken.push_back(5);
					ecken.push_back(6);
					if (ses->spheric_faces[i]->edge[2] == NULL)
					{
						ecken.push_back(7);
						ecken.push_back(8);
					}
				}
				else
				{
					if (ses->spheric_faces[i]->edge[2] == NULL)
					{
						ecken.remove(2);
						ecken.push_back(5);
						ecken.push_back(6);
					}
				}
			}
			else
			{
				if (ses->spheric_faces[i]->edge[1] == NULL)
				{
					ecken.remove(1);
					ecken.remove(2);
					ecken.push_back(3);
					ecken.push_back(4);
					if (ses->spheric_faces[i]->edge[2] == NULL)
					{
						ecken.remove(0);
						ecken.push_back(5);
						ecken.push_back(6);
					}
				}
				else
				{
					if (ses->spheric_faces[i]->edge[2] == NULL)
					{
						ecken.remove(0);
						ecken.remove(2);
						ecken.push_back(3);
						ecken.push_back(4);
					}
				}
			}
			TVector3<T> mass(0,0,0);
			std::list<Position>::iterator j;
			//for (Position j = 0; j < ses->spheric_faces[i]->vertex.size(); j++)
			for (j = ecken.begin(); j != ecken.end(); j++)
			{
				mass += ses->spheric_faces[i]->vertex[*j]->p;
				//mass += ses->spheric_faces[i]->vertex[j]->p;
			}
			mass -= (ses->spheric_faces[i]->rsface->getCenter()*(T)ecken.size());
			//mass -= (ses->spheric_faces[i]->rsface->getCenter()*(T)ses->spheric_faces[i]->vertex.size());
			mass.normalize();
			mass *= radius_of_probe;
			mass += ses->spheric_faces[i]->rsface->getCenter();
			TTrianglePoint<T>* point = new TTrianglePoint<T>;
			point->p = mass;
			point->n = ses->spheric_faces[i]->rsface->getCenter()-mass;
			TTriangle<T>* triangle;
			for (Position j = 0; j < ses->spheric_faces[i]->edge.size(); j++)
			{
				if (ses->spheric_faces[i]->edge[j] != NULL)
				{
					std::list<TTriangleEdge<T>*>::iterator e;
					for (e = edge[ses->spheric_faces[i]->edge[j]->index].begin();
							 e != edge[ses->spheric_faces[i]->edge[j]->index].end(); e++)
					{
						triangle = new TTriangle<T>;
						TVector3<T> norm(((*e)->point[0]->p-(*e)->point[1]->p)%
														 ((*e)->point[0]->p-point->p));
						if (Maths::isLess(norm*((*e)->point[0]->p-ses->spheric_faces[i]->rsface->getCenter()),0))
						{
							triangle->point[0] = (*e)->point[0];
							triangle->point[1] = (*e)->point[1];
						}
						else
						{
							triangle->point[0] = (*e)->point[1];
							triangle->point[1] = (*e)->point[0];
						}
						triangle->point[2] = point;
						face_part->triangles.push_back(triangle);
					}
				}
			}
			face_part->points.push_back(point);
						#ifdef debug_triangulation
						//TTS2HIN(*face_part,"DATA/MOLECULE/sphericFace"+IndexToString(i,0));
						#endif
			surface->join(*face_part);
		}
		delete face_part;
		return surface;
	}


	template <class T>
	void TriangulateToricFace
				(TSESFace<T>*																	face,
				 const T&																			radius_of_probe,
				 const T&																			sqrt_density,
				 TTriangulatedSurface<T>*&										surface,
				 std::vector<TTrianglePoint<T>*>							points,
				 std::vector<	std::list<TTriangleEdge<T>*> >& border)
	{
		surface = new TTriangulatedSurface<T>;
		if (face->isFree())
		{
			TriangulateFreeToricFace(face,radius_of_probe,sqrt_density,
															 points,surface,border);
			return;
		}
		if (face->rsedge->isSingular())
		{
			TriangulateSingularToricFace(face,radius_of_probe,sqrt_density,
																	 points,surface,border);
			return;
		}
		face->normalize(false);
		TSESEdge<T>* edge0 = face->edge[0];
		TSESEdge<T>* edge1 = face->edge[1];
		TSESEdge<T>* edge2 = face->edge[2];
		TSESEdge<T>* edge3 = face->edge[3];
		TSESVertex<T>* p0 = face->vertex[0];
		TSESVertex<T>* p1 = face->vertex[1];
		TSESVertex<T>* p2 = face->vertex[2];
		TSESVertex<T>* p3 = face->vertex[3];
		Size number_of_segments =
			(Size)Maths::round(face->rsedge->getPhi().value*
												 edge3->circle.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<T> psi(face->rsedge->getPhi().value/number_of_segments,true);
		TVector3<T> normal((p0->p-edge3->circle.p)%(p3->p-edge3->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().value,Constants::PI))
		{
			normal.negate();
		}
		TCircle3<T> circle3(edge3->circle.p,normal,edge3->circle.radius);
		TCircle3<T> circle1(edge1->circle.p,normal,edge1->circle.radius);
		vector< TVector3<T> > edge3_segments;
		vector< TVector3<T> > edge1_segments;
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
		BuildTriangles(face,edge0,edge1,edge2,edge3,centers,edge1_segments,edge3_segments,
									 points,radius_of_probe,sqrt_density,surface,border);
		TTriangle<T>* test_triangle = surface->triangles.front();
		TVector3<T> orth( (test_triangle->point[1]->p-test_triangle->point[0]->p) %
											(test_triangle->point[2]->p-test_triangle->point[0]->p));
		if (Maths::isGreater(orth*test_triangle->point[0]->p,orth*centers[0]))
		{
			typename std::list<TTriangle<T>*>::iterator t;
			for (t = surface->triangles.begin(); t != surface->triangles.end(); t++)
			{
				TTrianglePoint<T>* temp = (*t)->point[0];
				(*t)->point[0] = (*t)->point[1];
				(*t)->point[1] = temp;
			}
		}
	}


	template <class T>
	void TriangulateFreeToricFace
		(TSESFace<T>*														 face,
		 const T&																 radius_of_probe,
		 const T&																 sqrt_density,
		 std::vector<TTrianglePoint<T>*>				 points,
		 TTriangulatedSurface<T>*								 surface,
		 std::vector< list<TTriangleEdge<T>*> >& border)
	{
		TVector3<T> normal(face->edge[0]->circle.n);
		TCircle3<T> circle1(face->edge[0]->circle);
		TCircle3<T> circle2(face->edge[1]->circle.p,
												normal,
												face->edge[1]->circle.radius);
		TCircle3<T> circle3(face->rsedge->getCenterOfTorus(),
												normal,
												face->rsedge->getMajorRadiusOfTorus());
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
		BuildTriangles(face,dummy,face->edge[0],dummy,face->edge[1],
									 centers,points1,points2,
									 points,radius_of_probe,sqrt_density,surface,border);
	}


	template <class T>
	void TriangulateSingularToricFace
				(TSESFace<T>*																	face,
				 const T&																			radius_of_probe,
				 const T&																			sqrt_density,
				 std::vector< TTrianglePoint<T>* >						points,
				 TTriangulatedSurface<T>*											surface,
				 std::vector< std::list<TTriangleEdge<T>*> >& border)
	{
		face->normalize(true);
		Size number_of_segments
				= (Size)Maths::round(face->rsedge->getPhi().toRadian()*
														 face->edge[1]->circle.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<T> psi(face->rsedge->getPhi().toRadian()/number_of_segments,true);
		TVector3<T> normal((face->vertex[0]->p-face->edge[0]->circle.p)%
											 (face->vertex[2]->p-face->edge[0]->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().value,Constants::PI))
		{
			normal.negate();
		}
		Position offset = 0;
		std::list<TTriangle<T>*>::iterator last = surface->triangles.end();
		for (Position counter = 0; counter < 2; counter++)
		{
			TCircle3<T> circle(face->edge[0+offset]->circle.p,
												 normal,
												 face->edge[0+offset]->circle.radius);
			vector< TVector3<T> > edge_segments;
			edge_segments = PartitionOfCircle(circle,face->vertex[0+offset]->p,
																				psi,number_of_segments);
			edge_segments.pop_back();
			edge_segments.push_back(face->vertex[2+offset]->p);
			vector< TVector3<T> > point;
			for (Position i = 0; i < number_of_segments+1; i++)
			{
				point.push_back(face->vertex[1+offset]->p);
			}
			TCircle3<T> center_circle(face->rsedge->getCenterOfTorus(),
																normal,
																face->rsedge->getMajorRadiusOfTorus());
			vector< TVector3<T> > centers;
			centers = PartitionOfCircle(center_circle,face->edge[1+offset]->circle.p,
																	psi,number_of_segments);
			centers.pop_back();
			centers.push_back(face->edge[2+offset]->circle.p);
			TSESEdge<T>* dummy(NULL);
			BuildTriangles(face,face->edge[1+offset],face->edge[0+offset],
										 face->edge[2+offset],dummy,centers,edge_segments,point,
										 points,radius_of_probe,sqrt_density,surface,border);
			offset += 3;
			TTriangle<T>* test_triangle;
			typename std::list<TTriangle<T>*>::iterator start;
			if (last == surface->triangles.end())
			{
				test_triangle = surface->triangles.front();
				last--;
				start = surface->triangles.begin();
			}
			else
			{
				last++;
				test_triangle = *last;
				start = last;
			}
			TVector3<T> orth( (test_triangle->point[1]->p-test_triangle->point[0]->p) %
												(test_triangle->point[2]->p-test_triangle->point[0]->p));
			if (Maths::isGreater(orth*test_triangle->point[0]->p,orth*centers[0]))
			{
				typename std::list<TTriangle<T>*>::iterator t;
				for (t = start; t != surface->triangles.end(); t++)
				{
					TTrianglePoint<T>* temp = (*t)->point[0];
					(*t)->point[0] = (*t)->point[1];
					(*t)->point[1] = temp;
				}
			}
		}
	}


	template <class T>
	void PartitionSingularEdge(TSESEdge<T>* singular_edge, const T& sqrt_density,
														 const std::vector<TTrianglePoint<T>*>& point,
														 std::vector< std::list<TTriangleEdge<T>*> >& edge,
														 TTriangulatedSurface<T>* surface)
	{
		if (singular_edge->vertex1 == NULL)
		{
			TAngle<T> phi;
			phi.value = 2*Constants::PI;
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
			points = PartitionOfCircle(singular_edge->circle,
																 TVector3<T>::getZero(),
																 psi,number_of_segments);
			points.pop_back();
			TTrianglePoint<T>* p0;
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = new TTrianglePoint<T>;
			p1->p = points[0];
			p1->n = singular_edge->circle.p-p1->p;
			surface->points.push_back(p1);
			p0 = p1;
			for (Position k = 1; k < points.size(); k++)
			{
			p2 = new TTrianglePoint<T>;
				p2->p = points[k];
				p2->n = singular_edge->circle.p-p2->p;
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
			e = new TTriangleEdge<T>;
			e->point[0] = p1;
			e->point[1] = p0;
			e->triangle.clear();
			edge[singular_edge->index].push_back(e);
			p1->edge.push_back(e);
			p0->edge.push_back(e);
		}
		else
		{
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
			TVector3<T> normal((singular_edge->vertex1->p-singular_edge->circle.p)%
												 (singular_edge->vertex2->p-singular_edge->circle.p));
			singular_edge->circle.n = normal;
			vector< TVector3<T> > points;
			points = PartitionOfCircle(singular_edge->circle,
																 singular_edge->vertex1->p,
																 psi,number_of_segments);
			points.pop_back();
			points.push_back(singular_edge->vertex2->p);
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = point[singular_edge->vertex1->index];
			p1->n = p1->p-singular_edge->circle.p;
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
			p2->n = p2->p-singular_edge->circle.p;
			e = new TTriangleEdge<T>;
			e->point[0] = p1;
			e->point[1] = p2;
			e->triangle.clear();
			edge[singular_edge->index].push_back(e);
			p1->edge.push_back(e);
			p2->edge.push_back(e);
		}
	}


	template <class T>
	void TriangulateContactFace
			(TSESFace<T>*																				face,
			 const TSphere3<T>&																	sphere,
			 const T&																						density,
			 const T&																						,//probe_radius,
			 const HashMap<Size,TTriangulatedSurface<T>*>&			spheres,
			 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
			 TTriangulatedSurface<T>*														surface)
	{
					#ifdef debug_surface_processor_verbose
					std::cout << "    ... " << face->index << " ... ";
					#endif
		TTriangulatedSurface<T> part(*spheres[numberOfRefinements(density,sphere.radius)]);
		part.blowUp(sphere.radius);
		Position number_of_edges = face->edge.size();
		Position number_of_points = face->vertex.size();
		if (number_of_edges > 0)
		{
			TPlane3<T> plane;
			TSESEdge<T>* edge;
			for (Position k = 0; k < number_of_edges; k++)
			{
				edge = face->edge[k];
				plane.p = edge->circle.p-sphere.p;
				if (face->orientation[k] == 0)
				{
					plane.n = edge->circle.n;
				}
				else
				{
					plane.n = -(edge->circle.n);
				}
				part.cut(plane,0.05);
						#ifdef print_debug_info
						std::cout << "schneide mit plane " << plane << " von edge " << *edge
											<< " ... " << part.points.size() << "\n";
						#endif
						#ifdef debug_triangulation_with_planes
						plane.p += sphere.p;
						Plane2HIN(plane,"DATA/PLANES/circle"+IndexToString(k,0)+"of"+IndexToString(face->index,0)+".hin");
						#endif
			}
					//std::cout << part.points.size() << " ... ";
			TVector3<T> p1;
			TVector3<T> p2;
			TVector3<T> p3;
			TVector3<T> normal;
			TVector3<T> axis;
			TAngle<T> angle;
			TAngle<T> min;
			if (number_of_points == 3)
			{
				edge = face->edge[0];
				Position k = 0;
				while (edge->isFree())
				{
					k++;
					edge = face->edge[k];
				}
				p1.set(edge->vertex1->p);
				p2.set(edge->vertex2->p);
				p3.set(face->vertex[0]->p);
				Position l = 0;
				while ((p3 == p1) || (p3 == p2))
				{
					l++;
					p3.set(face->vertex[l]->p);
				}
				axis.set(p2-p1);
				TVector3<T> edge_normal(edge->circle.n);
				if (face->orientation[k] == 1)
				{
					edge_normal.negate();
				}
				normal.set(edge_normal%axis);
						#ifdef debug_triangulation_with_planes
						bool swaped = false;
						#endif
				if (edge->rsedge->getPhi().value > Constants::PI)
				{
					if (Maths::isGreater(normal*edge->circle.p,normal*p1))
					{
						axis.negate();
								#ifdef debug_triangulation_with_planes
								swaped = true;
								#endif
								#ifdef print_debug_info
								std::cout << "geswaped\n";
								#endif
					}
				}
				else
				{
					if (Maths::isLess(normal*edge->circle.p,normal*p1))
					{
						axis.negate();
								#ifdef debug_triangulation_with_planes
								swaped = true;
								#endif
								#ifdef print_debug_info
								std::cout << "geswaped\n";
								#endif
					}
				}
						#ifdef print_debug_info
						std::cout << "Drehachse ermittelt\n";
						#endif
						#ifdef debug_triangulation_with_planes
						if (swaped)
						{
							Line2HIN(p2,p1,"DATA/PLANES/axis0of"+IndexToString(face->index,0)+".hin");
						}
						else
						{
								Line2HIN(p1,p2,"DATA/PLANES/axis0of"+IndexToString(face->index,0)+".hin");
						}
						#endif
				plane.set(p1,p2,p3);
				if (plane.n*plane.n > 0.0)
				{
					if (getOrientedAngle(edge_normal,plane.n,axis) > getOrientedAngle(edge_normal,-plane.n,axis))
					{
						plane.n.negate();
					}
					plane.p -= sphere.p;
					part.cut(plane,0.05);
							#ifdef print_debug_info
							std::cout << "schneide mit plane " << plane << " durch die drei vertices der face\n";
							#endif
							#ifdef debug_triangulation_with_planes
							plane.p += sphere.p;
							Plane2HIN(plane,"DATA/PLANES/plane0of"+IndexToString(face->index,0)+".hin");
							#endif
				}
			}
			else
			{
				if (number_of_points > 3)
				{
					TPlane3<T> test_plane;
					Position k = 0;
					while (k < number_of_edges)
					{
						edge = face->edge[k];
						if (edge->isFree() == false)
						{
									#ifdef print_debug_info
									std::cout << "suche Ebene für edge " << edge << "\n";
									#endif
							p1.set(edge->vertex1->p);
							p2.set(edge->vertex2->p);
							axis.set(p2-p1);
									#ifdef debug_triangulation_with_planes
									bool swaped = false;
									#endif
							TVector3<T> edge_normal(edge->circle.n);
							if (face->orientation[k] == 1)
							{
								edge_normal.negate();
							}
							normal.set(edge_normal%axis);
							if (edge->rsedge->getPhi().value > Constants::PI)
							{
								if (Maths::isGreater(normal*edge->circle.p,normal*p1))
								{
									axis.negate();
											#ifdef debug_triangulation_with_planes
											swaped = true;
											#endif
											#ifdef print_debug_info
											std::cout << "geswaped\n";
											#endif
								}
							}
							else
							{
								if (Maths::isLess(normal*edge->circle.p,normal*p1))
								{
									axis.negate();
											#ifdef debug_triangulation_with_planes
											swaped = true;
											#endif
											#ifdef print_debug_info
											std::cout << "geswaped\n";
											#endif
								}
							}
									#ifdef print_debug_info
									std::cout << "Drehachse ermittelt\n";
									#endif
									#ifdef debug_triangulation_with_planes
									if (swaped)
									{
										Line2HIN(p2,p1,"DATA/PLANES/axis"+IndexToString(k,0)+"of"+IndexToString(face->index,0)+".hin");
									}
									else
									{
										Line2HIN(p1,p2,"DATA/PLANES/axis"+IndexToString(k,0)+"of"+IndexToString(face->index,0)+".hin");
									}
									#endif
							test_plane.p.set(p1);
							Position l = 0;
							min.set(3*Constants::PI,true);
							bool found = false;
							while (l < number_of_points)
							{
								p3.set(face->vertex[l]->p);
								test_plane.n.set((p3-p1)%(p2-p1));
								if (Maths::isGreater(test_plane.n*test_plane.n,0))
								{
									angle = getOrientedAngle(edge_normal,test_plane.n,axis);
									if (angle < min)
									{
										plane.set(test_plane);
										min = angle;
										found = true;
									}
									test_plane.n.negate();
									angle = getOrientedAngle(edge_normal,test_plane.n,axis);
									if (angle < min)
									{
										plane.set(test_plane);
										min = angle;
										found = true;
									}
								}
								l++;
							}
							if (found)
							{
										#ifdef print_debug_info
										std::cout << "Minimum gefunden ... ";
										#endif
								plane.p -= sphere.p;
								part.cut(plane,0.0);
										#ifdef debug_triangulation_with_planes
										plane.p += sphere.p;
										Plane2HIN(plane,"DATA/PLANES/plane"+IndexToString(k,0)+"of"+IndexToString(face->index,0)+".hin");
										#endif
										#ifdef print_debug_info
										std::cout << part.points.size() << "\n";
										#endif
							}
						}
						k++;
					}
				}
			}
					//std::cout << part.points.size() << " ... ";
			std::list<TTriangleEdge<T>*> border;
			part.getBorder(border);
			part.shift(sphere.p);
			part.setIndices();
					#ifdef print_debug_info
					int halt = 0;
					if (halt == 0) cin >> halt; else { cout << "\n"; halt--;}
					#endif
					#ifdef debug_triangulation
					//TTS2HIN(part,"DATA/MOLECULE/contactFaceCut"+IndexToString(face->index,0));
					#endif
					#ifdef debug_surface_processor_verbose
					bool empty = (border.size() == 0);
					#endif
			#ifdef print_debug_info
			BuildSphericTriangles(part,border,edge_contours,face->edge,sphere,halt);
			#else
			BuildSphericTriangles(part,border,edge_contours,face->edge,sphere);
			#endif
					#ifdef debug_surface_processor_verbose
					if (empty)
					{
						std::cout << "cut, joined (empty)\n";
					}
					else
					{
						std::cout << "cut, joined\n";
					}
					#endif
		}
		else
		{
			part.shift(sphere.p);
					#ifdef debug_surface_processor_verbose
					std::cout << "full sphere\n";
					#endif
		}
					#ifdef debug_triangulation
					TTS2HIN(part,"DATA/MOLECULE/contactFace"+IndexToString(face->index,0));
					#endif
		surface->join(part);
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
	void BuildSphericTriangles
		(			 TTriangulatedSurface<T>&											part,
					 std::list<TTriangleEdge<T>*>&								border,
		 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
		 const vector<TSESEdge<T>*>&												sesedge,
		 const TSphere3<T>&																	sphere
		 		#ifdef print_debug_info
		 		, int halt
		 		#endif
		)
	{
				#ifdef print_debug_info
				string pre = "      ";
				#endif
		HashSet<TTrianglePoint<T>*> points;
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = part.points.begin(); p != part.points.end(); p++)
		{
			points.insert(*p);
				#ifdef print_debug_info
				std::cout << (*p)->index << "  ";
				#endif
		}
				#ifdef print_debug_info
				std::cout << "\n";
				#endif
				#ifdef print_debug_info
				for (p = part.points.begin(); p != part.points.end(); p++)
				{
					std::cout << **p << "\n";
				}
				#endif
		typename std::list<TTriangleEdge<T>*>::const_iterator c;
		for (Position k = 0; k < sesedge.size(); k++)
		{
					#ifdef print_debug_info
					std::cout << sesedge[k]->index << ":  ";
					#endif
			for (c = edge_contours[sesedge[k]->index].begin();
					 c != edge_contours[sesedge[k]->index].end(); c++)
			{
				points.insert((*c)->point[0]);
				points.insert((*c)->point[1]);
				#ifdef print_debug_info
				std::cout << (*c)->point[0]->index << "  ";
				std::cout << (*c)->point[1]->index << "  ";
				#endif
				//if (Maths::isNotEqual(sphere.p.getDistance((*c)->point[0]->p),
				//											sphere.radius))
				//{
				//	TVector3<T> diff((*c)->point[0]->p-sphere.p);
				//	diff.normalize();
				//	diff *= sphere.radius;
				//	(*c)->point[0]->p = sphere.p+diff;
				//}
				//if (Maths::isNotEqual(sphere.p.getDistance((*c)->point[1]->p),
				//											sphere.radius))
				//{
				//	TVector3<T> diff((*c)->point[1]->p-sphere.p);
				//	diff.normalize();
				//	diff *= sphere.radius;
				//	(*c)->point[1]->p = sphere.p+diff;
				//}
			}
				#ifdef print_debug_info
				std::cout << "\n";
				#endif
		}
				#ifdef print_debug_info
				//HashSet<TTrianglePoint<T>*>::Iterator h;
				//for (h = points.begin(); h != points.end(); h++)
				//{
				//	std::cout << pre << (*h)->index << "\n";
				//}
				#endif
		if (border.size() == 0)
		{
			#ifdef print_debug_info
			BuildFirstTriangle(part,border,edge_contours,sesedge,sphere,points,halt);
			#else
			BuildFirstTriangle(part,border,edge_contours,sesedge,sphere,points);
			#endif
		}
		while (border.size() > 0)
		{
					#ifdef print_debug_info
					std::cout << pre << "derzeit " << border.size() << " Edges";
					if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
			  	#endif
			TTriangleEdge<T>* edge = border.front();
			border.pop_front();
					#ifdef print_debug_info
			  	std::cout << pre << *edge << " gepopt\n";
			  	#endif
			TTriangle<T>* start_triangle = edge->triangle[0];
			TTrianglePoint<T>* third_point = start_triangle->third(edge->point[0],edge->point[1]);
			TVector3<T> normal((third_point->p-edge->point[0]->p) %
												 (third_point->p-edge->point[1]->p)	);
			T test_value = normal*third_point->p;
			if (Maths::isLess(normal*sphere.p,test_value))
			{
				normal.negate();
				test_value = -test_value;
			}
			else
			{
				TTrianglePoint<T>* tmp = edge->point[0];
				edge->point[0] = edge->point[1];
				edge->point[1] = tmp;
			}
					#ifdef print_debug_info
					std::cout << pre << "starte mit Edge " << *edge << " (" << edge << ")\n";
					std::cout << pre << "third point: " << third_point->index << "\n";
					std::cout << pre << "Dreieck: " << *start_triangle << " (" << test_value << ")  ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			typename HashSet<TTrianglePoint<T>*>::Iterator next = points.begin();
			while ((*next == edge->point[0]) || (*next == edge->point[1]) || (*next == third_point))
			{
				next++;
			}
			std::list<TTrianglePoint<T>*> third;
			third.push_back(*next);
			normal.set(((*next)->p-edge->point[1]->p) %
								 ((*next)->p-edge->point[0]->p)	);
			test_value = normal*edge->point[0]->p;
					#ifdef print_debug_info
					std::cout << pre << "  dritter Punkt: " << (*next)->index
										<< " (" << test_value << ") " << " ... ";
					std::cout << "not ok (" << test_value << ")  "
										<< "--> " << normal << ":  " << test_value << "  ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			next++;
			while (next != points.end())
			{
				if ((*next != edge->point[0]) && (*next != edge->point[1]) &&
						(*next != third_point))
				{
					T this_value = normal*(*next)->p;
							#ifdef print_debug_info
							std::cout << pre << "  dritter Punkt: " << (*next)->index
									 << " (" << test_value << ") " << " ... ";
							#endif
					if (Maths::isGreater(this_value,test_value))
					{
								#ifdef print_debug_info
								std::cout << "not ok (" << this_value << ")  ";
								#endif
						third.clear();
						third.push_back(*next);
						normal.set(((*next)->p-edge->point[1]->p) %
											 ((*next)->p-edge->point[0]->p)	);
						test_value = normal*edge->point[0]->p;
								#ifdef print_debug_info
								std::cout << "--> " << normal << ":  " << test_value << "  ";
								#endif
					}
					else
					{
						if (Maths::isEqual(this_value,test_value))
						{
							third.push_back(*next);
								#ifdef print_debug_info
								std::cout << "added (" << this_value << ")  ";
								#endif
						}
								#ifdef print_debug_info
								else
								{
									std::cout << "ok (" << this_value << ")  ";
								}
								#endif
					}
				}
						#ifdef print_debug_info
						else
						{
							std::cout << pre << "  dritter Punkt: " << (*next)->index << " ... --  ";
						}
						#endif
				next++;
						#ifdef print_debug_info
						if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
						#endif
			}
			switch (third.size())
			{
				case 0 :
									#ifdef print_debug_info
									std::cout << pre << "no convex triangle found\n";
									#endif
									break;
				case 1 :
									#ifdef print_debug_info
									BuildUnambiguousTriangle(edge,third.front(),border,sphere,part,
																					 halt);
									#else
									BuildUnambiguousTriangle(edge,third.front(),border,sphere,part);
									#endif
									break;
				default :
									#ifdef print_debug_info
									BuildAmbiguousTriangles(edge,third,border,sphere,part,
																					halt);
									#else
									BuildAmbiguousTriangles(edge,third,border,sphere,part);
									#endif
									break;
			}
		}
	}


	template <class T>
	void BuildFirstTriangle
		(			 TTriangulatedSurface<T>&											part,
					 std::list<TTriangleEdge<T>*>&								border,
		 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
		 const vector<TSESEdge<T>*>&												sesedge,
		 const TSphere3<T>&																	sphere,
		 const HashSet<TTrianglePoint<T>*>&									points
					#ifdef print_debug_info
					, int& halt
					#endif
		)
	{
		Position i = 0;
		TSESEdge<T>* first_sesedge = sesedge[0];
		while (first_sesedge->type == 2)
		{
			i++;
			first_sesedge = sesedge[i];
		}
				#ifdef print_debug_info
				std::cout << "nichtsinguläre SESEdge: " << *first_sesedge << "\nPunkte: ";
				#endif
		HashSet<TTrianglePoint<T>*> same_edge;
		typename std::list<TTriangleEdge<T>*>::const_iterator c;
		for (c = edge_contours[first_sesedge->index].begin();
				 c != edge_contours[first_sesedge->index].end(); c++)
		{
			same_edge.insert((*c)->point[0]);
			same_edge.insert((*c)->point[1]);
		}
				#ifdef print_debug_info
				HashSet<TTrianglePoint<T>*>::Iterator se;
				for (se = same_edge.begin(); se != same_edge.end(); se++)
				{
					std::cout << (*se)->index << "  ";
				}
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		TTriangleEdge<T>* edge = edge_contours[first_sesedge->index].front();
				#ifdef print_debug_info
				std::cout << "suche Dreieck zu edge " << *edge;
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		typename HashSet<TTrianglePoint<T>*>::ConstIterator next = points.begin();
		typename HashSet<TTrianglePoint<T>*>::ConstIterator test;
		std::list<TTrianglePoint<T>*> third;
		TVector3<T> normal;
		while (next != points.end())
		{
			if (!same_edge.has(*next))
			{
				normal.set((edge->point[0]->p-edge->point[1]->p) %
									 (edge->point[0]->p-(*next)->p)		      );
				T test_value = normal*edge->point[0]->p;
				bool in = false;
				bool out = false;
				test = points.begin();
				while (test != points.end())
				{
					if ((*test != *next) && (*test != edge->point[0]) && (*test != edge->point[1]))
					{
						T this_value = normal*(*test)->p;
						if (Maths::isLess(this_value,test_value))
						{
							in = true;
						}
						else
						{
							if (Maths::isGreater(this_value,test_value))
							{
								out = true;
							}
						}
					}
					test++;
				}
				if (!(in && out))
				{
					third.push_back(*next);
							#ifdef print_debug_info
							std::cout << **next << " passt\n";
							#endif
				}
						#ifdef print_debug_info
						else
						{
							std::cout << **next << " passt nicht\n";
						}
						#endif
			}
			next++;
		}
				#ifdef print_debug_info
				std::cout << third.size() << " Kandidaten gefunden: ";
				std::list<TTrianglePoint<T>*>::iterator rt;
				for (rt = third.begin(); rt != third.end(); rt++)
				{
					std::cout << (*rt)->index << "  ";
				}
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
				//if (third.size() != 1) { std::cout << third.size() << "  "; }
		TTriangle<T>* triangle = edge->triangle[0];
		Index p0 = triangle->relativePointIndex(edge->point[0]);
		Index p1 = triangle->relativePointIndex(edge->point[1]);
		Index diff = p1-p0;
		if ((diff == 1) || (diff == -2))
		{
			TTrianglePoint<T>* tmp = edge->point[0];
			edge->point[0] = edge->point[1];
			edge->point[1] = tmp;
		}
				#ifdef print_debug_info
				std::cout << "edge " << *edge << " orientiert";
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		normal.set((triangle->point[0]->p-triangle->point[1]->p) %
							 (triangle->point[0]->p-triangle->point[2]->p)	);
		TAngle<T> min(3*Constants::PI,true);
		std::list<TTrianglePoint<T>*> real_third;
		std::list<TTrianglePoint<T>*>::iterator p = third.begin();
		TVector3<T> new_normal;
		TVector3<T> axis(edge->point[1]->p-edge->point[0]->p);
		TAngle<T> angle;
		while (p != third.end())
		{
			new_normal.set((edge->point[0]->p-edge->point[1]->p) %
										 (edge->point[0]->p-(*p)->p)						);
			angle = getOrientedAngle(normal,new_normal,axis);
						// ACHTUNG !!!
						angle.value += Constants::PI;
			if (angle < min)
			{
				real_third.clear();
				real_third.push_back(*p);
			}
			else
			{
				if (angle == min)
				{
					real_third.push_back(*p);
				}
			}
			p++;
		}
				#ifdef print_debug_info
				std::cout << real_third.size() << " tatsächliche Kandidaten: ";
				for (rt = real_third.begin(); rt != real_third.end(); rt++)
				{
					std::cout << (*rt)->index << "  ";
				}
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
				//if (third.size() != 1) { std::cout << real_third.size() << "\n"; }
		switch (real_third.size())
		{
			case 0 :
								#ifdef print_debug_info
								std::cout << "no convex triangle found\n";
								#endif
								break;
			case 1 :
								#ifdef print_debug_info
								BuildUnambiguousTriangle(edge,real_third.front(),border,sphere,part,
																				 halt);
								#else
								BuildUnambiguousTriangle(edge,real_third.front(),border,sphere,part);
								#endif
								break;
			default :
								#ifdef print_debug_info
								BuildAmbiguousTriangles(edge,real_third,border,sphere,part,
																				halt);
								#else
								BuildAmbiguousTriangles(edge,real_third,border,sphere,part);
								#endif
								break;
		}
	}
	/*template <class T>
	void TriangulateEmptyFace
		(TTriangulatedSurface<T>&														part,
		 TSESFace<T>*																				face,
		 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contour,
		 const T&																						probe_radius,
		 const TSphere3<T>&																	sphere)
	{
		TVector3<T> center(sphere.p);
		TVector3<T> mass(0,0,0);
		for (Position j = 0; j < face->vertex.size(); j++)
		{
			mass += face->vertex[j]->p;
		}
		mass -= (center*(T)face->vertex.size());
		mass.normalize();
		mass *= probe_radius;
		mass += center;
		TTrianglePoint<T>* point = new TTrianglePoint<T>;
		point->p = mass;
		point->n = center-mass;
		TTriangle<T>* triangle;
		for (Position j = 0; j < face->edge.size(); j++)
		{
			std::list<TTriangleEdge<T>*>::const_iterator e;
			for (e = edge_contour[face->edge[j]->index].begin();
					 e != edge_contour[face->edge[j]->index].end(); e++)
			{
				triangle = new TTriangle<T>;
				TVector3<T> norm(((*e)->point[0]->p-(*e)->point[1]->p)%
												 ((*e)->point[0]->p-point->p));
				if (Maths::isLess(norm*((*e)->point[0]->p-center),0))
				{
					triangle->point[0] = (*e)->point[0];
					triangle->point[1] = (*e)->point[1];
				}
				else
				{
					triangle->point[0] = (*e)->point[1];
					triangle->point[1] = (*e)->point[0];
				}
				triangle->point[2] = point;
				part.triangles.push_back(triangle);
			}
		}
		part.points.push_back(point);
	}*/


	template <class T>
	void BuildUnambiguousTriangle
		(TTriangleEdge<T>*						 edge,
		 TTrianglePoint<T>*						 point,
		 std::list<TTriangleEdge<T>*>& border,
		 const TSphere3<T>&						 sphere,
		 TTriangulatedSurface<T>&			 part
					#ifdef print_debug_info
					, int& halt
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
		CreateTriangleAndEdges(edge,point,sphere,edge1,old1,edge2,old2,triangle);
		if (old1 == true)
		{
			border.remove(edge1);
					#ifdef print_debug_info
					std::cout << pre << "remove Edge " << *edge1
										<< " (" << edge1 << ") from\n" << pre << "  border\n";
					#endif
		}
		else
		{
			edge1->point[0]->edge.push_back(edge1);
			edge1->point[1]->edge.push_back(edge1);
					#ifdef with_indices
					edge1->index = part.edges.size();
					#endif
			part.edges.push_back(edge1);
			border.push_back(edge1);
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge " << *edge1
										<< " (" << edge1 << ") to\n" << pre << "  part\n" << pre << "  border\n";
					#endif
		}
		if (old2 == true)
		{
			border.remove(edge2);
					#ifdef print_debug_info
					std::cout << pre << "remove Edge " << *edge2 << " (" << edge2 << ") from\n" << pre << "  border\n";
					#endif
		}
		else
		{
			edge2->point[0]->edge.push_back(edge2);
			edge2->point[1]->edge.push_back(edge2);
					#ifdef with_indices
					edge2->index = part.edges.size();
					#endif
			part.edges.push_back(edge2);
			border.push_back(edge2);
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge " << *edge2 << " (" << edge2 << ") to\n" << pre << "  part\n" << pre << "  border\n";
					#endif
		}
		edge->triangle.push_back(triangle);
		edge1->triangle.push_back(triangle);
		edge2->triangle.push_back(triangle);
		triangle->point[0]->triangle.push_back(triangle);
		triangle->point[1]->triangle.push_back(triangle);
		triangle->point[2]->triangle.push_back(triangle);
				#ifdef with_indices
				triangle->index = part.triangles.size();
				#endif
		part.triangles.push_back(triangle);
					#ifdef print_debug_info
					std::cout << pre << "geupdatet:\n";
					std::cout << pre << "  Edge " << *edge << " (" << edge << ")\n";
					std::cout << pre << "  Edge " << *edge1 << " (" << edge1 << ")\n";
					std::cout << pre << "  Edge " << *edge2 << " (" << edge2 << ")\n";
					std::cout << pre << "  Point " << *(triangle->point[0]) << " (" << triangle->point[0] << ")\n";
					std::cout << pre << "  Point " << *(triangle->point[1]) << " (" << triangle->point[1] << ")\n";
					std::cout << pre << "  Point " << *(triangle->point[2]) << " (" << triangle->point[2] << ")\n";
					std::cout << pre << "  Triangle " << *triangle << " (" << triangle << ")\n";
					std::cout << pre << "pushe Triangle " << *triangle << " (" << triangle << ") to\n" << pre << "  surface";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
	}



	template <class T>
	void BuildAmbiguousTriangles
		(TTriangleEdge<T>*						 edge,
		 std::list<TTrianglePoint<T>*> points,
		 std::list<TTriangleEdge<T>*>& border,
		 const TSphere3<T>&						 sphere,
		 TTriangulatedSurface<T>&			 part
					#ifdef print_debug_info
					, int& halt
					#endif
		)
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
				#ifdef print_debug_info
				string pre = "        ";
				std::cout << pre << points.size() << " Punkte gefunden ... (";
				for (p = points.begin(); p != points.end(); p++)
				{
					std::cout << (*p)->index << " ";
				}
				std::cout << ")\n";
				#endif
		points.push_back(edge->point[0]);
		points.push_back(edge->point[1]);
				#ifdef print_debug_info
				std::cout << pre << points.size() << " Punkte in einer Ebene ... (";
				for (p = points.begin(); p != points.end(); p++)
				{
					std::cout << (*p)->index << " ";
				}
				std::cout << ") ";
				if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
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
					std::cout << pre << "suche nach Dreieck für Edge " << *edge0 << " (" << edge0 << ") ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			planar_edges.pop_front();
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
							std::cout << pre << "baue Dreieck mit Punkt " << **p << " (" << *p << ") ";
							if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					CreateTriangleAndEdges(edge0,*p,sphere,edge1,old1,edge2,old2,triangle);
							#ifdef print_debug_info
							std::cout << pre << "teste dieses Dreieck: ";
							if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					if (edge0 == edge)
					{
						built = true;
					}
					else
					{
						TTriangle<T>* old_triangle = edge0->triangle[0];
						//TTrianglePoint<T>* third = old_triangle->third(edge0->point[0],edge0->point[1]);
								#ifdef print_debug_info
								std::cout << pre << "  teste gegen Dreieck " << *old_triangle;
								std::cout << " (" << old_triangle << ") ";
								if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						Index i1 = old_triangle->relativePointIndex(edge0->point[0]);
						Index i2 = old_triangle->relativePointIndex(edge0->point[1]);
						bool back = false;
						if ((i1-i2 == 1) || (i1-i2 == -2))
						{
							back = true;
						}
						i1 = triangle->relativePointIndex(edge0->point[0]);
						i2 = triangle->relativePointIndex(edge0->point[1]);
						if (back)
						{
							if ((i1-i2 == -1) || (i1-i2 == 2))
							{
								built = true;
							}
						}
						else
						{
							if ((i1-i2 == 1) || (i1-i2 == -2))
							{
								built = true;
							}
						}
						/*TVector3<T> orth1((edge0->point[0]->p-edge0->point[1]->p)%
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
								if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						if (phi > pi_2)
						{
							built = true;
						}*/
					}
							#ifdef print_debug_info	
							std::cout << pre << (built ? "ok " : "nok ") << border.size() << " " << planar_edges.size() << " ";
							if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					if (built)
					{
						edge0->triangle.push_back(triangle);
						edge1->triangle.push_back(triangle);
						edge2->triangle.push_back(triangle);
						triangle->point[0]->triangle.push_back(triangle);
						triangle->point[1]->triangle.push_back(triangle);
						triangle->point[2]->triangle.push_back(triangle);
						if (old1 == true)
						{
									#ifdef print_debug_info
									std::cout << pre << "remove Edge " << *edge1 << " (" << edge1 << ") from\n" << pre << "  border\n";
									#endif
							border.remove(edge1);
							planar_edges.remove(edge1);
						}
						else
						{
							border.push_back(edge1);		// ACHTUNG
							planar_edges.push_back(edge1);
							edge1->point[0]->edge.push_back(edge1);
							edge1->point[1]->edge.push_back(edge1);
									#ifdef with_indices
									edge1->index = part.edges.size();
									#endif
							part.edges.push_back(edge1);
									#ifdef print_debug_info
									std::cout << pre << "pushe Edge " << *edge1 << " (" << edge1 << ") to\n" << pre << "  planar_edges\n" << pre << "  part\n";
									#endif
						}
						if (old2 == true)
						{
									#ifdef print_debug_info
									std::cout << pre << "remove Edge " << *edge2 << " (" << edge2 << ") from\n" << pre << "  border\n";
									#endif
							border.remove(edge2);
							planar_edges.remove(edge2);
						}
						else
						{
							border.push_back(edge2);		// ACHTUNG
							planar_edges.push_back(edge2);
							edge2->point[0]->edge.push_back(edge2);
							edge2->point[1]->edge.push_back(edge2);
									#ifdef with_indices
									edge2->index = part.edges.size();
									#endif
							part.edges.push_back(edge2);
									#ifdef print_debug_info
									std::cout << pre << "pushe Edge " << *edge2 << " (" << edge2 << ") to\n" << pre << "  planar_edges\n" << pre << "  part\n";
									#endif
						}
						border.remove(edge0);
								#ifdef with_indices
								triangle->index = part.triangles.size();
								#endif
						part.triangles.push_back(triangle);
								#ifdef print_debug_info
								std::cout << pre << "remove Edge" << *edge0 << " (" << edge0 << ") from\n" << pre << "  border\n";
								std::cout << pre << "pushe Triangle " << *triangle << " (" << triangle << ") to\n" << pre << "  part";
								if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
					}
					else
					{
						p++;
								#ifdef print_debug_info
								std::cout << pre << "zerstöre Triangle "  << *triangle << " (" << triangle << ")\n";
								#endif
						delete triangle;
						if (old1 == false)
						{
									#ifdef print_debug_info
									std::cout << pre << "zerstöre Edge " << *edge1 << " (" << edge1 << ")\n";
									#endif
							delete edge1;
						}
						if (old2 == false)
						{
									#ifdef print_debug_info
									std::cout << pre << "zerstöre Edge " << *edge2 << " (" << edge2 << ")\n";
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
		(TTriangleEdge<T>*	edge,
		 TTrianglePoint<T>* point,
		 const TSphere3<T>& sphere,
		 TTriangleEdge<T>*& edge1,
		 bool&							old1,
		 TTriangleEdge<T>*& edge2,
		 bool&							old2,
		 TTriangle<T>*&			triangle
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
									<< pre << "  " << *edge1 << " (" << edge1 << ")\n"
									<< pre << "  " << *edge2 << " (" << edge2 << ")\n";
				#endif
		// do the edges exist ?
		TTriangleEdge<T>* test = edge->point[0]->has(edge1);
		if (test != NULL)
		{
					#ifdef print_debug_info
					std::cout << pre << "erste existiert schon: " << *test << " (" << test << ")\n";
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
		test = edge->point[1]->has(edge2);
		if (test != NULL)
		{
					#ifdef print_debug_info
					std::cout << pre << "zweite existiert schon: " << *test << " (" << test << ")\n";
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
		// build the new triangle
		triangle = new TTriangle<T>;
		triangle->point[0] = edge->point[1];
		triangle->point[1] = edge->point[0];
		triangle->point[2] = point;
		triangle->edge[0] = edge;
		triangle->edge[1] = edge1;
		triangle->edge[2] = edge2;
		// swap triangle if necessary
		TVector3<T> test_vector((triangle->point[1]->p-triangle->point[0]->p)%
														(triangle->point[2]->p-triangle->point[0]->p));
		if (Maths::isGreater(test_vector*sphere.p,
												 test_vector*triangle->point[0]->p))
		{
			TTrianglePoint<T>* temp = triangle->point[0];
			triangle->point[0] = triangle->point[1];
			triangle->point[1] = temp;
		}
				#ifdef print_debug_info
				std::cout << pre << "Dreieck gebaut: " << *triangle << " (" << triangle << ")\n";
				#endif
	}


	/********** BUILDTRIANGLES **************************************************/

	template <class T>
	bool BuildTriangles
		 (TSESFace<T>* /*face*/,
			TSESEdge<T>*																 edge0,
			TSESEdge<T>*																 edge1,
			TSESEdge<T>*																 edge2,
			TSESEdge<T>*																 edge3,
			const std::vector< TVector3<T> >&						 centers,
			const std::vector< TVector3<T> >&						 edge1_points,
			const std::vector< TVector3<T> >&						 edge3_points,
			std::vector<TTrianglePoint<T>*>							 existing_points,
			const T&																		 radius_of_probe,
			const T&																		 sqrt_density,
			TTriangulatedSurface<T>*										 surface,
			std::vector< std::list<TTriangleEdge<T>*> >& border)
	{
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
			TTrianglePoint<T>* top = existing_points[index];
			top->n = centers[centers.size()-1]-top->p;
			top->n.normalize();
			#ifdef with_indices
			top->index = (long)(number_of_segments+1)*number_of_triangles;
			#endif
			points[(number_of_segments+1)*number_of_triangles] = top;
		}
		Position end
			= ((edge3 != NULL) ? number_of_triangles+1 : number_of_triangles);
		for (Position i = 0; i < centers.size(); i++)
		{
			TCircle3<T> circle
				(centers[i],
				 (edge1_points[i]-centers[i])%(edge3_points[i]-centers[i]),
				 radius_of_probe);
			vector< TVector3<T> > line;
			line = PartitionOfCircle(circle,edge1_points[i],phi,number_of_triangles);
			for (Position j = 0; j < end; j++)
			{
				Index index = -1;
				if (edge0 != NULL)
				{
					if ((i == 0) && (j == 0))
					{
						index = ((line[0] == edge0->vertex1->p) ? edge0->vertex1->index
																										: edge0->vertex2->index);
					}
					if ((i == 0) && (j == end-1) && (edge3 != NULL))
					{
						index
							= ((line[end-1] == edge0->vertex1->p) ? edge0->vertex1->index
																										: edge0->vertex2->index);
					}
					if ((i == centers.size()-1) && (j == 0))
					{
						index = ((line[0] == edge2->vertex1->p) ? edge2->vertex1->index
																										: edge2->vertex2->index);
					}
					if ((i == centers.size()-1) && (j == end-1) && (edge3 != NULL))
					{
						index
							= ((line[end-1] == edge2->vertex1->p) ? edge2->vertex1->index
																										: edge2->vertex2->index);
					}
				}
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
				#ifdef with_indices
				point->index = (long)i*end+j;
				#endif
				points[i*end+j] = point;
			}
		}
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
		Position add1 = (end-1)*(number_of_segments+1);
		Position offset;
		Position pos;
		for (Position j = 0; j < number_of_segments; j++)
		{
			offset = j*end;
			for (Position i = 0; i < end; i++)
			{
				pos = offset+i+add1;
				edges[pos] = new TTriangleEdge<T>;
				edges[pos]->point[0] = points[offset+i];
				edges[pos]->point[1] = points[offset+i+end];
				edges[pos]->point[0]->edge.push_back(edges[pos]);
				edges[pos]->point[1]->edge.push_back(edges[pos]);
				edges[pos]->triangle.clear();
				if ((i == 0) && (edge1 != 0))
				{
					border[edge1->index].push_back(edges[pos]);
				}
				else
				{
					if ((i == end-1) && (edge3 != 0))
					{
						border[edge3->index].push_back(edges[pos]);
					}
				}
			}
		}
		Position add2 = add1+end*number_of_segments;
		for (Position j = 0; j < number_of_segments; j++)
		{
			offset = j*end-j;
			for (Position i = 0; i < end-1; i++)
			{
				pos = offset+i+add2;
				edges[pos] = new TTriangleEdge<T>;
				edges[pos]->point[0] = points[offset+i+j];
				edges[pos]->point[1] = points[offset+i+j+end+1];
				edges[pos]->point[0]->edge.push_back(edges[pos]);
				edges[pos]->point[1]->edge.push_back(edges[pos]);
				edges[pos]->triangle.clear();
			}
		}
		Size triangle_number
				= ((edge3 != NULL) ? 2*number_of_segments*number_of_triangles
													 : 2*number_of_segments*(number_of_triangles-1));
		std::vector<TTriangle<T>*>	triangles(triangle_number);
		for (Position i = 0; i < end-1; i++)
		{
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
							#ifdef with_indices
							t1->index = (long)2*(number_of_segments*i+j);
							#endif
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
							#ifdef with_indices
							t2->index = (long)2*(number_of_segments*i+j)+1;
							#endif
					triangles[2*(number_of_segments*i+j)+1] = t2;
			}
		}
		if (edge0 == NULL)
		{
			Position offset = number_of_segments*end;
			TTriangleEdge<T>* edge = new TTriangleEdge<T>;
				edge->point[0] = points[offset];
				edge->point[1] = points[0];
				edge->triangle.erase(edge->triangle.begin(),edge->triangle.end());
				edges.push_back(edge);
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
							#ifdef with_indices
							t1->index = (long)triangles.size();
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
							#ifdef with_indices
							t2->index = (long)triangles.size();
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
							#ifdef with_indices
							t->index = (long)triangles.size();
							#endif
					triangles.push_back(t);
			}
		}
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
		return true;
	}
	
	
	template <class T>
	Size numberOfRefinements(const T& density, const T& radius)
	{
		T test0 = (4.0*density*Constants::PI*radius*radius-12.0)/30.0;
		Size n = 0;
		if (Maths::isGreaterOrEqual(test0,(T)0))
		{
			Size test1;
			Size test2 = 1;
			while (Maths::isLess(test2,test0))
			{
				test1 = test2;
				test2 *= 4;
				n++;
			}
			if (Maths::isLess((T)test2-test0,test0-(T)test1))
			{
				n++;
			}
		}
		if (n > 3)
		{
			n = 3;
		}
		return n;
	}
	
	
	template <class T>
	void BuildSpheres(HashMap<Size,TTriangulatedSurface<T>*>& spheres)
	{
		spheres[0] = new TTriangulatedSurface<T>;
		spheres[0]->icosaeder(true);
		spheres[0]->setIndices();
		spheres[1] = new TTriangulatedSurface<T>;
		spheres[1]->icosaeder(true);
		spheres[1]->refineSphere(1,true);
		spheres[1]->setIndices();
		spheres[2] = new TTriangulatedSurface<T>;
		spheres[2]->icosaeder(true);
		spheres[2]->refineSphere(2,true);
		spheres[2]->setIndices();
		spheres[3] = new TTriangulatedSurface<T>;
		spheres[3]->icosaeder(true);
		spheres[3]->refineSphere(3,true);
		spheres[3]->setIndices();
	}
	
	
	#ifdef debug_triangulation
	template <class T>
	void TTS2HIN(const TTriangulatedSurface<T>& surface, string filename)
	{
		/*Molecule* molecule = new Molecule;
		Atom* atom1;
		Atom* atom2;
		std::list<TTriangleEdge<T>*>::const_iterator e;
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
		{
			atom1 = new Atom;
			atom1->setPosition((*e)->point[0]->p);
			atom1->setElement(PTE[Element::H]);
			molecule->insert(*atom1);
			atom2 = new Atom;
			atom2->setPosition((*e)->point[1]->p);
			atom2->setElement(PTE[Element::H]);
			molecule->insert(*atom2);
			#ifdef with_bonds
			atom1->createBond(*atom2);
			#endif
		}
		std::list<TTrianglePoint<T>*>::const_iterator p;
		for (p = surface.points.begin(); p != surface.points.end(); p++)
		{
			atom1 = new Atom;
			atom1->setPosition((*p)->p);
			atom1->setElement(PTE[Element::H]);
			molecule->insert(*atom1);
		}
		System* system = new System;
		system->insert(*molecule);
		string filename1 = filename;
		filename1 += "Edges.hin";
		HINFile hinfile1(filename1,std::ios::out);
		hinfile1 << *system;
		hinfile1.close();
		delete system;*/
		Molecule* molecule = new Molecule;
		Atom* atom1;
		Atom* atom2;
		//molecule = new Molecule;
		Atom* atom3;
		std::list<TTriangle<T>*>::const_iterator t;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
		{
			atom1 = new Atom;
			atom2 = new Atom;
			atom3 = new Atom;
			atom1->setPosition((*t)->point[0]->p);
			atom2->setPosition((*t)->point[1]->p);
			atom3->setPosition((*t)->point[2]->p);
			atom1->setElement(PTE[Element::H]);
			atom2->setElement(PTE[Element::H]);
			atom3->setElement(PTE[Element::H]);
			molecule->insert(*atom1);
			molecule->insert(*atom2);
			molecule->insert(*atom3);
			#ifdef with_bonds
			atom1->createBond(*atom3);
			atom1->createBond(*atom2);
			atom3->createBond(*atom2);
			#endif
		}
		System* system = new System;
		//system = new System;
		system->insert(*molecule);
		string filename2 = filename;
		filename2 += "Triangles.hin";
		HINFile hinfile2(filename2,std::ios::out);
		hinfile2 << *system;
		hinfile2.close();
		delete system;
	}
	#endif


	#ifdef debug_triangulation
	template <class T>
	void Plane2HIN(const TPlane3<T>& plane, const string& file)
	{
		std::vector<Atom*> atom;
		TVector3<T> d1;
		TVector3<T> d2;
		TVector3<T> origin = plane.p;
		Position size = 20;
		if (Maths::isZero(plane.n.x))
		{
			d1.set(0,plane.n.z,-plane.n.y);
		}
		else
		{
			d1.set(plane.n.y,-plane.n.x,0);
		}
		d2 = plane.n%d1;
		for (Position i = 0; i < 10; i++)
		{
			if (Maths::isZero(d1*d1))
			{
				d1 *= 10;
			}
		}
		for (Position i = 0; i < 10; i++)
		{
			if (Maths::isZero(d2*d2))
			{
				d2 *= 10;
			}
		}
		d1.normalize();
		d2.normalize();
		d1 /= 2;
		d2 /= 2;
		origin -= d1*(size/2);
		origin -= d2*(size/2);
		Molecule* molecule = new Molecule;
		for (Position i = 0; i < size; i++)
		{
			for (Position j = 0; j < size; j++)
			{
				Atom* a = new Atom;
				a->setPosition(origin+(d1*i)+(d2*j));
				a->setElement(PTE[Element::H]);
				atom.push_back(a);
				molecule->insert(*a);
			}
		}
		for (Position i = 0; i < size*(size-1); i++)
		{
			atom[i]->createBond(*atom[i+20]);
		}
		for (Position i = 0; i < size*size; i++)
		{
			if ((i%size) != 0)
			{
				atom[i]->createBond(*atom[i-1]);
			}
		}
		TVector3<T> normal = plane.n;
		normal.normalize();
		Atom* a1 = new Atom;
		a1->setPosition(plane.p+normal);
		a1->setElement(PTE[Element::H]);
		Atom* a2 = new Atom;
		a2->setPosition(plane.p);
		a2->setElement(PTE[Element::H]);
		a2->createBond(*a1);
		molecule->insert(*a1);
		molecule->insert(*a2);
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(file,std::ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}
	#endif


	#ifdef debug_triangulation
	template <class T>
	void Line2HIN(const TVector3<T>& v1, const TVector3<T>& v2, const string& file)
	{
		Atom* a1 = new Atom;
		a1->setPosition(v1);
		a1->setElement(PTE[Element::C]);
		Atom* a2 = new Atom;
		a2->setPosition(v2);
		a2->setElement(PTE[Element::O]);
		a2->createBond(*a1);
		Molecule* molecule = new Molecule;
		molecule->insert(*a1);
		molecule->insert(*a2);
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(file,std::ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}
	#endif






}	// namespace BALL

#endif	// BALL_STRUCTURE_SINGULARITIES_H

