// $Id: triangulatedSAS.h,v 1.1 2002/06/19 12:53:27 strobel Exp $

#ifndef BALL_STRUCTURE_TRIANGULATEDSAS_H
#define BALL_STRUCTURE_TRIANGULATEDSAS_H

//#define with_indices
//#define debug_triangulation
#ifdef debug_triangulation
#	define with_bonds
//#	define with_normals
#endif
#ifdef with_indices
#	define print_debug_info
#endif

#ifdef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
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
#	include <BALL/STRUCTURE/solventAccessibleSurface.h>
#endif

#include <list>
#include <vector>

namespace BALL
{

		#ifdef debug_triangulation
		Molecule* sewMolecule;
		System* sewSystem;
		#endif
	/** Generic TriangulatedSAS Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangulatedSAS.h}
			\\
	*/
	template <class T>
	class TTriangulatedSAS	:	public TTriangulatedSurface<T>
	{
		
		public:

		#ifdef debug_triangulation
		void printToHINFile(string filename);
		void Contour2HIN(const std::list<TTriangleEdge<T>*>& contour, const string& file);
		void SASEdge2HIN(TSASEdge<T>* edge, const string& file);
		#endif

		BALL_CREATE(TTriangulatedSAS)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangulatedSAS object.
		*/
		TTriangulatedSAS()
			throw();

		/**	Copy constructor.
				Create a new TriangulatedSAS object from another.
				@param	surface	the TriangulatedSAS object to be copied
				@param	bool		ignored - just for interface consistency
		*/
		TTriangulatedSAS(const TTriangulatedSAS<T>& surface, bool = true)
			throw(Exception::GeneralException);

		/** Detailed constructor.
				Create a new empty TriangulatedSAS object and set its SAS.
				@param	ses			a pointer to the corresponding SAS
				@param	density	the density to use by triangulation
		*/
		TTriangulatedSAS(TSolventAccessibleSurface<T>* sas, const T& density)
			throw();

		/**	Destructor.
				Destructs the TriangulatedSAS object.
		*/
		virtual ~TTriangulatedSAS()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another TriangulatedSAS.
				@param	surface	the TriangulatedSAS object to assign from
				@param	bool		ignored - just for interface consistency
		*/
		void set(const TTriangulatedSAS<T>& surface, bool = true)
			throw(Exception::GeneralException);

		/**	Assign from another TriangulatedSAS.
				@param	surface	the TriangulatedSAS object to assign from
		*/
		void operator=(const TTriangulatedSAS<T>& surface)
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

		void partitionEdge
				(TSASEdge<T>*															edge,
				 const T&																	sqrt_density,
				 std::vector< std::list< TVector3<T> > >& points)
			throw();

		void triangulateFace
				(TSASFace<T>*																			face,
				 const TSphere3<T>&																sphere,
				 const HashMap< Size,std::list< TVector3<T> > >&	spheres,
				 const std::vector< std::list< TVector3<T> > >&		points)
			throw();

		bool buildSphericTriangles
				(TTriangulatedSAS<T>&	part,
				 const TSphere3<T>&		sphere
			 			#ifdef print_debug_info
				 		, int halt
			 			#endif
			 	)
			throw();

		bool buildFirstTriangle
				(TTriangulatedSAS<T>&						part,
				 const TSphere3<T>&							sphere,
				 std::list<TTriangleEdge<T>*>&	border
							#ifdef print_debug_info
							, int& halt
							#endif
			 	)
			throw();

		void buildUnambiguousTriangle
				(TTriangleEdge<T>*						 edge,
				 TTrianglePoint<T>*						 point,
				 std::list<TTriangleEdge<T>*>& border,
				 const TSphere3<T>&						 sphere,
				 TTriangulatedSAS<T>&					 part
							#ifdef print_debug_info
							, int& halt
							#endif
				)
			throw();

		void buildAmbiguousTriangles
				(TTriangleEdge<T>*						 edge,
				 std::list<TTrianglePoint<T>*> points,
				 std::list<TTriangleEdge<T>*>& border,
				 const TSphere3<T>&						 sphere,
				 TTriangulatedSAS<T>&					 part
							#ifdef print_debug_info
							, int& halt
							#endif
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
				 TTriangle<T>*&			triangle
				)
			throw();

