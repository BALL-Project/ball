// $Id: triangulation.h,v 1.2 2000/10/11 08:18:28 oliver Exp $

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

#include <vector>
#include <list>
#include <set>
#include <fstream>


namespace BALL
{

	/**
	*/
	template <class T>
	TTriangulatedSurface<T>& Triangulate(TSolventExcludedSurface<T>* ses, TReducedSurface<T>* rs,
																			 const T& size)
	{
#ifdef debug_tri		
ofstream print("Triangulation.log");
#else
ofstream print;
#endif
//cout << "$1\n";
		T radius_of_probe(rs->getProbeRadius());
		vector< TTriangulatedSurface<T>* > triangulated_toric_faces(ses->toric_faces.size(),NULL);
		vector< list< TTriangulatedSurface<T>::Point* > > toric_borders(ses->edges.size());
		for (Position i = 0; i < ses->toric_faces.size(); i++)
			{
				if (ses->toric_faces[i] != NULL)
					{
						triangulated_toric_faces[i] = new TTriangulatedSurface<T>();
						TriangulateToricFace_(ses->toric_faces[i],ses,radius_of_probe,size,
																	*triangulated_toric_faces[i],toric_borders);
#ifdef debug_tri
triangulated_toric_faces[i]->setIndices();
#endif
					}
			}
		vector< TTriangulatedSurface<T>* > triangulated_free_toric_faces(ses->free_toric_faces.size());
//cout << "$2\n";
		for (Position i = 0; i < ses->free_toric_faces.size(); i++)
			{
				triangulated_free_toric_faces[i] = new TTriangulatedSurface<T>();
				TriangulateFreeToricFace(ses->free_toric_faces[i],radius_of_probe,size,
																 *triangulated_free_toric_faces[i],toric_borders);
			}
		std::vector< TTriangulatedSurface<T>* >	triangulated_contact_faces(ses->contact_faces.size());
		std::vector< std::set< TTriangulatedSurface<T>::Edge* > > contact_borders(ses->contact_faces.size());
//cout << "$3\n";
		for (Position i = 0; i < ses->contact_faces.size(); i++)
			{
//cout << "... " << i << "\n";
				TSphere3<T> atom(rs->getSphere(ses->contact_faces[i]->rsvertex->getAtom()));
				triangulated_contact_faces[i] = TriangulatedSphere(atom,size);
#ifdef debug_tri
triangulated_contact_faces[i]->setIndices();
#endif
				TriangulateSphericFace(ses->contact_faces[i],atom.radius,size,
															 *triangulated_contact_faces[i],contact_borders[i],print);
			}
		std::vector< TTriangulatedSurface<T>* >	triangulated_spheric_faces(ses->spheric_faces.size());
		std::vector< std::set< TTriangulatedSurface<T>::Edge* > > spheric_borders(ses->spheric_faces.size());
//cout << "$4\n";
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
			{
				TSphere3<T> sphere = TSphere3<T>(ses->spheric_faces[i]->rsface->getCenter(),radius_of_probe);
				triangulated_spheric_faces[i] = TriangulatedSphere(sphere,size);
				list< TTriangulatedSurface<T>::Point* >::iterator p;
				for (p = triangulated_spheric_faces[i]->points.begin();
						 p != triangulated_spheric_faces[i]->points.end();   p++)
				{
					(*p)->n.negate();
				}
#ifdef debug_tri
triangulated_spheric_faces[i]->setIndices();
#endif
				TriangulateSphericFace(ses->spheric_faces[i],radius_of_probe,size,
															 *triangulated_spheric_faces[i],spheric_borders[i],print);
			}
		TTriangulatedSurface<T>* surface = new TTriangulatedSurface<T>();
//cout << "$5\n";
		for (Position i = 0; i < triangulated_toric_faces.size(); i++)
			{
				if (triangulated_toric_faces[i] != NULL)
					{
						*surface += (*triangulated_toric_faces[i]);
					}
			}
//cout << "$6\n";
		for (Position i = 0; i < triangulated_free_toric_faces.size(); i++)
			{
				*surface += (*triangulated_free_toric_faces[i]);
			}
//cout << "$7\n";
		for (Position i = 0; i < triangulated_contact_faces.size(); i++)
			{
				*surface += (*triangulated_contact_faces[i]);
			}
//cout << "$8\n";
		for (Position i = 0; i < triangulated_spheric_faces.size(); i++)
			{
				*surface += (*triangulated_spheric_faces[i]);
			}
//cout << "$9\n";
#ifdef debug_tri
surface->setIndices();
for (Position i = 0; i < toric_borders.size(); i++)
{
//	cout << i << ":   ";
//	cout << toric_borders[i].size() << "\n";
/*	cout << toric_borders[i].front()->p << " :";
	list< TTriangulatedSurface<T>::Point* >::iterator j;
	for (j = toric_borders[i].begin(); j != toric_borders[i].end(); j++)
	{
		cout << (*j)->index << "   ";
	}
	cout << ": " << toric_borders[i].back()->p;
	cout << "\n";*/
}
#endif
//cout << "$10\n";
/*		for (Position i = 0; i < triangulated_contact_faces.size(); i++)
			{
				list< TTriangulatedSurface<T>::Point* > contour_out;
				GetContour(toric_borders,ses->contact_faces[i],contour_out,print);
				list< TTriangulatedSurface<T>::Point* > contour_in;
				SortContour(contact_borders[i],*(contour_out.begin()),*(++(contour_out.begin())),
										contour_in,(T)0);
				list< TTriangulatedSurface<T>::Point* >::iterator in = contour_in.begin();
				list< TTriangulatedSurface<T>::Point* >::iterator out = contour_out.begin();
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
					}
			}*/
#ifdef debug_tri
//print.close();
#endif
		return *surface;
	}


