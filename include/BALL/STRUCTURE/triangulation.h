// $Id: triangulation.h,v 1.11 2000/12/13 19:00:52 oliver Exp $

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

namespace BALL
{

	//template <class T>
	//typedef std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > > Contour;

	/**
	*/
	template <class T>
	TTriangulatedSurface<T>* Triangulate(TSolventExcludedSurface<T>* ses, TReducedSurface<T>* rs,
																			 const T& size)
	{
		TTriangulatedSurface<T>* surface = new TTriangulatedSurface<T>;
		T radius_of_probe(rs->getProbeRadius());
		TTriangulatedSurface<T>* atom = new TTriangulatedSurface<T>;
		atom->icosaeder(true);
		atom->refineSphere(0,true);
		TTriangulatedSurface<T>* probe = new TTriangulatedSurface<T>;
		probe->icosaeder(false);
		probe->refineSphere(0,false);
		std::vector< TTriangulatedSurface<T>* > triangulated_toric_face(ses->toric_faces.size(),NULL);
		std::vector< TTriangulatedSurface<T>* >	triangulated_contact_face;
		std::vector< TTriangulatedSurface<T>* >	triangulated_spheric_face;
		std::vector< std::list< TTriangulatedSurface<T>::Point* > > toric_contour(ses->edges.size());
		//std::vector< Contour<T> > contact_contour;
		std::vector< std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > > > contact_contour;
		//std::vector< Contour<T> > spheric_contour;
		std::vector< std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > > > spheric_contour;
		std::vector< std::vector< TSESEdge<T>* > > contact_border;
		std::vector< std::vector< TSESEdge<T>* > > spheric_border;
		for (Position i = 0; i < ses->toric_faces.size(); i++)
		{
			if (ses->toric_faces[i] != NULL)
			{
				triangulated_toric_face[i] = new TTriangulatedSurface<T>;
				TriangulateToricFace(ses->toric_faces[i],radius_of_probe,size,
														 *triangulated_toric_face[i],toric_contour);
				*surface += (*triangulated_toric_face[i]);
			}
		}
		for (Position i = 0; i < ses->singular_edges.size(); i++)
		{
			TTriangulatedSurface<T>::Point* p = new TTriangulatedSurface<T>::Point;
			p->p = ses->singular_edges[i]->vertex1->p;
			p->n = p->p-ses->singular_edges[i]->circle.p;
			toric_contour[ses->singular_edges[i]->index].push_back(p);
			p = new TTriangulatedSurface<T>::Point;
			p->p = ses->singular_edges[i]->vertex2->p;
			p->n = p->p-ses->singular_edges[i]->circle.p;
			toric_contour[ses->singular_edges[i]->index].push_back(p);
		}
		for (Position i = 0; i < ses->contact_faces.size(); i++)
		{
			if (ses->contact_faces[i] != NULL)
			{
				TSphere3<T> atom_(rs->getSphere(ses->contact_faces[i]->rsvertex->getAtom()));
				TriangulateSphericFace(ses->contact_faces[i],atom_,true,// *atom,
															 triangulated_contact_face,contact_contour,contact_border);
			}
		}
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
		{
			TSphere3<T> probe_sphere(ses->spheric_faces[i]->rsface->getCenter(),radius_of_probe);
			TriangulateSphericFace(ses->spheric_faces[i],probe_sphere,false,// *probe,
														 triangulated_spheric_face,spheric_contour,spheric_border);
		}
		for (Position i = 0; i < triangulated_contact_face.size(); i++)
		{
					std::cout << "c " << i << "\n";
			*surface += (*triangulated_contact_face[i]);
					std::cout << "  ... hinzugefügt\n";
			TSphere3<T> sphere(rs->getSphere(ses->contact_faces[i]->rsvertex->getAtom()));
					std::cout << "  ... Mittelpunkt ermittelt\n";
			SewFace(contact_contour[i],toric_contour,sphere,true,surface);
					std::cout << "  ... genäht\n";
		}
		for (Position i = 0; i < triangulated_spheric_face.size(); i++)
		{
					std::cout << "s " << i << "\n";
			*surface += (*triangulated_spheric_face[i]);
					std::cout << "  ... hinzugefügt\n";
			TSphere3<T> sphere(ses->spheric_faces[i]->rsface->getCenter(),radius_of_probe);
					std::cout << "  ... Mittelpunkt ermittelt\n";
			SewFace(spheric_contour[i],toric_contour,sphere,false,surface);
					std::cout << "  ... genäht\n";
		}
		return surface;
	}


