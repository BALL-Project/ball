// $Id: singularities.h,v 1.4 2000/10/30 00:19:27 amoll Exp $

#ifndef BALL_STRUCTURE_SINGULARITIES_H
#define BALL_STRUCTURE_SINGULARITIES_H

#ifndef BALL_MATHS_ANALYTICALGEOMERTY_H
#	include <BALL/MATHS/analyticalGeometry.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
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

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventExcludedSurface.h>
#endif

#include <vector>
#include <list>
#include <set>
#include <fstream>

namespace BALL
{

	template <class T>
	void TreatSingularities(TSolventExcludedSurface<T>* ses, const T& radius_of_probe)
	{
		list< TSESFace<T>* > singular_faces = GetSingularFaces(ses);
//		TreatFirstCathegory(ses,singular_faces,radius_of_probe);
//		TreatSecondCathegory(ses,radius_of_probe);
	}
	
	
	template <class T>
	list< TSESFace<T>* > GetSingularFaces(TSolventExcludedSurface<T>* ses)
	{
		list< TSESFace<T>* > back;
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
		{
			if (ses->spheric_faces[i]->rsface->isSingular() == true)
			{
				back.push_back(ses->spheric_faces[i]);
			}
		}
		return back;
	}


	template <class T>
	void TreatFirstCathegory(TSolventExcludedSurface<T>* ses, list< TSESFace<T>* >& singular_faces,
													 const T& radius_of_probe)
	{
		list< TSESFace<T>* > faces = GetFirstCathegoryFaces(singular_faces);
		while (faces.size() > 0)
		{
			TSESFace<T>* face1 = *faces.begin();
			faces.pop_front();
			singular_faces.remove(face1);
			TSESFace<T>* face2 = *faces.begin();
			faces.pop_front();
			singular_faces.remove(face2);
			Size intersections(0);
			for (Position i = 0; i < 3; i++)
			{
				if (face1->edge[i] == NULL)
				{
					intersections++;
				}
			}
			switch (intersections)
			{
				case 0 :	NoCut(face1,face2,radius_of_probe,ses);
									break;
				case 1 :	break;
				case 2 :	TwoCuts(face1,face2,radius_of_probe,ses);
									break;
				case 3 :	ThreeCuts(face1,face2,radius_of_probe,ses);
									break;
			}
		}
	}


	template <class T>
	void TreatSecondCathegory(TSolventExcludedSurface<T>* ses, const T& radius_of_probe)
	{
		vector< TSESFace<T>* > faces;
		vector< TVector3<T> > points;
		list<Index> indices;
		for (Position i = 0; i != ses->spheric_faces.size(); i++)
		{
			faces.push_back(ses->spheric_faces[i]);
			points.push_back(ses->spheric_faces[i]->rsface->getCenter());
			indices.push_back(i);
//cout << points[i] << "\n";
		}
		TBSDTree<T>* tree = new TBSDTree<T>(points,indices,"","");
		tree->build();
		for (Position i = 0; i < ses->singular_edges.size(); i++)
		{
			TreatSingularEdge(ses->singular_edges[i],faces,tree,ses,radius_of_probe);
		}
	}


	template <class T>
	list< TSESFace<T>* > GetFirstCathegoryFaces(const list< TSESFace<T>* >& singular_faces)
	{
		list< TSESFace<T>* > back;
		list< TSESFace<T>* > singular(singular_faces);
		while (singular.size() > 0)
		{
			TSESFace<T>* current = *singular.begin();
			singular.remove(current);
			list< TSESFace<T>* >::iterator i = singular.begin();
			while (i != singular.end())
			{
				if (current->rsface->similar(*((*i)->rsface)))
				{
					back.push_back(current);
					back.push_back(*i);
					singular.remove(*i);
					i = singular.end();
				}
				else
				{
					i++;
				}
			}
		}
		return back;
	}


