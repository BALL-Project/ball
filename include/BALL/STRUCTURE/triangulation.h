// $Id: triangulation.h,v 1.6 2000/10/19 17:03:21 oliver Exp $

#ifndef BALL_STRUCTURE_TRIANGULATION_H
#define BALL_STRUCTURE_TRIANGULATION_H

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_STRUCTURE_SESVERTEX_H
#	include <BALL/STRUCTURE/SESVertex.h>
#endif

#ifndef BALL_STRUCTURE_SESEDGE_H
#	include <BALL/STRUCTURE/SESEdge.h>
#endif

#ifndef BALL_STRUCTURE_SESFACE_H
#	include <BALL/STRUCTURE/SESFace.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
#endif

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventExcludedSurface.h>
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
#include <set>
#include <fstream>


namespace BALL
{

	/**
	*/
	template <class T>
	TTriangulatedSurface<T>* Triangulate(TSolventExcludedSurface<T>* ses, TReducedSurface<T>* rs,
																			 const T& size)
	{
		TTriangulatedSurface<T>* surface = new TTriangulatedSurface<T>();
		T radius_of_probe(rs->getProbeRadius());
		HashMap< T,TTriangulatedSurface<T> > spheres;
		TTriangulatedSurface<T>* probe_sphere(NULL);
		std::vector< TTriangulatedSurface<T>* > triangulated_toric_faces(ses->toric_faces.size(),NULL);
		std::vector< TTriangulatedSurface<T>* >	triangulated_contact_faces;
		std::vector< TTriangulatedSurface<T>* >	triangulated_spheric_faces;
		std::vector< std::list< TTriangulatedSurface<T>::Point* > > toric_borders(ses->edges.size());
		std::vector< std::set< TTriangulatedSurface<T>::Edge* > > contact_borders;
		std::vector< std::set< TTriangulatedSurface<T>::Edge* > > spheric_borders;
		std::vector< std::vector< TSESEdge<T>* > > contact_contours;
		std::vector< std::vector< TSESEdge<T>* > > spheric_contours;
		for (Position i = 0; i < ses->toric_faces.size(); i++)
		{
			if (ses->toric_faces[i] != NULL)
			{
				triangulated_toric_faces[i] = new TTriangulatedSurface<T>();
				TriangulateToricFace(ses->toric_faces[i],ses,radius_of_probe,size,
														 *triangulated_toric_faces[i],toric_borders);
				*surface += (*triangulated_toric_faces[i]);
			}
		}
		for (Position i = 0; i < ses->contact_faces.size(); i++)
		{
			TSphere3<T> atom(rs->getSphere(ses->contact_faces[i]->rsvertex->getAtom()));
			TriangulateContactFace(ses->contact_faces[i],atom,size,
														 spheres,triangulated_contact_faces,contact_borders,contact_contours);
		}
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
		{
			TSphere3<T> probe(ses->spheric_faces[i]->rsface->getCenter(),radius_of_probe);
			TriangulateSphericFace(ses->spheric_faces[i],probe,size,
														 probe_sphere,triangulated_spheric_faces,spheric_borders,spheric_contours);
		}
#ifdef debug_tri
surface->setIndices();
#endif
		for (Position i = 0; i < triangulated_contact_faces.size(); i++)
		{
			*surface += (*triangulated_contact_faces[i]);
			Sew(contact_borders[i],contact_contours[i],toric_borders,surface);
		}
		for (Position i = 0; i < triangulated_spheric_faces.size(); i++)
		{
			*surface += (*triangulated_spheric_faces[i]);
			Sew(spheric_borders[i],spheric_contours[i],toric_borders,surface);
		}
		return surface;
	}


