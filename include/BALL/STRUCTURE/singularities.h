// $Id: singularities.h,v 1.15 2001/07/29 17:25:31 oliver Exp $

#ifndef BALL_STRUCTURE_SINGULARITIES_H
#define BALL_STRUCTURE_SINGULARITIES_H

//#define debug_singularities

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

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
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

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
# include <BALL/KERNEL/molecule.h>
#endif

/*
#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_PTE_H
# include <BALL/KERNEL/PTE.h>
#endif


#ifndef BALL_FORMAT_HINFILE_H
# include <BALL/FORMAT/HINFile.h>
#endif
*/

#include <vector>
#include <list>
#include <fstream>

namespace BALL
{

	template <class T>
	void TreatSingularities(TSolventExcludedSurface<T>*& ses,
													TReducedSurface<T>*& rs, const T& radius_of_probe)
	{
		list<TSESFace<T>*> singular_faces;
		GetSingularFaces(ses,singular_faces);
		try
		{
			TreatFirstCategory(ses,rs,singular_faces,radius_of_probe);
		}
		catch (Exception::GeneralException e)
		{
			if (e.getMessage() == "reduced surface modified")
			{
				rs->clean();
						#ifdef debug_singularities
						std::cout << *rs;
						#endif
				delete ses;
				ses = new SolventExcludedSurface(rs);
				ses->get(rs);
				TreatSingularities(ses,rs,radius_of_probe);
				return;
			}
			else
			{
				// BAUSTELLE: add documentation for that fucking exception
				throw Exception::GeneralException(__FILE__, __LINE__, "undocumented exception in TreatSingularities!", "");
			}
		}				
		TreatSecondCategory(ses,radius_of_probe);
	}
	
	
	template <class T>
	void GetSingularFaces(TSolventExcludedSurface<T>* ses,
												list<TSESFace<T>*>& faces)
	{
		for (Position i = 0; i < ses->spheric_faces.size(); i++)
		{
			if (ses->spheric_faces[i]->rsface->isSingular() == true)
			{
				faces.push_back(ses->spheric_faces[i]);
			}
		}
	}


