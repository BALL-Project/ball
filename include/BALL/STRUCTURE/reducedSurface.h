// $Id: reducedSurface.h,v 1.14 2001/06/19 21:07:00 strobel Exp $

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#define BALL_STRUCTURE_REDUCEDSURFACE_H

#ifndef BALL_STRUCTURE_RSVERTEX_H
#	include <BALL/STRUCTURE/RSVertex.h>
#endif

#ifndef BALL_STRUCTURE_RSEDGE_H
#	include <BALL/STRUCTURE/RSEdge.h>
#endif

#ifndef BALL_STRUCTURE_RSFACE_H
#	include <BALL/STRUCTURE/RSFace.h>
#endif

#ifndef BALL_STRUCTURE_BSDTREE_H
#	include <BALL/STRUCTURE/BSDTree.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_LINE3_H
#	include <BALL/MATHS/line3.h>
#endif

#ifndef BALL_MATHS_ANALYTICALGEOMETRY_H
#	include <BALL/MATHS/analyticalGeometry.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif


#include <vector>
#include <list>
#include <fstream>


namespace BALL
{

			// debuging
			template <class T>
			string IndexToString(Index nr, const T& /*dummy*/)
			{
				if (nr == 0) return "0";
				string back = "";
				while (nr != 0)
				{
					Index rest = nr % 10;
					switch (rest)
					{
						case 0 :	back += "0"; break;
						case 1 :	back += "1"; break;
						case 2 :	back += "2"; break;
						case 3 :	back += "3"; break;
						case 4 :	back += "4"; break;
						case 5 :	back += "5"; break;
						case 6 :	back += "6"; break;
						case 7 :	back += "7"; break;
						case 8 :	back += "8"; break;
						case 9 :	back += "9"; break;
					}
					nr = nr / 10;
				}
				string number = "";
				for (Position i = back.size(); i > 0;)
				{
					number += back[--i];
				}
				return number;
			}


	/** Generic ReducedSurface Class.	\\
			{\bf Definition:} \URL{BALL/STRUCTURE/reducedSurface.h}
	*/
	template <class T>
	class TReducedSurface
	{
		public:

		BALL_CREATE(TReducedSurface)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				All components are initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TReducedSurface()
			throw();

		/**	Copy constructor.
				@param reduced_surface the ReducedSurface object to be copied
				@param bool ignored - just for interface consistency
		*/
		TReducedSurface(const TReducedSurface<T>& reduced_surface)
			throw();

		/**	Detailled constructor.
				Create a new ReducedSurface object from a list of spheres.
		*/
		TReducedSurface(const std::vector< TSphere3<T> >& spheres,
				const T& probe_radius)
			throw();