	template <class T>
	void TriangulateToricFace(TSESFace<T>* face, TSolventExcludedSurface<T>* ses,
														const T& radius_of_probe, const T& size,
														TTriangulatedSurface<T>& surface,
														std::vector< list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
		if (face->isFree())
		{
			TriangulateFreeToricFace(face,radius_of_probe,size,surface,border);
			return;
		}
		TSESEdge<T>* edge0;
		TSESEdge<T>* edge1;
		TSESEdge<T>* edge2;
		TSESEdge<T>* edge3;
		TSESVertex<T>* p0;
		TSESVertex<T>* p1;
		TSESVertex<T>* p2;
		TSESVertex<T>* p3;
		Index i = 0;
		while (face->edge[i]->type != 1)
		{
			i++;
		}
		edge0 = face->edge[i];								// edge0 = first concave edge
		i++;
		while (face->edge[i]->type != 1)
		{
			i++;
		}
		edge2 = face->edge[i];								// edge2 = second concave edge
		p0 = edge0->vertex1;
		p1 = edge0->vertex2;
		Index e;
		if (face->getEdge(p1->index,edge2->vertex1->index,e))
		{
			edge1 = ses->edges[e];
			p2 = edge2->vertex1;
			p3 = edge2->vertex2;
		}
		else
		{
			if (face->getEdge(p1->index,edge2->vertex2->index,e))
			{
				edge1 = ses->edges[e];
				p2 = edge2->vertex2;
				p3 = edge2->vertex1;
			}
			else
			{
				// should never happen
			}
		}
		if (face->getEdge(p0->index,p3->index,e))
		{
			edge3 = ses->edges[e];
		}
		else
		{
			// should never happen
		}
		Size number_of_segments = (int)Maths::round(face->rsedge->getPhi().toRadian()*edge3->circle.radius/size);
		TAngle<T> psi(face->rsedge->getPhi().toRadian()/number_of_segments,true);
		vector< TVector3<T> > edge3_segments;
		vector< TVector3<T> > edge1_segments;
		TVector3<T> normal((p0->p-edge3->circle.p)%(p3->p-edge3->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().toRadian(),Constants::PI))
		{
			normal.negate();
		}
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
		BuildTriangles(face,edge0,edge1,edge2,edge3,centers,edge1_segments,edge3_segments,
									 radius_of_probe,size,surface,border);
	}