	template <class T>
	void TreatFirstCategory(TSolventExcludedSurface<T>* ses,
													 TReducedSurface<T>* rs,
													 list<TSESFace<T>*>& singular_faces,
													 const T& radius_of_probe)
	{
		list<TSESFace<T>*> faces;
		GetFirstCategoryFaces(singular_faces,faces);
				#ifdef debug_singularities
				std::cout << "SingularFaces.size() = " << singular_faces.size() << "\n";
				std::cout << "FirstCategoryFaces.size() = " << faces.size() << "\n";
				#endif
		while (faces.size() > 0)
		{
			TSESFace<T>* face1 = faces.front();
			faces.pop_front();
			singular_faces.remove(face1);
			TSESFace<T>* face2 = faces.front();
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
				//case 2 :	TwoCuts(face1,face2,radius_of_probe,ses);
				//					break;
				//case 3 :	ThreeCuts(face1,face2,radius_of_probe,ses);
				//					break;
				case 2 :	;
				case 3 :	rs->deleteSimilarFaces(face1->rsface,face2->rsface);
									std::ofstream print("singularities.log");
									print << *rs;
									print.close();
									throw Exception::GeneralException(__FILE__,__LINE__,
																										"SingularBreak",
																										"reduced surface modified");
			}
		}
	}


	template <class T>
	void TreatSecondCategory(TSolventExcludedSurface<T>* ses,
														const T& radius_of_probe)
	{
		vector<TSESFace<T>*> faces;
		vector< TVector3<T> > points;
		HashSet<Index> indices;
		for (Position i = 0; i != ses->spheric_faces.size(); i++)
		{
			faces.push_back(ses->spheric_faces[i]);
			points.push_back(ses->spheric_faces[i]->rsface->getCenter());
			indices.insert(i);
		}
		TBSDTree<T>* tree = new TBSDTree<T>(points,indices);
		tree->build();
		typename list<TSESEdge<T>*>::iterator edge = ses->singular_edges.begin();
		typename list<TSESEdge<T>*>::iterator tmp;
		while (edge != ses->singular_edges.end())
		{
			tmp = edge;
			tmp++;
			TreatSingularEdge(*edge,faces,tree,ses,radius_of_probe);
			edge = tmp;
		}
	}


	template <class T>
	void GetFirstCategoryFaces(const list< TSESFace<T>* >& singular_faces,
															list<TSESFace<T>*>& faces)
	{
		list<TSESFace<T>*> singular(singular_faces);
		while (singular.size() > 0)
		{
			TSESFace<T>* current = *singular.begin();
			singular.remove(current);
			typename list<TSESFace<T>*>::iterator i = singular.begin();
			while (i != singular.end())
			{
				if (current->rsface->similar(*((*i)->rsface)))
				{
					faces.push_back(current);
					faces.push_back(*i);
					singular.remove(*i);
					i = singular.end();
				}
				else
				{
					i++;
				}
			}
		}
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

  /*
	template <class T>
	void TwoCuts(TSESFace<T>* face1, TSESFace<T>* face2, const T& radius_of_probe,
							 TSolventExcludedSurface<T>* ses)
	{
		TSphere3<T> s1(face1->rsface->getCenter(),radius_of_probe);
		TSphere3<T> s2(face2->rsface->getCenter(),radius_of_probe);
		TCircle3<T> circle;
		GetIntersection(s1,s2,circle);
		TSESEdge<T>* triangular_edge1
				= new TSESEdge<T>(NULL,NULL,face1,NULL,circle,NULL,-1);
		TSESEdge<T>* triangular_edge2
				= new TSESEdge<T>(NULL,NULL,face2,NULL,circle,NULL,-1);
		TSESEdge<T>* quadrilateral_edge1
				= new TSESEdge<T>(NULL,NULL,face1,NULL,circle,NULL,-1);
		TSESEdge<T>* quadrilateral_edge2
				= new TSESEdge<T>(NULL,NULL,face2,NULL,circle,NULL,-1);
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
	void BuildThreeEdges(TSESFace<T>* face, const T& ,
											 TSolventExcludedSurface<T>* ,
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
	}*/


	template <class T>
	void TreatSingularEdge(TSESEdge<T>* edge, vector<TSESFace<T>*> faces,
												 TBSDTree<T>* tree, TSolventExcludedSurface<T>* ses,
												 const T& radius_of_probe)
	{
				#ifdef debug_singularities
				std::cout << "TreatSingularEdge( " << *edge << " , [" << faces.size() << "] , tree, ses, "
									<< radius_of_probe << ")\n";
				#endif
		HashSet<Index> candidates;
		tree->get(edge->circle.p,edge->circle.radius+radius_of_probe,candidates);
		if (candidates.size() == 0)
		{
					#ifdef debug_singularities
					std::cout << "end\n";
					#endif
			return;
		}
		TVector3<T> normal((edge->vertex1->p-edge->circle.p)%
											 (edge->vertex2->p-edge->circle.p));
				#ifdef debug_singularities
				std::cout << "  Drehvektor: " << normal << "\n";
				#endif
		TAngle<T> phi;
		GetAngle(edge->vertex1->p-edge->circle.p,
						 edge->vertex2->p-edge->circle.p,
						 phi);
				#ifdef debug_singularities
				std::cout << "  Winkel der Edge: " << phi << "\n";
				#endif
		TAngle<T> min_phi(Constants::PI,true);
		TAngle<T> max_phi(0,true);
		TVector3<T> min_point;
		TVector3<T> max_point;
		TSphere3<T> min_probe;
		TSphere3<T> max_probe;
		Index min = -1;
		Index max = -1;
		HashSet<Index>::Iterator i;
		TVector3<T> p1;
		TVector3<T> p2;
		TAngle<T> phi1;
		TAngle<T> phi2;
		TSphere3<T> probe;
		probe.radius = radius_of_probe;
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 0.001;
		for (i = candidates.begin(); i != candidates.end(); i++)
		{
			probe.p = faces[*i]->rsface->getCenter();
					#ifdef debug_singularities
					std::cout << "  Schneide edge mit probe " << *i << " (" << probe << ") ...\n";
					#endif
			if (GetIntersectionPointsAndAngles(edge,probe,normal,p1,phi1,p2,phi2))
			{
						#ifdef debug_singularities
						std::cout << "    " << p1 << "  " << phi1 << "\n";
						std::cout << "    " << p2 << "  " << phi2 << "\n";
						#endif
				if (Maths::isGreater(phi1.value,0) && (phi1 < min_phi))
				{
					min_phi = phi1;
					min_point = p1;
					min_probe = probe;
					min = (Index)*i;
							#ifdef debug_singularities
							std::cout << "    ... new min\n";
							#endif
				}
				if (Maths::isLess(phi2.value,phi.value) && (phi2 > max_phi))
				{
					max_phi = phi2;
					max_point = p2;
					max_probe = probe;
					max = (Index)*i;
							#ifdef debug_singularities
							std::cout << "    ... new max\n";
							#endif
				}
			}
			else
			{
						#ifdef debug_singularities
						std::cout << "    ... kein Schntt\n";
						#endif
			}
		}
		Constants::EPSILON = epsilon;
				#ifdef debug_singularities
				std::cout << "  Edge: " << *edge << "\n";
				if (min != -1)
				{
					std::cout << "  minimaler Schnitt:\n"
										<< "    Probe: " << min << ":  (" << min_probe << ")\n"
										<< "    Punkt: " << min_point << "  (" << min_phi << ")\n";
				}
				if (max != -1)
				{
					std::cout << "  maximaler Schnitt:\n"
										<< "    Probe: " << max << ":  (" << max_probe << ")\n"
										<< "    Punkt: " << max_point << "  (" << max_phi << ")\n";
				}
				/*Molecule* molecule = new Molecule;
				if (min != -1)
				{
					Atom* atom = new Atom;
					atom->setPosition(min_point);
					atom->setElement(PTE[Element::O]);
					molecule->insert(*atom);
				}
				if (max != -1)
				{
					Atom* atom = new Atom;
					atom->setPosition(max_point);
					atom->setElement(PTE[Element::O]);
					molecule->insert(*atom);
				}
				System* system = new System;
				system->insert(*molecule);
				HINFile output("HIN/SES/singularPoints"+IndexToString(edge->index,0)+".hin",std::ios::out);
				output << *system;
				output.close();
				delete system;*/
				#endif
		if (min_phi > max_phi)
		{
			return;
		}
		TSESVertex<T>* ns1;
		TSESVertex<T>* ns2;
		TSESEdge<T>* a(NULL);
		TSESEdge<T>* na(NULL);
		if (Maths::isEqual(min_phi.value,0))
		{
			ns1 = edge->vertex1;
		}
		else
		{
			ns1 = new TSESVertex<T>(min_point,TVector3<T>::getZero(),-2,-1);
			a = new TSESEdge<T>(*edge);
			a->vertex2 = ns1;
		}
		if (Maths::isEqual(max_phi.value,phi.value))
		{
			ns2 = edge->vertex2;
		}
		else
		{
			ns2 = new TSESVertex<T>(max_point,TVector3<T>::getZero(),-2,-1);
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
				#ifdef debug_singularities
				std::cout << "  probe1: " << probe1 << "\n";
				std::cout << "  probe2: " << probe2 << "\n";
				std::cout << "  ns1: " << *ns1 << "\n";
				std::cout << "  ns2: " << *ns2 << "\n  a:   ";
				if (a == NULL) std::cout << "(nil)\n  na:  "; else std::cout << *a << "\n  na:  ";
				if (na == NULL) std::cout << "(nil)\n"; else std::cout << *na << "\n";
				/*Molecule* molecule = new Molecule;
				Atom* atom = new Atom;
				atom->setPosition(ns1->p);
				atom->setElement(PTE[Element::O]);
				molecule->insert(*atom);
				atom = new Atom;
				atom->setPosition(ns2->p);
				atom->setElement(PTE[Element::O]);
				molecule->insert(*atom);
				System* system = new System;
				system->insert(*molecule);
				HINFile output("HIN/SES/singularPoints"+IndexToString(edge->index,0)+".hin",std::ios::out);
				output << *system;
				output.close();
				delete system;*/
				#endif
		if (min == max)
		{
			if (edge->face1->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe1,min_probe,circle);
				a1 = new TSESEdge<T>(ns1,ns2,edge->face1,faces[min],circle,NULL,-1);
			}
			if (edge->face2->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe2,min_probe,circle);
				a2 = new TSESEdge<T>(ns1,ns2,edge->face2,faces[min],circle,NULL,-1);
			}
		}
		else
		{
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
			try
			{
				EndEdges(a1,a2,a3,a4,probe1,probe2,min_probe,max_probe,ns1,ns2);
			}
			catch (Exception::GeneralException)
			{
				if (a1 != NULL)
				{
					delete a1;
				}
				if (a2 != NULL)
				{
					delete a2;
				}
				if (a3 != NULL)
				{
					delete a3;
				}
				if (a4 != NULL)
				{
					delete a4;
				}
				if (a != NULL)
				{
					delete a;
					delete ns1;
				}
				if (na != NULL)
				{
					delete na;
					delete ns2;
				}
				return;
			}
		}
		if (a != NULL)
		{
			a->index = ses->edges.size();
			a->type = 2;
			ses->edges.push_back(a);
			ses->singular_edges.push_front(a);
			a->face1->edge.push_back(a);
			a->face1->orientation.push_back(a->face1->orientation[a->face1->getRelativeEdgeIndex(edge->index)]);
			a->face2->edge.push_back(a);
			a->face2->orientation.push_back(a->face2->orientation[a->face2->getRelativeEdgeIndex(edge->index)]);
					#ifdef debug_singularities
					std::cout << "  a == " << *a << " != NULL\n";
					#endif
		}
		if (na != NULL)
		{
			na->index = ses->edges.size();
			na->type = 2;
			ses->edges.push_back(na);
			ses->singular_edges.push_front(na);
			na->face1->edge.push_back(na);
			na->face1->orientation.push_back(na->face1->orientation[na->face1->getRelativeEdgeIndex(edge->index)]);
			na->face2->edge.push_back(na);
			na->face2->orientation.push_back(na->face2->orientation[na->face2->getRelativeEdgeIndex(edge->index)]);
					#ifdef debug_singularities
					std::cout << "  na == " << *na << " != NULL\n";
					#endif
		}
		if (a1 != NULL)
		{
			a1->index = ses->edges.size();
			a1->type = 2;
			ses->edges.push_back(a1);
			ses->singular_edges.push_back(a1);
			a1->face1->edge.push_back(a1);
			a1->face1->orientation.push_back(0);
			a1->face2->edge.push_back(a1);
			a1->face2->orientation.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a1 == " << *a1 << " != NULL\n";
					#endif
		}
		if (a2 != NULL)
		{
			a2->index = ses->edges.size();
			a2->type = 2;
			ses->edges.push_back(a2);
			ses->singular_edges.push_back(a2);
			a2->face1->edge.push_back(a2);
			a2->face1->orientation.push_back(0);
			a2->face2->edge.push_back(a2);
			a2->face2->orientation.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a2 == " << *a2 << " != NULL\n";
					#endif
		}
		if (a3 != NULL)
		{
			a3->index = ses->edges.size();
			a3->type = 2;
			ses->edges.push_back(a3);
			ses->singular_edges.push_back(a3);
			a3->face1->edge.push_back(a3);
			a3->face1->orientation.push_back(0);
			a3->face2->edge.push_back(a3);
			a3->face2->orientation.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a3 == " << *a3 << " != NULL\n";
					#endif
		}
		if (a4 != NULL)
		{
			a4->index = ses->edges.size();
			a4->type = 2;
			ses->edges.push_back(a4);
			ses->singular_edges.push_back(a4);
			a4->face1->edge.push_back(a4);
			a4->face1->orientation.push_back(0);
			a4->face2->edge.push_back(a4);
			a4->face2->orientation.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a4 == " << *a4 << " != NULL\n";
					#endif
		}
		edge->face1->edge[edge->face1->getRelativeEdgeIndex(edge->index)] = NULL;
		edge->face2->edge[edge->face2->getRelativeEdgeIndex(edge->index)] = NULL;
		ses->edges[edge->index] = NULL;
		ses->singular_edges.remove(edge);
		delete edge;
		if (a != NULL)
		{
			ns1->index = ses->vertices.size();
			ses->vertices.push_back(ns1);
		}
		if (na != NULL)
		{
			ns2->index = ses->vertices.size();
			ses->vertices.push_back(ns2);
		}
				#ifdef debug_singularities
				//ofstream print("singular.log");
				//print << *ses;
				//print.close();
				//std::cout << "\n\n" << *ses << "\n\n";
				#endif
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
					//TAngle<T> pi(Constants::PI,true);			// ACHTUNG !!!
					//phi1 += pi;
					//phi2 += pi;
			//TAngle<T> two_pi(2*Constants::PI,true);
					if (phi1.value <= 0)								// ACHTUNG !!!
					{
						phi1.value += Constants::PI;
					}
					else
					{
						phi1.value -= Constants::PI;
					}
					if (phi2.value <= 0)
					{
						phi2.value += Constants::PI;
					}
					else
					{
						phi2.value -= Constants::PI;
					}
			//double epsilon = Constants::EPSILON;
			//Constants::EPSILON = 0.0001;
			//if (phi1 == two_pi)
			//{
			//	phi1.set(0);
			//}
			//if (phi2 == two_pi)
			//{
			//	phi2.set(0);
			//}
			//Constants::EPSILON = epsilon;
			if (phi2 < phi1)
			{
				T phi(phi1.value);
				phi1.value = phi2.value;
				phi2.value = phi;
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
	void EndEdges
		(TSESEdge<T>*				edge1,
		 TSESEdge<T>*				edge2,
		 TSESEdge<T>*				edge3,
		 TSESEdge<T>*				edge4,
		 const TSphere3<T>& probe1,
		 const TSphere3<T>& probe2,
		 const TSphere3<T>& min_probe,
		 const TSphere3<T>& max_probe,
		 TSESVertex<T>*			ns1,
		 TSESVertex<T>*			ns2)
	{
		if (edge1 != NULL)
		{
			if (Maths::isGreater(edge1->circle.n*(ns2->p-edge1->circle.p),(T)0))
			{
				edge1->circle.n.negate();
			}
		}
		if (edge2 != NULL)
		{
			if (Maths::isGreater(edge2->circle.n*(ns2->p-edge2->circle.p),(T)0))
			{
				edge2->circle.n.negate();
			}
		}
		if (edge3 != NULL)
		{
			if (Maths::isGreater(edge3->circle.n*(ns1->p-edge3->circle.p),(T)0))
			{
				edge3->circle.n.negate();
			}
		}
		if (edge4 != NULL)
		{
			if (Maths::isGreater(edge4->circle.n*(ns1->p-edge4->circle.p),(T)0))
			{
				edge4->circle.n.negate();
			}
		}
		TAngle<T> phi1;
		TAngle<T> phi2;
				TAngle<T> pi(Constants::PI,true);					// Achtung
		TVector3<T> point1;
		TVector3<T> point2;
		TSESVertex<T>* v;
		if (GetIntersection(probe1,min_probe,max_probe,point1,point2))
		{
			if (edge1 != NULL)
			{
				phi1 = getOrientedAngle(ns1->p,point1,edge1->circle.n);
				phi2 = getOrientedAngle(ns1->p,point2,edge1->circle.n);
							phi1 += pi;													// Achtung
							phi2 += pi;													// Achtung
				v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->p = point1;
				}
				else
				{
					v->p = point2;
				}
				edge1->vertex2 = v;
			}
			if (edge3 != NULL)
			{
				phi1 = getOrientedAngle(ns2->p,point1,edge3->circle.n);
				phi2 = getOrientedAngle(ns2->p,point2,edge3->circle.n);
							phi1 += pi;													// Achtung
							phi2 += pi;													// Achtung
				v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->p = point1;
				}
				else
				{
					v->p = point2;
				}
				edge3->vertex2 = v;
			}
		}
		else
		{
			throw Exception::GeneralException(__FILE__,__LINE__,
																				"CanNotTreatSingularity",
																				"no end-point found for SES-edge");
		}
		if (GetIntersection(probe2,min_probe,max_probe,point1,point2))
		{
			if (edge2 != NULL)
			{
				phi1 = getOrientedAngle(ns1->p,point1,edge2->circle.n);
				phi2 = getOrientedAngle(ns1->p,point2,edge2->circle.n);
							phi1 += pi;													// Achtung
							phi2 += pi;													// Achtung
				TSESVertex<T>* v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->p = point1;
				}
				else
				{
					v->p = point2;
				}
				edge2->vertex2 = v;
			}
			if (edge4 != NULL)
			{
				phi1 = getOrientedAngle(ns2->p,point1,edge4->circle.n);
				phi2 = getOrientedAngle(ns2->p,point2,edge4->circle.n);
							phi1 += pi;													// Achtung
							phi2 += pi;													// Achtung
				v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->p = point1;
				}
				else
				{
					v->p = point2;
				}
				edge4->vertex2 = v;
			}
		}
		else
		{
			throw Exception::GeneralException(__FILE__,__LINE__,
																				"CanNotTreatSingularity",
																				"no end-point found for SES-edge");
		}
	}


	template <class T>
	bool GetIntersection(const TSphere3<T>& sphere, const TCircle3<T>& circle,
											 TVector3<T>& p1, TVector3<T>& p2)
	{
		TSphere3<T> s2(circle.p,circle.radius);
		TCircle3<T> intersection_circle;
		if (GetIntersection(sphere,s2,intersection_circle) == false)
		{
			return false;
		}
		TPlane3<T> plane1(circle.p,circle.n);
		TPlane3<T> plane2(intersection_circle.p,intersection_circle.n);
		TLine3<T> line;
		if (GetIntersection(plane1,plane2,line) == false)
		{
			return false;
		}
		T a(line.d.getSquareLength());
		T b(line.d*(line.p-circle.p)*2);
		T c((line.p-circle.p).getSquareLength()-circle.radius*circle.radius);
		T lambda1;
		T lambda2;
		if (SolveQuadraticEquation(a,b,c,lambda1,lambda2) == 0)
		{
			return false;
		}
		p1 = line.p+lambda1*line.d;
		p2 = line.p+lambda2*line.d;
		//std::cout << sphere << " * " << circle << "  =  [" << p1 << "," << p2 << "\n";
		return (circle.has(p1) && circle.has(p2) && sphere.has(p1) && sphere.has(p2));
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
			TVector3<T> norm((circle2.p-circle1.p)*2.0);
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



	template <class T>
	bool getIntersection(const TPlane3<T>& plane1, const TPlane3<T>& plane2,
											 TLine3<T>& line)
	{
		T u = plane1.p*plane1.n;
		T v = plane2.p*plane2.n;
		T det = plane1.n.x*plane2.n.y-plane1.n.y*plane2.n.x;
		if (Maths::isZero(det))
		{
			det = plane1.n.x*plane2.n.z-plane1.n.z*plane2.n.x;
			if (Maths::isZero(det))
			{
				det = plane1.n.y*plane2.n.z-plane1.n.z*plane2.n.y;
				if (Maths::isZero(det))
				{
					return false;
				}
				else
				{
					T a = plane2.n.z/det;
					T b = -plane1.n.z/det;
					T c = -plane2.n.y/det;
					T d = plane1.n.y/det;
					line.p.x = 0;
					line.p.y = a*u+b*v;
					line.p.z = c*u+d*v;
					line.d.x = -1;
					line.d.y = a*plane1.n.x+b*plane2.n.x;
					line.d.z = c*plane1.n.x+d*plane2.n.x;
				}
			}
			else
			{
				T a = plane2.n.z/det;
				T b = -plane1.n.z/det;
				T c = -plane2.n.x/det;
				T d = plane1.n.x/det;
				line.p.x = a*u+b*v;
				line.p.y = 0;
				line.p.z = c*u+d*v;
				line.d.x = a*plane1.n.y+b*plane2.n.y;
				line.d.y = -1;
				line.d.z = c*plane1.n.y+d*plane2.n.y;
			}
		}
		else
		{
			T a = plane2.n.y/det;
			T b = -plane1.n.y/det;
			T c = -plane2.n.x/det;
			T d = plane1.n.x/det;
			line.p.x = a*u+b*v;
			line.p.y = c*u+d*v;
			line.p.z = 0;
			line.d.x = a*plane1.n.z+b*plane2.n.z;
			line.d.y = c*plane1.n.z+d*plane2.n.z;
			line.d.z = -1;
		}
		return true;
	}



}

#endif		// BALL_STRUCTURE_SINGULARITIES_H
