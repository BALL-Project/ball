// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: solventExcludedSurface.h,v 1.21.4.6 2002/11/26 15:28:26 oliver Exp $

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#define BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H

#ifdef DEBUG
//#	define debug_singularities
//#	define print_clean_info
//#	define print_ses
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

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#	include <BALL/STRUCTURE/reducedSurface.h>
#endif

#ifndef BALL_MATHS_ANALYTICALGEOMETRY_H
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

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector4.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#	include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#include <vector>
#include <list>

#	include <BALL/MATHS/vector3.h>
#	include <BALL/MATHS/vector4.h>
#	include <BALL/MATHS/angle.h>
#	include <BALL/MATHS/quaternion.h>
#	include <BALL/MATHS/matrix44.h>



namespace BALL
{

	template <class T>
	class TTriangulatedSES;

	/** Generic SolventExcludedSurface Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/solventExcludedSurface.h}	\\
	*/
	template <class T>
	class TSolventExcludedSurface
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class TTriangulatedSES<T>
				\end{itemize}
		*/
		friend class TTriangulatedSES<T>;

		BALL_CREATE(TSolventExcludedSurface)

		typedef std::pair< std::pair<TAngle<T>,Index>,TVector3<T> > Intersection;

		struct ProbeIntersection
		{
			std::vector< TVector3<T> > point;
			//TVector3<T> point1;
			//TVector3<T> point2;
			//int point1;
			//int point2;
			//Vector3 point1;
			//Vector3 point2;
		};

		/**	@name	Constructors and Destructors
		*/
		//@{
		
	  #ifndef no_ses_print_functions
		void partitionOfCircle
			(const TCircle3<T>&			circle,
			 const TVector3<T>&			p0,
			 const TAngle<T>&				phi,
			 Size										number_of_segments,
			 vector< TVector3<T> >&	partition,
			 bool										on_surface);
		void partitionFreeEdge
			(TSESEdge<T>* edge,
			 const float& sqrt_density,
			 Molecule* molecule);
		void partitionEdge
			(TSESEdge<T>* edge,
			 const float& sqrt_density,
			 Molecule* molecule);
		void partitionSingularEdge
			(TSESEdge<T>* edge,
			 const float& sqrt_density,
			 Molecule* molecule);
		void buildHINFiles();
		#endif
		void check()
		{
			Position ok = 0;
			for (Position i = 0; i < number_of_vertices_; i++)
			{
				if (vertices_[i]->edges_.size() != vertices_[i]->faces_.size())
				{
					ok++;
					cout << "\n" << *vertices_[i] << "\n";
				}
			}
			cout << ok << "v  ";
			ok = 0;
			for (Position i = 0; i < number_of_contact_faces_; i++)
			{
				if (contact_faces_[i]->edge_.size() != contact_faces_[i]->vertex_.size())
				{
					int test = contact_faces_[i]->edge_.size()-contact_faces_[i]->vertex_.size();
					typename std::list<TSESEdge<T>*>::iterator e = contact_faces_[i]->edge_.begin();
					while (e != contact_faces_[i]->edge_.end())
					{
						if ((*e)->vertex_[0] == NULL)
						{
							test--;
						}
						e++;
					}
					if (test != 0)
					{
						ok++;
					}
				}
			}
			cout << ok << "c  ";
			ok = 0;
			for (Position i = 0; i < number_of_toric_faces_; i++)
			{
				if (toric_faces_[i]->edge_.size() != toric_faces_[i]->vertex_.size())
				{
					int test = toric_faces_[i]->edge_.size()-toric_faces_[i]->vertex_.size();
					typename std::list<TSESEdge<T>*>::iterator e = toric_faces_[i]->edge_.begin();
					while (e != toric_faces_[i]->edge_.end())
					{
						if ((*e)->vertex_[0] == NULL)
						{
							test--;
						}
						e++;
					}
					if (test != 0)
					{
						ok++;
					}
				}
			}
			cout << ok << "t  ";
			ok = 0;
			for (Position i = 0; i < number_of_spheric_faces_; i++)
			{
				if (spheric_faces_[i]->edge_.size() != spheric_faces_[i]->vertex_.size())
				{
					int test = spheric_faces_[i]->edge_.size()-spheric_faces_[i]->vertex_.size();
					typename std::list<TSESEdge<T>*>::iterator e = spheric_faces_[i]->edge_.begin();
					while (e != spheric_faces_[i]->edge_.end())
					{
						if ((*e)->vertex_[0] == NULL)
						{
							test--;
						}
						e++;
					}
					if (test != 0)
					{
						ok++;
						cout << "\n" << *spheric_faces_[i] << "\n";
					}
				}
			}
			cout << ok << "s\n";
		}

		/**	Default constructor.
				This method creates a new SolventExcludedSurface object.
		*/
		TSolventExcludedSurface()
			throw();

		/**	Copy constructor.
				Create a new SolventExcludedSurface object from another.
				@param	ses		the SolventExcludedSurface object to be copied
				@param	bool	ignored - just for interface consistency
		*/
		//TSolventExcludedSurface(const TSolventExcludedSurface<T>& ses, bool = false)
		//	throw();

		/** Detailed constructor
		*/
		TSolventExcludedSurface(TReducedSurface<T>* reduced_surface)
			throw();

		/**	Destructor.
				As there are no dynamic	data structures, nothing happens.
		*/
		virtual ~TSolventExcludedSurface()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{
		
		void set(TReducedSurface<T>* rs)
			throw();

		void clear();
		void clean(const T& density);

		/**	Computes the solvent excluded surface from a ReducedSurface object
		*/
		void compute()
			throw(Exception::GeneralException);
		void splitSphericFaces()
			throw();

		//@}

		private:

		void splitSphericFace(Position i)
			throw();

		/**	@name	SES computation (private)
		*/
		//@{

		void get()
			throw();

		void createSphericFace(Position j)
			throw();

		TSESVertex<T>* createVertex
			(const TVector3<T>& probe_center,
			 Index index)
			throw();

		void pushVertex
			(TSESFace<T>* face,
			 const TSphere3<T>& probe,
			 TRSVertex<T>* rsvertex)
			throw();

		TSESEdge<T>* createConcaveEdge
			(TSESFace<T>* spheric_face,
			 Position p1,
			 Position p2,
			 Index index,
			 const T& radius_of_probe)
			throw();

		void pushConcaveEdge
			(TSESFace<T>* face,
			 Position p1,
			 Position p2,
			 const T& radius_of_probe)
			throw();

		TSESEdge<T>* createConvexEdge
			(TSESFace<T>* toric_face,
			 TRSVertex<T>* rsvertex)
			throw();

		void createToricFace(Position i)
			throw();

		void treatSingularToricFace(Position i)
			throw();

		void createFreeToricFace(Position i)
			throw();

		void getPoint
			(const TVector3<T>& p1,
			 const TVector3<T>& p2,
			 const T&						dist,
			 TVector3<T>&				result)
			throw();

		void deleteSmallToricFace(TSESFace<T>* face)
			throw();

		void deleteSmallSingularToricFace(TSESFace<T>* face)
			throw();

		Index vertexExists(TVector3<T> point)
			throw();

		//@}

		/**	@name	Treatment of singularities (private)
		*/
		//@{

		void treatSingularities()
			throw();

		void getSingularFaces(::std::list<TSESFace<T>*>& faces)
			throw();

		void treatFirstCategory(::std::list<TSESFace<T>*>& singular_faces)
			throw();

		void treatSecondCategory()
			throw();

		void getFirstCategoryFaces
			(::std::list<TSESFace<T>*>& singular_faces,
			 ::std::list<TSESFace<T>*>& first_category_faces)
			throw();

		void noCut(TSESFace<T>* face1, TSESFace<T>* face2)
			throw();

		void twoCuts(TSESFace<T>* face1, TSESFace<T>* face2)
			throw();

		void treatSingularEdge
			(TSESEdge<T>*												edge,
			 HashGrid3<Position>&								grid,
			 ::std::list<TSESEdge<T>*>&					deletable_edges)
			throw();

		void getIntersectionsOfSingularEdge
			(TSESEdge<T>*							edge,
			 const TAngle<T>&					phi,
			 HashGrid3<Position>&			grid,
			 std::list<Intersection>& intersections)
			throw();

		bool getIntersectionPointsAndAngles
			(//TSESEdge<T>*	edge,
			 const TCircle3<T>& circle,
			 const TVector3<T>& point,
			 Position			index1,
			 Position			index2,
			 Position			probe_index,
			 TAngle<T>&		phi1,
			 TVector3<T>&	point1,
			 TAngle<T>&		phi2,
			 TVector3<T>&	point2)
			throw();

		bool isIntersection
			 (const TAngle<T>&	 min_phi,
				const TAngle<T>&	 max_phi,
				const TAngle<T>&	 phi,
				const TVector3<T>& middle,
				const TSphere3<T>& probe)
			throw();

		void buildEndEdges
			(TSESEdge<T>*					edge,
			 const std::list<Intersection>&	min,
			 const std::list<Intersection>&	max,
			 TSESVertex<T>*&			vertex1,
			 TSESVertex<T>*&			vertex2,
			 Index&								actual_min,
			 Index&								actual_max)
			throw();

		void buildEdge
			(TSESEdge<T>*						edge,
			 Index									face1,
			 Index&									face2,
			 Index									end,
			 TSESVertex<T>*&				vertex,
			 const HashSet<Index>&	indices,
			 bool										minimum)
			throw();

		bool probeIntersection
			 (Index				 face1,
				Index				 face2,
				Index				 face3,
				TVector3<T>& point1,
				TVector3<T>& point2)
			throw();

		Index compareAngles
			 (const TVector3<T>& start_point,
				const TVector3<T>& point1,
				const TVector3<T>& point2,
				const TCircle3<T>& circle)
			throw();

		void sort
			 (Index		u1, Index		u2, Index		u3,
				Index&	s1,	Index&	s2, Index&	s3)
			throw();

		//@}


		//protected:
		public:

		Position number_of_vertices_;
		std::vector<TSESVertex<T>*> vertices_;
		HashGrid3<Index> vertex_grid_;
		Position number_of_edges_;
		std::vector<TSESEdge<T>*> edges_;
		Position number_of_singular_edges_;
		std::list<TSESEdge<T>*> singular_edges_;
		Position number_of_contact_faces_;
		std::vector<TSESFace<T>*> contact_faces_;
		Position number_of_toric_faces_;
		std::vector<TSESFace<T>*> toric_faces_;
		Position number_of_spheric_faces_;
		std::vector<TSESFace<T>*> spheric_faces_;
		TReducedSurface<T>* reduced_surface_;
		HashMap< Position,
						 HashMap< Position,
											HashMap< Position,
															 ProbeIntersection*
														 >
										>
					 > probe_intersections_;
				#ifdef debug_singularities
				int STOP_SINGULARITIES;
				#endif

	};


	/**	Output- Operator.
			Prints out a SolventExcludedSurface
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSolventExcludedSurface<T>& ses)
	{
		s << "Vertices:\n";
		for (Position i = 0; i < ses.vertices_.size(); i++)
		{
			if (ses.vertices_[i] != NULL)
			{
				s << "  " << *ses.vertices_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "Edges:\n";
		for (Position i = 0; i < ses.edges_.size(); i++)
		{
			if (ses.edges_[i] != NULL)
			{
				s << "  " << *ses.edges_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "singular Edges:\n";
		typename ::std::list<TSESEdge<T>*>::const_iterator se;
		for (se = ses.singular_edges_.begin(); se != ses.singular_edges_.end(); se++)
		{
			if (*se != NULL)
			{
				s << "  " << **se << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "contact Faces:\n";
		for (Position i = 0; i < ses.contact_faces_.size(); i++)
		{
			if (ses.contact_faces_[i] != NULL)
			{
				s << "  " << *ses.contact_faces_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "toric Faces:\n";
		for (Position i = 0; i < ses.toric_faces_.size(); i++)
		{
			if (ses.toric_faces_[i] != NULL)
			{
				s << "  " << *ses.toric_faces_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "spheric Faces:\n";
		for (Position i = 0; i < ses.spheric_faces_.size(); i++)
		{
			if (ses.spheric_faces_[i] != NULL)
			{
				s << "  " << *ses.spheric_faces_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		return s;
	}


	/**	The Default SolventExcludedSurface type.
			If double precision is not needed, {\tt TSolventExcludedSurface<float>} should
			be used. It is predefined as {\tt SolventExcludedSurface} for convenience.
	*/
	typedef TSolventExcludedSurface<float> SolventExcludedSurface;


	template <typename T>
	TSolventExcludedSurface<T>::TSolventExcludedSurface()
		throw()
		:	number_of_vertices_(0),
			vertices_(),
			number_of_edges_(0),
			edges_(),
			number_of_singular_edges_(0),
			singular_edges_(),
			number_of_contact_faces_(0),
			contact_faces_(),
			number_of_toric_faces_(0),
			toric_faces_(),
			number_of_spheric_faces_(0),
			spheric_faces_(),
			reduced_surface_(NULL)
	{
	}


	/*template <typename T>
	TSolventExcludedSurface<T>::TSolventExcludedSurface
		(const TSolventExcludedSurface<T>& ses, bool)
		throw()
		:	number_of_vertices_(ses.vertices_.size()),
			vertices_(number_of_vertices_),
			number_of_edges_(ses.edges_.size()),
			edges_(number_of_edges_),
			number_of_singular_edges_(0),
			singular_edges_(),
			number_of_contact_faces_(ses.contact_faces_.size()),
			contact_faces_(number_of_contact_faces_),
			number_of_toric_faces_(ses.toric_faces_.size()),
			toric_faces_(number_of_toric_faces_),
			number_of_spheric_faces_(ses.spheric_faces_.size()),
			spheric_faces_(number_of_spheric_faces_),
			reduced_surface_(NULL)
	{				// ????? !!!
		//for (Position i = 0; i < number_of_vertices_; i++)
		//{
		//	vertices_[i] = new TSESVertex<T>(*ses.vertices_[i],false);
		//}
		//for (Position i = 0; i < number_of_edges_; i++)
		//{
		//	edges_[i] = new TSESVertex<T>(*ses.edges_[i],false);
		//}
		//typename ::std::list<TSESEdge<T>*>::const_iterator se;
		//for (se = ses.singular_edges_.begin(); se != ses.singular_edges_.end(); se++)
		//{
		//	singular_edges_.push_back(edges_[(*se)->index_]);
		//}
		//for (Position i = 0; i < number_of_contact_faces_; i++)
		//{
		//	contact_faces_[i] = new TSESFace<T>(*ses.contact_faces_[i],false);
		//}
		//for (Position i = 0; i < number_of_toric_faces_; i++)
		//{
		//	toric_faces_[i] = new TSESFace<T>(*ses.toric_faces_[i],false);
		//}
		//for (Position i = 0; i < number_of_spheric_faces_; i++)
		//{
		//	spheric_faces_[i] = new TSESFace<T>(*ses.spheric_faces_[i],false);
		//}
	}*/