	template <class T>
	void TriangulateFreeToricFace(TSESFace<T>* face, const T& radius_of_probe, const T& size,
																TTriangulatedSurface<T>& surface,
																std::vector< list< typename TTriangulatedSurface<T>::Point* > >& border)
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
		Size number_of_points = (int)Maths::round(2*Constants::PI*circle1.radius/size);
		TAngle<T> phi(2*Constants::PI/number_of_points,true);
		vector< TVector3<T> > points1;
		vector< TVector3<T> > points2;
		vector< TVector3<T> > centers;
		points1 = PartitionOfCircle(circle1,p1,phi,number_of_points);
		points2 = PartitionOfCircle(circle2,p2,phi,number_of_points);
		centers = PartitionOfCircle(circle3,p3,phi,number_of_points);
		points1.pop_back();
		points2.pop_back();
		centers.pop_back();
		TSESEdge<T>* dummy(NULL);
		BuildTriangles(face,dummy,face->edge[0],dummy,face->edge[1],centers,points1,points2,
									 radius_of_probe,size,surface,border);
	}


	template <class T>
	void TriangulateContactFace(TSESFace<T>* face, const TSphere3<T> atom, const T& size,
															HashMap< T,TTriangulatedSurface<T> >& spheres,
															std::vector<TTriangulatedSurface<T>*>& triangulated_faces,
															std::vector< std::set<typename TTriangulatedSurface<T>::Edge*> >& borders,
															std::vector< std::vector<TSESEdge<T>* > >& contours)
	{
		std::vector< std::vector< TSESEdge<T>* > > this_contours = GetFaceContours(face);
		for (Position c = 0; c < this_contours.size(); c++)
		{
			TSESFace<T>* face_part = new TSESFace<T>(*face);
			face_part->edge = this_contours[c];
			TTriangulatedSurface<T>* triangulated_face_part;
//			if (spheres.has(atom.radius))
//			{
//				triangulated_face_part = new TTriangulatedSurface<T>(spheres[atom.radius]);
//				triangulated_face_part->shift(atom.p);
//			}
//			else
//			{
				triangulated_face_part = new TTriangulatedSurface<T>();
				triangulated_face_part->icosaeder(atom.radius,true);
				triangulated_face_part->refineSphere(atom.radius,1,true);
//				spheres[atom.radius] = *triangulated_face_part;
				triangulated_face_part->shift(atom.p);
//			}
			std::set<TTriangulatedSurface<T>::Edge*> border;
			CutSphericFace(face_part,*triangulated_face_part,border);
			triangulated_faces.push_back(triangulated_face_part);
			borders.push_back(border);
			contours.push_back(this_contours[c]);
		}
	}


	template <class T>
	void TriangulateSphericFace(TSESFace<T>* face, const TSphere3<T>& probe, const T& size,
//															TTriangulatedSurface<T>*& triangulated_probe,
															TTriangulatedSurface<T>* triangulated_probe,
															std::vector<TTriangulatedSurface<T>*>& triangulated_faces,
															std::vector< std::set<typename TTriangulatedSurface<T>::Edge*> >& borders,
															std::vector< std::vector< TSESEdge<T>* > >& contours)
	{
		std::vector< std::vector< TSESEdge<T>* > > this_contours = GetFaceContours(face);
		for (Position c = 0; c < this_contours.size(); c++)
		{
			TSESFace<T>* face_part = new TSESFace<T>(*face);
			face_part->edge = this_contours[c];
			TTriangulatedSurface<T>* triangulated_face_part;
			if (triangulated_probe == NULL)
			{
				triangulated_probe = new TTriangulatedSurface<T>();
				triangulated_probe->icosaeder(probe.radius);
				triangulated_probe->refineSphere(probe.radius,2,false);
			}
			triangulated_face_part = new TTriangulatedSurface<T>(*triangulated_probe);
			triangulated_face_part->shift(probe.p);
			std::set<TTriangulatedSurface<T>::Edge*> border;
			CutSphericFace(face_part,*triangulated_face_part,border);
			triangulated_faces.push_back(triangulated_face_part);
			borders.push_back(border);
			contours.push_back(this_contours[c]);
		}
	}


	template <class T>
	void CutSphericFace(TSESFace<T>* face, TTriangulatedSurface<T>& surface,
											std::set< typename TTriangulatedSurface<T>::Edge* >& border)
	{
		for (Position i = 0; i < face->edge.size(); i++)
		{
			if (face->edge[i] != NULL)
			{
				list< TTriangulatedSurface<T>::Point* >::iterator p;
				for (p = surface.points.begin(); p != surface.points.end(); p++)
				{
					(*p)->state = 0;
				}
				TVector3<T> norm = face->edge[i]->circle.n;
				if (face->orientation[i] == 1)
				{
					norm.negate();
				}
				T test_value = norm*face->edge[i]->circle.p;
				list< TTriangulatedSurface<T>::Triangle* >::iterator t = surface.triangles.begin();
				while (t != surface.triangles.end())
				{
					TTriangulatedSurface<T>::Triangle* triangle = *t;
					t++;
					Position del = 0;
					Position offset = 1;
					for (Position j = 0; j < 3; j++)
					{
						switch (triangle->point[j]->state)
						{
							case 0 :	if (Maths::isLess(norm*triangle->point[j]->p,test_value))
												{
													triangle->point[j]->state = 1;
													del += offset;
												}
												else
												{
													triangle->point[j]->state = 2;
												}
												break;
							case 1 :	del += offset;
												break;
							case 2 :	break;
						}
						offset *= 2;
					}
					if (del != 0)
					{
						surface.triangles.remove(triangle);
						for (Position k = 0; k < 3; k++)
						{
							triangle->edge[k]->del(triangle);
							triangle->point[k]->triangle.remove(triangle);
						}
						delete triangle;
					}
				}
			}
		}
		HashSet< TTriangulatedSurface<T>::Edge* > existing_edges;
		HashSet< TTriangulatedSurface<T>::Point* > existing_points;
		list< TTriangulatedSurface<T>::Triangle* >::iterator t;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
		{
			for (Position k = 0; k < 3; k++)
			{
				TTriangulatedSurface<T>::Edge* edge = (*t)->edge[k];
				existing_edges.insert(edge);
				surface.edges.remove(edge);
				if ((edge->triangle[0] == NULL) || (edge->triangle[1] == NULL))
				{
					border.insert(edge);
				}
				TTriangulatedSurface<T>::Point* point = (*t)->point[k];
				existing_points.insert(point);
				surface.points.remove(point);
			}
		}
		while (surface.edges.size() > 0)
		{
			TTriangulatedSurface<T>::Edge* edge = surface.edges.front();
			edge->point[0]->edge.remove(edge);
			edge->point[1]->edge.remove(edge);
			delete edge;
			surface.edges.pop_front();
		}
		while (surface.points.size() > 0)
		{
			delete surface.points.front();
			surface.points.pop_front();
		}
		HashSet< TTriangulatedSurface<T>::Edge* >::Iterator e;
		for (e = existing_edges.begin(); e != existing_edges.end(); e++)
		{
			surface.edges.push_back(*e);
		}
		HashSet< TTriangulatedSurface<T>::Point* >::Iterator point;
		for (point = existing_points.begin(); point != existing_points.end(); point++)
		{
			surface.points.push_back(*point);
		}
	}




	template <class T>
	vector< TVector3<T> > PartitionOfCircle(const TCircle3<T>& circle_, const TVector3<T>& p0,
																					const TAngle<T>& phi, const Size number_of_segments)
	{
		TCircle3<T> circle(circle_);
		for (Position i = 0; i < 3; i++)
		{
			if (Maths::isZero(circle.n[i]))
			{
				circle.n[i] = (T)0;
			}
		}
		TVector4<T> e1;
		if (circle.has(p0,true))
		{
			TVector3<T> e1_(p0-circle.p);
			e1.set(e1_.x,e1_.y,e1_.z,(T)0);
		}
		else
		{
			e1.set(circle.n.y,-circle.n.x,(T)0,(T)0);
			if (e1 == TVector4<T>::getZero())
			{
				e1.set(circle.n.z,(T)0,-circle.n.x,(T)0);
			}
		}
		e1.normalize();
		TVector3<T> e2_(circle.n%TVector3<T>(e1.x,e1.y,e1.z));
		e2_.normalize();
		TVector4<T> e2(e2_.x,e2_.y,e2_.z,(T)0);
		TVector3<T> e3_(circle.n);
		e3_.normalize();
		TVector4<T> e3(e3_.x,e3_.y,e3_.z,(T)0);
		for (Position i = 0; i < 3; i++)
		{
			if (Maths::isZero(e1[i]))
			{
				e1[i] = (T)0;
			}
			if (Maths::isZero(e2[i]))
			{
				e2[i] = (T)0;
			}
		}
		TMatrix4x4<T> change(e1.x,e1.y,e1.z,(T)0,
												 e2.x,e2.y,e2.z,(T)0,
												 e3.x,e3.y,e3.z,(T)0,
												 (T)0,(T)0,(T)0,(T)1);
		TMatrix4x4<T> unchange;
		change.invert(unchange);
		TMatrix4x4<T> rotate(cos(phi.toRadian()),-sin(phi.toRadian()),(T)0,(T)0,
												 sin(phi.toRadian()), cos(phi.toRadian()),(T)0,(T)0,
												 (T)0               , (T)0               ,(T)1,(T)0,
												 (T)0               , (T)0               ,(T)0,(T)1);
		TMatrix4x4<T> rotation(unchange*rotate*change);
		TVector4<T> p(e1);
		p *= circle.radius;
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
	bool BuildTriangles(TSESFace<T>* face,
											TSESEdge<T>* edge0, TSESEdge<T>* edge1, TSESEdge<T>* edge2, TSESEdge<T>* edge3,
											const std::vector< TVector3<T> >& centers,
											const std::vector< TVector3<T> >& edge1_points,
											const std::vector< TVector3<T> >& edge3_points,
											const T& radius_of_probe, const T& size,
											TTriangulatedSurface<T>& surface,
											std::vector< std::list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
		TAngle<T> pi(Constants::PI,true);
		TAngle<T> psi;
		psi = getOrientedAngle(edge1_points[0]-centers[0],edge3_points[0]-centers[0],
													 (edge1_points[0]-centers[0])%(edge3_points[0]-centers[0]))+pi;
		Size number_of_triangles = (int)Maths::round(psi.toRadian()*radius_of_probe/size);
		TAngle<T> phi(psi/number_of_triangles,true);
		Size number_of_segments = centers.size()-1;
		std::vector< TTriangulatedSurface<T>::Point* > points((number_of_segments+1)*(number_of_triangles+1));
		for (Position i = 0; i < centers.size(); i++)
		{
			TCircle3<T> circle(centers[i],
												 (edge1_points[i]-centers[i])%(edge3_points[i]-centers[i]),
												 radius_of_probe);
			vector< TVector3<T> > line;
			line = PartitionOfCircle(circle,edge1_points[i],phi,number_of_triangles);
			for (Position j = 0; j < line.size(); j++)
			{
				TTriangulatedSurface<T>::Point* point = new TTriangulatedSurface<T>::Point();
				point->p = line[j];
				point->n = centers[i]-line[j];
				point->index = i*(number_of_triangles+1)+j;
				points[i*(number_of_triangles+1)+j] = point;
				if (j == 0)
				{
					border[edge1->index].push_back(point);
				}
				else
				{
					if (j == line.size()-1)
					{
						border[edge3->index].push_back(point);
					}
				}
				if ((i == 0) && (edge0 != NULL))
				{
					border[edge0->index].push_back(point);
				}
				else
				{
					if ((i == centers.size()-1) && (edge2 != NULL))
					{
						border[edge2->index].push_back(point);
					}
				}
			}
		}
		std::vector< TTriangulatedSurface<T>::Triangle* > triangles(2*number_of_segments*number_of_triangles);
		for (Position i = 0; i < number_of_segments; i++)
		{
			for (Position j = 0; j < number_of_triangles; j++)
			{
				TTriangulatedSurface<T>::Triangle* t1 = new TTriangulatedSurface<T>::Triangle();
					t1->point[0] = points[i*(number_of_triangles+1)+j];
					points[i*(number_of_triangles+1)+j]->triangle.push_back(t1);
					t1->point[1] = points[i*(number_of_triangles+1)+j+1];
					points[i*(number_of_triangles+1)+j+1]->triangle.push_back(t1);
					t1->point[2] = points[(i+1)*(number_of_triangles+1)+j+1];
					points[(i+1)*(number_of_triangles+1)+j+1]->triangle.push_back(t1);
					t1->index = 2*(number_of_segments*j+i);
					triangles[2*(number_of_segments*j+i)] = t1;
				TTriangulatedSurface<T>::Triangle* t2 = new TTriangulatedSurface<T>::Triangle();
					t2->point[0] = points[i*(number_of_triangles+1)+j];
					points[i*(number_of_triangles+1)+j]->triangle.push_back(t2);
					t2->point[1] = points[(i+1)*(number_of_triangles+1)+j];
					points[(i+1)*(number_of_triangles+1)+j]->triangle.push_back(t2);
					t2->point[2] = points[(i+1)*(number_of_triangles+1)+j+1];
					points[(i+1)*(number_of_triangles+1)+j+1]->triangle.push_back(t2);
					t2->index = 2*(number_of_segments*j+i)+1;
					triangles[2*(number_of_segments*j+i)+1] = t2;
			}
		}
		if (edge0 == NULL)
		{
			for (Position i = 0; i < number_of_segments; i++)
			{
				TTriangulatedSurface<T>::Triangle* t1 = new TTriangulatedSurface<T>::Triangle();
					t1->point[0] = points[(i+1)*(number_of_triangles+1)-1];
					points[(i+1)*(number_of_triangles+1)-1]->triangle.push_back(t1);
					t1->point[1] = points[i*(number_of_triangles+1)];
					points[i*(number_of_triangles+1)]->triangle.push_back(t1);
					t1->point[2] = points[(i+1)*(number_of_triangles+1)];
					points[(i+1)*(number_of_triangles+1)]->triangle.push_back(t1);
					t1->index = triangles.size();
					triangles.push_back(t1);
				TTriangulatedSurface<T>::Triangle* t2 = new TTriangulatedSurface<T>::Triangle();
					t2->point[0] = points[(i+1)*(number_of_triangles+1)-1];
					points[(i+1)*(number_of_triangles+1)-1]->triangle.push_back(t2);
					t2->point[1] = points[(i+2)*(number_of_triangles+1)-1];
					points[i*(number_of_triangles+1)]->triangle.push_back(t2);
					t2->point[2] = points[(i+1)*(number_of_triangles+1)];
					points[(i+1)*(number_of_triangles+1)]->triangle.push_back(t2);
					t2->index = triangles.size();
					triangles.push_back(t2);
			}
		}
		for (Position i = 0; i < points.size(); i++)
		{
			surface.points.push_back(points[i]);
		}
		for (Position i = 0; i < triangles.size(); i++)
		{
			surface.triangles.push_back(triangles[i]);
		}
		return true;
	}


	template <class T>
	typename TTriangulatedSurface<T>::Point* GetNext
		(typename std::list<typename TTriangulatedSurface<T>::Point* >::iterator& in,
		 typename std::list<typename TTriangulatedSurface<T>::Point* >::iterator& out,
		 std::list<typename TTriangulatedSurface<T>::Point* >& contour_in,
		 std::list<typename TTriangulatedSurface<T>::Point* >& contour_out)
	{
		if (in == contour_in.end())
		{
			out++;
			return *out;
		}
		if (out == contour_out.end())
		{
			in++;
			return *in;
		}
		list< TTriangulatedSurface<T>::Point* >::iterator next = out;
		next++;
		TTriangulatedSurface<T>::Point* third = *next;
		list< TTriangulatedSurface<T>::Point* > test_points;
		if (out == contour_out.begin())
		{
			test_points.push_back(contour_out.back());
		}
		else
		{
			list< TTriangulatedSurface<T>::Point* >::iterator pre = out;
			pre--;
			test_points.push_back(*pre);
		}
		if (next == --contour_out.end())
		{
			test_points.push_back(contour_out.front());
		}
		else
		{
			list< TTriangulatedSurface<T>::Point* >::iterator post = next;
			post++;
			test_points.push_back(*post);
		}
		list< TTriangulatedSurface<T>::Edge* >::iterator e;
		for (e = (*in)->edge.begin(); e != (*in)->edge.end(); e++)
		{
			test_points.push_back((*e)->point[0]);
			test_points.push_back((*e)->point[1]);
		}
		test_points.remove(*in);
		TVector3<T> norm = ((*in)->p-(*out)->p)%(third->p-(*out)->p);
		T test_value = norm*(*out)->p;
		int counter = 0;
		list< TTriangulatedSurface<T>::Point* >::iterator l;
		for (l = test_points.begin(); l != test_points.end(); l++)
		{
			if (Maths::isLess(norm*(*l)->p,test_value))
				{
					counter++;
				}
		}
		if ((counter == 0) || (counter == test_points.size()))
		{
			out++;
			return third;
		}
		else
		{
			in++;
			return *in;
		}
	}


	template <class  T>
	std::vector< std::vector< TSESEdge<T>* > > GetFaceContours(TSESFace<T>* face)
	{
		std::vector< std::vector< TSESEdge<T>* > > back;
		if (face->edge.size() == 0)
		{
			return back;
		}
		std::list< TSESEdge<T>* > edges;
		for (Position i = 0; i < face->edge.size(); i++)
		{
			edges.push_back(face->edge[i]);
		}
		while (edges.size() > 0)
		{
			std::vector< TSESEdge<T>* > contour;
			TSESEdge<T>* first_edge = NULL;
			while (first_edge == NULL)
			{
				first_edge = edges.front();
				edges.pop_front();
			}
			contour.push_back(first_edge);
			TSESVertex<T>* first_vertex = first_edge->vertex1;
			TSESVertex<T>* next_vertex = first_edge->vertex2;
			while (next_vertex != first_vertex)
			{
				bool found = false;
				std::list< TSESEdge<T>* >::iterator i = edges.begin();
				while (found == false)
				{
					while ((*i) == NULL)
					{
						i++;
					}
					if ((*i)->vertex1 == next_vertex)
					{
						found = true;
						contour.push_back(*i);
						next_vertex = (*i)->vertex2;
						edges.remove(*i);
					}
					else
					{
						if ((*i)->vertex2 == next_vertex)
						{
							found = true;
							contour.push_back(*i);
							next_vertex = (*i)->vertex1;
							edges.remove(*i);
						}
						else
						{
							i++;
						}
					}
				}
			}
			back.push_back(contour);
		}
		return back;
	}


	template <class T>
	void Sew(std::set<typename TTriangulatedSurface<T>::Edge* > contour, std::vector< TSESEdge<T>* > border,
					 std::vector< std::list<typename TTriangulatedSurface<T>::Point* > > edge_contours,
					 TTriangulatedSurface<T>* surface)
	{
		std::list< TTriangulatedSurface<T>::Point* > contour_out;
		GetContour(border,edge_contours,contour_out);
		std::list< TTriangulatedSurface<T>::Point* > contour_in;
		SortContour(contour,contour_out.front(),*(++(contour_out.begin())),contour_in,(T)0);
/*		std::list< TTriangulatedSurface<T>::Point* >::iterator in = contour_in.begin();
		std::list< TTriangulatedSurface<T>::Point* >::iterator out = contour_out.begin();
		TTriangulatedSurface<T>::Point* p1;
		TTriangulatedSurface<T>::Point* p2;
		TTriangulatedSurface<T>::Point* p3;
		while ((out != contour_out.end()) || (in != contour_in.end()))
		{
			p1 = *in;
			p2 = *out;
			p3 = GetNext(in,out,contour_in,contour_out);
			TTriangulatedSurface<T>::Triangle* t = new TTriangulatedSurface<T>::Triangle();
			t->point[0] = p1;
			t->point[2] = p2;
			t->point[3] = p3;
			surface->triangles.push_back(t);
		}*/
	}


	template <class T>
	void GetContour(std::vector< TSESEdge<T>* >& border,
									std::vector< std::list<typename TTriangulatedSurface<T>::Point* > > edge_contours,
									std::list< typename TTriangulatedSurface<T>::Point* >& contour)
	{
		if (border.size() == 1)
		{
			contour = edge_contours[border[0]->index];
			return;
		}
		std::list<TTriangulatedSurface<T>::Point*> current = edge_contours[border[0]->index];
		TSESVertex<T>* last_point = border[0]->vertex2;
		if (current.front()->p != border[0]->vertex1->p)
		{
			current.reverse();
		}
		std::list<TTriangulatedSurface<T>::Point*>::iterator p;
		for (p = current.begin(); p != current.end(); p++)
		{
			contour.push_back(*p);
		}
		contour.pop_back();
		for (Position i = 1; i < border.size(); i++)
		{
			current = edge_contours[border[i]->index];
			if (border[i]->vertex1 == last_point)
			{
				last_point = border[i]->vertex2;
			}
			else
			{
				last_point = border[i]->vertex1;
			}
			if (current.front()->p == last_point->p)
			{
				current.reverse();
			}
			for (p = current.begin(); p != current.end(); p++)
			{
				contour.push_back(*p);
			}
			contour.pop_back();
		}
	}


	template <class T>
	void SortContour
		(std::set< typename TTriangulatedSurface<T>::Edge* >& border,
		 typename TTriangulatedSurface<T>::Point* point1, typename TTriangulatedSurface<T>::Point* point2,
 		 std::list< typename TTriangulatedSurface<T>::Point* >& contour, const T& dummy)
	{
		std::set< TTriangulatedSurface<T>::Edge* > border_;
		border_.insert(border.begin(),border.end());
		//	find the first point: the closest point to point1
		TTriangulatedSurface<T>::Edge* next_edge = *border.begin();
		T dist = point1->p.getDistance(next_edge->point[0]->p);
		TTriangulatedSurface<T>::Point* first = next_edge->point[0];
		std::set< TTriangulatedSurface<T>::Edge* >::iterator b;
		for (b = border_.begin(); b != border_.end(); b++)
		{
			if (Maths::isLess(point1->p.getDistance((*b)->point[0]->p),dist))
			{
				dist = point1->p.getDistance((*b)->point[0]->p);
				first = (*b)->point[0];
			}
			if (Maths::isLess(point1->p.getDistance((*b)->point[1]->p),dist))
			{
				dist = point1->p.getDistance((*b)->point[1]->p);
				first = (*b)->point[1];
			}
		}
		contour.push_back(first);
		//	find the second point: the neighbour of the first wich is closest to point2
		std::set< TTriangulatedSurface<T>::Point* > candidates;
		std::list<TTriangulatedSurface<T>::Edge*>::iterator e;
		for (e = first->edge.begin(); e != first->edge.end(); e++)
		{
			if (border.find(*e) != border.end())
			{
				candidates.insert((*e)->point[0]);
				candidates.insert((*e)->point[1]);
			}
		}
		candidates.erase(first);
		TTriangulatedSurface<T>::Point* second;
		if (Maths::isLess(point2->p.getDistance((*candidates.begin())->p),
											point2->p.getDistance((*(--candidates.end()))->p)) )
		{
			second = *candidates.begin();
		}
		else
		{
			second = *(--candidates.end());
		}
		contour.push_back(second);
		TTriangulatedSurface<T>::Edge* last_edge;
		for (e = first->edge.begin(); e != first->edge.end(); e++)
		{
			if ( ((*e)->point[0]->p == second->p) ||
					 ((*e)->point[1]->p == second->p)    )
			{
				last_edge = *e;
				border_.erase(*e);
			}
		}
		//	now complete the contour
		while (border_.size() > 0)
		{
			TTriangulatedSurface<T>::Point* last_point = contour.back();
			list< TTriangulatedSurface<T>::Edge* > edge_candidates;
			list< TTriangulatedSurface<T>::Edge* >::iterator e;
			for (e = last_point->edge.begin(); e != last_point->edge.end(); e++)
			{
				if (border.find(*e) != border.end())
				{
					edge_candidates.push_back(*e);
				}
			}
			edge_candidates.remove(last_edge);
			next_edge = *edge_candidates.begin();
			if (next_edge->point[0]->p == last_point->p)
			{
				contour.push_back(next_edge->point[1]);
			}
			else
			{
				contour.push_back(next_edge->point[0]);
			}
			border_.erase(next_edge);
			last_edge = next_edge;
		}
		TTriangulatedSurface<T>::Point* last = *contour.begin();
		contour.remove(last);
		contour.push_front(last);
	}


}	// namespace BALL

#endif	// BALL_STRUCTURE_SINGULARITIES_H


