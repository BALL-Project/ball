// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: triangulatedSES.h,v 1.6.4.2 2002/11/08 18:52:13 oliver Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSES_H
#define BALL_STRUCTURE_TRIANGULATEDSES_H

#ifdef DEBUG
//#	define with_indices
//#	define debug_triangulation
#	ifdef debug_triangulation
#		define with_bonds
//#		define with_normals
//#		define debug_triangulation_with_planes
#	endif
#	ifdef with_indices
#		define print_debug_info
#	endif
#endif

#ifdef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_VECTOR4_H
#	include <BALL/MATHS/vector4.h>
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

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEPOINT_H
#	include <BALL/STRUCTURE/trianglePoint.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLEEDGE_H
#	include <BALL/STRUCTURE/triangleEdge.h>
#endif

#ifndef BALL_STRUCTURE_TRIANGLE_H
#	include <BALL/STRUCTURE/triangle.h>
#endif

#ifndef BALL_TRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventExcludedSurface.h>
#endif

#include <list>
#include <vector>
#include <fstream>

namespace BALL
{

		#ifdef debug_triangulation
		int tindex = 0;
		Molecule* sewMolecule;
		System* sewSystem;
		#endif
	/** Generic TriangulatedSES Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangulatedSES.h}
			\\
	*/
	template <class T>
	class TTriangulatedSES	:	public TTriangulatedSurface<T>
	{
		
		public:

		#ifdef debug_triangulation
		void printToHINFile(string filename);
		void Contour2HIN(const std::list<TTriangleEdge<T>*>& contour, const string& file);
		void SESEdge2HIN(TSESEdge<T>* edge, const string& file);
		#endif

		BALL_CREATE(TTriangulatedSES)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSES object.
		*/
		TTriangulatedSES()
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSES object from another.
				@param	surface	the TriangulatedSES object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TTriangulatedSES(const TTriangulatedSES<T>& surface, bool = true)
			throw(Exception::GeneralException);

		/** Detailed constructor.
				Create a new empty TriangulatedSES object and set its SES.
				@param	ses			a pointer to the corresponding SES
				@param	density	the density to use by triangulation
		*/
		TTriangulatedSES(TSolventExcludedSurface<T>* ses, const T& density)
			throw();

		/**	Destructor.
				Destructs the TriangulatedSES object.
		*/
		virtual ~TTriangulatedSES()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSES.
				@param	surface	the TriangulatedSES object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TTriangulatedSES<T>& surface, bool = true)
			throw(Exception::GeneralException);

		/**	Assign from another TriangulatedSES.
				@param	surface	the TriangulatedSES object to assign from
		*/
		void operator=(const TTriangulatedSES<T>& surface)
			throw(Exception::GeneralException);

		/** Set the density used by triangulation.
		*/
		void setDensity(const T& density)
			throw();

		/** Get the density used by triangulation.
		*/
		T getDensity() const
			throw();

		//@}

		/**	@name	Predicates
		*/
		//@{
		//@}

		/** @name Accessors
		*/
		//@{

		void compute()
			throw(Exception::GeneralException,Exception::DivisionByZero);

		private:

		void partitionSingularEdge
				(TSESEdge<T>* singular_edge,
				 const T& sqrt_density,
				 const std::vector<TTrianglePoint<T>*>& point,
				 std::vector< std::list<TTriangleEdge<T>*> >& edge)
			throw();

		void triangulateToricFace
				(TSESFace<T>*																	face,
				 const T&																			radius_of_probe,
				 const T&																			sqrt_density,
				 std::vector<TTrianglePoint<T>*>							points,
				 std::vector<	std::list<TTriangleEdge<T>*> >& border)
			throw();

		void triangulateFreeToricFace
				(TSESFace<T>*														 face,
				 const T&																 radius_of_probe,
				 const T&																 sqrt_density,
				 std::vector<TTrianglePoint<T>*>				 points,
				 std::vector< list<TTriangleEdge<T>*> >& border)
			throw();

		void triangulateSingularToricFace
				(TSESFace<T>*																	face,
				 const T&																			radius_of_probe,
				 const T&																			sqrt_density,
				 std::vector< TTrianglePoint<T>* >						points,
				 std::vector< std::list<TTriangleEdge<T>*> >& border)
			throw();

		void triangulateContactFace
				(TSESFace<T>*																				face,
				 const TSphere3<T>&																	sphere,
				 //const HashMap<Size,TTriangulatedSES<T>*>&					spheres,
				 const HashMap< Size,std::list<TTrianglePoint<T>*> >&	spheres,
				 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours)
			throw();

		bool triangulateSphericFace
				(TSESFace<T>* face,
				 const TSphere3<T>&																	sphere,
				 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours)
			throw();

		bool buildSphericTriangles
				(			 TTriangulatedSES<T>&													part,
							 std::list<TTriangleEdge<T>*>&								border,
				 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
				 const std::list<TSESEdge<T>*>&											sesedge,
				 const TSphere3<T>&																	sphere,
			 			#ifdef print_debug_info
				 		int halt,
			 			#endif
			 	 bool																								convex = true)
			throw();

		bool buildFirstTriangle
				(			 TTriangulatedSES<T>&													part,
							 std::list<TTriangleEdge<T>*>&								border,
							 Size&																				border_size,
				 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
				 const std::list<TSESEdge<T>*>&											sesedge,
				 const TSphere3<T>&																	sphere,
				 const HashSet<TTrianglePoint<T>*>&									points,
							#ifdef print_debug_info
							int& halt,
							#endif
			 	 bool																								convex)
			throw();

		void buildUnambiguousTriangle
				(TTriangleEdge<T>*						 edge,
				 TTrianglePoint<T>*						 point,
				 std::list<TTriangleEdge<T>*>& border,
				 Size&												 border_size,
				 const TSphere3<T>&						 sphere,
				 TTriangulatedSES<T>&					 part,
							#ifdef print_debug_info
							int& halt,
							#endif
			 	 bool													 convex
				)
			throw();

		void buildAmbiguousTriangles
				(TTriangleEdge<T>*						 edge,
				 std::list<TTrianglePoint<T>*> points,
				 std::list<TTriangleEdge<T>*>& border,
				 Size&												 border_size,
				 const TSphere3<T>&						 sphere,
				 TTriangulatedSES<T>&					 part,
							#ifdef print_debug_info
							int& halt,
							#endif
			 	 bool													 convex
				)
			throw();

		void createTriangleAndEdges
				(TTriangleEdge<T>*	edge,
				 TTrianglePoint<T>* point,
				 const TSphere3<T>& sphere,
				 TTriangleEdge<T>*& edge1,
				 bool&							old1,
				 TTriangleEdge<T>*& edge2,
				 bool&							old2,
				 TTriangle<T>*&			triangle,
			 	 bool								convex
				)
			throw();

		void partitionOfCircle
				(const TCircle3<T>&			circle,
				 const TVector3<T>&			p0,
				 const TAngle<T>&				phi,
				 Size										number_of_segments,
				 vector< TVector3<T> >&	partition,
				 bool										on_surface = true)
			throw();

		Size numberOfRefinements(const T& density, const T& radius)
			throw();

		//void buildSpheres(HashMap<Size,TTriangulatedSES<T>*>& spheres)
		void buildSpheres(HashMap< Size,std::list<TTrianglePoint<T>*> >& spheres)
			throw();

		void buildTriangles
			 (TSESEdge<T>*																 edge0,
				TSESEdge<T>*																 edge1,
				TSESEdge<T>*																 edge2,
				TSESEdge<T>*																 edge3,
				const std::vector< TVector3<T> >&						 centers,
				const std::vector< TVector3<T> >&						 edge1_points,
				const std::vector< TVector3<T> >&						 edge3_points,
				std::vector<TTrianglePoint<T>*>							 existing_points,
				const T&																		 radius_of_probe,
				const T&																		 sqrt_density,
				std::vector< std::list<TTriangleEdge<T>*> >& border)
			throw();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		TSolventExcludedSurface<T>* ses_;

		T density_;