	template <typename T>
	TSolventExcludedSurface<T>::TSolventExcludedSurface(TReducedSurface<T>* reduced_surface)
		throw()
		:	number_of_vertices_(0),
			vertices_(),
			number_of_edges_(0),
			edges_(),
			number_of_singular_edges_(0),
			singular_edges_(),
			number_of_contact_faces_(reduced_surface->number_of_vertices_),
			contact_faces_(number_of_contact_faces_),
			number_of_toric_faces_(reduced_surface->number_of_edges_),
			toric_faces_(number_of_toric_faces_),
			number_of_spheric_faces_(reduced_surface->number_of_faces_),
			spheric_faces_(number_of_spheric_faces_),
			reduced_surface_(reduced_surface)
	{
		Position i;
		for (i = 0; i < number_of_contact_faces_; i++)
		{
			contact_faces_[i] = new TSESFace<T>;
			contact_faces_[i]->type_ = TSESFace<T>::TYPE_CONTACT;
			contact_faces_[i]->rsvertex_ = reduced_surface_->vertices_[i];
			contact_faces_[i]->rsedge_ = NULL;
			contact_faces_[i]->rsface_ = NULL;
			contact_faces_[i]->index_ = i;
		}
		for (i = 0; i < number_of_toric_faces_; i++)
		{
			toric_faces_[i] = new TSESFace<T>;
			toric_faces_[i]->type_ = TSESFace<T>::TYPE_TORIC;
			toric_faces_[i]->rsvertex_ = NULL;
			toric_faces_[i]->rsedge_ = reduced_surface_->edges_[i];
			toric_faces_[i]->rsface_ = NULL;
			toric_faces_[i]->index_ = i;
		}
		for (i = 0; i < number_of_spheric_faces_; i++)
		{
			spheric_faces_[i] = new TSESFace<T>;
			spheric_faces_[i]->type_ = TSESFace<T>::TYPE_SPHERIC;
			spheric_faces_[i]->rsvertex_ = NULL;
			spheric_faces_[i]->rsedge_ = NULL;
			spheric_faces_[i]->rsface_ = reduced_surface_->faces_[i];
			spheric_faces_[i]->index_ = i;
		}
		T x_min = reduced_surface_->x_min_;
		T y_min = reduced_surface_->y_min_;
		T z_min = reduced_surface_->z_min_;
		T x_max = reduced_surface_->x_max_;
		T y_max = reduced_surface_->y_max_;
		T z_max = reduced_surface_->z_max_;
		T dist = reduced_surface_->r_max_/3;
		Position nx = (Position)((x_max-x_min)/dist+5);
		Position ny = (Position)((y_max-y_min)/dist+5);
		Position nz = (Position)((z_max-z_min)/dist+5);
		TVector3<T> origin(x_min-2*dist,y_min-2*dist,z_min-2*dist);
		vertex_grid_ = HashGrid3<Index>(origin,nx,ny,nz,dist);
	}


	template <typename T>
	TSolventExcludedSurface<T>::~TSolventExcludedSurface()
		throw()
	{
		clear();
	}


	template <typename T>
	void TSolventExcludedSurface<T>::set(TReducedSurface<T>* rs)
		throw()
	{
		clear();
		reduced_surface_ = rs;
		number_of_contact_faces_ = reduced_surface_->number_of_vertices_;
		number_of_toric_faces_ = reduced_surface_->number_of_edges_;
		number_of_spheric_faces_ = reduced_surface_->number_of_faces_;
		Position i;
		for (i = 0; i < number_of_contact_faces_; i++)
		{
			TSESFace<T>* face = new TSESFace<T>;
			face->type_ = TSESFace<T>::TYPE_CONTACT;
			face->rsvertex_ = reduced_surface_->vertices_[i];
			face->rsedge_ = NULL;
			face->rsface_ = NULL;
			face->index_ = i;
			contact_faces_.push_back(face);
		}
		for (i = 0; i < number_of_toric_faces_; i++)
		{
			TSESFace<T>* face = new TSESFace<T>;
			face->type_ = TSESFace<T>::TYPE_TORIC;
			face->rsvertex_ = NULL;
			face->rsedge_ = reduced_surface_->edges_[i];
			face->rsface_ = NULL;
			face->index_ = i;
			toric_faces_.push_back(face);
		}
		for (i = 0; i < number_of_spheric_faces_; i++)
		{
			TSESFace<T>* face = new TSESFace<T>;
			face->type_ = TSESFace<T>::TYPE_SPHERIC;
			face->rsvertex_ = NULL;
			face->rsedge_ = NULL;
			face->rsface_ = reduced_surface_->faces_[i];
			face->index_ = i;
			spheric_faces_.push_back(face);
		}
	}


