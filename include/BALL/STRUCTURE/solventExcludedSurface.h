// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: solventExcludedSurface.h,v 1.23 2002/04/18 18:23:57 strobel Exp $

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

#ifndef BALL_STRUCTURE_BSDTREE_H
#	include <BALL/STRUCTURE/BSDTree.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#	include <BALL/DATATYPE/hashGrid.h>
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

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		#ifdef print_ses
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

		//@}

		private:

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
			(TSESEdge<T>* edge,
			 ::std::vector<TSESFace<T>*> faces,
			 TBSDTree<T>* tree,
			 ::std::list<TSESEdge<T>*>& deletable_edges)
			throw();

		bool getIntersectionPointsAndAngles
			(TSESEdge<T>*				edge,
			 const TSphere3<T>& probe,
			 Position						probe_index,
			 TVector3<T>&				p1,
			 TAngle<T>&					phi1,
			 TVector3<T>&				p2,
			 TAngle<T>&					phi2)
			throw();

		void endEdges
			(TSESEdge<T>*				edge,
			 TSESEdge<T>*				edge1,
			 TSESEdge<T>*				edge2,
			 TSESEdge<T>*				edge3,
			 TSESEdge<T>*				edge4,
			 bool&							big_edge1,
			 bool&							big_edge2,
			 bool&							big_edge3,
			 bool&							big_edge4,
			 const TSphere3<T>& probe1,
			 const TSphere3<T>& probe2,
			 const TSphere3<T>& min_probe,
			 const TSphere3<T>& max_probe,
			 Position						face1,
			 Position						face2,
			 Position						min,
			 Position						max,
			 TSESVertex<T>*			ns1,
			 TSESVertex<T>*			ns2)
			throw(Exception::GeneralException);

		bool getIntersection
			(const TSphere3<T>& probe,
			 Position						probe_index,
			 TSESEdge<T>*				edge,
			 TVector3<T>&				p1,
			 TVector3<T>&				p2)
			throw();

		//@}


		//protected:
		public:

		Position number_of_vertices_;
		::std::vector<TSESVertex<T>*> vertices_;
		HashGrid3<Index> vertex_grid_;
		Position number_of_edges_;
		::std::vector<TSESEdge<T>*> edges_;
		Position number_of_singular_edges_;
		::std::list<TSESEdge<T>*> singular_edges_;
		Position number_of_contact_faces_;
		::std::vector<TSESFace<T>*> contact_faces_;
		Position number_of_toric_faces_;
		::std::vector<TSESFace<T>*> toric_faces_;
		Position number_of_spheric_faces_;
		::std::vector<TSESFace<T>*> spheric_faces_;
		TReducedSurface<T>* reduced_surface_;
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
							if (face->vertex_[0] == face->vertex_[2])
							{
								if (face->edge_[0]->rsedge_->phi_.value < Constants::PI)
								{
									deleteSmallSingularToricFace(face);
									done = false;
								}
								else
								{
									face->edge_[0]->rsedge_->phi_.value = 2*Constants::PI;
								}
							}
							else
							{
								if (face->vertex_[3] == face->vertex_[5])
								{
									if (face->edge_[3]->rsedge_->phi_.value < Constants::PI)
									{
										deleteSmallSingularToricFace(face);
										done = false;
									}
									else
									{
										face->edge_[3]->rsedge_->phi_.value = 2*Constants::PI;
									}
								}
								else
								{
									T exact_number_of_segments(face->rsedge_->phi_.value*
																						 face->edge_[3]->circle_.radius*
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
							if (face->vertex_[0] == face->vertex_[3])
							{
								if (face->edge_[3]->rsedge_->phi_.value < Constants::PI)
								{
									deleteSmallToricFace(face);
									done = false;
								}
								else
								{
									face->edge_[3]->rsedge_->phi_.value = 2*Constants::PI;
								}
							}
							else
							{
								if (face->vertex_[1] == face->vertex_[2])
								{
									if (face->edge_[1]->rsedge_->phi_.value < Constants::PI)
									{
										deleteSmallToricFace(face);
										done = false;
									}
									else
									{
										face->edge_[1]->rsedge_->phi_.value = 2*Constants::PI;
									}
								}
								else
								{
									T exact_number_of_segments(face->rsedge_->phi_.value*
																						 face->edge_[3]->circle_.radius*
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
		face->number_of_vertices_++;
		vertex->faces_.insert(face);
		// Get the RSEdges of the corresponding RSVertex ...
		TRSEdge<T>* tf1;
		TRSEdge<T>* tf2;
		face->rsface_->getEdges(rsvertex,tf1,tf2);
		// ... and push the vertex to these toric faces.
		toric_faces_[tf1->index_]->vertex_.push_back(vertex);
		toric_faces_[tf1->index_]->number_of_vertices_++;
		vertex->faces_.insert(toric_faces_[tf1->index_]);
		toric_faces_[tf2->index_]->vertex_.push_back(vertex);
		toric_faces_[tf2->index_]->number_of_vertices_++;
		vertex->faces_.insert(toric_faces_[tf2->index_]);
		// Push the vertex to the contact face of the corresponding RSVertex ...
		contact_faces_[rsvertex->index_]->vertex_.push_back(vertex);
		contact_faces_[rsvertex->index_]->number_of_vertices_++;
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
		face->number_of_edges_++;
		toric_faces_[index]->edge_.push_back(edge);
		toric_faces_[index]->number_of_edges_++;
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
		edge->vertex_[0] = spheric_face->vertex_[p1];
		edge->vertex_[1] = spheric_face->vertex_[p2];
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
		//   get the third vertex of the face
		//TVector3<T> test_point = spheric_face->vertex_[3-p1-p2]->point_;
		//if (Maths::isLess((test_point*normal),(rsface->center_*normal)))
		//{
		//	normal.negate();
		//}
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
				face->number_of_edges_++;
				contact_faces_[index]->edge_.push_back(edge1);
				contact_faces_[index]->number_of_edges_++;
				edge1->vertex_[0]->edges_.insert(edge1);
				edge1->vertex_[1]->edges_.insert(edge1);
				edges_.push_back(edge1);
				number_of_edges_++;
			rsvertex = face->rsedge_->vertex_[1];
			index = rsvertex->index_;
			TSESEdge<T>* edge2;
			edge2 = createConvexEdge(face,rsvertex);
				face->edge_.push_back(edge2);
				face->number_of_edges_++;
				contact_faces_[index]->edge_.push_back(edge2);
				contact_faces_[index]->number_of_edges_++;
				edge2->vertex_[0]->edges_.insert(edge2);
				edge2->vertex_[1]->edges_.insert(edge2);
				edges_.push_back(edge2);
				number_of_edges_++;
			if (Maths::isEqual(face->rsedge_->phi_.value,Constants::PI))
			{
cout << "Winkel von toroidaler Fläche Nummer " << face->index_ << " ist Pi " << face->rsedge_->phi_ << "\n";
cout << "konvexe Kanten: " << *edge1 << "\n                " << edge2 << "\n";
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
cout << "erste umgedreht:  " << *edge1 << "\n";
				}
				middle = edge2->circle_.n%(edge2->vertex_[0]->point_-edge2->circle_.p);
				middle.normalize();
				middle *= edge2->circle_.radius;
				middle += edge2->circle_.p;
				if (test*(plane.n*(middle-plane.p)) > 0)
				{
					edge2->revert();
cout << "zweite umgedreht: " << *edge2 << "\n";
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
		Position i = 0;
		// find the first vertex of the toric face which lies on the surface of the rsvertex
		while (toric_face->vertex_[i]->atom_ != atom)
		{
			i++;
		}
		edge->vertex_[0] = toric_face->vertex_[i];
		// find the second vertex of the toric face which lies on the surface of the rsvertex
		i++;
		while (toric_face->vertex_[i]->atom_ != atom)
		{
			i++;
		}
		edge->vertex_[1] = toric_face->vertex_[i];
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
		TAngle<T> test_phi(getOrientedAngle(v1,v2,n));
TAngle<T> tf(getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
															edge->vertex_[1]->point_-edge->circle_.p,
															edge->circle_.n));
		if ((test_phi.value-(double)Constants::PI)*((double)edge->rsedge_->phi_.value-(double)Constants::PI) < 0)
		{
			if (Maths::abs(test_phi.value-(double)edge->rsedge_->phi_.value) <
					Maths::abs((double)Constants::PI-(double)edge->rsedge_->phi_.value))
			{
				cout << "    different results!\n";
			}
			edge->revert();
		}
		else
		{
			if (Maths::abs(test_phi.value-(double)edge->rsedge_->phi_.value) >
					Maths::abs((double)Constants::PI-(double)edge->rsedge_->phi_.value))
			{
				cout << "    different results!\n";
			}
		}
if ((test_phi.value-(double)Constants::PI)*((double)edge->rsedge_->phi_.value-(double)Constants::PI) < 0)
{
	if ((tf.value-Constants::PI)*(edge->rsedge_->phi_.value-Constants::PI) >= 0)
	{
		cout << "    different result with double in tf" << toric_face->index_ << "!\n";
	}
}
else
{
	if ((tf.value-Constants::PI)*(edge->rsedge_->phi_.value-Constants::PI) < 0)
	{
		cout << "    different result with double in tf" << toric_face->index_ << "!\n";
	}
}
		return edge;
	}


	template <typename T>
	void TSolventExcludedSurface<T>::treatSingularToricFace(Position i)
		throw()
	{
		TSESFace<T>* face = toric_faces_[i];
		face->normalize(false);
		TSESEdge<T>* edge0 = face->edge_[0];
		TSESEdge<T>* edge1 = face->edge_[1];
		TSESEdge<T>* edge2 = face->edge_[2];
		TSESEdge<T>* edge3 = face->edge_[3];
		TSESVertex<T>* p0 = face->vertex_[0];
		TSESVertex<T>* p1 = face->vertex_[1];
		TSESVertex<T>* p2 = face->vertex_[2];
		TSESVertex<T>* p3 = face->vertex_[3];
		Position j = 0;
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
		new_edge0->vertex_[0] = p0;
		new_edge0->vertex_[1] = new_point3;
		new_edge0->index_ = number_of_edges_;
		new_edge0->vertex_[0]->edges_.insert(new_edge0);
		new_edge0->vertex_[1]->edges_.insert(new_edge0);
		edges_.push_back(new_edge0);
		number_of_edges_++;
		TSESEdge<T>* new_edge1 = new TSESEdge<T>(*edge0,true);
		new_edge1->vertex_[0] = p1;
		new_edge1->vertex_[1] = new_point1;
		new_edge1->index_ = number_of_edges_;
		new_edge1->vertex_[0]->edges_.insert(new_edge1);
		new_edge1->vertex_[1]->edges_.insert(new_edge1);
		edges_.push_back(new_edge1);
		number_of_edges_++;
		TSESEdge<T>* new_edge2 = new TSESEdge<T>(*edge2,true);
		new_edge2->vertex_[0] = p2;
		new_edge2->vertex_[1] = new_point1;
		new_edge2->index_ = number_of_edges_;
		new_edge2->vertex_[0]->edges_.insert(new_edge2);
		new_edge2->vertex_[1]->edges_.insert(new_edge2);
		edges_.push_back(new_edge2);
		number_of_edges_++;
		TSESEdge<T>* new_edge3 = new TSESEdge<T>(*edge2,true);
		new_edge3->vertex_[0] = p3;
		new_edge3->vertex_[1] = new_point3;
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
		for (j = 0; j < neighbour0->edge_.size(); j++)
		{
			if (neighbour0->edge_[j] == edge0)
			{
				neighbour0->edge_[j] = NULL;
			}
		}
		for (j = 0; j < neighbour2->edge_.size(); j++)
		{
			if (neighbour2->edge_[j] == edge2)
			{
				neighbour2->edge_[j] = NULL;
			}
		}
		neighbour0->edge_.push_back(new_edge0);
		neighbour0->edge_.push_back(new_edge1);
		neighbour0->edge_.push_back(new_edge4);
		neighbour0->number_of_edges_ += 3;
		neighbour2->edge_.push_back(new_edge2);
		neighbour2->edge_.push_back(new_edge3);
		neighbour2->edge_.push_back(new_edge4);
		neighbour2->number_of_edges_ += 3;
		neighbour0->vertex_.push_back(new_point1);
		neighbour0->vertex_.push_back(new_point3);
		neighbour0->number_of_vertices_ += 2;
		neighbour2->vertex_.push_back(new_point1);
		neighbour2->vertex_.push_back(new_point3);
		neighbour2->number_of_vertices_ += 2;
		// update the toric face
		::std::vector<TSESVertex<T>*> face_vertices(6);
		::std::vector<TSESEdge<T>*> face_edges(6);
		face_vertices[0] = p0;
		face_vertices[1] = p1;
		face_vertices[2] = p2;
		face_vertices[3] = p3;
		face_vertices[4] = new_point1;
		face_vertices[5] = new_point3;
		face_edges[0] = edge1;
		face_edges[1] = edge3;
		face_edges[2] = new_edge0;
		face_edges[3] = new_edge1;
		face_edges[4] = new_edge2;
		face_edges[5] = new_edge3;
		TSESFace<T>* new_face =
				new TSESFace<T>(TSESFace<T>::TYPE_TORIC_SINGULAR,NULL,face->rsedge_,NULL,
												face_vertices,face_edges,face->index_);
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
				HINFile hinfile("DATA/SES/singularEdge"+IndexToString(new_edge4->index_,0)+".hin",ios::out);
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
			face->number_of_edges_++;
			contact_faces_[index1]->edge_.push_back(edge);
			contact_faces_[index1]->number_of_edges_++;
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
			face->number_of_edges_++;
			contact_faces_[index2]->edge_.push_back(edge);
			contact_faces_[index2]->number_of_edges_++;
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
		TSESEdge<T>* edge0 = face->edge_[0];
		TSESEdge<T>* edge1 = face->edge_[1];
		TSESEdge<T>* edge2 = face->edge_[2];
		TSESEdge<T>* edge3 = face->edge_[3];
		TSESVertex<T>* p0 = face->vertex_[0];
		TSESVertex<T>* p1 = face->vertex_[1];
		TSESVertex<T>* p2 = face->vertex_[2];
		TSESVertex<T>* p3 = face->vertex_[3];
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
		typename std::vector<TSESEdge<T>*>::iterator e = neighbour1->edge_.begin();
		while (*e != edge1)
		{
			e++;
		}
		neighbour1->edge_.erase(e);
		neighbour1->number_of_edges_--;
		typename std::vector<TSESVertex<T>*>::iterator p = neighbour1->vertex_.begin();
		while (*p != p1)
		{
			p++;
		}
		neighbour1->vertex_.erase(p);
		neighbour1->number_of_vertices_--;
		e = neighbour3->edge_.begin();
		while (*e != edge3)
		{
			e++;
		}
		neighbour3->edge_.erase(e);
		neighbour3->number_of_edges_--;
		p = neighbour3->vertex_.begin();
		while (*p != p0)
		{
			p++;
		}
		neighbour3->vertex_.erase(p);
		neighbour3->number_of_vertices_--;
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
		TSESEdge<T>* edge0 = face->edge_[0];
		TSESEdge<T>* edge1 = face->edge_[1];
		TSESEdge<T>* edge2 = face->edge_[2];
		TSESEdge<T>* edge3 = face->edge_[3];
		TSESEdge<T>* edge4 = face->edge_[4];
		TSESEdge<T>* edge5 = face->edge_[5];
		TSESVertex<T>* p0 = face->vertex_[0];
		TSESVertex<T>* p1 = face->vertex_[1];
		TSESVertex<T>* p2 = face->vertex_[2];
		TSESVertex<T>* p3 = face->vertex_[3];
		TSESVertex<T>* p4 = face->vertex_[4];
		TSESVertex<T>* p5 = face->vertex_[5];
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
		typename std::vector<TSESEdge<T>*>::iterator e = neighbour0->edge_.begin();
		while (*e != edge0)
		{
			e++;
		}
		neighbour0->edge_.erase(e);
		neighbour0->number_of_edges_--;
		typename std::vector<TSESVertex<T>*>::iterator p = neighbour0->vertex_.begin();
		while (*p != p0)
		{
			p++;
		}
		neighbour0->vertex_.erase(p);
		neighbour0->number_of_vertices_--;
		e = neighbour3->edge_.begin();
		while (*e != edge3)
		{
			e++;
		}
		neighbour3->edge_.erase(e);
		neighbour3->number_of_edges_--;
		p = neighbour3->vertex_.begin();
		while (*p != p3)
		{
			p++;
		}
		neighbour3->vertex_.erase(p);
		neighbour3->number_of_vertices_--;
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
				#ifdef debug_singularities
				STOP_SINGULARITIES = 0;
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
				Position intersections = 0;
				for (Position i = 0; i < 3; i++)
				{
					if (face1->edge_[i] == NULL)
					{
						intersections++;
					}
				}
				switch (intersections)
				{
					case 0 :	noCut(face1,face2);
										break;
					case 1 :	break;
					case 2 :	twoCuts(face1,face2);
										break;
					case 3 :	reduced_surface_->deleteSimilarFaces(face1->rsface_,face2->rsface_);
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
			face1->number_of_edges_++;
			face2->edge_.push_back(edge);
			face2->number_of_edges_++;
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
		sesvertex1[0] = face1->vertex_[0];
		sesvertex1[1] = face1->vertex_[1];
		sesvertex1[2] = face1->vertex_[2];
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
		for (Position i = 0; i < 3; i++)
		{
			for (Position j = 0; j < 3; j++)
			{
				if (face2->vertex_[j]->atom_ == sesvertex1[i]->atom_)
				{
					sesvertex2[i] = face2->vertex_[j];
				}
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
		face1->number_of_edges_++;
		face2->edge_.push_back(new_edge1);
		face2->number_of_edges_++;
		sesvertex1[2]->edges_.insert(new_edge1);
		sesvertex1[6]->edges_.insert(new_edge1);
		number_of_edges_++;
		circle.n.negate();
		TSESEdge<T>* new_edge2 = new TSESEdge<T>(sesvertex1[3],sesvertex1[5],face1,face2,
																						 circle,NULL,TSESEdge<T>::TYPE_SINGULAR,
																						 number_of_edges_);
		edges_.push_back(new_edge2);
		singular_edges_.push_back(new_edge2);
		face1->edge_.push_back(new_edge2);
		face1->number_of_edges_++;
		face2->edge_.push_back(new_edge2);
		face2->number_of_edges_++;
		sesvertex1[3]->edges_.insert(new_edge2);
		sesvertex1[5]->edges_.insert(new_edge2);
		number_of_edges_++;
		if (sesedge1[2] == sesedge2[2])
		{
			edges_[sesedge1[2]->index_] = NULL;
			singular_edges_.remove(sesedge1[2]);
			sesvertex1[2]->edges_.erase(sesedge1[2]);
			sesvertex1[3]->edges_.erase(sesedge1[2]);
			face1->edge_[face1->getRelativeEdgeIndex(sesedge1[2])] = NULL;
			face2->edge_[face2->getRelativeEdgeIndex(sesedge1[2])] = NULL;
			delete sesedge1[2];
						sesedge1[2] = NULL;
		}
		else
		{
			/*TSESFace<T>* neighbour1 = sesedge1[2]->otherFace(face1);
			TSESFace<T>* neighbour2 = sesedge2[2]->otherFace(face2);
			sphere1.p = neighbour1->rsface_->center_;
			sphere2.p = neighbour2->rsface_->center_;
			GetIntersection(sphere1,sphere2,sesedge1[2]->circle_);
			TAngle<T> phi(getOrientedAngle(sesedge1[2]->vertex_[0]->point_-sesedge1[2]->circle_.p,
																		 sesedge1[2]->vertex_[1]->point_-sesedge1[2]->circle_.p,
																		 sesedge1[2]->circle_.n));
			TAngle<T> psi;
			reduced_surface_->getAngle(neighbour1->rsface_,neighbour2->rsface_,psi);
			if (psi.value > Constants::PI)
			{
				if (phi.value < Constants::PI)
				{
					sesedge1[2]->circle_.n.negate();
				}
			}
			else
			{
				if (phi.value > Constants::PI)
				{
					sesedge1[2]->circle_.n.negate();
				}
			}
			sesedge1[2]->rsedge_ = NULL;
			sesedge1[2]->face_[0] = neighbour1;
			sesedge1[2]->face_[1] = neighbour2;
			edges_[sesedge2[2]->index_] = NULL;
			singular_edges_.remove(sesedge2[2]);
			sesvertex1[2]->edges_.erase(sesedge2[2]);
			sesvertex1[3]->edges_.erase(sesedge2[2]);
			neighbour2->edge_[neighbour2->getRelativeEdgeIndex(sesedge2[2])] = sesedge1[2];
			delete sesedge2[2];*/
		}
		if (sesedge1[5] == sesedge2[5])
		{
			edges_[sesedge1[5]->index_] = NULL;
			singular_edges_.remove(sesedge1[5]);
			sesvertex1[5]->edges_.erase(sesedge1[5]);
			sesvertex1[6]->edges_.erase(sesedge1[5]);
			face1->edge_[face1->getRelativeEdgeIndex(sesedge1[5])] = NULL;
			face2->edge_[face2->getRelativeEdgeIndex(sesedge1[5])] = NULL;
			delete sesedge1[5];
						sesedge1[5] = NULL;
		}
		else
		{
			/*TSESFace<T>* neighbour1 = sesedge1[5]->otherFace(face1);
			TSESFace<T>* neighbour2 = sesedge2[5]->otherFace(face2);
			sphere1.p = neighbour1->rsface_->center_;
			sphere2.p = neighbour2->rsface_->center_;
			GetIntersection(sphere1,sphere2,sesedge1[5]->circle_);
			TAngle<T> phi(getOrientedAngle(sesedge1[5]->vertex_[0]->point_-sesedge1[5]->circle_.p,
																		 sesedge1[5]->vertex_[1]->point_-sesedge1[5]->circle_.p,
																		 sesedge1[5]->circle_.n));
			TAngle<T> psi;
			reduced_surface_->getAngle(neighbour1->rsface_,neighbour2->rsface_,psi);
			if (psi.value > Constants::PI)
			{
				if (phi.value < Constants::PI)
				{
					sesedge1[5]->circle_.n.negate();
				}
			}
			else
			{
				if (phi.value > Constants::PI)
				{
					sesedge1[5]->circle_.n.negate();
				}
			}
			sesedge1[5]->rsedge_ = NULL;
			sesedge1[5]->face_[0] = neighbour1;
			sesedge1[5]->face_[1] = neighbour2;
			edges_[sesedge2[5]->index_] = NULL;
			singular_edges_.remove(sesedge2[5]);
			sesvertex1[5]->edges_.erase(sesedge2[5]);
			sesvertex1[6]->edges_.erase(sesedge2[5]);
			neighbour2->edge_[neighbour2->getRelativeEdgeIndex(sesedge2[5])] = sesedge1[5];
			delete sesedge2[5];*/
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
		::std::vector<TSESFace<T>*> faces;
		::std::vector< TVector3<T> > points;
		HashSet<Index> indices;
		for (Position i = 0; i != number_of_spheric_faces_; i++)
		{
			faces.push_back(spheric_faces_[i]);
			points.push_back(spheric_faces_[i]->rsface_->center_);
			indices.insert(i);
		}
		TBSDTree<T>* tree = new TBSDTree<T>(points,indices);
		tree->build();
		typename ::std::list<TSESEdge<T>*>::iterator edge;
		::std::list<TSESEdge<T>*> deletable_edges;
		for (edge = singular_edges_.begin(); edge != singular_edges_.end(); edge++)
		{
			treatSingularEdge(*edge,faces,tree,deletable_edges);
		}
		for (edge = deletable_edges.begin(); edge != deletable_edges.end(); edge++)
		{
			(*edge)->face_[0]->edge_[(*edge)->face_[0]->getRelativeEdgeIndex(*edge)] = NULL;
			(*edge)->face_[1]->edge_[(*edge)->face_[1]->getRelativeEdgeIndex(*edge)] = NULL;
			(*edge)->vertex_[0]->edges_.erase(*edge);
			(*edge)->vertex_[1]->edges_.erase(*edge);
			edges_[(*edge)->index_] = NULL;
			singular_edges_.remove(*edge);
			delete *edge;
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::treatSingularEdge
		(TSESEdge<T>* edge,
		 ::std::vector<TSESFace<T>*> faces,
		 TBSDTree<T>* tree,
		 ::std::list<TSESEdge<T>*>& deletable_edges)
		throw()
	{
				#ifdef debug_singularities
				ofstream sesfile("ses.log");
				sesfile << *this;
				sesfile.close();
				buildHINFiles();
				std::cout << "TreatSingularEdge( " << *edge << " , [" << faces.size() << "] , tree)  ";
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		if (edge->vertex_[0] == NULL)
		{
			return;
		}

				#ifdef print_ses
				Molecule* molecule = new Molecule;
				partitionSingularEdge(edge,10,molecule);
				System* system = new System;
				system->insert(*molecule);
				HINFile hinfile("DATA/SES/edge"+IndexToString(edge->index_,(T)0)+".hin",ios::out);
				hinfile << *system;
				hinfile.close();
				delete system;
				#endif

		T radius_of_probe = reduced_surface_->probe_radius_;
		HashSet<Index> candidates;
		tree->get(edge->circle_.p,edge->circle_.radius+radius_of_probe,candidates);
		if (candidates.size() == 0)
		{
					#ifdef debug_singularities
					std::cout << "end";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						 else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
			return;
		}
				#ifdef debug_singularities
				//std::cout << "  Drehvektor: " << normal;
				std::cout << "  Drehvektor: " << edge->circle_.n;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		TAngle<T> phi(getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
																	 edge->vertex_[1]->point_-edge->circle_.p,
																	 edge->circle_.n));
				#ifdef debug_singularities
				std::cout << "  Winkel der Edge: " << phi;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		Index face1 = edge->face_[0]->index_;
		Index face2 = edge->face_[1]->index_;
		TSphere3<T> probe1(edge->face_[0]->rsface_->center_,radius_of_probe);
		TSphere3<T> probe2(edge->face_[1]->rsface_->center_,radius_of_probe);
		TAngle<T> min_phi(phi,true);
		TAngle<T> max_phi(0,true);
		TAngle<T> min_of_max;
		TAngle<T> max_of_min;
		TVector3<T> min_point;
		TVector3<T> max_point;
		TSphere3<T> min_probe;
		TSphere3<T> max_probe;
		Index min = -1;
		Index max = -1;
		typename HashSet<Index>::Iterator i;
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
			if ((*i != face1) && (*i != face2))
			{
				probe.p = faces[*i]->rsface_->center_;
						#ifdef debug_singularities
						std::cout << "  Schneide edge mit probe " << *i << " (" << probe << ") ...\n";
						#endif
				if (getIntersectionPointsAndAngles(edge,probe,faces[*i]->index_,p1,phi1,p2,phi2))
				{
							#ifdef debug_singularities
							std::cout << "    " << p1 << "  " << phi1 << "\n";
							std::cout << "    " << p2 << "  " << phi2 << "\n";
							#endif
					bool is_zero = Maths::isZero(phi1.value);
					bool is_phi = (phi2 == phi);
					bool new_min = (phi1 < min_phi);
					bool new_max = (phi2 > max_phi);
					bool equal_min = (phi1 == min_phi);
					bool equal_max = (phi2 == max_phi);
					bool is_min = (new_min || equal_min) && (phi1 < phi);
					bool is_max = (new_max || equal_max) && (phi2 <= phi);
/*std::cout << "is_zero:   " << (is_zero ? "true" : "false") << "\n";
std::cout << "is_phi:    " << (is_phi ? "true" : "false") << "\n";
std::cout << "new_min:   " << (new_min ? "true" : "false") << "\n";
std::cout << "new_max:   " << (new_max ? "true" : "false") << "\n";
std::cout << "equal_min: " << (equal_min ? "true" : "false") << "\n";
std::cout << "equal_max: " << (equal_max ? "true" : "false") << "\n";
std::cout << "is_min:    " << (is_min ? "true" : "false") << "\n";
std::cout << "is_max:    " << (is_max ? "true" : "false") << "\n";*/
					if (is_zero && is_phi)
					{
								#ifdef debug_singularities
								cout << "    ... teste exakt";
								#endif
						TQuaternion<T> rotate(edge->circle_.n,phi/2);
						TMatrix4x4<T> rotation;
						rotate.getRotationMatrix(rotation);
						TVector4<T> v1(edge->vertex_[0]->point_.x-edge->circle_.p.x,
													 edge->vertex_[0]->point_.y-edge->circle_.p.y,
													 edge->vertex_[0]->point_.z-edge->circle_.p.z,
													 0);
						v1 = rotation*v1;
						if (probe.has(TVector3<T>(v1.x+edge->circle_.p.x,
																			v1.y+edge->circle_.p.y,
																			v1.z+edge->circle_.p.z)))
						{
							/*min_phi = phi1;
							max_of_min = phi2;
							min_point = p1;
							min_probe = probe;
							min = (Index)*i;
							max_phi = phi2;
							min_of_max = phi1;
							max_point = p2;
							max_probe = probe;
							max = (Index)*i;*/
						}
						else
						{
									#ifdef debug_singularities
									std::cout << " ... no new min, no new max\n";
									#endif
							is_min = false;
							is_max = false;
						}
					}
					else
					{
						if (is_zero && (phi2 > phi))
						{
							is_min = false;
							is_max = false;
						}
						if (!new_min && (!equal_min || !new_max))
						{
							is_min = false;
						}
						if (!new_max && (!equal_max || !new_min))
						{
							is_max = false;
						}
					}
					if (is_min)
					{
						min_phi = phi1;
						max_of_min = phi2;
						min_point = p1;
						min_probe = probe;
						min = (Index)*i;
								#ifdef debug_singularities
								std::cout << "    ... new min";
								#endif
					}
					if (is_max)
					{
						max_phi = phi2;
						min_of_max = phi1;
						max_point = p2;
						max_probe = probe;
						max = (Index)*i;
								#ifdef debug_singularities
								if ((new_min || (equal_min && new_max)) && is_min)
								{
									std::cout << ", new max\n";
								}
								else
								{
									std::cout << "    ... new max\n";
								}
								#endif
					}
					#ifdef debug_singularities
					if (!is_max)
					{
						std::cout << "\n";
					}
					#endif
				}
							#ifdef debug_singularities
							else
							{
								std::cout << "    ... kein Schnitt\n";
							}
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
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		if (min_phi > max_phi)
		{
			return;
		}
		TSESVertex<T>* ns1;
		TSESVertex<T>* ns2;
		TSESEdge<T>* a = NULL;
		TSESEdge<T>* na = NULL;
		bool new_ns1 = false;
		bool new_ns2 = false;
		if (Maths::isEqual(min_phi.value,0))
		{
			ns1 = edge->vertex_[0];
			ns1->faces_.insert(spheric_faces_[min]);
		}
		else
		{
			Index test = vertexExists(min_point);
			if (test == -1)
			{
				ns1 = new TSESVertex<T>(min_point,edge->circle_.p-min_point,-2,-1);
				ns1->normal_.normalize();
				new_ns1 = true;
			}
			else
			{
				ns1 = vertices_[test];
			}
			if (ns1 != edge->vertex_[0])
			{
				a = new TSESEdge<T>(*edge,true);
				a->vertex_[1] = ns1;
				a->rsedge_ = NULL;
				ns1->faces_.insert(edge->face_[0]);
				ns1->faces_.insert(edge->face_[1]);
				ns1->faces_.insert(spheric_faces_[min]);
				ns1->edges_.insert(a);
				a->vertex_[0]->faces_.insert(spheric_faces_[min]);
				a->vertex_[0]->edges_.insert(a);
			}
		}
		if (Maths::isEqual(max_phi.value,phi.value))
		{
			ns2 = edge->vertex_[1];
			ns2->faces_.insert(spheric_faces_[max]);
		}
		else
		{
			Index test = vertexExists(max_point);
			if (test == -1)
			{
				ns2 = new TSESVertex<T>(max_point,edge->circle_.p-max_point,-2,-1);
				ns2->normal_.normalize();
				new_ns2 = true;
			}
			else
			{
				ns2 = vertices_[test];
			}
			if (ns2 != edge->vertex_[1])
			{
				na = new TSESEdge<T>(*edge,true);
				na->vertex_[0] = ns2;
				na->rsedge_ = NULL;
				ns2->faces_.insert(edge->face_[0]);
				ns2->faces_.insert(edge->face_[1]);
				ns2->faces_.insert(spheric_faces_[max]);
				ns2->edges_.insert(na);
				na->vertex_[0]->faces_.insert(spheric_faces_[min]);
				na->vertex_[0]->edges_.insert(na);
			}
		}
		TCircle3<T> circle;
		TSESEdge<T>* a1 = NULL;
		TSESEdge<T>* a2 = NULL;
		TSESEdge<T>* a3 = NULL;
		TSESEdge<T>* a4 = NULL;
				#ifdef debug_singularities
				std::cout << "  probe1: " << probe1 << "\n";
				std::cout << "  probe2: " << probe2 << "\n";
				std::cout << "  ns1: " << *ns1 << "\n";
				std::cout << "  ns2: " << *ns2 << "\n  a:   ";
				if (a == NULL) std::cout << "(nil)\n  na:  "; else std::cout << *a << "\n  na:  ";
				if (na == NULL) std::cout << "(nil)\n"; else std::cout << *na << "\n";
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		bool big_a1;
		bool big_a2;
		bool big_a3;
		bool big_a4;
		if (min == max)
		{
			TAngle<T> small(Constants::PI/9,true);
			if (edge->face_[0]->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe1,min_probe,circle);
				if (((probe1.p-edge->circle_.p)*edge->circle_.n)*((probe1.p-circle.p)*circle.n) < 0)
				{
					circle.n.negate();
							#ifdef debug_singularities
							cout << "negiert\n";
							#endif
				}
				a1 = new TSESEdge<T>(ns1,ns2,edge->face_[0],faces[min],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
				big_a1 = (getOrientedAngle(ns1->point_-circle.p,ns2->point_-circle.p,circle.n) > small);
				ns1->edges_.insert(a1);
				ns2->edges_.insert(a1);
			}
			if (edge->face_[1]->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe2,min_probe,circle);
				if (((probe2.p-edge->circle_.p)*edge->circle_.n)*((probe2.p-circle.p)*circle.n) < 0)
				{
					circle.n.negate();
							#ifdef debug_singularities
							cout << "negiert\n";
							#endif
				}
				a2 = new TSESEdge<T>(ns1,ns2,edge->face_[1],faces[min],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
				big_a2 = (getOrientedAngle(ns1->point_-circle.p,ns2->point_-circle.p,circle.n) > small);
				ns1->edges_.insert(a2);
				ns2->edges_.insert(a2);
			}
		}
		else
		{
			if (edge->face_[0]->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe1,min_probe,circle);
				a1 = new TSESEdge<T>(ns1,NULL,edge->face_[0],faces[min],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
			}
			if (edge->face_[1]->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe2,min_probe,circle);
				a2 = new TSESEdge<T>(ns1,NULL,edge->face_[1],faces[min],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
			}
			if (edge->face_[0]->isNeighbouredTo(faces[max]) == false)
			{
				GetIntersection(probe1,max_probe,circle);
				a3 = new TSESEdge<T>(ns2,NULL,edge->face_[0],faces[max],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
			}
			if (edge->face_[1]->isNeighbouredTo(faces[max]) == false)
			{
				GetIntersection(probe2,max_probe,circle);
				a4 = new TSESEdge<T>(ns2,NULL,edge->face_[1],faces[max],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
			}
			try
			{
std::cout << "löse Singularität auf ... ";
				endEdges(edge,a1,a2,a3,a4,big_a1,big_a2,big_a3,big_a4,
								 probe1,probe2,min_probe,max_probe,
								 edge->face_[0]->index_,edge->face_[1]->index_,min,max,
								 ns1,ns2);
			}
			catch (Exception::GeneralException)
			{
std::cout << "FAILED\n";
				delete a1;
				delete a2;
				delete a3;
				delete a4;
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
				#ifdef debug_singularities
				STOP_SINGULARITIES = 0;
				std::cout << "couldn't end edges\n";
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
				return;
			}
std::cout << "ok\n";
		}
		if (a != NULL)
		{
			a->index_ = number_of_edges_;
			edges_.push_back(a);
			number_of_edges_++;
			singular_edges_.push_front(a);
			number_of_singular_edges_++;
			a->face_[0]->edge_.push_back(a);
			a->face_[0]->number_of_edges_++;
			a->face_[1]->edge_.push_back(a);
			a->face_[1]->number_of_edges_++;
			a->vertex_[0]->edges_.insert(a);
			a->vertex_[1]->edges_.insert(a);
						#ifdef debug_singularities
						std::cout << "  a == " << *a << " != NULL\n";
						#endif
						#ifdef print_ses
						Molecule* molecule = new Molecule;
						partitionSingularEdge(a,10,molecule);
						System* system = new System;
						system->insert(*molecule);
						HINFile hinfile("DATA/SES/edge"+IndexToString(a->index_,(T)0)+".hin",ios::out);
						hinfile << *system;
						hinfile.close();
						delete system;
						#endif
		}
		if (na != NULL)
		{
			na->index_ = number_of_edges_;
			edges_.push_back(na);
			number_of_edges_++;
			singular_edges_.push_front(na);
			number_of_singular_edges_++;
			na->face_[0]->edge_.push_back(na);
			na->face_[0]->number_of_edges_++;
			na->face_[1]->edge_.push_back(na);
			na->face_[1]->number_of_edges_++;
			na->vertex_[0]->edges_.insert(na);
			na->vertex_[1]->edges_.insert(na);
						#ifdef debug_singularities
						std::cout << "  na == " << *na << " != NULL\n";
						#endif
						#ifdef print_ses
						Molecule* molecule = new Molecule;
						partitionSingularEdge(na,10,molecule);
						System* system = new System;
						system->insert(*molecule);
						HINFile hinfile("DATA/SES/edge"+IndexToString(na->index_,(T)0)+".hin",ios::out);
						hinfile << *system;
						hinfile.close();
						delete system;
						#endif
		}
		if (a1 != NULL)
		{
			a1->index_ = number_of_edges_;
			edges_.push_back(a1);
			number_of_edges_++;
			if (big_a1)
			{
				singular_edges_.push_back(a1);
			}
			else
			{
				singular_edges_.push_front(a1);
cout << "#########\n";
			}
			number_of_singular_edges_++;
			a1->face_[0]->edge_.push_back(a1);
			a1->face_[0]->number_of_edges_++;
			a1->face_[1]->edge_.push_back(a1);
			a1->face_[1]->number_of_edges_++;
			a1->vertex_[0]->edges_.insert(a1);
			a1->vertex_[1]->edges_.insert(a1);
					#ifdef debug_singularities
					std::cout << "  a1 == " << *a1 << " != NULL\n";
					#endif
						#ifdef print_ses
						Molecule* molecule = new Molecule;
						partitionSingularEdge(a1,10,molecule);
						System* system = new System;
						system->insert(*molecule);
						HINFile hinfile("DATA/SES/edge"+IndexToString(a1->index_,(T)0)+".hin",ios::out);
						hinfile << *system;
						hinfile.close();
						delete system;
						#endif
		}
		if (a2 != NULL)
		{
			a2->index_ = number_of_edges_;
			edges_.push_back(a2);
			number_of_edges_++;
			if (big_a2)
			{
				singular_edges_.push_back(a2);
			}
			else
			{
				singular_edges_.push_front(a2);
cout << "#########\n";
			}
			number_of_singular_edges_++;
			a2->face_[0]->edge_.push_back(a2);
			a2->face_[0]->number_of_edges_++;
			a2->face_[1]->edge_.push_back(a2);
			a2->face_[1]->number_of_edges_++;
			a2->vertex_[0]->edges_.insert(a2);
			a2->vertex_[1]->edges_.insert(a2);
					#ifdef debug_singularities
					std::cout << "  a2 == " << *a2 << " != NULL\n";
					#endif
						#ifdef print_ses
						Molecule* molecule = new Molecule;
						partitionSingularEdge(a2,10,molecule);
						System* system = new System;
						system->insert(*molecule);
						HINFile hinfile("DATA/SES/edge"+IndexToString(a2->index_,(T)0)+".hin",ios::out);
						hinfile << *system;
						hinfile.close();
						delete system;
						#endif
		}
		if (a3 != NULL)
		{
			a3->index_ = number_of_edges_;
			edges_.push_back(a3);
			number_of_edges_++;
			if (big_a3)
			{
				singular_edges_.push_back(a3);
			}
			else
			{
				singular_edges_.push_front(a3);
cout << "#########\n";
			}
			number_of_singular_edges_++;
			a3->face_[0]->edge_.push_back(a3);
			a3->face_[0]->number_of_edges_++;
			a3->face_[1]->edge_.push_back(a3);
			a3->face_[1]->number_of_edges_++;
			a3->vertex_[0]->edges_.insert(a3);
			a3->vertex_[1]->edges_.insert(a3);
					#ifdef debug_singularities
					std::cout << "  a3 == " << *a3 << " != NULL\n";
					#endif
						#ifdef print_ses
						Molecule* molecule = new Molecule;
						partitionSingularEdge(a3,10,molecule);
						System* system = new System;
						system->insert(*molecule);
						HINFile hinfile("DATA/SES/edge"+IndexToString(a3->index_,(T)0)+".hin",ios::out);
						hinfile << *system;
						hinfile.close();
						delete system;
						#endif
		}
		if (a4 != NULL)
		{
			a4->index_ = number_of_edges_;
			edges_.push_back(a4);
			number_of_edges_++;
			if (big_a4)
			{
				singular_edges_.push_back(a4);
			}
			else
			{
				singular_edges_.push_front(a4);
cout << "#########\n";
			}
			number_of_singular_edges_++;
			a4->face_[0]->edge_.push_back(a4);
			a4->face_[0]->number_of_edges_++;
			a4->face_[1]->edge_.push_back(a4);
			a4->face_[1]->number_of_edges_++;
			a4->vertex_[0]->edges_.insert(a4);
			a4->vertex_[1]->edges_.insert(a4);
					#ifdef debug_singularities
					std::cout << "  a4 == " << *a4 << " != NULL\n";
					#endif
						#ifdef print_ses
						Molecule* molecule = new Molecule;
						partitionSingularEdge(a4,10,molecule);
						System* system = new System;
						system->insert(*molecule);
						HINFile hinfile("DATA/SES/edge"+IndexToString(a4->index_,(T)0)+".hin",ios::out);
						hinfile << *system;
						hinfile.close();
						delete system;
						#endif
		}
		if (new_ns1)
		{
			ns1->index_ = number_of_vertices_;
			vertices_.push_back(ns1);
			vertex_grid_.insert(ns1->point_,ns1->index_);
			number_of_vertices_++;
		}
		if (new_ns2)
		{
			ns2->index_ = number_of_vertices_;
			vertices_.push_back(ns2);
			vertex_grid_.insert(ns2->point_,ns2->index_);
			number_of_vertices_++;
		}
				#ifdef debug_singularities
				std::cout << "edge(" << edge << "):  ";
				if (edge != NULL) std::cout << *edge; else std::cout << "---"; std::cout << "\n";
				std::cout << "a(" << a << "):  ";
				if (a != NULL) std::cout << *a; else std::cout << "---"; std::cout << "\n";
				std::cout << "na(" << na << "):  ";
				if (na != NULL) std::cout << *na; else std::cout << "---"; std::cout << "\n";
				std::cout << "a1(" << a1 << "):  ";
				if (a1 != NULL) std::cout << *a1; else std::cout << "---"; std::cout << "\n";
				std::cout << "a2(" << a2 << "):  ";
				if (a2 != NULL) std::cout << *a2; else std::cout << "---"; std::cout << "\n";
				std::cout << "a3(" << a3 << "):  ";
				if (a3 != NULL) std::cout << *a3; else std::cout << "---"; std::cout << "\n";
				std::cout << "a4(" << a4 << "):  ";
				if (a4 != NULL) std::cout << *a4; else std::cout << "---"; std::cout << "\n";
				std::cout << "ns1(" << ns1 << "):  ";
				if (ns1 != NULL) std::cout << *ns1; else std::cout << "---"; std::cout << "\n";
				std::cout << "ns2(" << ns2 << "):  ";
				if (ns2 != NULL) std::cout << *ns2; else std::cout << "---"; std::cout << "\n";
				STOP_SINGULARITIES = 0;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		deletable_edges.push_back(edge);
	}


	template <class T>
	bool TSolventExcludedSurface<T>::getIntersectionPointsAndAngles
		(TSESEdge<T>*				edge,
		 const TSphere3<T>& probe,
		 Position						probe_index,
		 TVector3<T>&				p1,
		 TAngle<T>&					phi1,
		 TVector3<T>&				p2,
		 TAngle<T>&					phi2)
		throw()
	{
		if (getIntersection(probe,probe_index,edge,p1,p2))
		{
			phi1 = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
															p1-edge->circle_.p,
															edge->circle_.n);
			phi2 = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,
															p2-edge->circle_.p,
															edge->circle_.n);
			if (Maths::isEqual(phi1.value,2*Constants::PI))
			{
				phi1.value = (T)0;
			}
			if (Maths::isEqual(phi2.value,2*Constants::PI))
			{
				phi2.value = (T)0;
			}
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
	void TSolventExcludedSurface<T>::endEdges
		(TSESEdge<T>*				edge,
		 TSESEdge<T>*				edge1,
		 TSESEdge<T>*				edge2,
		 TSESEdge<T>*				edge3,
		 TSESEdge<T>*				edge4,
		 bool&							big_edge1,
		 bool&							big_edge2,
		 bool&							big_edge3,
		 bool&							big_edge4,
		 const TSphere3<T>& probe1,
		 const TSphere3<T>& probe2,
		 const TSphere3<T>& min_probe,
		 const TSphere3<T>& max_probe,
		 Position						face1,
		 Position						face2,
		 Position						min,
		 Position						max,
		 TSESVertex<T>*			ns1,
		 TSESVertex<T>*			ns2)
		throw(Exception::GeneralException)
	{
		double epsilon = Constants::EPSILON;
		Constants::EPSILON = 1e-8;
		if ((probe1.p-edge->circle_.p)*edge->circle_.n > 0)
		{
			// probe1 above singular edge
			if (edge1 != NULL)
			{
				if ((probe1.p-edge1->circle_.p)*edge1->circle_.n < 0)
				{
					// probe1 lower first new edge
					edge1->circle_.n.negate();
				}
			}
			if (edge2 != NULL)
			{
				if ((probe2.p-edge2->circle_.p)*edge2->circle_.n > 0)
				{
					// probe2 above second new edge
					edge2->circle_.n.negate();
				}
			}
			if (edge3 != NULL)
			{
				if ((probe1.p-edge3->circle_.p)*edge3->circle_.n > 0)
				{
					// probe1 above third new edge
					edge3->circle_.n.negate();
				}
			}
			if (edge4 != NULL)
			{
				if ((probe2.p-edge4->circle_.p)*edge4->circle_.n < 0)
				{
					// probe2 lower fourth new edge
					edge4->circle_.n.negate();
				}
			}
		}
		else
		{
			// probe1 lower singular edge
			if (edge1 != NULL)
			{
				if ((probe1.p-edge1->circle_.p)*edge1->circle_.n > 0)
				{
					// probe1 above first new edge
					edge1->circle_.n.negate();
				}
			}
			if (edge2 != NULL)
			{
				if ((probe2.p-edge2->circle_.p)*edge2->circle_.n < 0)
				{
					// probe2 lower second new edge
					edge2->circle_.n.negate();
				}
			}
			if (edge3 != NULL)
			{
				if ((probe1.p-edge3->circle_.p)*edge3->circle_.n < 0)
				{
					// probe1 lower third new edge
					edge3->circle_.n.negate();
				}
			}
			if (edge4 != NULL)
			{
				if ((probe2.p-edge4->circle_.p)*edge4->circle_.n > 0)
				{
					// probe2 above fourth new edge
					edge4->circle_.n.negate();
				}
			}
		}
		TAngle<T> phi1;
		TAngle<T> phi2;
		TAngle<T> small(Constants::PI/9,true);
		TVector3<T> point1;
		TVector3<T> point2;
		TSESVertex<T>* v;
		bool intersecting;
		if (face1 < min)
		{
			if (min < max)
			{
				intersecting = GetIntersection(probe1,min_probe,max_probe,point1,point2,false);
			}
			else
			{
				if (face1 < max)
				{
					intersecting = GetIntersection(probe1,max_probe,min_probe,point1,point2,false);
				}
				else
				{
					intersecting = GetIntersection(max_probe,probe1,min_probe,point1,point2,false);
				}
			}
		}
		else
		{
			if (face1 < max)
			{
				intersecting = GetIntersection(min_probe,probe1,max_probe,point1,point2,false);
			}
			else
			{
				if (min < max)
				{
					intersecting = GetIntersection(min_probe,max_probe,probe1,point1,point2,false);
				}
				else
				{
					intersecting = GetIntersection(max_probe,min_probe,probe1,point1,point2,false);
				}
			}
		}
		if (intersecting)
		{
			if (edge1 != NULL)
			{
				phi1 = getOrientedAngle(ns1->point_-edge1->circle_.p,
																point1-edge1->circle_.p,
																edge1->circle_.n);
				phi2 = getOrientedAngle(ns1->point_-edge1->circle_.p,
																point2-edge1->circle_.p,
																edge1->circle_.n);
				if (phi1 < phi2)
				{
					Index test = vertexExists(point1);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point1;
						v->normal_ = edge1->circle_.p-point1;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge1 = (phi1 > small);
				}
				else
				{
					Index test = vertexExists(point2);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point2;
						v->normal_ = edge1->circle_.p-point2;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge1 = (phi2 > small);
				}
				edge1->vertex_[1] = v;
			}
			if (edge3 != NULL)
			{
				phi1 = getOrientedAngle(ns2->point_-edge3->circle_.p,
																point1-edge3->circle_.p,
																edge3->circle_.n);
				phi2 = getOrientedAngle(ns2->point_-edge3->circle_.p,
																point2-edge3->circle_.p,
																edge3->circle_.n);
				if (phi1 < phi2)
				{
					Index test = vertexExists(point1);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point1;
						v->normal_ = edge3->circle_.p-point1;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge3 = (phi1 > small);
				}
				else
				{
					Index test = vertexExists(point2);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point2;
						v->normal_ = edge3->circle_.p-point2;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge3 = (phi2 > small);
				}
				edge3->vertex_[1] = v;
			}
		}
		else
		{
			Constants::EPSILON = epsilon;
			throw Exception::GeneralException(__FILE__,__LINE__,
																				"CanNotTreatSingularity",
																				"no end-point found for SES-edge");
		}
		if (face2 < min)
		{
			if (min < max)
			{
				intersecting = GetIntersection(probe2,min_probe,max_probe,point1,point2,false);
			}
			else
			{
				if (face2 < max)
				{
					intersecting = GetIntersection(probe2,max_probe,min_probe,point1,point2,false);
				}
				else
				{
					intersecting = GetIntersection(max_probe,probe2,min_probe,point1,point2,false);
				}
			}
		}
		else
		{
			if (face2 < max)
			{
				intersecting = GetIntersection(min_probe,probe2,max_probe,point1,point2,false);
			}
			else
			{
				if (min < max)
				{
					intersecting = GetIntersection(min_probe,max_probe,probe2,point1,point2,false);
				}
				else
				{
					intersecting = GetIntersection(max_probe,min_probe,probe2,point1,point2,false);
				}
			}
		}
		if (intersecting)
		{
			if (edge2 != NULL)
			{
				phi1 = getOrientedAngle(ns1->point_-edge2->circle_.p,
																point1-edge2->circle_.p,
																edge2->circle_.n);
				phi2 = getOrientedAngle(ns1->point_-edge2->circle_.p,
																point2-edge2->circle_.p,
																edge2->circle_.n);
				if (phi1 < phi2)
				{
					Index test = vertexExists(point1);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point1;
						v->normal_ = edge2->circle_.p-point1;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge2 = (phi1 > small);
				}
				else
				{
					Index test = vertexExists(point2);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point2;
						v->normal_ = edge2->circle_.p-point2;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge2 = (phi2 > small);
				}
				edge2->vertex_[1] = v;
			}
			if (edge4 != NULL)
			{
				phi1 = getOrientedAngle(ns2->point_-edge4->circle_.p,
																point1-edge4->circle_.p,
																edge4->circle_.n);
				phi2 = getOrientedAngle(ns2->point_-edge4->circle_.p,
																point2-edge4->circle_.p,
																edge4->circle_.n);
				if (phi1 < phi2)
				{
					Index test = vertexExists(point1);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point1;
						v->normal_ = edge4->circle_.p-point1;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge4 = (phi1 > small);
				}
				else
				{
					Index test = vertexExists(point2);
					if (test == -1)
					{
						v = new TSESVertex<T>;
						v->point_ = point2;
						v->normal_ = edge4->circle_.p-point2;
						v->normal_.normalize();
						v->index_ = number_of_vertices_;
						vertices_.push_back(v);
						vertex_grid_.insert(v->point_,v->index_);
						number_of_vertices_++;
					}
					else
					{
						v = vertices_[test];
					}
					big_edge4 = (phi2 > small);
				}
				edge4->vertex_[1] = v;
			}
		}
		else
		{
			Constants::EPSILON = epsilon;
			throw Exception::GeneralException(__FILE__,__LINE__,
																				"CanNotTreatSingularity",
																				"no end-point found for SES-edge");
		}
		Constants::EPSILON = epsilon;
	}


	template <class T>
	bool TSolventExcludedSurface<T>::getIntersection
		(const TSphere3<T>& probe,
		 Position						probe_index,
		 TSESEdge<T>*				edge,
		 TVector3<T>&				p1,
		 TVector3<T>&				p2)
		throw()
	{
		Position probe_index1 = edge->face_[0]->index_;
		Position probe_index2 = edge->face_[1]->index_;
		TVector3<double> center0((double)probe.p.x,
														 (double)probe.p.y,
														 (double)probe.p.z);
		TVector3<double> center1((double)edge->face_[0]->rsface_->center_.x,
														 (double)edge->face_[0]->rsface_->center_.y,
														 (double)edge->face_[0]->rsface_->center_.z);
		TVector3<double> center2((double)edge->face_[1]->rsface_->center_.x,
														 (double)edge->face_[1]->rsface_->center_.y,
														 (double)edge->face_[1]->rsface_->center_.z);
		TSphere3<double> probe0(center0,probe.radius);
		TSphere3<double> probe1(center1,probe.radius);
		TSphere3<double> probe2(center2,probe.radius);
		TVector3<double> point1;
		TVector3<double> point2;
		bool back;
		if (probe_index < probe_index1)
		{
			if (probe_index1 < probe_index2)
			{
				back = GetIntersection(probe0,probe1,probe2,point1,point2,false);
			}
			else
			{
				if (probe_index < probe_index2)
				{
					back = GetIntersection(probe0,probe2,probe1,point1,point2,false);
				}
				else
				{
					back = GetIntersection(probe2,probe0,probe1,point1,point2,false);
				}
			}
		}
		else
		{
			if (probe_index < probe_index2)
			{
				back = GetIntersection(probe1,probe0,probe2,point1,point2,false);
			}
			else
			{
				if (probe_index1 < probe_index2)
				{
					back = GetIntersection(probe1,probe2,probe0,point1,point2,false);
				}
				else
				{
					back = GetIntersection(probe2,probe1,probe0,point1,point2,false);
				}
			}
		}
		p1.set((T)point1.x,(T)point1.y,(T)point1.z);
		p2.set((T)point2.x,(T)point2.y,(T)point2.z);
		return back;
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
		Constants::EPSILON = epsilon;
		return -1;
	}




  #ifdef print_ses
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
		vector<Molecule*> molecule(number_of_edges_);
		std::cerr << "  build molecules from SES-edges ...\n";
		T sqrt_density = 3;
		for (Position i = 0; i < number_of_edges_; i++)
		{
			if (edges_[i] != NULL)
			{
				molecule[i] = new Molecule;
				SESEdge* edge = edges_[i];
				if (edge->vertex_[0] == NULL)
				{
					partitionFreeEdge(edge,sqrt_density,molecule[i]);
				}
				else
				{
					if (edge->type_ == SESEdge::TYPE_SINGULAR)
					{
						partitionSingularEdge(edge,sqrt_density,molecule[i]);
					}
					else
					{
						partitionEdge(edge,sqrt_density,molecule[i]);
					}
				}
				System* system = new System;
				system->insert(*molecule[i]);
				HINFile hinfile("DATA/SES/edge"+IndexToString(i,0)+".hin",ios::out);
				hinfile << *system;
				hinfile.close();
				delete system;
			}
		}
	}
	#endif

} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H