		/**	Destructor.
		*/
		virtual ~TReducedSurface()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{
		/** Delete all vertices, edges and faces
		*/
		void clear()
			throw();

		/** Remove all NULL-pointers
		*/
		void clean()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Return the number of atoms.
				@return Size the number of atoms
		*/
		Size numberOfAtoms()
			throw();

		/** Return the number of rsvertices.
				@return Size the number of rsvertices
		*/
		Size numberOfVertices()
			throw();

		/** Return the number of rsedges.
				@return Size the number of rsedges
		*/
		Size numberOfEdges()
			throw();

		/** Return the number of rsfaces.
				@return Size the number of rsfaces
		*/
		Size numberOfFaces()
			throw();
		
		/** Return the probe radius
				@return T the radius of the used probe sphere
		*/
		T getProbeRadius()
			throw();

		/** Return the maximal radius of all spheres
				@return T the maximal radius
		*/
    T getMaximalRadius()
			throw();

		/** Return the i'th sphere.
				@param	i	the index of the sphere that should be given back
				@return TSpherer3<T>, the i'th sphere
		*/
    TSphere3<T> getSphere(Position i)
			throw();

		/** Return the vertices
				@return vector< TRSVertex<T>* > all vertices
		*/
		std::vector< TRSVertex<T>* > getVertices()
			throw();

		/** Return the i'th rsvertex.
				@param	i	the index of the rsvertex that should be given back
				@return TSpherer3<T>, the i'th rsvertex
		*/
		TRSVertex<T>* getVertex(Position i)
			throw();

		/** Return the edges
				@return vector< TRSEdge<T>* > all edges
		*/
		std::vector< TRSEdge<T>* > getEdges()
			throw();

		/** Return the i'th rsedge.
				@param	i	the index of the rsedge that should be given back
				@return TSpherer3<T>, the i'th rsedge
		*/
		TRSEdge<T>* getEdge(Position i)
			throw();

		/** Return the faces
				@return vector< TRSFace<T>* > all faces
		*/
		std::vector< TRSFace<T>* > getFaces()
			throw();

		/** Return the i'th rsface.
				@param	i	the index of the rsface that should be given back
				@return TSpherer3<T>, the i'th rsface
		*/
		TRSFace<T>* getFace(Position i)
			throw();

		/** Delete a pair of similar faces
				@param	face1	a pointer to the first edge
				@param	face2	a pointer to the second face
		*/
		void deleteSimilarFaces(TRSFace<T>* face1, TRSFace<T>* face2)
			throw();

		/** Compute the reduced surface
		*/
		void compute()
			throw();

		//@}


		private:

		/*_ @name Computing reduced surface
		*/
		//@{

		/*_ Compute the RSComponent of a given face
			@param	face					the starting face
			@param	indices				a HashSet of the indices of all atoms wich are not
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void getRSComponent
			 (TRSFace<T>*																	 face,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw();

		/*_ treat all edges of a face
			@param	face					the RSFace to be treated
			@param	indices				a HashSet of the indices of all atoms wich are not
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void treatFace
			 (TRSFace<T>*																	 face,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw();

		/*_ roll over an edge that belongs to onlyone face and find the other one
			@param	edge					the RSEdge to be treated
			@param	indices				a HashSet of the indices of all atoms wich are not
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void treatEdge
			 (TRSEdge<T>*																	 edge,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw();

		/*_ Check all new created vertices for extensions
			@param	indices				a HashSet of the indices of all atoms wich are not
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void extendComponent
			 (HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw();


		/*_ Find a third atom rolling over two vertices starting on a face
			@param	vertex1	the first vertex
			@param	vertex2	the second vertex
			@param	face		the starting face
			@param	probe		the new probe sphere
			@param	phi			the rotation angle
			@return	Index		index of the found atom
		*/
		Index thirdAtom
			 (TRSVertex<T>* vertex1,
				TRSVertex<T>* vertex2,
				TRSFace<T>*		face,
				TSphere3<T>&	probe,
				TAngle<T>&		phi)
			throw();

		//@}
		/*_ @name Finding a start position
		*/
		//@{

		/*_ Find a start position
			@param	indices				a HashSet of the indices of all atoms wich are not
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertex				a pointer to the found vertex, if only a vertex
														can be found
			@param	edge					a pointer to the found edge, if only an edge can be
														found
			@param	face					a pointer to the found face, if a face can be found
			@param	vertices			a list of RSVertices for each atom
			@return Byte					0, if no start position is found,
														1, if a single vertex is found,
														2, if an edge is found,
														3, if a face is found
		*/
		Byte getStartPosition
			 (HashSet<Index>&														 indices,
				HashSet<TRSVertex<T>*>&										 new_vertices,
				TRSVertex<T>*&														 vertex,
				TRSEdge<T>*&															 edge,
				TRSFace<T>*&															 face,
				std::vector< std::list< TRSVertex<T>* > >& vertices)
			throw();

		//@}
		/*_ @name Finding a first face
		*/
		//@{

		/*_ Try to find a starting face
			@param	indices			a HashSet of the indices of all atoms that are not
													yet part of the reduced surface
			@return	TRSFace<T>*	a pointer to the found face, if a face can be found,
													NULL otherwise
		*/
		TRSFace<T>* findFirstFace(HashSet<Index>& indices)
			throw();

		/*_ Try to find a starting face in a given direction
			@param	indices			a HashSet of the indices of all atoms wich are not
													yet part of the reduced surface
			@param	direction		search in x-direction, if direction is 0,
													search in y-direction, if direction is 1,
													search in z-direction, if direction is 2
			@param	extrem			search in min direction, if extrem is 0,
													search in max direction, if extrem is 1
			@return	TRSFace<T>*	a pointer to the found face, if a face can be found,
													NULL otherwise
		*/
		TRSFace<T>* findFace
			 (HashSet<Index>& indices,
				Byte						direction,
				Byte						extrem)
			throw();

		//@}
		/*_ @name Finding a first edge
		*/
		//@{

		/*_ Try to find a starting edge
			@param	indices			a HashSet of the indices of all atoms that are not
													yet part of the reduced surface
			@return	TRSEdge<T>*	a pointer to the found edge, if a face can be found,
													NULL otherwise
		*/
		TRSEdge<T>* findFirstEdge(HashSet<Index>& indices)
			throw();

		/*_ Try to find a starting edge in a given direction
			@param	indices			a HashSet of the indices of all atoms that are not
													yet part of the reduced surface
			@param	direction		search in x-direction, if direction is 0,
													search in y-direction, if direction is 1,
													search in z-direction, if direction is 2
			@param	extrem			search in min direction, if extrem is 0,
													search in max direction, if extrem is 1
			@return	TRSEdge<T>*	a pointer to the found edge, if a face can be found,
													NULL otherwise
		*/
		TRSEdge<T>* findEdge(HashSet<Index>& indices, Byte direction, Byte extrem)
			throw();

		//@}
		/*_ @name Finding a first vertex
		*/
		//@{

		/*_ Try to find a single atom
			@param	indices				a HashSet of the indices of all atoms that are not
														yet part of the reduced surface
			@return	TRSVertex<T>*	a pointer to the found vertex, if a vertex can be
														found, NULL otherwise
		*/
		TRSVertex<T>* findFirstVertex(HashSet<Index>& indices)
			throw();

		/*_ Find a single atom in a given direction
			@param	indices		a HashSet of the indices of all atoms that are not
												yet part of the reduced surface
			@param	direction	search in x-direction, if direction is 0,
												search in y-direction, if direction is 1,
												search in z-direction, if direction is 2
			@param	extrem		search in min direction, if extrem is 0,
												search in max direction, if extrem is 1
			@return	Index			the index of the found atom
		*/
		Index findFirstAtom(const HashSet<Index>& indices, Byte direction, Byte extrem)
			throw();

		/*_ Find a second atom close enougth to the first atom in a given direction
			@param	atom1			the index of the first atom
			@param	atom_list	a HashSet of the indices of all candidate atoms
			@param	direction	search in x-direction, if direction is 0,
												search in y-direction, if direction is 1,
												search in z-direction, if direction is 2
			@param	extrem		search in min direction, if extrem is 0,
												search in max direction, if extrem is 1
			@return	Index			the index of the found atom
		*/
		Index findSecondAtom
			 (Index					 atom1,
				HashSet<Index> atom_list,
				Byte					 direction,
				Byte					 extrem)
			throw();

		/*_ Find a second atom close enougth to the first two atoms
			@param	atom1			the index of the first atom
			@param	atom2			the index of the second atom
			@param	atom_list	a HashSet of the indices of all candidate atoms
			@return	::std::list< ::std::pair< Index,TSphere3<T> > >
												a list of all candidates with their probe spheres
		*/
		::std::list< ::std::pair< Index,TSphere3<T> > > findThirdAtom
			 (Index									atom1,
				Index									atom2,
				const HashSet<Index>& third)
			throw();

		//@}
		/*_ @name Some utilities
		*/
		//@{

		/*_ Find all atoms close enougth to a given atom
			@param	atom0				the index of the given atom
			@param	input_set		HashSet of candidate atoms
			@param	output_set	HashSet of all atoms close enougth to the given atom
		*/
		void neighboursOfOneAtom
			 (Index									atom0,
				const HashSet<Index>& input_set,
				HashSet<Index>&				output_set)
			throw();

		/*_ Find all atoms close enougth to two given atoms
			@param	atom1				the index of the first given atom
			@param	atom2				the index of the second given atom
			@param	input_set		HashSet of candidate atoms
			@param	output_set	HashSet of all atoms close enougth to the given atoms
		*/
		void neighboursOfTwoAtoms
			 (Index									atom1,
				Index									atom2,
				const HashSet<Index>& input_set,
				HashSet<Index>&				output_set)
			throw();


    /*_ Getb the extrem coordinate of a circle in a given direction
    	@param	circle		the circle
			@param	direction	search in x-direction, if direction is 0,
												search in y-direction, if direction is 1,
												search in z-direction, if direction is 2
			@param	extrem		search in min direction, if extrem is 0,
												search in max direction, if extrem is 1
			@return	T					the extrem coordinate
    */
		T getCircleExtremum
			 (const TCircle3<T>& circle,
				Position					 direction,
				Position					 extrem)
			throw();

		//@}
		/*_ @name Creating / updating edges / faces
		*/
		//@{

		/*_ Create a free edge from two vertices if it is a free edge
			@param	vertex1			a pointer to the first vertex
			@param	vertex2			a pointer to the second vertex
			@param	neighbours	HashSet of indices of atoms in environment of the two
													vertices
			@return	TRSEdge<T>* a pointer to the created free edge, if there is one,
													NULL otherwise
		*/
		TRSEdge<T>* createFreeEdge
				 (TRSVertex<T>*					vertex1,
					TRSVertex<T>*					vertex2,
					const HashSet<Index>& neighbours)
			throw();

		/*_ Get the circle described by the center of the probe sphere and the two
				contact circles with the atoms when the probe sphere rolls over two
				atoms
				@param	atom1		the index of the first atom
				@param	atom2		the index of the second atom
				@param	circle1	the circle described by the center of the probe sphere
				@param	circle2	the contact circle with atom1
				@param	circle3	the contact circle with atom2
				@return	bool		true, if the probe sphere can touch both atoms,
												false, otherwise
		*/
		bool getCircles
			 (Index atom1,
				Index atom2,
				TCircle3<T>& circle1,
				TCircle3<T>& circle2,
				TCircle3<T>& circle3)
			throw();

		/*_ Get the normal vector of the face described by three atoms and a probe
			@param	atom1				the index of the first atom
			@param	atom2				the index of the second atom
			@param	atom3				the index of the third atom
			@param	probe				the probe sphere lying on atom1, atom2, atom3
			@return	TVector3<T>	the normal vector
		*/
		TVector3<T> getFaceNormal
			 (const TSphere3<T>& atom1,
				const TSphere3<T>& atom2,
				const TSphere3<T>& atom3,
				const TSphere3<T>& probe)
			throw();

		/*_ Update a face and it's edges
			@param	v1		the first vertex of the face
			@param	v2		the second vertex of the face
			@param	v3		the third vertex of the face
			@param	probe	the probe sphere of the face
			@param	mode	
			@param	f			the face
			@param	e1		the first edge. it is updated if mode & 1 == 1. it's the
										edge from v1 to v2
			@param	e2		the second edge. it is updated if mode & 2 == 2. it's the
										edge from v2 to v3
			@param	e3		the third edge. it is updated if mode & 4 == 4. it's the
										edge from v3 to v1
		*/
		void updateFaceAndEdges
			 (TRSVertex<T>* v1,
				TRSVertex<T>* v2,
				TRSVertex<T>* v3,
				const TSphere3<T>& probe,
				Byte mode,
				TRSFace<T>* f,
				TRSEdge<T>* e1,
				TRSEdge<T>* e2,
				TRSEdge<T>* e3)
			throw();

		/*_ Test, weather a face exists or not
			@param	face				a pointer to the face to be tested
			@param	vertices		a list of RSVertices for each atom
			@return	TRSFace<T>*	a pointer to the face, if it exists, otherwise NULL
		*/
		TRSFace<T>* faceExists
			 (TRSFace<T>* face,
				const ::std::list< TRSVertex<T>* >& vertices)
			throw();

		//@}
		/*_ @name Finding a probe sphere
		*/
		//@{

		/*_ Get the centers of the probe sphere when it lies on three atoms
			@param	a1		the first atom
			@param	a2		the second atom
			@param	a3		the third atom
			@param	c1		the first center
			@param	c2		the second center
			@return	bool	true, if the probe sphere can touch the three atoms,
										false, otherwise
		*/
		bool centerOfProbe
			 (Index				 a1,
				Index				 a2,
				Index				 a3,
				TVector3<T>& c1,
				TVector3<T>& c2)
			throw();

		/*_ Check,weather a probe sphere is inside an atom
			@param	probe	the probe sphere to be tested
			@return	bool	true, if the probe sphere is not intersecting any atom
										false, otherwise
		*/
		bool checkProbe(const TSphere3<T>& probe)
			throw();


		protected:

    /*_ the atoms of the molecule
    */
    std::vector< TSphere3<T> > atom_;
    /*_	probe radius
    */
    T probe_radius_;
    /*_ BSDTree of the atoms
    */
    TBSDTree<T>* tree_;
    /*_ the vertices of the reduced surface
    */
    std::vector< TRSVertex<T>* > vertices_;
    /*_ the edges of the reduced surface
    */
    std::vector< TRSEdge<T>* > edges_;
    /*_ the faces of the reduced surface
    */
    std::vector< TRSFace<T>* > faces_;
    /*_ maximal radius of all atoms
    */
    T r_max_;
    /*_ the number of vertices of the reduced surface
    */
    Size number_of_vertices_;
    /*_ the number of edges of the reduced surface
    */
    Size number_of_edges_;
    /*_ the number of faces of the reduced surface
    */
    Size number_of_faces_;
	
	};


	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator.
			Reads in a ReducedSurface
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TReducedSurface<T>& rs)
	{
		// BAUSTELLE
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	/**	Output- Operator.
			Prints out a ReducedSurface
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TReducedSurface<T>& rs)
		{
			s << "Spheres:\n";
			for (Position i = 0; i < rs.numberOfAtoms(); i++)
			{
				s << "  " << rs.getSphere(i) << "\n";
		  }
		  s << "RSVertices:\n";
			for (Position i = 0; i < rs.numberOfVertices(); i++)
			{
				if (rs.getVertex(i) == NULL)
	      {
					s << "  --\n";
	      }
	      else
	      {
					s << "  " << *(rs.getVertex(i)) << "\n";
	      }
		  }
		  s << "RSEdges:\n";
			for (Position i = 0; i < rs.numberOfEdges(); i++)
			{
				if (rs.getEdge(i) == NULL)
	      {
					s << "  --\n";
	      }
	      else
	      {
					s << "  " << *(rs.getEdge(i)) << "\n";
				}
		  }
		  s << "RSFaces:\n";
			for (Position i = 0; i < rs.numberOfFaces(); i++)
			{
				if (rs.getFace(i) == NULL)
	      {
					s << "  --\n";
	      }
	      else
	      {
					s << "  " << *(rs.getFace(i)) << "\n";
				}
		  }
		  return s;
		}

	//@}

	
	/**	The Default ReducedSurface Type.
			If double precision is not needed, {\tt TReducedSurface<float>} should
			be used. It is predefined as {\tt ReducedSurface} for convenience.
	*/
	typedef TReducedSurface<float> ReducedSurface;


	template <typename T>
	TReducedSurface<T>::TReducedSurface()
		throw()
		:	atom_(),
			probe_radius_((T)0),
			tree_(),
			vertices_(),
			edges_(),
			faces_(),
			r_max_((T)0),
			number_of_vertices_(0),
			number_of_edges_(0),
			number_of_faces_(0)
	{
	}


	template <typename T>
	TReducedSurface<T>::TReducedSurface(const TReducedSurface<T>& reduced_surface)
		throw()
		: atom_(reduced_surface.atom_),
			probe_radius_(reduced_surface.probe_radius_),
			tree_(reduced_surface.tree_),
			vertices_(reduced_surface.vertices_),
			edges_(reduced_surface.edges_),
			faces_(reduced_surface.faces_),
			r_max_(reduced_surface.r_max_),
			number_of_vertices_(reduced_surface.number_of_vertices_),
			number_of_edges_(reduced_surface.number_of_edges_),
			number_of_faces_(reduced_surface.number_of_faces_)
	{
	}


	template <typename T>
	TReducedSurface<T>::TReducedSurface
		 (const ::std::vector< TSphere3<T> >& spheres,
			const T& probe_radius)
		throw()
		:	atom_(),
			probe_radius_(probe_radius),
			tree_(),
			vertices_(),
			edges_(),
			faces_(),
			r_max_((T)0),
			number_of_vertices_(0),
			number_of_edges_(0),
			number_of_faces_(0)
	{
		std::vector< TVector3<T> > point_list;
		for (Position i = 0; i < spheres.size(); i++)
		{
			atom_.push_back(spheres[i]);
			point_list.push_back(spheres[i].p);
			r_max_ = ((r_max_ < spheres[i].radius) ? spheres[i].radius : r_max_);
		}
		tree_ = new TBSDTree<T>(point_list);
		tree_->build();
	}


	template <typename T>
	TReducedSurface<T>::~TReducedSurface()
		throw()
	{
		clear();
		delete tree_;
	}


	template <typename T>
	void TReducedSurface<T>::clear()
		throw()
	{
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			if (vertices_[i] != NULL)
			{
				delete vertices_[i];
			}
		}
		for (Position i = 0; i < number_of_edges_; i++)
		{
			delete edges_[i];
		}
		for (Position i = 0; i < number_of_faces_; i++)
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
	void TReducedSurface<T>::clean()
		throw()
	{
		std::vector<TRSVertex<T>*> v;
		Size end = number_of_vertices_;
		number_of_vertices_ = 0;
		for (Position i = 0; i < end; i++)
		{
			if (vertices_[i] != NULL)
			{
				vertices_[i]->setIndex(number_of_vertices_);
				v.push_back(vertices_[i]);
				number_of_vertices_++;
			}
		}
		vertices_ = v;
		std::vector<TRSEdge<T>*> e;
		end = number_of_edges_;
		number_of_edges_ = 0;
		for (Position i = 0; i < end; i++)
		{
			if (edges_[i] != NULL)
			{
				edges_[i]->setIndex(number_of_edges_);
				e.push_back(edges_[i]);
				number_of_edges_++;
			}
		}
		edges_ = e;
		std::vector<TRSFace<T>*> f;
		end = number_of_faces_;
		number_of_faces_ = 0;
		for (Position i = 0; i < end; i++)
		{
			if (faces_[i] != NULL)
			{
				faces_[i]->setIndex(number_of_faces_);
				f.push_back(faces_[i]);
				number_of_faces_++;
			}
		}
		faces_ = f;
	}


	template <typename T>
	Size TReducedSurface<T>::numberOfAtoms()
		throw()
	{
		return atom_.size();
	}


	template <typename T>
	Size TReducedSurface<T>::numberOfVertices()
		throw()
	{
		return number_of_vertices_;
	}


	template <typename T>
	Size TReducedSurface<T>::numberOfEdges()
		throw()
	{
		return number_of_edges_;
	}


	template <typename T>
	Size TReducedSurface<T>::numberOfFaces()
		throw()
	{
		return number_of_faces_;
	}


	template <typename T>
	T TReducedSurface<T>::getProbeRadius()
		throw()
	{
		return probe_radius_;
	}


	template <typename T>
	T TReducedSurface<T>::getMaximalRadius()
		throw()
	{
		return r_max_;
	}


	template <typename T>
	TSphere3<T> TReducedSurface<T>::getSphere(Position i)
		throw()
	{
		if (i < atom_.size())
		{
			return atom_[i];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__,i,atom_.size()-1);
		}
	}