	template <class T>
	void TriangulateSphericFace(TSESFace<T>* face, const T& radius, const T& size,
															TTriangulatedSurface<T>& surface,
															std::set< typename TTriangulatedSurface<T>::Edge* >& border,
															std::ofstream& print)
	{
#ifdef debug_tri1
print << surface;
#endif
//		std::set< TTriangulatedSurface<T>::Point* > border_points;
		for (Position i = 0; i < face->edge.size(); i++)
		{
			if (face->edge[i] != NULL)
			{
				TVector3<T> norm = face->edge[i]->circle.n;
				if (face->orientation[i] == 1)
					{
						norm.negate();
					}
#ifdef debug_tri1
print << "\nNormalenvektor: " << norm << "\n";
#endif
				T test_value = norm*face->edge[i]->circle.p;
#ifdef debug_tri1
print << "Testvalue: " << norm << "*" << face->edge[i]->circle.p << " = " << test_value << "\n";
#endif
				std::set< TTriangulatedSurface<T>::Point* > wrong_points;
				std::set< TTriangulatedSurface<T>::Edge* > wrong_edges;
				std::set< TTriangulatedSurface<T>::Triangle* > wrong_triangles;
				std::list< TTriangulatedSurface<T>::Triangle* >::iterator t = surface.triangles.begin();
				while (t != surface.triangles.end())
					{
#ifdef debug_tri1
print << "aktuelles Dreieck: " << (*t)->index << "\n";
#endif
						list<TTriangulatedSurface<T>::Edge*>::iterator e;
						bool del = false;
						if ((*t)->point[0] == NULL)
							{
#ifdef debug_tri1
print << "  kein Punkt 0\n";
#endif
								del = true;
							}
							else
							{
								if (Maths::isLess(norm*(*t)->point[0]->p,test_value))
									{
#ifdef debug_tri1
print << "  Punkt[" << (*t)->point[0]->index << "] falsch\n";
#endif
										del = true;
										wrong_points.insert((*t)->point[0]);
										for (e = (*t)->point[0]->edge.begin(); e != (*t)->point[0]->edge.end(); e++)
											{
#ifdef debug_tri1
print << "    Edge " << (*e)->index << " falsch\n";
#endif
												wrong_edges.insert(*e);
											}
									}
							}
						if ((*t)->point[1] == NULL)
							{
#ifdef debug_tri1
print << "  kein Punkt 1\n";
#endif
								del = true;
							}
							else
							{
								if (Maths::isLess(norm*(*t)->point[1]->p,test_value))
									{
#ifdef debug_tri1
print << "  Punkt[" << (*t)->point[1]->index << "] falsch\n";
#endif
										del = true;
										for (e = (*t)->point[1]->edge.begin(); e != (*t)->point[1]->edge.end(); e++)
											{
#ifdef debug_tri1
print << "    Edge " << (*e)->index << " falsch\n";
#endif
												wrong_edges.insert(*e);
											}
										wrong_points.insert((*t)->point[1]);
									}
							}
						if ((*t)->point[2] == NULL)
							{
#ifdef debug_tri1
print << "  kein Punkt 1\n";
#endif
								del = true;
							}
							else
							{
								if (Maths::isLess(norm*(*t)->point[2]->p,test_value))
									{
#ifdef debug_tri1
print << "  Punkt[" << (*t)->point[2]->index << "] falsch\n";
#endif
										del = true;
										for (e = (*t)->point[2]->edge.begin(); e != (*t)->point[2]->edge.end(); e++)
											{
#ifdef debug_tri1
print << "    Edge " << (*e)->index << " falsch\n";
#endif
												wrong_edges.insert(*e);
											}
										wrong_points.insert((*t)->point[2]);
									}
							}
						if (del == true)
							{
#ifdef debug_tri1
print << "  Dreieck " << (*t)->index << " flasch\n";
#endif
								wrong_triangles.insert(*t);
							}
						t++;
					}
				std::set<TTriangulatedSurface<T>::Edge*>::iterator e;
				for (e = wrong_edges.begin(); e != wrong_edges.end(); e++)
					{
#ifdef debug_tri1
print << "Edge[" << (*e)->index << "] aus Punkt[" << (*e)->point[0]->index << "] gelöscht\n";
print << "Edge[" << (*e)->index << "] aus Punkt[" << (*e)->point[1]->index << "] gelöscht\n";
#endif
						(*e)->point[0]->edge.remove(*e);
						(*e)->point[1]->edge.remove(*e);
					}
				std::set<TTriangulatedSurface<T>::Triangle*>::iterator t_;
				for (t_ = wrong_triangles.begin(); t_ != wrong_triangles.end(); t_++)
					{
#ifdef debug_tri1
print << "Dreieck[" << (*t_)->index << "] aus Punkt[" << (*t_)->point[0]->index << "] gelöscht\n";
print << "Dreieck[" << (*t_)->index << "] aus Punkt[" << (*t_)->point[1]->index << "] gelöscht\n";
print << "Dreieck[" << (*t_)->index << "] aus Punkt[" << (*t_)->point[2]->index << "] gelöscht\n";
print << "Dreieck[" << (*t_)->index << "] aus Edge[" << (*t_)->edge[0]->index << "] gelöscht\n";
print << "Dreieck[" << (*t_)->index << "] aus Edge[" << (*t_)->edge[1]->index << "] gelöscht\n";
print << "Dreieck[" << (*t_)->index << "] aus Edge[" << (*t_)->edge[2]->index << "] gelöscht\n";
#endif
						(*t_)->point[0]->triangle.remove(*t_);
						(*t_)->point[1]->triangle.remove(*t_);
						(*t_)->point[2]->triangle.remove(*t_);
						(*t_)->edge[0]->del(*t_);
						(*t_)->edge[1]->del(*t_);
						(*t_)->edge[2]->del(*t_);
					}
				while (wrong_points.size() > 0)
					{
						TTriangulatedSurface<T>::Point* point = *wrong_points.begin();
#ifdef debug_tri1
print << "Punkt[" << point->index << "] gelöscht\n";
#endif
						surface.points.remove(point);
//						border_points.erase(point);
						wrong_points.erase(point);
						delete point;
					}
				while (wrong_edges.size() > 0)
					{
						TTriangulatedSurface<T>::Edge* edge = *wrong_edges.begin();
#ifdef debug_tri1
print << "Edge[" << edge->index << "] gelöscht\n";
#endif
						surface.edges.remove(edge);
						wrong_edges.erase(edge);
						delete edge;
					}
				while (wrong_triangles.size() > 0)
					{
						TTriangulatedSurface<T>::Triangle* triangle = *wrong_triangles.begin();
#ifdef debug_tri1
print << "Dreieck[" << triangle->index << "] gelöscht\n";
#endif
						surface.triangles.remove(triangle);
						wrong_triangles.erase(triangle);
						delete triangle;
					}
			}
		}
		while (surface.points.size() > 0)
			{
				while ((*surface.points.begin())->edge.size() > 0)
					{
						(*surface.points.begin())->edge.pop_front();
					}
				surface.points.pop_front();
			}
		surface.edges.erase(surface.edges.begin(),surface.edges.end());
		list<TTriangulatedSurface<T>::Triangle*>::iterator t;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
			{
				surface.points.push_back((*t)->point[0]);
				surface.points.push_back((*t)->point[1]);
				surface.points.push_back((*t)->point[2]);
				surface.edges.push_back((*t)->edge[0]);
				surface.edges.push_back((*t)->edge[1]);
				surface.edges.push_back((*t)->edge[2]);
			}
		surface.points.sort();
		surface.points.unique();
		surface.edges.sort();
		surface.edges.unique();
		list<TTriangulatedSurface<T>::Edge*>::iterator e;
		for (e = surface.edges.begin(); e != surface.edges.end(); e++)
			{
				(*e)->point[0]->edge.push_back(*e);
				(*e)->point[1]->edge.push_back(*e);
			}
		list<TTriangulatedSurface<T>::Point*>::iterator p;
		for (p = surface.points.begin(); p != surface.points.end(); p++)
			{
				(*p)->edge.sort();
				(*p)->edge.unique();
			}
		border.insert(surface.edges.begin(),surface.edges.end());
		std::set< TTriangulatedSurface<T>::Edge* > test_edges;
		for (t = surface.triangles.begin(); t != surface.triangles.end(); t++)
			{
				for (Position i = 0; i < 3; i++)
					{
						if (test_edges.find((*t)->edge[i]) == test_edges.end())
							{
								test_edges.insert((*t)->edge[i]);
							}
							else
							{
								border.erase((*t)->edge[i]);
							}
					}
			}
	}