	template <typename T>
	void TSolventExcludedSurface<T>::clear()
	{
		Position i;
		for (i = 0; i < number_of_vertices_; i++)
		{
			delete vertices_[i];
		}
		for (i = 0; i < number_of_edges_; i++)
		{
			delete edges_[i];
		}
		for (i = 0; i < number_of_contact_faces_; i++)
		{
			delete contact_faces_[i];
		}
		for (i = 0; i < number_of_toric_faces_; i++)
		{
			delete toric_faces_[i];
		}
		for (i = 0; i < number_of_spheric_faces_; i++)
		{
			delete spheric_faces_[i];
		}
		vertices_.clear();
		edges_.clear();
		singular_edges_.clear();
		contact_faces_.clear();
		toric_faces_.clear();
		spheric_faces_.clear();
		number_of_vertices_ = 0;
		number_of_edges_ = 0;
		number_of_singular_edges_ = 0;
		number_of_contact_faces_ = 0;
		number_of_toric_faces_ = 0;
		number_of_spheric_faces_ = 0;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::clean(const T& density)
	{
		TSESFace<T>* face;
		bool done = false;
		while (!done)
		{
			done = true;
			for (Position i = 0; i < toric_faces_.size(); i++)
			{
						#ifdef print_clean_info
						std::cout << "überprüfe toric face " << i << "\n";
						#endif
				face = toric_faces_[i];
				if (face != NULL)
				{
					if (!face->isFree())
					{
						if (face->type_ == TSESFace<T>::TYPE_TORIC_SINGULAR)
						{
							face->normalize(true);
							typename std::list<TSESEdge<T>*>::iterator e = face->edge_.begin();
							TSESEdge<T>* edge0 = *e;
							e++;
							e++;
							e++;
							TSESEdge<T>* edge3 = *e;
							typename std::list<TSESVertex<T>*>::iterator v = face->vertex_.begin();
							TSESVertex<T>* v0 = *v;
							v++;
							v++;
							TSESVertex<T>* v2 = *v;
							v++;
							TSESVertex<T>* v3 = *v;
							v++;
							v++;
							TSESVertex<T>* v5 = *v;
							if (v0 == v2)
							{
								if (edge0->rsedge_->phi_.value < Constants::PI)
								{
									deleteSmallSingularToricFace(face);
									done = false;
								}
								else
								{
									edge0->rsedge_->phi_.value = 2*Constants::PI;
								}
							}
							else
							{
								if (v3 == v5)
								{
									if (edge3->rsedge_->phi_.value < Constants::PI)
									{
										deleteSmallSingularToricFace(face);
										done = false;
									}
									else
									{
										edge3->rsedge_->phi_.value = 2*Constants::PI;
									}
								}
								else
								{
									T exact_number_of_segments(face->rsedge_->phi_.value*
																						 edge3->circle_.radius*
																						 sqrt(density));
												#ifdef print_clean_info
												std::cout << "   " << exact_number_of_segments << "\n";
												#endif
									if (Maths::isLess(exact_number_of_segments,0.1))
									{
										deleteSmallSingularToricFace(face);
										done = false;
									}
								}
							}
						}
						else
						{
							face->normalize(false);
							typename std::list<TSESEdge<T>*>::iterator e = face->edge_.begin();
							e++;
							TSESEdge<T>* edge1 = *e;
							e++;
							e++;
							TSESEdge<T>* edge3 = *e;
							typename std::list<TSESVertex<T>*>::iterator v = face->vertex_.begin();
							TSESVertex<T>* v0 = *v;
							v++;
							TSESVertex<T>* v1 = *v;
							v++;
							TSESVertex<T>* v2 = *v;
							v++;
							TSESVertex<T>* v3 = *v;
							if (v0 == v3)
							{
								if (edge3->rsedge_->phi_.value < Constants::PI)
								{
									deleteSmallToricFace(face);
									done = false;
								}
								else
								{
									edge3->rsedge_->phi_.value = 2*Constants::PI;
								}
							}
							else
							{
								if (v1 == v2)
								{
									if (edge1->rsedge_->phi_.value < Constants::PI)
									{
										deleteSmallToricFace(face);
										done = false;
									}
									else
									{
										edge1->rsedge_->phi_.value = 2*Constants::PI;
									}
								}
								else
								{
									T exact_number_of_segments(face->rsedge_->phi_.value*
																						 edge3->circle_.radius*
																						 sqrt(density));
									if (Maths::isLess(exact_number_of_segments,0.1))
									{
										deleteSmallToricFace(face);
										done = false;
									}
								}
							}
						}
					}
				}
			}
		}
		if (number_of_vertices_ > 0)
		{
			while (vertices_[number_of_vertices_-1] == NULL)
			{
				vertices_.pop_back();
				number_of_vertices_--;
			}
			for (Position i = 0; i < number_of_vertices_; i++)
			{
				if (vertices_[i] == NULL)
				{
					vertices_[i] = vertices_[number_of_vertices_-1];
					vertices_[i]->index_ = i;
					vertices_.pop_back();
					number_of_vertices_--;
					while (vertices_[number_of_vertices_-1] == NULL)
					{
						vertices_.pop_back();
						number_of_vertices_--;
					}
				}
			}
		}
		if (number_of_edges_ > 0)
		{
			while (edges_[number_of_edges_-1] == NULL)
			{
				edges_.pop_back();
				number_of_edges_--;
			}
			for (Position i = 0; i < number_of_edges_; i++)
			{
				if (edges_[i] == NULL)
				{
					edges_[i] = edges_[number_of_edges_-1];
					edges_[i]->index_ = i;
					edges_.pop_back();
					number_of_edges_--;
					while (edges_[number_of_edges_-1] == NULL)
					{
						edges_.pop_back();
						number_of_edges_--;
					}
				}
			}
		}
		if (number_of_toric_faces_ > 0)
		{
			while (toric_faces_[number_of_toric_faces_-1] == NULL)
			{
				toric_faces_.pop_back();
				number_of_toric_faces_--;
			}
			for (Position i = 0; i < number_of_toric_faces_; i++)
			{
				if (toric_faces_[i] == NULL)
				{
					toric_faces_[i] = toric_faces_[number_of_toric_faces_-1];
					toric_faces_[i]->index_ = i;
					toric_faces_.pop_back();
					number_of_toric_faces_--;
					while (toric_faces_[number_of_toric_faces_-1] == NULL)
					{
						toric_faces_.pop_back();
						number_of_toric_faces_--;
					}
				}
			}
		}
		if (number_of_spheric_faces_ > 0)
		{
			while (spheric_faces_[number_of_spheric_faces_-1] == NULL)
			{
				spheric_faces_.pop_back();
				number_of_spheric_faces_--;
			}
			for (Position i = 0; i < number_of_spheric_faces_; i++)
			{
				if (spheric_faces_[i] == NULL)
				{
					spheric_faces_[i] = spheric_faces_[number_of_spheric_faces_-1];
					spheric_faces_[i]->index_ = i;
					spheric_faces_.pop_back();
					number_of_spheric_faces_--;
					while (spheric_faces_[number_of_spheric_faces_-1] == NULL)
					{
						spheric_faces_.pop_back();
						number_of_spheric_faces_--;
					}
				}
			}
		}
		if (number_of_contact_faces_ > 0)
		{
			while (contact_faces_[number_of_contact_faces_-1] == NULL)
			{
				contact_faces_.pop_back();
				number_of_contact_faces_--;
			}
			for (Position i = 0; i < number_of_contact_faces_; i++)
			{
				if (contact_faces_[i] == NULL)
				{
					contact_faces_[i] = contact_faces_[number_of_contact_faces_-1];
					contact_faces_[i]->index_ = i;
					contact_faces_.pop_back();
					number_of_contact_faces_--;
					while (contact_faces_[number_of_contact_faces_-1] == NULL)
					{
						contact_faces_.pop_back();
						number_of_contact_faces_--;
					}
				}
			}
		}
	}


	template <typename T>
	void TSolventExcludedSurface<T>::compute()
		throw(Exception::GeneralException)
	{
		get();
		treatSingularities();
	}


	template <typename T>
	void TSolventExcludedSurface<T>::splitSphericFaces()
		throw()
	{
		for (Position i = 0; i < number_of_spheric_faces_; i++)
		{
			splitSphericFace(i);
		}
	}


	template <typename T>
	void TSolventExcludedSurface<T>::splitSphericFace(Position i)
		throw()
	{
		TSESFace<T>* face = spheric_faces_[i];
		typename std::list<TSESEdge<T>*>::iterator e = face->edge_.begin();
		while (e != face->edge_.end())
		{
			if ((*e)->vertex_[0] == NULL)
			{
				return;
			}
			e++;
		}
		std::list<TSESEdge<T>*> contour;
		std::list<TSESVertex<T>*> vertices;
		e = face->edge_.begin();
		TSESVertex<T>* start = (*e)->vertex_[0];
		TSESVertex<T>* next = (*e)->vertex_[1];
		TSESEdge<T>* current = *e;
		contour.push_back(current);
		vertices.push_back(next);
		while (next != start)
		{
			for (e = face->edge_.begin(); e != face->edge_.end(); e++)
			{
				if (*e != current)
				{
					if ((*e)->vertex_[0] == next)
					{
						contour.push_back(*e);
						next = (*e)->vertex_[1];
						vertices.push_back(next);
						current = *e;
					}
					else
					{
						if ((*e)->vertex_[1] == next)
						{
							contour.push_back(*e);
							next = (*e)->vertex_[0];
							vertices.push_back(next);
							current = *e;
						}
					}
				}
			}
		}
		if (contour.size() != face->edge_.size())
		{
			TSESFace<T>* new_face = new TSESFace<T>(*face,true);
			for (e = contour.begin(); e != contour.end(); e++)
			{
				new_face->edge_.remove(*e);
			}
			typename std::list<TSESVertex<T>*>::iterator v;
			for (v = vertices.begin(); v != vertices.end(); v++)
			{
				new_face->vertex_.remove(*v);
			}
			new_face->index_ = number_of_spheric_faces_;
			spheric_faces_.push_back(new_face);
			number_of_spheric_faces_++;
			face->edge_ = contour;
			face->vertex_ = vertices;
		}
	}


	template <typename T>
	void TSolventExcludedSurface<T>::get()
		throw()
	{
		for (Position i = 0; i < spheric_faces_.size(); i++)
		{
			createSphericFace(i);
		}
		for (Position i = 0; i < toric_faces_.size(); i++)
		{
			createToricFace(i);
		}
	}


	template <typename T>
	void TSolventExcludedSurface<T>::createSphericFace(Position j)
		throw()
	{
		TSESFace<T>* face = spheric_faces_[j];
		TRSFace<T>* rsface = face->rsface_;
		T probe_radius = reduced_surface_->probe_radius_;
		TSphere3<T> probe(rsface->center_,probe_radius);
		// create three vertices and push them to their faces and in the list of vertices
		pushVertex(face,probe,rsface->vertex_[0]);
		pushVertex(face,probe,rsface->vertex_[1]);
		pushVertex(face,probe,rsface->vertex_[2]);
		// create three concace edges and push them to their faces and in the list of edges
		pushConcaveEdge(face,0,1,probe_radius);
		pushConcaveEdge(face,1,2,probe_radius);
		pushConcaveEdge(face,2,0,probe_radius);
	}


	template <typename T>
	void TSolventExcludedSurface<T>::pushVertex
		(TSESFace<T>*				face,
		 const TSphere3<T>& probe,
		 TRSVertex<T>*			rsvertex)
		throw()
	{
		// Create a new vertex on the right position ...
		TSESVertex<T>* vertex(createVertex(probe.p,rsvertex->atom_));
		// ... and push it to the face's vertices.
		face->vertex_.push_back(vertex);
		vertex->faces_.insert(face);
		// Get the RSEdges of the corresponding RSVertex ...
		TRSEdge<T>* tf1;
		TRSEdge<T>* tf2;
		face->rsface_->getEdges(rsvertex,tf1,tf2);
		// ... and push the vertex to these toric faces.
		toric_faces_[tf1->index_]->vertex_.push_back(vertex);
		vertex->faces_.insert(toric_faces_[tf1->index_]);
		toric_faces_[tf2->index_]->vertex_.push_back(vertex);
		vertex->faces_.insert(toric_faces_[tf2->index_]);
		// Push the vertex to the contact face of the corresponding RSVertex ...
		contact_faces_[rsvertex->index_]->vertex_.push_back(vertex);
		vertex->faces_.insert(contact_faces_[rsvertex->index_]);
		// ... and to the verices of the SES.
		vertices_.push_back(vertex);
		vertex_grid_.insert(vertex->point_,vertex->index_);
		number_of_vertices_++;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::pushConcaveEdge
		(TSESFace<T>* face,
		 Position			p1,
		 Position			p2,
		 const T&			radius_of_probe)
		throw()
	{
		TRSFace<T>* rsface = face->rsface_;
		TRSEdge<T>* rsedge;
		// get the corresponding RSEdge
		rsface->getEdge(rsface->getVertex(p1),rsface->getVertex(p2),rsedge);
		Index index = rsedge->index_;
		// create a new SESEdge
		TSESEdge<T>* edge(createConcaveEdge(face,p1,p2,index,radius_of_probe));
		// and push it to all it's places
		face->edge_.push_back(edge);
		toric_faces_[index]->edge_.push_back(edge);
		edge->vertex_[0]->edges_.insert(edge);
		edge->vertex_[1]->edges_.insert(edge);
		edges_.push_back(edge);
		number_of_edges_++;
	}


	template <typename T>
	TSESVertex<T>* TSolventExcludedSurface<T>::createVertex
		(const TVector3<T>& probe_center,
		 Index							index)
		throw()
	{
		TSESVertex<T>* vertex = new TSESVertex<T>;
		TSphere3<T> atom(reduced_surface_->atom_[index]);
		// get the position of the new vertex
		getPoint(atom.p,probe_center,atom.radius,vertex->point_);
		vertex->normal_.set(probe_center-vertex->point_);
		vertex->normal_.normalize();
		vertex->atom_ = index;
		vertex->index_ = number_of_vertices_;
		return vertex;
	}


	template <typename T>
	TSESEdge<T>* TSolventExcludedSurface<T>::createConcaveEdge
		(TSESFace<T>* spheric_face,
		 Position			p1,
		 Position			p2,
		 Index				index,
		 const T&			radius_of_probe)
		throw()
	{
		TSESEdge<T>* edge = new TSESEdge<T>;
		// set the vertices of the edge
		typename std::list<TSESVertex<T>*>::iterator v = spheric_face->vertex_.begin();
		for (Position i = 0; i < p1; i++)
		{
			v++;
		}
		edge->vertex_[0] = *v;
		v = spheric_face->vertex_.begin();
		for (Position i = 0; i < p2; i++)
		{
			v++;
		}
		edge->vertex_[1] = *v;
		// set the faces of the edge
		edge->face_[0] = spheric_face;
		edge->face_[1] = toric_faces_[index];
		//
		edge->rsedge_ = NULL;
		edge->type_ = TSESEdge<T>::TYPE_CONCAVE;
		edge->index_ = number_of_edges_;
		// compute the circle on which the edge lies
		TRSFace<T>* rsface = spheric_face->rsface_;
		TVector3<T> normal = (edge->vertex_[0]->point_-rsface->center_)%
												 (edge->vertex_[1]->point_-rsface->center_);
		edge->circle_.set(rsface->center_,normal,radius_of_probe);
		return edge;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::createToricFace(Position i)
		throw()
	{
		TSESFace<T>* face = toric_faces_[i];
		if (face->isFree())
		{
			createFreeToricFace(i);
		}
		else
		{
			TSESEdge<T>* edge1;
			TRSVertex<T>* rsvertex = face->rsedge_->vertex_[0];
			Index index = rsvertex->index_;
			edge1 = createConvexEdge(face,rsvertex);
				face->edge_.push_back(edge1);
				contact_faces_[index]->edge_.push_back(edge1);
				edge1->vertex_[0]->edges_.insert(edge1);
				edge1->vertex_[1]->edges_.insert(edge1);
				edges_.push_back(edge1);
				number_of_edges_++;
			rsvertex = face->rsedge_->vertex_[1];
			index = rsvertex->index_;
			TSESEdge<T>* edge2;
			edge2 = createConvexEdge(face,rsvertex);
				face->edge_.push_back(edge2);
				contact_faces_[index]->edge_.push_back(edge2);
				edge2->vertex_[0]->edges_.insert(edge2);
				edge2->vertex_[1]->edges_.insert(edge2);
				edges_.push_back(edge2);
				number_of_edges_++;
			if (Maths::isEqual(face->rsedge_->phi_.value,Constants::PI))
			{
//cout << "Winkel von toroidaler Fläche Nummer " << face->index_ << " ist Pi " << face->rsedge_->phi_ << "\n";
//cout << "konvexe Kanten: " << *edge1 << "\n                " << edge2 << "\n";
				TRSFace<T>* rsface1 = face->rsedge_->face_[0];
				TRSFace<T>* rsface2 = face->rsedge_->face_[1];
				TRSVertex<T>* rsvertex1 = face->rsedge_->vertex_[0];
				TRSVertex<T>* rsvertex2 = face->rsedge_->vertex_[1];
				TRSVertex<T>* rsvertex3 = rsface1->thirdVertex(rsvertex1,rsvertex2);
				TRSVertex<T>* rsvertex4 = rsface2->thirdVertex(rsvertex1,rsvertex2);
				TVector3<T> point1(reduced_surface_->atom_[rsvertex1->atom_].p);
				TVector3<T> point2(reduced_surface_->atom_[rsvertex2->atom_].p);
				TVector3<T> point3(reduced_surface_->atom_[rsvertex3->atom_].p);
				TVector3<T> point4(reduced_surface_->atom_[rsvertex4->atom_].p);

				TVector3<T> middle(edge1->circle_.n%(edge1->vertex_[0]->point_-edge1->circle_.p));
				middle.normalize();
				middle *= edge1->circle_.radius;
				middle += edge1->circle_.p;
				TPlane3<T> plane(point1,point2,rsface1->center_);
				T test = plane.n*(point3-plane.p);
				if (test*(plane.n*(middle-plane.p)) > 0)
				{
					edge1->revert();
//cout << "erste umgedreht:  " << *edge1 << "\n";
				}
				middle = edge2->circle_.n%(edge2->vertex_[0]->point_-edge2->circle_.p);
				middle.normalize();
				middle *= edge2->circle_.radius;
				middle += edge2->circle_.p;
				if (test*(plane.n*(middle-plane.p)) > 0)
				{
					edge2->revert();
//cout << "zweite umgedreht: " << *edge2 << "\n";
				}
			}
			if (face->rsedge_->singular_)
			{
				treatSingularToricFace(i);
			}
		}
	}


	template <typename T>
	TSESEdge<T>* TSolventExcludedSurface<T>::createConvexEdge
		(TSESFace<T>*		toric_face,
		 TRSVertex<T>*	rsvertex)
		throw()
	{
		TSESEdge<T>* edge = new TSESEdge<T>;
		Index atom = rsvertex->atom_;
		Index index = rsvertex->index_;
		// find the first vertex of the toric face which lies on the surface of the rsvertex
		typename std::list<TSESVertex<T>*>::iterator v = toric_face->vertex_.begin();
		while ((*v)->atom_ != atom)
		{
			v++;
		}
		edge->vertex_[0] = *v;
		// find the second vertex of the toric face which lies on the surface of the rsvertex
		v++;
		while ((*v)->atom_ != atom)
		{
			v++;
		}
		edge->vertex_[1] = *v;
		// set the faces of the edge
		edge->face_[0] = toric_face;
		edge->face_[1] = contact_faces_[index];
		// set the rsedge, type and index of the edge
		edge->rsedge_ = toric_face->rsedge_;
		edge->type_ = TSESEdge<T>::TYPE_CONVEX;
		edge->index_ = number_of_edges_;
		// compute the circle on which the edge lies
		TCircle3<T> circle0(edge->rsedge_->circle0_);
		TCircle3<T> circle1(edge->rsedge_->circle1_);
		if (edge->rsedge_->vertex_[0]->index_ == index)
		{
			edge->circle_.set(circle0.p,circle0.p-circle1.p,circle0.radius);
		}
		else
		{
			edge->circle_.set(circle1.p,circle1.p-circle0.p,circle1.radius);
		}
		TVector3<T> v1_(edge->vertex_[0]->point_-edge->circle_.p);
		TVector3<double> v1((double)v1_.x,(double)v1_.y,(double)v1_.z);
		TVector3<T> v2_(edge->vertex_[1]->point_-edge->circle_.p);
		TVector3<double> v2((double)v2_.x,(double)v2_.y,(double)v2_.z);
		TVector3<T> n_(edge->circle_.n);
		TVector3<double> n((double)n_.x,(double)n_.y,(double)n_.z);
		TAngle<T> test_phi((double)getOrientedAngle(v1, v2, n));
		TAngle<T> tf(getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
															edge->vertex_[1]->point_-edge->circle_.p,
															edge->circle_.n));
		if ((test_phi.value-(double)Constants::PI)*((double)edge->rsedge_->phi_.value-(double)Constants::PI) < 0)
		{
			if (Maths::abs(test_phi.value-(double)edge->rsedge_->phi_.value) <
					Maths::abs((double)Constants::PI-(double)edge->rsedge_->phi_.value))
			{
//				cout << "    different results!\n";
			}
			edge->revert();
		}
		else
		{
			if (Maths::abs(test_phi.value-(double)edge->rsedge_->phi_.value) >
					Maths::abs((double)Constants::PI-(double)edge->rsedge_->phi_.value))
			{
//				cout << "    different results!\n";
			}
		}
/*if ((test_phi.value-(double)Constants::PI)*((double)edge->rsedge_->phi_.value-(double)Constants::PI) < 0)
{
	if ((tf.value-Constants::PI)*(edge->rsedge_->phi_.value-Constants::PI) >= 0)
	{
//		cout << "    different result with double in tf" << toric_face->index_ << "!\n";
	}
}
else
{
	if ((tf.value-Constants::PI)*(edge->rsedge_->phi_.value-Constants::PI) < 0)
	{
//		cout << "    different result with double in tf" << toric_face->index_ << "!\n";
	}
}*/
		return edge;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::treatSingularToricFace(Position i)
		throw()
	{
		TSESFace<T>* face = toric_faces_[i];
		face->normalize(false);
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
		v++;
		TSESVertex<T>* p2 = *v;
		v++;
		TSESVertex<T>* p3 = *v;
		//Position j = 0;
		Position ip1 = ((p1->atom_ == face->rsedge_->vertex_[0]->atom_) ? 0 : 1);
		Position ip3 = 1-ip1;
		// create the new points
		TSESVertex<T>* new_point1;
		TSESVertex<T>* new_point3;
		Index test = vertexExists(face->rsedge_->getIntersectionPoint(ip1));
		if (test == -1)
		{
		 	new_point1 = new TSESVertex<T>(face->rsedge_->getIntersectionPoint(ip1),
																		 TVector3<T>::getZero(),
																		 face->rsedge_->getVertex(ip1)->atom_,
																		 number_of_vertices_);
			vertices_.push_back(new_point1);
			vertex_grid_.insert(new_point1->point_,new_point1->index_);
			number_of_vertices_++;
		}
		else
		{
			new_point1 = vertices_[test];
		}
		test = vertexExists(face->rsedge_->getIntersectionPoint(ip3));
		if (test == -1)
		{
			new_point3 = new TSESVertex<T>(face->rsedge_->getIntersectionPoint(ip3),
																		 TVector3<T>::getZero(),
																		 face->rsedge_->getVertex(ip3)->atom_,
																		 number_of_vertices_);
			vertices_.push_back(new_point3);
			vertex_grid_.insert(new_point3->point_,new_point3->index_);
			number_of_vertices_++;
		}
		else
		{
			new_point3 = vertices_[test];
		}
		// create the new edges
		TSESEdge<T>* new_edge0 = new TSESEdge<T>(*edge0,true);
		TSESEdge<T>* new_edge1 = new TSESEdge<T>(*edge0,true);
			if (p0 == edge0->vertex_[0])
			{
				new_edge0->vertex_[0] = p0;
				new_edge0->vertex_[1] = new_point3;
				new_edge1->vertex_[0] = new_point1;
				new_edge1->vertex_[1] = p1;
			}
			else
			{
				new_edge1->vertex_[0] = p1;
				new_edge1->vertex_[1] = new_point1;
				new_edge0->vertex_[0] = new_point3;
				new_edge0->vertex_[1] = p0;
			}
			new_edge0->index_ = number_of_edges_;
			new_edge0->vertex_[0]->edges_.insert(new_edge0);
			new_edge0->vertex_[1]->edges_.insert(new_edge0);
			edges_.push_back(new_edge0);
			number_of_edges_++;
			new_edge1->index_ = number_of_edges_;
			new_edge1->vertex_[0]->edges_.insert(new_edge1);
			new_edge1->vertex_[1]->edges_.insert(new_edge1);
			edges_.push_back(new_edge1);
			number_of_edges_++;
		TSESEdge<T>* new_edge2 = new TSESEdge<T>(*edge2,true);
		TSESEdge<T>* new_edge3 = new TSESEdge<T>(*edge2,true);
			if (p2 == edge2->vertex_[0])
			{
				new_edge2->vertex_[0] = p2;
				new_edge2->vertex_[1] = new_point1;
				new_edge3->vertex_[0] = new_point3;
				new_edge3->vertex_[1] = p3;
			}
			else
			{
				new_edge3->vertex_[0] = p3;
				new_edge3->vertex_[1] = new_point3;
				new_edge2->vertex_[0] = new_point1;
				new_edge2->vertex_[1] = p2;
			}
			new_edge2->index_ = number_of_edges_;
			new_edge2->vertex_[0]->edges_.insert(new_edge2);
			new_edge2->vertex_[1]->edges_.insert(new_edge2);
			edges_.push_back(new_edge2);
			number_of_edges_++;
			new_edge3->index_ = number_of_edges_;
			new_edge3->vertex_[0]->edges_.insert(new_edge3);
			new_edge3->vertex_[1]->edges_.insert(new_edge3);
			edges_.push_back(new_edge3);
			number_of_edges_++;
		p0->edges_.erase(edge0);
		p1->edges_.erase(edge0);
		p2->edges_.erase(edge2);
		p3->edges_.erase(edge2);
		p0->faces_.erase(face);
		p1->faces_.erase(face);
		p2->faces_.erase(face);
		p3->faces_.erase(face);
		//
		TSESFace<T>* neighbour0 = edge0->otherFace(face);
		TSESFace<T>* neighbour2 = edge2->otherFace(face);
		T radius_of_probe = reduced_surface_->probe_radius_;
		TSphere3<T> probe1(neighbour0->rsface_->center_,radius_of_probe);
		TSphere3<T> probe2(neighbour2->rsface_->center_,radius_of_probe);
		TCircle3<T> intersection_circle;
		GetIntersection(probe1,probe2,intersection_circle);
		new_point1->normal_ = intersection_circle.p-new_point1->point_;
		new_point1->normal_.normalize();
		new_point3->normal_ = intersection_circle.p-new_point3->point_;
		new_point3->normal_.normalize();
		if (face->rsedge_->phi_.value > Constants::PI)
		{
			TAngle<T> phi(getOrientedAngle(new_point1->point_-intersection_circle.p,
																		 new_point3->point_-intersection_circle.p,
																		 intersection_circle.n));
			if (phi.value < Constants::PI)
			{
				intersection_circle.n.negate();
			}
		}
		else
		{
			TAngle<T> phi(getOrientedAngle(new_point1->point_-intersection_circle.p,
																		 new_point3->point_-intersection_circle.p,
																		 intersection_circle.n));
			if (phi.value > Constants::PI)
			{
				intersection_circle.n.negate();
			}
		}
		TSESEdge<T>* new_edge4 = new TSESEdge<T>(new_point1,new_point3,neighbour0,neighbour2,
																						 intersection_circle,face->rsedge_,
																						 TSESEdge<T>::TYPE_SINGULAR,number_of_edges_);
		new_point1->edges_.insert(new_edge4);
		new_point3->edges_.insert(new_edge4);
		new_point1->faces_.insert(neighbour0);
		new_point1->faces_.insert(neighbour2);
		new_point3->faces_.insert(neighbour0);
		new_point3->faces_.insert(neighbour2);
		edges_.push_back(new_edge4);
		singular_edges_.push_back(new_edge4);
		number_of_edges_++;
		number_of_singular_edges_++;
		// update the neighbour faces
		neighbour0->edge_.remove(edge0);
		neighbour0->edge_.push_back(new_edge0);
		neighbour0->edge_.push_back(new_edge1);
		neighbour0->edge_.push_back(new_edge4);
		neighbour0->vertex_.push_back(new_point1);
		neighbour0->vertex_.push_back(new_point3);
		neighbour2->edge_.remove(edge2);
		neighbour2->edge_.push_back(new_edge2);
		neighbour2->edge_.push_back(new_edge3);
		neighbour2->edge_.push_back(new_edge4);
		neighbour2->vertex_.push_back(new_point1);
		neighbour2->vertex_.push_back(new_point3);
		// update the toric face
		TSESFace<T>* new_face = new TSESFace<T>;
		new_face->type_ = TSESFace<T>::TYPE_TORIC_SINGULAR;
		new_face->rsedge_ = face->rsedge_;
		new_face->index_ = face->index_;
		new_face->vertex_.push_back(p0);
		new_face->vertex_.push_back(p1);
		new_face->vertex_.push_back(p2);
		new_face->vertex_.push_back(p3);
		new_face->vertex_.push_back(new_point1);
		new_face->vertex_.push_back(new_point3);
		new_face->edge_.push_back(edge1);
		new_face->edge_.push_back(edge3);
		new_face->edge_.push_back(new_edge0);
		new_face->edge_.push_back(new_edge1);
		new_face->edge_.push_back(new_edge2);
		new_face->edge_.push_back(new_edge3);
		new_edge0->face_[0] = new_face;
		new_edge0->face_[1] = neighbour0;
		new_edge1->face_[0] = new_face;
		new_edge1->face_[1] = neighbour0;
		new_edge2->face_[0] = new_face;
		new_edge2->face_[1] = neighbour2;
		new_edge3->face_[0] = new_face;
		new_edge3->face_[1] = neighbour2;
		edge1->face_[0] = edge1->otherFace(face);
		edge1->face_[1] = new_face;
		edge3->face_[0] = edge3->otherFace(face);
		edge3->face_[1] = new_face;
		p0->faces_.insert(new_face);
		p1->faces_.insert(new_face);
		p2->faces_.insert(new_face);
		p3->faces_.insert(new_face);
		new_point1->faces_.insert(new_face);
		new_point3->faces_.insert(new_face);
		// delete the intersecting edges
		edges_[edge0->index_] = NULL;
		delete edge0;
		edges_[edge2->index_] = NULL;
		delete edge2;
		// replace the old edge by the new
		toric_faces_[face->index_] = new_face;
		#ifdef print_ses
				Molecule* molecule = new Molecule;
				partitionSingularEdge(new_edge4,10,molecule);
				System* system = new System;
				system->insert(*molecule);
				HINFile hinfile("DATA/SES/singularEdge"+IndexToString(new_edge4->index_,0)+".hin", std::ios::out);
				hinfile << *system;
				hinfile.close();
				delete system;
		#endif

		delete face;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::createFreeToricFace(Position i)
		throw()
	{
		TSESFace<T>* face(toric_faces_[i]);
		TCircle3<T> circle1(face->rsedge_->circle0_);
		TCircle3<T> circle2(face->rsedge_->circle1_);
		Index index1(face->rsedge_->vertex_[0]->index_);
		Index index2(face->rsedge_->vertex_[1]->index_);
		TSESEdge<T>* edge = new TSESEdge<T>;
			edge->type_ = TSESEdge<T>::TYPE_CONVEX;
			edge->vertex_[0] = NULL;
			edge->vertex_[1] = NULL;
			edge->rsedge_ = face->rsedge_;
			edge->face_[0] = face;
			edge->face_[1] = contact_faces_[index1];
			edge->circle_.set(circle1.p,circle1.p-circle2.p,circle1.radius);
			edge->index_ = number_of_edges_;
			face->edge_.push_back(edge);
			contact_faces_[index1]->edge_.push_back(edge);
			edges_.push_back(edge);
			number_of_edges_++;
		edge = new TSESEdge<T>;
			edge->type_ = TSESEdge<T>::TYPE_CONVEX;
			edge->vertex_[0] = NULL;
			edge->vertex_[1] = NULL;
			edge->rsedge_ = face->rsedge_;
			edge->face_[0] = face;
			edge->face_[1] = contact_faces_[index2];
			edge->circle_.set(circle2.p,circle2.p-circle1.p,circle2.radius);
			edge->index_ = number_of_edges_;
			face->edge_.push_back(edge);
			contact_faces_[index2]->edge_.push_back(edge);
			edges_.push_back(edge);
			number_of_edges_++;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::getPoint
		(const TVector3<T>& p1,
		 const TVector3<T>& p2,
		 const T&						dist,
		 TVector3<T>&				result)
		throw()
	{
		result.set(p2-p1);
		result.normalize();
		result *= dist;
		result += p1;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::deleteSmallToricFace(TSESFace<T>* face)
		throw()
	{
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
		v++;
		TSESVertex<T>* p2 = *v;
		v++;
		TSESVertex<T>* p3 = *v;
		if (p0 != p3)
		{ 	
			p0->join(*p3);
			p3->substitute(p0);
		}
		if (p1 != p2)
		{
			p1->join(*p2);
			p2->substitute(p1);
		}
		p0->edges_.erase(edge2);
		p0->edges_.erase(edge3);
		p1->edges_.erase(edge2);
		p1->edges_.erase(edge1);
		p0->faces_.erase(face);
		p1->faces_.erase(face);
		TSESFace<T>* neighbour1;
		TSESFace<T>* neighbour2;
		TSESFace<T>* neighbour3;
		neighbour1 = edge1->otherFace(face);
		neighbour2 = edge2->otherFace(face);
		neighbour3 = edge3->otherFace(face);
		edge0->substituteFace(face,neighbour2);
		neighbour2->substituteEdge(edge2,edge0);
		e = neighbour1->edge_.begin();
		while (*e != edge1)
		{
			e++;
		}
		neighbour1->edge_.erase(e);
		v = neighbour1->vertex_.begin();
		while (*v != p1)
		{
			v++;
		}
		neighbour1->vertex_.erase(v);
		e = neighbour3->edge_.begin();
		while (*e != edge3)
		{
			e++;
		}
		neighbour3->edge_.erase(e);
		v = neighbour3->vertex_.begin();
		while (*v != p0)
		{
			v++;
		}
		neighbour3->vertex_.erase(v);
		if (p2 != p1)
		{
			vertices_[p2->index_] = NULL;
			//vertex_grid_.remove(p2->point_,p2->index_);
			delete p2;
		}
		else
		{
			p1->faces_.erase(neighbour1);
			contact_faces_[neighbour1->index_] = NULL;
			delete neighbour1;
		}
		if (p3 != p0)
		{
			vertices_[p3->index_] = NULL;
			//vertex_grid_.remove(p3->point_,p3->index_);
			delete p3;
		}
		else
		{
			p0->faces_.erase(neighbour3);
			contact_faces_[neighbour3->index_] = NULL;
			delete neighbour3;
		}
		edges_[edge1->index_] = NULL;
		delete edge1;
		edges_[edge2->index_] = NULL;
		delete edge2;
		edges_[edge3->index_] = NULL;
		delete edge3;
		toric_faces_[face->index_] = NULL;
		delete face;
		edge0->type_ = TSESEdge<T>::TYPE_SINGULAR;
		TAngle<T> phi(getOrientedAngle(edge0->vertex_[0]->point_-edge0->circle_.p,
																	 edge0->vertex_[1]->point_-edge0->circle_.p,
																	 edge0->circle_.n));
		if (phi.value > Constants::PI)
		{
			edge0->circle_.n.negate();
		}
		singular_edges_.push_back(edge0);
	}


	template <typename T>
	void TSolventExcludedSurface<T>::deleteSmallSingularToricFace(TSESFace<T>* face)
		throw()
	{
		typename std::list<TSESEdge<T>*>::iterator e = face->edge_.begin();
		TSESEdge<T>* edge0 = *e;
		e++;
		TSESEdge<T>* edge1 = *e;
		e++;
		TSESEdge<T>* edge2 = *e;
		e++;
		TSESEdge<T>* edge3 = *e;
		e++;
		TSESEdge<T>* edge4 = *e;
		e++;
		TSESEdge<T>* edge5 = *e;
		typename std::list<TSESVertex<T>*>::iterator v = face->vertex_.begin();
		TSESVertex<T>* p0 = *v;
		v++;
		TSESVertex<T>* p1 = *v;
		v++;
		TSESVertex<T>* p2 = *v;
		v++;
		TSESVertex<T>* p3 = *v;
		v++;
		TSESVertex<T>* p4 = *v;
		v++;
		TSESVertex<T>* p5 = *v;
		TSESFace<T>* neighbour0;
		TSESFace<T>* neighbour1;
		TSESFace<T>* neighbour2;
		TSESFace<T>* neighbour3;
		TSESFace<T>* neighbour4;
		TSESFace<T>* neighbour5;
				#ifdef print_clean_info
				std::cout << "dist(p0,p2)^2 = " << p0->point_.getSquareDistance(p2->point_) << " <=> "
									<< 10*Constants::EPSILON << "\n";
				neighbour0 = edge0->otherFace(face);
				neighbour1 = edge1->otherFace(face);
				neighbour2 = edge2->otherFace(face);
				neighbour3 = edge3->otherFace(face);
				neighbour4 = edge4->otherFace(face);
				neighbour5 = edge5->otherFace(face);
				std::cout << "delete small face\n";
				std::cout << "toric face:  " << *face << "\n";
				std::cout << "neighbours:  " << *neighbour0 << "\n";
				std::cout << "             " << *neighbour1 << "\n";
				std::cout << "             " << *neighbour2 << "\n";
				std::cout << "             " << *neighbour3 << "\n";
				std::cout << "             " << *neighbour4 << "\n";
				std::cout << "             " << *neighbour5 << "\n";
				std::cout << "edges:       " << *edge0 << "\n";
				std::cout << "             " << *edge1 << "\n";
				std::cout << "             " << *edge2 << "\n";
				std::cout << "             " << *edge3 << "\n";
				std::cout << "             " << *edge4 << "\n";
				std::cout << "             " << *edge5 << "\n";
				std::cout << "points:      " << *p0 << "\n";
				std::cout << "             " << *p1 << "\n";
				std::cout << "             " << *p2 << "\n";
				std::cout << "             " << *p3 << "\n";
				std::cout << "             " << *p4 << "\n";
				std::cout << "             " << *p5 << "\n\n";
				std::cout << "other faces:\n";
				for (Position k = 0; k < neighbour1->edge_.size(); k++)
				{
					if (neighbour1->edge_[k] != NULL)
					{
						std::cout << "             " << *(neighbour1->edge_[k]->otherFace(neighbour1)) << "\n";
					}
				}
				for (Position k = 0; k < neighbour2->edge_.size(); k++)
				{
					if (neighbour2->edge_[k] != NULL)
					{
						std::cout << "             " << *(neighbour2->edge_[k]->otherFace(neighbour2)) << "\n";
					}
				}
				std::cout << "other edges:\n";
				typename HashSet<TSESEdge<T>*>::Iterator ee;
				for (ee = p0->edges_.begin(); ee != p0->edges_.end(); ee++)
				{
					std::cout << "             " << **ee << "\n";
				}
				for (ee = p2->edges_.begin(); ee != p2->edges_.end(); ee++)
				{
					std::cout << "             " << **ee << "\n";
				}
				for (ee = p3->edges_.begin(); ee != p3->edges_.end(); ee++)
				{
					std::cout << "             " << **ee << "\n";
				}
				for (ee = p5->edges_.begin(); ee != p5->edges_.end(); ee++)
				{
					std::cout << "             " << **ee << "\n";
				}
				#endif
		if (p0 != p2)
		{
			p0->join(*p2);
			p2->substitute(p0);
		}
		if (p3 != p5)
		{
			p3->join(*p5);
			p5->substitute(p3);
		}
		p0->edges_.erase(edge0);
		p0->edges_.erase(edge2);
		p1->edges_.erase(edge2);
		p3->edges_.erase(edge3);
		p3->edges_.erase(edge5);
		p4->edges_.erase(edge5);
		p0->faces_.erase(face);
		p1->faces_.erase(face);
		p3->faces_.erase(face);
		p4->faces_.erase(face);
		neighbour0 = edge0->otherFace(face);
		neighbour1 = edge1->otherFace(face);
		neighbour2 = edge2->otherFace(face);
		neighbour3 = edge3->otherFace(face);
		neighbour4 = edge4->otherFace(face);
		neighbour5 = edge5->otherFace(face);
		edge1->substituteFace(face,neighbour2);
		edge4->substituteFace(face,neighbour5);
		neighbour2->substituteEdge(edge2,edge1);
		neighbour5->substituteEdge(edge5,edge4);
		e = neighbour0->edge_.begin();
		while (*e != edge0)
		{
			e++;
		}
		neighbour0->edge_.erase(e);
		v = neighbour0->vertex_.begin();
		while (*v != p0)
		{
			v++;
		}
		neighbour0->vertex_.erase(v);
		e = neighbour3->edge_.begin();
		while (*e != edge3)
		{
			e++;
		}
		neighbour3->edge_.erase(e);
		v = neighbour3->vertex_.begin();
		while (*v != p3)
		{
			v++;
		}
		neighbour3->vertex_.erase(v);
				#ifdef print_clean_info
				bool n0 = true;
				bool n3 = true;
				#endif
		if (p2 != p0)
		{
			vertices_[p2->index_] = NULL;
			//vertex_grid_.remove(p2->point_,p2->index_);
			delete p2;
		}
		else
		{
			p0->faces_.erase(neighbour0);
			contact_faces_[neighbour0->index_] = NULL;
			delete neighbour0;
					#ifdef print_clean_info
					n0 = false;
					#endif
		}
		if (p3 != p5)
		{
			vertices_[p5->index_] = NULL;
			//vertex_grid_.remove(p5->point_,p5->index_);
			delete p5;
		}
		else
		{
			p3->faces_.erase(neighbour3);
			contact_faces_[neighbour3->index_] = NULL;
			delete neighbour3;
					#ifdef print_clean_info
					n3 = false;
					#endif
		}
		edges_[edge0->index_] = NULL;
		delete edge0;
		edges_[edge2->index_] = NULL;
		delete edge2;
		edges_[edge3->index_] = NULL;
		delete edge3;
		edges_[edge5->index_] = NULL;
		delete edge5;
		toric_faces_[face->index_] = NULL;
		delete face;
		edge1->type_ = TSESEdge<T>::TYPE_SINGULAR;
		TAngle<T> phi(getOrientedAngle(edge1->vertex_[0]->point_-edge1->circle_.p,
																	 edge1->vertex_[1]->point_-edge1->circle_.p,
																	 edge1->circle_.n));
		if (phi.value > Constants::PI)
		{
			edge1->circle_.n.negate();
		}
		edge4->type_ = TSESEdge<T>::TYPE_SINGULAR;
		phi = getOrientedAngle(edge4->vertex_[0]->point_-edge4->circle_.p,
													 edge4->vertex_[1]->point_-edge4->circle_.p,
													 edge4->circle_.n);
		if (phi.value > Constants::PI)
		{
			edge4->circle_.n.negate();
		}
		singular_edges_.push_back(edge1);
		singular_edges_.push_back(edge4);
								#ifdef print_clean_info
								std::cout << "done\n";
								std::cout << "toric face: ---\n";
								std::cout << "neighbours: ";
								if (n0)
								{
									std::cout << *neighbour0 << "\n";
								}
								else
								{
									std::cout << "-----\n";
								}
								std::cout << "            " << *neighbour1 << "\n";
								std::cout << "            " << *neighbour2 << "\n";
								if (n3)
								{
									std::cout << "            " << *neighbour3 << "\n";
								}
								else
								{
									std::cout << "            -----\n";
								}
								std::cout << "            " << *neighbour4 << "\n";
								std::cout << "            " << *neighbour5 << "\n";
								std::cout << "edges:      " << *edge1 << "\n";
								std::cout << "            " << *edge4 << "\n";
								std::cout << "points:     " << *p0 << "\n";
								std::cout << "            " << *p1 << "\n";
								std::cout << "            " << *p3 << "\n";
								std::cout << "            " << *p4 << "\n";
								std::cout << "other faces:\n";
								for (Position k = 0; k < neighbour1->edge_.size(); k++)
								{
									if (neighbour1->edge_[k] != NULL)
									{
										std::cout << "             " << *(neighbour1->edge_[k]->otherFace(neighbour1)) << "\n";
									}
								}
								for (Position k = 0; k < neighbour2->edge_.size(); k++)
								{
									if (neighbour2->edge_[k] != NULL)
									{
										std::cout << "             " << *(neighbour2->edge_[k]->otherFace(neighbour2)) << "\n";
									}
								}
								std::cout << "other edges:\n";
															typename HashSet<TSESEdge<T>*>::Iterator ee;
								for (ee = p0->edges_.begin(); ee != p0->edges_.end(); ee++)
								{
									std::cout << "             " << **ee << "\n";
								}
								std::cout << "             -------\n";
								for (ee = p3->edges_.begin(); ee != p3->edges_.end(); ee++)
								{
									std::cout << "             " << **ee << "\n";
								}
								std::cout << "done\n";
								#endif
	}


	template <typename T>
	void TSolventExcludedSurface<T>::treatSingularities()
		throw()
	{
				#ifdef print_ses
				buildHINFiles();
				#endif
		if (number_of_singular_edges_ > 0)
		{
				#ifdef debug_singularities
				//STOP_SINGULARITIES = 0;
				#endif
				#ifdef print_ses
				ofstream sesfile("ses.pre.log");
				sesfile << *this;
				sesfile.close();
				buildHINFiles();
				#endif
			::std::list<TSESFace<T>*> singular_faces;
			getSingularFaces(singular_faces);
			treatFirstCategory(singular_faces);
				#ifdef print_ses
				sesfile.open("ses.first.log");
				sesfile << *this;
				sesfile.close();
				buildHINFiles();
				#endif
			treatSecondCategory();
				#ifdef print_ses
				sesfile.open("ses.second.log");
				sesfile << *this;
				sesfile.close();
				buildHINFiles();
				#endif
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::getSingularFaces(::std::list<TSESFace<T>*>& faces)
		throw()
	{
		for (Position i = 0; i < number_of_spheric_faces_; i++)
		{
			if (spheric_faces_[i]->rsface_->singular_ == true)
			{
				faces.push_back(spheric_faces_[i]);
			}
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::treatFirstCategory(::std::list<TSESFace<T>*>& singular_faces)
		throw()
	{
		::std::list<TSESFace<T>*> first_category_faces;
		getFirstCategoryFaces(singular_faces,first_category_faces);
				#ifdef debug_singularities
				std::cout << "SingularFaces.size() = " << singular_faces.size() << "\n";
				std::cout << "FirstCategoryFaces.size() = " << first_category_faces.size();
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		TSESFace<T>* face1;
		TSESFace<T>* face2;
		bool modified = true;
		while (modified && (first_category_faces.size() > 0))
		{
			modified = false;
			while (first_category_faces.size() > 0)
			{
				face1 = first_category_faces.front();
				first_category_faces.pop_front();
				face2 = first_category_faces.front();
				first_category_faces.pop_front();
				//Position intersections = 0;
				//for (Position i = 0; i < 3; i++)
				//{
				//	if (face1->edge_[i] == NULL)
				//	{
				//		intersections++;
				//	}
				//}
				switch (face1->edge_.size())
				{
					case 3 :	noCut(face1,face2);
										break;
					case 5 :	break;
					case 7 :	twoCuts(face1,face2);
										break;
					case 9 :	reduced_surface_->deleteSimilarFaces(face1->rsface_,face2->rsface_);
										modified = true;
										break;
				}
			}
			if (modified)
			{
				reduced_surface_->clean();
				set(reduced_surface_);
				get();
				singular_faces.clear();
				getSingularFaces(singular_faces);
				getFirstCategoryFaces(singular_faces,first_category_faces);
			}
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::getFirstCategoryFaces
		(::std::list<TSESFace<T>*>& singular_faces,
		 ::std::list<TSESFace<T>*>& first_category_faces)
		throw()
	{
		::std::list<TSESFace<T>*> singular;
		while (singular_faces.size() > 0)
		{
			TSESFace<T>* current = singular_faces.front();
			singular_faces.pop_front();
			bool found = false;
			typename ::std::list<TSESFace<T>*>::iterator i = singular_faces.begin();
			while (i != singular_faces.end())
			{
				if (current->rsface_->similar(*((*i)->rsface_)))
				{
					first_category_faces.push_back(current);
					first_category_faces.push_back(*i);
					singular_faces.erase(i);
					i = singular_faces.end();
					found = true;
				}
				else
				{
					i++;
				}
			}
			if (!found)
			{
				singular.push_back(current);
			}
		}
		singular_faces = singular;
	}


	template <class T>
	void TSolventExcludedSurface<T>::noCut(TSESFace<T>* face1, TSESFace<T>* face2)
		throw()
	{
		TCircle3<T> circle;
		T radius_of_probe = reduced_surface_->probe_radius_;
		TSphere3<T> s1(face1->rsface_->center_,radius_of_probe);
		TSphere3<T> s2(face2->rsface_->center_,radius_of_probe);
		GetIntersection(s1,s2,circle);
		// test wether the circle is really an edge
		TVector3<T> normal(face1->rsface_->normal_);
		TVector3<T> point1(reduced_surface_->atom_[face1->rsface_->vertex_[0]->atom_].p);
		TVector3<T> point2(reduced_surface_->atom_[face1->rsface_->vertex_[1]->atom_].p);
		TVector3<T> point3(reduced_surface_->atom_[face1->rsface_->vertex_[2]->atom_].p);
		TVector3<T> u(normal%(point1-point2));
		TVector3<T> v(normal%(point2-point3));
		TVector3<T> w(normal%(point3-point1));
		TVector3<T> diff1(point1-circle.p);
		TVector3<T> diff2(point2-circle.p);
		T test1 = u*diff1;
		T test2 = v*diff2;
		T test3 = w*diff1;
		if ((Maths::isLess(test1,(T)0) && Maths::isLess(test2,(T)0) && Maths::isLess(test3,(T)0)) ||
				(Maths::isGreater(test1,(T)0) && Maths::isGreater(test2,(T)0) && Maths::isGreater(test3,(T)0)))
		{
			TSESEdge<T>* edge = new TSESEdge<T>(NULL,NULL,face1,face2,circle,NULL,
																					TSESEdge<T>::TYPE_SINGULAR,number_of_edges_);
			edges_.push_back(edge);
			singular_edges_.push_back(edge);
			number_of_edges_++;
			face1->edge_.push_back(edge);
			face2->edge_.push_back(edge);
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::twoCuts(TSESFace<T>* face1, TSESFace<T>* face2)
		throw()
	{
		std::vector<TSESEdge<T>*> sesedge1(7);
		std::vector<TSESEdge<T>*> sesedge2(7);
		std::vector<TSESVertex<T>*> sesvertex1(7);
		std::vector<TSESVertex<T>*> sesvertex2(7);
		typename std::list<TSESVertex<T>*>::iterator v = face1->vertex_.begin();
		sesvertex1[0] = *v;
		v++;
		sesvertex1[1] = *v;
		v++;
		sesvertex1[2] = *v;
		if (!face1->getEdge(sesvertex1[0],sesvertex1[1],sesedge1[0]))
		{
			sesvertex1[3] = sesvertex1[0];
			sesvertex1[0] = sesvertex1[2];
			sesvertex1[2] = sesvertex1[3];
			if (!face1->getEdge(sesvertex1[0],sesvertex1[1],sesedge1[0]))
			{
				sesvertex1[3] = sesvertex1[2];
				sesvertex1[2] = sesvertex1[1];
				sesvertex1[1] = sesvertex1[3];
				face1->getEdge(sesvertex1[0],sesvertex1[1],sesedge1[0]);
			}
		}
		bool found;
		for (Position i = 0; i < 3; i++)
		{
			found = false;
			v = face2->vertex_.begin();
			while ((!found) && (v != face2->vertex_.end()))
			{
				if ((*v)->atom_ == sesvertex1[i]->atom_)
				{
					sesvertex2[i] = *v;
					found = true;
				}
				v++;
			}
		}
		face2->getEdge(sesvertex2[0],sesvertex2[1],sesedge2[0]);
		TSESEdge<T>* sesedge;
		for (Position i = 1; i < 6; i++)
		{
			face1->getEdges(sesvertex1[i],sesedge1[i],sesedge);
			if (sesedge != sesedge1[i-1])
			{
				sesedge1[i] = sesedge;
			}
			face2->getEdges(sesvertex2[i],sesedge2[i],sesedge);
			if (sesedge != sesedge2[i-1])
			{
				sesedge2[i] = sesedge;
			}
			sesvertex1[i+1] = sesedge1[i]->otherVertex(sesvertex1[i]);
			sesvertex2[i+1] = sesedge2[i]->otherVertex(sesvertex2[i]);
		}
		face1->getEdge(sesvertex1[0],sesvertex1[6],sesedge1[6]);
		face2->getEdge(sesvertex2[0],sesvertex2[6],sesedge2[6]);
		TCircle3<T> circle;
		TSphere3<T> sphere1(face1->rsface_->center_,reduced_surface_->probe_radius_);
		TSphere3<T> sphere2(face2->rsface_->center_,reduced_surface_->probe_radius_);
		GetIntersection(sphere1,sphere2,circle);
		TAngle<T> phi(getOrientedAngle(sesvertex1[3]->point_-circle.p,
																	 sesvertex1[5]->point_-circle.p,
																	 circle.n));
		if (phi.value < Constants::PI)
		{
			circle.n.negate();
		}
		TSESEdge<T>* new_edge1 = new TSESEdge<T>(sesvertex1[2],sesvertex1[6],face1,face2,
																						 circle,NULL,TSESEdge<T>::TYPE_SINGULAR,
																						 number_of_edges_);
		edges_.push_back(new_edge1);
		singular_edges_.push_back(new_edge1);
		face1->edge_.push_back(new_edge1);
		face2->edge_.push_back(new_edge1);
		sesvertex1[2]->edges_.insert(new_edge1);
		sesvertex1[6]->edges_.insert(new_edge1);
		number_of_edges_++;
		number_of_singular_edges_++;
		circle.n.negate();
		TSESEdge<T>* new_edge2 = new TSESEdge<T>(sesvertex1[3],sesvertex1[5],face1,face2,
																						 circle,NULL,TSESEdge<T>::TYPE_SINGULAR,
																						 number_of_edges_);
		edges_.push_back(new_edge2);
		singular_edges_.push_back(new_edge2);
		face1->edge_.push_back(new_edge2);
		face2->edge_.push_back(new_edge2);
		sesvertex1[3]->edges_.insert(new_edge2);
		sesvertex1[5]->edges_.insert(new_edge2);
		number_of_edges_++;
		number_of_singular_edges_++;
		if (sesedge1[2] == sesedge2[2])
		{
			edges_[sesedge1[2]->index_] = NULL;
			singular_edges_.remove(sesedge1[2]);
			sesvertex1[2]->edges_.erase(sesedge1[2]);
			sesvertex1[3]->edges_.erase(sesedge1[2]);
			face1->edge_.remove(sesedge1[2]);
			face2->edge_.remove(sesedge1[2]);
			delete sesedge1[2];
						sesedge1[2] = NULL;
		}
		if (sesedge1[5] == sesedge2[5])
		{
			edges_[sesedge1[5]->index_] = NULL;
			singular_edges_.remove(sesedge1[5]);
			sesvertex1[5]->edges_.erase(sesedge1[5]);
			sesvertex1[6]->edges_.erase(sesedge1[5]);
			face1->edge_.remove(sesedge1[5]);
			face2->edge_.remove(sesedge1[5]);
			delete sesedge1[5];
						sesedge1[5] = NULL;
		}
				/*std::cout << *sesvertex1[0] << "\n";
				std::cout << *sesvertex1[1] << "\n";
				std::cout << *sesvertex1[2] << "\n";
				std::cout << *sesvertex1[3] << "\n";
				std::cout << *sesvertex1[4] << "\n";
				std::cout << *sesvertex1[5] << "\n";
				std::cout << *sesvertex1[6] << "\n";
				std::cout << *sesedge1[0] << "\n";
				std::cout << *sesedge1[1] << "\n";
				((sesedge1[2] != NULL) ? std::cout << *sesedge1[2] << "\n" : cout << "--\n");
				std::cout << *sesedge1[3] << "\n";
				std::cout << *sesedge1[4] << "\n";
				((sesedge1[5] != NULL) ? std::cout << *sesedge1[5] << "\n" : cout << "--\n");
				std::cout << *sesedge1[6] << "\n\n";
				std::cout << *sesvertex2[0] << "\n";
				std::cout << *sesvertex2[1] << "\n";
				std::cout << *sesvertex2[2] << "\n";
				std::cout << *sesvertex2[3] << "\n";
				std::cout << *sesvertex2[4] << "\n";
				std::cout << *sesvertex2[5] << "\n";
				std::cout << *sesvertex2[6] << "\n";
				std::cout << *sesedge2[0] << "\n";
				std::cout << *sesedge2[1] << "\n";
				std::cout << *sesedge2[3] << "\n";
				std::cout << *sesedge2[4] << "\n";
				std::cout << *sesedge2[6] << "\n\n";
				std::cout << *new_edge1 << "\n";
				std::cout << *new_edge2 << "\n";
				std::cout << *face1 << "\n";
				std::cout << *face2 << "\n\n\n";*/
	}


	template <class T>
	void TSolventExcludedSurface<T>::treatSecondCategory()
		throw()
	{
		T x_min = spheric_faces_[0]->rsface_->center_.x;
		T y_min = spheric_faces_[0]->rsface_->center_.y;
		T z_min = spheric_faces_[0]->rsface_->center_.z;
		T x_max = spheric_faces_[0]->rsface_->center_.x;
		T y_max = spheric_faces_[0]->rsface_->center_.y;
		T z_max = spheric_faces_[0]->rsface_->center_.z;
		for (Position i = 1; i != number_of_spheric_faces_; i++)
		{
			x_min = ((x_min > spheric_faces_[i]->rsface_->center_.x)
											? spheric_faces_[i]->rsface_->center_.x
											: x_min);
			y_min = ((y_min > spheric_faces_[i]->rsface_->center_.y)
											? spheric_faces_[i]->rsface_->center_.y
											: y_min);
			z_min = ((z_min > spheric_faces_[i]->rsface_->center_.z)
											? spheric_faces_[i]->rsface_->center_.z
											: z_min);
			x_max = ((x_max < spheric_faces_[i]->rsface_->center_.x)
											? spheric_faces_[i]->rsface_->center_.x
											: x_max);
			y_max = ((y_max < spheric_faces_[i]->rsface_->center_.y)
											? spheric_faces_[i]->rsface_->center_.y
											: y_max);
			z_max = ((z_max < spheric_faces_[i]->rsface_->center_.z)
											? spheric_faces_[i]->rsface_->center_.z
											: z_max);
		}
		T dist = 2*reduced_surface_->probe_radius_;
		Position nx = (Position)((x_max-x_min)/dist+5);
		Position ny = (Position)((y_max-y_min)/dist+5);
		Position nz = (Position)((z_max-z_min)/dist+5);
		TVector3<T> origin(x_min-2*dist,y_min-2*dist,z_min-2*dist);
		HashGrid3<Position> grid(origin,nx,ny,nz,dist);
		for (Position i = 0; i != number_of_spheric_faces_; i++)
		{
			grid.insert(spheric_faces_[i]->rsface_->center_,i);
		}
		typename ::std::list<TSESEdge<T>*>::iterator edge;
		::std::list<TSESEdge<T>*> deletable_edges;
		for (edge = singular_edges_.begin(); edge != singular_edges_.end(); edge++)
		{
			treatSingularEdge(*edge,grid,deletable_edges);
		}
		for (edge = deletable_edges.begin(); edge != deletable_edges.end(); edge++)
		{
			(*edge)->face_[0]->edge_.remove(*edge);
			(*edge)->face_[1]->edge_.remove(*edge);
			(*edge)->vertex_[0]->edges_.erase(*edge);
			(*edge)->vertex_[1]->edges_.erase(*edge);
			edges_[(*edge)->index_] = NULL;
			singular_edges_.remove(*edge);
			delete *edge;
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::treatSingularEdge
		(TSESEdge<T>*												edge,
		 HashGrid3<Position>&								grid,
		 ::std::list<TSESEdge<T>*>&					deletable_edges)
		throw()
	{
				#ifdef print_ses
				buildHINFiles();
				#endif
				#ifdef debug_singularities
				ofstream sesfile("ses.log");
				sesfile << *this;
				sesfile.close();
				std::cout << "TreatSingularEdge( " << *edge << " , [" << spheric_faces_.size() << "] , grid)  ";
//				STOP_SINGULARITIES = 0;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		if (edge->vertex_[0] == NULL)
		{
			return;
		}
		TAngle<T> phi = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
																		 edge->vertex_[1]->point_-edge->circle_.p,
																		 edge->circle_.n);
				#ifdef debug_singularities
				std::cout << "  Drehvektor:      " << edge->circle_.n << "\n";
				std::cout << "  Winkel der Edge: " << phi;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		std::list<Intersection> intersections;
		getIntersectionsOfSingularEdge(edge,phi,grid,intersections);
		if (!intersections.empty())
		{
			HashSet<Index> indices;
			TAngle<T> min_phi(2*Constants::PI);
			TAngle<T> max_phi(0,true);
			//TVector3<T> min_point(intersections.front().second);
			//TVector3<T> max_point(intersections.front().second);
			typename std::list<Intersection>::const_iterator i;
			std::list<Intersection> min;
			std::list<Intersection> max;
			//double epsilon = Constants::EPSILON;
			//Constants::EPSILON = 0.001;
			//for (i = intersections.begin(); i != intersections.end(); i++)
			//{
			//	Index cmp = compareAngles(edge->vertex_[0]->point_,i->second,min_point,edge->circle_);
			//	if (cmp <= 0)
			//	{
			//		if (cmp < 0)
			//		{
			//			min.clear();
			//			min_point = i->second;
			//		}
			//		min.push_back(*i);
			//	}
			//	cmp = compareAngles(edge->vertex_[0]->point_,i->second,max_point,edge->circle_);
			//	if (cmp >= 0)
			//	{
			//		if (cmp > 0)
			//		{
			//			max.clear();
			//			max_point = i->second;
			//		}
			//		max.push_back(*i);
			//	}
			//	//indices.insert(i->first.second);
			//}
			//Constants::EPSILON = epsilon;
			double epsilon = Constants::EPSILON;
			Constants::EPSILON = 0.0001;
			for (i = intersections.begin(); i != intersections.end(); i++)
			{
				if (i->first.first <= min_phi)
				{
					if (i->first.first < min_phi)
					{
						min.clear();
						min_phi = i->first.first;
					}
					min.push_back(*i);
				}
				if (i->first.first >= max_phi)
				{
					if (i->first.first > max_phi)
					{
						max.clear();
						max_phi = i->first.first;
					}
					max.push_back(*i);
				}
				//indices.insert(i->first.second);
			}
			Constants::EPSILON = epsilon;
					#ifdef debug_singularities
					std::cout << "    Minimum: ";
					for (i = min.begin(); i != min.end(); i++)
					{
						std::cout << i->first.second << " ";
					}
					std::cout << "\n    Maximum: ";
					for (i = max.begin(); i != max.end(); i++)
					{
						std::cout << i->first.second << " ";
					}
					std::cout << "\n";
					#endif
					for (i = min.begin(); i != min.end(); i++)
					{
						indices.insert(i->first.second);
					}
					for (i = max.begin(); i != max.end(); i++)
					{
						indices.insert(i->first.second);
					}
			indices.insert(edge->face_[0]->index_);
			indices.insert(edge->face_[1]->index_);
					#ifdef debug_singularities
					if (indices.has(5) || indices.has(9)) STOP_SINGULARITIES = 0;
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
			TSESVertex<T>* end_vertex1;
			TSESVertex<T>* end_vertex2;
			Index actual_min;
			Index actual_max;
			buildEndEdges(edge,min,max,end_vertex1,end_vertex2,actual_min,actual_max);
			Index face1 = edge->face_[0]->index_;
			Index face2 = edge->face_[1]->index_;
			Index next_face = actual_min;
			TSESVertex<T>* vertex = end_vertex1;
			while ((next_face != face2) && (vertex != NULL))
			{
				buildEdge(edge,face1,next_face,face2,vertex,indices,true);
			}
			if (next_face != face2)
			{
				next_face = actual_max;
				vertex = end_vertex2;
				while ((next_face != face2) && (vertex != NULL))
				{
					buildEdge(edge,face1,next_face,face2,vertex,indices,false);
				}
			}
			face2 = face1;
			face1 = edge->face_[1]->index_;
			next_face = actual_min;
			vertex = end_vertex1;
			while ((next_face != face2) && (vertex != NULL))
			{
				buildEdge(edge,face1,next_face,face2,vertex,indices,true);
			}
			if (next_face != face2)
			{
				next_face = actual_max;
				vertex = end_vertex2;
				while ((next_face != face2) && (vertex != NULL))
				{
					buildEdge(edge,face1,next_face,face2,vertex,indices,false);
				}
			}
			deletable_edges.push_back(edge);
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::getIntersectionsOfSingularEdge
		(TSESEdge<T>*							edge,
		 const TAngle<T>&					phi,
		 HashGrid3<Position>&			grid,
		 std::list<Intersection>& intersections)
		throw()
	{
		TQuaternion<T> rotate(edge->circle_.n,phi/2);
		TMatrix4x4<T> rotation;
		rotate.getRotationMatrix(rotation);
		TVector4<T> middle_(edge->vertex_[0]->point_.x-edge->circle_.p.x,
												edge->vertex_[0]->point_.y-edge->circle_.p.y,
												edge->vertex_[0]->point_.z-edge->circle_.p.z,
												0);
		middle_ = rotation*middle_;
		TVector3<T> middle(middle_.x+edge->circle_.p.x,
											 middle_.y+edge->circle_.p.y,
											 middle_.z+edge->circle_.p.z);
		Index face1 = edge->face_[0]->index_;
		Index face2 = edge->face_[1]->index_;
		TAngle<T> phi1;
		TAngle<T> phi2;
		TVector3<T> point1;
		TVector3<T> point2;
		TSphere3<T> probe;
		probe.radius = reduced_surface_->probe_radius_;
		Intersection intersection;
		HashGridBox3<Position>* box;
		HashGridBox3<Position>::ConstBoxIterator b;
		HashGridBox3<Position>::ConstDataIterator d;
		box = grid.getBox(edge->circle_.p);
		if (box != NULL)
		{
			for (b = box->beginBox(); b != box->endBox(); b++)
			{
				for (d = b->beginData(); d != b->endData(); d++)
				{
					if (((Index)*d != face1) && ((Index)*d != face2))
					{
								#ifdef debug_singularities
								std::cout << "  Schneide edge mit probe " << *d << " ...\n";
								#endif
						if (getIntersectionPointsAndAngles(edge->circle_,edge->vertex_[0]->point_,
																							 edge->face_[0]->index_,edge->face_[1]->index_,
																							 spheric_faces_[*d]->index_,
																							 phi1,point1,phi2,point2))
						{
							probe.p = spheric_faces_[*d]->rsface_->center_;
									#ifdef debug_singularities
									std::cout << "    " << point1 << " " << phi1 << "\n";
									std::cout << "    " << point2 << " " << phi2 << "\n";
									#endif
							if (isIntersection(phi1,phi2,phi,middle,probe))
							{
								intersection.first.second = *d;
								intersection.first.first = phi1;
								intersection.second = point1;
								intersections.push_back(intersection);
								intersection.first.first = phi2;
								intersection.second = point2;
								intersections.push_back(intersection);
										#ifdef debug_singularities
										std::cout << "    is intersection\n";
										#endif
							}
									#ifdef debug_singularities
									else
									std::cout << "    no intersection\n";
									#endif
						}
					}
				}
			}
		}
	}


	template <class T>
	bool TSolventExcludedSurface<T>::getIntersectionPointsAndAngles
		(//TSESEdge<T>*	edge,
		 const TCircle3<T>& circle,
		 const TVector3<T>& point,
		 Position			index1,
		 Position			index2,
		 Position			probe_index,
		 TAngle<T>&		phi1,
		 TVector3<T>&	point1,
		 TAngle<T>&		phi2,
		 TVector3<T>&	point2)
		throw()
	{
		if (probeIntersection(index1,
													index2,
													probe_index,
													point1,
													point2))
		//if (probeIntersection(edge->face_[0]->index_,
		//											edge->face_[1]->index_,
		//											probe_index,
		//											point1,
		//											point2))
		{
			TVector3<double> dpoint((double)point.x,(double)point.y,(double)point.z);
			TVector3<double> dpoint1((double)point1.x,(double)point1.y,(double)point1.z);
			TVector3<double> dpoint2((double)point2.x,(double)point2.y,(double)point2.z);
			TVector3<double> dcenter((double)circle.p.x,(double)circle.p.y,(double)circle.p.z);
			TVector3<double> dnormal((double)circle.n.x,(double)circle.n.y,(double)circle.n.z);
			TAngle<double> dphi1;
			TAngle<double> dphi2;
			//phi1 = getOrientedAngle(point-circle.p,
			//												point1-circle.p,
			//												circle.n);
			//phi2 = getOrientedAngle(point-circle.p,
			//												point2-circle.p,
			//												circle.n);
			dphi1 = getOrientedAngle(dpoint-dcenter,
															 dpoint1-dcenter,
															 dnormal);
			dphi2 = getOrientedAngle(dpoint-dcenter,
															 dpoint2-dcenter,
															 dnormal);
			//phi1 = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
			//												point1-edge->circle_.p,
			//												edge->circle_.n);
			//phi2 = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
			//												point2-edge->circle_.p,
			//												edge->circle_.n);
			double epsilon = Constants::EPSILON;
			Constants::EPSILON = 0.001;
			if (Maths::isEqual(dphi1.value,2*Constants::PI))
			{
				dphi1.value = (T)0;
			}
			if (Maths::isEqual(dphi2.value,2*Constants::PI))
			{
				dphi2.value = (T)0;
			}
			Constants::EPSILON = epsilon;

			if (dphi2 < dphi1)
			{
				dphi1.swap(dphi2);
				point1.swap(point2);
			}
			phi1.set((T)dphi1.value,true);
			phi2.set((T)dphi2.value,true);
			return true;
		}
		else
		{
			return false;
		}
	}


	template <typename T>
	bool TSolventExcludedSurface<T>::isIntersection
		 (const TAngle<T>&	 min_phi,
			const TAngle<T>&	 max_phi,
			const TAngle<T>&	 phi,
			const TVector3<T>& middle,
			const TSphere3<T>& probe)
		throw()
	{
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 0.001;
		bool back;
		if (max_phi > phi)
		{
			back = false;
		}
		else
		{
			if (Maths::isNotZero(min_phi.value) || (max_phi < phi))
			{
				back = true;
			}
			else
			{
				double epsilon = Constants::EPSILON;
				Constants::EPSILON = 1e-6;
				if (Maths::isGreater(probe.p.getSquareDistance(middle),probe.radius*probe.radius))
				{
					back = false;
				}
				else
				{
					back = true;
				}
				Constants::EPSILON = epsilon;
			}
		}
		Constants::EPSILON = epsilon;
		return back;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::buildEndEdges
		(TSESEdge<T>*					edge,
		 const std::list<Intersection>&	min,
		 const std::list<Intersection>&	max,
		 TSESVertex<T>*&			vertex1,
		 TSESVertex<T>*&			vertex2,
		 Index&								actual_min,
		 Index&								actual_max)
		throw()
	{
				#ifdef debug_singularities
				std::cout << "  baue End-Edges\n";
				#endif
		TSESEdge<T>* new_edge;
		typename std::list<Intersection>::const_iterator m;
		vertex1 = NULL;
		for (m = min.begin(); m != min.end(); m++)
		{
			Index test = vertexExists(m->second);
			if (test != -1)
			{
				vertex1 = vertices_[test];
				actual_min = m->first.second;
			}
		}
		if (vertex1 == NULL)
		{
			Intersection absolute_min = min.front();
			for (m = min.begin(); m != min.end(); m++)
			{
				if (m->first.first.value < absolute_min.first.first.value)
				{
					absolute_min = *m;
				}
			}
			actual_min = absolute_min.first.second;
			vertex1 = new TSESVertex<T>(absolute_min.second,
																	edge->circle_.p-absolute_min.second,
																	-2,
																	number_of_vertices_);
			vertex1->normal_.normalize();
			vertices_.push_back(vertex1);
			vertex_grid_.insert(vertex1->point_,vertex1->index_);
			number_of_vertices_++;
		}
				#ifdef debug_singularities
				std::cout << "    ns1:  " << *vertex1 << "\n";
				#endif
		if (vertex1 != edge->vertex_[0])
		{
			new_edge = new TSESEdge<T>(*edge,true);
			new_edge->vertex_[1] = vertex1;
			new_edge->rsedge_ = NULL;
			new_edge->index_ = number_of_edges_;
			edges_.push_back(new_edge);
			number_of_edges_++;
			singular_edges_.push_front(new_edge);
			number_of_singular_edges_++;
			new_edge->face_[0]->edge_.push_back(new_edge);
			new_edge->face_[1]->edge_.push_back(new_edge);
			new_edge->vertex_[0]->edges_.insert(new_edge);
			new_edge->vertex_[1]->edges_.insert(new_edge);
			new_edge->face_[0]->pushVertex(new_edge->vertex_[1]);
			new_edge->face_[1]->pushVertex(new_edge->vertex_[1]);
			new_edge->vertex_[1]->faces_.insert(new_edge->face_[0]);
			new_edge->vertex_[1]->faces_.insert(new_edge->face_[1]);
					#ifdef debug_singularities
					std::cout << "    edge: " << *new_edge << "\n";
					#endif
		}
				#ifdef debug_singularities
				else
				std::cout << "    edge: ---\n";
				#endif
		vertex2 = NULL;
		for (m = max.begin(); m != max.end(); m++)
		{
			Index test = vertexExists(m->second);
			if (test != -1)
			{
				vertex2 = vertices_[test];
				actual_max = m->first.second;
			}
		}
		if (vertex2 == NULL)
		{
			Intersection absolute_max = max.front();
			for (m = max.begin(); m != max.end(); m++)
			{
				if (m->first.first.value < absolute_max.first.first.value)
				{
					absolute_max = *m;
				}
			}
			actual_max = absolute_max.first.second;
			vertex2 = new TSESVertex<T>(absolute_max.second,
																	edge->circle_.p-absolute_max.second,
																	-2,
																	number_of_vertices_);
			vertex2->normal_.normalize();
			vertices_.push_back(vertex2);
			vertex_grid_.insert(vertex2->point_,vertex2->index_);
			number_of_vertices_++;
		}
				#ifdef debug_singularities
				std::cout << "    ns2:  " << *vertex2 << "\n";
				#endif
		if (vertex2 != edge->vertex_[1])
		{
			new_edge = new TSESEdge<T>(*edge,true);
			new_edge->vertex_[0] = vertex2;
			new_edge->rsedge_ = NULL;
			new_edge->index_ = number_of_edges_;
			edges_.push_back(new_edge);
			number_of_edges_++;
			singular_edges_.push_front(new_edge);
			number_of_singular_edges_++;
			new_edge->face_[0]->edge_.push_back(new_edge);
			new_edge->face_[1]->edge_.push_back(new_edge);
			new_edge->vertex_[0]->edges_.insert(new_edge);
			new_edge->vertex_[1]->edges_.insert(new_edge);
			new_edge->face_[0]->pushVertex(new_edge->vertex_[0]);
			new_edge->face_[1]->pushVertex(new_edge->vertex_[0]);
			new_edge->vertex_[0]->faces_.insert(new_edge->face_[0]);
			new_edge->vertex_[0]->faces_.insert(new_edge->face_[1]);
					#ifdef debug_singularities
					std::cout << "    edge: " << *new_edge << "\n";
					#endif
		}
				#ifdef debug_singularities
				else
				std::cout << "    edge: ---\n";
				#endif
	}

  /*
	template <typename T>
	void TSolventExcludedSurface<T>::buildEdge
		(TSESEdge<T>*						edge,
		 Index									face1,
		 Index&									face2,
		 Index									end,
		 //TVector3<T>&						point,
		 TSESVertex<T>*&				vertex,
		 const HashSet<Index>&	indices)
		throw()
	{
				#ifdef debug_singularities
				std::cout << "  baue edge zwischen face " << face1 << " und face " << face2 << "\n"
									<< "  ausgehend von ";
				if (vertex == NULL)
				{
					std::cout << " (-,-,-) auf " << "(nil)\n";
				}
				else
				{
					std::cout << vertex->point_ << " auf " << *vertex << "\n";
				}
				std::cout << "    (nötig bei Behandlung von " << *edge << ")\n";
				#endif
		TSESFace<T>* spheric_face1 = spheric_faces_[face1];
		TSESFace<T>* spheric_face2 = spheric_faces_[face2];
		TSphere3<T> probe1(spheric_face1->rsface_->center_,
											 reduced_surface_->probe_radius_);
		TSphere3<T> probe2(spheric_face2->rsface_->center_,
											 reduced_surface_->probe_radius_);
		TCircle3<T> circle;
		GetIntersection(probe1,probe2,circle);
		if (((probe1.p-edge->circle_.p)*edge->circle_.n)*
				((probe1.p-circle.p)*circle.n)								 < 0)
		{
			circle.n.negate();
		}
				#ifdef debug_singularities
				std::cout << "    edge auf " << circle << "\n";
				#endif
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 0.001;
		TVector3<T> point1;
		TVector3<T> point2;
		TAngle<T> phi1;
		TAngle<T> phi2;
		std::list< std::pair<TVector3<T>,Index> > min;
		typename HashSet<Index>::ConstIterator i = indices.begin();
		while ((*i == face1) || (*i == face2))
		{
			i++;
		}
		probeIntersection(face1,face2,*i,point1,point2);
				TVector3<T> point(vertex->point_);
		TVector3<T> min_point(point1);
		if (min_point == point)
		{
			min_point.set(point2);
		}
		std::pair<TVector3<T>,Index> new_min;
		for (i = indices.begin(); i != indices.end(); i++)
		{
			if ((*i != face1) && (*i != face2))
			{
				if (probeIntersection(face1,face2,*i,point1,point2))
				{
							#ifdef debug_singularities
							std::cout << "    Ende bei face " << *i << ":\n";
							#endif
					new_min.second = *i;
					new_min.first = point1;
					Index cmp = compareAngles(point,point1,min_point,circle);
					if ((cmp <= 0) && (point1 != point))
					{
						if (cmp < 0)
						{
							min.clear();
						}
						min_point = point1;
						min.push_back(new_min);
								#ifdef debug_singularities
								TVector3<T> p1, p2;
								getIntersectionPointsAndAngles(circle,point,face1,face2,*i,phi1,p1,phi2,p2);
								std::cout << "      " << point1 << " bei " << (point1 == p1 ? phi1 : phi2)
													<< " ist neues Minimum (" << min.size() << ")\n";
								#endif
					}
							#ifdef debug_singularities
							else
							{
								TVector3<T> p1, p2;
								getIntersectionPointsAndAngles(circle,point,face1,face2,*i,phi1,p1,phi2,p2);
								std::cout << "      " << point1 << " bei " << (point1 == p1 ? phi1 : phi2)
													<< " ist kein neues Minimum\n";
							}
							#endif
					new_min.first = point2;
					cmp = compareAngles(point,point2,min_point,circle);
					if ((cmp <= 0) && (point2 != point))
					{
						if (cmp < 0)
						{
							min.clear();
						}
						min_point = point2;
						min.push_back(new_min);
								#ifdef debug_singularities
								TVector3<T> p1, p2;
								getIntersectionPointsAndAngles(circle,point,face1,face2,*i,phi1,p1,phi2,p2);
								std::cout << "      " << point2 << " bei " << (point2 == p1 ? phi1 : phi2)
													<< " ist neues Minimum (" << min.size() << ")\n";
								#endif
					}
							#ifdef debug_singularities
							else
							{
								TVector3<T> p1, p2;
								getIntersectionPointsAndAngles(circle,point,face1,face2,*i,phi1,p1,phi2,p2);
								std::cout << "      " << point2 << " bei " << (point2 == p1 ? phi1 : phi2)
													<< " ist kein neues Minimum\n";
							}
							#endif
				}
			}
		}
		Constants::EPSILON = epsilon;
		Index index = -1;
		TVector3<T> point_;
		TSESVertex<T>* new_vertex = NULL;
		typename std::list< std::pair<TVector3<T>,Index> >::iterator m = min.begin();
		bool found = false;
		while ((!found) && (m != min.end()))
		{
			if (m->second == end)
			{
				point_ = m->first;
				face2 = end;
				found = true;
			}
			m++;
		}
		if (found)
		{
			index = vertexExists(point_);
			if (index != -1)
			{
				new_vertex = vertices_[index];
			}
		}
		else
		{
			m = min.begin();
			while ((index == -1) && (m != min.end()))
			{
				point_ = m->first;
				face2 = m->second;
				index = vertexExists(point_);
				if (index != -1)
				{
					new_vertex = vertices_[index];
				}
				m++;
			}
		}
				#ifdef debug_singularities
				std::cout << "    beende edge mit Punkt " << point_ << " an face " << face2 << "\n";
				#endif
		if (spheric_face1->isNeighbouredTo(spheric_face2) == false)
		{
			if (vertex == NULL)
			{
				index = vertexExists(point);
				if (index != -1)
				{
					vertex = vertices_[index];
				}
				else
				{
					vertex = new TSESVertex<T>(point,circle.p-point,-2,number_of_vertices_);
					vertex->normal_.normalize();
					vertices_.push_back(vertex);
					vertex_grid_.insert(vertex->point_,vertex->index_);
					number_of_vertices_++;
				}
			}
						#ifdef debug_singularities
						std::cout << "    erster vertex: " << *vertex << "\n";
						#endif
			if (new_vertex == NULL)
			{
				new_vertex = new TSESVertex<T>(point_,circle.p-point_,-2,number_of_vertices_);
				new_vertex->normal_.normalize();
				vertices_.push_back(new_vertex);
				vertex_grid_.insert(new_vertex->point_,new_vertex->index_);
				number_of_vertices_++;
			}
						#ifdef debug_singularities
						std::cout << "    zweiter vertex: " << *new_vertex << "\n";
						#endif
			TSESEdge<T>* new_edge = new TSESEdge<T>;
			new_edge->vertex_[0] = vertex;
			new_edge->vertex_[1] = new_vertex;
			new_edge->face_[0] = spheric_face1;
			new_edge->face_[1] = spheric_face2;
			new_edge->type_ = TSESEdge<T>::TYPE_SINGULAR;
			new_edge->circle_ = circle;
			new_edge->rsedge_ = NULL;
			new_edge->index_ = number_of_edges_;
			edges_.push_back(new_edge);
			number_of_edges_++;
			singular_edges_.push_back(new_edge);
			number_of_singular_edges_++;
			new_edge->face_[0]->edge_.push_back(new_edge);
			new_edge->face_[1]->edge_.push_back(new_edge);
			new_edge->vertex_[0]->edges_.insert(new_edge);
			new_edge->vertex_[1]->edges_.insert(new_edge);
			new_edge->face_[0]->pushVertex(vertex);
			new_edge->face_[1]->pushVertex(vertex);
			new_edge->face_[0]->pushVertex(new_vertex);
			new_edge->face_[1]->pushVertex(new_vertex);
			vertex->faces_.insert(new_edge->face_[0]);
			vertex->faces_.insert(new_edge->face_[1]);
			new_vertex->faces_.insert(new_edge->face_[0]);
			new_vertex->faces_.insert(new_edge->face_[1]);
			vertex = new_vertex;
					#ifdef debug_singularities
					std::cout << "    " << *new_edge << " gepushed\n";
					int in; cin >> in;
					#endif
		}
		else
		{
			vertex = NULL;
					#ifdef debug_singularities
					std::cout << "    keine neue edge gebildet\n";
					int in; cin >> in;
					#endif
		}
		point = point_;
	}
	*/
	template <typename T>
	void TSolventExcludedSurface<T>::buildEdge
		(TSESEdge<T>*						edge,
		 Index									face1,
		 Index&									face2,
		 Index									end,
		 TSESVertex<T>*&				vertex,
		 const HashSet<Index>&	indices,
		 bool										minimum)
		throw()
	{
				#ifdef debug_singularities
				std::cout << "  baue edge zwischen face " << face1 << " und face " << face2 << "\n"
									<< "  ausgehend von ";
				if (vertex == NULL)
				{
					std::cout << " (-,-,-) auf " << "(nil)\n";
				}
				else
				{
					std::cout << vertex->point_ << " auf " << *vertex << "\n";
				}
				std::cout << "    (nötig bei Behandlung von " << *edge << ")\n";
				#endif
		TSESFace<T>* spheric_face1 = spheric_faces_[face1];
		TSESFace<T>* spheric_face2 = spheric_faces_[face2];
		TSphere3<T> probe1(spheric_face1->rsface_->center_,
											 reduced_surface_->probe_radius_);
		TSphere3<T> probe2(spheric_face2->rsface_->center_,
											 reduced_surface_->probe_radius_);
		TCircle3<T> circle;
		GetIntersection(probe1,probe2,circle);
		if (minimum)
		{
			if (((probe1.p-edge->circle_.p)*edge->circle_.n)*
					((probe1.p-circle.p)*circle.n)								 < 0)
			{
				circle.n.negate();
			}
		}
		else
		{
			if (((probe1.p-edge->circle_.p)*edge->circle_.n)*
					((probe1.p-circle.p)*circle.n)								 > 0)
			{
				circle.n.negate();
			}
		}
				#ifdef debug_singularities
				std::cout << "    edge auf " << circle << "\n";
				#endif
		typename HashSet<Index>::ConstIterator i;
		TVector3<T> point(vertex->point_);
		TVector3<T> point1;
		TVector3<T> point2;
		TAngle<T> phi1;
		TAngle<T> phi2;
		std::list< std::pair<TVector3<T>,Index> > min;
		TAngle<T> min_phi(2*Constants::PI,true);
		std::pair<TVector3<T>,Index> new_min;
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 0.001;
		for (i = indices.begin(); i != indices.end(); i++)
		{
			if ((*i != face1) && (*i != face2))
			{
				//if (probeIntersection(face1,face2,*i,point1,point2))
				if (getIntersectionPointsAndAngles(circle,point,face1,face2,*i,phi1,point1,phi2,point2))
				{
							#ifdef debug_singularities
							std::cout << "    Ende bei face " << *i << ":\n";
							#endif
					new_min.second = *i;
					//getIntersectionPointsAndAngles(circle,point,face1,face2,*i,phi1,point1,phi2,point2);
					if ((phi1 <= min_phi) && Maths::isGreater(phi1.value,(T)0))
					{
						if (phi1 < min_phi)
						{
							min.clear();
						}
						min_phi = phi1;
						new_min.first = point1;
						min.push_back(new_min);
								#ifdef debug_singularities
								std::cout << "      " << point1 << " bei " << phi1 << " ist neues Minimum ("
													<< min.size() << ")\n";
								#endif
					}
							#ifdef debug_singularities
							else
							std::cout << "      " << point1 << " bei " << phi1 << " ist kein neues Minimum\n";
							#endif
					if ((phi2 <= min_phi) && Maths::isGreater(phi2.value,(T)0))
					{
						if (phi2 < min_phi)
						{
							min.clear();
						}
						min_phi = phi2;
						new_min.first = point2;
						min.push_back(new_min);
								#ifdef debug_singularities
								std::cout << "      " << point2 << " bei " << phi2 << " ist neues Minimum ("
													<< min.size() << ")\n";
								#endif
					}
							#ifdef debug_singularities
							else
							std::cout << "      " << point2 << " bei " << phi2 << " ist kein neues Minimum\n";
							#endif
					//new_phi = getOrientedAngle(point-circle.p,point1-circle.p,circle.n);
					//new_min.first = point1;
					//if ((new_phi <= min_phi) && Maths::isGreater(new_phi.value,(T)0))
					//{
					//	if (new_phi < min_phi)
					//	{
					//		min.clear();
					//	}
					//	min_phi = new_phi;
					//	min.push_back(new_min);
					//			#ifdef debug_singularities
					//			std::cout << "      " << point1 << " bei " << new_phi << " ist neues Minimum ("
					//								<< min.size() << ")\n";
					//			#endif
					//}
					//		#ifdef debug_singularities
					//		else
					//		std::cout << "      " << point1 << " bei " << new_phi << " ist kein neues Minimum\n";
					//		#endif
					//new_phi = getOrientedAngle(point-circle.p,point2-circle.p,circle.n);
					//new_min.first = point2;
					//if ((new_phi <= min_phi) && Maths::isGreater(new_phi.value,(T)0))
					//{
					//	if (new_phi < min_phi)
					//	{
					//		min.clear();
					//	}
					//	min_phi = new_phi;
					//	min.push_back(new_min);
					//			#ifdef debug_singularities
					//			std::cout << "      " << point2 << " bei " << new_phi << " ist neues Minimum ("
					//								<< min.size() << ")\n";
					//			#endif
					//}
					//		#ifdef debug_singularities
					//		else
					//		std::cout << "      " << point2 << " bei " << new_phi << " ist kein neues Minimum\n";
					//		#endif
				}
			}
		}
		Constants::EPSILON = epsilon;
		Index index = -1;
		TVector3<T> point_;
		TSESVertex<T>* new_vertex = NULL;
		typename std::list< std::pair<TVector3<T>,Index> >::iterator m = min.begin();
		bool found = false;
		while ((!found) && (m != min.end()))
		{
			if (m->second == end)
			{
				point_ = m->first;
				face2 = end;
				found = true;
			}
			m++;
		}
		if (found)
		{
			index = vertexExists(point_);
			if (index != -1)
			{
				new_vertex = vertices_[index];
			}
		}
		else
		{
			m = min.begin();
			while ((index == -1) && (m != min.end()))
			{
				point_ = m->first;
				face2 = m->second;
				index = vertexExists(point_);
				if (index != -1)
				{
					new_vertex = vertices_[index];
				}
				m++;
			}
		}
				#ifdef debug_singularities
				std::cout << "    beende edge mit Punkt " << point_ << " an face " << face2 << "\n";
				#endif
		if (spheric_face1->isNeighbouredTo(spheric_face2) == false)
		{
			if (vertex == NULL)
			{
				index = vertexExists(point);
				if (index != -1)
				{
					vertex = vertices_[index];
				}
				else
				{
					vertex = new TSESVertex<T>(point,circle.p-point,-2,number_of_vertices_);
					vertex->normal_.normalize();
					vertices_.push_back(vertex);
					vertex_grid_.insert(vertex->point_,vertex->index_);
					number_of_vertices_++;
				}
			}
						#ifdef debug_singularities
						std::cout << "    erster vertex: " << *vertex << "\n";
						#endif
			if (new_vertex == NULL)
			{
				new_vertex = new TSESVertex<T>(point_,circle.p-point_,-2,number_of_vertices_);
				new_vertex->normal_.normalize();
				vertices_.push_back(new_vertex);
				vertex_grid_.insert(new_vertex->point_,new_vertex->index_);
				number_of_vertices_++;
			}
						#ifdef debug_singularities
						std::cout << "    zweiter vertex: " << *new_vertex << "\n";
						#endif
			TSESEdge<T>* new_edge = new TSESEdge<T>;
			new_edge->vertex_[0] = vertex;
			new_edge->vertex_[1] = new_vertex;
			new_edge->face_[0] = spheric_face1;
			new_edge->face_[1] = spheric_face2;
			new_edge->type_ = TSESEdge<T>::TYPE_SINGULAR;
			new_edge->circle_ = circle;
			new_edge->rsedge_ = NULL;
			new_edge->index_ = number_of_edges_;
			edges_.push_back(new_edge);
			number_of_edges_++;
			singular_edges_.push_back(new_edge);
			number_of_singular_edges_++;
			new_edge->face_[0]->edge_.push_back(new_edge);
			new_edge->face_[1]->edge_.push_back(new_edge);
			new_edge->vertex_[0]->edges_.insert(new_edge);
			new_edge->vertex_[1]->edges_.insert(new_edge);
			new_edge->face_[0]->pushVertex(vertex);
			new_edge->face_[1]->pushVertex(vertex);
			new_edge->face_[0]->pushVertex(new_vertex);
			new_edge->face_[1]->pushVertex(new_vertex);
			vertex->faces_.insert(new_edge->face_[0]);
			vertex->faces_.insert(new_edge->face_[1]);
			new_vertex->faces_.insert(new_edge->face_[0]);
			new_vertex->faces_.insert(new_edge->face_[1]);
			vertex = new_vertex;
					#ifdef debug_singularities
					std::cout << "    " << *new_edge << " gepushed ";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
		}
		else
		{
			vertex = NULL;
					#ifdef debug_singularities
					std::cout << "    keine neue edge gebildet ";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
		}
		point = point_;
	}


	template <typename T>
	bool TSolventExcludedSurface<T>::probeIntersection
		 (Index				 face1,
			Index				 face2,
			Index				 face3,
			TVector3<T>& point1,
			TVector3<T>& point2)
		throw()
	{
		// sort the indices of the spheric faces
		sort(face1,face2,face3,face1,face2,face3);
		// try to find the intersection points
		typename HashMap< Position,HashMap< Position,HashMap< Position,ProbeIntersection* > > >::Iterator i1;
		typename HashMap< Position,HashMap< Position,ProbeIntersection* > >::Iterator i2;
		typename HashMap< Position,ProbeIntersection* >::Iterator i3;
		bool back;
		bool found = false;
		i1 = probe_intersections_.find(face1);
		if (i1 != probe_intersections_.end())
		{
			i2 = i1->second.find(face2);
			if (i2 != i1->second.end())
			{
				i3 = i2->second.find(face3);
				if (i3 != i2->second.end())
				{
					found = true;
					if (i3->second == NULL)
					{
						back = false;
					}
					else
					{
						point1 = i3->second->point[0];
						point2 = i3->second->point[1];
						back = true;
					}
				}
			}
		}
		// if the intersection points are not computed yet, compute them now
		if (found == false)
		{
			TSphere3<double> s1(TVector3<double>((double)spheric_faces_[face1]->rsface_->center_.x,
																					 (double)spheric_faces_[face1]->rsface_->center_.y,
																					 (double)spheric_faces_[face1]->rsface_->center_.z ),
													(double)reduced_surface_->probe_radius_);
			TSphere3<double> s2(TVector3<double>((double)spheric_faces_[face2]->rsface_->center_.x,
																					 (double)spheric_faces_[face2]->rsface_->center_.y,
																					 (double)spheric_faces_[face2]->rsface_->center_.z ),
													(double)reduced_surface_->probe_radius_);
			TSphere3<double> s3(TVector3<double>((double)spheric_faces_[face3]->rsface_->center_.x,
																					 (double)spheric_faces_[face3]->rsface_->center_.y,
																					 (double)spheric_faces_[face3]->rsface_->center_.z ),
													(double)reduced_surface_->probe_radius_);
			TVector3<double> p1;
			TVector3<double> p2;
			if (GetIntersection(s1,s2,s3,p1,p2,false))
			{
				point1.set((T)p1.x,(T)p1.y,(T)p1.z);
				point2.set((T)p2.x,(T)p2.y,(T)p2.z);
				ProbeIntersection* intersection = new ProbeIntersection;
				intersection->point.push_back(point1);
				intersection->point.push_back(point2);
				//probe_intersections_[face1][face2][face3] = intersection;
				i1 = probe_intersections_.find(face1);
				if (i1 != probe_intersections_.end())
				{
					i2 = i1->second.find(face2);
					if (i2 != i1->second.end())
					{
						i2->second[face3] = intersection;
					}
					else
					{
						HashMap< Position,ProbeIntersection* > map2;
						map2[face3] = intersection;
						i1->second[face2] = map2;
					}
				}
				else
				{
					HashMap< Position,HashMap< Position,ProbeIntersection* > > map1;
					HashMap< Position,ProbeIntersection* > map2;
					map2[face3] = intersection;
					map1[face2] = map2;
					probe_intersections_[face1] = map1;
				}
				back = true;
			}
			else
			{
				//probe_intersections_[face1][face2][face3] = NULL;
				i1 = probe_intersections_.find(face1);
				if (i1 != probe_intersections_.end())
				{
					i2 = i1->second.find(face2);
					if (i2 != i1->second.end())
					{
						i2->second[face3] = NULL;
					}
					else
					{
						HashMap< Position,ProbeIntersection* > map2;
						map2[face3] = NULL;
						i1->second[face2] = map2;
					}
				}
				else
				{
					HashMap< Position,HashMap< Position,ProbeIntersection* > > map1;
					HashMap< Position,ProbeIntersection* > map2;
					map2[face3] = NULL;
					map1[face2] = map2;
					probe_intersections_[face1] = map1;
				}
				back = false;
			}
		}
		return back;
	}


	template <typename T>
	Index TSolventExcludedSurface<T>::compareAngles
		 (const TVector3<T>& start_point,
			const TVector3<T>& point1,
			const TVector3<T>& point2,
			const TCircle3<T>& circle)
		throw()
	{
//cout << "compareAngles( " << start_point << "," << point1 << "," << point2 << "," << circle << " )\n";
				//cout << "  " << start_point << (circle.has(start_point) ? " " : " not ") << "on circle\n";
				//cout << "  " << point1 << (circle.has(point1) ? " " : " not ") << "on circle\n";
				//cout << "  " << point2 << (circle.has(point2) ? " " : " not ") << "on circle\n";
		TVector3<T> v1(start_point-circle.p);
		TVector3<T> v2(point1-circle.p);
		TVector3<T> v3(point2-circle.p);
		T test1 = circle.n*(v1%v2);
		T test2 = circle.n*(v1%v3);
				//cout << "  " << circle.n << "  " << v1%v2 << "  " << v1%v3 << "\n";
//cout << "  " << test1 << "  " << test2 << "\n";
		if (Maths::isZero(test1))
		{
			if (Maths::isZero(test2))
			{
//cout << "  0\n";
				return 0;
			}
			else
			{
//cout << "  -1\n";
				return -1;
			}
		}
		else
		{
			if (Maths::isZero(test2))
			{
//cout << "  1\n";
				return 1;
			}
		}
		if ((test1 > 0) && (test2 < 0))
		{
//cout << "  -1\n";
			return -1;
		}
		if ((test1 < 0) && (test2 > 0))
		{
//cout << "  1\n";
			return 1;
		}
		T test3 = v1*v2;
		T test4 = v1*v3;
				//cout << "  " << v1 << "  " << v2 << "  " << v3 << "\n";
//cout << "  " << test3 << "  " << test4 << "\n";
		Index cmp;
		if ((test3 > 0) && (test4 < 0))
		{
			cmp = 1;
		}
		else
		{
			if ((test3 < 0) && (test4 > 0))
			{
				cmp = -1;
			}
			else
			{
				T test5 = (test3*test3)/(v2*v2);
				T test6 = (test4*test4)/(v3*v3);
//cout << "  " << test5 << "  " << test6 << "\n";
						//T test7 = test3/(v1.getLength()*v2.getLength());
						//T test8 = test4/(v1.getLength()*v3.getLength());
						//cout << "  " << test7 << "  " << test8 << "\n";
						//cout << "  " << acos(test7) << "  " << acos(test8) << "\n";
				if ((test3 > 0) || (test4 > 0))
				{
					if (Maths::isLess(test5,test6))
					{
						cmp = -1;
					}
					else
					{
						if (Maths::isGreater(test5,test6))
						{
							cmp = 1;
						}
						else
						{
							test5 = sqrt(test5);
							test6 = sqrt(test6);
							if (Maths::isLess(test5,test6))
							{
								cmp = -1;
							}
							else
							{
								if (Maths::isGreater(test5,test6))
								{
									cmp = 1;
								}
								else
								{
									cmp = 0;
								}
							}
						}
					}
				}
				else
				{
					if (Maths::isLess(test5,test6))
					{
						cmp = 1;
					}
					else
					{
						if (Maths::isGreater(test5,test6))
						{
							cmp = -1;
						}
						else
						{
							test5 = sqrt(test5);
							test6 = sqrt(test6);
							if (Maths::isLess(test5,test6))
							{
								cmp = 1;
							}
							else
							{
								if (Maths::isGreater(test5,test6))
								{
									cmp = -1;
								}
								else
								{
									cmp = 0;
								}
							}
						}
					}
				}
			}
		}
		if (test1 > 0)
		{
//cout << "  " << -cmp << "\n";
			return -cmp;
		}
		else
		{
//cout << "  " << cmp << "\n";
			return cmp;
		}
	}


	template <typename T>
	void TSolventExcludedSurface<T>::sort
		 (Index		u1,
			Index		u2,
			Index		u3,
			Index&	s1,
			Index&	s2,
			Index&	s3)
		throw()
	{
		s1 = u1;
		s2 = u2;
		s3 = u3;
		Index tmp;
		if (s1 > s2)
		{
			tmp = s1;
			s1 = s2;
			s2 = tmp;
		}
		if (s1 > s3)
		{
			tmp = s1;
			s1 = s3;
			s3 = tmp;
		}
		if (s2 > s3)
		{
			tmp = s2;
			s2 = s3;
			s3 = tmp;
		}
	}


	template <class T>
	Index TSolventExcludedSurface<T>::vertexExists(TVector3<T> point)
		throw()
	{
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 0.001;
		HashGridBox3<Index>* box = vertex_grid_.getBox(point);
		HashGridBox3<Index>::ConstBoxIterator b;
		HashGridBox3<Index>::ConstDataIterator d;
		if (box != NULL)
		{
			for (b = box->beginBox(); b != box->endBox(); b++)
			{
				for (d = b->beginData(); d != b->endData(); d++)
				{
					if (vertices_[*d]->point_ == point)
					{
						Constants::EPSILON = epsilon;
						return *d;
					}
				}
			}
		}
		Constants::EPSILON = epsilon;
		return -1;
	}




  #ifndef no_ses_print_functions
	template <class T>
	void TSolventExcludedSurface<T>::partitionOfCircle
			(const TCircle3<T>&			circle,
			 const TVector3<T>&			p0,
			 const TAngle<T>&				phi,
			 Size										number_of_segments,
			 vector< TVector3<T> >&	partition,
			 bool										on_surface)
	{
		Vector4 p;
		if (on_surface)
		{
			Vector3 p_(p0-circle.p);
			p.set(p_.x,p_.y,p_.z,0);
		}
		else
		{
			p.set(circle.n.y,-circle.n.x,0,0);
			if (p == Vector4::getZero())
			{
				p.set(circle.n.z,0,-circle.n.x,0);
			}
			p.normalize();
			p *= circle.radius;
		}
		Quaternion rotate(circle.n,phi);
		Matrix4x4 rotation;
		rotate.getRotationMatrix(rotation);
		partition.push_back(Vector3(p.x,p.y,p.z)+circle.p);
		for (Size i = 0; i < number_of_segments; i++)
		{
			p = rotation*p;
			partition.push_back(Vector3(p.x,p.y,p.z)+circle.p);
		}
	}

	template <class T>
	void TSolventExcludedSurface<T>::partitionFreeEdge
			(TSESEdge<T>* edge,
			 const float& sqrt_density,
			 Molecule* molecule)
	{
		Angle phi;
		phi.value = 2*Constants::PI;
		Size number_of_segments = (Size)Maths::round(phi.value*edge->circle_.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		Angle psi(phi.value/number_of_segments,true);
		std::vector<Vector3> points;
		partitionOfCircle(edge->circle_,Vector3::getZero(),psi,number_of_segments,points,false);
		points.pop_back();
		Atom* a0;
		Atom* a1;
		Atom* a2;
		a1 = new Atom;
		a1->setPosition(points[0]);
		a1->setElement(PTE[Element::H]);
		molecule->insert(*a1);
		a0 = a1;
		for (Position k = 1; k < points.size(); k++)
		{
			a2 = new Atom;
			a2->setPosition(points[k]);
			a2->setElement(PTE[Element::H]);
			a2->createBond(*a1);
			a1 = a2;
			molecule->insert(*a1);
		}
		a1->createBond(*a0);
	}

	template <class T>
	void TSolventExcludedSurface<T>::partitionEdge
			(TSESEdge<T>* edge,
			 const float& sqrt_density,
			 Molecule* molecule)
	{
		Angle phi;
		if (edge->rsedge_ == NULL)
		{
			GetAngle(edge->vertex_[0]->point_-edge->circle_.p,
							 edge->vertex_[1]->point_-edge->circle_.p,
							 phi);
		}
		else
		{
			phi = edge->rsedge_->getPhi();
		}
		Size number_of_segments = (Size)Maths::round(phi.value*edge->circle_.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		Angle psi(phi.value/number_of_segments,true);
		vector<Vector3> points;
		partitionOfCircle(edge->circle_,edge->vertex_[0]->point_,psi,number_of_segments,points,true);
		points.pop_back();
		points.push_back(edge->vertex_[1]->point_);
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
			a1->createBond(*a2);
			a1 = a2;
			molecule->insert(*a2);
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::partitionSingularEdge
			(TSESEdge<T>* edge,
			 const float& sqrt_density,
			 Molecule* molecule)
	{
		Angle phi(getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
															 edge->vertex_[1]->point_-edge->circle_.p,
															 edge->circle_.n));
		Size number_of_segments	= (Size)Maths::round(phi.value*edge->circle_.radius*sqrt_density);
		if (number_of_segments == 0)
		{
			number_of_segments++;
		}
		Angle psi(phi.value/number_of_segments,true);
		std::vector<Vector3> points;
		partitionOfCircle(edge->circle_,edge->vertex_[0]->point_,psi,number_of_segments,points,true);
		points.pop_back();
		points.push_back(edge->vertex_[1]->point_);
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
			a2->createBond(*a1);
			a1 = a2;
			molecule->insert(*a1);
		}
	}
	
	template <class T>
	void TSolventExcludedSurface<T>::buildHINFiles()
	{
		vector<Molecule*> edge(number_of_edges_);
		std::cerr << "  build molecules from SES-edges ...\n";
		T sqrt_density = 3;
		for (Position i = 0; i < number_of_edges_; i++)
		{
			if (edges_[i] != NULL)
			{
				SESEdge* sesedge = edges_[i];
				edge[i] = new Molecule;
				if (sesedge->vertex_[0] == NULL)
				{
					partitionFreeEdge(sesedge,sqrt_density,edge[i]);
				}
				else
				{
					if (sesedge->type_ == SESEdge::TYPE_SINGULAR)
					{
						partitionSingularEdge(sesedge,sqrt_density,edge[i]);
					}
					else
					{
						partitionEdge(sesedge,sqrt_density,edge[i]);
					}
				}
				System *system = new System;
				Molecule molecule(*edge[i]);
				system->insert(molecule);
				HINFile hinfile("DATA/SES/edge"+IndexToString(i,0)+".hin", std::ios::out);
				hinfile << *system;
				hinfile.close();
				delete system;
			}
		}
		for (Position i = 0; i < number_of_spheric_faces_; i++)
		{
			if (spheric_faces_[i] != NULL)
			{
 				System face1;
				System face2;
				typename std::list<SESEdge*>::iterator e;
				for (e = spheric_faces_[i]->edge_.begin(); e != spheric_faces_[i]->edge_.end(); e++)
				{
					SESEdge* sesedge = *e;
					Molecule* molecule = new Molecule(*edge[sesedge->index_]);
					face2.insert(*molecule);
					if (sesedge->type_ != SESEdge::TYPE_SINGULAR)
					{
						Molecule* molecule = new Molecule(*edge[sesedge->index_]);
						face1.insert(*molecule);
					}
				}
				HINFile hinfile("DATA/SES/faceWithOut"+IndexToString(i,0)+".hin",std::ios::out);
				hinfile << face1;
				hinfile.close();
				hinfile.open("DATA/SES/faceWith"+IndexToString(i,0)+".hin",std::ios::out);
				hinfile << face2;
				hinfile.close();
				for (e = spheric_faces_[i]->edge_.begin(); e != spheric_faces_[i]->edge_.end(); e++)
				{
					if ((*e)->type_ != SESEdge::TYPE_SINGULAR)
					{
						face1.remove(*edge[(*e)->index_]);
					}
					face2.remove(*edge[(*e)->index_]);
				}
			}
		}
	}
	#endif

} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H

