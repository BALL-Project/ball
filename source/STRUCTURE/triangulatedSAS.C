// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSAS.C,v 1.1 2002/12/17 17:14:23 anker Exp $

#ifdef debug_triangulation
#	include <BALL/KERNEL/atom.h>
#	include <BALL/KERNEL/molecule.h>
#	include <BALL/KERNEL/system.h>
#	include <BALL/KERNEL/PTE.h>
#	include <BALL/FORMAT/HINFile.h>
#	include <BALL/DATATYPE/string.h>
#endif

#include <BALL/STRUCTURE/solventAccessibleSurface.h>
#include <BALL/STRUCTURE/triangle.h>
#include <BALL/STRUCTURE/triangleEdge.h>
#include <BALL/STRUCTURE/trianglePoint.h>
#include <BALL/STRUCTURE/triangulatedSAS.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/plane3.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/vector4.h>
#include <list>
#include <vector>

namespace BALL
{

	TriangulatedSAS::TriangulatedSAS()
		throw()
		:	TriangulatedSurface(),
			sas_(NULL),
			density_(4.5)
	{
	}


	TriangulatedSAS::TriangulatedSAS(const TriangulatedSAS& surface, bool deep)
		throw()
		:	TriangulatedSurface(surface,deep),
			sas_(surface.sas_),
			density_(surface.density_)
	{
	}


	TriangulatedSAS::TriangulatedSAS
			(SolventAccessibleSurface* sas,
			 const double& density)
		throw()
		:	TriangulatedSurface(),
			sas_(sas),
			density_(density)
	{
	}


	TriangulatedSAS::~TriangulatedSAS()
		throw()
	{
	}


	void TriangulatedSAS::set(const TriangulatedSAS& surface, bool)
		throw()
	{
		if (this != &surface)
		{
			TriangulatedSurface::set(surface);
			sas_ = surface.sas_;
			density_ = surface.density_;
		}
	}


	TriangulatedSAS& TriangulatedSAS::operator =
			(const TriangulatedSAS& surface)
		throw()
	{
		if (this != &surface)
		{
			TriangulatedSurface::operator = (surface);
			sas_ = surface.sas_;
			density_ = surface.density_;
		}
		return *this;
	}


	void TriangulatedSAS::setDensity(const double& density)
		throw()
	{
		density_ = density;
	}


	double TriangulatedSAS::getDensity() const
		throw()
	{
		return density_;
	}


	void TriangulatedSAS::compute()
		throw()
	{
		SASTriangulator sast(this);
		sast.run();
	}


	#ifdef debug_triangulation
	void TriangulatedSAS::printToHIN(const string& filename)
		throw()
	{
		Molecule* molecule = new Molecule;
		std::list<Triangle*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			TVector3<double> norm(((*t)->vertex_[0]->point_-(*t)->vertex_[1]->point_) %
									 ((*t)->vertex_[0]->point_-(*t)->vertex_[2]->point_)	);
			norm.normalize();
			norm /= 4;
			Atom* atom1 = new Atom;
			Atom* atom2 = new Atom;
			Atom* atom3 = new Atom;
			Atom* atom4 = new Atom;
			Atom* atom5 = new Atom;
			Atom* atom6 = new Atom;
			atom1->setElement(PTE[Element::H]);
			atom2->setElement(PTE[Element::H]);
			atom3->setElement(PTE[Element::H]);
			atom4->setElement(PTE[Element::H]);
			atom5->setElement(PTE[Element::H]);
			atom6->setElement(PTE[Element::H]);
			atom1->setPosition((*t)->vertex_[0]->point_);
			atom2->setPosition((*t)->vertex_[1]->point_);
			atom3->setPosition((*t)->vertex_[2]->point_);
			atom4->setPosition((*t)->vertex_[0]->point_+norm);
			atom5->setPosition((*t)->vertex_[1]->point_+norm);
			atom6->setPosition((*t)->vertex_[2]->point_+norm);
			#ifdef with_bonds
			atom1->createBond(*atom2);
			atom2->createBond(*atom3);
			atom3->createBond(*atom1);
			atom1->createBond(*atom4);
			atom2->createBond(*atom5);
			atom3->createBond(*atom6);
			#endif
			molecule->insert(*atom1);
			molecule->insert(*atom2);
			molecule->insert(*atom3);
			molecule->insert(*atom4);
			molecule->insert(*atom5);
			molecule->insert(*atom6);
		}
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(filename,ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}
	#endif


	SASTriangulator::SASTriangulator()
		throw()
		:	tsas_(NULL),
			sqrt_density_(0.0),
			edge_(),
			template_spheres_()
	{
	}


	SASTriangulator::SASTriangulator(TriangulatedSAS* tsas)
		throw()
		:	tsas_(tsas),
			sqrt_density_(sqrt(tsas->density_)),
			edge_(tsas_->sas_->number_of_edges_),
			template_spheres_()
	{
	}


	SASTriangulator::~SASTriangulator()
		throw()
	{
	}


	void SASTriangulator::run()
		throw()
	{
		// Store for each SASEdge a list of TVector3<double>'s.
		std::vector<SASEdge*>::iterator e;
		for (e = tsas_->sas_->edges_.begin(); e != tsas_->sas_->edges_.end(); e++)
		{
			partitionEdge(*e);
		}
		// Now we triangulate the faces.
		// build template spheres with different densities and outside normal vectors
		buildTemplateSpheres();
		for (Position i = 0; i < tsas_->sas_->number_of_faces_; i++)
		{
			triangulateFace(tsas_->sas_->faces_[i]);
		}
	}


