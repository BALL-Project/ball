// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSAS.C,v 1.5 2004/02/18 19:03:30 oliver Exp $

#	include <BALL/KERNEL/atom.h>
#	include <BALL/KERNEL/molecule.h>
#	include <BALL/KERNEL/system.h>
#	include <BALL/KERNEL/PTE.h>
#	include <BALL/FORMAT/HINFile.h>
#	include <BALL/DATATYPE/string.h>

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

void PartitionOfCircle(const TCircle3<double>& circle, std::list<Vector3>& partition)
{
	Size density = 64;
	Vector3 center(circle.p.x,circle.p.y,circle.p.z);
	Vector3 normal(circle.n.x,circle.n.y,circle.n.z);
	Vector4 p;
	p.set(normal.y,-normal.x,0.0,0.0);
	if (p == Vector4::getZero())
	{
		p.set(normal.z,0.0,-normal.x,0.0);
	}
	p.normalize();
	p *= circle.radius;
	Quaternion rotate(normal,Angle(Constants::PI/density,true));
	Matrix4x4 rotation;
	rotate.getRotationMatrix(rotation);
	partition.push_back(Vector3(p.x,p.y,p.z)+center);
	for (Size i = 0; i < 2*density+1; i++)
	{
		p = rotation*p;
		partition.push_back(Vector3(p.x,p.y,p.z)+center);
	}
}

void Circle2HIN(const TCircle3<double>& circle, const String& file)
{
	std::list<Vector3> points;
	PartitionOfCircle(circle,points);
	Molecule* molecule = new Molecule;
	Atom* a1;
	Atom* a2;
	a1 = new Atom;
	a1->setPosition(points.front());
	a1->setElement(PTE[Element::H]);
	molecule->insert(*a1);
	std::list<Vector3>::iterator p;
	for (p = points.begin(); p != points.end(); p++)
	{
		a2 = new Atom;
		a2->setPosition(*p);
		a2->setElement(PTE[Element::H]);
		a1->createBond(*a2);
		a1 = a2;
		molecule->insert(*a1);
	}
	a1 = new Atom;
	a1->setPosition(Vector3(circle.p.x,circle.p.y,circle.p.z));
	a1->setElement(PTE[Element::H]);
	a2 = new Atom;
	a2->setPosition(Vector3(circle.p.x,circle.p.y,circle.p.z)+Vector3(circle.n.x,circle.n.y,circle.n.z));
	a2->setElement(PTE[Element::H]);
	a1->createBond(*a2);
	molecule->insert(*a1);
	molecule->insert(*a2);
	System* system = new System;
	system->insert(*molecule);
	HINFile output(file,std::ios::out);
	output << *system;
	output.close();
	delete system;
}

void Plane2HIN(const TPlane3<double>& plane, const String& file, Position size = 20)
{
	std::vector<Atom*> atom;
	Vector3 d1;
	Vector3 d2;
	Vector3 origin(plane.p.x,plane.p.y,plane.p.z);
	if (Maths::isZero(plane.n.x))
	{
		d1.set(0,plane.n.z,-plane.n.y);
	}
	else
	{
		d1.set(plane.n.y,-plane.n.x,0);
	}
	d2 = Vector3(plane.n.x,plane.n.y,plane.n.z)%d1;
	d1.normalize();
	d2.normalize();
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
		atom[i]->createBond(*atom[i+size]);
	}
	for (Position i = 0; i < size*size; i++)
	{
		if ((i%size) != 0)
		{
			atom[i]->createBond(*atom[i-1]);
		}
	}
	origin += d1*(size/2);
	origin += d2*(size/2);
	Vector3 normal(plane.n.x,plane.n.y,plane.n.z);
	normal.normalize();
	Atom* a1 = new Atom;
	a1->setPosition(origin+normal);
	a1->setElement(PTE[Element::H]);
	Atom* a2 = new Atom;
	a2->setPosition(origin);
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