	template <class T>
	void NoCut(TSESFace<T>* face1, TSESFace<T>* face2, const T& radius_of_probe,
						 TSolventExcludedSurface<T>* ses)
	{
		TCircle3<T> circle;
		TSphere3<T> s1(face1->rsface->getCenter(),radius_of_probe);
		TSphere3<T> s2(face2->rsface->getCenter(),radius_of_probe);
		GetIntersection(s1,s2,circle);
		TSESEdge<T>* edge = new TSESEdge<T>(NULL,NULL,face1,face2,circle,NULL,ses->edges.size());
		ses->edges.push_back(edge);
		face1->edge.push_back(edge);
		face2->edge.push_back(edge);
		if (Maths::isGreater(circle.n*face1->rsface->getCenter(),circle.n*circle.p))
		{
			face1->orientation.push_back(0);
			face2->orientation.push_back(1);
		}
		else
		{
			face1->orientation.push_back(1);
			face2->orientation.push_back(0);
		}
	}


	template <class T>
	void TwoCuts(TSESFace<T>* face1, TSESFace<T>* face2, const T& radius_of_probe,
							 TSolventExcludedSurface<T>* ses)
	{
		TSphere3<T> s1(face1->rsface->getCenter(),radius_of_probe);
		TSphere3<T> s2(face2->rsface->getCenter(),radius_of_probe);
		TCircle3<T> circle;
		GetIntersection(s1,s2,circle);
		TSESEdge<T>* triangular_edge1    = new TSESEdge<T>(NULL,NULL,face1,NULL,circle,NULL,-1);
		TSESEdge<T>* triangular_edge2    = new TSESEdge<T>(NULL,NULL,face2,NULL,circle,NULL,-1);
		TSESEdge<T>* quadrilateral_edge1 = new TSESEdge<T>(NULL,NULL,face1,NULL,circle,NULL,-1);
		TSESEdge<T>* quadrilateral_edge2 = new TSESEdge<T>(NULL,NULL,face2,NULL,circle,NULL,-1);
		BuildTwoEdges(face1,ses,triangular_edge1,quadrilateral_edge1);
		BuildTwoEdges(face2,ses,triangular_edge2,quadrilateral_edge2);
		triangular_edge1->face2 = face2;
		face2->substituteEdge(triangular_edge2,triangular_edge1);
		delete triangular_edge2;
		quadrilateral_edge1->face2 = face2;
		face2->substituteEdge(quadrilateral_edge2,quadrilateral_edge1);
		delete quadrilateral_edge2;
		triangular_edge1->index = ses->edges.size();
		ses->edges.push_back(triangular_edge1);
		quadrilateral_edge1->index = ses->edges.size();
		ses->edges.push_back(quadrilateral_edge1);
	}


	template <class T>
	void ThreeCuts(TSESFace<T>* face1, TSESFace<T>* face2, const T& radius_of_probe,
								 TSolventExcludedSurface<T>* ses)
	{
		TSphere3<T> s1(face1->rsface->getCenter(),radius_of_probe);
		TSphere3<T> s2(face2->rsface->getCenter(),radius_of_probe);
		TCircle3<T> circle;
		GetIntersection(s1,s2,circle);
		vector< TSESEdge<T>* > triangular_edges1(3);
		vector< TSESEdge<T>* > triangular_edges2(3);
		for (Position i = 0; i < 3; i++)
		{
			triangular_edges1[i] = new TSESEdge<T>(NULL,NULL,face1,NULL,circle,NULL,-1);
			triangular_edges2[i] = new TSESEdge<T>(NULL,NULL,face2,NULL,circle,NULL,-1);
		}
		BuildThreeEdges(face1,radius_of_probe,ses,triangular_edges1);
		BuildThreeEdges(face2,radius_of_probe,ses,triangular_edges2);
		for (Position i = 0; i < 3; i++)
		{
			triangular_edges1[i]->index = ses->edges.size();
			ses->edges.push_back(triangular_edges1[i]);
			for (Position j = 0; j < 3; j++)
			{
				if (triangular_edges1[i]->similar(*triangular_edges2[j]))
				{
					triangular_edges1[i]->face2 = face2;
					face2->substituteEdge(triangular_edges2[j],triangular_edges1[i]);
					delete triangular_edges2[j];
				}
			}
		}
	}


