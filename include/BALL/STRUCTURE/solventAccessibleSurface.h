// $Id: solventAccessibleSurface.h,v 1.1.2.1 2002/08/22 17:17:55 oliver Exp $

#ifndef BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H
#define BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H

#ifndef BALL_STRUCTURE_SASVERTEX_H
#	include <BALL/STRUCTURE/SASVertex.h>
#endif

#ifndef BALL_STRUCTURE_SASEDGE_H
#	include <BALL/STRUCTURE/SASEdge.h>
#endif

#ifndef BALL_STRUCTURE_SASFACE_H
#	include <BALL/STRUCTURE/SASFace.h>
#endif

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#	include <BALL/STRUCTURE/reducedSurface.h>
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

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#include <list>



namespace BALL
{

	template <class T>
	class TTriangulatedSAS;

	/** Generic SolventAccessibleSurface Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/solventAccessibleSurface.h}	\\
	*/
	template <class T>
	class TSolventAccessibleSurface
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class TTriangulatedSAS<T>
				\end{itemize}
		*/
		friend class TTriangulatedSAS<T>;

		BALL_CREATE(TSolventAccessibleSurface)

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
				This method creates a new SolventAccessibleSurface object.
		*/
		TSolventAccessibleSurface()
			throw();

		/**	Copy constructor.
				Create a new SolventAccessibleSurface object from another.
				@param	sas		the SolventAccessibleSurface object to be copied
				@param	bool	ignored - just for interface consistency
		*/
		//TSolventAccessibleSurface(const TSolventAccessibleSurface<T>& sas, bool = false)
		//	throw();

		/** Detailed constructor
		*/
		TSolventAccessibleSurface(TReducedSurface<T>* reduced_surface)
			throw();

		/**	Destructor.
				As there are no dynamic	data structures, nothing happens.
		*/
		virtual ~TSolventAccessibleSurface()
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

		/**	@name	SAS computation (private)
		*/
		//@{

		void get()
			throw();

		void createFace(Position j)
			throw();

		void createVertex(Position j)
			throw();

		void createEdge(Position j)
			throw();

		//@}


		//protected:
		public:

		Position number_of_vertices_;
		::std::vector<TSASVertex<T>*> vertices_;
		Position number_of_edges_;
		::std::vector<TSASEdge<T>*> edges_;
		Position number_of_faces_;
		::std::vector<TSASFace<T>*> faces_;
		TReducedSurface<T>* reduced_surface_;

	};


	/**	Output- Operator.
			Prints out a SolventAccessibleSurface
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSolventAccessibleSurface<T>& sas)
	{
		s << "Vertices:\n";
		for (Position i = 0; i < sas.vertices_.size(); i++)
		{
			if (sas.vertices_[i] != NULL)
			{
				s << "  " << *sas.vertices_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "Edges:\n";
		for (Position i = 0; i < sas.edges_.size(); i++)
		{
			if (sas.edges_[i] != NULL)
			{
				s << "  " << *sas.edges_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		s << "Faces:\n";
		for (Position i = 0; i < sas.faces_.size(); i++)
		{
			if (sas.faces_[i] != NULL)
			{
				s << "  " << *sas.faces_[i] << "\n";
			}
			else
			{
				s << "  --\n";
			}
		}
		return s;
	}


	/**	The Default SolventAccessibleSurface type.
			If double precision is not needed, {\tt TSolventAccessibleSurface<float>} should
			be used. It is predefined as {\tt SolventAccessibleSurface} for convenience.
	*/
	typedef TSolventAccessibleSurface<float> SolventAccessibleSurface;


	template <typename T>
	TSolventAccessibleSurface<T>::TSolventAccessibleSurface()
		throw()
		:	number_of_vertices_(0),
			vertices_(),
			number_of_edges_(0),
			edges_(),
			number_of_faces_(0),
			faces_(),
			reduced_surface_(NULL)
	{
	}


	/*template <typename T>
	TSolventAccessibleSurface<T>::TSolventAccessibleSurface
		(const TSolventAccessibleSurface<T>& ses, bool)
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
		//	vertices_[i] = new TSASVertex<T>(*ses.vertices_[i],false);
		//}
		//for (Position i = 0; i < number_of_edges_; i++)
		//{
		//	edges_[i] = new TSASVertex<T>(*ses.edges_[i],false);
		//}
		//typename ::std::list<TSASEdge<T>*>::const_iterator se;
		//for (se = ses.singular_edges_.begin(); se != ses.singular_edges_.end(); se++)
		//{
		//	singular_edges_.push_back(edges_[(*se)->index_]);
		//}
		//for (Position i = 0; i < number_of_contact_faces_; i++)
		//{
		//	contact_faces_[i] = new TSASFace<T>(*ses.contact_faces_[i],false);
		//}
		//for (Position i = 0; i < number_of_toric_faces_; i++)
		//{
		//	toric_faces_[i] = new TSASFace<T>(*ses.toric_faces_[i],false);
		//}
		//for (Position i = 0; i < number_of_spheric_faces_; i++)
		//{
		//	spheric_faces_[i] = new TSASFace<T>(*ses.spheric_faces_[i],false);
		//}
	}*/


	template <typename T>
	TSolventAccessibleSurface<T>::TSolventAccessibleSurface(TReducedSurface<T>* reduced_surface)
		throw()
		:	number_of_vertices_(reduced_surface->number_of_faces_),
			vertices_(number_of_vertices_),
			number_of_edges_(reduced_surface->number_of_edges_),
			edges_(number_of_edges_),
			number_of_faces_(reduced_surface->number_of_vertices_),
			faces_(number_of_faces_),
			reduced_surface_(reduced_surface)
	{
		Position i;
		for (i = 0; i < number_of_vertices_; i++)
		{
			vertices_[i] = new TSASVertex<T>;
			vertices_[i]->index_ = i;
		}
		for (i = 0; i < number_of_edges_; i++)
		{
			edges_[i] = new TSASEdge<T>;
			edges_[i]->index_ = i;
		}
		for (i = 0; i < number_of_faces_; i++)
		{
			faces_[i] = new TSASFace<T>;
			faces_[i]->index_ = i;
		}
	}


	template <typename T>
	TSolventAccessibleSurface<T>::~TSolventAccessibleSurface()
		throw()
	{
		clear();
	}


	template <typename T>
	void TSolventAccessibleSurface<T>::set(TReducedSurface<T>* rs)
		throw()
	{
		clear();
		reduced_surface_ = rs;
		number_of_vertices_ = reduced_surface_->number_of_faces_;
		number_of_edge_ = reduced_surface_->number_of_edges_;
		number_of_faces_ = reduced_surface_->number_of_vertices_;
		Position i;
		for (i = 0; i < number_of_vertices_; i++)
		{
			vertices_[i] = new TSASVertex<T>;
			vertices_[i]->index_ = i;
		}
		for (i = 0; i < number_of_edges_; i++)
		{
			edges_[i] = new TSASEdge<T>;
			edges_[i]->index_ = i;
		}
		for (i = 0; i < number_of_faces_; i++)
		{
			faces_[i] = new TSASFace<T>;
			faces_[i]->index_ = i;
		}
	}


	template <typename T>
	void TSolventAccessibleSurface<T>::clear()
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
		for (i = 0; i < number_of_faces_; i++)
		{
			delete faces_[i];
		}
		vertices_.clear();
		edges_.clear();
		faces_.clear();
		number_of_vertices_ = 0;
		number_of_edges_ = 0;
		number_of_faces_ = 0;
	}


	template <typename T>
	void TSolventAccessibleSurface<T>::compute()
		throw()
	{
//cout << "#1\n";
		for (Position i = 0; i < number_of_vertices_; i++)
		{
//cout << "  " << i << "\n";
			createVertex(i);
		}
//cout << "#2\n";
		for (Position i = 0; i < number_of_edges_; i++)
		{
//cout << "  " << i << "\n";
			createEdge(i);
		}
//cout << "#3\n";
		for (Position i = 0; i < number_of_faces_; i++)
		{
//cout << "  " << i << "\n";
			createFace(i);
		}
//cout << "#4\n";
	}


	template <typename T>
	void TSolventAccessibleSurface<T>::createVertex(Position j)
		throw()
	{
		TSASVertex<T>* vertex = vertices_[j];
		vertex->index_ = j;
		TRSFace<T>* rsface = reduced_surface_->faces_[j];
		vertex->point_ = rsface->center_;
		for (Position i = 0; i < 3; i++)
		{
			vertex->edges_.insert(edges_[rsface->edge_[i]->index_]);
			vertex->faces_.insert(faces_[rsface->vertex_[i]->index_]);
		}
	}


	template <typename T>
	void TSolventAccessibleSurface<T>::createEdge(Position j)
		throw()
	{
		TSASEdge<T>* edge = edges_[j];
		edge->index_ = j;
		TRSEdge<T>* rsedge = reduced_surface_->edges_[j];
		if (rsedge->face_[0] != NULL)
		{
			edge->vertex_[0] = vertices_[rsedge->face_[0]->index_];
			edge->vertex_[1] = vertices_[rsedge->face_[1]->index_];
		}
		else
		{
			edge->vertex_[0] = NULL;
			edge->vertex_[1] = NULL;
		}
		edge->face_[0] = faces_[rsedge->vertex_[0]->index_];
		edge->face_[1] = faces_[rsedge->vertex_[1]->index_];
		edge->angle_ = rsedge->phi_;
		edge->circle_.p = rsedge->center_of_torus_;
		edge->circle_.radius = rsedge->radius_of_torus_;
		TSphere3<T> atom1(reduced_surface_->atom_[rsedge->vertex_[0]->atom_]);
		TSphere3<T> atom2(reduced_surface_->atom_[rsedge->vertex_[1]->atom_]);
		edge->circle_.n = atom1.p-atom2.p;
	}


	template <typename T>
	void TSolventAccessibleSurface<T>::createFace(Position j)
		throw()
	{
		TSASFace<T>* face = faces_[j];
		face->index_ = j;
		TRSVertex<T>* rsvertex = reduced_surface_->vertices_[j];
		typename HashSet<TRSEdge<T>*>::Iterator e;
		for (e = rsvertex->edges_.begin(); e != rsvertex->edges_.end(); e++)
		{
			face->edge_.push_back(edges_[(*e)->index_]);
			if ((*e)->vertex_[0]->index_ == face->index_)
			{
				face->orientation_.push_back(true);
			}
			else
			{
				face->orientation_.push_back(false);
			}
		}
		typename HashSet<TRSFace<T>*>::Iterator f;
		for (f = rsvertex->faces_.begin(); f != rsvertex->faces_.end(); f++)
		{
			face->vertex_.push_back(vertices_[(*f)->index_]);
		}
		face->sphere_ = reduced_surface_->atom_[rsvertex->atom_];
		face->sphere_.radius += reduced_surface_->probe_radius_;
	}







} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H