	template <typename T>
	std::vector< TRSVertex<T>* > TReducedSurface<T>::getVertices()
		throw()
	{
		return vertices_;
	}


	template <typename T>
	TRSVertex<T>* TReducedSurface<T>::getVertex(Position i)
		throw()
	{
		if (i < number_of_vertices_)
		{
			return vertices_[i];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__,i,number_of_vertices_-1);
		}
	}


	template <typename T>
	std::vector< TRSEdge<T>* > TReducedSurface<T>::getEdges()
		throw()
	{
		return edges_;
	}


	template <typename T>
	TRSEdge<T>* TReducedSurface<T>::getEdge(Position i)
		throw()
	{
		if (i < number_of_edges_)
		{
			return edges_[i];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__,i,number_of_edges_-1);
		}
	}


	template <typename T>
	std::vector< TRSFace<T>* > TReducedSurface<T>::getFaces()
		throw()
	{
		return faces_;
	}


	template <typename T>
	TRSFace<T>* TReducedSurface<T>::getFace(Position i)
		throw()
	{
		if (i < number_of_faces_)
		{
			return faces_[i];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__,i,number_of_faces_-1);
		}
	}


	template <typename T>
	void TReducedSurface<T>::deleteSimilarFaces
		 (TRSFace<T>* face1,
			TRSFace<T>* face2)
		throw()
	{
		if (face1->similar(*face2))
		{
			// find the similar edges
			std::vector<TRSEdge<T>*> rsedge1(3);
			std::vector<TRSEdge<T>*> rsedge2(3);
			rsedge1[0] = face1->getEdge(0);
			rsedge1[1] = face1->getEdge(1);
			rsedge1[2] = face1->getEdge(2);
			for (Position j = 0; j < 3; j++)
			{
				for (Position i = 0; i < 3; i++)
				{
					if (face2->getEdge(i)->similar(*rsedge1[j]))
					{
						rsedge2[j] = face2->getEdge(i);
					}
				}
			}
			// find the similar vertices
			std::vector<TRSVertex<T>*> rsvertex1(3);
			std::vector<TRSVertex<T>*> rsvertex2(3);
			rsvertex1[0] = face1->getVertex(0);
			rsvertex1[1] = face1->getVertex(1);
			rsvertex1[2] = face1->getVertex(2);
			for (Position j = 0; j < 3; j++)
			{
				for (Position i = 0; i < 3; i++)
				{
					if (face2->getVertex(i)->similar(*rsvertex1[j]))
					{
						rsvertex2[j] = face2->getVertex(i);
					}
				}
			}
			// join the similar vertices and delete the faces in their face-lists
			for (Position i = 0; i < 3; i++)
			{
				if (rsvertex1[i] != rsvertex2[i])
				{
					rsvertex1[i]->join(*rsvertex2[i]);
					rsvertex2[i]->substitute(rsvertex1[i]);
					vertices_[rsvertex2[i]->getIndex()] = NULL;
					delete rsvertex2[i];
				}
				rsvertex1[i]->deleteFace(face1);
				rsvertex1[i]->deleteFace(face2);
			}
			// correct the edges
			for (Position i = 0; i < 3; i++)
			{
				if (rsedge1[i] == rsedge2[i])
				{
					rsedge1[i]->getVertex(0)->deleteEdge(rsedge1[i]);
					rsedge1[i]->getVertex(1)->deleteEdge(rsedge1[i]);
					edges_[rsedge1[i]->getIndex()] = NULL;
					delete rsedge1[i];
				}
				else
				{
					TRSFace<T>* neighbour2 = rsedge2[i]->other(face2);
					if (rsedge1[i]->getFace(0) == face1)
					{
						rsedge1[i]->setFace(0,neighbour2);
					}
					else
					{
						rsedge1[i]->setFace(1,neighbour2);
					}
					for (Position j = 0; j < 3; j++)
					{
						if (neighbour2->getEdge(j) == rsedge2[i])
						{
							neighbour2->setEdge(j,rsedge1[i]);
						}
					}
					rsedge2[i]->getVertex(0)->deleteEdge(rsedge2[i]);
					rsedge2[i]->getVertex(1)->deleteEdge(rsedge2[i]);
					edges_[rsedge2[i]->getIndex()] = NULL;
					delete rsedge2[i];
				}
			}
			faces_[face1->getIndex()] = NULL;
			faces_[face2->getIndex()] = NULL;
			delete face1;
			delete face2;
		}
	}


	template <typename T>
	void TReducedSurface<T>::compute()
		throw()
	{
		// indices of the atoms that do not belong to reduced surface
		HashSet<Index> indices;
		// all atom indices
		HashSet<Index> all_indices;
		for (Position i = 0; i < atom_.size(); i++)
		{
			indices.insert(i);
			all_indices.insert(i);
		}
		// all new created vertices wich are not yet checked for extensions
		HashSet<TRSVertex<T>*> new_vertices;
		TRSVertex<T>* start_vertex(NULL);
		TRSEdge<T>* start_edge(NULL);
		TRSFace<T>* start_face(NULL);
		// for each atom a list of the rsvertices of the atom
		std::vector< std::list<TRSVertex<T>*> > vertices(atom_.size());
		Byte start = 1;
		//while (start != 0)
		//{
			start = getStartPosition(indices,new_vertices,
															 start_vertex,start_edge,start_face,
															 vertices);
			try
			{
				switch (start)
				{
					case 2 :	extendComponent(indices,new_vertices,vertices);
										break;
					case 3 :	getRSComponent(start_face,indices,new_vertices,vertices);
										break;
					default :	break;
				}
			}
			catch (Exception::GeneralException e)
			{
				if (e.getMessage() == "probe touches four atoms")
				{
					clear();
					indices = all_indices;
							std::cerr << "\nProbeSphere berührt vier Atome, starte neu ...\n\n";
					compute();
					return;
				}
				else
				{
					throw;
				}
			}				
		//}
		clean();
	}


	template <typename T>
	void TReducedSurface<T>::getRSComponent
		 (TRSFace<T>*																	 face,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
		throw()
	{
		treatFace(face,indices,new_vertices,vertices);
		extendComponent(indices,new_vertices,vertices);
	}


	template <typename T>
	void TReducedSurface<T>::treatFace
		 (TRSFace<T>*																	 face,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw()
	{
		TRSEdge<T>* edge;
		for (Position i = 0; i < 3; i++)
		{
			edge = face->getEdge(i);
			if (edge->getFace(1) == NULL)
			{
				treatEdge(edge,indices,new_vertices,vertices);
			}
		}
	}


	template <typename T>
	void TReducedSurface<T>::treatEdge
		 (TRSEdge<T>*																	 edge,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
		throw()
	{
		// find third atom
		TAngle<T> phi;
		TSphere3<T> probe;
		TRSFace<T>* start_face(edge->getFace(0));
		TRSVertex<T>* vertex1(edge->getVertex(0));
		TRSVertex<T>* vertex2(edge->getVertex(1));
		TRSVertex<T>* vertex3(NULL);
		Index atom1(vertex1->getAtom());
		Index atom2(vertex2->getAtom());
		Index atom3(thirdAtom(vertex1,vertex2,start_face,probe,phi));
		if (atom3 == -1)
		{
			throw Exception::GeneralException(__FILE__,__LINE__,"CanNotTreatEdge","no third atom found");
		}
		TSphere3<T> sphere1(atom_[atom1]);
		TSphere3<T> sphere2(atom_[atom2]);
		TSphere3<T> sphere3(atom_[atom3]);
		// build a new face and two new edges
		vertex3 = new TRSVertex<T>(atom3);
		TRSEdge<T>* edge1;
		TRSEdge<T>* edge2;
		TRSFace<T>* new_face
				= new TRSFace<T>(vertex1,vertex2,vertex3,NULL,NULL,NULL,
												 probe.p,getFaceNormal(sphere1,sphere2,sphere3,probe),false,-1);
		TRSFace<T>* test = faceExists(new_face,vertices[vertex1->getAtom()]);
		if (test == NULL)
		{
			vertex3->setIndex(number_of_vertices_);
			vertices_.push_back(vertex3);
			number_of_vertices_++;
			indices.erase(atom3);
			new_vertices.insert(vertex3);
			vertices[atom3].push_back(vertex3);
			edge1 = new TRSEdge<T>;
			edge2 = new TRSEdge<T>;
			updateFaceAndEdges(vertex1,vertex2,vertex3,probe,6,new_face,edge,edge1,edge2);
			new_face->setIndex(number_of_faces_);
			faces_.push_back(new_face);
			number_of_faces_++;
			vertex1->pushFace(new_face);
			vertex2->pushFace(new_face);
			vertex3->pushFace(new_face);
			vertex1->pushEdge(edge2);
			vertex2->pushEdge(edge1);
			vertex3->pushEdge(edge1);
			vertex3->pushEdge(edge2);
		}
		else
		{
			Index i = 0;
			TRSEdge<T>* test_edge(test->getEdge(0));
			while (test_edge->similar(*edge) == false)
			{
				i++;
				if (i > 2)
				{
					throw Exception::GeneralException
							(__FILE__,__LINE__,"CanNotTreatEdge",
							 "corresponding edge in face not found");
				}
				test_edge = test->getEdge(i);
			}
			TRSVertex<T>* test_vertex1 = test_edge->getVertex(0);
			TRSVertex<T>* test_vertex2 = test_edge->getVertex(1);
			if (test_vertex1->similar(*vertex1))
			{
				if (*vertex1 != *test_vertex1)
				{
					vertex1->join(*test_vertex1);
					test_vertex1->substitute(vertex1);
					vertices_[test_vertex1->getIndex()] = NULL;
					new_vertices.erase(test_vertex1);
					vertices[test_vertex1->getAtom()].remove(test_vertex1);
					delete test_vertex1;
				}
				if (*vertex2 != *test_vertex2)
				{
					vertex2->join(*test_vertex2);
					test_vertex2->substitute(vertex2);
					vertices_[test_vertex2->getIndex()] = NULL;
					new_vertices.erase(test_vertex2);
					vertices[test_vertex2->getAtom()].remove(test_vertex2);
					delete test_vertex2;
				}
				vertex1->deleteEdge(test_edge);
				vertex2->deleteEdge(test_edge);
			}
			else
			{
				if (*vertex1 != *test_vertex2)
				{
					vertex1->join(*test_vertex2);
					test_vertex2->substitute(vertex1);
					vertices_[test_vertex2->getIndex()] = NULL;
					new_vertices.erase(test_vertex2);
					vertices[test_vertex2->getAtom()].remove(test_vertex2);
					delete test_vertex2;
				}
				if (*vertex2 != *test_vertex1)
				{
					vertex2->join(*test_vertex1);
					test_vertex1->substitute(vertex2);
					vertices_[test_vertex1->getIndex()] = NULL;
					new_vertices.erase(test_vertex1);
					vertices[test_vertex1->getAtom()].remove(test_vertex1);
					delete test_vertex1;
				}
				vertex1->deleteEdge(test_edge);
				vertex2->deleteEdge(test_edge);
			}
			test->setEdge(i,edge);
			delete new_face;
			delete test_edge;
			delete vertex3;
			new_face = test;
		}
		// update edge
		TCircle3<T> circle1;
		TCircle3<T> circle2;
		TCircle3<T> circle3;
		getCircles(atom1,atom2,circle1,circle2,circle3);
		TVector3<T> ip1;
		TVector3<T> ip2;
		TLine3<T> line(sphere1.p,sphere2.p,TLine3<T>::FORM__TWO_POINTS);
		bool singular(GetIntersection(probe,line,ip1,ip2));
		if (Maths::isLess(ip1.getDistance(sphere2.p),ip2.getDistance(sphere2.p)))
		{
			ip1.swap(ip2);
		}
		edge->set(vertex1,vertex2,start_face,new_face,circle1.p,circle1.radius,
							phi,circle2,circle3,ip1,ip2,singular,number_of_edges_);
		edges_.push_back(edge);
		number_of_edges_++;
		//new_edges.remove(edge);
		treatFace(edge->getFace(2),indices,new_vertices,vertices);
	}


	template <typename T>
	void TReducedSurface<T>::extendComponent
		 (HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw()
	{
		while (new_vertices.size() > 0)
		{
			TRSFace<T>* face = NULL;
			TRSVertex<T>* vertex1 = *new_vertices.begin();
			new_vertices.erase(new_vertices.begin());
  		Index atom1(vertex1->getAtom());
 			HashSet<Index> atom_list;
			neighboursOfOneAtom(atom1,indices,atom_list);
			HashSet<Index>::Iterator i = atom_list.begin();
			while (i != atom_list.end())
			{
				Index atom2 = *i;
				list< pair< Index,TSphere3<T> > >
						candidates(findThirdAtom(atom1,atom2,atom_list));
				if (candidates.size() == 0)
				{
					TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
					TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,atom_list);
					if (edge != NULL)
					{
						edge->setIndex(number_of_edges_);
						edges_.push_back(edge);
						number_of_edges_++;
						vertex1->pushEdge(edge);
						vertex2->pushEdge(edge);
						vertex2->setIndex(number_of_vertices_);
						vertices_.push_back(vertex2);
						number_of_vertices_++;
						indices.erase(atom2);
						vertices[atom2].push_back(vertex2);
						new_vertices.insert(vertex1);
						new_vertices.insert(vertex2);
						i = atom_list.end();
					}
					else
					{
						delete vertex2;
						i++;
					}
				}
				else
				{
					bool found = false;
					list< pair< Index,TSphere3<T> > >::iterator j = candidates.begin();
					while (j != candidates.end())
					{
						Index atom3 = j->first;
						TSphere3<T> probe = j->second;
						if (checkProbe(probe) == true)
						{
							face = new TRSFace<T>();
							TRSEdge<T>* edge1 = new TRSEdge<T>();
							TRSEdge<T>* edge2 = new TRSEdge<T>();
							TRSEdge<T>* edge3 = new TRSEdge<T>();
							TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
							TRSVertex<T>* vertex3 = new TRSVertex<T>(atom3);
							vertices[atom2].push_back(vertex2);
							vertices[atom3].push_back(vertex3);
							updateFaceAndEdges(vertex1,vertex2,vertex3,probe,7,
																 face,edge1,edge2,edge3);
							face->setIndex(number_of_faces_);
							faces_.push_back(face);
							number_of_faces_++;
							vertex2->setIndex(number_of_vertices_);
							vertices_.push_back(vertex2);
							number_of_vertices_++;
							indices.erase(atom2);
							vertex3->setIndex(number_of_vertices_);
							vertices_.push_back(vertex3);
							number_of_vertices_++;
							indices.erase(atom3);
							vertex1->pushFace(face);
							vertex2->pushFace(face);
							vertex3->pushFace(face);
							vertex1->pushEdge(edge1);
							vertex2->pushEdge(edge1);
							vertex2->pushEdge(edge2);
							vertex3->pushEdge(edge2);
							vertex3->pushEdge(edge3);
							vertex1->pushEdge(edge3);
							new_vertices.insert(vertex1);
							new_vertices.insert(vertex2);
							new_vertices.insert(vertex3);
							i = atom_list.end();
							j = candidates.end();
							found = true;
						}
						else
						{
							j++;
						}
					} // while j
					if (found == false)
					{
						i++;
					}
				}
			} // while i
			if (face != NULL)
			{
				getRSComponent(face,indices,new_vertices,vertices);
			}
		}
	}


	template <typename T>
	Index TReducedSurface<T>::thirdAtom
		 (TRSVertex<T>* vertex1,
			TRSVertex<T>* vertex2,
			TRSFace<T>*		face,
			TSphere3<T>&	probe,
			TAngle<T>&		phi)
		throw()
	{
		Index atom1(vertex1->getAtom());
		Index atom2(vertex2->getAtom());
		TSphere3<T> sphere1(atom_[atom1]);
		TSphere3<T> sphere2(atom_[atom2]);
		sphere1.radius += probe_radius_;
		sphere2.radius += probe_radius_;
		TCircle3<T> circle;
		GetIntersection(sphere1,sphere2,circle);
		HashSet<Index> atom_list;
		tree_->get(circle.p,circle.radius+r_max_+probe_radius_,atom_list);
		if (atom_list.size() == 0)
		{
			throw Exception::GeneralException
							(__FILE__,__LINE__,
							 "CanNotFindThirdAtom","no atom close enougth");
		}
		list< pair< Index,TSphere3<T> > > candidates
					= findThirdAtom(atom1,atom2,atom_list);
		list< pair< Index,TSphere3<T> > >::iterator k;
		Index back(-1);
		TAngle<T> old_angle(2*Constants::PI,true);
		TAngle<T> new_angle;
		TAngle<T> pi(Constants::PI,true);
		TVector3<T> norm = atom_[atom1].p-atom_[atom2].p;
		TVector3<T> test_vector = face->getNormal()%norm;
		if (Maths::isLess
					(test_vector*atom_[face->thirdVertex(vertex1,vertex2)->getAtom()].p,
					 test_vector*atom_[atom1].p)      )
		{
			norm.negate();
		}
		TVector3<T> v1 = face->getCenter()-circle.p;
		for (k = candidates.begin(); k != candidates.end(); k++)
		{
			if (((k->first == face->getVertex(0)->getAtom()) ||
					 (k->first == face->getVertex(1)->getAtom()) ||
					 (k->first == face->getVertex(2)->getAtom())   ) &&
					Maths::isGreaterOrEqual
					   (face->getNormal()*k->second.p,face->getNormal()*sphere1.p) )
			{
				// found the starting face
			}
			else
			{
				TVector3<T> v2 = k->second.p-circle.p;
						// ACHTUNG !!!
				new_angle = getOrientedAngle(v1,v2,norm)+pi;
				/*new_angle = getOrientedAngle(v1,v2,norm);
				if (Maths::isLess(new_angle.toRadian(),(T)0))
				{
					new_angle += pi;
					new_angle += pi;
				}*/
				if (new_angle == TAngle<T>(0,true))
				{
					atom_[k->first].radius -= 10*Constants::EPSILON;
					throw Exception::GeneralException
							(__FILE__,__LINE__,"can't compute RS","probe touches four atoms");
				}
				if (new_angle < old_angle)
				{
					old_angle = new_angle;
					back = k->first;
					probe = k->second;
					phi = new_angle;
				}
			}
		}
		return back;
	}


	template <typename T>
	Byte TReducedSurface<T>::getStartPosition
		 (HashSet<Index>&														 indices,
			HashSet<TRSVertex<T>*>&										 new_vertices,
			TRSVertex<T>*&														 vertex,
			TRSEdge<T>*&															 edge,
			TRSFace<T>*&															 face,
			std::vector< std::list< TRSVertex<T>* > >& vertices)
		throw()
	{
		face = findFirstFace(indices);
		if (face != NULL)
		{
			TRSVertex<T>* vertex1 = face->getVertex(0);
			TRSVertex<T>* vertex2 = face->getVertex(1);
			TRSVertex<T>* vertex3 = face->getVertex(2);
			new_vertices.insert(vertex1);
			new_vertices.insert(vertex2);
			new_vertices.insert(vertex3);
			vertices[vertex1->getAtom()].push_back(vertex1);
			vertices[vertex2->getAtom()].push_back(vertex2);
			vertices[vertex3->getAtom()].push_back(vertex3);
			return 3;
		}
		edge = findFirstEdge(indices);
		if (edge != NULL)
		{
			TRSVertex<T>* vertex1 = edge->getVertex(0);
			TRSVertex<T>* vertex2 = edge->getVertex(1);
			new_vertices.insert(vertex1);
			new_vertices.insert(vertex2);
			vertices[vertex1->getAtom()].push_back(vertex1);
			vertices[vertex2->getAtom()].push_back(vertex2);
			return 2;
		}
		vertex = findFirstVertex(indices);
		if (vertex != NULL)
		{
			new_vertices.insert(vertex);
			vertices[vertex->getAtom()].push_back(vertex);
			return 1;
		}
		return 0;
	}


	template <typename T>
	TRSFace<T>* TReducedSurface<T>::findFirstFace(HashSet<Index>& indices)
		throw()
	{
		for (Byte direction = 0; direction < 3; direction++)
		{
			for (Byte extrem = 0; extrem < 1; extrem++)
			{
				TRSFace<T>* face = findFace(indices,direction,extrem);
				if (face != NULL)
				{
					return face;
				}
			}
		}
		return NULL;
	}


	template <class T>
	TRSFace<T>* TReducedSurface<T>::findFace
		 (HashSet<Index>& indices,
			Byte						direction,
			Byte						extrem)
		throw()
	{
		if (indices.size() == 0)
		{
			return NULL;
		}
		Index a1 = findFirstAtom(indices,direction,extrem);
		HashSet<Index> atom_list;
		tree_->get(atom_[a1].p,atom_[a1].radius+r_max_+2*probe_radius_,atom_list);
		Index a2 = findSecondAtom(a1,atom_list,direction,extrem);
		if (a2 == -1)
		{
			return NULL;
		}
		TSphere3<T> s1(atom_[a1].p,atom_[a1].radius+probe_radius_);
		TSphere3<T> s2(atom_[a2].p,atom_[a2].radius+probe_radius_);
		TCircle3<T> circle;
		GetIntersection(s1,s2,circle);
		atom_list.clear();
		tree_->get(circle.p,circle.radius+r_max_+probe_radius_,atom_list);
		list< pair< Index,TSphere3<T> > >
				candidates = findThirdAtom(a1,a2,atom_list);
		if (candidates.size() == 0)
		{
			return NULL;
		}
		list< pair< Index,TSphere3<T> > >::iterator i = candidates.begin();
		Index a3 = candidates.begin()->first;
		TSphere3<T> probe = candidates.begin()->second;
		while (checkProbe(probe) == false)
		{
			i++;
			if (i == candidates.end())
			{
				return NULL;
			}
			a3 = i->first;
			probe = i->second;
		}
		TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
		TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
		TRSVertex<T>* vertex3 = new TRSVertex<T>(a3);
		TRSEdge<T>* e1 = new TRSEdge<T>();
		TRSEdge<T>* e2 = new TRSEdge<T>();
		TRSEdge<T>* e3 = new TRSEdge<T>();
		TRSFace<T>* face = new TRSFace<T>();
		updateFaceAndEdges(vertex1,vertex2,vertex3,probe,7,face,e1,e2,e3);
		face->setIndex(number_of_faces_);
		faces_.push_back(face);
		number_of_faces_++;
		vertex1->pushFace(face);
		vertex2->pushFace(face);
		vertex3->pushFace(face);
		vertex1->pushEdge(e1);
		vertex2->pushEdge(e1);
		vertex2->pushEdge(e2);
		vertex3->pushEdge(e2);
		vertex3->pushEdge(e3);
		vertex1->pushEdge(e3);
		indices.erase(a1);
		indices.erase(a2);
		indices.erase(a3);
		vertex1->setIndex(number_of_vertices_);
		vertices_.push_back(vertex1);
		number_of_vertices_++;
		vertex2->setIndex(number_of_vertices_);
		vertices_.push_back(vertex2);
		number_of_vertices_++;
		vertex3->setIndex(number_of_vertices_);
		vertices_.push_back(vertex3);
		number_of_vertices_++;
		return face;
	}


	template <typename T>
	TRSEdge<T>* TReducedSurface<T>::findFirstEdge(HashSet<Index>& indices)
		throw()
	{
		for (Byte direction = 0; direction < 3; direction++)
		{
			for (Byte extrem = 0; extrem < 1; extrem++)
			{
				TRSEdge<T>* edge = findEdge(indices,direction,extrem);
				if (edge != NULL)
				{
					return edge;
				}
			}
		}
		return NULL;
	}


	template <typename T>
	TRSEdge<T>* TReducedSurface<T>::findEdge
		 (HashSet<Index>& indices,
			Byte						direction,
			Byte						extrem)
		throw()
	{
		if (indices.size() == 0)
		{
			return NULL;
		}
		Index a1 = findFirstAtom(indices,direction,extrem);
		HashSet<Index> atom_list;
		tree_->get(atom_[a1].p,atom_[a1].radius+r_max_+2*probe_radius_,atom_list);
		Index a2 = findSecondAtom(a1,atom_list,direction,extrem);
		if (a2 == -1)
		{
			return NULL;
		}
		TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
		TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
		HashSet<Index> atoms;
		neighboursOfTwoAtoms(a1,a2,atom_list,atoms);
		TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,atoms);
		if (edge != NULL)
		{
			edge->setIndex(number_of_edges_);
			edges_.push_back(edge);
			number_of_edges_++;
			vertex1->pushEdge(edge);
			vertex1->setIndex(number_of_vertices_);
			vertices_.push_back(vertex1);
			number_of_vertices_++;
			indices.erase(a1);
			vertex2->pushEdge(edge);
			vertex2->setIndex(number_of_vertices_);
			vertices_.push_back(vertex2);
			number_of_vertices_++;
			indices.erase(a2);
		}
		else
		{
			delete vertex1;
			delete vertex2;
		}
		return edge;
	}


	template <typename T>
	TRSVertex<T>* TReducedSurface<T>::findFirstVertex(HashSet<Index>& indices)
		throw()
	{
		if (indices.size() == 0)
		{
			return NULL;
		}
		for (Byte direction = 0; direction < 3; direction++)
		{
			for (Byte extrem = 0; extrem < 1; extrem++)
			{
				Index v = findFirstAtom(indices,direction,extrem);
				if (v != -1)
				{
					TRSVertex<T>* vertex = new TRSVertex<T>(v);
					vertex->setIndex(number_of_vertices_);
					vertices_.push_back(vertex);
					number_of_vertices_++;
					indices.erase(v);
					return vertex;
				}
			}
		}
		return NULL;
	}


	template <typename T>
	Index TReducedSurface<T>::findFirstAtom
		 (const HashSet<Index>& indices,
			Byte									direction,
			Byte									extrem)
		throw()
	{
		TSphere3<T> next_atom(atom_[*indices.begin()]);
		T dir_ex = ((extrem == 0) ? next_atom.p[direction]-next_atom.radius
															: next_atom.p[direction]+next_atom.radius);
		Index a_ex = *indices.begin();
		HashSet<Index>::ConstIterator i;
		for (i = indices.begin(); i != indices.end(); i++)
		{
			next_atom = atom_[*i];
			T extremum = ((extrem == 0) ? next_atom.p[direction]-next_atom.radius
																	: next_atom.p[direction]+next_atom.radius);
			if (((extrem == 0) && Maths::isLess(extremum,dir_ex)) ||
					((extrem != 0) && Maths::isGreater(extremum,dir_ex)))
			{
				dir_ex = extremum;
				a_ex = *i;
			}
		}
		return a_ex;
	}


	template <typename T>
	Index TReducedSurface<T>::findSecondAtom
		 (Index					 atom1,
			HashSet<Index> atom_list,
			Byte					 direction,
			Byte					 extrem)
		throw()
	{
		HashSet<Index> neighbour;
		neighboursOfOneAtom(atom1,atom_list,neighbour);
		if (neighbour.size() == 0)
		{
			return -1;
		}
		TSphere3<T> first_atom(atom_[atom1]);
		first_atom.radius += probe_radius_;
		TCircle3<T> intersection_circle;
		T x_min = ((extrem == 0) ? first_atom.p[direction]+first_atom.radius
														 : first_atom.p[direction]-first_atom.radius);
		Index a_min = -1;
		HashSet<Index>::Iterator i;
		for (i = neighbour.begin(); i != neighbour.end(); i++)
		{
			TSphere3<T> next_atom(atom_[*i]);
			next_atom.radius += probe_radius_;
			if (GetIntersection(first_atom,next_atom,intersection_circle))
			{
				T next_min = getCircleExtremum(intersection_circle, direction, extrem);
				if (((extrem == 0) && Maths::isLess(next_min,x_min)) ||
						((extrem != 0) && Maths::isGreater(next_min,x_min)))
				{
					x_min = next_min;
					a_min = *i;
				}
			}
		}
		return a_min;
	}


	template <class T>
	::std::list< ::std::pair< Index,TSphere3<T> > > TReducedSurface<T>::findThirdAtom
		 (Index									atom1,
			Index									atom2,
			const HashSet<Index>& third)
		throw()
	{
		list< pair< Index,TSphere3<T> > > back;
		if (third.size() == 0)
		{
			throw Exception::GeneralException
					(__FILE__,__LINE__,"CanNotFindThirdAtom","no third atom found");
		}
		pair< Index,TSphere3<T> > candidate;
		HashSet<Index>::ConstIterator i = third.begin();
		while (i != third.end())
		{
			TVector3<T> center1, center2;
			if (centerOfProbe(atom1,atom2,*i,center1,center2))
			{
				TSphere3<T> probe(center1,probe_radius_);
				candidate.first = *i;
				candidate.second = probe;
				back.push_back(candidate);
				probe.p = center2;
				candidate.first = *i;
				candidate.second = probe;
				back.push_back(candidate);
			}
			i++;
		}
		return back;
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfOneAtom
		 (Index									atom0,
			const HashSet<Index>& input_set,
			HashSet<Index>&				output_set)
		throw()
	{
		TSphere3<T> first_atom(atom_[atom0]);
		TSphere3<T> next_atom;
		T offset = first_atom.radius+2*probe_radius_;
		HashSet<Index>::ConstIterator i;
		for (i = input_set.begin(); i != input_set.end(); i++)
		{
			if (*i != atom0)
			{
				next_atom = atom_[*i];
				T dist = next_atom.p.getDistance(first_atom.p);
				T max_dist = next_atom.radius+offset;
				if (Maths::isLessOrEqual(dist,max_dist))
				{
					output_set.insert(*i);
				}
			}
		}
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfTwoAtoms
		 (Index									atom1,
			Index									atom2,
			const HashSet<Index>& input_set,
			HashSet<Index>&				output_set)
		throw()
	{
		TSphere3<T> first_atom(atom_[atom1]);
		first_atom.radius += probe_radius_;
		TSphere3<T> second_atom(atom_[atom2]);
		second_atom.radius += probe_radius_;
		TCircle3<T> intersection_circle;
		GetIntersection(first_atom,second_atom,intersection_circle);
		HashSet<Index>::ConstIterator i;
		for (i = input_set.begin(); i != input_set.end(); i++)
		{
			if ((*i != atom1) && (*i != atom2))
			{
				T dist = atom_[*i].p.getDistance(intersection_circle.p);
				T max_dist
						= probe_radius_+intersection_circle.radius+atom_[*i].radius;
				if (Maths::isLess(dist,max_dist))
				{
					output_set.insert(*i);
				}
			}
		}
	}


	template <typename T>
	T TReducedSurface<T>::getCircleExtremum
		 (const TCircle3<T>& circle,
			Position					 direction,
			Position					 extrem)
		throw()
	{
		T min, max;
		TVector3<T> norm2(circle.n.x * circle.n.x,
											circle.n.y * circle.n.y,
											circle.n.z * circle.n.z);
		switch (direction)
		{
			case 0 :
				if (Maths::isZero(circle.n.y) && Maths::isZero(circle.n.z))
				{
					min = max = circle.p.x;
				}
				else
				{
					T x_norm = norm2.y + norm2.z;
					x_norm /= norm2.x+x_norm;
					x_norm = circle.radius * sqrt(x_norm);
					min = (circle.p.x) - x_norm;
					max = (circle.p.x) + x_norm;
				}
				break;
			case 1 :
				if (Maths::isZero(circle.n.x) && Maths::isZero(circle.n.z))
				{
					min = max = circle.p.y;
				}
				else
				{
					T y_norm = norm2.x + norm2.z;
					y_norm /= norm2.y + y_norm;
					y_norm = circle.radius * sqrt(y_norm);
					min = (circle.p.y)-y_norm;
					max = (circle.p.y)+y_norm;
				}
				break;
			case 2 :
				if (Maths::isZero(circle.n.x) && Maths::isZero(circle.n.y))
				{
					min = max = circle.p.z;
				}
				else
				{
					T z_norm = norm2.x + norm2.y;
					z_norm /= norm2.z + z_norm;
					z_norm = circle.radius * sqrt(z_norm);
					min = circle.p.z - z_norm;
					max = circle.p.z + z_norm;
				}
				break;
		}
		if (extrem == 0)
		{
			return Maths::min(min, max);
		}
		else
		{
			return Maths::max(min, max);
		}
	}


	template <typename T>
	TRSEdge<T>* TReducedSurface<T>::createFreeEdge
			 (TRSVertex<T>*					vertex1,
				TRSVertex<T>*					vertex2,
				const HashSet<Index>& neighbours)
		throw()
	{
		TCircle3<T> circle1;
		TCircle3<T> circle2;
		TCircle3<T> circle3;
		if (getCircles(vertex1->getAtom(),vertex2->getAtom(),
									 circle1,circle2,circle3) &&
				Maths::isGreater(circle1.radius,probe_radius_)		 )
		{
			TPlane3<T> plane(circle1.p,circle1.n);
			HashSet<Index>::ConstIterator i;
			for (i = neighbours.begin(); i != neighbours.end(); i++)
			{
				TCircle3<T> test_circle;
				TSphere3<T> sphere(atom_[*i].p,atom_[*i].radius+probe_radius_);
				if (GetIntersection(sphere,plane,test_circle))
				{
					if (Maths::isGreaterOrEqual(
												Maths::abs(test_circle.radius-circle1.radius),
												test_circle.p.getDistance(circle1.p)          ) )
					{
						return NULL;
					}
				}
			}
			TVector3<T> vector(0,0,0);
			TRSEdge<T>* edge
					 = new TRSEdge<T>(vertex1,vertex2,NULL,NULL,
					 									circle1.p,circle1.radius,
														TAngle<T>(2*Constants::PI,true),
														circle2,circle3,
														vector,vector,false,-1);
			return edge;
		}
		return NULL;
	}


	template <typename T>
	bool TReducedSurface<T>::getCircles
		 (Index atom1,
			Index atom2,
			TCircle3<T>& circle1,
			TCircle3<T>& circle2,
			TCircle3<T>& circle3)
		throw()
	{
		TSphere3<T> sphere1(atom_[atom1]);
		TSphere3<T> sphere2(atom_[atom2]);
		sphere1.radius += probe_radius_;
		sphere2.radius += probe_radius_;
		if (GetIntersection(sphere1,sphere2,circle1))
		{
			T ratio1(atom_[atom1].radius/sphere1.radius);
			circle2.radius = circle1.radius*ratio1;
			circle2.p = sphere1.p+(circle1.p-sphere1.p)*(ratio1);
			T ratio2(atom_[atom2].radius/sphere2.radius);
			circle3.radius = circle1.radius*ratio2;
			circle3.p = sphere2.p+(circle1.p-sphere2.p)*(ratio2);
			return true;
		}
		return false;
	}


	template <typename T>
	TVector3<T> TReducedSurface<T>::getFaceNormal
		 (const TSphere3<T>& atom1,
			const TSphere3<T>& atom2,
			const TSphere3<T>& atom3,
			const TSphere3<T>& probe)
		throw()
	{
		TPlane3<T> plane(atom1.p,atom2.p,atom3.p);
		TVector3<T> norm(plane.n);
		if (Maths::isLess(norm*probe.p,norm*atom1.p))
		{
			norm.negate();
		}
		norm.normalize();
		return norm;
	}


	template <typename T>
	void TReducedSurface<T>::updateFaceAndEdges
		 (TRSVertex<T>* v1,
			TRSVertex<T>* v2,
			TRSVertex<T>* v3,
			const TSphere3<T>& probe,
			Byte mode,
			TRSFace<T>* f,
			TRSEdge<T>* e1,
			TRSEdge<T>* e2,
			TRSEdge<T>* e3)
		throw()
	{
		TVector3<T> vector(0,0,0);
		TAngle<T> angle(0,true);
		TCircle3<T> circle(vector,vector,0);
		if ((mode & 1) == 1)
		{
			e1->set(v1,v2,f,NULL,vector,(T)0,angle,circle,circle,
							vector,vector,false,-1);
		}
		if ((mode & 2) == 2)
		{
			e2->set(v2,v3,f,NULL,vector,(T)0,angle,circle,circle,
							vector,vector,false,-1);
		}
		if ((mode & 4) == 4)
		{
			e3->set(v3,v1,f,NULL,vector,(T)0,angle,circle,circle,
							vector,vector,false,-1);
		}
		TVector3<T> norm(f->getNormal());
		if (norm == vector)
		{
			norm = getFaceNormal(atom_[v1->getAtom()],
													 atom_[v2->getAtom()],
													 atom_[v3->getAtom()],
													 probe);
		}
		TPlane3<T> plane(atom_[v1->getAtom()].p,
										 atom_[v2->getAtom()].p,
										 atom_[v3->getAtom()].p);
		TCircle3<T> dummy;
		f->set(v1,v2,v3,e1,e2,e3,probe.p,norm,GetIntersection(probe,plane,dummy),-1);
	}


	template <typename T>
	TRSFace<T>* TReducedSurface<T>::faceExists
		 (TRSFace<T>* face,
			const std::list< TRSVertex<T>* >& vertices)
		throw()
	{
		std::list< TRSVertex<T>* >::const_iterator v;
		for (v = vertices.begin(); v != vertices.end(); v++)
		{
			TRSFace<T>* f = (*v)->has(face);
			if (f != NULL)
			{
				return f;
			}
		}
		return NULL;
	}


	template <typename T>
	bool TReducedSurface<T>::centerOfProbe
		 (Index				 a1,
			Index				 a2,
			Index				 a3,
			TVector3<T>& c1,
			TVector3<T>& c2)
		throw()
	{
		//TSphere3<T> s1(atom_[a1].p,atom_[a1].radius+probe_radius_);
		//TSphere3<T> s2(atom_[a2].p,atom_[a2].radius+probe_radius_);
		//TSphere3<T> s3(atom_[a3].p,atom_[a3].radius+probe_radius_);
		TSphere3<double> s1(TVector3<double>((double)atom_[a1].p.x,
																				 (double)atom_[a1].p.y,
																				 (double)atom_[a1].p.z ),
												(double)atom_[a1].radius+(double)probe_radius_);
		TSphere3<double> s2(TVector3<double>((double)atom_[a2].p.x,
																				 (double)atom_[a2].p.y,
																				 (double)atom_[a2].p.z ),
												(double)atom_[a2].radius+(double)probe_radius_);
		TSphere3<double> s3(TVector3<double>((double)atom_[a3].p.x,
																				 (double)atom_[a3].p.y,
																				 (double)atom_[a3].p.z ),
												(double)atom_[a3].radius+(double)probe_radius_);
		TVector3<double> p1;
		TVector3<double> p2;
		if (GetIntersection(s1,s2,s3,p1,p2))
		{
			c1 = TVector3<T>((T)p1.x,(T)p1.y,(T)p1.z);
			c2 = TVector3<T>((T)p2.x,(T)p2.y,(T)p2.z);
			return true;
		}
		else
		{
			return false;
		}
	}


	template <typename T>
	bool TReducedSurface<T>::checkProbe(const TSphere3<T>& probe)
		throw()
	{
		HashSet<Index> atom_list;
		tree_->get(probe.p,probe.radius+r_max_,atom_list);
		HashSet<Index>::Iterator i;
		for (i = atom_list.begin(); i != atom_list.end(); i++)
		{
			if (Maths::isLess(probe.p.getDistance(atom_[*i].p),
												probe.radius+atom_[*i].radius))
			{
				return false;
			}
		}
		return true;
	}



} // namespace BALL

#endif  // BALL_STRUCTURE_REDUCEDSURFACE_H