	template <class T>
	void BuildTwoEdges(TSESFace<T>* face, TSolventExcludedSurface<T>* ses,
										 TSESEdge<T>* triangular_edge, TSESEdge<T>* quadrilateral_edge)
	{
		vector< list< TSESVertex<T>* > > ipnv(3);		// ipnv stands for intersection_point_near_vertex
		list< TSESVertex<T>* > singular_points;
		for (Position i = 3; i < 7; i++)
		{
			for (Position j = 0; j < 3; j++)
			{
				if (face->vertex[i]->atom == face->vertex[j]->atom)
				{
					ipnv[j].push_back(face->vertex[i]);
				}
			}
		}
		for (Position i = 0; i < 3; i++)
		{
			if (ipnv[i].size() == 1)
			{
				singular_points.push_back(ipnv[i].front());
			}
			else		// face1_ipnv[i].size() == 2
			{
				triangular_edge->vertex1 = ipnv[i].front();
				triangular_edge->vertex2 = ipnv[i].back();
			}
		}
		face->edge.push_back(triangular_edge);
		quadrilateral_edge->vertex1 = ses->vertices[singular_points.front()->index];
		quadrilateral_edge->vertex2 = ses->vertices[singular_points.back()->index];
		face->edge.push_back(quadrilateral_edge);
		if (Maths::isGreater(triangular_edge->circle.n*face->rsface->getCenter(),
												 triangular_edge->circle.n*triangular_edge->circle.p))
		{
			face->orientation.push_back(0);
			face->orientation.push_back(0);
		}
		else
		{
			face->orientation.push_back(1);
			face->orientation.push_back(1);
		}
	}


	template <class T>
	void BuildThreeEdges(TSESFace<T>* face, const T& radius_of_probe, TSolventExcludedSurface<T>* ses,
											 vector< TSESEdge<T>* > triangular_edge)
	{
		vector< list< TSESVertex<T>* > > ipnv(3);		// ipnv stands for intersection_point_near_vertex
		for (Position i = 3; i < 9; i++)
		{
			for (Position j = 0; j < 3; j++)
			{
				if (face->vertex[i]->atom == face->vertex[j]->atom)
				{
					ipnv[j].push_back(face->vertex[i]);
				}
			}
		}
		for (Position i = 0; i < 3; i++)
		{
			triangular_edge[i]->vertex1 = ipnv[i].front();
			triangular_edge[i]->vertex2 = ipnv[i].back();
			face->edge.push_back(triangular_edge[i]);
		}
		if (Maths::isGreater(triangular_edge[0]->circle.n*face->rsface->getCenter(),
												 triangular_edge[0]->circle.n*triangular_edge[0]->circle.p))
		{
			face->orientation.push_back(0);
			face->orientation.push_back(0);
			face->orientation.push_back(0);
		}
		else
		{
			face->orientation.push_back(1);
			face->orientation.push_back(1);
			face->orientation.push_back(1);
		}
	}