	template <class T>
	bool TriangulateToricFace_(TSESFace<T>* face, TSolventExcludedSurface<T>* ses,
														const T& radius_of_probe, const T& size,
														TTriangulatedSurface<T>& surface,
														vector< list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
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
		int e;
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
//cout << "Edge0:  " << edge0->index << "\n";
//cout << "Edge1:  " << edge1->index << "\n";
//cout << "Edge2:  " << edge2->index << "\n";
//cout << "Edge3:  " << edge3->index << "\n";
//cout << "Point0: " << p0->index << "\n";
//cout << "Point1: " << p1->index << "\n";
//cout << "Point2: " << p2->index << "\n";
//cout << "Point3: " << p3->index << "\n";
		Size number_of_segments = (int)Maths::round(face->rsedge->getPhi().toRadian()*edge3->circle.radius/size);
		TAngle<T> psi(face->rsedge->getPhi().toRadian()/number_of_segments,true);
		vector< TVector3<T> > edge3_segments;
		vector< TVector3<T> > edge1_segments;
		TVector3<T> normal((p0->p-edge3->circle.p)%(p3->p-edge3->circle.p));
		if (Maths::isGreater(face->rsedge->getPhi().toRadian(),Constants::PI))
			{
				normal.negate();
			}
//cout << face->rsedge->getPhi() << "\n";
		TCircle3<T> circle3(edge3->circle.p,
												normal,
												edge3->circle.radius);
		TCircle3<T> circle1(edge1->circle.p,
												normal,
												edge1->circle.radius);
		edge3_segments = PartitionOfCircle_(circle3,p0->p,psi,number_of_segments);
//		edge3_segments.pop_back();
//		edge3_segments.push_back(p3->p);
		edge1_segments = PartitionOfCircle_(circle1,p1->p,psi,number_of_segments);
//		edge1_segments.pop_back();
//		edge1_segments.push_back(p2->p);
//cout << "Edge3: " << *edge3 << "\n\t" << p0->p << "\n";
for (Position j = 0; j < edge3_segments.size(); j++)
{
//	cout << "\t\t" << edge3_segments[j] << "\n";
}
//cout << "\t" << p3->p << "\n";
//cout << "Edge1: " << *edge1 << "\n\t" << p1->p << "\n";
for (Position j = 0; j < edge1_segments.size(); j++)
{
//	cout << "\t\t" << edge1_segments[j] << "\n";
}
//cout << "\t" << p2->p << "\n";
		TCircle3<T> center_circle(face->rsedge->getCenterOfTorus(),
															normal,
															face->rsedge->getMajorRadiusOfTorus());
		vector< TVector3<T> > centers;
		centers = PartitionOfCircle_(center_circle,edge0->circle.p,psi,number_of_segments);
//		centers.pop_back();
//		centers.push_back(edge2->circle.p);
//cout << "Centers: " << "\n\t" << edge0->circle.p << "\n";
for (Position j = 0; j < centers.size(); j++)
{
//	cout << "\t\t" << centers[j] << "\n";
}
//cout << "\t" << edge2->circle.p << "\n";
		BuildTriangles(face,edge0,edge1,edge2,edge3,centers,edge1_segments,edge3_segments,
									 radius_of_probe,size,surface,border);
		return true;
	}