	void SASTriangulator::partitionEdge(SASEdge* edge)
		throw()
	{
		TAngle<double> phi(edge->angle_);
		Size number_of_segments
				= (Size)Maths::round(phi.value*edge->circle_.radius*sqrt_density_);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<double> psi(phi.value/number_of_segments,true);
		if (edge->vertex_[0] == NULL)
		{
			partitionOfCircle(edge->circle_,TVector3<double>::getZero(),
												psi,number_of_segments,
												edge_[edge->index_],false);
			edge_[edge->index_].pop_back();
		}
		else
		{
			partitionOfCircle(edge->circle_,edge->vertex_[0]->point_,
												psi,number_of_segments,
												edge_[edge->index_],true);
			edge_[edge->index_].pop_front();
			edge_[edge->index_].pop_back();
		}
	}


	void SASTriangulator::triangulateFace(SASFace* face)
		throw()
	{
		TSphere3<double> sphere(face->sphere_);
		std::list< TVector3<double> > border_points;
		std::list<SASEdge*>::iterator edge;
		std::list< TVector3<double> >::const_iterator p;
		for (edge = face->edge_.begin(); edge != face->edge_.end(); edge++)
		{
			for (p = edge_[(*edge)->index_].begin();
					 p != edge_[(*edge)->index_].end();
					 p++)
			{
				border_points.push_back(*p);
			}
		}
		std::list<SASVertex*>::iterator vertex;
		for (vertex = face->vertex_.begin();
				 vertex != face->vertex_.end();
				 vertex++)
		{
			border_points.push_back((*vertex)->point_);
		}
		Position number_of_edges = face->edge_.size();
		if (number_of_edges > 0)
		{
			// the face does not come from a free vertex
			if ((number_of_edges == 2) &&
					(edge_[face->edge_.front()->index_].size() == 0) &&
					(edge_[face->edge_.back()->index_].size() == 0)			)
			{
				return;
			}
			if (number_of_edges == 3)
			{
				Position counter = 0;
				std::list<SASEdge*>::iterator it;
				for (it = face->edge_.begin(); it != face->edge_.end(); it++)
				{
					if (edge_[(*it)->index_].size() == 0)
					{
						counter++;
					}
				}
				if (counter == 3)
				{
					Triangle* triangle = new Triangle;
					Position k = 0;
					for (p = border_points.begin(); p != border_points.end(); p++)
					{
						triangle->vertex_[k] = new TrianglePoint;
						triangle->vertex_[k]->point_ = *p;
						triangle->vertex_[k]->normal_ = *p-sphere.p;
						k++;
					}
					tsas_->triangles_.push_back(triangle);
					tsas_->points_.push_back(triangle->vertex_[0]);
					tsas_->points_.push_back(triangle->vertex_[1]);
					tsas_->points_.push_back(triangle->vertex_[2]);
					tsas_->number_of_triangles_++;
					tsas_->number_of_points_ += 3;
					return;
				}
			}
			// get a template sphere for the face to triangulate
			TriangulatedSAS part;
			HashMap< Size,std::list< TVector3<double> > >::ConstIterator s
				= template_spheres_.find(0);
//				= template_spheres_.find(numberOfRefinements(tsas_->density_,
//																										 sphere.radius));
			TrianglePoint* point;
			for (p = s->second.begin(); p != s->second.end(); p++)
			{
				point = new TrianglePoint;
				point->point_ = *p;
				point->normal_ = *p-sphere.p;
				point->index_ = 1;
				part.points_.push_back(point);
				part.number_of_points_++;
			}
			part.blowUp(sphere.radius);
			// cut the face with all its edges
			TPlane3<double> plane;
			std::list<bool>::iterator o = face->orientation_.begin();
			for (edge = face->edge_.begin(); edge != face->edge_.end(); edge++)
			{
				plane.p = (*edge)->circle_.p-sphere.p;
				if (*o)
				{
					plane.n = (*edge)->circle_.n;
				}
				else
				{
					plane.n = -(*edge)->circle_.n;
				}
				part.cut(plane,0.05);
				o++;
						#ifdef print_debug_info
						std::cout << "schneide mit plane " << plane << " von edge " << **edge
											<< " ... " << part.number_of_points_ << " (" << part.points_.size() << ")\n";
						#endif
			}
			part.shift(sphere.p);
			for (p = border_points.begin(); p != border_points.end(); p++)
			{
				point = new TrianglePoint;
				point->point_ = *p;
				point->normal_ = *p-sphere.p;
				part.points_.push_back(point);
				part.number_of_points_++;
			}
					#ifdef print_debug_info
					int halt = 0;
					if (halt == 0) cin >> halt; else { cout << "\n"; halt--;}
					#endif
					#ifdef debug_triangulation
					string filename = face->index_;
					filename = "DATA/MOLECULE/contactFaceCut"+filename+".hin";
					part.printToHINFile(filename);
					Molecule* sewMolecule = new Molecule;
					#endif
			#ifdef print_debug_info
			buildSphericTriangles(part,sphere,halt,sewMolecule);
			#else
			buildSphericTriangles(part,sphere);
			#endif
			// delete all isolated points
			part.deleteIsolatedPoints();
					#ifdef debug_triangulation
					System* sewSystem = new System;
					sewSystem->insert(*sewMolecule);
					filename = face->index_;
					filename = "DATA/MOLECULE/contactFaceSew"+filename+".hin";
					HINFile output(filename,std::ios::out);
					output << *sewSystem;
					output.close();
					delete sewSystem;
					#endif
			tsas_->join(part);
		}
		else
		{
			// the face comes from a free vertex
			TriangulatedSphere part;
			part.icosaeder();
			part.refine(numberOfRefinements(tsas_->density_,sphere.radius));
			part.shift(sphere.p);
			tsas_->join(part);
		}
	}