	template <class T>
	void TriangulateToricFace
				(TSESFace<T>* face, const T& radius_of_probe, const T& size,
				 TTriangulatedSurface<T>& surface,
				 std::vector< std::list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
		if (face->isFree())
		{
			TriangulateFreeToricFace(face,radius_of_probe,size,surface,border);
			return;
		}
		if (face->rsedge->isSingular())
		{
			TriangulateSingularToricFace(face,radius_of_probe,size,surface,border);
			return;
		}
				//std::ofstream print("Toric.log");
		face->normalize(false);
		TSESEdge<T>* edge0 = face->edge[0];
		TSESEdge<T>* edge1 = face->edge[1];
		TSESEdge<T>* edge2 = face->edge[2];
		TSESEdge<T>* edge3 = face->edge[3];
		TSESVertex<T>* p0 = face->vertex[0];
		TSESVertex<T>* p1 = face->vertex[1];
		TSESVertex<T>* p2 = face->vertex[2];
		TSESVertex<T>* p3 = face->vertex[3];
		Size number_of_segments = (int)Maths::round(face->rsedge->getPhi().toRadian()*edge3->circle.radius/size);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
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
		bool swap = false;
		TTriangulatedSurface<T>::Triangle* test_triangle = surface.triangles.front();
		TVector3<T> orth( (test_triangle->point[1]->p-test_triangle->point[0]->p) %
											(test_triangle->point[2]->p-test_triangle->point[0]->p));
		if (Maths::isGreater(orth*test_triangle->point[0]->p,orth*centers[0]))
		{
			swap = true;
		}
		if (swap == true)
		{
			std::list<TTriangulatedSurface<T>::Triangle*>::iterator t;
			for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
			{
				TTriangulatedSurface<T>::Point* temp = (*t)->point[0];
				(*t)->point[0] = (*t)->point[1];
				(*t)->point[1] = temp;
			}
		}
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
	void TriangulateSingularToricFace
				(TSESFace<T>* face, const T& radius_of_probe, const T& size,
				 TTriangulatedSurface<T>& surface,
				 std::vector< std::list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
		face->normalize(true);
		Size number_of_segments
				= (Size)Maths::round(face->rsedge->getPhi().toRadian()*face->edge[1]->circle.radius/size);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<T> psi(face->rsedge->getPhi().toRadian()/number_of_segments,true);
		TVector3<T> normal((face->vertex[0]->p-face->edge[0]->circle.p)%
											 (face->vertex[2]->p-face->edge[0]->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().toRadian(),Constants::PI))
		{
			normal.negate();
		}
		Position offset = 0;
		list<TTriangulatedSurface<T>::Triangle*>::iterator last = surface.triangles.end();
		for (Position counter = 0; counter < 2; counter++)
		{
			TCircle3<T> circle(face->edge[0+offset]->circle.p,
												 normal,
												 face->edge[0+offset]->circle.radius);
			vector< TVector3<T> > edge_segments;
			edge_segments = PartitionOfCircle(circle,face->vertex[0+offset]->p,psi,number_of_segments);
			edge_segments.pop_back();
			edge_segments.push_back(face->vertex[0+offset]->p);
			vector< TVector3<T> > point;
			for (Position i = 0; i < number_of_segments; i++)
			{
				point.push_back(face->vertex[1+offset]->p);
			}
			TCircle3<T> center_circle(face->rsedge->getCenterOfTorus(),
																normal,
																face->rsedge->getMajorRadiusOfTorus());
			vector< TVector3<T> > centers;
			centers = PartitionOfCircle(center_circle,face->edge[1+offset]->circle.p,psi,number_of_segments);
			centers.pop_back();
			centers.push_back(face->edge[2+offset]->circle.p);
			TSESEdge<T>* dummy(NULL);
			BuildTriangles(face,face->edge[1+offset],face->edge[0+offset],face->edge[2+offset],dummy,
										 centers,edge_segments,point,radius_of_probe,size,surface,border);
			offset += 3;
			TTriangulatedSurface<T>::Triangle* test_triangle;
			if (last == surface.triangles.end())
			{
				test_triangle = surface.triangles.front();
				last--;
			}
			else
			{
				last++;
				test_triangle = *last;
			}
			bool swap = false;
			TVector3<T> orth( (test_triangle->point[1]->p-test_triangle->point[0]->p) %
												(test_triangle->point[2]->p-test_triangle->point[0]->p));
			if (Maths::isGreater(orth*test_triangle->point[0]->p,orth*centers[0]))
			{
				swap = true;
			}
			if (swap == true)
			{
				std::list<TTriangulatedSurface<T>::Triangle*>::iterator t;
				for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
				{
					TTriangulatedSurface<T>::Point* temp = (*t)->point[0];
					(*t)->point[0] = (*t)->point[1];
					(*t)->point[1] = temp;
				}
			}
		}
	}


	template <class T>
	void TriangulateSphericFace
		(TSESFace<T>* face, const TSphere3<T>& sphere,
		 bool out,
		 //const TTriangulatedSurface<T>& triangulated_sphere,
		 std::vector<TTriangulatedSurface<T>*>& triangulated_faces,
		 //std::vector< Contour<T> > contour,
		 std::vector< std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > > >& contour,
		 std::vector< std::vector< TSESEdge<T>* > >& border)
	{
		TTriangulatedSurface<T>* triangulated_face = new TTriangulatedSurface<T>;
																							 //new TTriangulatedSurface<T>(triangulated_sphere);
				triangulated_face->icosaeder(out);
				triangulated_face->refineSphere(2,out);
		triangulated_face->blow_up(sphere.radius);
		triangulated_face->shift(sphere.p);
		std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > > this_contour;
		//Contour<T> this_contour;
		if (face->edge.size() > 0)
		{
			CutSphericFace(face,*triangulated_face,this_contour);
		}
		triangulated_faces.push_back(triangulated_face);
		contour.push_back(this_contour);
		border.push_back(face->edge);
	}


	/*template <class T>
	void TriangulateSphericFace(TSESFace<T>* face, const TSphere3<T>& sphere,
															bool out,
															//const TTriangulatedSurface<T>& triangulated_sphere,
															std::vector<TTriangulatedSurface<T>*>& triangulated_faces,
															std::vector< Position >& sphere_number,
															std::vector< HashSet<typename TTriangulatedSurface<T>::Edge*> >& borders,
															std::vector< std::vector< TSESEdge<T>* > >& contours)
	{
		std::vector< std::vector< TSESEdge<T>* > > this_contours = GetFaceContours(face);
		if (this_contours.size() == 0)
		{
			TTriangulatedSurface<T>* triangulated_face = new TTriangulatedSurface<T>;
																								 //new TTriangulatedSurface<T>(triangulated_sphere);
					triangulated_face->icosaeder(out);
					triangulated_face->refineSphere(2,out);
			triangulated_face->blow_up(sphere.radius);
			triangulated_face->shift(sphere.p);
			triangulated_faces.push_back(triangulated_face);
			return;
		}
		for (Position c = 0; c < this_contours.size(); c++)
		{
			TSESFace<T>* face_part = new TSESFace<T>(*face);
			face_part->edge = this_contours[c];
			TTriangulatedSurface<T>* triangulated_face = new TTriangulatedSurface<T>;
																								 //new TTriangulatedSurface<T>(triangulated_sphere);
					triangulated_face->icosaeder(out);
					triangulated_face->refineSphere(2,out);
			triangulated_face->blow_up(sphere.radius);
			triangulated_face->shift(sphere.p);
			HashSet<TTriangulatedSurface<T>::Edge*> border;
			CutSphericFace(face_part,*triangulated_face,border);
			triangulated_faces.push_back(triangulated_face);
			sphere_number.push_back(face->index);
			borders.push_back(border);
			contours.push_back(this_contours[c]);
		}
	}*/


	template <class T>
	void CutSphericFace(TSESFace<T>* face, TTriangulatedSurface<T>& surface,
											std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >& contour)
											//Contour<T>& contour)
	{
		std::vector< TPlane3<T> > planes;
		for (Position i = 0; i < face->edge.size(); i++)
		{
			if (face->edge[i] != NULL)
			{
				TPlane3<T> plane(face->edge[i]->circle.p,face->edge[i]->circle.n);
				planes.push_back(plane);
			}
		}
		std::vector< list<TTriangulatedSurface<T>::Edge*> > contours;
		surface.cut(planes,contours);
		Position counter = 0;
		for (Position i = 0; i < face->edge.size(); i++)
		{
			if (face->edge[i] != NULL)
			{
				pair< Position,std::list<TTriangulatedSurface<T>::Edge*> > this_contour;
				this_contour.first = face->edge[i]->index;
				this_contour.second = contours[counter];
				contour.push_back(this_contour);
				counter++;
			}
		}
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
	typename TTriangulatedSurface<T>::Point* GetNext
		(typename std::list<typename TTriangulatedSurface<T>::Point* >::iterator& in,
		 typename std::list<typename TTriangulatedSurface<T>::Point* >::iterator& out,
		 std::list<typename TTriangulatedSurface<T>::Point* >& contour_in,
		 std::list<typename TTriangulatedSurface<T>::Point* >& contour_out, const T& /*dummy*/)
	{
		if (in == --contour_in.end())
		{
			out++;
			return *out;
		}
		if (out == --contour_out.end())
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
		Position counter = 0;
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


	template <class T>
	void SewFace(std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >& contour,
							 std::vector< std::list<typename TTriangulatedSurface<T>::Point* > >& edge_contours,
							 const TSphere3<T>& sphere, bool convex,
							 TTriangulatedSurface<T>* surface)
	{
		HashSet<Position> empty;
		std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >::iterator c;
		c = contour.begin();
		while (c != contour.end())
		{
			if (c->second.size() == 0)
			{
				empty.insert(c->first);
						std::cout << "    " << c->first << "\n    ... empty\n";
			}
			else
			{
				std::list< TTriangulatedSurface<T>::Point* > contour_in;
				std::list< TTriangulatedSurface<T>::Point* > contour_out;
						std::cout << "    " << c->first << "\n";
				contour_out = edge_contours[c->first];
						std::cout << "    ... " << contour_out.size() << " Punkte außen\n";
						std::cout << "    ... " << c->second.size()+1 << " Punkte innen\n";
				SortContour(c->second,contour_out,contour_in,(T)0);
						std::cout << "    ... innen sortiert\n";
				SewContours(contour_in,contour_out,sphere.p,convex,surface);
						std::cout << "    ... genäht\n";
			}
			c++;
		}
		std::cout << "    ... behandle leere Contouren\n";
		if (empty.size() == contour.size())
		{
			SewEmptyContour(contour,edge_contours,sphere,convex,surface);
		}
		else
		{
			//SewEmptyEdges(contour,edge_contours,empty,surface);
		}
		std::cout << "    ... fertig\n";
	}


	template <class T>
	void SewContours(list<TTriangulatedSurface<T>::Point*>& contour_in,
									 list<TTriangulatedSurface<T>::Point*>& contour_out,
									 const TVector3<T>& center, bool convex,
									 TTriangulatedSurface<T>* surface)
	{
		std::list< TTriangulatedSurface<T>::Triangle* >::iterator old_last = surface->triangles.end();
		old_last--;
		std::list< TTriangulatedSurface<T>::Point* >::iterator in = contour_in.begin();
		std::list< TTriangulatedSurface<T>::Point* >::iterator out = contour_out.begin();
		TTriangulatedSurface<T>::Point* p1;
		TTriangulatedSurface<T>::Point* p2;
		TTriangulatedSurface<T>::Point* p3;
		while ((out != --contour_out.end()) || (in != --contour_in.end()))
		{
			p1 = *in;
			p2 = *out;
			p3 = GetNext(in,out,contour_in,contour_out,(T)0);
			TTriangulatedSurface<T>::Triangle* t = new TTriangulatedSurface<T>::Triangle;
			t->point[0] = p1;
			t->point[1] = p2;
			t->point[2] = p3;
			surface->triangles.push_back(t);
		}
		TTriangulatedSurface<T>::Triangle* t = new TTriangulatedSurface<T>::Triangle;
		t->point[0] = contour_in.back();
		t->point[1] = contour_out.back();
		t->point[2] = contour_in.front();
		surface->triangles.push_back(t);
		t = new TTriangulatedSurface<T>::Triangle;
		t->point[0] = contour_in.front();
		t->point[1] = contour_out.back();
		t->point[2] = contour_out.front();
		surface->triangles.push_back(t);
		std::list< TTriangulatedSurface<T>::Triangle* >::iterator new_triangle = old_last;
		new_triangle++;
		TVector3<T> norm( ((*new_triangle)->point[1]->p-(*new_triangle)->point[0]->p) %
											((*new_triangle)->point[2]->p-(*new_triangle)->point[0]->p)   );
		if ( (Maths::isGreater(norm*(*new_triangle)->point[0]->p,norm*center) && (convex == false)) ||
				 (Maths::isLess(norm*(*new_triangle)->point[0]->p,norm*center)    && (convex == true))		)
		{
			while (new_triangle != surface->triangles.end())
			{
				TTriangulatedSurface<T>::Point* temp = (*new_triangle)->point[1];
				(*new_triangle)->point[1] = (*new_triangle)->point[2];
				(*new_triangle)->point[2] = temp;
				new_triangle++;
			}
		}
	}


	template <class T>
	void SortContour
			(std::list<TTriangulatedSurface<T>::Edge*>& edges,
			 std::list<TTriangulatedSurface<T>::Point*>& counterpart,
			 std::list<TTriangulatedSurface<T>::Point*>& contour, const T& /*dummy*/)
	{
		std::list<TTriangulatedSurface<T>::Edge*> sorted_edges;
		TTriangulatedSurface<T>::Edge* start_edge = edges.front();
		TTriangulatedSurface<T>::Point* start_point = start_edge->point[0];
		edges.pop_front();
		TTriangulatedSurface<T>::Edge* last_edge = start_edge;
		TTriangulatedSurface<T>::Point* last_point = start_point;
		sorted_edges.push_back(last_edge);
		contour.push_back(last_point);
		bool found = true;
		while ((edges.size() > 0) && found)
		{
			found = false;
			std::list<TTriangulatedSurface<T>::Edge*>::iterator e = edges.begin();
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
		last_edge = start_edge;
		last_point = last_edge->point[1];
		contour.push_front(last_point);
		found = true;
		while ((edges.size() > 0) && found)
		{
			found = false;
			std::list<TTriangulatedSurface<T>::Edge*>::iterator e = edges.begin();
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
		edges = sorted_edges;
		HashMap<TTriangulatedSurface<T>::Point*,TTriangulatedSurface<T>::Point*> map;
		std::vector<TTriangulatedSurface<T>::Point*> p(4);
		p[0] = contour.front();	    p[1] = contour.back();
		p[2] = counterpart.front();	p[3] = counterpart.back();
		T d02 = p[0]->p.getDistance(p[2]->p);
		T d03 = p[0]->p.getDistance(p[3]->p);
		T d12 = p[1]->p.getDistance(p[2]->p);
		T d13 = p[1]->p.getDistance(p[3]->p);
		map[p[0]] = ((Maths::isLess(d02,d03)) ? p[2] : p[3]);
		map[p[1]] = ((Maths::isLess(d12,d13)) ? p[2] : p[3]);
		map[p[2]] = ((Maths::isLess(d02,d12)) ? p[0] : p[1]);
		map[p[3]] = ((Maths::isLess(d03,d13)) ? p[0] : p[1]);
		std::list<TTriangulatedSurface<T>::Point*> four_points;
		four_points.push_back(p[0]); four_points.push_back(p[1]);
		four_points.push_back(p[2]); four_points.push_back(p[3]);
		for (Position i = 0; i < 4; i++)
		{
			four_points.remove(map[p[i]]);
		}
		std::list<TTriangulatedSurface<T>::Point*>::iterator k;
		for (k = four_points.begin(); k != four_points.end(); k++)
		{
			map[map[*k]] = *k;
		}
		if (map[p[0]] == p[3])
		{
			contour.reverse();
			edges.reverse();
		}
	}


	template <class T>
	void SewEmptyContour
			(std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >& contour,
			 std::vector< std::list<typename TTriangulatedSurface<T>::Point* > >& edge_contours,
			 const TSphere3<T>& sphere, bool convex,
			 TTriangulatedSurface<T>* surface)
	{
		TVector3<T> point(0,0,0);
		Size n = 0;
		std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >::iterator c;
		for (c = contour.begin(); c != contour.end(); c++)
		{
			point += edge_contours[c->first].front()->p;
			point += edge_contours[c->first].back()->p;
			n += 2;
		}
		point -= (sphere.p*n);
		point.normalize();
		point *= sphere.radius;
		point += sphere.p;
		TTriangulatedSurface<T>::Point* center = new TTriangulatedSurface<T>::Point;
		center->p = point;
		center->n = (convex ? point-sphere.p : sphere.p-point);
		surface->points.push_back(center);
		for (c = contour.begin(); c != contour.end(); c++)
		{
			std::list<TTriangulatedSurface<T>::Point*>::iterator p = edge_contours[c->first].begin();
			p++;
			TTriangulatedSurface<T>::Triangle* t = new TTriangulatedSurface<T>::Triangle;
			t->point[0] = center;
			t->point[1] = edge_contours[c->first].front();
			t->point[2] = *p;
			surface->triangles.push_back(t);
			while (p != --edge_contours[c->first].end())
			{
				t = new TTriangulatedSurface<T>::Triangle;
				t->point[0] = center;
				t->point[1] = *p;
				p++;
				t->point[2] = *p;
				surface->triangles.push_back(t);
			}
		}
	}


	template <class T>
	void SewEmptyEdges
			(std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >& contour,
			 std::vector< std::list<typename TTriangulatedSurface<T>::Point* > >& edge_contours,
			 HashSet<Position>& empty,
			 TTriangulatedSurface<T>* surface)
	{
		std::cout << "#1\n";
		std::list<TTriangulatedSurface<T>::Point*> points;
		std::list< std::pair< Index,std::list<TTriangulatedSurface<T>::Edge*> > >::iterator c;
		HashSet<Position>::Iterator e;
		std::list<TTriangulatedSurface<T>::Point*>::iterator p;
		std::cout << "#2\n";
		while (empty.size() > 0)
		{
			Position edge = *empty.begin();
			empty.erase(edge);
		std::cout << "#3\n";
			for (p = edge_contours[edge].begin(); p != edge_contours[edge].end(); p++)
			{
				points.push_back(*p);
			}
		std::cout << "#4\n";
			bool done = true;
			while (done)
			{
		std::cout << "#5\n";
				done = false;
				e = empty.begin();
		std::cout << "#6\n";
				while (e != empty.end())
				{
		std::cout << "#7\n";
					if (edge_contours[*e].front() == points.front())
					{
						for (p = ++edge_contours[*e].begin(); p != edge_contours[*e].end(); p++)
						{
							points.push_front(*p);
						}
						empty.erase(*e);
						done = true;
					}
		std::cout << "#8\n";
					if (edge_contours[*e].front() == points.back())
					{
						for (p = ++edge_contours[*e].begin(); p != edge_contours[*e].end(); p++)
						{
							points.push_back(*p);
						}
						empty.erase(*e);
						done = true;
					}
		std::cout << "#9\n";
					if (edge_contours[*e].back() == points.front())
					{
						for (p = edge_contours[*e].end(); p != edge_contours[*e].begin();)
						{
							points.push_front(*(--p));
						}
						empty.erase(*e);
						done = true;
					}
		std::cout << "#10\n";
					if (edge_contours[*e].back() == points.back())
					{
						for (p = edge_contours[*e].end(); p != edge_contours[*e].begin();)
						{
							points.push_back(*(--p));
						}
						empty.erase(*e);
						done = true;
					}
		std::cout << "#11\n";
					e++;
				}
			}
		std::cout << "#12\n";
			TTriangulatedSurface<T>::Point* middle;
			c = contour.begin();
		std::cout << "#13\n";
			while (c != contour.end())
			{
		std::cout << "#14\n";
				if (edge_contours[c->first].front() == points.front())
				{
					c = contour.end();
					middle = c->second.front()->point[0];
				}
				else
				{
					if (edge_contours[c->first].back() == points.front())
					{
						c = contour.end();
						middle = c->second.back()->point[1];
					}
					else
					{
						c++;
					}
				}
			}
		std::cout << "#15\n";
			p = points.begin();
			p++;
			TTriangulatedSurface<T>::Triangle* t = new TTriangulatedSurface<T>::Triangle;
			t->point[0] = middle;
			t->point[1] = points.front();
			t->point[2] = *p;
			surface->triangles.push_back(t);
		std::cout << "#16\n";
			while (p != --points.end())
			{
				t = new TTriangulatedSurface<T>::Triangle;
				t->point[0] = middle;
				t->point[1] = *p;
				p++;
				t->point[2] = *p;
				surface->triangles.push_back(t);
			}
		std::cout << "#17\n";
		}
		std::cout << "#18\n";
	}



	/********** BUILDTRIANGLES **************************************************/

	template <class T>
	bool BuildTriangles
		 (TSESFace<T>* face,
			TSESEdge<T>* edge0, TSESEdge<T>* edge1,
			TSESEdge<T>* edge2, TSESEdge<T>* edge3,
			const std::vector< TVector3<T> >& centers,
			const std::vector< TVector3<T> >& edge1_points,
			const std::vector< TVector3<T> >& edge3_points,
			const T& radius_of_probe, const T& size,
			TTriangulatedSurface<T>& surface,
			std::vector< std::list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
						std::ofstream print("Triangulation.log");
						print << "trianguliere " << *face << "\n";
						print << "edges:\n";
						print << "  " << edge0; if (edge0 != NULL) print << ": " << *edge0; print << "\n";
						print << "  " << edge1; if (edge1 != NULL) print << ": " << *edge1; print << "\n";
						print << "  " << edge2; if (edge2 != NULL) print << ": " << *edge2; print << "\n";
						print << "  " << edge3; if (edge3 != NULL) print << ": " << *edge3; print << "\n";
						print << "Punkte von edge1:\n";
						for (Position i = 0; i < edge1_points.size(); i++) print << "  " << edge1_points[i] << "\n";
						print << "Punkte von edge3:\n";
						for (Position i = 0; i < edge3_points.size(); i++) print << "  " << edge3_points[i] << "\n";
						print << "Mittelpunkte:\n";
						for (Position i = 0; i < centers.size(); i++) print << "  " << centers[i] << "\n";
		TAngle<T> pi(Constants::PI,true);
		TAngle<T> psi;
		psi = getOrientedAngle(edge1_points[0]-centers[0],edge3_points[0]-centers[0],
													 (edge1_points[0]-centers[0])%(edge3_points[0]-centers[0]))+pi;
		Size number_of_triangles = (Size)Maths::round(psi.toRadian()*radius_of_probe/size);
		TAngle<T> phi(psi/number_of_triangles,true);
		Size number_of_segments = centers.size()-1;
		Size number_of_points = ((edge3 != NULL) ? (number_of_segments+1)*(number_of_triangles+1)
																						 : (number_of_segments+1)* number_of_triangles +1 );
		std::vector< TTriangulatedSurface<T>::Point* > points(number_of_points);
		if (edge3 == NULL)
		{
			TTriangulatedSurface<T>::Point* point = new TTriangulatedSurface<T>::Point;
			point->p = edge3_points[0];
			point->n = centers[centers.size()-1]-point->p;
			point->index = (number_of_segments+1)*number_of_triangles;
			points[(number_of_segments+1)*number_of_triangles] = point;
		}
		Position end = ((edge3 != NULL) ? number_of_triangles+1 : number_of_triangles);
				print << "berechne restliche Punkte:\n";
		for (Position i = 0; i < centers.size(); i++)
		{
			TCircle3<T> circle(centers[i],
												 (edge1_points[i]-centers[i])%(edge3_points[i]-centers[i]),
												 radius_of_probe);
					print << "  Kreis: " << circle
								<< "  von: " << edge1_points[i] << "  bis: " << edge3_points[i] << "\n";
					TPlane3<T> plane(centers[i],edge1_points[i],edge3_points[i]);
					print << "  Ebene durch " << centers[i] << ", " << edge1_points[i] << ", "
								<< edge3_points[i] << ": " << plane << "\n";
			vector< TVector3<T> > line;
			line = PartitionOfCircle(circle,edge1_points[i],phi,number_of_triangles);
			for (Position j = 0; j < end; j++)
			{
				TTriangulatedSurface<T>::Point* point = new TTriangulatedSurface<T>::Point;
				point->p = line[j];
						print << "    " << line[j] << "\n";
				point->n = centers[i]-line[j];
				point->index = i*end+j;
				points[i*end+j] = point;
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
				print << "Punkte:\n";
				for (Position i = 0; i < points.size(); i++) print << "  " << i << " " << points[i]->p << "\n";
				print << "baue Dreiecke:\n";
		Size triangle_number
				= ((edge3 != NULL) ? 2*number_of_segments*number_of_triangles
													 : 2*number_of_segments*(number_of_triangles-1));
		std::vector< TTriangulatedSurface<T>::Triangle* >	triangles(triangle_number);
		for (Position i = 0; i < end-1; i++)
		{
			for (Position j = 0; j < number_of_segments; j++)
			{
				TTriangulatedSurface<T>::Triangle* t1 = new TTriangulatedSurface<T>::Triangle;
					t1->point[0] = points[  j  *end+ i ];
					t1->point[1] = points[  j  *end+i+1];
					t1->point[2] = points[(j+1)*end+i+1];
					points[  j  *end+ i ]->triangle.push_back(t1);
					points[  j  *end+i+1]->triangle.push_back(t1);
					points[(j+1)*end+i+1]->triangle.push_back(t1);
					t1->index = 2*(number_of_segments*i+j);
							print << "  Dreieck" << 2*(number_of_segments*i+j) << ": "
										<< j*end+i << ", "	<< (j+1)*end+i+1
										<< ", " << j*end+i+1 << "\n";
					triangles[2*(number_of_segments*i+j)] = t1;
				TTriangulatedSurface<T>::Triangle* t2 = new TTriangulatedSurface<T>::Triangle;
					t2->point[0] = points[  j  *end+ i ];
					t2->point[1] = points[(j+1)*end+i+1];
					t2->point[2] = points[(j+1)*end+ i ];
					points[  j  *end+ i ]->triangle.push_back(t2);
					points[(j+1)*end+i+1]->triangle.push_back(t2);
					points[(j+1)*end+ i ]->triangle.push_back(t2);
					t2->index = 2*(number_of_segments*i+j)+1;
							print << "  Dreieck" << 2*(number_of_segments*i+j)+1 << ": "
										<< j*end+i << ", " << (j+1)*end+i
										<< ", " << (j+1)*end+i+1 << "\n";
					triangles[2*(number_of_segments*i+j)+1] = t2;
			}
		}
		if (edge0 == NULL)
		{
			Position offset = number_of_segments*end;
			for (Position i = 0; i < number_of_triangles; i++)
			{
				TTriangulatedSurface<T>::Triangle* t1 = new TTriangulatedSurface<T>::Triangle;
					t1->point[0] = points[offset+ i ];
					t1->point[1] = points[offset+i+1];
					t1->point[2] = points[       i+1];
					points[offset+ i ]->triangle.push_back(t1);
					points[offset+i+1]->triangle.push_back(t1);
					points[       i+1]->triangle.push_back(t1);
					t1->index = triangles.size();
							print << "  Dreieck" << triangles.size() << ": " << offset+i << ", " << i+1 << ", "
										<< offset+i+1 << "\n";
					triangles.push_back(t1);
				TTriangulatedSurface<T>::Triangle* t2 = new TTriangulatedSurface<T>::Triangle;
					t2->point[0] = points[offset+ i ];
					t2->point[1] = points[       i+1];
					t2->point[2] = points[        i ];
					points[offset+ i ]->triangle.push_back(t2);
					points[       i+1]->triangle.push_back(t2);
					points[        i ]->triangle.push_back(t2);
					t2->index = triangles.size();
							print << "  Dreieck" << triangles.size() << ": " << offset+i << ", " << i << ", "
										<< i+1 << "\n";
					triangles.push_back(t2);
			}
		}
		if (edge3 == NULL)
		{
			Position last = (number_of_segments+1)*number_of_triangles;
			Position offset = last-number_of_segments;
			for (Position i = 0; i < number_of_segments; i++)
			{
				TTriangulatedSurface<T>::Triangle* t = new TTriangulatedSurface<T>::Triangle;
					t->point[0] = points[offset+ i ];
					t->point[1] = points[last];
					t->point[2] = points[offset+i+1];
					points[offset+ i ]->triangle.push_back(t);
					points[       i+1]->triangle.push_back(t);
					points[last]->triangle.push_back(t);
					t->index = triangles.size();
							print << "  Dreieck" << triangles.size() << ": " << offset+i << ", " << last << ", "
										<< offset+i+1 << "\n";
					triangles.push_back(t);
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




}	// namespace BALL

#endif	// BALL_STRUCTURE_SINGULARITIES_H