//////////////////////////////////////////////////

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
		// build template spheres with different densities and outside normal vectors
		buildTemplateSpheres();
		// use tese to triangulete the sas faces
		for (Position i = 0; i < tsas_->sas_->number_of_faces_; i++)
		{
			triangulateFace(tsas_->sas_->faces_[i]);
		}
	}


	void SASTriangulator::triangulateFace(SASFace* face)
		throw()
	{
		// store the planes of the SAS edges
		std::list< std::pair<TPlane3<double>,double> > planes;
		createPlanes(face,planes);
		// get template sphere
		TSphere3<double>* sphere = &face->sphere_;
		HashMap<Size,TriangulatedSurface>::ConstIterator s
			= template_spheres_.find(numberOfRefinements(tsas_->density_,
																									 sphere->radius));
		TriangulatedSurface part = s->second;
		part.blowUp(sphere->radius);
		part.shift(sphere->p);
		// tag inside points
		tagPoints(part,planes);
		// remove inside triangles and then isolated edges and points
		removeInsideTriangles(part);
		part.deleteIsolatedEdges();
		part.deleteIsolatedPoints();
		// create HashGrid of triangle points
//		HashGrid3<TrianglePoint*> point_grid(createHashGrid(part));
		// create points on the cutting planes
//		createPoints(part,planes,point_grid);
		// create new triangles
//		createNewTriangles(part,point_grid);
		// remove isolated edges and points
//		part.deleteIsolatedEdges();
//		part.deleteIsolatedPoints();
		// join the triangulation of this sas face with the sas
		tsas_->join(part);
	}


	void SASTriangulator::createPlanes
			(SASFace* face,
			 std::list< std::pair<TPlane3<double>,double> >& planes)
		throw()
	{
		std::pair<TPlane3<double>,double> plane;
		std::list<SASEdge*>::iterator edge;
		std::list<bool>::iterator o = face->orientation_.begin();
		for (edge = face->edge_.begin(); edge != face->edge_.end(); edge++)
		{
			plane.first.p = (*edge)->circle_.p;
			if (*o)
			{
				plane.first.n = (*edge)->circle_.n;
			}
			else
			{
				plane.first.n = -(*edge)->circle_.n;
			}
			plane.second = plane.first.n*plane.first.p;
			planes.push_back(plane);
			o++;
		}
	}


	void SASTriangulator::tagPoints
			(TriangulatedSurface& part,
			 const std::list< std::pair<TPlane3<double>,double> >& planes)
		throw()
	{
		TriangulatedSurface::PointIterator p;
		std::list< std::pair<TPlane3<double>,double> >::const_iterator plane;
		for (p = part.beginPoint(); p != part.endPoint(); p++)
		{
			(*p)->index_ = 0;
			plane = planes.begin();
			while (plane != planes.end())
			{
				if (Maths::isLessOrEqual(plane->first.n*(*p)->point_,plane->second))
				{
					(*p)->index_ = 1;
					plane = planes.end();
				}
				else
				{
					plane++;
				}
			}
		}
	}


	void SASTriangulator::removeInsideTriangles(TriangulatedSurface& part)
		throw()
	{
		TriangulatedSurface::TriangleIterator t1;
		TriangulatedSurface::TriangleIterator t2;
		t1 = part.beginTriangle();
		while (t1 != part.endTriangle())
		{
			if ((*t1)->vertex_[0]->index_+
			    (*t1)->vertex_[1]->index_+
			    (*t1)->vertex_[2]->index_  == 3)
			{
				t2 = t1;
				t2++;
				if (t2 == part.endTriangle())
				{
					part.remove(t1);
					t1 = part.endTriangle();
				}
				else
				{
					part.remove(t1);
					t1 = t2;
				}
			}
			else
			{
				t1++;
			}
		}
	}


	HashGrid3<TrianglePoint*> SASTriangulator::createHashGrid(const TriangulatedSurface& part)
		throw()
	{
		double x_min = (*(part.beginPoint()))->point_.x;
		double y_min = (*(part.beginPoint()))->point_.y;
		double z_min = (*(part.beginPoint()))->point_.z;
		double x_max = (*(part.beginPoint()))->point_.x;
		double y_max = (*(part.beginPoint()))->point_.y;
		double z_max = (*(part.beginPoint()))->point_.z;
		TriangulatedSurface::ConstPointIterator p = part.beginPoint()++;
		while (p != part.endPoint())
		{
			x_min = Maths::min(x_min,(*p)->point_.x);
			y_min = Maths::min(y_min,(*p)->point_.y);
			z_min = Maths::min(z_min,(*p)->point_.z);
			x_max = Maths::max(x_min,(*p)->point_.x);
			y_max = Maths::max(y_max,(*p)->point_.y);
			z_max = Maths::max(z_max,(*p)->point_.z);
			p++;
		}
		//double dist = ses_->reduced_surface_->r_max_/3;
		double dist = 1.0;
		Position nx = (Position)((x_max-x_min)/dist+5);
		Position ny = (Position)((y_max-y_min)/dist+5);
		Position nz = (Position)((z_max-z_min)/dist+5);
		Vector3 origin(x_min-2*dist,y_min-2*dist,z_min-2*dist);
		HashGrid3<TrianglePoint*> grid = HashGrid3<TrianglePoint*>(origin,nx,ny,nz,dist);
		p = part.beginPoint()++;
		while (p != part.endPoint())
		{
			grid.insert(Vector3((*p)->point_.x,(*p)->point_.y,(*p)->point_.z),*p);
			p++;
		}
		return grid;
	}


	void SASTriangulator::createPoints
			(TriangulatedSurface& part,
			 const std::list< std::pair<TPlane3<double>,double> >& planes,
			 HashGrid3<TrianglePoint*>& grid)
		throw()
	{
		TriangulatedSurface::EdgeIterator edge = part.beginEdge();
		std::list< std::pair<TPlane3<double>,double> >::const_iterator plane;
		while (edge != part.endEdge())
		{
			if ((*edge)->vertex_[0]->index_+(*edge)->vertex_[1]->index_ == 1)
			{
				// edge intersects one of the cutting planes
				TrianglePoint* v1;
				TrianglePoint* v2;
				if ((*edge)->vertex_[0]->index_ == 0)
				{
					v1 = (*edge)->vertex_[0];
					v2 = (*edge)->vertex_[1];
				}
				else
				{
					v1 = (*edge)->vertex_[1];
					v2 = (*edge)->vertex_[0];
				}
				TVector3<double> diff(v2->point_-v1->point_);
				TVector3<double> point(v2->point_);
				double min = 1.0;
				double div;
				Size counter = 0;
				for (plane = planes.begin(); plane != planes.end(); plane++)
				{
					// intersecting point  =  v1 + lambda * (v2-v1)
					div = plane->first.n*diff;
					if (Maths::isNotZero(div))
					{
						double lambda = (plane->second-(plane->first.n*v1->point_))/div;
						if (Maths::isGreaterOrEqual(lambda,0) &&
							 (Maths::isLessOrEqual(lambda,min)))
						{
							min = lambda;
							point.set(v1->point_+(lambda*diff));
							(*edge)->index_ = counter;
						}
					}
					counter++;
				}
				v2->edges_.erase(*edge);
				TrianglePoint* new_point = vertexExists(point,grid);
				if (new_point == NULL)
				{
					new_point = new TrianglePoint;
					new_point->point_ = point;
					new_point->index_ = -1;
					new_point->edges_.insert(*edge),
					part.insert(new_point);
					grid.insert(Vector3(point.x,point.y,point.z),new_point);
				}
				if ((*edge)->vertex_[0] == v1)
				{
					(*edge)->vertex_[1] = new_point;
				}
				else
				{
					(*edge)->vertex_[0] = new_point;
				}
			}
			else
			{
				(*edge)->index_ = -1;
			}
			edge++;
		}
	}


	void SASTriangulator::createNewTriangles
			(TriangulatedSurface& part,
			 HashGrid3<TrianglePoint*>& grid)
		throw()
	{
		TriangulatedSurface::TriangleIterator t = part.beginTriangle();
		while (t != part.endTriangle())
		{
			Position type = 0;
			if ((*t)->vertex_[0]->index_ == 1)
			{
				type++;
			}
			if ((*t)->vertex_[1]->index_ == 1)
			{
				type += 2;
			}
			if ((*t)->vertex_[2]->index_ == 1)
			{
				type += 4;
			}
			switch (type)
			{
				case 0 :  break; // triangle not intersected
				case 1 :  onePointOutside(0,*t,part,grid);    break;
				case 2 :  onePointOutside(1,*t,part,grid);    break;
				case 3 :  twoPointsOutside(0,1,*t,part,grid); break;
				case 4 :  onePointOutside(2,*t,part,grid);    break;
				case 5 :  twoPointsOutside(2,0,*t,part,grid); break;
				case 6 :  twoPointsOutside(1,2,*t,part,grid); break;
				case 7 :  break; // should never happen
			}
			t++;
		}
	}


	void SASTriangulator::onePointOutside
			(Index outside,
			 Triangle* t,
			 TriangulatedSurface& part,
			 HashGrid3<TrianglePoint*>& grid)
		throw()
	{
std::cout << "onePointOutside ...\n";
		// get the relative indices of the intersected edges
		Position edge[3];
		Position i = 0;
		for (Position j = 0; j < 3; j++)
		{
			if (t->edge_[j]->index_ != -1)
			{
				edge[i] = j;
				i++;
			}
			else
			{
				edge[2] = j;
			}
		}
		// v1 = new vertex of the first intersected edge
		// v2 = old vertex of the first intersected edge
		// v3 = new vertex of the second intersected edge
		// v4 = old vertex of the second intersected edge
		TrianglePoint* v1;
		TrianglePoint* v2;
		TrianglePoint* v3;
		TrianglePoint* v4;
		Position test = ((t->edge_[edge[0]]->vertex_[0]->index_ == -1) ? 0 : 1);
		v1 = t->edge_[edge[0]]->vertex_[test];
		v2 = t->edge_[edge[0]]->vertex_[1-test];
		test = ((t->edge_[edge[1]]->vertex_[0]->index_ == -1) ? 0 : 1);
		v3 = t->edge_[edge[1]]->vertex_[test];
		v4 = t->edge_[edge[1]]->vertex_[1-test];

		// get the relative index of v4 which is needed
		// to compute the correct orientation of the first new triangle
		Index index = 0;
		for (Position j = 0; j < 3; j++)
		{
			if (t->vertex_[j] == v4)
			{
				index = j;
			}
		}
		// resize the original triangle
		t->vertex_[outside]->faces_.erase(t);
		t->vertex_[outside] = v1;
		v1->faces_.insert(t);
		// create a new triangle
		Triangle* new_triangle = new Triangle;
		new_triangle->vertex_[0] = v1;
		if ((outside-index == 1) || (outside-index == -2))
		{
			new_triangle->vertex_[1] = v4;
			new_triangle->vertex_[2] = v3;
		}
		else
		{
			new_triangle->vertex_[1] = v3;
			new_triangle->vertex_[2] = v4;
		}
		v1->faces_.insert(new_triangle);
		v3->faces_.insert(new_triangle);
		v4->faces_.insert(new_triangle);
		part.insert(new_triangle);
		// does the triangle intersect two different SAS edges?
		if (t->edge_[edge[0]]->index_ != t->edge_[edge[1]]->index_)
		{
			// ACHTUNG: pos MUSS NOCH BERECHNET WERDEN !!!
			TVector3<double> pos(v1->point_);
			TrianglePoint* new_point = vertexExists(pos,grid);
			if (new_point == NULL)
			{
				new_point = new TrianglePoint;
				new_point->point_ = pos;
				new_point->index_ = -1;
				part.insert(new_point);
				grid.insert(Vector3(pos.x,pos.y,pos.z),new_point);
			}
			Triangle* third_triangle = new Triangle;
			third_triangle->vertex_[0] = v1;
			if ((outside-index == 1) || (outside-index == -2))
			{
				third_triangle->vertex_[1] = v3;
				third_triangle->vertex_[2] = new_point;
			}
			else
			{
				third_triangle->vertex_[1] = new_point;
				third_triangle->vertex_[2] = v3;
			}
			v1->faces_.insert(third_triangle);
			v3->faces_.insert(third_triangle);
			new_point->faces_.insert(third_triangle);
			part.insert(third_triangle);
		}
std::cout << "... ok\n";
	}


	void SASTriangulator::twoPointsOutside
			(Position outside1,
			 Position outside2,
			 Triangle* t,
			 TriangulatedSurface& part,
			 HashGrid3<TrianglePoint*>& grid)
		throw()
	{
std::cout << "twoPointsOutside ...\n";
		// get the relative indices of the intersected edges
		Position edge[3];
		Position i = 0;
		for (Position j = 0; j < 3; j++)
		{
			if (t->edge_[j]->index_ != -1)
			{
				edge[i] = j;
				i++;
			}
			else
			{
				edge[2] = j;
			}
		}
		// v1 = new vertex of the first intersected edge
		// v2 = old vertex of the first intersected edge
		// v3 = new vertex of the second intersected edge
		TrianglePoint* v1;
		TrianglePoint* v2;
		TrianglePoint* v3;
		Position test = ((t->edge_[edge[0]]->vertex_[0]->index_ == -1) ? 0 : 1);
		v1 = t->edge_[edge[0]]->vertex_[test];
		v2 = t->edge_[edge[0]]->vertex_[1-test];
		test = ((t->edge_[edge[1]]->vertex_[0]->index_ == -1) ? 0 : 1);
		v3 = t->edge_[edge[1]]->vertex_[test];
		// resize the original triangle
		t->vertex_[outside1]->faces_.erase(t);
		t->vertex_[outside2]->faces_.erase(t);
		TLine3<double> line(v1->point_,v2->point_,TLine3<double>::FORM__TWO_POINTS);
		bool one = line.has(t->vertex_[outside1]->point_);
		if (one)
		{
			t->vertex_[outside1] = v1;
			t->vertex_[outside2] = v3;
		}
		else
		{
			t->vertex_[outside1] = v3;
			t->vertex_[outside2] = v1;
		}
		v1->faces_.insert(t);
		v2->faces_.insert(t);
		// does the triangle intersect two different SAS edges?
		if (t->edge_[edge[0]]->index_ != t->edge_[edge[1]]->index_)
		{
			// ACHTUNG: pos MUSS NOCH BERECHNET WERDEN !!!
			TVector3<double> pos(v1->point_);
			TrianglePoint* new_point = vertexExists(pos,grid);
			if (new_point == NULL)
			{
				new_point = new TrianglePoint;
				new_point->point_ = pos;
				new_point->index_ = -1;
				part.insert(new_point);
				grid.insert(Vector3(pos.x,pos.y,pos.z),new_point);
			}
			Triangle* new_triangle = new Triangle;
			new_triangle->vertex_[0] = t->vertex_[outside2];
			new_triangle->vertex_[1] = t->vertex_[outside1];
			new_triangle->vertex_[2] = new_point;
			v1->faces_.insert(new_triangle);
			v3->faces_.insert(new_triangle);
			new_point->faces_.insert(new_triangle);
			part.insert(new_triangle);
		}
std::cout << "... ok\n";
	}


	TrianglePoint* SASTriangulator::vertexExists
			(const TVector3<double>& point,
			 HashGrid3<TrianglePoint*>& grid)
		throw()
	{
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 0.001;
		Vector3 p(point.x,point.y,point.z);
		HashGridBox3<TrianglePoint*>* box = grid.getBox(p);
		HashGridBox3<TrianglePoint*>::ConstBoxIterator b;
		HashGridBox3<TrianglePoint*>::ConstDataIterator d;
		if (box != NULL)
		{
			for (b = box->beginBox(); b != box->endBox(); b++)
			{
				for (d = b->beginData(); d != b->endData(); d++)
				{
					if ((*d)->point_ == point)
					{
						Constants::EPSILON = epsilon;
						return *d;
					}
				}
			}
		}
		Constants::EPSILON = epsilon;
		return NULL;
	}


	Size SASTriangulator::numberOfRefinements(const double& density, const double& radius)
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
		if (n > 4)
		{
			n = 4;
		}
		return n;
	}


	void SASTriangulator::buildTemplateSpheres()
		throw()
	{
		TriangulatedSphere sphere;
		sphere.icosaeder(true);
		sphere.setIndices();
		template_spheres_[0] = sphere;
		sphere.refine(1,true);
		sphere.setIndices();
		template_spheres_[1] = sphere;
		sphere.refine(1,true);
		sphere.setIndices();
		template_spheres_[2] = sphere;
		sphere.refine(1,true);
		sphere.setIndices();
		template_spheres_[3] = sphere;
		sphere.refine(1,true);
		sphere.setIndices();
		template_spheres_[4] = sphere;
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