		void partitionOfCircle
				(const TCircle3<T>&					circle,
				 const TVector3<T>&					p0,
				 const TAngle<T>&						phi,
				 Size												number_of_segments,
				 std::list< TVector3<T> >&	partition,
				 bool												on_surface = true)
			throw();

		Size numberOfRefinements(const T& density, const T& radius)
			throw();

		void buildSpheres(HashMap< Size,std::list< TVector3<T> > >& spheres)
			throw();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		TSolventAccessibleSurface<T>* sas_;

		T density_;

		//@}

	};


	/**	The Default TriangulatedSAS type.
			If double precision is not needed, {\tt TTriangulatedSAS<float>} should
			be used. It is predefined as {\tt TriangulatedSAS} for convenience.
	*/
	typedef TTriangulatedSAS<float> TriangulatedSAS;



	template <class T>
	TTriangulatedSAS<T>::TTriangulatedSAS()
		throw()
		:	TTriangulatedSurface<T>(),
			sas_(NULL),
			density_(4.5)
	{
	}


	template <class T>
	TTriangulatedSAS<T>::TTriangulatedSAS(const TTriangulatedSAS<T>& surface, bool deep)
		throw(Exception::GeneralException)
		:	TTriangulatedSurface<T>(surface,deep),
			sas_(surface.sas_),
			density_(surface.density_)
	{
	}


	template <class T>
	TTriangulatedSAS<T>::TTriangulatedSAS(TSolventAccessibleSurface<T>* sas, const T& density)
		throw()
		:	TTriangulatedSurface<T>(),
			sas_(sas),
			density_(density)
	{
	}


	template <class T>
	TTriangulatedSAS<T>::~TTriangulatedSAS()
		throw()
	{
//cout << "#1\n";
//Position i = 0;
		typename std::list<TTrianglePoint<T>*>::iterator p;
		for (p = points_.begin(); p != points_.end(); p++)
		{
//cout << "p" << i << "\n";
//i++;
			delete *p;
		}
//cout << "#2\n";
//i = 0;
		typename std::list<TTriangleEdge<T>*>::iterator e;
		for (e = edges_.begin(); e != edges_.end(); e++)
		{
//cout << "e" << i << "\n";
//i++;
			delete *e;
		}
//cout << "#3\n";
//i = 0;
		typename std::list<TTriangle<T>*>::iterator t;
		for (t = triangles_.begin(); t != triangles_.end(); t++)
		{
//cout << "t" << i << "\n";
//i++;
			delete *t;
		}
//cout << "#4\n";
		points_.clear();
		edges_.clear();
		triangles_.clear();
	}


	template <class T>
	void TTriangulatedSAS<T>::set(const TTriangulatedSAS<T>& surface, bool)
		throw(Exception::GeneralException)
	{
		*this = surface;
	}


	template <class T>
	void TTriangulatedSAS<T>::operator=(const TTriangulatedSAS<T>& surface)
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
		sas_ = surface.sas_;
		density_ = surface.density_;
	}


	template <class T>
	void TTriangulatedSAS<T>::setDensity(const T& density)
		throw()
	{
		density_ = density;
	}


	template <class T>
	T TTriangulatedSAS<T>::getDensity() const
		throw()
	{
		return density_;
	}


	template <class T>
	void TTriangulatedSAS<T>::compute()
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
		// Sometimes we need the square root of the density.
  	T sqrt_density = sqrt(density_);
		// Store for each SASEdge a list of Vector3's.
		std::vector< std::list< TVector3<T> > > edge(sas_->number_of_edges_);
		typename std::vector<TSASEdge<T>*>::iterator e;
		for (e = sas_->edges_.begin(); e != sas_->edges_.end(); e++)
		{
			partitionEdge(*e,sqrt_density,edge);
		}
				#ifdef print_debug_info
				for (Position i = 0; i < edge.size(); i++)
				{
					if ((edge[i].size() == 0) && (sas_->edges_[i] != NULL))
					{
						std::cout << "Edge" << i << " ohne Kontur!\n";
					}
				}
				#endif
		// Now we triangulate the faces.
		// build template spheres with different densities and outside normal vectors
		HashMap< Size,std::list< TVector3<T> > > spheres;
		buildSpheres(spheres);
		TSASFace<T>* current_face;
		//TSphere3<T> sphere;
		for (Position i = 0; i < sas_->number_of_faces_; i++)
		{
			current_face = sas_->faces_[i];
			triangulateFace(current_face,current_face->sphere_,spheres,edge);
if (number_of_edges_ > 14)//1493)
{
//setIndices();
//ofstream file("suface.log");
//file << (TriangulatedSurface)*this;
//file.close();
//cout << i << " " << sas_->number_of_faces_ << "\n";
//int in; cin >> in;
}
		}
