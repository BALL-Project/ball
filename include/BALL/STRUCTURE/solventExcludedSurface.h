// $Id: solventExcludedSurface.h,v 1.19 2001/12/08 17:08:18 strobel Exp $

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#define BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H

//#define debug_singularities

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

#ifndef BALL_STRUCTURE_BSDTREE_H
#	include <BALL/STRUCTURE/BSDTree.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#include <vector>
#include <list>


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

		/**	Computes the solvent excluded surface from a ReducedSurface object
		*/
		void compute()
			throw();

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

		void treatSingularEdge
			(TSESEdge<T>* edge,
			 ::std::vector<TSESFace<T>*> faces,
			 TBSDTree<T>* tree)
			throw();

		bool getIntersectionPointsAndAngles
			(TSESEdge<T>*				edge,
			 const TSphere3<T>& probe,
			 const TVector3<T>& normal,
			 TVector3<T>&				p1,
			 TAngle<T>&					phi1,
			 TVector3<T>&				p2,
			 TAngle<T>&					phi2)
			throw();

		void endEdges
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
			throw(Exception::GeneralException);

		bool getIntersection
			(const TSphere3<T>& sphere,
			 const TCircle3<T>& circle,
			 TVector3<T>&				p1,
			 TVector3<T>&				p2)
			throw();

		//@}


		//protected:
		public:

		Position number_of_vertices_;
		::std::vector<TSESVertex<T>*> vertices_;
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
			s << "  " << *ses.vertices_[i] << "\n";
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
	{				// BAUSTELLE !!!
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
	void TSolventExcludedSurface<T>::compute()
		throw()
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
		// Get the RSEdges of the corresponding RSVertex ...
		TRSEdge<T>* tf1;
		TRSEdge<T>* tf2;
		face->rsface_->getEdges(rsvertex,tf1,tf2);
		// ... and push the vertex to these toric faces.
		toric_faces_[tf1->index_]->vertex_.push_back(vertex);
		toric_faces_[tf1->index_]->number_of_vertices_++;
		toric_faces_[tf2->index_]->vertex_.push_back(vertex);
		toric_faces_[tf2->index_]->number_of_vertices_++;
		// Push the vertex to the contact face of the corresponding RSVertex ...
		contact_faces_[rsvertex->index_]->vertex_.push_back(vertex);
		contact_faces_[rsvertex->index_]->number_of_vertices_++;
		// ... and to the verices of the SES.
		vertices_.push_back(vertex);
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
		face->orientation_.push_back(0);
		toric_faces_[index]->edge_.push_back(edge);
		toric_faces_[index]->number_of_edges_++;
		toric_faces_[index]->orientation_.push_back(1);
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
		TVector3<T> normal = (rsface->center_-edge->vertex_[0]->point_)%
												 (rsface->center_-edge->vertex_[1]->point_);
		//   get the third vertex of the face
		TVector3<T> test_point = spheric_face->vertex_[3-p1-p2]->point_;
		if (Maths::isLess((test_point*normal),(rsface->center_*normal)))
		{
			normal.negate();
		}
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
			TSESEdge<T>* edge;
			TRSVertex<T>* rsvertex = face->rsedge_->vertex_[0];
			Index index = rsvertex->index_;
			edge = createConvexEdge(face,rsvertex);
				face->edge_.push_back(edge);
				face->number_of_edges_++;
				face->orientation_.push_back(1);
				contact_faces_[index]->edge_.push_back(edge);
				contact_faces_[index]->number_of_edges_++;
				contact_faces_[index]->orientation_.push_back(0);
				edges_.push_back(edge);
				number_of_edges_++;
			rsvertex = face->rsedge_->vertex_[1];
			index = rsvertex->index_;
			edge = createConvexEdge(face,rsvertex);
				face->edge_.push_back(edge);
				face->number_of_edges_++;
				face->orientation_.push_back(1);
				contact_faces_[index]->edge_.push_back(edge);
				contact_faces_[index]->number_of_edges_++;
				contact_faces_[index]->orientation_.push_back(0);
				edges_.push_back(edge);
				number_of_edges_++;
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
		TSESVertex<T>* new_point1 = new TSESVertex<T>(face->rsedge_->getIntersectionPoint(ip1),
																									TVector3<T>::getZero(),
																									face->rsedge_->getVertex(ip1)->atom_,
																									number_of_vertices_);
		vertices_.push_back(new_point1);
		number_of_vertices_++;
		TSESVertex<T>* new_point3 = new TSESVertex<T>(face->rsedge_->getIntersectionPoint(ip3),
																									TVector3<T>::getZero(),
																									face->rsedge_->getVertex(ip3)->atom_,
																									number_of_vertices_);
		vertices_.push_back(new_point3);
		number_of_vertices_++;
		// create the new edges
		TSESEdge<T>* new_edge0 = new TSESEdge<T>(*edge0,true);
		new_edge0->vertex_[0] = p0;
		new_edge0->vertex_[1] = new_point3;
		new_edge0->index_ = number_of_edges_;
		edges_.push_back(new_edge0);
		number_of_edges_++;
		TSESEdge<T>* new_edge1 = new TSESEdge<T>(*edge0,true);
		new_edge1->vertex_[0] = p1;
		new_edge1->vertex_[1] = new_point1;
		new_edge1->index_ = number_of_edges_;
		edges_.push_back(new_edge1);
		number_of_edges_++;
		TSESEdge<T>* new_edge2 = new TSESEdge<T>(*edge2,true);
		new_edge2->vertex_[0] = p2;
		new_edge2->vertex_[1] = new_point1;
		new_edge2->index_ = number_of_edges_;
		edges_.push_back(new_edge2);
		number_of_edges_++;
		TSESEdge<T>* new_edge3 = new TSESEdge<T>(*edge2,true);
		new_edge3->vertex_[0] = p3;
		new_edge3->vertex_[1] = new_point3;
		new_edge3->index_ = number_of_edges_;
		edges_.push_back(new_edge3);
		number_of_edges_++;
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
		TSESEdge<T>* new_edge4 = new TSESEdge<T>(new_point1,new_point3,neighbour0,neighbour2,
																						 intersection_circle,face->rsedge_,
																						 TSESEdge<T>::TYPE_SINGULAR,number_of_edges_);
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
		neighbour0->orientation_.push_back(0);
		neighbour0->orientation_.push_back(0);
		neighbour2->orientation_.push_back(0);
		neighbour2->orientation_.push_back(0);
		if (Maths::isGreater(new_edge4->circle_.n*neighbour0->rsface_->center_,
												 new_edge4->circle_.n*new_edge4->circle_.p))
		{
			neighbour0->orientation_.push_back(1);
			neighbour2->orientation_.push_back(0);
		}
		else
		{
			neighbour0->orientation_.push_back(0);
			neighbour2->orientation_.push_back(1);
		}
		// update the toric face
		::std::vector<TSESVertex<T>*> face_vertices(6);
		::std::vector<TSESEdge<T>*> face_edges(6);
		::std::vector<bool> orientations(0);
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
												face_vertices,face_edges,orientations,face->index_);
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
		// delete the intersecting edges
		edges_[edge0->index_] = NULL;
		delete edge0;
		edges_[edge2->index_] = NULL;
		delete edge2;
		// replace the old edge by the new
		toric_faces_[face->index_] = new_face;
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
			face->orientation_.push_back(1);
			contact_faces_[index1]->edge_.push_back(edge);
			contact_faces_[index1]->number_of_edges_++;
			contact_faces_[index1]->orientation_.push_back(0);
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
			face->orientation_.push_back(1);
			contact_faces_[index2]->edge_.push_back(edge);
			contact_faces_[index2]->number_of_edges_++;
			contact_faces_[index2]->orientation_.push_back(0);
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
	void TSolventExcludedSurface<T>::treatSingularities()
		throw()
	{
				#ifdef debug_singularities
				STOP_SINGULARITIES = 0;
				#endif
		::std::list<TSESFace<T>*> singular_faces;
		getSingularFaces(singular_faces);
		treatFirstCategory(singular_faces);
		treatSecondCategory();
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
					case 2 :	;
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
		TSESEdge<T>* edge = new TSESEdge<T>(NULL,NULL,face1,face2,circle,NULL,
																				TSESEdge<T>::TYPE_CONCAVE,number_of_edges_);
		edges_.push_back(edge);
		number_of_edges_++;
		face1->edge_.push_back(edge);
		face1->number_of_edges_++;
		face2->edge_.push_back(edge);
		face2->number_of_edges_++;
		if (Maths::isGreater(circle.n*face1->rsface_->center_,circle.n*circle.p))
		{
			face1->orientation_.push_back(0);
			face2->orientation_.push_back(1);
		}
		else
		{
			face1->orientation_.push_back(1);
			face2->orientation_.push_back(0);
		}
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
		typename ::std::list<TSESEdge<T>*>::iterator edge = singular_edges_.begin();
		typename ::std::list<TSESEdge<T>*>::iterator tmp;
		while (edge != singular_edges_.end())
		{
			tmp = edge;
			tmp++;
			treatSingularEdge(*edge,faces,tree);
			edge = tmp;
		}
	}


	template <class T>
	void TSolventExcludedSurface<T>::treatSingularEdge
		(TSESEdge<T>* edge,
		 ::std::vector<TSESFace<T>*> faces,
		 TBSDTree<T>* tree)
		throw()
	{
				#ifdef debug_singularities
				std::cout << "TreatSingularEdge( " << *edge << " , [" << faces.size() << "] , tree)  ";
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
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
		TVector3<T> normal((edge->vertex_[0]->point_-edge->circle_.p)%
											 (edge->vertex_[1]->point_-edge->circle_.p));
				#ifdef debug_singularities
				std::cout << "  Drehvektor: " << normal;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		TAngle<T> phi;
		GetAngle(edge->vertex_[0]->point_-edge->circle_.p,
						 edge->vertex_[1]->point_-edge->circle_.p,
						 phi);
				#ifdef debug_singularities
				std::cout << "  Winkel der Edge: " << phi;
				if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
					 else { STOP_SINGULARITIES--; std::cout << "\n"; }
				#endif
		//TAngle<T> min_phi(Constants::PI,true);
		TAngle<T> min_phi(phi,true);
		TAngle<T> max_phi(0,true);
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
			probe.p = faces[*i]->rsface_->center_;
					#ifdef debug_singularities
					std::cout << "  Schneide edge mit probe " << *i << " (" << probe << ") ... ";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						 else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
			if (getIntersectionPointsAndAngles(edge,probe,normal,p1,phi1,p2,phi2))
			{
						#ifdef debug_singularities
						std::cout << "    " << p1 << "  " << phi1 << "\n";
						std::cout << "    " << p2 << "  " << phi2 << "  ";
						if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							 else { STOP_SINGULARITIES--; std::cout << "\n"; }
						#endif
				if (Maths::isGreater(phi1.value,0) && (phi1 < min_phi))
				{
					min_phi = phi1;
					min_point = p1;
					min_probe = probe;
					min = (Index)*i;
							#ifdef debug_singularities
							std::cout << "    ... new min  ";
							if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
								 else { STOP_SINGULARITIES--; std::cout << "\n"; }
							#endif
				}
				if (Maths::isLess(phi2.value,phi.value) && (phi2 > max_phi))
				{
					max_phi = phi2;
					max_point = p2;
					max_probe = probe;
					max = (Index)*i;
							#ifdef debug_singularities
							std::cout << "    ... new max  ";
							if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
								 else { STOP_SINGULARITIES--; std::cout << "\n"; }
							#endif
				}
			}
						#ifdef debug_singularities
						else
						{
						std::cout << "    ... kein Schnitt  ";
						if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							 else { STOP_SINGULARITIES--; std::cout << "\n"; }
						}
						#endif
		}
		Constants::EPSILON = epsilon;
				#ifdef debug_singularities
				std::cout << "  Edge: " << *edge << "\n";
				if (min != -1)
				{
					//STOP_SINGULARITIES = 0;
					std::cout << "  minimaler Schnitt:\n"
										<< "    Probe: " << min << ":  (" << min_probe << ")\n"
										<< "    Punkt: " << min_point << "  (" << min_phi << ")\n";
				}
				if (max != -1)
				{
					//STOP_SINGULARITIES = 0;
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
		if (Maths::isEqual(min_phi.value,0))
		{
			ns1 = edge->vertex_[0];
		}
		else
		{
			ns1 = new TSESVertex<T>(min_point,edge->circle_.p-min_point,-2,-1);
			ns1->normal_.normalize();
			a = new TSESEdge<T>(*edge,true);
			a->vertex_[1] = ns1;
		}
		if (Maths::isEqual(max_phi.value,phi.value))
		{
			ns2 = edge->vertex_[1];
		}
		else
		{
			ns2 = new TSESVertex<T>(max_point,edge->circle_.p-max_point,-2,-1);
			ns2->normal_.normalize();
			na = new TSESEdge<T>(*edge,true);
			na->vertex_[0] = ns2;
		}
		TSphere3<T> probe1(edge->face_[0]->rsface_->center_,radius_of_probe);
		TSphere3<T> probe2(edge->face_[1]->rsface_->center_,radius_of_probe);
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
		if (min == max)
		{
			if (edge->face_[0]->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe1,min_probe,circle);
				a1 = new TSESEdge<T>(ns1,ns2,edge->face_[0],faces[min],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
			}
			if (edge->face_[1]->isNeighbouredTo(faces[min]) == false)
			{
				GetIntersection(probe2,min_probe,circle);
				a2 = new TSESEdge<T>(ns1,ns2,edge->face_[1],faces[min],circle,NULL,
														 TSESEdge<T>::TYPE_SINGULAR,-1);
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
				endEdges(a1,a2,a3,a4,probe1,probe2,min_probe,max_probe,ns1,ns2);
			}
			catch (Exception::GeneralException)
			{
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
		}
		if (a != NULL)
		{
			a->index_ = number_of_edges_;
			//a->type_ = TSESEdge<T>::TYPE_SINGULAR;
			edges_.push_back(a);
			number_of_edges_++;
			singular_edges_.push_front(a);
			number_of_singular_edges_++;
			a->face_[0]->edge_.push_back(a);
			a->face_[0]->number_of_edges_++;
			a->face_[0]->orientation_.push_back
					(a->face_[0]->orientation_[a->face_[0]->getRelativeEdgeIndex(edge)]);
			a->face_[1]->edge_.push_back(a);
			a->face_[1]->number_of_edges_++;
			a->face_[1]->orientation_.push_back
					(a->face_[1]->orientation_[a->face_[1]->getRelativeEdgeIndex(edge)]);
						#ifdef debug_singularities
						std::cout << "  a == " << *a << " != NULL\n";
						if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							 else { STOP_SINGULARITIES--; std::cout << "\n"; }
						#endif
		}
		if (na != NULL)
		{
			na->index_ = number_of_edges_;
			//na->type_ = TSESEdge<T>::TYPE_SINGULAR;
			edges_.push_back(na);
			number_of_edges_++;
			singular_edges_.push_front(na);
			number_of_singular_edges_++;
			na->face_[0]->edge_.push_back(na);
			na->face_[0]->number_of_edges_++;
			na->face_[0]->orientation_.push_back
					(na->face_[0]->orientation_[na->face_[0]->getRelativeEdgeIndex(edge)]);
			na->face_[1]->edge_.push_back(na);
			na->face_[1]->number_of_edges_++;
			na->face_[1]->orientation_.push_back
					(na->face_[1]->orientation_[na->face_[1]->getRelativeEdgeIndex(edge)]);
						#ifdef debug_singularities
						std::cout << "  na == " << *na << " != NULL\n";
						if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
							 else { STOP_SINGULARITIES--; std::cout << "\n"; }
						#endif
		}
		if (a1 != NULL)
		{
			a1->index_ = number_of_edges_;
			//a1->type_ = TSESEdge<T>::TYPE_SINGULAR;
			edges_.push_back(a1);
			number_of_edges_++;
			singular_edges_.push_back(a1);
			number_of_singular_edges_++;
			a1->face_[0]->edge_.push_back(a1);
			a1->face_[0]->number_of_edges_++;
			a1->face_[0]->orientation_.push_back(0);
			a1->face_[1]->edge_.push_back(a1);
			a1->face_[1]->number_of_edges_++;
			a1->face_[1]->orientation_.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a1 == " << *a1 << " != NULL\n";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
		}
		if (a2 != NULL)
		{
			a2->index_ = number_of_edges_;
			//a2->type_ = TSESEdge<T>::TYPE_SINGULAR;
			edges_.push_back(a2);
			number_of_edges_++;
			singular_edges_.push_back(a2);
			number_of_singular_edges_++;
			a2->face_[0]->edge_.push_back(a2);
			a2->face_[0]->number_of_edges_++;
			a2->face_[0]->orientation_.push_back(0);
			a2->face_[1]->edge_.push_back(a2);
			a2->face_[1]->number_of_edges_++;
			a2->face_[1]->orientation_.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a2 == " << *a2 << " != NULL\n";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
		}
		if (a3 != NULL)
		{
			a3->index_ = number_of_edges_;
			//a3->type_ = TSESEdge<T>::TYPE_SINGULAR;
			edges_.push_back(a3);
			number_of_edges_++;
			singular_edges_.push_back(a3);
			number_of_singular_edges_++;
			a3->face_[0]->edge_.push_back(a3);
			a3->face_[0]->number_of_edges_++;
			a3->face_[0]->orientation_.push_back(0);
			a3->face_[1]->edge_.push_back(a3);
			a3->face_[1]->number_of_edges_++;
			a3->face_[1]->orientation_.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a3 == " << *a3 << " != NULL\n";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						 else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
		}
		if (a4 != NULL)
		{
			a4->index_ = number_of_edges_;
			//a4->type_ = TSESEdge<T>::TYPE_SINGULAR;
			edges_.push_back(a4);
			number_of_edges_++;
			singular_edges_.push_back(a4);
			number_of_singular_edges_++;
			a4->face_[0]->edge_.push_back(a4);
			a4->face_[0]->number_of_edges_++;
			a4->face_[0]->orientation_.push_back(0);
			a4->face_[1]->edge_.push_back(a4);
			a4->face_[1]->number_of_edges_++;
			a4->face_[1]->orientation_.push_back(1);
					#ifdef debug_singularities
					std::cout << "  a4 == " << *a4 << " != NULL\n";
					if (STOP_SINGULARITIES == 0) std::cin >> STOP_SINGULARITIES;
						 else { STOP_SINGULARITIES--; std::cout << "\n"; }
					#endif
		}
		edge->face_[0]->edge_[edge->face_[0]->getRelativeEdgeIndex(edge)] = NULL;
		edge->face_[1]->edge_[edge->face_[1]->getRelativeEdgeIndex(edge)] = NULL;
		edges_[edge->index_] = NULL;
		singular_edges_.remove(edge);
		if (a != NULL)
		{
			ns1->index_ = number_of_vertices_;
			vertices_.push_back(ns1);
			number_of_vertices_++;
		}
		if (na != NULL)
		{
			ns2->index_ = number_of_vertices_;
			vertices_.push_back(ns2);
			number_of_vertices_++;
		}
				#ifdef debug_singularities
				//ofstream print("singular.log");
				//print << *this;
				//print.close();
				//std::cout << "\n\n" << *this << "\n\n";
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
		delete edge;
	}


	template <class T>
	bool TSolventExcludedSurface<T>::getIntersectionPointsAndAngles
		(TSESEdge<T>*				edge,
		 const TSphere3<T>& probe,
		 const TVector3<T>& normal,
		 TVector3<T>&				p1,
		 TAngle<T>&					phi1,
		 TVector3<T>&				p2,
		 TAngle<T>&					phi2)
		throw()
	{
		if (getIntersection(probe,edge->circle_,p1,p2) == true)
		{
			phi1 = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,p1-edge->circle_.p,normal);
			phi2 = getOrientedAngle(edge->vertex_[0]->point_-edge->circle_.p,p2-edge->circle_.p,normal);
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
		throw(Exception::GeneralException)
	{
		if (edge1 != NULL)
		{
			if (Maths::isGreater(edge1->circle_.n*(ns2->point_-edge1->circle_.p),(T)0))
			{
				edge1->circle_.n.negate();
			}
		}
		if (edge2 != NULL)
		{
			if (Maths::isGreater(edge2->circle_.n*(ns2->point_-edge2->circle_.p),(T)0))
			{
				edge2->circle_.n.negate();
			}
		}
		if (edge3 != NULL)
		{
			if (Maths::isGreater(edge3->circle_.n*(ns1->point_-edge3->circle_.p),(T)0))
			{
				edge3->circle_.n.negate();
			}
		}
		if (edge4 != NULL)
		{
			if (Maths::isGreater(edge4->circle_.n*(ns1->point_-edge4->circle_.p),(T)0))
			{
				edge4->circle_.n.negate();
			}
		}
		TAngle<T> phi1;
		TAngle<T> phi2;
		TVector3<T> point1;
		TVector3<T> point2;
		TSESVertex<T>* v;
		if (GetIntersection(probe1,min_probe,max_probe,point1,point2))
		{
			if (edge1 != NULL)
			{
				phi1 = getOrientedAngle(ns1->point_,point1,edge1->circle_.n);
				phi2 = getOrientedAngle(ns1->point_,point2,edge1->circle_.n);
				v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->point_ = point1;
					v->normal_ = edge1->circle_.p-point1;
				}
				else
				{
					v->point_ = point2;
					v->normal_ = edge1->circle_.p-point2;
				}
				v->normal_.normalize();
				edge1->vertex_[1] = v;
				v->index_ = number_of_vertices_;
				vertices_.push_back(v);
				number_of_vertices_++;
			}
			if (edge3 != NULL)
			{
				phi1 = getOrientedAngle(ns2->point_,point1,edge3->circle_.n);
				phi2 = getOrientedAngle(ns2->point_,point2,edge3->circle_.n);
				v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->point_ = point1;
					v->normal_ = edge3->circle_.p-point1;
				}
				else
				{
					v->point_ = point2;
					v->normal_ = edge3->circle_.p-point2;
				}
				v->normal_.normalize();
				edge3->vertex_[1] = v;
				v->index_ = number_of_vertices_;
				vertices_.push_back(v);
				number_of_vertices_++;
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
				phi1 = getOrientedAngle(ns1->point_,point1,edge2->circle_.n);
				phi2 = getOrientedAngle(ns1->point_,point2,edge2->circle_.n);
				TSESVertex<T>* v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->point_ = point1;
					v->normal_ = edge2->circle_.p-point1;
				}
				else
				{
					v->point_ = point2;
					v->normal_ = edge2->circle_.p-point2;
				}
				v->normal_.normalize();
				edge2->vertex_[1] = v;
				v->index_ = number_of_vertices_;
				vertices_.push_back(v);
				number_of_vertices_++;
			}
			if (edge4 != NULL)
			{
				phi1 = getOrientedAngle(ns2->point_,point1,edge4->circle_.n);
				phi2 = getOrientedAngle(ns2->point_,point2,edge4->circle_.n);
				v = new TSESVertex<T>;
				if (phi1 < phi2)
				{
					v->point_ = point1;
					v->normal_ = edge4->circle_.p-point1;
				}
				else
				{
					v->point_ = point2;
					v->normal_ = edge4->circle_.p-point2;
				}
				v->normal_.normalize();
				edge4->vertex_[1] = v;
				v->index_ = number_of_vertices_;
				vertices_.push_back(v);
				number_of_vertices_++;
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
	bool TSolventExcludedSurface<T>::getIntersection
		(const TSphere3<T>& sphere,
		 const TCircle3<T>& circle,
		 TVector3<T>&				p1,
		 TVector3<T>&				p2)
		throw()
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
		return (circle.has(p1) && circle.has(p2) && sphere.has(p1) && sphere.has(p2));
	}
		


} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