	template <class T>
	void TreatSingularEdge(TSESEdge<T>* edge, vector< TSESFace<T>* > faces, TBSDTree<T>* tree,
												 TSolventExcludedSurface<T>* ses, const T& radius_of_probe)
	{
//cout << *edge << "\n";
		list<Index> candidates = tree->get(edge->circle.p,edge->circle.radius+radius_of_probe);
		if (candidates.size() == 0)
		{
			return;
		}
		TVector3<T> normal((edge->vertex1->p-edge->circle.p)%(edge->vertex2->p-edge->circle.p));
		TAngle<T> phi(getOrientedAngle(edge->vertex1->p-edge->circle.p,
																	 edge->vertex2->p-edge->circle.p, normal));
		TAngle<T> min_phi(Constants::PI,true);
		TAngle<T> max_phi(-Constants::PI,true);
		TVector3<T> min_point;
		TVector3<T> max_point;
		TSphere3<T> min_probe;
		TSphere3<T> max_probe;
		Position min = -1;
		Position max = -1;
		list<Index>::iterator i;
		for (i = candidates.begin(); i != candidates.end(); i++)
		{
//cout << *i << ": ";
			TVector3<T> p1;
			TVector3<T> p2;
			TAngle<T> phi1;
			TAngle<T> phi2;
			TSphere3<T> probe(faces[*i]->rsface->getCenter(),radius_of_probe);
			if (GetIntersectionPointsAndAngles(edge,probe,normal,p1,phi1,p2,phi2))
			{
				if (phi1 < min_phi)
				{
					min_phi = phi1;
					min_point = p1;
					min_probe = probe;
					min = *i;
				}
				if (phi2 > max_phi)
				{
					max_phi = phi2;
					max_point = p2;
					max_probe = probe;
					max = *i;
				}
			}
		}
//cout << min << ":  " << min_probe << "  " << min_phi << "  " << min_point << "\n";
//cout << max << ":  " << max_probe << "  " << max_phi << "  " << max_point << "\n";
		if (min_phi > max_phi)
		{
			return;
		}
		TSESVertex<T>* ns1;
		TSESVertex<T>* ns2;
		TSESEdge<T>* a(NULL);
		TSESEdge<T>* na(NULL);
		if (min_phi == TAngle<T>(0,true))
		{
			ns1 = edge->vertex1;
		}
		else
		{
			ns1 = new TSESVertex<T>(min_point,-2,-1);
			a = new TSESEdge<T>(*edge);
			a->vertex2 = ns1;
		}
		if (max_phi == phi)
		{
			ns2 = edge->vertex2;
		}
		else
		{
			ns2 = new TSESVertex<T>(max_point,-2,-1);
			na = new TSESEdge<T>(*edge);
			na->vertex1 = ns2;
		}
		TSphere3<T> probe1(edge->face1->rsface->getCenter(),radius_of_probe);
		TSphere3<T> probe2(edge->face2->rsface->getCenter(),radius_of_probe);
		TCircle3<T> circle;
		TSESEdge<T>* a1(NULL);
		TSESEdge<T>* a2(NULL);
		TSESEdge<T>* a3(NULL);
		TSESEdge<T>* a4(NULL);
		if (edge->face1->isNeighbouredTo(faces[min]) == false)
		{
			GetIntersection(probe1,min_probe,circle);
			a1 = new TSESEdge<T>(ns1,NULL,edge->face1,faces[min],circle,NULL,-1);
		}
		if (edge->face2->isNeighbouredTo(faces[min]) == false)
		{
			GetIntersection(probe2,min_probe,circle);
			a2 = new TSESEdge<T>(ns1,NULL,edge->face2,faces[min],circle,NULL,-1);
		}
		if (edge->face1->isNeighbouredTo(faces[max]) == false)
		{
			GetIntersection(probe1,max_probe,circle);
			a3 = new TSESEdge<T>(ns2,NULL,edge->face1,faces[max],circle,NULL,-1);
		}
		if (edge->face2->isNeighbouredTo(faces[max]) == false)
		{
			GetIntersection(probe2,max_probe,circle);
			a4 = new TSESEdge<T>(ns2,NULL,edge->face2,faces[max],circle,NULL,-1);
		}
		EndEdges(a1,a3,probe1,min_probe,max_probe);
		EndEdges(a2,a4,probe2,min_probe,max_probe);
		if (a != NULL)
		{
			a->index = ses->edges.size();
			ses->edges.push_back(a);
			a->face1->edge.push_back(a);
			a->face1->orientation.push_back(a->face1->orientation[a->face1->getRelativeEdgeIndex(edge->index)]);
			a->face2->edge.push_back(a);
			a->face2->orientation.push_back(a->face2->orientation[a->face2->getRelativeEdgeIndex(edge->index)]);
		}
		if (na != NULL)
		{
			na->index = ses->edges.size();
			ses->edges.push_back(na);
			na->face1->edge.push_back(na);
			na->face1->orientation.push_back(na->face1->orientation[na->face1->getRelativeEdgeIndex(edge->index)]);
			na->face2->edge.push_back(na);
			na->face2->orientation.push_back(na->face2->orientation[na->face2->getRelativeEdgeIndex(edge->index)]);
		}
		if (a1 != NULL)
		{
			a1->index = ses->edges.size();
			ses->edges.push_back(a1);
//				ses->singular_edges.push_back(a1);
			a1->face1->edge.push_back(a1);
			a1->face1->orientation.push_back(0);
			a1->face2->edge.push_back(a1);
			a1->face2->orientation.push_back(1);
		}
		if (a2 != NULL)
		{
			a2->index = ses->edges.size();
			ses->edges.push_back(a2);
//				ses->singular_edges.push_back(a2);
			a2->face1->edge.push_back(a2);
			a2->face1->orientation.push_back(0);
			a2->face2->edge.push_back(a2);
			a2->face2->orientation.push_back(1);
		}
		if (a3 != NULL)
		{
			a3->index = ses->edges.size();
			ses->edges.push_back(a3);
//				ses->singular_edges.push_back(a3);
			a3->face1->edge.push_back(a3);
			a3->face1->orientation.push_back(0);
			a3->face2->edge.push_back(a3);
			a3->face2->orientation.push_back(1);
		}
		if (a4 != NULL)
		{
			a4->index = ses->edges.size();
			ses->edges.push_back(a4);
//				ses->singular_edges.push_back(a4);
			a4->face1->edge.push_back(a4);
			a4->face1->orientation.push_back(0);
			a4->face2->edge.push_back(a4);
			a4->face2->orientation.push_back(1);
		}
		edge->face1->edge[edge->face1->getRelativeEdgeIndex(edge->index)] = NULL;
		edge->face2->edge[edge->face2->getRelativeEdgeIndex(edge->index)] = NULL;
		ses->edges[edge->index] = NULL;
		delete edge;
		ns1->index = ses->vertices.size();
		ses->vertices.push_back(ns1);
		ns2->index = ses->vertices.size();
		ses->vertices.push_back(ns2);
	}