/*typename std::list<TTrianglePoint<T>*>::const_iterator ppp;
cout << "Points: " << number_of_points_ << " = " << points_.size() << "\n";
Position k = 1;
Position s = points_.size();
for (ppp = points_.begin(); ppp != points_.end(); ppp++)
{
	cout << "##\n";
	cout << **ppp << "\n";
	cout << k << " von " << s << "\n";
	k++;
}
cout << "$$$\n";
typename std::list<TTriangleEdge<T>*>::const_iterator eee;
cout << "Edges: \n";
cout << number_of_edges_ << " = \n";
//cout << edges_.size() << "\n";
k = 1;
for (eee = edges_.begin(); eee != edges_.end(); eee++)
{
	cout << *eee << "\n";
	cout << "v" << (*eee)->vertex_[0] << "\n";
	cout << "v" << (*eee)->vertex_[1] << "\n";
	cout << "f" << (*eee)->face_[0] << "\n";
	cout << "f" << (*eee)->face_[1] << "\n";
	cout << (*eee)->vertex_[0]->index_ << "\n";
	cout << (*eee)->vertex_[1]->index_ << "\n";
	cout << (((*eee)->face_[0] == NULL) ? -2 : (*eee)->face_[0]->index_) << "\n";
	cout << (((*eee)->face_[1] == NULL) ? -2 : (*eee)->face_[1]->index_) << "\n";
	cout << **eee << "\n";
	cout << k << " von " << number_of_edges_ << "\n";
	k++;
}
typename std::list<TTriangle<T>*>::const_iterator ttt;
cout << "Triangles: " << number_of_triangles_ << " = " << triangles_.size() << "\n";
for (ttt = triangles_.begin(); ttt != triangles_.end(); ttt++)
{
	cout << **ttt << "\n";
}
setIndices();*/
	}


	template <class T>
	void TTriangulatedSAS<T>::partitionEdge
			(TSASEdge<T>*															edge,
			 const T&																	sqrt_density,
			 std::vector< std::list< TVector3<T> > >&	points)
		throw()
	{
		TAngle<T> phi(edge->angle_);
		Size number_of_segments
				= (Size)Maths::round(phi.value*edge->circle_.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		TAngle<T> psi(phi.value/number_of_segments,true);
		if (edge->vertex_[0] == NULL)
		{
			partitionOfCircle(edge->circle_,TVector3<T>::getZero(),psi,number_of_segments,
												points[edge->index_],false);
			points[edge->index_].pop_back();
		}
		else
		{
			partitionOfCircle(edge->circle_,edge->vertex_[0]->point_,psi,number_of_segments,
												points[edge->index_],true);
			points[edge->index_].pop_front();
			points[edge->index_].pop_back();
			//points[edge->index_].push_back(edge->vertex_[1]->point_);
		}
	}


	template <class T>
	void TTriangulatedSAS<T>::triangulateFace
			(TSASFace<T>*																			face,
			 const TSphere3<T>&																sphere,
			 const HashMap< Size,std::list< TVector3<T> > >&	spheres,
			 const std::vector< std::list< TVector3<T> > >&		edge_contours)
		throw()
	{
					#ifdef debug_surface_processor_verbose
					std::cout << "    ... " << face->index_ << " ... ";
					#endif
//cout << "$0\n";
		std::list< TVector3<T> > border_points;
		typename std::list<TSASEdge<T>*>::iterator edge;
		typename std::list< TVector3<T> >::const_iterator p;
		for (edge = face->edge_.begin(); edge != face->edge_.end(); edge++)
		{
			for (p = edge_contours[(*edge)->index_].begin();
					 p != edge_contours[(*edge)->index_].end();
					 p++)
			{
				border_points.push_back(*p);
			}
		}
		typename std::list<TSASVertex<T>*>::iterator vertex;
		for (vertex = face->vertex_.begin(); vertex != face->vertex_.end(); vertex++)
		{
			border_points.push_back((*vertex)->point_);
		}
//cout << "$0.5\n";
		Position number_of_edges = face->edge_.size();
		if (number_of_edges > 0)
		{
			// the face does not come from a free vertex
			if (number_of_edges == 2)
			{
				if ((edge_contours[face->edge_.front()->index_].size() == 2) &&
						(edge_contours[face->edge_.back()->index_].size() == 2)		)
				{
					return;
				}
			}
			if (number_of_edges == 3)
			{
				Position counter = 0;
				typename std::list<TSASEdge<T>*>::iterator it;
				for (it = face->edge_.begin(); it != face->edge_.end(); it++)
				{
					if (edge_contours[(*it)->index_].size() == 2)
					{
						counter++;
					}
				}
				if (counter == 3)
				{
					TTriangle<T>* triangle = new TTriangle<T>;
					Position k = 0;
					for (p = border_points.begin(); p != border_points.end(); p++)
					{
						triangle->vertex_[k] = new TTrianglePoint<T>;
						triangle->vertex_[k]->point_ = *p;
						triangle->vertex_[k]->normal_ = *p-sphere.p;
						triangle->vertex_[k]->normal_.normalize();
						k++;
					}
					triangles_.push_back(triangle);
					points_.push_back(triangle->vertex_[0]);
					points_.push_back(triangle->vertex_[1]);
					points_.push_back(triangle->vertex_[2]);
					number_of_triangles_++;
					number_of_points_ += 3;
							#ifdef print_debug_info
							std::cout << "einzelnes Dreieck\n";
							#endif
					return;
				}
			}
			// get a template sphere for the face to triangulate
			TTriangulatedSAS<T> part;
			typename HashMap< Size,std::list< TVector3<T> > >::ConstIterator s
					= spheres.find(numberOfRefinements(density_,sphere.radius));
			TTrianglePoint<T>* point;
			for (p = s->second.begin(); p != s->second.end(); p++)
			{
				point = new TTrianglePoint<T>;
				point->point_ = *p;
				point->normal_ = *p-sphere.p;
				point->normal_.normalize();
				point->index_ = 1;
				part.points_.push_back(point);
				part.number_of_points_++;
			}
			part.blowUp(sphere.radius);
				#ifdef print_debug_info
				std::cout << part.number_of_points_ << " (" << part.points_.size() << ")\n";
				#endif
			// cut the face with all its edges
			TPlane3<T> plane;
			typename std::list<bool>::iterator o = face->orientation_.begin();
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
				point = new TTrianglePoint<T>;
				point->point_ = *p;
				point->normal_ = *p-sphere.p;
				point->normal_.normalize();
				part.points_.push_back(point);
				part.number_of_points_++;
			}
					#ifdef debug_triangulation
					part.printToHINFile("DATA/MOLECULE/contactFaceCut"+IndexToString(face->index_,0)+".hin");
					#endif
					#ifdef print_debug_info
					int halt = 0;
					if (halt == 0) cin >> halt; else { cout << "\n"; halt--;}
					#endif
					#ifdef debug_triangulation
					sewSystem = new System;
					sewMolecule = new Molecule;
					#endif
			#ifdef print_debug_info
			buildSphericTriangles(part,sphere,halt);
			#else
			buildSphericTriangles(part,sphere);
			#endif
//std::cout << "    ... " << face->index_ << "\n";
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
//std::cout << "    ... " << face->index_ << "\n";
					#ifdef debug_triangulation
					sewSystem->insert(*sewMolecule);
					HINFile output("DATA/MOLECULE/contactFaceSew"+IndexToString(face->index_,0)+".hin",std::ios::out);
					output << *sewSystem;
					output.close();
					delete sewSystem;
					#endif
//std::cout << "    ... " << face->index_ << "\n";
			join(part);
					#ifdef debug_surface_processor_verbose
					std::cout << "cut, joined\n";
					#endif
		}
		else
		{
			// the face comes from a free vertex
			TTriangulatedSAS<T> part;
			part.icosaeder();
			part.refineSphere(numberOfRefinements(density_,sphere.radius));
			part.shift(sphere.p);
			join(part);
					#ifdef debug_surface_processor_verbose
					std::cout << "full sphere\n";
					#endif
		}
//cout << "$10\n";
	}


	template <class T>
	bool TTriangulatedSAS<T>::buildSphericTriangles
			(TTriangulatedSAS<T>&	part,
			 const TSphere3<T>&		sphere
		 			#ifdef print_debug_info
			 		, int halt
			 		#endif
			)
		throw()
	{
//cout << "#-1\n";
				#ifdef print_debug_info
				string pre = "      ";
				#endif
		bool ok;
		std::list<TTriangleEdge<T>*> border;
		#ifdef print_debug_info
		ok = buildFirstTriangle(part,sphere,border,halt);
		#else
		ok = buildFirstTriangle(part,sphere,border);
		#endif
		if (!ok)
		{
			return false;
		}
		while (!border.empty())
		{
//cout << "#0\n";
					#ifdef print_debug_info
					std::cout << pre << "derzeit (" << border.size() << ") Edges";
					if (halt == 0) std::cin >> halt; else { std::cout << "\n"; halt--; }
			  	#endif
			TTriangleEdge<T>* edge = border.front();
			border.pop_front();
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
			if ((diff == -1) || (diff == 2))
			{
				TTrianglePoint<T>* tmp = edge->vertex_[0];
				edge->vertex_[0] = edge->vertex_[1];
				edge->vertex_[1] = tmp;
			}
					#ifdef print_debug_info
					std::cout << pre << "starte mit Edge " << *edge << " (" << edge << ")\n";
					std::cout << pre << "third point: " << third_point->index_ << "\n";
					std::cout << pre << "Dreieck: " << *start_triangle << " (" << test_value << ")  ";
					if (halt == 0)  std::cin >> halt; else { std::cout << "\n"; halt--; }
					#endif
			typename std::list<TTrianglePoint<T>*>::iterator next = part.points_.begin();
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
			while (next != part.points_.end())
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
									buildUnambiguousTriangle(edge,third.front(),border,sphere,part,halt);
									#else
									buildUnambiguousTriangle(edge,third.front(),border,sphere,part);
									#endif
//cout << "#1\n";
									break;
				default :
									#ifdef print_debug_info
									buildAmbiguousTriangles(edge,third,border,sphere,part,halt);
									#else
									buildAmbiguousTriangles(edge,third,border,sphere,part);
									#endif
									break;
			}
//cout << "#2\n";
		}