	bool SASTriangulator::buildSphericTriangles
			(TriangulatedSAS&	part,
			 const TSphere3<double>&		sphere
		 			#ifdef print_debug_info
			 		, int halt,
			 		Molecule* sewMolecule
			 		#endif
			)
		throw()
	{
				#ifdef print_debug_info
				string pre = "      ";
				#endif
		bool ok;
		std::list<TriangleEdge*> border;
		#ifdef print_debug_info
		ok = buildFirstTriangle(part,sphere,border,halt,sewMolecule);
		#else
		ok = buildFirstTriangle(part,sphere,border);
		#endif
		if (!ok)
		{
			return false;
		}
		while (!border.empty())
		{
					#ifdef print_debug_info
					std::cout << pre << "derzeit (" << border.size() << ") Edges";
					if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
			  	#endif
			TriangleEdge* edge = border.front();
			border.pop_front();
					#ifdef print_debug_info
			  	std::cout << pre << *edge << " gepopt\n";
			  	#endif
			Triangle* start_triangle = edge->face_[0];
			TrianglePoint* third_point
					= start_triangle->third(edge->vertex_[0],edge->vertex_[1]);
			TVector3<double> normal((third_point->point_-edge->vertex_[0]->point_) %
												 (third_point->point_-edge->vertex_[1]->point_)	);
			double test_value = normal*third_point->point_;
			Index p0 = start_triangle->getRelativeIndex(edge->vertex_[0]);
			Index p1 = start_triangle->getRelativeIndex(edge->vertex_[1]);
			Index diff = p1-p0;
			if ((diff == -1) || (diff == 2))
			{
				TrianglePoint* tmp = edge->vertex_[0];
				edge->vertex_[0] = edge->vertex_[1];
				edge->vertex_[1] = tmp;
			}
					#ifdef print_debug_info
					std::cout << pre << "starte mit Edge " << *edge << " (" << edge << ")\n";
					std::cout << pre << "third point: " << third_point->index_ << "\n";
					std::cout << pre << "Dreieck: " << *start_triangle << " (" << test_value << ")  ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			std::list<TrianglePoint*>::iterator next
					= part.points_.begin();
			while ((*next == edge->vertex_[0]) || (*next == edge->vertex_[1]) ||
						 (*next == third_point))
			{
						#ifdef print_debug_info
						std::cout << pre << "  dritter Punkt: " << (*next)->index_ << " ... --  ";
						#endif
				next++;
			}
			std::list<TrianglePoint*> third;
			third.push_back(*next);
			normal.set(((*next)->point_-edge->vertex_[1]->point_) %
								 ((*next)->point_-edge->vertex_[0]->point_)	);
			test_value = normal*edge->vertex_[0]->point_;
					#ifdef print_debug_info
					std::cout << pre << "  dritter Punkt: " << (*next)->index_
										<< " (" << test_value << ")" << " ... " << "not ok (" << test_value << ") "
										<< "--> " << normal << ":  " << test_value << "  ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			next++;
			while (next != part.points_.end())
			{
				if ((*next != edge->vertex_[0]) && (*next != edge->vertex_[1]) &&
						(*next != third_point))
				{
					double this_value = normal*(*next)->point_;
							#ifdef print_debug_info
							std::cout << pre << "  dritter Punkt: " << (*next)->index_
									 << " (" << test_value << ") " << " ... ";
							#endif
					if (Maths::isGreater(this_value,test_value))
					{
								#ifdef print_debug_info
								std::cout << "not ok (" << this_value << ")  ";
								#endif
						third.clear();
						third.push_back(*next);
						normal.set(((*next)->point_-edge->vertex_[1]->point_) %
											 ((*next)->point_-edge->vertex_[0]->point_)	);
						test_value = normal*edge->vertex_[0]->point_;
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
							std::cout << pre << "  dritter Punkt: " << (*next)->index_ << " ... --  ";
						}
						#endif
				next++;
						#ifdef print_debug_info
						std::cout << "\n";
						#endif
			}
					#ifdef print_debug_info
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			switch (third.size())
			{
				case 0 :
									#ifdef print_debug_info
									std::cout << pre << "no convex triangle found\n";
									#endif
									break;
				case 1 :
									#ifdef print_debug_info
									buildUnambiguousTriangle(edge,third.front(),border,sphere,part,halt,sewMolecule);
									#else
									buildUnambiguousTriangle(edge,third.front(),border,
																					 sphere,part);
									#endif
									break;
				default :
									#ifdef print_debug_info
									buildAmbiguousTriangles(edge,third,border,sphere,part,halt,sewMolecule);
									#else
									buildAmbiguousTriangles(edge,third,border,sphere,part);
									#endif
									break;
			}
		}
		return true;
	}


	bool SASTriangulator::buildFirstTriangle
			(TriangulatedSAS&						part,
			 const TSphere3<double>&							sphere,
			 std::list<TriangleEdge*>&	border
						#ifdef print_debug_info
						, int& halt,
						Molecule* sewMolecule
						#endif
			)
		throw()
	{
		std::list<TrianglePoint*> inner_points;
		std::list<TrianglePoint*>::iterator p;
		for (p = part.points_.begin(); p != part.points_.end(); p++)
		{
			if ((*p)->index_ > 0)
			{
				inner_points.push_back(*p);
			}
		}
				#ifdef print_debug_info
				std::cout << "number of inner points: " << inner_points.size() << "\n";
				#endif
		if (inner_points.empty())
		{
					#ifdef print_debug_info
					cout << "#########\n";
					#endif
			return false;
		}
		else
		{
			TrianglePoint* point1 = NULL;
			TrianglePoint* point2 = NULL;
			TrianglePoint* point3 = NULL;
			point1 = inner_points.front();
			std::list<TrianglePoint*>::iterator p1;
			std::list<TrianglePoint*>::iterator p2;
			std::list<TrianglePoint*>::iterator test;
			bool found = false;
			p1 = part.points_.begin();
			while ((!found) && (p1 != part.points_.end()))
			{
				if (*p1 != point1)
				{
					p2 = p1;
					p2++;
					while ((!found) && (p2 != part.points_.end()))
					{
						if (*p2 != point1)
						{
							TPlane3<double> plane(point1->point_,(*p1)->point_,(*p2)->point_);
							double test_value = plane.n*point1->point_;
							if (Maths::isGreater(plane.n*sphere.p,test_value))
							{
								plane.n.negate();
								test_value = -test_value;
							}
							bool ok = true;
							test = part.points_.begin();
							while (ok && (test != part.points_.end()))
							{
								if ((*test != point1) && (test != p1) && (test != p2))
								{
									ok = Maths::isLessOrEqual(plane.n*(*test)->point_,test_value);
								}
								test++;
							}
							if (ok)
							{
								point2 = *p1;
								point3 = *p2;
								found = true;
							}
						}
						p2++;
					}
				}
				p1++;
			}
			if (found)
			{
						#ifdef print_debug_info
						std::cout << "point1: " << point1->index_ << "\n";
						std::cout << "point2: " << point2->index_ << "\n";
						std::cout << "point3: " << point3->index_ << "\n";
						#endif
				Triangle* triangle = new Triangle;
				TriangleEdge* edge1 = new TriangleEdge;
				TriangleEdge* edge2 = new TriangleEdge;
				TriangleEdge* edge3 = new TriangleEdge;
				TVector3<double> test((point2->point_-point1->point_)%
												 (point3->point_-point1->point_));
				if (Maths::isGreater(test*(sphere.p-point1->point_),0.0))
				{
					TrianglePoint* tmp = point2;
					point2 = point3;
					point3 = tmp;
				}
				triangle->vertex_[0] = point1;
				triangle->vertex_[1] = point2;
				triangle->vertex_[2] = point3;
				triangle->edge_[0] = edge1;
				triangle->edge_[1] = edge2;
				triangle->edge_[2] = edge3;
				edge1->vertex_[0] = point1;
				edge1->vertex_[1] = point2;
				edge1->face_[0] = triangle;
				edge2->vertex_[0] = point2;
				edge2->vertex_[1] = point3;
				edge2->face_[0] = triangle;
				edge3->vertex_[0] = point3;
				edge3->vertex_[1] = point1;
				edge3->face_[0] = triangle;
				point1->edges_.insert(edge1);
				point1->edges_.insert(edge3);
				point2->edges_.insert(edge1);
				point2->edges_.insert(edge2);
				point3->edges_.insert(edge2);
				point3->edges_.insert(edge3);
				part.edges_.push_back(edge1);
				part.edges_.push_back(edge2);
				part.edges_.push_back(edge3);
				part.number_of_edges_ += 3;
				part.triangles_.push_back(triangle);
				part.number_of_triangles_++;
				border.push_back(edge1);
				border.push_back(edge2);
				border.push_back(edge3);
			}
		}
				#ifdef print_debug_info
				if (halt == 0) cin >> halt; else { cout << "\n"; halt--;}
				#endif
		return true;
	}


	void SASTriangulator::buildUnambiguousTriangle
			(TriangleEdge*						 edge,
			 TrianglePoint*						 point,
			 std::list<TriangleEdge*>& border,
			 const TSphere3<double>&						 sphere,
			 TriangulatedSAS&					 part
						#ifdef print_debug_info
						, int& halt,
						Molecule* sewMolecule
						#endif
			)
		throw()
	{
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "eindeutiges Dreieck\n";
				#endif
		TriangleEdge* edge1;
		TriangleEdge* edge2;
		Triangle* triangle;
		bool old1, old2;
		createTriangleAndEdges(edge,point,sphere,edge1,old1,edge2,old2,triangle);
		if (old1 == true)
		{
			if (edge1->face_[0] == NULL)
			{
				edge1->face_[0] = triangle;
			}
			else
			{
				edge1->face_[1] = triangle;
			}
			border.remove(edge1);
					#ifdef print_debug_info
					std::cout << pre << "remove Edge " << *edge1
										<< " (" << edge1 << ") from\n" << pre << "  border\n";
					#endif
		}
		else
		{
			edge1->vertex_[0]->edges_.insert(edge1);
			edge1->vertex_[1]->edges_.insert(edge1);
			edge1->face_[0] = triangle;
					#ifdef with_indices
					edge1->index_ = part.edges_.size();
					#endif
			part.edges_.push_back(edge1);
			part.number_of_edges_++;
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge " << *edge1 << " (" << edge1 << ") to\n"
										<< pre << "  part\n";
					#endif
			if ((edge1->vertex_[0]->index_ > 0) || (edge1->vertex_[1]->index_ > 0))
			{
				border.push_back(edge1);
						#ifdef print_debug_info
						std::cout << pre << "  border\n";
						#endif
			}
		}
		if (old2 == true)
		{
			if (edge2->face_[0] == NULL)
			{
				edge2->face_[0] = triangle;
			}
			else
			{
				edge2->face_[1] = triangle;
			}
			border.remove(edge2);
					#ifdef print_debug_info
					std::cout << pre << "remove Edge " << *edge2 << " (" << edge2 << ") from\n"
										<< pre << "  border\n";
					#endif
		}
		else
		{
			edge2->vertex_[0]->edges_.insert(edge2);
			edge2->vertex_[1]->edges_.insert(edge2);
			edge2->face_[0] = triangle;
					#ifdef with_indices
					edge2->index_ = part.edges_.size();
					#endif
			part.edges_.push_back(edge2);
			part.number_of_edges_++;
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge " << *edge2 << " (" << edge2 << ") to\n"
										<< pre << "  part\n";
					#endif
			if ((edge2->vertex_[0]->index_ > 0) || (edge2->vertex_[1]->index_ > 0))
			{
				border.push_back(edge2);
						#ifdef print_debug_info
						std::cout << pre << "  border\n";
						#endif
			}
		}
		edge->face_[1] = triangle;
		triangle->vertex_[0]->faces_.insert(triangle);
		triangle->vertex_[1]->faces_.insert(triangle);
		triangle->vertex_[2]->faces_.insert(triangle);
				#ifdef with_indices
				triangle->index_ = part.number_of_triangles_;
				#endif
		part.triangles_.push_back(triangle);
		part.number_of_triangles_++;
					#ifdef print_debug_info
					std::cout << pre << "geupdatet:\n";
					std::cout << pre << "  Edge " << *edge << " (" << edge << ")\n";
					std::cout << pre << "  Edge " << *edge1 << " (" << edge1 << ")\n";
					std::cout << pre << "  Edge " << *edge2 << " (" << edge2 << ")\n";
					std::cout << pre << "  Point " << *(triangle->vertex_[0]) << " (" << triangle->vertex_[0] << ")\n";
					std::cout << pre << "  Point " << *(triangle->vertex_[1]) << " (" << triangle->vertex_[1] << ")\n";
					std::cout << pre << "  Point " << *(triangle->vertex_[2]) << " (" << triangle->vertex_[2] << ")\n";
					std::cout << pre << "  Triangle " << *triangle << " (" << triangle << ")\n";
					std::cout << pre << "pushe Triangle " << *triangle << " (" << triangle << ") to\n"
										<< pre << "  surface";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
					#ifdef debug_triangulation
					Atom* atom1 = new Atom;
					Atom* atom2 = new Atom;
					Atom* atom3 = new Atom;
					atom1->setPosition(triangle->vertex_[0]->point_);
					atom2->setPosition(triangle->vertex_[1]->point_);
					atom3->setPosition(triangle->vertex_[2]->point_);
					atom1->setElement(PTE[Element::H]);
					atom2->setElement(PTE[Element::H]);
					atom3->setElement(PTE[Element::H]);
					#ifdef with_bonds
					atom1->createBond(*atom2);
					atom2->createBond(*atom3);
					atom3->createBond(*atom1);
					#endif
					sewMolecule->insert(*atom1);
					sewMolecule->insert(*atom2);
					sewMolecule->insert(*atom3);
					#endif
	}


	void SASTriangulator::buildAmbiguousTriangles
			(TriangleEdge*						 edge,
			 std::list<TrianglePoint*> points,
			 std::list<TriangleEdge*>& border,
			 const TSphere3<double>&						 sphere,
			 TriangulatedSAS&					 part
						#ifdef print_debug_info
						, int& halt,
						Molecule* sewMolecule
						#endif
			)
		throw()
	{
		std::list<TrianglePoint*>::iterator p;
				#ifdef print_debug_info
				string pre = "        ";
				std::cout << pre << points.size() << " Punkte gefunden ... (";
				for (p = points.begin(); p != points.end(); p++)
				{
					std::cout << (*p)->index_ << " ";
				}
				std::cout << ")\n";
				#endif
		points.push_back(edge->vertex_[0]);
		points.push_back(edge->vertex_[1]);
				#ifdef print_debug_info
				std::cout << pre << points.size() << " Punkte in einer Ebene ... (";
				for (p = points.begin(); p != points.end(); p++)
				{
					std::cout << (*p)->index_ << " ";
				}
				std::cout << ") ";
				if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		std::list<TriangleEdge*> planar_edges;
		planar_edges.push_back(edge);
		TriangleEdge* edge0;
		TriangleEdge* edge1;
		TriangleEdge* edge2;
		Triangle* triangle;
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
				if ((*p == edge0->vertex_[0]) || (*p == edge0->vertex_[1]))
				{
					p++;
				}
				else
				{
							#ifdef print_debug_info
							std::cout << pre << "baue Dreieck mit Punkt " << **p << " (" << *p << ") ";
							if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					createTriangleAndEdges(edge0,*p,sphere,
																 edge1,old1,edge2,old2,triangle);
							#ifdef print_debug_info
							std::cout << pre << "teste dieses Dreieck: ";
							if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					if (edge0 == edge)
					{
						// the first triangle has not to be tested
						built = true;
					}
					else
					{
						Triangle* old_triangle = edge0->face_[0];
								#ifdef print_debug_info
								std::cout << pre << "  teste gegen Dreieck " << *old_triangle;
								std::cout << " (" << old_triangle << ") ";
								if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						Index i1 = old_triangle->getRelativeIndex(edge0->vertex_[0]);
						Index i2 = old_triangle->getRelativeIndex(edge0->vertex_[1]);
						bool back = false;
						if ((i1-i2 == 1) || (i1-i2 == -2))
						{
							back = true;
						}
						i1 = triangle->getRelativeIndex(edge0->vertex_[0]);
						i2 = triangle->getRelativeIndex(edge0->vertex_[1]);
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
					}
							#ifdef print_debug_info	
							std::cout << pre << (built ? "ok " : "nok ") << " " << planar_edges.size() << " ";
							if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					if (built)
					{
						edge0->face_[1] = triangle;
						triangle->vertex_[0]->faces_.insert(triangle);
						triangle->vertex_[1]->faces_.insert(triangle);
						triangle->vertex_[2]->faces_.insert(triangle);
						if (old1 == true)
						{
							if (edge1->face_[0] == NULL)
							{
								edge1->face_[0] = triangle;
							}
							else
							{
								edge1->face_[1] = triangle;
							}
									#ifdef print_debug_info
									std::cout << pre << "remove Edge " << *edge1 << " (" << edge1 << ") from\n"
														<< pre << "  border\n";
									#endif
							border.remove(edge1);
							planar_edges.remove(edge1);
						}
						else
						{
							edge1->face_[0] = triangle;
							if ((edge1->vertex_[0]->index_ > 0) ||
									(edge1->vertex_[1]->index_ > 0))
							{
								border.push_back(edge1);
							}
							planar_edges.push_back(edge1);
							edge1->vertex_[0]->edges_.insert(edge1);
							edge1->vertex_[1]->edges_.insert(edge1);
									#ifdef with_indices
									edge1->index_ = part.number_of_edges_;
									#endif
							part.edges_.push_back(edge1);
							part.number_of_edges_++;
									#ifdef print_debug_info
									std::cout << pre << "pushe Edge " << *edge1 << " (" << edge1 << ") to\n"
														<< pre << "  planar_edges\n" << pre << "  part\n";
									#endif
						}
						if (old2 == true)
						{
							if (edge2->face_[0] == NULL)
							{
								edge2->face_[0] = triangle;
							}
							else
							{
								edge2->face_[1] = triangle;
							}
									#ifdef print_debug_info
									std::cout << pre << "remove Edge " << *edge2 << " (" << edge2 << ") from\n"
														<< pre << "  border\n";
									#endif
							border.remove(edge2);
							planar_edges.remove(edge2);
						}
						else
						{
							edge2->face_[0] = triangle;
							if ((edge2->vertex_[0]->index_ > 0) ||
									(edge2->vertex_[1]->index_ > 0))
							{
								border.push_back(edge2);		
							}
							planar_edges.push_back(edge2);
							edge2->vertex_[0]->edges_.insert(edge2);
							edge2->vertex_[1]->edges_.insert(edge2);
									#ifdef with_indices
									edge2->index_ = part.number_of_edges_;
									#endif
							part.edges_.push_back(edge2);
							part.number_of_edges_++;
									#ifdef print_debug_info
									std::cout << pre << "pushe Edge " << *edge2 << " (" << edge2 << ") to\n"
														<< pre << "  planar_edges\n" << pre << "  part\n";
									#endif
						}
						border.remove(edge0);
								#ifdef with_indices
								triangle->index_ = part.number_of_triangles_;
								#endif
						part.triangles_.push_back(triangle);
						part.number_of_triangles_++;
								#ifdef print_debug_info
								std::cout << pre << "remove Edge" << *edge0 << " (" << edge0 << ") from\n"
													<< pre << "  border\n";
								std::cout << pre << "pushe Triangle " << *triangle << " (" << triangle << ") to\n"
													<< pre << "  part";
								if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
								#ifdef debug_triangulation
								Atom* atom1 = new Atom;
								Atom* atom2 = new Atom;
								Atom* atom3 = new Atom;
								atom1->setPosition(triangle->vertex_[0]->point_);
								atom2->setPosition(triangle->vertex_[1]->point_);
								atom3->setPosition(triangle->vertex_[2]->point_);
								atom1->setElement(PTE[Element::H]);
								atom2->setElement(PTE[Element::H]);
								atom3->setElement(PTE[Element::H]);
								#ifdef with_bonds
								atom1->createBond(*atom2);
								atom2->createBond(*atom3);
								atom3->createBond(*atom1);
								#endif
								sewMolecule->insert(*atom1);
								sewMolecule->insert(*atom2);
								sewMolecule->insert(*atom3);
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


	void SASTriangulator::createTriangleAndEdges
			(TriangleEdge*	edge,
			 TrianglePoint* point,
			 const TSphere3<double>& sphere,
			 TriangleEdge*& edge1,
			 bool&					old1,
			 TriangleEdge*& edge2,
			 bool&					old2,
			 Triangle*&			triangle
			)
		throw()
	{
		// build two new edges
		edge1	= new TriangleEdge;
		edge1->vertex_[0] = edge->vertex_[0];
		edge1->vertex_[1] = point;
		edge2 = new TriangleEdge;
		edge2->vertex_[0] = point;
		edge2->vertex_[1] = edge->vertex_[1];
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "zwei Edges konstruiert\n"
									<< pre << "  " << *edge1 << " (" << edge1 << ")\n"
									<< pre << "  " << *edge2 << " (" << edge2 << ")\n";
				#endif
		// do the edges exist ?
		TriangleEdge* test = edge->vertex_[0]->has(edge1);
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
		test = edge->vertex_[1]->has(edge2);
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
		triangle = new Triangle;
		triangle->vertex_[0] = edge->vertex_[1];
		triangle->vertex_[1] = edge->vertex_[0];
		triangle->vertex_[2] = point;
		triangle->edge_[0] = edge;
		triangle->edge_[1] = edge1;
		triangle->edge_[2] = edge2;
		// swap triangle if necessary
		TVector3<double> test_vector((triangle->vertex_[1]->point_-
																triangle->vertex_[0]->point_)%
														(triangle->vertex_[2]->point_-
																triangle->vertex_[0]->point_));
		double test_value = test_vector*(sphere.p-triangle->vertex_[0]->point_);
		if (Maths::isGreater(test_value,0))
		{
			TrianglePoint* temp = triangle->vertex_[0];
			triangle->vertex_[0] = triangle->vertex_[1];
			triangle->vertex_[1] = temp;
		}
				#ifdef print_debug_info
				std::cout << pre << "Dreieck gebaut: " << *triangle << " (" << triangle << ")\n";
				#endif
	}


	void SASTriangulator::partitionOfCircle
			(const TCircle3<double>&					circle,
			 const TVector3<double>&					p0,
			 const TAngle<double>&						phi,
			 Size												number_of_segments,
			 std::list< TVector3<double> >&	partition,
			 bool												on_surface)
		throw()
	{
		TVector4<double> p;
		if (on_surface)
		{
			TVector3<double> p_(p0-circle.p);
			p.set(p_.x,p_.y,p_.z,0.0);
		}
		else
		{
			p.set(circle.n.y,-circle.n.x,0.0,0.0);
			if (p == TVector4<double>::getZero())
			{
				p.set(circle.n.z,0.0,-circle.n.x,0.0);
			}
			p.normalize();
			p *= circle.radius;
		}
		TQuaternion<double> rotate(circle.n,phi);
		TMatrix4x4<double> rotation;
		rotate.getRotationMatrix(rotation);
		partition.push_back(TVector3<double>(p.x,p.y,p.z)+circle.p);
		for (Size i = 0; i < number_of_segments; i++)
		{
			p = rotation*p;
			partition.push_back(TVector3<double>(p.x,p.y,p.z)+circle.p);
		}
	}


	Size SASTriangulator::numberOfRefinements
			(const double& density, const double& radius)
		throw()
	{
		double test0 = (4.0*density*Constants::PI*radius*radius-12.0)/30.0;
		Size n = 0;
		if (Maths::isGreaterOrEqual(test0,0.0))
		{
			double test1 = 1;
			double test2 = 1;
			while (Maths::isLess(test2,test0))
			{
				test1 = test2;
				test2 *= 4;
				n++;
			}
			if (Maths::isLess(test2-test0,test0-test1))
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


	void SASTriangulator::buildTemplateSpheres()
		throw()
	{
		TriangulatedSphere::ConstPointIterator p;
		TrianglePoint* point;
		TriangulatedSphere sphere;
		sphere.icosaeder(true);
		for (p = sphere.beginPoint(); p != sphere.endPoint(); p++)
		{
			point = new TrianglePoint;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			template_spheres_[0].push_back(point->point_);
		}
		sphere.refine(1,true);
		for (p = sphere.beginPoint(); p != sphere.endPoint(); p++)
		{
			point = new TrianglePoint;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			template_spheres_[1].push_back(point->point_);
		}
		sphere.refine(1,true);
		for (p = sphere.beginPoint(); p != sphere.endPoint(); p++)
		{
			point = new TrianglePoint;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			template_spheres_[2].push_back(point->point_);
		}
		sphere.refine(1,true);
		for (p = sphere.beginPoint(); p != sphere.endPoint(); p++)
		{
			point = new TrianglePoint;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			template_spheres_[3].push_back(point->point_);
		}
	}





	#ifdef debug_triangulation
	void SASTriangulator::printToHINFile(string filename)
	{
		Molecule* molecule = new Molecule;
		#ifdef with_bonds
		std::list<Triangle*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			Atom* atom1 = new Atom;
			atom1->setPosition((*t)->vertex_[0]->point_);
			atom1->setElement(PTE[Element::H]);
			molecule->insert(*atom1);
				#ifdef with_normals
				Atom* normal1 = new Atom;
				normal1->setPosition((*t)->vertex_[0]->point_+(*t)->vertex_[0]->normal_);
				normal1->createBond(*atom1);
				normal1->setElement(PTE[Element::O]);
				molecule->insert(*normal1);
				#endif
			Atom* atom2 = new Atom;
			atom2->setPosition((*t)->vertex_[1]->point_);
			atom2->setElement(PTE[Element::H]);
			molecule->insert(*atom2);
				#ifdef with_normals
				Atom* normal2 = new Atom;
				normal2->setPosition((*t)->vertex_[1]->point_+(*t)->vertex_[1]->normal_);
				normal2->createBond(*atom2);
				normal2->setElement(PTE[Element::O]);
				molecule->insert(*normal1);
				#endif
			Atom* atom3 = new Atom;
			atom3->setPosition((*t)->vertex_[2]->point_);
			atom3->setElement(PTE[Element::H]);
			molecule->insert(*atom3);
				#ifdef with_normals
				Atom* normal3 = new Atom;
				normal3->setPosition((*t)->vertex_[2]->point_+(*t)->vertex_[2]->normal_);
				normal3->createBond(*atom3);
				normal3->setElement(PTE[Element::O]);
				molecule->insert(*normal1);
				#endif
			atom1->createBond(*atom3);
			atom1->createBond(*atom2);
			atom3->createBond(*atom2);
		}
		#else
		std::list<TrianglePoint*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
			Atom* new_atom = new Atom;
			new_atom->setPosition((*p)->point_);
			new_atom->setElement(PTE[Element::H]);
			atom.push_back(new_atom);
			molecule->insert(*new_atom);
				#ifdef with_normals
				Atom* normal = new Atom;
				normal->setPosition((*p)->point_+(*p)->normal_);
				normal->createBond(*new_atom);
				normal->setElement(PTE[Element::O]);
				molecule->insert(*normal);
				#endif
		}
		#endif
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(filename,std::ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}


	void SASTriangulator::Contour2HIN(const std::list<TriangleEdge*>& contour, const string& file)
	{
		HashSet<TrianglePoint*> points;
		std::list<TriangleEdge*>::const_iterator c;
		for (c = contour.begin(); c != contour.end(); c++)
		{
			points.insert((*c)->vertex_[0]);
			points.insert((*c)->vertex_[1]);
		}
		Molecule* molecule = new Molecule;
		for (c = contour.begin(); c != contour.end(); c++)
		{
			Atom* atom1 = new Atom;
			atom1->setPosition((*c)->vertex_[0]->point_);
			atom1->setElement(PTE[Element::H]);
			Atom* atom2 = new Atom;
			atom2->setPosition((*c)->vertex_[1]->point_);
			atom2->setElement(PTE[Element::H]);
			atom1->createBond(*atom2);
			molecule->insert(*atom1);
			molecule->insert(*atom2);
		}
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(file,std::ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}


	void SASTriangulator::SASEdge2HIN(SASEdge* edge, const string& file)
	{
		std::list<TriangleEdge*> contour;
		std::list<TrianglePoint*> contour_points;
		if (edge->vertex_[0] == NULL)
		{
			TAngle<double> phi(2*Constants::PI,true);
			Size number_of_segments
					= (Size)Maths::round(phi.value*edge->circle_.radius*2.12);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<double> psi(phi.value/number_of_segments,true);
			std::vector< TVector3<double> > points;
			partitionOfCircle(edge->circle_,TVector3<double>::getZero(),psi,number_of_segments,points,false);
			points.pop_back();
			TrianglePoint* p0;
			TrianglePoint* p1;
			TrianglePoint* p2;
			TriangleEdge* e;
			p1 = new TrianglePoint;
			p1->point_ = points[0];
			p1->normal_ = edge->circle_.p-p1->point_;
			contour_points.push_back(p1);
			p0 = p1;
			for (Position k = 1; k < points.size(); k++)
			{
				p2 = new TrianglePoint;
				p2->point_ = points[k];
				p2->normal_ = edge->circle_.p-p2->point_;
				contour_points.push_back(p2);
				e = new TriangleEdge;
				e->vertex_[0] = p1;
				e->vertex_[1] = p2;
				contour.push_back(e);
				p1->edges_.insert(e);
				p2->edges_.insert(e);
				p1 = p2;
			}
			e = new TriangleEdge;
			e->vertex_[0] = p1;
			e->vertex_[1] = p0;
			contour.push_back(e);
			p1->edges_.insert(e);
			p0->edges_.insert(e);
		}
		else
		{
			TAngle<double> phi;
			GetTAngle<double>(edge->vertex_[0]->point_-edge->circle_.p,
							 edge->vertex_[1]->point_-edge->circle_.p,
							 phi);
			Size number_of_segments
					= (Size)Maths::round(phi.value*edge->circle_.radius*2.12);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<double> psi(phi.value/number_of_segments,true);
			TVector3<double> normal((edge->vertex_[0]->point_-edge->circle_.p)%
												 (edge->vertex_[1]->point_-edge->circle_.p));
			edge->circle_.n = normal;
			std::vector< TVector3<double> > points;
			partitionOfCircle(edge->circle_,edge->vertex_[0]->point_,psi,number_of_segments,
												points);
			points.pop_back();
			points.push_back(edge->vertex_[1]->point_);
			TrianglePoint* p1;
			TrianglePoint* p2;
			TriangleEdge* e;
			p1 = new TrianglePoint;
			p1->point_ = points[0];
			p1->normal_ = edge->circle_.p-p1->point_;
			contour_points.push_back(p1);
			for (Position k = 1; k < points.size(); k++)
			{
				p2 = new TrianglePoint;
				p2->point_ = points[k];
				p2->normal_ = edge->circle_.p-p2->point_;
				contour_points.push_back(p2);
				e = new TriangleEdge;
				e->vertex_[0] = p1;
				e->vertex_[1] = p2;
				contour.push_back(e);
				p1->edges_.insert(e);
				p2->edges_.insert(e);
				p1 = p2;
			}
		}
		Contour2HIN(contour,file);
		std::list<TriangleEdge*>::iterator e;
		for (e = contour.begin(); e != contour.end(); e++)
		{
			delete *e;
		}
		std::list<TrianglePoint*>::iterator p;
		for (p = contour_points.begin(); p != contour_points.end(); p++)
		{
			delete *p;
		}
	}
	#endif


}	//namespace BALL