	template <class T>
	bool BuildTriangles(TSESFace<T>* face,
											TSESEdge<T>* edge0, TSESEdge<T>* edge1, TSESEdge<T>* edge2, TSESEdge<T>* edge3,
											const vector< TVector3<T> >& centers,
											const vector< TVector3<T> >& edge1_points, const vector< TVector3<T> >& edge3_points,
											const T& radius_of_probe, const T& size,
											TTriangulatedSurface<T>& surface,
											vector< list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
		TAngle<T> pi(Constants::PI,true);
		TAngle<T> psi;
		psi = getOrientedAngle(edge1_points[0]-centers[0],edge3_points[0]-centers[0],
													 (edge1_points[0]-centers[0])%(edge3_points[0]-centers[0]))+pi;
		Size number_of_triangles = (int)Maths::round(psi.toRadian()*radius_of_probe/size);
		TAngle<T> phi(psi/number_of_triangles,true);
		Size number_of_segments = centers.size()-1;
		vector< TTriangulatedSurface<T>::Point* > points((number_of_segments+1)*(number_of_triangles+1));
		for (Position i = 0; i < centers.size(); i++)
			{
				TCircle3<T> circle(centers[i],
													 (edge1_points[i]-centers[i])%(edge3_points[i]-centers[i]),
													 radius_of_probe);
				vector< TVector3<T> > line;
				line = PartitionOfCircle_(circle,edge1_points[i],phi,number_of_triangles);
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
		vector< TTriangulatedSurface<T>::Triangle* > triangles(2*number_of_segments*number_of_triangles);
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
	vector< TVector3<T> > PartitionOfCircle_(const TCircle3<T>& circle_, const TVector3<T>& p0,
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
//cout << "\t\t" << circle << "\t" << p0 << "\n";
		TVector4<T> e1;
		if (circle.has(p0,true))
			{
				TVector3<T> e1_(p0-circle.p);
				e1.set(e1_.x,e1_.y,e1_.z,(T)0);
//cout << "#\t" << e1_ << "  " << e1_.getLength() << "  " << circle.radius << "\n";
			}
			else
			{
				e1.set(circle.n.y,-circle.n.x,(T)0,(T)0);
				if (e1 == TVector4<T>::getZero())
					{
						e1.set(circle.n.z,(T)0,-circle.n.x,(T)0);
					}
			}
//cout << e1 << "\n";
		e1.normalize();
//cout << e1 << "\n";
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
//cout << p << "\n";
		p *= circle.radius;
//cout << p << "\n";
		vector< TVector3<T> > back;
		back.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
//cout << p << "\n";
		for (Size i = 0; i < number_of_segments; i++)
			{
				p = rotation*p;
				back.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
//cout << p << "\n";
			}
		return back;
	}
	

	
		
	
	template <class T>
	bool TriangulateToricFace(TSESFace<T>* face, const T& radius_of_probe, const T& size,
														TTriangulatedSurface<T>& surface,
														vector< list< typename TTriangulatedSurface<T>::Point* > >& border)
	{
		Size number_of_segments = (int)(face->rsedge->phi.toRadian()*face->edge[2]->circle.radius/size);
		int index1;
		int index2;
		vector< TVector3<T> > points1;
			TVector3<T> p1 = face->edge[2]->vertex1->p;
			TVector3<T> p2 = face->edge[2]->vertex2->p;
			if ((face->edge[2]->vertex2 == face->edge[0]->vertex1) ||
					(face->edge[2]->vertex2 == face->edge[0]->vertex2))
				{
					p1.swap(p2);
					index1 = face->edge[2]->vertex2->index;
					index2 = face->edge[2]->vertex1->index;
				}
				else
				{
					index1 = face->edge[2]->vertex1->index;
					index2 = face->edge[2]->vertex2->index;
				}
		points1 = PartitionOfCircle(face->edge[2]->circle,p1,p2,number_of_segments);
		int index3;
		int index4;
		vector< TVector3<T> > points2;
			TVector3<T> p3 = face->edge[3]->vertex1->p;
			TVector3<T> p4 = face->edge[3]->vertex2->p;
			if ((face->edge[3]->vertex2 == face->edge[0]->vertex1) ||
					(face->edge[3]->vertex2 == face->edge[0]->vertex2))
				{
					p3.swap(p4);
					index3 = face->edge[3]->vertex2->index;
					index4 = face->edge[3]->vertex1->index;
				}
				else
				{
					index3 = face->edge[3]->vertex1->index;
					index4 = face->edge[3]->vertex2->index;
				}
		points2 = PartitionOfCircle(face->edge[3]->circle,p3,p4,number_of_segments);
		TCircle3<T> maior_circle = TCircle3<T>(face->rsedge->getCenterOfTorus(),
																					 TVector3<T>(0,0,0),
																					 face->rsedge->getMajorRadiusOfTorus());
		int edge1 = face->edge[2]->index;
		int edge2 = face->edge[3]->index;
		int edge3 = face->edge[0]->index;
		int edge4 = face->edge[1]->index;
		vector< TVector3<T> > minor_centers;
			TVector3<T> center1 = face->rsedge->face[0]->center;
			TVector3<T> center2 = face->rsedge->face[1]->center;
			if (Maths::isLess(center2.getDistance(p1),center2.getDistance(p2)))
				{
					center1.swap(center2);
				}
		minor_centers = PartitionOfCircle(maior_circle,center1,center2,number_of_segments);
		TAngle<T> phi;
		GetAngle(face->edge[0]->vertex1->p-center1,face->edge[0]->vertex2->p-center1,phi);
		Size number_of_triangles = (int)(phi.toRadian()*face->edge[0]->circle.radius/size);
		vector< TTriangulatedSurface<T>::Point* > points((number_of_segments+1)*(number_of_triangles+1));
		for (Position i = 0; i < minor_centers.size(); i++)
			{
				TCircle3<T> circle = TCircle3<T>(minor_centers[i],TVector3<T>(0,0,0),radius_of_probe);
				vector< TVector3<T> > line;
				line = PartitionOfCircle(circle,points1[i],points2[i],number_of_triangles);
				for (Position j = 0; j < line.size(); j++)
					{
						TTriangulatedSurface<T>::Point* point = new TTriangulatedSurface<T>::Point();
						point->p = line[j];
						point->index = i*(number_of_triangles+1)+j;
						points[i*(number_of_triangles+1)+j] = point;
						if (j == 0)
							{
								border[edge1].push_back(point);
							}
							else
							{
								if (j == line.size()-1)
									{
										border[edge2].push_back(point);
									}
							}
						if (i == 0)
							{
								border[edge3].push_back(point);
							}
							else
							{
								if (i == minor_centers.size()-1)
									{
										border[edge4].push_back(point);
									}
							}
					}
			}
		vector< TTriangulatedSurface<T>::Triangle* > triangles(2*number_of_segments*number_of_triangles);
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
	bool TriangulateFreeToricFace(TSESFace<T>* face, const T& radius_of_probe, const T& size,
																TTriangulatedSurface<T>& surface,
																vector< list< typename TTriangulatedSurface<T>::Point* > >& border)
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
		points1 = PartitionOfCircle_(circle1,p1,phi,number_of_points);
		points2 = PartitionOfCircle_(circle2,p2,phi,number_of_points);
		centers = PartitionOfCircle_(circle3,p3,phi,number_of_points);
		points1.pop_back();
		points2.pop_back();
		centers.pop_back();
		TSESEdge<T>* dummy(NULL);
		BuildTriangles(face,dummy,face->edge[0],dummy,face->edge[1],centers,points1,points2,
									 radius_of_probe,size,surface,border);
		return true;
	}


	template <class T>
	TTriangulatedSurface<T>* TriangulatedSphere(const TSphere3<T>& sphere, const T& size)
	{
		TTriangulatedSurface<T>* back = new TTriangulatedSurface<T>();
		back->isokaeder(sphere.radius);
		back->refineSphere(sphere.radius,4);
		back->shift(sphere.p);
		return back;
	}


	template <class T>
	vector< TVector3<T> > PartitionOfCircle(const TCircle3<T>& circle, const TVector3<T>& p1,
																					const TVector3<T>& p2, const Size number_of_segments)
	{
		vector< TVector3<T> > back;
		TAngle<T> psi(Constants::PI/(2*number_of_segments),true);
		back.push_back(p1);
//cout << p1 << "  ";
		for (Size i = 1; i < number_of_segments; i++)
			{
				T cosinus = cos(i*psi.toRadian());
				T sinus = sin(i*psi.toRadian());
				TVector3<T> next;
				next = cosinus*(p1-circle.p)+sinus*(p2-circle.p)+circle.p;
				back.push_back(next);
//cout << next << "  ";
			}
		back.push_back(p2);
//cout << p2 << "\n";
		return back;
	}


	template <class T>
	void GetContour(std::vector< list< typename TTriangulatedSurface<T>::Point* > >& border, TSESFace<T>* face,
									std::list< typename TTriangulatedSurface<T>::Point* >& contour, std::ofstream& print)
	{
#ifdef debug_tri2
/*print << "suche Contour von face[" << face->index << "]\n";
for (Position i = 0; i < border.size(); i++)
{
	print << (*(border[i].begin()))->p << " :" << (*(border[i].begin()))->index << "  ";
	list< TTriangulatedSurface<T>::Point* >::iterator j = border[i].begin()++;
	while (j != border[i].end())
	{
		print << (*j)->index << "  ";
		j++;
	}
	print << ": " << (*(--(border[i].end())))->p << "\n";
}*/
#endif
//cout << "$1\n";
		list< list< TTriangulatedSurface<T>::Point* > > contours;
		for (Position i = 0; i < face->edge.size(); i++)
			{
/*				TSESEdge<T>* edge = face->edge[i];
#ifdef debug_tri2
print << "  1. Edge: " << edge->index << "\n";
#endif
				TVector3<T> p1 = edge->vertex1->p;
				TVector3<T> p2 = edge->vertex2->p;
#ifdef debug_tri2
print << "    Punkte: " << edge->vertex1->index << ": " << p1 << ",  "
			<< edge->vertex2->index << ": " << p2 << "\n";
#endif
				int index = edge->other(face)->index;
#ifdef debug_tri2
print << "    zugehörige 2. Face: face[" << index << "]\n";
#endif
				list< TTriangulatedSurface<T>::Point* > border_part = border[index];
				list< TTriangulatedSurface<T>::Point* >::iterator start = border_part.begin();
				list< TTriangulatedSurface<T>::Point* >::iterator end;
#ifdef debug_tri2
print << "    suche nach Punkten in border[" << index << "]:\n";
#endif
				while (((*start)->p != p1) && ((*start)->p != p2))
					{
						start++;
					}
				if (start != border_part.begin())
					{
						start++;
					}
#ifdef debug_tri2
print << "    start: " << (*start)->p << "\n";
#endif
				end = start;
				end++;
				while (((*end)->p != p1) && ((*end)->p != p2))
					{
						end++;
					}
#ifdef debug_tri2
print << "    end: " << (*end)->p << "\n";
#endif
				end++;
				border_part.erase(border_part.begin(),start);
				border_part.erase(end,border_part.end());
#ifdef debug_tri2
print << "  border:  ";
list< TTriangulatedSurface<T>::Point* >::iterator j;
for (j = border_part.begin(); j != border_part.end(); j++)
{
	print << (*j)->index << "  ";
}
print << "\n";
#endif*/
				contours.push_back(border[face->edge[i]->index]);
			}
//cout << "$2\n";
#ifdef debug_tri2
print << "ordne Contouren, fange mit erster gefundener an\n";
/*list< list< TTriangulatedSurface<T>::Point* > >::iterator i1;
for (i1 = contours.begin(); i1 != contours.end(); i1++)
{
	cout << i1->front()->p << " :";
	list< TTriangulatedSurface<T>::Point* >::iterator i2;
	for (i2 = i1->begin(); i2 != i1->end(); i2++)
	{
		cout << (*i2)->index << "  ";
	}
	cout << ": " << i1->back()->p;
	cout << "\n";
}*/
#endif
		contour = *contours.begin();
		contours.pop_front();
//cout << "$3\n";
		while (contours.size() > 0)
			{
//cout << "$3a\n";
				list< list< TTriangulatedSurface<T>::Point* > >::iterator l = contours.begin();
				bool front = false;
				bool back = false;
				TVector3<T> last = contour.back()->p;
//cout << "$3b\n";
				while ((back == false) && (front == false))
					{
						TVector3<T> this_first = l->front()->p;
						TVector3<T> this_last = l->back()->p;
						front = (this_first == last);
						back = (this_last == last);
						l++;
					}
//cout << "$3c\n";
				l--;
//cout << "$3d\n";
				if (back == true)
					{
						(*l).reverse();
					}
#ifdef debug_tri2
print << "Partner gefunden: " << contour.back()->p << " == " << l->front()->p << "\n";
#endif
//cout << "$3e\n";
				list< TTriangulatedSurface<T>::Point* >::iterator p;
//cout << "$3f\n";
				for (p = l->begin(); p != l->end(); p++)
					{
						contour.push_back(*p);
					}
//cout << "$3g\n";
				contours.remove(*l);
//cout << "$3h\n";
			}
//cout << "$4\n";
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


}	// namespace BALL

#endif	// BALL_STRUCTURE_SINGULARITIES_H