	template <class T>
	bool GetIntersectionPointsAndAngles(TSESEdge<T>* edge, const TSphere3<T>& probe,
																			const TVector3<T>& normal,
																			TVector3<T>& p1, TAngle<T>& phi1,
																			TVector3<T>& p2, TAngle<T>& phi2)
	{
		if (GetIntersection(probe,edge->circle,p1,p2) == true)
		{
			phi1 = getOrientedAngle(edge->vertex1->p-edge->circle.p,p1-edge->circle.p,normal);
			phi2 = getOrientedAngle(edge->vertex1->p-edge->circle.p,p2-edge->circle.p,normal);
			if (phi2 < phi1)
			{
				TAngle<T> phi(phi1);
				phi1 = phi2;
				phi2 = phi;
				TVector3<T> p(p1);
				p1 = p2;
				p2 = p;
			}
			return true;
		}
		else
		{
			return false;
		}
	}


	template <class T>
	void EndEdges(TSESEdge<T>* edge1, TSESEdge<T>* edge2, const TSphere3<T>& probe,
								const TSphere3<T>& probe1, const TSphere3<T>& probe2)
	{
		if (edge1 == NULL)
		{
			if (edge2 == NULL)
			{
				return;
			}
			TVector3<T> p1;
			TVector3<T> p2;
			GetIntersection(probe1,edge2->circle,p1,p2);
			TVector3<T> test_point = edge2->face1->vertex[0]->p;
			if (Maths::isLess((test_point*edge2->circle.n),(edge2->circle.p*edge2->circle.n)))
			{
				edge2->circle.n.negate();
			}
			return;
		}
		if (edge2 == NULL)
		{
			TVector3<T> p1;
			TVector3<T> p2;
			GetIntersection(probe1,edge1->circle,p1,p2);
			TVector3<T> test_point = edge1->face1->vertex[0]->p;
			if (Maths::isLess((test_point*edge1->circle.n),(edge1->circle.p*edge1->circle.n)))
			{
				edge1->circle.n.negate();
			}
			return;
		}
		TVector3<T> p1;
		TVector3<T> p2;
		GetIntersection(edge1->circle,edge2->circle,p1,p2);
		TVector3<T> test_point = edge1->face1->vertex[0]->p;
		if (Maths::isLess((test_point*edge1->circle.n),(edge1->circle.p*edge1->circle.n)))
		{
			edge1->circle.n.negate();
		}
		test_point = edge2->face1->vertex[0]->p;
		if (Maths::isLess((test_point*edge2->circle.n),(edge2->circle.p*edge2->circle.n)))
		{
			edge2->circle.n.negate();
		}
		return;
	}


	template <class T>
	bool GetIntersection(const TSphere3<T>& sphere, const TCircle3<T>& circle,
											 TVector3<T>& p1, TVector3<T>& p2)
	{
		TSphere3<T> s2(circle.p,circle.radius);
		TCircle3<T> intersection_circle;
		if (GetIntersection(sphere,s2,intersection_circle) == false)
		{
//cout << "#1\n";
			return false;
		}
		TPlane3<T> plane1(circle.p,circle.n);
		TPlane3<T> plane2(intersection_circle.p,intersection_circle.n);
		TLine3<T> line;
		if (GetIntersection(plane1,plane2,line) == false)
		{
//cout << "#2\n";
			return false;
		}
		T a(line.d.getSquareLength());
		T b(line.d*(circle.p-line.p)*2);
		T c((circle.p-line.p).getSquareLength()-circle.radius*circle.radius);
		T lambda1;
		T lambda2;
		if (SolveQuadraticEquation(a,b,c,lambda1,lambda2) == 0)
		{
//cout << "#3\n";
			return false;
		}
		p1 = line.p+lambda1*line.d;
		p2 = line.p+lambda2*line.d;
//cout << sphere << " * " << circle << "  =  [" << p1 << "," << p2 << "\n";
		return true;
	}