		//@}

	};


	/**	The Default TriangulatedSES type.
			If double precision is not needed, {\tt TTriangulatedSES<float>} should
			be used. It is predefined as {\tt TriangulatedSES} for convenience.
	*/
	typedef TTriangulatedSES<float> TriangulatedSES;



	template <class T>
	TTriangulatedSES<T>::TTriangulatedSES()
		throw()
		:	TTriangulatedSurface<T>(),
			ses_(NULL),
			density_(4.5)
	{
	}


	template <class T>
	TTriangulatedSES<T>::TTriangulatedSES(const TTriangulatedSES<T>& surface, bool deep)
		throw(Exception::GeneralException)
		:	TTriangulatedSurface<T>(surface,deep),
			ses_(surface.ses_),
			density_(surface.density_)
	{
	}


	template <class T>
	TTriangulatedSES<T>::TTriangulatedSES(TSolventExcludedSurface<T>* ses, const T& density)
		throw()
		:	TTriangulatedSurface<T>(),
			ses_(ses),
			density_(density)
	{
	}


	template <class T>
	TTriangulatedSES<T>::~TTriangulatedSES()
		throw()
	{
	}


	template <class T>
	void TTriangulatedSES<T>::set(const TTriangulatedSES<T>& surface, bool)
		throw(Exception::GeneralException)
	{
		*this = surface;
	}


	template <class T>
	void TTriangulatedSES<T>::operator=(const TTriangulatedSES<T>& surface)
		throw(Exception::GeneralException)
	{
		number_of_points_ = surface.number_of_points_;
		number_of_edges_ = surface.number_of_edges_;
		number_of_triangles_ = surface.number_of_triangles_;
		try
		{
			copy(surface);
		}
		catch (Exception::GeneralException)
		{
			typename std::list<TTrianglePoint<T>*>::iterator p;
			typename std::list<TTriangleEdge<T>*>::iterator e;
			typename std::list<TTriangle<T>*>::iterator t;
			for (p = points_.begin(); p != points_.end(); p++)
			{
				delete *p;
			}
			for (e = edges_.begin(); e != edges_.end(); e++)
			{
				delete *e;
			}
			for (t = triangles_.begin(); t != triangles_.end(); t++)
			{
				delete *t;
			}
			points_.clear();
			edges_.clear();
			triangles_.clear();
			number_of_points_ = 0;
			number_of_edges_ = 0;
			number_of_triangles_ = 0;
			throw;
		}
		ses_ = surface.ses_;
		density_ = surface.density_;
	}


	template <class T>
	void TTriangulatedSES<T>::setDensity(const T& density)
		throw()
	{
		density_ = density;
	}


	template <class T>
	T TTriangulatedSES<T>::getDensity() const
		throw()
	{
		return density_;
	}


	template <class T>
	void TTriangulatedSES<T>::compute()
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
		ses_->clean(density_);
				#ifdef debug_surface_processor_print
				ofstream file("SolventExcludedSurface.clean.log");
				file << *ses_;
				file.close();
				#endif
		ses_->splitSphericFaces();
				#ifdef debug_surface_processor_print
				file.open("SolventExcludedSurface.split.log");
				file << *ses_;
				file.close();
				#endif
		// Store for each SESVertex a TrianglePoint ...
		std::vector<TTrianglePoint<T>*> point(ses_->vertices_.size());
		for (Position i = 0;  i < ses_->vertices_.size(); i++)
		{
			point[i] = new TTrianglePoint<T>;
			point[i]->point_ = ses_->vertices_[i]->point_;
			point[i]->normal_ = ses_->vertices_[i]->normal_;
			points_.push_back(point[i]);
		}
		// ... and for each SESEdge a list of TriangleEdges.
		std::vector< std::list<TTriangleEdge<T>*> > edge(ses_->number_of_edges_);
		// Sometimes we need the square root of the density.
  	T sqrt_density = sqrt(density_);
		// At first we triangulate the toric faces.
				#ifdef debug_surface_processor
				std::cerr << "  triangulate toric faces\n";
				#endif
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 1e-4;
		T radius_of_probe(ses_->reduced_surface_->probe_radius_);
		for (Position i = 0; i < ses_->number_of_toric_faces_; i++)
		{
			triangulateToricFace(ses_->toric_faces_[i],radius_of_probe,sqrt_density,point,edge);
		}
		Constants::EPSILON = epsilon;
		// Apart from the singular SESEdges all SESEdges have now a partition in TriangleEdges.
		// Let's build such a partition for the singular SESEdges, too.
				#ifdef debug_surface_processor
				std::cerr << "  treat singular edges\n";
				#endif
		typename std::list<TSESEdge<T>*>::iterator se;
		for (se = ses_->singular_edges_.begin(); se != ses_->singular_edges_.end(); se++)
		{
			partitionSingularEdge(*se,sqrt_density,point,edge);
		}
				#ifdef print_debug_info
				for (Position i = 0; i < edge.size(); i++)
				{
					if ((edge[i].size() == 0) && (ses_->edges_[i] != NULL))
					{
						std::cout << "Edge" << i << " ohne Kontur!\n";
					}
				}
				#endif
		// Now we triangulate the contact faces.
				#ifdef debug_surface_processor
				std::cerr << "  triangulate contact faces\n";
				#endif
				#ifdef with_indices
				setIndices();
				#endif
		// build template spheres with different densities and outside normal vectors
		//HashMap<Size,TTriangulatedSES<T>*> spheres;
		HashMap< Size,std::list<TTrianglePoint<T>*> > spheres;
		buildSpheres(spheres);
		TSESFace<T>* current_face;
		TSphere3<T> sphere;
		for (Position i = 0; i < ses_->number_of_contact_faces_; i++)
		{
			current_face = ses_->contact_faces_[i];
			sphere = ses_->reduced_surface_->atom_[current_face->rsvertex_->atom_];
			triangulateContactFace(current_face,sphere,spheres,edge);
		}
		// And finaly, we triangulate the spheric faces.
				#ifdef debug_surface_processor
				std::cerr << "  triangulate spheric reentrant faces\n";
				#endif
		sphere.radius = ses_->reduced_surface_->probe_radius_;
		std::list<TSESFace<T>*> not_triangulated_faces;
		for (Position i = 0; i < ses_->number_of_spheric_faces_; i++)
		{
			sphere.p = ses_->spheric_faces_[i]->rsface_->center_;
			if (!triangulateSphericFace(ses_->spheric_faces_[i],sphere,edge))
			{
				not_triangulated_faces.push_back(ses_->spheric_faces_[i]);
			}
		}
		while (!not_triangulated_faces.empty())
		{
			current_face = not_triangulated_faces.front();
			not_triangulated_faces.pop_front();
			bool ok = false;
			typename std::list<TSESEdge<T>*>::iterator e = current_face->edge_.begin();
			while (!ok && (e != current_face->edge_.end()))
			{
				if (edge[(*e)->index_].front()->face_[0] != NULL)
				{
					(*e)->type_ = TSESEdge<T>::TYPE_CONCAVE;
					ok = true;
				}
				e++;
			}
			if (ok)
			{
				sphere.p = current_face->rsface_->center_;
				triangulateSphericFace(current_face,sphere,edge);
			}
			else
			{
				not_triangulated_faces.push_back(current_face);
			}
		}
	}


	template <class T>
	void TTriangulatedSES<T>::triangulateToricFace
				(TSESFace<T>*																	face,
				 const T&																			radius_of_probe,
				 const T&																			sqrt_density,
				 std::vector<TTrianglePoint<T>*>							points,
				 std::vector<	std::list<TTriangleEdge<T>*> >& border)
		throw()
	{
		if (face->isFree())
		{
						#ifdef debug_surface_processor_verbose
						std::cout << "    ... " << face->index_ << " ... free\n";
						#endif
			triangulateFreeToricFace(face,radius_of_probe,sqrt_density,points,border);
			return;
		}
		if (face->rsedge_->isSingular())
		{
						#ifdef debug_surface_processor_verbose
						std::cout << "    ... " << face->index_ << " ... singular\n";
						#endif
			triangulateSingularToricFace(face,radius_of_probe,sqrt_density,points,border);
			return;
		}
						#ifdef debug_surface_processor_verbose
						std::cout << "    ... " << face->index_ << "\n";;
						#endif
		//face->normalize(false);
		typename std::list<TSESEdge<T>*>::iterator e = face->edge_.begin();
		TSESEdge<T>* edge0 = *e;
		e++;
		TSESEdge<T>* edge1 = *e;
		e++;
		TSESEdge<T>* edge2 = *e;
		e++;
		TSESEdge<T>* edge3 = *e;
		typename std::list<TSESVertex<T>*>::iterator v = face->vertex_.begin();
		TSESVertex<T>* p0 = *v;
		v++;
		TSESVertex<T>* p1 = *v;
		//TSESVertex<T>* p2 = face->vertex_[2];
		//TSESVertex<T>* p3 = face->vertex_[3];
		// In how much segments the edge should be triangulated? This depends on the angle of the
		// corresponding RSEdge and the density.
		Size number_of_segments =
				(Size)Maths::round(face->rsedge_->phi_.value*edge3->circle_.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<T> psi(face->rsedge_->phi_.value/number_of_segments,true);
		// Get the normal vector of the rotation used to partition the SESEdges of the SESFace ...
		TCircle3<T> circle3(edge3->circle_);
		TCircle3<T> circle1(edge1->circle_);
		TVector3<T> normal(circle3.n);
		// ... and now partition the SESEdges ...
		vector< TVector3<T> > edge3_segments;
		vector< TVector3<T> > edge1_segments;
		partitionOfCircle(circle3,edge3->vertex_[0]->point_,psi,number_of_segments,edge3_segments);
		edge3_segments.pop_back();
		edge3_segments.push_back(edge3->vertex_[1]->point_);
		if (edge3->vertex_[0] != p0)
		{
			TVector3<T> tmp;
			for (Position i = 0; i < (number_of_segments+1)/2; i++)
			{
				tmp = edge3_segments[i];
				edge3_segments[i] = edge3_segments[number_of_segments-i];
				edge3_segments[number_of_segments-i] = tmp;
			}
			normal.negate();
		}
		partitionOfCircle(circle1,edge1->vertex_[0]->point_,psi,number_of_segments,edge1_segments);
		edge1_segments.pop_back();
		edge1_segments.push_back(edge1->vertex_[1]->point_);
		if (edge1->vertex_[0] != p1)
		{
			TVector3<T> tmp;
			for (Position i = 0; i < (number_of_segments+1)/2; i++)
			{
				tmp = edge1_segments[i];
				edge1_segments[i] = edge1_segments[number_of_segments-i];
				edge1_segments[number_of_segments-i] = tmp;
			}
		}
		TCircle3<T> center_circle(face->rsedge_->center_of_torus_,
															normal,
															face->rsedge_->radius_of_torus_);
		// ... and the "center_circle". This is the circle on which the centers oft the circle lie which
		// define the segments of the face.
		vector< TVector3<T> > centers;
		partitionOfCircle(center_circle,edge0->circle_.p,psi,number_of_segments,centers);
		centers.pop_back();
		centers.push_back(edge2->circle_.p);
		// save an iterator to the last triangle
		typename std::list<TTriangle<T>*>::iterator last_triangle = triangles_.end();
		last_triangle--;
		buildTriangles(edge0,edge1,edge2,edge3,centers,edge1_segments,edge3_segments,
									 points,radius_of_probe,sqrt_density,border);
		last_triangle++;
		TTriangle<T>* test_triangle = *last_triangle;
		// swap the triangles if necessary
		TVector3<T> orth( (test_triangle->vertex_[1]->point_-test_triangle->vertex_[0]->point_) %
											(test_triangle->vertex_[2]->point_-test_triangle->vertex_[0]->point_));
		if (Maths::isGreater(orth*test_triangle->vertex_[0]->point_,orth*centers[0]))
		{
			typename std::list<TTriangle<T>*>::iterator t;
			for (t = last_triangle; t != triangles_.end(); t++)
			{
				TTrianglePoint<T>* temp = (*t)->vertex_[0];
				(*t)->vertex_[0] = (*t)->vertex_[1];
				(*t)->vertex_[1] = temp;
			}
		}
	}


	template <class T>
	void TTriangulatedSES<T>::triangulateFreeToricFace
			(TSESFace<T>*														 face,
			 const T&																 radius_of_probe,
			 const T&																 sqrt_density,
			 std::vector<TTrianglePoint<T>*>				 points,
			 std::vector< list<TTriangleEdge<T>*> >& border)
		throw()
	{
		TVector3<T> normal(face->edge_.front()->circle_.n);
		TCircle3<T> circle1(face->edge_.front()->circle_);
		TCircle3<T> circle2(face->edge_.back()->circle_.p,
												normal,
												face->edge_.back()->circle_.radius);
		TCircle3<T> circle3(face->rsedge_->center_of_torus_,
												normal,
												face->rsedge_->radius_of_torus_);
		TVector3<T> orth(normal.y,-normal.x,(T)0);
		if (orth == TVector3<T>::getZero())
		{
			orth.set(normal.z,(T)0,-normal.x);
		}
		orth.normalize();
		TVector3<T> p1(circle1.p+(orth*circle1.radius));
		TVector3<T> p2(circle2.p+(orth*circle2.radius));
		TVector3<T> p3(circle3.p+(orth*circle3.radius));
		Size number_of_points = (Size)Maths::round(2*Constants::PI*circle1.radius*sqrt_density);
		if (number_of_points == 0)
		{
			number_of_points++;
		}
		TAngle<T> phi(2*Constants::PI/number_of_points,true);
		std::vector< TVector3<T> > points1;
		std::vector< TVector3<T> > points2;
		std::vector< TVector3<T> > centers;
		partitionOfCircle(circle1,p1,phi,number_of_points,points1);
		partitionOfCircle(circle2,p2,phi,number_of_points,points2);
		partitionOfCircle(circle3,p3,phi,number_of_points,centers);
		points1.pop_back();
		points2.pop_back();
		centers.pop_back();
		TSESEdge<T>* dummy(NULL);
		buildTriangles(dummy,face->edge_.front(),dummy,face->edge_.back(),
									 centers,points1,points2,
									 points,radius_of_probe,sqrt_density,border);
	}


	template <class T>
	void TTriangulatedSES<T>::triangulateSingularToricFace
			(TSESFace<T>*																	face,
			 const T&																			radius_of_probe,
			 const T&																			sqrt_density,
			 std::vector< TTrianglePoint<T>* >						points,
			 std::vector< std::list<TTriangleEdge<T>*> >& border)
		throw()
	{
		face->normalize(true);
		typename std::list<TSESEdge<T>*>::iterator e = face->edge_.begin();
		std::vector<TSESEdge<T>*> edge(6);
		edge[0] = *e;
		e++;
		edge[1] = *e;
		e++;
		edge[2] = *e;
		e++;
		edge[3] = *e;
		e++;
		edge[4] = *e;
		e++;
		edge[5] = *e;
		typename std::list<TSESVertex<T>*>::iterator v = face->vertex_.begin();
		std::vector<TSESVertex<T>*> vertex(6);
		vertex[0] = *v;
		v++;
		vertex[1] = *v;
		v++;
		vertex[2] = *v;
		v++;
		vertex[3] = *v;
		v++;
		vertex[4] = *v;
		v++;
		vertex[5] = *v;
		Size number_of_segments = (Size)Maths::round(face->rsedge_->phi_.value*
																								 edge[1]->circle_.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<T> psi(face->rsedge_->phi_.value/number_of_segments,true);
		Position offset = 0;
		typename std::list<TTriangle<T>*>::iterator last = triangles_.end();
		last--;
		for (Position counter = 0; counter < 2; counter++)
		{
			std::vector< TVector3<T> > edge_segments;
			partitionOfCircle(edge[0+offset]->circle_,
												edge[0+offset]->vertex_[0]->point_,
												psi,
												number_of_segments,
												edge_segments);
			edge_segments.pop_back();
			edge_segments.push_back(edge[0+offset]->vertex_[1]->point_);
			TVector3<T> axis(edge[0+offset]->circle_.n);
			if (edge[0+offset]->vertex_[0] != vertex[0+offset])
			{
				TVector3<T> tmp;
				for (Position i = 0; i < (number_of_segments+1)/2; i++)
				{
					tmp = edge_segments[i];
					edge_segments[i] = edge_segments[number_of_segments-i];
					edge_segments[number_of_segments-i] = tmp;
				}
				axis.negate();
			}
			std::vector< TVector3<T> > point;
			for (Position i = 0; i < number_of_segments+1; i++)
			{
				point.push_back(vertex[1+offset]->point_);
			}
			TCircle3<T> center_circle(face->rsedge_->center_of_torus_,
																axis,
																face->rsedge_->radius_of_torus_);
			vector< TVector3<T> > centers;
			partitionOfCircle(center_circle,edge[1+offset]->circle_.p,psi,number_of_segments,centers);
			centers.pop_back();
			centers.push_back(edge[2+offset]->circle_.p);
			TSESEdge<T>* dummy(NULL);
			buildTriangles(edge[1+offset],edge[0+offset],edge[2+offset],
										 dummy,centers,edge_segments,point,
										 points,radius_of_probe,sqrt_density,border);
			offset += 3;
			TTriangle<T>* test_triangle;
			typename std::list<TTriangle<T>*>::iterator start;
			last++;
			test_triangle = *last;
			start = last;
			last = triangles_.end();
			last--;
			TVector3<T> orth( (test_triangle->vertex_[1]->point_-test_triangle->vertex_[0]->point_) %
												(test_triangle->vertex_[2]->point_-test_triangle->vertex_[0]->point_));
			if (Maths::isGreater(orth*test_triangle->vertex_[0]->point_,orth*centers[0]))
			{
				typename std::list<TTriangle<T>*>::iterator t;
				TTrianglePoint<T>* temp;
				for (t = start; t != triangles_.end(); t++)
				{
					temp = (*t)->vertex_[0];
					(*t)->vertex_[0] = (*t)->vertex_[1];
					(*t)->vertex_[1] = temp;
				}
			}
		}
	}


	template <class T>
	void TTriangulatedSES<T>::partitionSingularEdge
			(TSESEdge<T>* singular_edge,
			 const T& sqrt_density,
			 const std::vector<TTrianglePoint<T>*>& point,
			 std::vector< std::list<TTriangleEdge<T>*> >& edge)
		throw()
	{
		if (singular_edge->vertex_[0] == NULL)
		{
			TAngle<T> phi;
			phi.value = 2*Constants::PI;
			Size number_of_segments
					= (Size)Maths::round(phi.value*singular_edge->circle_.radius*sqrt_density);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<T> psi(phi.value/number_of_segments,true);
			std::vector< TVector3<T> > points;
			partitionOfCircle(singular_edge->circle_,TVector3<T>::getZero(),psi,number_of_segments,points,false);
			points.pop_back();
			TTrianglePoint<T>* p0;
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = new TTrianglePoint<T>;
			p1->point_ = points[0];
			p1->normal_ = singular_edge->circle_.p-p1->point_;
			points_.push_back(p1);
			p0 = p1;
			for (Position k = 1; k < points.size(); k++)
			{
				p2 = new TTrianglePoint<T>;
				p2->point_ = points[k];
				p2->normal_ = singular_edge->circle_.p-p2->point_;
				points_.push_back(p2);
				e = new TTriangleEdge<T>;
				e->vertex_[0] = p1;
				e->vertex_[1] = p2;
				edges_.push_back(e);
				edge[singular_edge->index_].push_back(e);
				//p1->edges_.push_back(e);
				//p2->edges_.push_back(e);
				p1->edges_.insert(e);
				p2->edges_.insert(e);
				p1 = p2;
			}
			e = new TTriangleEdge<T>;
			e->vertex_[0] = p1;
			e->vertex_[1] = p0;
			edges_.push_back(e);
			edge[singular_edge->index_].push_back(e);
			//p1->edges_.push_back(e);
			//p0->edges_.push_back(e);
			p1->edges_.insert(e);
			p0->edges_.insert(e);
					#ifdef debug_triangulation
					Contour2HIN(edge[singular_edge->index_],
											"DATA/MOLECULE/edge"+IndexToString(singular_edge->index_,(T)0)+".hin");
					#endif
		}
		else
		{
			TAngle<T> phi(getOrientedAngle(singular_edge->vertex_[0]->point_-singular_edge->circle_.p,
																		 singular_edge->vertex_[1]->point_-singular_edge->circle_.p,
																		 singular_edge->circle_.n));
			/*TAngle<T> phi;
			GetAngle(singular_edge->vertex_[0]->point_-singular_edge->circle_.p,
							 singular_edge->vertex_[1]->point_-singular_edge->circle_.p,
							 phi);
			if (singular_edge->rsedge_ != NULL)
			{
				if (singular_edge->rsedge_->phi_.value > Constants::PI)
				{
					phi = 2*Constants::PI-phi;
				}
			}*/
			Size number_of_segments
					= (Size)Maths::round(phi.value*singular_edge->circle_.radius*sqrt_density);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<T> psi(phi.value/number_of_segments,true);
			/*TVector3<T> normal((singular_edge->vertex_[0]->point_-singular_edge->circle_.p)%
												 (singular_edge->vertex_[1]->point_-singular_edge->circle_.p));
			if (singular_edge->rsedge_ != NULL)
			{
				if (singular_edge->rsedge_->phi_.value > Constants::PI)
				{
					normal.negate();
				}
			}
			singular_edge->circle_.n = normal;*/
			std::vector< TVector3<T> > points;
			partitionOfCircle(singular_edge->circle_,singular_edge->vertex_[0]->point_,psi,number_of_segments,
												points);
			points.pop_back();
			points.push_back(singular_edge->vertex_[1]->point_);
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = point[singular_edge->vertex_[0]->index_];
			p1->normal_ = singular_edge->circle_.p-p1->point_;
			for (Position k = 1; k < points.size()-1; k++)
			{
				p2 = new TTrianglePoint<T>;
				p2->point_ = points[k];
				p2->normal_ = singular_edge->circle_.p-p2->point_;
				points_.push_back(p2);
				e = new TTriangleEdge<T>;
				e->vertex_[0] = p1;
				e->vertex_[1] = p2;
				edges_.push_back(e);
				edge[singular_edge->index_].push_back(e);
				//p1->edges_.push_back(e);
				//p2->edges_.push_back(e);
				p1->edges_.insert(e);
				p2->edges_.insert(e);
				p1 = p2;
			}
			p2 = point[singular_edge->vertex_[1]->index_];
			p2->normal_ = singular_edge->circle_.p-p2->point_;
			e = new TTriangleEdge<T>;
			e->vertex_[0] = p1;
			e->vertex_[1] = p2;
			edges_.push_back(e);
			edge[singular_edge->index_].push_back(e);
			//p1->edges_.push_back(e);
			//p2->edges_.push_back(e);
			p1->edges_.insert(e);
			p2->edges_.insert(e);
					#ifdef debug_triangulation
					Contour2HIN(edge[singular_edge->index_],
											"DATA/MOLECULE/edge"+IndexToString(singular_edge->index_,(T)0)+".hin");
					#endif
		}
	}


	template <class T>
	void TTriangulatedSES<T>::triangulateContactFace
			(TSESFace<T>*																					face,
			 const TSphere3<T>&																		sphere,
			 const HashMap< Size,std::list<TTrianglePoint<T>*> >&	spheres,
			 const std::vector< std::list<TTriangleEdge<T>*> >&		edge_contours)
		throw()
	{
					#ifdef debug_surface_processor_verbose
					std::cout << "    ... " << face->index_ << " ... ";
					#endif
		Position number_of_edges = face->edge_.size();
		if (number_of_edges > 0)
		{
			// the face does not come from a free vertex
			if (number_of_edges == 2)
			{
				if ((edge_contours[face->edge_.front()->index_].size() == 1) &&
						(edge_contours[face->edge_.back()->index_].size() == 1)		)
				{
					return;
				}
			}
			if (number_of_edges == 3)
			{
				Position counter = 0;
				TTrianglePoint<T>* point0(NULL);
				TTrianglePoint<T>* point1(NULL);
				TTrianglePoint<T>* point2(NULL);
				typename std::list<TSESEdge<T>*>::iterator it;
				for (it = face->edge_.begin(); it != face->edge_.end(); it++)
				{
					if (edge_contours[(*it)->index_].size() == 1)
					{
						counter++;
						TTriangleEdge<T>* edge = edge_contours[(*it)->index_].front();
						TTrianglePoint<T>* p0 = edge->vertex_[0];
						TTrianglePoint<T>* p1 = edge->vertex_[1];
						if ((p0 != point0) && (p0 != point1) && (p0 != point2))
						{
							if (point0 == NULL)
							{
								point0 = p0;
							}
							else
							{
								if (point1 == NULL)
								{
									point1 = p0;
								}
								else
								{
									if (point2 == NULL)
									{
										point2 = p0;
									}
								}
							}
						}
						if ((p1 != point0) && (p1 != point1) && (p1 != point2))
						{
							if (point0 == NULL)
							{
								point0 = p1;
							}
							else
							{
								if (point1 == NULL)
								{
									point1 = p1;
								}
								else
								{
									if (point2 == NULL)
									{
										point2 = p1;
									}
								}
							}
						}
					}
				}
				if (counter == 3)
				{
					TTriangle<T>* triangle = new TTriangle<T>;
					TVector3<T> normal((point0->point_-point1->point_)%(point0->point_-point2->point_));
					if (Maths::isLess(normal*(sphere.p-point1->point_),(T)0))
					{
						triangle->vertex_[0] = point0;
						triangle->vertex_[1] = point1;
						triangle->vertex_[2] = point2;
					}
					else
					{
						triangle->vertex_[0] = point0;
						triangle->vertex_[1] = point2;
						triangle->vertex_[2] = point1;
					}
					triangles_.push_back(triangle);
					number_of_triangles_++;
					return;
				}
			}
			// get a template sphere for the face to triangulate
			TTriangulatedSES<T> part;
			typename HashMap< Size,std::list<TTrianglePoint<T>*> >::ConstIterator s
					= spheres.find(numberOfRefinements(density_,sphere.radius));
			typename std::list<TTrianglePoint<T>*>::const_iterator p;
			TTrianglePoint<T>* point;
			for (p = s->second.begin(); p != s->second.end(); p++)
			{
				point = new TTrianglePoint<T>;
				point->point_ = (*p)->point_;
				point->normal_ = (*p)->normal_;
				part.points_.push_back(point);
				part.number_of_points_++;
			}
			part.blowUp(sphere.radius);
				#ifdef print_debug_info
				std::cout << part.number_of_points_ << " (" << part.points_.size() << ")\n";
				#endif
			// cut the face with all its edges
			TPlane3<T> plane;
			//TSESEdge<T>* edge;
			typename std::list<TSESEdge<T>*>::iterator edge;
			for (edge = face->edge_.begin(); edge != face->edge_.end(); edge++)
			{
				plane.p = (*edge)->circle_.p-sphere.p;
				plane.n = (*edge)->circle_.n;
				part.cut(plane,0.05);
						#ifdef print_debug_info
						std::cout << "schneide mit plane " << plane << " von edge " << **edge
											<< " ... " << part.number_of_points_ << " (" << part.points_.size() << ")\n";
						#endif
						#ifdef debug_triangulation_with_planes
						plane.p += sphere.p;
						Plane2HIN(plane,"DATA/PLANES/circle"+IndexToString(k,0)+"of"+
														IndexToString(face->index_,0)+".hin");
						#endif
			}
			std::list<TTriangleEdge<T>*> border;
			part.shift(sphere.p);
					#ifdef debug_triangulation
					part.printToHINFile("DATA/MOLECULE/contactFaceCut"+IndexToString(face->index_,0)+".hin");
					#endif
					#ifdef print_debug_info
					part.setIndices();
					typename std::list<TTrianglePoint<T>*>::iterator ppart;
					for (ppart = part.points_.begin(); ppart != part.points_.end(); ppart++)
					{
						(*ppart)->index_ += 10000;
					}
					int halt = 0;
					if ((face->index_ == 54) || (face->index_ == 88))
					{
						//halt = 0;
					}
					if (halt == 0) cin >> halt; else { cout << "\n"; halt--;}
					#endif
					#ifdef debug_triangulation
					sewSystem = new System;
					sewMolecule = new Molecule;
					#endif
			#ifdef print_debug_info
			buildSphericTriangles(part,border,edge_contours,face->edge_,sphere,halt);
			#else
			buildSphericTriangles(part,border,edge_contours,face->edge_,sphere);
			#endif
			// delete all isolated points
			typename std::list<TTrianglePoint<T>*>::iterator pp1;
			typename std::list<TTrianglePoint<T>*>::iterator pp2;
			pp1 = part.points_.begin();
			while (pp1 != part.points_.end())
			{
				if ((*pp1)->faces_.size() == 0)
				{
					pp2 = pp1;
					pp2++;
					if (pp2 == part.points_.end())
					{
						part.points_.erase(pp1);
						pp1 = part.points_.end();
					}
					else
					{
						part.points_.erase(pp1);
						pp1 = pp2;
					}
				}
				else
				{
					pp1++;
				}
			}
					#ifdef debug_triangulation
					sewSystem->insert(*sewMolecule);
					HINFile output("DATA/MOLECULE/contactFaceSew"+IndexToString(face->index_,0)+".hin",std::ios::out);
					output << *sewSystem;
					output.close();
					delete sewSystem;
					#endif
			join(part);
					#ifdef debug_surface_processor_verbose
					std::cout << "cut, joined\n";
					#endif
		}
		else
		{
			// the face comes from a free vertex
			TTriangulatedSES<T> part;
			part.icosaeder();
			part.refineSphere(numberOfRefinements(density_,sphere.radius));
			part.shift(sphere.p);
					#ifdef debug_surface_processor_verbose
					std::cout << "full sphere\n";
					#endif
			join(part);
		}
	}


	template <class T>
	bool TTriangulatedSES<T>::triangulateSphericFace
			(TSESFace<T>*																				face,
			 const TSphere3<T>&																	sphere,
			 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours)
		throw()
	{
					#ifdef debug_surface_processor_verbose
					std::cout << "    ... " << face->index_ << " ... ";
					#endif
					#ifdef print_debug_info
					int halt = 0;
					if (face->index_ == 699)
					{
						halt = 0;
					}
					if (halt == 0) cin >> halt; else { cout << "\n"; halt--;}
					#endif
		TTriangulatedSES<T> part;
		std::list<TTriangleEdge<T>*> border;
					#ifdef debug_triangulation
					sewSystem = new System;
					sewMolecule = new Molecule;
					#endif
		bool ok;
		#ifdef print_debug_info
		ok = buildSphericTriangles(part,border,edge_contours,face->edge_,sphere,halt,false);
		#else
		ok = buildSphericTriangles(part,border,edge_contours,face->edge_,sphere,false);
		#endif
		if (ok)
		{
						#ifdef debug_triangulation
						sewSystem->insert(*sewMolecule);
						HINFile output("DATA/MOLECULE/sphericFaceSew"+IndexToString(face->index_,0)+".hin",
													 std::ios::out);
						output << *sewSystem;
						output.close();
						delete sewSystem;
						#endif
						#ifdef debug_surface_processor_verbose
						std::cout << "cut, joined\n";
						#endif
			join(part);
			return true;
		}
		else
		{
						#ifdef debug_surface_processor_verbose
						std::cout << "cut, not joined\n";
						#endif
			return false;
		}
	}


	template <class T>
	bool TTriangulatedSES<T>::buildSphericTriangles
			(			 TTriangulatedSES<T>&													part,
						 std::list<TTriangleEdge<T>*>&								border,
			 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
			 const std::list<TSESEdge<T>*>&											sesedge,
			 const TSphere3<T>&																	sphere,
		 			#ifdef print_debug_info
			 		int halt,
			 		#endif
			 bool																								convex
			)
		throw()
	{
				#ifdef print_debug_info
				string pre = "      ";
				#endif
		HashSet<TTrianglePoint<T>*> points;
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = part.points_.begin(); p != part.points_.end(); p++)
		{
			points.insert(*p);
				#ifdef print_debug_info
				std::cout << (*p)->index_ << "  ";
				#endif
		}
				#ifdef print_debug_info
				std::cout << "\n";
				#endif
				#ifdef print_debug_info
				for (p = part.points_.begin(); p != part.points_.end(); p++)
				{
					std::cout << **p << "\n";
				}
				#endif
		typename std::list<TTriangleEdge<T>*>::const_iterator c;
		typename std::list<TSESEdge<T>*>::const_iterator e;
		for (e = sesedge.begin(); e != sesedge.end(); e++)
		{
					#ifdef print_debug_info
					std::cout << (*e)->index_ << ":  ";
					#endif
			for (c = edge_contours[(*e)->index_].begin();
					 c != edge_contours[(*e)->index_].end(); c++)
			{
				points.insert((*c)->vertex_[0]);
				points.insert((*c)->vertex_[1]);
						#ifdef print_debug_info
						std::cout << (*c)->vertex_[0]->index_;
						if (sphere.has((*c)->vertex_[0]->point_))
						{
							std::cout << "+  ";
						}
						else
						{
							std::cout << "-  ";
						}
						std::cout << (*c)->vertex_[1]->index_;
						if (sphere.has((*c)->vertex_[1]->point_))
						{
							std::cout << "+  ";
						}
						else
						{
							std::cout << "-  ";
						}
						#endif
			}
					#ifdef print_debug_info
					std::cout << "\n";
					#endif
		}
				#ifdef print_debug_info
				for (e = sesedge.begin(); e != sesedge.end(); e++)
				{
					std::cout << (*e)->index_ << ":  ";
					for (c = edge_contours[(*e)->index_].begin();
							 c != edge_contours[(*e)->index_].end(); c++)
					{
						std::cout << "  " << **c << "\n";
						std::cout << "    " << *((*c)->vertex_[0]) << "\n";
						std::cout << "    " << *((*c)->vertex_[1]) << "\n";
					}
				}
				#endif
		Size border_size = border.size();
		if (border_size == 0)
		{
			bool ok;
			#ifdef print_debug_info
			ok = buildFirstTriangle(part,border,border_size,edge_contours,sesedge,sphere,points,halt,convex);
			#else
			ok = buildFirstTriangle(part,border,border_size,edge_contours,sesedge,sphere,points,convex);
			#endif
			if (!ok)
			{
				return false;
			}
		}
				#ifdef print_debug_info
				typename std::list<TTriangleEdge<T>*>::iterator bit;
				for (bit = border.begin(); bit != border.end(); bit++)
				{
					cout << **bit << "\n";
				}
				ofstream file("surface.cut.log");
				file << part;
				file.close();
				#endif
		//while (border_size > 0)
		while (border.size() > 0)
		{
					#ifdef print_debug_info
					std::cout << pre << "derzeit " << border_size << " (" << border.size() << ") Edges";
					if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
			  	#endif
			TTriangleEdge<T>* edge = border.front();
			border.pop_front();
			border_size--;
					#ifdef print_debug_info
			  	std::cout << pre << *edge << " gepopt\n";
			  	#endif
			TTriangle<T>* start_triangle = edge->face_[0];
			TTrianglePoint<T>* third_point = start_triangle->thirdPoint(edge->vertex_[0],edge->vertex_[1]);
			TVector3<T> normal((third_point->point_-edge->vertex_[0]->point_) %
												 (third_point->point_-edge->vertex_[1]->point_)	);
			T test_value = normal*third_point->point_;
			Index p0 = start_triangle->getRelativeVertexIndex(edge->vertex_[0]);
			Index p1 = start_triangle->getRelativeVertexIndex(edge->vertex_[1]);
			Index diff = p1-p0;
			if (convex)
			{
				if ((diff == -1) || (diff == 2))
				{
					TTrianglePoint<T>* tmp = edge->vertex_[0];
					edge->vertex_[0] = edge->vertex_[1];
					edge->vertex_[1] = tmp;
				}
			}
			else
			{
				if ((diff == 1) || (diff == -2))
				{
					TTrianglePoint<T>* tmp = edge->vertex_[0];
					edge->vertex_[0] = edge->vertex_[1];
					edge->vertex_[1] = tmp;
				}
			}
					#ifdef print_debug_info
					std::cout << pre << "starte mit Edge " << *edge << " (" << edge << ")\n";
					std::cout << pre << "third point: " << third_point->index_ << "\n";
					std::cout << pre << "Dreieck: " << *start_triangle << " (" << test_value << ")  ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			typename HashSet<TTrianglePoint<T>*>::Iterator next = points.begin();
			while ((*next == edge->vertex_[0]) || (*next == edge->vertex_[1]) || (*next == third_point))
			{
						#ifdef print_debug_info
						std::cout << pre << "  dritter Punkt: " << (*next)->index_ << " ... --  ";
						#endif
				next++;
			}
			std::list<TTrianglePoint<T>*> third;
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
			while (next != points.end())
			{
				if ((*next != edge->vertex_[0]) && (*next != edge->vertex_[1]) &&
						(*next != third_point))
				{
					T this_value = normal*(*next)->point_;
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
						//if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
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
									buildUnambiguousTriangle(edge,third.front(),border,border_size,sphere,part,
																					 halt,convex);
									#else
									buildUnambiguousTriangle(edge,third.front(),border,border_size,sphere,part,convex);
									#endif
									break;
				default :
									#ifdef print_debug_info
									buildAmbiguousTriangles(edge,third,border,border_size,sphere,part,
																					halt,convex);
									#else
									buildAmbiguousTriangles(edge,third,border,border_size,sphere,part,convex);
									#endif
									break;
			}
		}
		return true;
	}


	template <class T>
	bool TTriangulatedSES<T>::buildFirstTriangle
			(			 TTriangulatedSES<T>&													part,
						 std::list<TTriangleEdge<T>*>&								border,
						 Size&																				border_size,
			 const std::vector< std::list<TTriangleEdge<T>*> >& edge_contours,
			 const std::list<TSESEdge<T>*>&											sesedge,
			 const TSphere3<T>&																	sphere,
			 const HashSet<TTrianglePoint<T>*>&									points,
						#ifdef print_debug_info
						int& halt,
						#endif
				bool																							convex
			)
		throw()
	{
		TSESEdge<T>* first_sesedge;
		TTriangleEdge<T>* edge;
		bool found = false;
		bool found_;
		typename std::list<TSESEdge<T>*>::const_iterator e = sesedge.begin();
		while ((found == false) && (e != sesedge.end()))
		{
			found_ = false;
			while ((found_ == false) && (e != sesedge.end()))
			{
				if ((*e)->type_ == TSESEdge<T>::TYPE_SINGULAR)
				{
					e++;
				}
				else
				{
					found_ = true;
					first_sesedge = *e;
				}
			}
			if (found_)
			{
						#ifdef print_debug_info
						//halt = 0;
						std::cout << "nichtsinguläre SESEdge: " << *first_sesedge << "\n";
						#endif
				edge = edge_contours[first_sesedge->index_].front();
				found = true;
				if (edge_contours[first_sesedge->index_].size() == 1)
				{
					TVector3<T> diff(edge->vertex_[0]->point_-edge->vertex_[1]->point_);
					if (diff*diff < 0.01)
					{
						found = false;
						e++;
					}
				}
			}
		}
		if (!found)
		{
			return false;
		}
		HashSet<TTrianglePoint<T>*> same_edge;
		typename std::list<TTriangleEdge<T>*>::const_iterator c;
		for (c = edge_contours[first_sesedge->index_].begin();
				 c != edge_contours[first_sesedge->index_].end(); c++)
		{
			same_edge.insert((*c)->vertex_[0]);
			same_edge.insert((*c)->vertex_[1]);
		}
				#ifdef print_debug_info
				//halt = 0;
				std::cout << "nichtsinguläre SESEdge: " << *first_sesedge << "\nPunkte: ";
				typename HashSet<TTrianglePoint<T>*>::Iterator se;
				for (se = same_edge.begin(); se != same_edge.end(); se++)
				{
					std::cout << (*se)->index_ << "  ";
				}
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				std::cout << "suche Dreieck zu edge " << *edge;
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		// orientiere die edge so, wie sie im existierenden Dreieck NICHT orientiert ist
		// (so soll sie im neuen Dreieck orientiert sein)
		TVector3<T> edge_vector(edge->vertex_[0]->point_-edge->vertex_[1]->point_);
		TTriangle<T>* triangle = edge->face_[0];
		Index p0 = triangle->getRelativeVertexIndex(edge->vertex_[0]);
		Index p1 = triangle->getRelativeVertexIndex(edge->vertex_[1]);
		Index diff = p1-p0;
		if ((diff == 1) || (diff == -2))
		{
			TTrianglePoint<T>* tmp = edge->vertex_[0];
			edge->vertex_[0] = edge->vertex_[1];
			edge->vertex_[1] = tmp;
			edge_vector = -edge_vector;
		}
				#ifdef print_debug_info
				std::cout << "edge " << *edge << " orientiert ";
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		typename HashSet<TTrianglePoint<T>*>::ConstIterator next = points.begin();
		typename HashSet<TTrianglePoint<T>*>::ConstIterator test;
		std::list<TTrianglePoint<T>*> third;
		TVector3<T> normal;
		while (next != points.end())
		{
						#ifdef print_debug_info
						std::cout << **next << " ...\n";
						#endif
			if (!same_edge.has(*next))
			{
				normal.set(edge_vector % (edge->vertex_[0]->point_-(*next)->point_));
				T test_value = normal*edge->vertex_[0]->point_;
				bool is_convex = true;
				test = points.begin();
				while (test != points.end())
				{
							#ifdef print_debug_info
							std::cout << "    " << (*test)->index_ << " ... ";
							#endif
					if ((*test != *next) && (*test != edge->vertex_[0]) && (*test != edge->vertex_[1]))
					{
						T this_value = normal*(*test)->point_;
						if ((Maths::isGreater(this_value,test_value) && convex) ||
								(Maths::isLess(this_value,test_value) && !convex))
						{
							is_convex = false;
									#ifdef print_debug_info
									std::cout << "not ok\n";
									#endif
						}
								#ifdef print_debug_info
								else
								{
									std::cout << "ok\n";
								}
								#endif
					}
							#ifdef print_debug_info
							else
							{
								std::cout << "--\n";
							}
							#endif
					test++;
				}
				if (is_convex)
				{
					third.push_back(*next);
							#ifdef print_debug_info
							std::cout << "  ... passt\n";
							#endif
				}
						#ifdef print_debug_info
						else
						{
							std::cout << "  ... passt nicht\n";
						}
						#endif
			}
			next++;
		}
				#ifdef print_debug_info
				std::cout << third.size() << " Kandidaten gefunden: ";
				typename std::list<TTrianglePoint<T>*>::iterator rt;
				for (rt = third.begin(); rt != third.end(); rt++)
				{
					std::cout << (*rt)->index_ << "  ";
				}
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		// normal vector of the old triangle (shows out of the molecule)
		normal.set((triangle->vertex_[0]->point_-triangle->vertex_[1]->point_) %
							 (triangle->vertex_[0]->point_-triangle->vertex_[2]->point_)	);
		TAngle<T> min(3*Constants::PI,true);
		std::list<TTrianglePoint<T>*> real_third;
		typename std::list<TTrianglePoint<T>*>::iterator p = third.begin();
		TVector3<T> new_normal;
		TAngle<T> angle;
		while (p != third.end())
		{
			new_normal.set(edge_vector % (edge->vertex_[0]->point_-(*p)->point_));
			angle = getOrientedAngle(normal,new_normal,edge_vector);
			if (angle < min)
			{
				real_third.clear();
				real_third.push_back(*p);
				min = angle;
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
					std::cout << (*rt)->index_ << "  ";
				}
				if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
				#endif
		switch (real_third.size())
		{
			case 0 :
								#ifdef print_debug_info
								std::cout << "no convex triangle found\n";
								#endif
								break;
			case 1 :
								#ifdef print_debug_info
								buildUnambiguousTriangle(edge,real_third.front(),border,border_size,sphere,part,
																				 halt,convex);
								#else
								buildUnambiguousTriangle(edge,real_third.front(),border,border_size,sphere,part,convex);
								#endif
								break;
			default :
								#ifdef print_debug_info
								buildAmbiguousTriangles(edge,real_third,border,border_size,sphere,part,
																				halt,convex);
								#else
								buildAmbiguousTriangles(edge,real_third,border,border_size,sphere,part,convex);
								#endif
								break;
		}
		return true;
	}


	template <class T>
	void TTriangulatedSES<T>::buildUnambiguousTriangle
			(TTriangleEdge<T>*						 edge,
			 TTrianglePoint<T>*						 point,
			 std::list<TTriangleEdge<T>*>& border,
			 Size&												 border_size,
			 const TSphere3<T>&						 sphere,
			 TTriangulatedSES<T>&					 part,
						#ifdef print_debug_info
						int& halt,
						#endif
				bool													convex
			)
		throw()
	{
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "eindeutiges Dreieck\n";
				#endif
		TTriangleEdge<T>* edge1;
		TTriangleEdge<T>* edge2;
		TTriangle<T>* triangle;
		bool old1, old2;
		createTriangleAndEdges(edge,point,sphere,edge1,old1,edge2,old2,triangle,convex);
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
			border_size--;
					#ifdef print_debug_info
					std::cout << pre << "remove Edge " << *edge1
										<< " (" << edge1 << ") from\n" << pre << "  border\n";
					#endif
		}
		else
		{
			//edge1->vertex_[0]->edges_.push_back(edge1);
			//edge1->vertex_[1]->edges_.push_back(edge1);
			edge1->vertex_[0]->edges_.insert(edge1);
			edge1->vertex_[1]->edges_.insert(edge1);
			edge1->face_[0] = triangle;
					#ifdef with_indices
					edge1->index_ = part.edges_.size();
					#endif
			part.edges_.push_back(edge1);
			part.number_of_edges_++;
			border.push_back(edge1);
			border_size++;
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge " << *edge1 << " (" << edge1 << ") to\n"
										<< pre << "  part\n" << pre << "  border\n";
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
			border.remove(edge2);
			border_size--;
					#ifdef print_debug_info
					std::cout << pre << "remove Edge " << *edge2 << " (" << edge2 << ") from\n"
										<< pre << "  border\n";
					#endif
		}
		else
		{
			//edge2->vertex_[0]->edges_.push_back(edge2);
			//edge2->vertex_[1]->edges_.push_back(edge2);
			edge2->vertex_[0]->edges_.insert(edge2);
			edge2->vertex_[1]->edges_.insert(edge2);
			edge2->face_[0] = triangle;
					#ifdef with_indices
					edge2->index_ = part.edges_.size();
					#endif
			part.edges_.push_back(edge2);
			part.number_of_edges_++;
			border.push_back(edge2);
			border_size++;
					#ifdef print_debug_info
					std::cout << pre << "pushe Edge " << *edge2 << " (" << edge2 << ") to\n"
										<< pre << "  part\n" << pre << "  border\n";
					#endif
		}
		edge->face_[1] = triangle;
		//triangle->vertex_[0]->faces_.push_back(triangle);
		//triangle->vertex_[1]->faces_.push_back(triangle);
		//triangle->vertex_[2]->faces_.push_back(triangle);
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
					atom1->createBond(*atom2);
					atom2->createBond(*atom3);
					atom3->createBond(*atom1);
					sewMolecule->insert(*atom1);
					sewMolecule->insert(*atom2);
					sewMolecule->insert(*atom3);
					#endif
	}


	template <class T>
	void TTriangulatedSES<T>::buildAmbiguousTriangles
			(TTriangleEdge<T>*						 edge,
			 std::list<TTrianglePoint<T>*> points,
			 std::list<TTriangleEdge<T>*>& border,
			 Size&												 border_size,
			 const TSphere3<T>&						 sphere,
			 TTriangulatedSES<T>&					 part,
						#ifdef print_debug_info
						int& halt,
						#endif
				bool												 convex
			)
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::iterator p;
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
					createTriangleAndEdges(edge0,*p,sphere,edge1,old1,edge2,old2,triangle,convex);
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
						TTriangle<T>* old_triangle = edge0->face_[0];
								#ifdef print_debug_info
								std::cout << pre << "  teste gegen Dreieck " << *old_triangle;
								std::cout << " (" << old_triangle << ") ";
								if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
								#endif
						Index i1 = old_triangle->getRelativeVertexIndex(edge0->vertex_[0]);
						Index i2 = old_triangle->getRelativeVertexIndex(edge0->vertex_[1]);
						bool back = false;
						if ((i1-i2 == 1) || (i1-i2 == -2))
						{
							back = true;
						}
						i1 = triangle->getRelativeVertexIndex(edge0->vertex_[0]);
						i2 = triangle->getRelativeVertexIndex(edge0->vertex_[1]);
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
							std::cout << pre << (built ? "ok " : "nok ") << border_size
												<< " " << planar_edges.size() << " ";
							if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
							#endif
					if (built)
					{
						edge0->face_[1] = triangle;
						//triangle->vertex_[0]->faces_.push_back(triangle);
						//triangle->vertex_[1]->faces_.push_back(triangle);
						//triangle->vertex_[2]->faces_.push_back(triangle);
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
							border_size--;
							planar_edges.remove(edge1);
						}
						else
						{
							edge1->face_[0] = triangle;
							border.push_back(edge1);
							border_size++;
							planar_edges.push_back(edge1);
							//edge1->vertex_[0]->edges_.push_back(edge1);
							//edge1->vertex_[1]->edges_.push_back(edge1);
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
							border_size--;
							planar_edges.remove(edge2);
						}
						else
						{
							edge2->face_[0] = triangle;
							border.push_back(edge2);		
							border_size++;
							planar_edges.push_back(edge2);
							//edge2->vertex_[0]->edges_.push_back(edge2);
							//edge2->vertex_[1]->edges_.push_back(edge2);
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
						border_size--;
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
								atom1->createBond(*atom2);
								atom2->createBond(*atom3);
								atom3->createBond(*atom1);
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


	template <class T>
	void TTriangulatedSES<T>::createTriangleAndEdges
			(TTriangleEdge<T>*	edge,
			 TTrianglePoint<T>* point,
			 const TSphere3<T>& sphere,
			 TTriangleEdge<T>*& edge1,
			 bool&							old1,
			 TTriangleEdge<T>*& edge2,
			 bool&							old2,
			 TTriangle<T>*&			triangle,
			 bool								convex
			)
		throw()
	{
		// build two new edges
		edge1	= new TTriangleEdge<T>;
		edge1->vertex_[0] = edge->vertex_[0];
		edge1->vertex_[1] = point;
		edge2 = new TTriangleEdge<T>;
		edge2->vertex_[0] = point;
		edge2->vertex_[1] = edge->vertex_[1];
				#ifdef print_debug_info
				string pre = "      ";
				std::cout << pre << "zwei Edges konstruiert\n"
									<< pre << "  " << *edge1 << " (" << edge1 << ")\n"
									<< pre << "  " << *edge2 << " (" << edge2 << ")\n";
				#endif
		// do the edges exist ?
		TTriangleEdge<T>* test = edge->vertex_[0]->has(edge1);
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
		triangle = new TTriangle<T>;
		triangle->vertex_[0] = edge->vertex_[1];
		triangle->vertex_[1] = edge->vertex_[0];
		triangle->vertex_[2] = point;
		triangle->edge_[0] = edge;
		triangle->edge_[1] = edge1;
		triangle->edge_[2] = edge2;
		// swap triangle if necessary
		TVector3<T> test_vector((triangle->vertex_[1]->point_-triangle->vertex_[0]->point_)%
														(triangle->vertex_[2]->point_-triangle->vertex_[0]->point_));
		T test_value = test_vector*(sphere.p-triangle->vertex_[0]->point_);
		if ((Maths::isGreater(test_value,0) && convex) ||
				(Maths::isLess(test_value,0) && !convex)			)
		{
			TTrianglePoint<T>* temp = triangle->vertex_[0];
			triangle->vertex_[0] = triangle->vertex_[1];
			triangle->vertex_[1] = temp;
		}
				#ifdef print_debug_info
				std::cout << pre << "Dreieck gebaut: " << *triangle << " (" << triangle << ")\n";
				#endif
	}


	template <class T>
	void TTriangulatedSES<T>::partitionOfCircle
			(const TCircle3<T>&			circle,
			 const TVector3<T>&			p0,
			 const TAngle<T>&				phi,
			 Size										number_of_segments,
			 vector< TVector3<T> >&	partition,
			 bool										on_surface)
		throw()
	{
		TVector4<T> p;
		if (on_surface)
		{
			TVector3<T> p_(p0-circle.p);
			p.set(p_.x,p_.y,p_.z,(T)0);
		}
		else
		{
			/*TVector3<T> p_(p0-circle.p);
			TVector3<T> d(circle.n%(circle.n%p_));
			d.normalize();
			TVector3<T> p1(circle.radius*d);
			TVector3<T> p2(-p1);
			TVector3<T> test1(p1-p_);
			TVector3<T> test2(p2-p_);
			if (test1*test1 < test2*test2)
			{
				p.set(p1.x,p1.y,p1.z,(T)0);
			}
			else
			{
				p.set(p2.x,p2.y,p2.z,(T)0);
			}*/
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
		partition.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
		for (Size i = 0; i < number_of_segments; i++)
		{
			p = rotation*p;
			partition.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
		}
	}


	template <class T>
	Size TTriangulatedSES<T>::numberOfRefinements(const T& density, const T& radius)
		throw()
	{
		T test0 = (4.0*density*Constants::PI*radius*radius-12.0)/30.0;
		Size n = 0;
		if (Maths::isGreaterOrEqual(test0,(T)0))
		{
			T test1;
			T test2 = 1;
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


	template <class T>
	//void TTriangulatedSES<T>::buildSpheres(HashMap<Size,TTriangulatedSES<T>*>& spheres)
	void TTriangulatedSES<T>::buildSpheres(HashMap< Size,std::list<TTrianglePoint<T>*> >& spheres)
		throw()
	{
		/*spheres[0] = new TTriangulatedSES<T>;
		spheres[0]->icosaeder(true);
		spheres[0]->setIndices();
		spheres[1] = new TTriangulatedSES<T>;
		spheres[1]->icosaeder(true);
		spheres[1]->refineSphere(1,true);
		spheres[1]->setIndices();
		spheres[2] = new TTriangulatedSES<T>;
		spheres[2]->icosaeder(true);
		spheres[2]->refineSphere(2,true);
		spheres[2]->setIndices();
		spheres[3] = new TTriangulatedSES<T>;
		spheres[3]->icosaeder(true);
		spheres[3]->refineSphere(3,true);
		spheres[3]->setIndices();*/
		typename std::list<TTrianglePoint<T>*>::const_iterator p;
		TTrianglePoint<T>* point;
		TTriangulatedSES<T> sphere;
		sphere.icosaeder(true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			point = new TTrianglePoint<T>;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			spheres[0].push_back(point);
		}
		sphere.refineSphere(1,true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			point = new TTrianglePoint<T>;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			spheres[1].push_back(point);
		}
		sphere.refineSphere(1,true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			point = new TTrianglePoint<T>;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			spheres[2].push_back(point);
		}
		sphere.refineSphere(1,true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			point = new TTrianglePoint<T>;
			point->point_ = (*p)->point_;
			point->normal_ = (*p)->normal_;
			spheres[3].push_back(point);
		}
	}


	template <class T>
	void TTriangulatedSES<T>::buildTriangles
		 (TSESEdge<T>*																 edge0,
			TSESEdge<T>*																 edge1,
			TSESEdge<T>*																 edge2,
			TSESEdge<T>*																 edge3,
			const std::vector< TVector3<T> >&						 centers,
			const std::vector< TVector3<T> >&						 edge1_points,
			const std::vector< TVector3<T> >&						 edge3_points,
			std::vector<TTrianglePoint<T>*>							 existing_points,
			const T&																		 radius_of_probe,
			const T&																		 sqrt_density,
			std::vector< std::list<TTriangleEdge<T>*> >& border)
		throw()
	{
		TAngle<T> pi(Constants::PI,true);
		TAngle<T> psi;
		// How much triangles should be built in each segment of the face?
		// Attention: number_of_triangles is the half of the number of triangles for each segment.
		GetAngle(edge1_points[0]-centers[0],edge3_points[0]-centers[0],psi);
		Size number_of_triangles = (Size)Maths::round(psi.value*radius_of_probe*sqrt_density);
		if (number_of_triangles == 0)
		{
			number_of_triangles++;
		}
		TAngle<T> phi(psi/number_of_triangles,true);
		Size number_of_segments = centers.size()-1;
		// How much TrianglePoints lie on the face?
		// Remark: edge3 == NULL iff the face is singular
		Size number_of_points = ((edge3 != NULL) ? (number_of_segments+1)*(number_of_triangles+1)
																						 : (number_of_segments+1)* number_of_triangles +1 );
		std::vector<TTrianglePoint<T>*> points(number_of_points);
		if (edge3 == NULL)
		{
			// singular face
			Index index;
			// get the index of the SESVertex on the top of the face
			// (remember: singular faces are two triangles)
			if (edge0->vertex_[0]->point_ == edge3_points[0])
			{
				index = edge0->vertex_[0]->index_;
			}
			else
			{
				index = edge0->vertex_[1]->index_;
			}
			TTrianglePoint<T>* top = existing_points[index];
			top->normal_ = centers[number_of_segments]-top->point_;
			top->normal_.normalize();
					#ifdef with_indices
					top->index_ = (number_of_segments+1)*number_of_triangles;
					#endif
			points[(number_of_segments+1)*number_of_triangles] = top;
		}
		Position end = ((edge3 != NULL) ? number_of_triangles+1 : number_of_triangles);
		for (Position i = 0; i <= number_of_segments; i++)
		{
			TCircle3<T> circle(centers[i],
												 (edge1_points[i]-centers[i])%(edge3_points[i]-centers[i]),
												 radius_of_probe);
			std::vector< TVector3<T> > line;
			line.clear();
			partitionOfCircle(circle,edge1_points[i],phi,number_of_triangles,line);
			for (Position j = 0; j < end; j++)
			{
				Index index = -1;
				// get the index of the point if it just exists (point on the corner)
				if (edge0 != NULL)
				{
					// not free face
					if ((i == 0) && (j == 0))
					{
						T dist1 = line[0].getSquareDistance(edge0->vertex_[0]->point_);
						T dist2 = line[0].getSquareDistance(edge0->vertex_[1]->point_);
						index = ((dist1 < dist2) ? edge0->vertex_[0]->index_ : edge0->vertex_[1]->index_);
					}
					if ((i == 0) && (j == number_of_triangles) && (edge3 != NULL))
					{
						T dist1 = line[number_of_triangles].getSquareDistance(edge0->vertex_[0]->point_);
						T dist2 = line[number_of_triangles].getSquareDistance(edge0->vertex_[1]->point_);
						index = ((dist1 < dist2) ? edge0->vertex_[0]->index_ : edge0->vertex_[1]->index_);
					}
					if ((i == number_of_segments) && (j == 0))
					{
						T dist1 = line[0].getSquareDistance(edge2->vertex_[0]->point_);
						T dist2 = line[0].getSquareDistance(edge2->vertex_[1]->point_);
						index = ((dist1 < dist2) ? edge2->vertex_[0]->index_ : edge2->vertex_[1]->index_);
					}
					if ((i == number_of_segments) && (j == number_of_triangles) && (edge3 != NULL))
					{
						T dist1 = line[number_of_triangles].getSquareDistance(edge2->vertex_[0]->point_);
						T dist2 = line[number_of_triangles].getSquareDistance(edge2->vertex_[1]->point_);
						index = ((dist1 < dist2) ? edge2->vertex_[0]->index_ : edge2->vertex_[1]->index_);
					}
				}
				TTrianglePoint<T>* point;
				if (index == -1)
				{
					// create a new point if it not exists yet
					point = new TTrianglePoint<T>;
					points_.push_back(point);
					point->point_ = line[j];
					point->normal_ = centers[i]-line[j];
				}
				else
				{
					point = existing_points[index];
				}
						#ifdef with_indices
						point->index_ = i*end+j;
						#endif
				points[i*end+j] = point;
			}
		}
		// number of edges to create
		Size edge_number = ((edge3 != NULL) ? 3*number_of_segments*number_of_triangles +
																					number_of_segments+number_of_triangles
																				: 3*number_of_segments*number_of_triangles-
																				 	2*number_of_segments+number_of_triangles-1	 );
		std::vector<TTriangleEdge<T>*> edges(edge_number);
		// build the vertical edges
		Position offset;
		Position offset2;
		TTriangleEdge<T>* new_edge;
		for (Position j = 0; j <= number_of_segments; j++)
		{
			offset = j*end;
			for (Position i = 0; i < end-1; i++)
			{
				offset2 = offset+i;
				new_edge = new TTriangleEdge<T>;
				edges_.push_back(new_edge);
				new_edge->vertex_[0] = points[offset2];
				//new_edge->vertex_[0]->edges_.push_back(new_edge);
				new_edge->vertex_[0]->edges_.insert(new_edge);
				new_edge->vertex_[1] = points[offset2+1];
				//new_edge->vertex_[1]->edges_.push_back(new_edge);
				new_edge->vertex_[1]->edges_.insert(new_edge);
				if ((j == 0) && (edge0 != 0))
				{
					border[edge0->index_].push_back(new_edge);
				}
				else
				{
					if ((j == number_of_segments) && (edge0 != 0))
					{
						border[edge2->index_].push_back(new_edge);
					}
				}
				edges[offset2-j] = new_edge;
			}
		}
		// build the horizontal edges
		Position add1 = (end-1)*(number_of_segments+1);
		for (Position j = 0; j < number_of_segments; j++)
		{
			offset = j*end;
			for (Position i = 0; i < end; i++)
			{
				offset2 = offset+i;
				new_edge = new TTriangleEdge<T>;
				edges_.push_back(new_edge);
				new_edge->vertex_[0] = points[offset2];
				//new_edge->vertex_[0]->edges_.push_back(new_edge);
				new_edge->vertex_[0]->edges_.insert(new_edge);
				new_edge->vertex_[1] = points[offset2+end];
				//new_edge->vertex_[1]->edges_.push_back(new_edge);
				new_edge->vertex_[1]->edges_.insert(new_edge);
				if ((i == 0) && (edge1 != 0))
				{
					border[edge1->index_].push_back(new_edge);
				}
				else
				{
					if ((i == end-1) && (edge3 != 0))
					{
						border[edge3->index_].push_back(new_edge);
					}
				}
				edges[offset2+add1] = new_edge;
			}
		}
		// buid the diagonal edges
		Position add2 = add1+end*number_of_segments;
		for (Position j = 0; j < number_of_segments; j++)
		{
			offset = j*end-j;
			for (Position i = 0; i < end-1; i++)
			{
				offset2 = offset+i;
				new_edge = new TTriangleEdge<T>;
				edges_.push_back(new_edge);
				new_edge->vertex_[0] = points[offset2+j];
				//new_edge->vertex_[0]->edges_.push_back(new_edge);
				new_edge->vertex_[0]->edges_.insert(new_edge);
				new_edge->vertex_[1] = points[offset2+j+end+1];
				//new_edge->vertex_[1]->edges_.push_back(new_edge);
				new_edge->vertex_[1]->edges_.insert(new_edge);
				edges[offset2+add2] = new_edge;
			}
		}
		// number of triangles to build
		Size triangle_number = 2*number_of_segments*(end-1);
		//Size triangle_number = ((edge3 != NULL) ? 2*number_of_segments*number_of_triangles
		//																				: 2*number_of_segments*(number_of_triangles-1));
		std::vector<TTriangle<T>*> triangles(triangle_number);
		// build the triangles
		for (Position i = 0; i < end-1; i++)
		{
			for (Position j = 0; j < number_of_segments; j++)
			{
				offset2 = j*end+i;
				TTriangle<T>* t1 = new TTriangle<T>;
					triangles_.push_back(t1);
					t1->vertex_[0] = points[offset2];
					t1->vertex_[1] = points[offset2+1];
					t1->vertex_[2] = points[offset2+1+end];
					t1->edge_[0] = edges[offset2-j];
					t1->edge_[1] = edges[offset2+add1+1];
					t1->edge_[2] = edges[offset2-j+add2];
					//t1->vertex_[0]->faces_.push_back(t1);
					//t1->vertex_[1]->faces_.push_back(t1);
					//t1->vertex_[2]->faces_.push_back(t1);
					t1->vertex_[0]->faces_.insert(t1);
					t1->vertex_[1]->faces_.insert(t1);
					t1->vertex_[2]->faces_.insert(t1);
					t1->edge_[0]->face_[0] = t1;
					t1->edge_[1]->face_[0] = t1;
					t1->edge_[2]->face_[0] = t1;
							#ifdef with_indices
							t1->index_ = 2*(number_of_segments*i+j);
							#endif
					triangles[2*(number_of_segments*i+j)] = t1;
				TTriangle<T>* t2 = new TTriangle<T>;
					triangles_.push_back(t2);
					t2->vertex_[0] = points[offset2];
					t2->vertex_[1] = points[offset2+1+end];
					t2->vertex_[2] = points[offset2+end];
					t2->edge_[0] = edges[offset2-j+end-1];
					t2->edge_[1] = edges[offset2+add1];
					t2->edge_[2] = edges[offset2-j+add2];
					//t2->vertex_[0]->faces_.push_back(t2);
					//t2->vertex_[1]->faces_.push_back(t2);
					//t2->vertex_[2]->faces_.push_back(t2);
					t2->vertex_[0]->faces_.insert(t2);
					t2->vertex_[1]->faces_.insert(t2);
					t2->vertex_[2]->faces_.insert(t2);
					t2->edge_[0]->face_[0] = t2;
					t2->edge_[1]->face_[0] = t2;
					t2->edge_[2]->face_[0] = t2;
							#ifdef with_indices
							t2->index_ = 2*(number_of_segments*i+j)+1;
							#endif
					triangles[2*(number_of_segments*i+j)+1] = t2;
			}
		}
		// close free faces
		if (edge0 == NULL)
		{
			offset = number_of_segments*end;
			TTriangleEdge<T>* edge = new TTriangleEdge<T>;
				edges_.push_back(edge);
				edge->vertex_[0] = points[offset];
				edge->vertex_[1] = points[0];
				//points[offset]->edges_.push_back(edge);
				//points[0]->edges_.push_back(edge);
				points[offset]->edges_.insert(edge);
				points[0]->edges_.insert(edge);
				border[edge1->index_].push_back(edge);
			for (Position i = 0; i < number_of_triangles; i++)
			{
				offset2 = offset+i;
				TTriangle<T>* t1 = new TTriangle<T>;
					triangles_.push_back(t1);
					t1->vertex_[0] = points[offset2];
					t1->vertex_[1] = points[i+1];
					t1->vertex_[2] = points[i];
					t1->edge_[0] = edge;
					edge = new TTriangleEdge<T>;
						edges_.push_back(edge);
						edge->vertex_[0] = points[offset2];
						edge->vertex_[1] = points[i+1];
						//points[offset2]->edges_.push_back(edge);
						//points[i+1]->edges_.push_back(edge);
						points[offset2]->edges_.insert(edge);
						points[i+1]->edges_.insert(edge);
					t1->edge_[1] = edge;
					t1->edge_[2] = edges[i];
					//t1->vertex_[0]->faces_.push_back(t1);
					//t1->vertex_[1]->faces_.push_back(t1);
					//t1->vertex_[2]->faces_.push_back(t1);
					t1->vertex_[0]->faces_.insert(t1);
					t1->vertex_[1]->faces_.insert(t1);
					t1->vertex_[2]->faces_.insert(t1);
					t1->edge_[0]->face_[0] = t1;
					t1->edge_[1]->face_[0] = t1;
					t1->edge_[2]->face_[0] = t1;
							#ifdef with_indices
							t1->index_ = triangles.size();
							#endif
					triangles.push_back(t1);
				TTriangle<T>* t2 = new TTriangle<T>;
					triangles_.push_back(t2);
					t2->vertex_[0] = points[offset2];
					t2->vertex_[1] = points[offset2+1];
					t2->vertex_[2] = points[i+1];
					t2->edge_[0] = edge;
					t2->edge_[1] = edges[offset2-number_of_segments];
					edge = new TTriangleEdge<T>;
						edges_.push_back(edge);
						edge->vertex_[0] = points[offset2+1];
						edge->vertex_[1] = points[i+1];
						//points[offset2+1]->edges_.push_back(edge);
						//points[i+1]->edges_.push_back(edge);
						points[offset2+1]->edges_.insert(edge);
						points[i+1]->edges_.insert(edge);
					t2->edge_[2] = edge;
					//t2->vertex_[0]->faces_.push_back(t2);
					//t2->vertex_[1]->faces_.push_back(t2);
					//t2->vertex_[2]->faces_.push_back(t2);
					t2->vertex_[0]->faces_.insert(t2);
					t2->vertex_[1]->faces_.insert(t2);
					t2->vertex_[2]->faces_.insert(t2);
					t2->edge_[0]->face_[0] = t2;
					t2->edge_[1]->face_[0] = t2;
					t2->edge_[2]->face_[0] = t2;
							#ifdef with_indices
							t2->index_ = triangles.size();
							#endif
					triangles.push_back(t2);
			}
			border[edge3->index_].push_back(edge);
		}
		// get the top of a singular face
		if (edge3 == NULL)
		{
			Position last = (number_of_segments+1)*number_of_triangles;
			offset = (end-1)*(number_of_segments+2);
			TTriangleEdge<T>* edge = new TTriangleEdge<T>;
				edges_.push_back(edge);
				edge->vertex_[0] = points[number_of_triangles-1];
				edge->vertex_[1] = points[last];
				//edge->vertex_[0]->edges_.push_back(edge);
				//edge->vertex_[1]->edges_.push_back(edge);
				edge->vertex_[0]->edges_.insert(edge);
				edge->vertex_[1]->edges_.insert(edge);
				border[edge0->index_].push_back(edge);
			for (Position i = 0; i < number_of_segments; i++)
			{
				TTriangle<T>* t = new TTriangle<T>;
					triangles_.push_back(t);
					t->vertex_[0] = points[number_of_triangles*(i+1)-1];
					t->vertex_[1] = points[last];
					t->vertex_[2] = points[number_of_triangles*(i+2)-1];
					t->edge_[0] = edge;
					t->edge_[1] = edges[offset+i*end];
					edge = new TTriangleEdge<T>;
						edges_.push_back(edge);
						edge->vertex_[0] = points[number_of_triangles*(i+2)-1];
						edge->vertex_[1] = points[last];
						//edge->vertex_[0]->edges_.push_back(edge);
						//edge->vertex_[1]->edges_.push_back(edge);
						edge->vertex_[0]->edges_.insert(edge);
						edge->vertex_[1]->edges_.insert(edge);
						if (i == number_of_segments-1)
						{
							border[edge2->index_].push_back(edge);
						}
					t->edge_[2] = edge;
					//t->vertex_[0]->faces_.push_back(t);
					//t->vertex_[1]->faces_.push_back(t);
					//t->vertex_[2]->faces_.push_back(t);
					t->vertex_[0]->faces_.insert(t);
					t->vertex_[1]->faces_.insert(t);
					t->vertex_[2]->faces_.insert(t);
					t->edge_[0]->face_[0] = t;
					t->edge_[1]->face_[0] = t;
					t->edge_[2]->face_[0] = t;
							#ifdef with_indices
							t->index_ = triangles.size();
							#endif
					triangles.push_back(t);
			}
		}
					#ifdef debug_triangulation
					TTriangulatedSES<T>* face_part = new TTriangulatedSES<T>;
					for (Position i = 0; i != points.size(); i++)
					{
						face_part->points_.push_back(points[i]);
					}
					for (Position i = 0; i != edges.size(); i++)
					{
						face_part->edges_.push_back(edges[i]);
					}
					for (Position i = 0; i != triangles.size(); i++)
					{
						face_part->triangles_.push_back(triangles[i]);
					}
					face_part->printToHINFile("DATA/MOLECULE/toricFace"+IndexToString(tindex,(T)0)+".hin");
					tindex++;
					face_part->points_.clear();
					face_part->edges_.clear();
					face_part->triangles_.clear();
					delete face_part;
					/*if (edge0 != NULL)
					{
						Contour2HIN(border[edge0->index_],"DATA/MOLECULE/edge"+IndexToString(edge0->index_,(T)0)+".hin");
					}
					if (edge1 != NULL)
					{
						Contour2HIN(border[edge1->index_],"DATA/MOLECULE/edge"+IndexToString(edge1->index_,(T)0)+".hin");
					}
					if (edge2 != NULL)
					{
						Contour2HIN(border[edge2->index_],"DATA/MOLECULE/edge"+IndexToString(edge2->index_,(T)0)+".hin");
					}
					if (edge3 != NULL)
					{
						Contour2HIN(border[edge3->index_],"DATA/MOLECULE/edge"+IndexToString(edge3->index_,(T)0)+".hin");
					}*/
					#endif
	}







	#ifdef debug_triangulation
	template <class T>
	void TTriangulatedSES<T>::printToHINFile(string filename)
	{
		setIndices();
		std::vector<Atom*> atom;
		Molecule* molecule = new Molecule;
		typename std::list<TTrianglePoint<T>*>::iterator p;
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
		#ifdef with_bonds
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
			Atom* atom1 = atom[(*t)->vertex_[0]->index_];
			Atom* atom2 = atom[(*t)->vertex_[1]->index_];
			Atom* atom3 = atom[(*t)->vertex_[2]->index_];
			atom1->createBond(*atom3);
			atom1->createBond(*atom2);
			atom3->createBond(*atom2);
			/*if (atom1.number_of_bonds_ >= Atom::MAX_NUMBER_OF_BONDS)
			{
				atom[(*t)->vertex_[0]->index_] = new Atom;
				atom[(*t)->vertex_[0]->index_]->setPosition(atom1->getPosition());
				atom[(*t)->vertex_[0]->index_]->setElement(PTE[Element::H]);
				molecule->insert(*atom[(*t)->vertex_[0]->index_]);
			}
			if (atom2.number_of_bonds_ >= Atom::MAX_NUMBER_OF_BONDS)
			{
				atom[(*t)->vertex_[1]->index_] = new Atom;
				atom[(*t)->vertex_[1]->index_]->setPosition(atom2->getPosition());
				atom[(*t)->vertex_[1]->index_]->setElement(PTE[Element::H]);
				molecule->insert(*atom[(*t)->vertex_[1]->index_]);
			}
			if (atom2.number_of_bonds_ >= Atom::MAX_NUMBER_OF_BONDS)
			{
				atom[(*t)->vertex_[2]->index_] = new Atom;
				atom[(*t)->vertex_[2]->index_]->setPosition(atom3->getPosition());
				atom[(*t)->vertex_[2]->index_]->setElement(PTE[Element::H]);
				molecule->insert(*atom[(*t)->vertex_[2]->index_]);
			}*/
		}
		#endif
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(filename,std::ios::out);
		hinfile << *system;
		hinfile.close();
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


	#ifdef debug_triangulation
	template <class T>
	void TTriangulatedSES<T>::Contour2HIN(const std::list<TTriangleEdge<T>*>& contour, const string& file)
	{
		HashSet<TTrianglePoint<T>*> points;
		typename std::list<TTriangleEdge<T>*>::const_iterator c;
		for (c = contour.begin(); c != contour.end(); c++)
		{
			points.insert((*c)->vertex_[0]);
			points.insert((*c)->vertex_[1]);
		}
		Molecule* molecule = new Molecule;
		std::vector<Atom*> atom;
		typename HashSet<TTrianglePoint<T>*>::Iterator p;
		for (p = points.begin(); p != points.end(); p++)
		{
			(*p)->index_ = atom.size();
			Atom* new_atom = new Atom;
			new_atom->setPosition((*p)->point_);
			new_atom->setElement(PTE[Element::H]);
			atom.push_back(new_atom);
			molecule->insert(*new_atom);
		}
		for (c = contour.begin(); c != contour.end(); c++)
		{
			atom[(*c)->vertex_[0]->index_]->createBond(*atom[(*c)->vertex_[1]->index_]);
		}
		System* system = new System;
		system->insert(*molecule);
		HINFile hinfile(file,std::ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}


	template <typename T>
	void TTriangulatedSES<T>::SESEdge2HIN(TSESEdge<T>* edge, const string& file)
	{
		std::list<TTriangleEdge<T>*> contour;
		std::list<TTrianglePoint<T>*> contour_points;
		if (edge->vertex_[0] == NULL)
		{
			TAngle<T> phi(2*Constants::PI,true);
			Size number_of_segments
					= (Size)Maths::round(phi.value*edge->circle_.radius*2.12);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<T> psi(phi.value/number_of_segments,true);
			std::vector< TVector3<T> > points;
			partitionOfCircle(edge->circle_,TVector3<T>::getZero(),psi,number_of_segments,points,false);
			points.pop_back();
			TTrianglePoint<T>* p0;
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = new TTrianglePoint<T>;
			p1->point_ = points[0];
			p1->normal_ = edge->circle_.p-p1->point_;
			contour_points.push_back(p1);
			p0 = p1;
			for (Position k = 1; k < points.size(); k++)
			{
				p2 = new TTrianglePoint<T>;
				p2->point_ = points[k];
				p2->normal_ = edge->circle_.p-p2->point_;
				contour_points.push_back(p2);
				e = new TTriangleEdge<T>;
				e->vertex_[0] = p1;
				e->vertex_[1] = p2;
				contour.push_back(e);
				p1->edges_.insert(e);
				p2->edges_.insert(e);
				p1 = p2;
			}
			e = new TTriangleEdge<T>;
			e->vertex_[0] = p1;
			e->vertex_[1] = p0;
			contour.push_back(e);
			p1->edges_.insert(e);
			p0->edges_.insert(e);
		}
		else
		{
			TAngle<T> phi;
			GetAngle(edge->vertex_[0]->point_-edge->circle_.p,
							 edge->vertex_[1]->point_-edge->circle_.p,
							 phi);
			Size number_of_segments
					= (Size)Maths::round(phi.value*edge->circle_.radius*2.12);
			if (number_of_segments == 0)
			{
				number_of_segments++;
			}
			TAngle<T> psi(phi.value/number_of_segments,true);
			TVector3<T> normal((edge->vertex_[0]->point_-edge->circle_.p)%
												 (edge->vertex_[1]->point_-edge->circle_.p));
			edge->circle_.n = normal;
			std::vector< TVector3<T> > points;
			partitionOfCircle(edge->circle_,edge->vertex_[0]->point_,psi,number_of_segments,
												points);
			points.pop_back();
			points.push_back(edge->vertex_[1]->point_);
			TTrianglePoint<T>* p1;
			TTrianglePoint<T>* p2;
			TTriangleEdge<T>* e;
			p1 = new TTrianglePoint<T>;
			p1->point_ = points[0];
			p1->normal_ = edge->circle_.p-p1->point_;
			contour_points.push_back(p1);
			for (Position k = 1; k < points.size(); k++)
			{
				p2 = new TTrianglePoint<T>;
				p2->point_ = points[k];
				p2->normal_ = edge->circle_.p-p2->point_;
				contour_points.push_back(p2);
				e = new TTriangleEdge<T>;
				e->vertex_[0] = p1;
				e->vertex_[1] = p2;
				contour.push_back(e);
				p1->edges_.insert(e);
				p2->edges_.insert(e);
				p1 = p2;
			}
		}
		Contour2HIN(contour,file);
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = contour.begin(); e != contour.end(); e++)
		{
			delete *e;
		}
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = contour_points.begin(); p != contour_points.end(); p++)
		{
			delete *p;
		}
	}
	#endif


	#ifdef debug_triangulation
	template <class T>
	void PartitionOfCircle
			(const TCircle3<T>&			circle,
			 const TVector3<T>&			p0,
			 const TAngle<T>&				phi,
			 Size										number_of_segments,
			 vector< TVector3<T> >&	partition)
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
		partition.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
		for (Size i = 0; i < number_of_segments; i++)
		{
			p = rotation*p;
			partition.push_back(TVector3<T>(p.x,p.y,p.z)+circle.p);
		}
	}
	
	template <class T>
	void Circle2HIN(const TCircle3<T>& circle, const string& file)
	{
		vector< TVector3<T> > points;
		PartitionOfCircle(circle,TVector3<T>(1,1,1),TAngle<T>(Constants::PI/12.0,true),12,points);
		System* system = new System;
		Molecule* molecule = new Molecule;
		Atom* a1;
		Atom* a2;
		a1 = new Atom;
		a1->setPosition(points[0]);
		a1->setElement(PTE[Element::H]);
		molecule->insert(*a1);
		for (Position k = 1; k < points.size(); k++)
		{
			a2 = new Atom;
			a2->setPosition(points[k]);
			a2->setElement(PTE[Element::H]);
			molecule->insert(*a2);
			a1->createBond(*a2);
			a1 = a2;
		}
		TVector3<T> vector(points[points.size()-1]);
		points.clear();
		system->insert(*molecule);
		PartitionOfCircle(circle,vector,TAngle<T>(Constants::PI/12.0,true),12,points);
		a1 = new Atom;
		a1->setPosition(points[0]);
		a1->setElement(PTE[Element::H]);
		molecule->insert(*a1);
		for (Position k = 1; k < points.size(); k++)
		{
			a2 = new Atom;
			a2->setPosition(points[k]);
			a2->setElement(PTE[Element::H]);
			molecule->insert(*a2);
			a1->createBond(*a2);
			a1 = a2;
		}
		system->insert(*molecule);
		HINFile hinfile(file,ios::out);
		hinfile << *system;
		hinfile.close();
		delete system;
	}
	#endif



}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSES_H