//cout << "#3\n";
		return true;
	}


	template <class T>
	bool TTriangulatedSAS<T>::buildFirstTriangle
			(TTriangulatedSAS<T>&						part,
			 const TSphere3<T>&							sphere,
			 std::list<TTriangleEdge<T>*>&	border
						#ifdef print_debug_info
						, int& halt
						#endif
			)
		throw()
	{
		std::list<TTrianglePoint<T>*> inner_points;
		typename std::list<TTrianglePoint<T>*>::iterator p;
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
			cout << "#########\n";
			return false;
		}
		else
		{
			TTrianglePoint<T>* point1;
			TTrianglePoint<T>* point2;
			TTrianglePoint<T>* point3;
			point1 = inner_points.front();
			typename std::list<TTrianglePoint<T>*>::iterator p1;
			typename std::list<TTrianglePoint<T>*>::iterator p2;
			typename std::list<TTrianglePoint<T>*>::iterator test;
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
							TPlane3<T> plane(point1->point_,(*p1)->point_,(*p2)->point_);
							T test_value = plane.n*point1->point_;
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
				TTriangle<T>* triangle = new TTriangle<T>;
				TTriangleEdge<T>* edge1 = new TTriangleEdge<T>;
				TTriangleEdge<T>* edge2 = new TTriangleEdge<T>;
				TTriangleEdge<T>* edge3 = new TTriangleEdge<T>;
				TVector3<T> test((point2->point_-point1->point_)%
												 (point3->point_-point1->point_));
				if (Maths::isGreater(test*(sphere.p-point1->point_),(T)0))
				{
					TTrianglePoint<T>* tmp = point2;
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
//cout << "$$\n";
//cout << *edge1 << "\n";
//cout << *edge2 << "\n";
//cout << *edge3 << "\n";
//cout << "$$\n";
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


	template <class T>
	void TTriangulatedSAS<T>::buildUnambiguousTriangle
			(TTriangleEdge<T>*						 edge,
			 TTrianglePoint<T>*						 point,
			 std::list<TTriangleEdge<T>*>& border,
			 const TSphere3<T>&						 sphere,
			 TTriangulatedSAS<T>&					 part
						#ifdef print_debug_info
						, int& halt
						#endif
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
//cout << "##\n";
//cout << *edge1 << "\n";
//cout << "##\n";
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
//cout << "--\n";
//cout << *edge2 << "\n";
//cout << "--\n";
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
					atom1->createBond(*atom2);
					atom2->createBond(*atom3);
					atom3->createBond(*atom1);
					sewMolecule->insert(*atom1);
					sewMolecule->insert(*atom2);
					sewMolecule->insert(*atom3);
					#endif
	}


	template <class T>
	void TTriangulatedSAS<T>::buildAmbiguousTriangles
			(TTriangleEdge<T>*						 edge,
			 std::list<TTrianglePoint<T>*> points,
			 std::list<TTriangleEdge<T>*>& border,
			 const TSphere3<T>&						 sphere,
			 TTriangulatedSAS<T>&					 part
						#ifdef print_debug_info
						, int& halt
						#endif
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
					createTriangleAndEdges(edge0,*p,sphere,edge1,old1,edge2,old2,triangle);
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
							if ((edge1->vertex_[0]->index_ > 0) || (edge1->vertex_[1]->index_ > 0))
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
//cout << "++\n";
//cout << *edge1 << "\n";
//cout << "++\n";
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
							if ((edge2->vertex_[0]->index_ > 0) || (edge2->vertex_[1]->index_ > 0))
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
//cout << "//\n";
//cout << *edge2 << "\n";
//cout << "//\n";
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
	void TTriangulatedSAS<T>::createTriangleAndEdges
			(TTriangleEdge<T>*	edge,
			 TTrianglePoint<T>* point,
			 const TSphere3<T>& sphere,
			 TTriangleEdge<T>*& edge1,
			 bool&							old1,
			 TTriangleEdge<T>*& edge2,
			 bool&							old2,
			 TTriangle<T>*&			triangle
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
		if (Maths::isGreater(test_value,0))
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
	void TTriangulatedSAS<T>::partitionOfCircle
			(const TCircle3<T>&					circle,
			 const TVector3<T>&					p0,
			 const TAngle<T>&						phi,
			 Size												number_of_segments,
			 std::list< TVector3<T> >&	partition,
			 bool												on_surface)
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
	Size TTriangulatedSAS<T>::numberOfRefinements(const T& density, const T& radius)
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
//n=1;
		return n;
	}


	template <class T>
	void TTriangulatedSAS<T>::buildSpheres(HashMap< Size,std::list< TVector3<T> > >& spheres)
		throw()
	{
		typename std::list<TTrianglePoint<T>*>::const_iterator p;
		TTriangulatedSAS<T> sphere;
		sphere.icosaeder(true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			spheres[0].push_back((*p)->point_);
		}
		sphere.refineSphere(1,true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			spheres[1].push_back((*p)->point_);
		}
		sphere.refineSphere(1,true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			spheres[2].push_back((*p)->point_);
		}
		sphere.refineSphere(1,true);
		for (p = sphere.points_.begin(); p != sphere.points_.end(); p++)
		{
			spheres[3].push_back((*p)->point_);
		}
	}





	#ifdef debug_triangulation
	template <class T>
	void TTriangulatedSAS<T>::printToHINFile(string filename)
	{
		Molecule* molecule = new Molecule;
		#ifdef with_bonds
		typename std::list<TTriangle<T>*>::iterator t;
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
	void TTriangulatedSAS<T>::Contour2HIN(const std::list<TTriangleEdge<T>*>& contour, const string& file)
	{
		HashSet<TTrianglePoint<T>*> points;
		typename std::list<TTriangleEdge<T>*>::const_iterator c;
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


	template <typename T>
	void TTriangulatedSAS<T>::SASEdge2HIN(TSASEdge<T>* edge, const string& file)
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


#endif	// BALL_STRUCTURE_TRIANGULATEDSAS_H