	template <class T>
	bool GetIntersection(const TCircle3<T>& circle1, const TCircle3<T>& circle2,
											 TVector3<T>& p1, TVector3<T>& p2)
	{
		if (circle1 == circle2)
		{
			return false;
		}
		TPlane3<T> plane1(circle1.p,circle1.n);
		TPlane3<T> plane2(circle2.p,circle2.n);
		TLine3<T> line;
		if (GetIntersection(plane1,plane2,line) == true)
		{
			TSphere3<T> sphere(circle1.p,circle1.radius);
			if (GetIntersection(sphere,line,p1,p2) == false)
			{
				return false;
			}
			else
			{
				return (circle1.has(p1,true) && circle1.has(p2,true) &&
								circle2.has(p1,true) && circle2.has(p2,true));
			}
		}
		if (plane1.has(plane2.p))
		{
			TVector3<T> norm((circle2.p-circle1.p)*2);
			TPlane3<T> plane3(norm.x,norm.y,norm.z,
												circle1.p.getSquareLength()-circle2.p.getSquareLength()
												-circle1.radius+circle2.radius);
			GetIntersection(plane1,plane3,line);
			TSphere3<T> sphere(circle1.p,circle1.radius);
			if (GetIntersection(sphere,line,p1,p2) == false)
			{
				return false;
			}
			else
			{
				return (circle1.has(p1,true) && circle1.has(p2,true) &&
								circle2.has(p1,true) && circle2.has(p2,true));
			}
		}
		else
		{
			return false;
		}
	}


/***********************************************************************************************************/
/*	
	void ThreeCuts(TSESFace<T>* face1, TSESFace<T>* face2, const TReducedSurface<T>& rs)
	{
		vector<Index> atom(3,-1);
		for (Position i = 0; i < 3; i++)
			{
				atom[i] = face1->vertex[i]->atom;
			}
		vector< TRSEdge<T>* > face1_edges(3);
		vector< TRSEdge<T>* > face2_edges(3);
		Index edge_index;
		TRSFace<T>* rsface1(face1->rsface);
		TRSFace<T>* rsface2(face2->rsface);
		rsface1->getEdge(atom[0],atom[1],edge_index);
		face1_edges[0] = rs->edges[edge_index];
		rsface2->getEdge(atom[0],atom[1],edge_index);
		face2_edges[0] = rs->edges[edge_index];
		rsface1->getEdge(atom[1],atom[2],edge_index);
		face1_edges[1] = rs->edges[edge_index];
		rsface2->getEdge(atom[1],atom[2],edge_index);
		face2_edges[1] = rs->edges[edge_index];
		rsface1->getEdge(atom[2],atom[0],edge_index);
		face1_edges[2] = rs->edges[edge_index];
		rsface2->getEdge(atom[2],atom[0],edge_index);
		face2_edges[2] = rs->edges[edge_index];
		for (Index i = 0; i < 3; i++)
			{
				if (face1_edges[i] == face2_edges[i])
					{
						Position index = face1_edges[i]->index;
						TSESEdge<T>* sesedge1 = ses->toric_faces[index]->edge[2];
						TSESEdge<T>* sesedge2 = ses->toric_faces[index]->edge[3];
						ses->contact_faces[face1_edges[i]->vertex[0]]->edges.remove(sesedge1);
						ses->contact_faces[face1_edges[i]->vertex[1]]->edges.remove(sesedge1);
						ses->contact_faces[face1_edges[i]->vertex[0]]->edges.remove(sesedge2);
						ses->contact_faces[face1_edges[i]->vertex[1]]->edges.remove(sesedge2);
						delete ses->toric_faces[index];
						ses->toric_faces[index] = NULL;
						delete rs->edges[index];
						rs->edges[index] = NULL;
					}
					else
					{
						TRSEdge<T>* new_rsedge = new TRSEdge(face1_edges[i]);
						TSESFace<T>* neighbour_face1 = face1_edges[i]->other(rsface1);
						TSESFace<T>* neighbour_face2 = face2_edges[i]->other(rsface2);
						new_rsedge->face[0] = neighbour_face1;
						new_rsedge->face[1] = neighbour_face2;
						Position v1 = new_rsedge->vertex[0];
						Position v2 = new_rsedge->vertex[1];
						TVector3<T> norm(rs->atom[v1]->atom.p-rs->atom[v2]->atom.p);
						TVector3<T> test_vector = rsface1->normal%norm;
						if (Maths::isLess(test_vector*rs->atom[rsface1->thirdVertexIndex(v1,v2)]->atom.p,
															test_vector*rs->atom[v1]->atom.p))
							{
								norm.negate();
							}
						new_rsedge->phi = getOrientedAngle(neighbour_face1->center-new_rsedge->center_of_torus,
																							 neighbour_face2->center-new_rsedge->center_of_torus,
																							 norm);
						neighbour_face1->edge.remove(face1_edges[i]);
						neighbour_face2->edge.remove(face2_edges[i]);
						neighbour_face1->edge.push_back(new_rsedge);
						neighbour_face2->edge.push_back(new_rsedge);
						rs->atom[v1]->edges.remove(face1_edges[i]);
						rs->atom[v1]->edges.remove(face2_edges[i]);
						rs->atom[v1]->faces.remove(rsface1);
						rs->atom[v1]->faces.remove(rsface2);
						rs->atom[v2]->edges.remove(face1_edges[i]);
						rs->atom[v2]->edges.remove(face2_edges[i]);
						rs->atom[v2]->faces.remove(rsface1);
						rs->atom[v2]->faces.remove(rsface2);
						rs->atom[v1]->edges.push_back(new_rsedge);
						rs->atom[v2]->edges.push_back(new_rsedge);

						Position index = face1_edges[i]->index;
						TSESEdge<T>* sesedge1 = ses->toric_faces[index]->edge[2];
						TSESEdge<T>* sesedge2 = ses->toric_faces[index]->edge[3];
						ses->contact_faces[v1]->edges.remove(sesedge1);
						ses->contact_faces[v2]->edges.remove(sesedge1);
						ses->contact_faces[v1]->edges.remove(sesedge2);
						ses->contact_faces[v2]->edges.remove(sesedge2);
						delete ses->toric_faces[index];
						ses->toric_faces[index] = NULL;
						Position index = face2_edges[i]->index;
						TSESEdge<T>* sesedge1 = ses->toric_faces[index]->edge[2];
						TSESEdge<T>* sesedge2 = ses->toric_faces[index]->edge[3];
						ses->contact_faces[v1]->edges.remove(sesedge1);
						ses->contact_faces[v2]->edges.remove(sesedge1);
						ses->contact_faces[v1]->edges.remove(sesedge2);
						ses->contact_faces[v2]->edges.remove(sesedge2);
						delete ses->toric_faces[index];
						ses->toric_faces[index] = NULL;

						ses->toric_faces[index] = new TSESFace<T>();
						ses->spheric_faces[rsface1->index]->getEdge(v1,v2,edge_index);
						ses->toric_faces[index]->edge.push_back(ses->edges[edge_index]);
						ses->toric_faces[index]->vertex.push_back(ses->edges[edge_index]->vertex1);
						ses->toric_faces[index]->vertex.push_back(ses->edges[edge_index]->vertex2);
						ses->spheric_faces[rsface2->index]->getEdge(v1,v2,edge_index);
						ses->toric_faces[index]->edge.push_back(ses->edges[edge_index]);
						ses->toric_faces[index]->vertex.push_back(ses->edges[edge_index]->vertex1);
						ses->toric_faces[index]->vertex.push_back(ses->edges[edge_index]->vertex2);
						ses->createToricFace(index,radius_of_probe,rs);;
					}
			}
		Position index = rsface1->index;
		delete rsface1;
		rs->faces[index] = NULL;
		Position index = rsface2->index;
		delete rsface2;
		rs->faces[index] = NULL;
		Position index1 = face1->index;
		Position index2 = face2->index;
		delete face1;
		delete face2;
		ses->spheric_faces[index1] = NULL;
		ses->spheric_faces[index2] = NULL;
	}
*/

}

#endif		// BALL_STRUCTURE_SINGULARITIES_H
