// $Id: reducedSurface.h,v 1.24 2001/09/19 17:29:58 strobel Exp $

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#define BALL_STRUCTURE_REDUCEDSURFACE_H

//#define debug_rs
//#define print_rs_debug_info
#define debug_surface_processor
#ifdef debug_surface_processor
#	define debug_surface_processor_verbose
//#	define debug_surface_processor_print
#endif

#ifndef BALL_STRUCTURE_RSVERTEX_H
#	include <BALL/STRUCTURE/RSVertex.h>
#endif

#ifndef BALL_STRUCTURE_RSEDGE_H
#	include <BALL/STRUCTURE/RSEdge.h>
#endif

#ifndef BALL_STRUCTURE_RSFACE_H
#	include <BALL/STRUCTURE/RSFace.h>
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

#ifndef BALL_DATATYPE_HASHGRID_H
#	include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif


#include <vector>
#include <list>
#include <fstream>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/HINFile.h>


namespace BALL
{

			// debuging
			template <class T>
			string IndexToString(Index nr, const T&)
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


	/** Generic ReducedSurface Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/reducedSurface.h}
	*/
	template <typename T>
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
			throw(Exception::GeneralException);

		/**	Detailed constructor.
				Create a new ReducedSurface object from a list of spheres.
		*/
		TReducedSurface(const ::std::vector< TSphere3<T> >& spheres,
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

		/**	Assign from another ReducedSurface.
				@param reduced_surface the ReducedSurface object to assign from
		*/
		void operator=(const TReducedSurface<T>& reduced_surface)
			throw(Exception::GeneralException);

		/**	Assign from another ReducedSurface.
				@param reduced_surface the ReducedSurface object to assign from
		*/
		void set(const TReducedSurface<T>& reduced_surface)
			throw(Exception::GeneralException);

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
			throw(Exception::IndexOverflow);

		/** Return the vertices
				@return vector< TRSVertex<T>* > all vertices
		*/
		::std::vector< TRSVertex<T>* > getVertices()
			throw();

		/** Return the i'th rsvertex.
				@param	i	the index of the rsvertex that should be given back
				@return TSpherer3<T>, the i'th rsvertex
		*/
		TRSVertex<T>* getVertex(Position i)
			throw(Exception::IndexOverflow);

		/** Return the edges
				@return vector< TRSEdge<T>* > all edges
		*/
		::std::vector< TRSEdge<T>* > getEdges()
			throw();

		/** Return the i'th rsedge.
				@param	i	the index of the rsedge that should be given back
				@return TSpherer3<T>, the i'th rsedge
		*/
		TRSEdge<T>* getEdge(Position i)
			throw(Exception::IndexOverflow);

		/** Return the faces
				@return vector< TRSFace<T>* > all faces
		*/
		::std::vector< TRSFace<T>* > getFaces()
			throw();

		/** Return the i'th rsface.
				@param	i	the index of the rsface that should be given back
				@return TSpherer3<T>, the i'th rsface
		*/
		TRSFace<T>* getFace(Position i)
			throw(Exception::IndexOverflow);

		/** Delete a pair of similar faces
				@param	face1	a pointer to the first face
				@param	face2	a pointer to the second face
		*/
		void deleteSimilarFaces(TRSFace<T>* face1, TRSFace<T>* face2)
			throw();

		/** Compute the reduced surface
		*/
		void compute()
			throw(Exception::GeneralException,Exception::DivisionByZero);

		//@}


		private:

		/*_ @name Computing reduced surface
		*/
		//@{

		/*_ Compute the RSComponent of a given face
			@param	face					the starting face
			@param	indices				a HashSet of the indices of all atoms which are not	
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void getRSComponent
			 (TRSFace<T>*																	 /*face*/,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		/*_ treat all edges of a face
			@param	face					the RSFace to be treated
			@param	indices				a HashSet of the indices of all atoms which are not	
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void treatFace
			 (TRSFace<T>*																	 face,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		/*_ roll over an edge that belongs to onlyone face and find the other one
			@param	edge					the RSEdge to be treated
			@param	indices				a HashSet of the indices of all atoms which are not	
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void treatEdge
			 (TRSEdge<T>*																	 edge,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		void correct(Index atom,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  	throw();

		/*_ Check all new created vertices for extensions
			@param	indices				a HashSet of the indices of all atoms which are not	
														yet part of the reduced surface
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void extendComponent
			 (HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);


		/*_ Find a third atom rolling over two vertices starting on a face.
				From all atoms which can be touced by the probe sphere when it	
				touches the given two vertices this one is chosen, for which is	
				the rotation angle the smalest.	
				If the rotation angle equals zero, the probe sphere can touch four	
				atoms an an exception is thrown.	
				If no atom can be found an exception is thrown.
			@param	vertex1	the first vertex
			@param	vertex2	the second vertex
			@param	face		the starting face
			@param	probe		the new probe sphere
			@param	phi			the rotation angle
			@return	Index		index of the found atom
		*/
		Index thirdAtom
			 (TRSVertex<T>* 															 vertex1,
				TRSVertex<T>* 															 vertex2,
				TRSFace<T>*																	 face,
				TSphere3<T>&																 probe,
				TAngle<T>&																	 phi,
				HashSet<Index>&															 indices,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		//@}
		/*_ @name Finding a start position
		*/
		//@{

		/*_ Find a start position
			@param	indices				a HashSet of the indices of all atoms which are not	
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
			throw(Exception::DivisionByZero);

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
			throw(Exception::DivisionByZero);

		/*_ Try to find a starting face in a given direction
			@param	indices			a HashSet of the indices of all atoms which are not	
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
			throw(Exception::DivisionByZero);

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
		Index findSecondAtom(Index atom, Byte direction, Byte extrem)
			throw();

		/*_ Find a second atom close enougth to the first two atoms
			@param	atom1			the index of the first atom
			@param	atom2			the index of the second atom
			@param	atom_list	a HashSet of the indices of all candidate atoms
			@return	::std::list< ::std::pair< Index,TSphere3<T> > >
												a list of all candidates with their probe spheres
		*/
		void findThirdAtom
			 (Index																						 atom1,
				Index																						 atom2,
				const ::std::list<Index>&												 third,
				::std::list< ::std::pair< Index,TSphere3<T> > >& atoms)
			throw();

		//@}
		/*_ @name Some utilities
		*/
		//@{

		/*_ Find all atoms close enougth to a given atom.
			The indices of all atoms which can be touched by the probe sphere when	
			it touches the given atom are pushed to neighbours_[atom]. Then this	
			list is sorted.
			@param	atom	the index of the given atom
		*/
		void neighboursOfOneAtom(Index atom)
			throw();

		/*_ Find all atoms close enougth to two given atoms.
			The indices of all atoms which can be touched by the probe sphere when	
			it touches the given atoms are computed.
			@param	atom1				the index of the first given atom
			@param	atom2				the index of the second given atom
			@param	output_list	list of all atoms close enougth to the given atoms
		*/
		void neighboursOfTwoAtoms
			 (Index								atom1,
				Index							 	atom2,
				::std::list<Index>& output_list)
			throw();

		/*_ Find all atoms close enougth to three given atoms.
			The indices of all atoms which can be touched by the probe sphere when	
			it touches the given atoms are computed.
			@param	atom1				the index of the first given atom
			@param	atom2				the index of the second given atom
			@param	atom3				the index of the third given atom
			@param	output_list	list of all atoms close enougth to the given atoms
		*/
		void neighboursOfThreeAtoms
			 (Index								atom1,
				Index								atom2,
				Index								atom3,
				::std::list<Index>& output_list)
			throw();


    /*_ Get the extrem coordinate of a circle in a given direction
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

		/*_ Create a free edge from two vertices if it is a free edge
			@param	vertex1			a pointer to the first vertex
			@param	vertex2			a pointer to the second vertex
			@param	neighbours	list of indices of atoms in environment of the two
													vertices
			@return	TRSEdge<T>* a pointer to the created free edge, if there is one,
													NULL otherwise
		*/
		TRSEdge<T>* createFreeEdge
				 (TRSVertex<T>*							vertex1,
					TRSVertex<T>*							vertex2,
					const ::std::list<Index>& neighbours)
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
			throw(Exception::DivisionByZero);

		/*_ Update a face and it's edges
			@param	v1		the first vertex of the face
			@param	v2		the second vertex of the face
			@param	v3		the third vertex of the face
			@param	e1		the first edge. it is updated if mode & 1 == 1. it's the
										edge from v1 to v2
			@param	e2		the second edge. it is updated if mode & 2 == 2. it's the
										edge from v2 to v3
			@param	e3		the third edge. it is updated if mode & 4 == 4. it's the
										edge from v3 to v1
			@param	f			the face
			@param	probe	the probe sphere of the face
		*/
		void updateFaceAndEdges
			 (TRSVertex<T>* v1,
				TRSVertex<T>* v2,
				TRSVertex<T>* v3,
				TRSEdge<T>* e1,
				TRSEdge<T>* e2,
				TRSEdge<T>* e3,
				TRSFace<T>* f,
				const TSphere3<T>& probe)
			throw(Exception::DivisionByZero);

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
		//bool checkProbe(const TSphere3<T>& probe)
		bool checkProbe(const TSphere3<T>& probe,
				Index atom1,
				Index atom2,
				Index atom3)
			throw();

		//@}

		protected:

    /*_ the atoms of the molecule
    */
    std::vector< TSphere3<T> > atom_;
    /*_	probe radius
    */
    T probe_radius_;
    /*_ HasGrid of the atoms
    */
		HashGrid3<Position> grid_;
		/*_ for each atom a list of its neighbours
		*/
		std::vector< std::list<Index> > neighbours_;
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
				#ifdef debug_rs
  	  	std::ofstream print;
	  	  string pre;
				#endif
				#ifdef print_rs_debug_info
				int HALT;
				#endif
	
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
					s << "  " << rs.getVertex(i) << "  " << *(rs.getVertex(i)) << "\n";
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
					s << "  " << rs.getEdge(i) << "  " << *(rs.getEdge(i)) << "\n";
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
					s << "  " << rs.getFace(i) << "  " << *(rs.getFace(i)) << "\n";
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
			grid_(),
			neighbours_(),
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
		throw(Exception::GeneralException)
		: atom_(reduced_surface.atom_),
			probe_radius_(reduced_surface.probe_radius_),
			grid_(reduced_surface.grid_),
			neighbours_(reduced_surface.neighbours_),
			vertices_(reduced_surface.number_of_vertices_),
			edges_(reduced_surface.number_of_edges_),
			faces_(reduced_surface.number_of_faces_),
			r_max_(reduced_surface.r_max_),
			number_of_vertices_(reduced_surface.number_of_vertices_),
			number_of_edges_(reduced_surface.number_of_edges_),
			number_of_faces_(reduced_surface.number_of_faces_)
	{
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			if (reduced_surface.vertices_[i] != NULL)
			{
				if (reduced_surface.vertices_[i]->index_ < 0)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY REDUCED SURFACE",
																						"NEGATIVE INDEX FOUND");
				}
				else
				{
					vertices_[i] = new TRSVertex<T>(*reduced_surface.vertices_[i],false);
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY REDUCED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		for (Position i = 0; i < number_of_edges_; i++)
		{
			if (reduced_surface.edges_[i] != NULL)
			{
				if (reduced_surface.edges_[i]->index_ < 0)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY REDUCED SURFACE",
																						"NEGATIVE INDEX FOUND");
				}
				else
				{
					edges_[i] = new TRSEdge<T>(*reduced_surface.edges_[i],false);
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY REDUCED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		for (Position i = 0; i < number_of_faces_; i++)
		{
			if (reduced_surface.faces_[i] != NULL)
			{
				if (reduced_surface.faces_[i]->index_ < 0)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY REDUCED SURFACE",
																						"NEGATIVE INDEX FOUND");
				}
				else
				{
					faces_[i] = new TRSFace<T>(*reduced_surface.faces_[i],false);
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY REDUCED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		HashSet<TRSEdge<T>*>::ConstIterator e;
		HashSet<TRSFace<T>*>::ConstIterator f;
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			for (e = reduced_surface.vertices_[i]->edges_.begin();
					 e != reduced_surface.vertices_[i]->edges_.end();  e++)
			{
				vertices_[i]->edges_.insert(edges_[(*e)->index_]);
			}
			for (f = reduced_surface.vertices_[i]->faces_.begin();
					 f != reduced_surface.vertices_[i]->faces_.end();  f++)
			{
				vertices_[i]->faces_.insert(faces_[(*f)->index_]);
			}
		}
		for (Position i = 0; i < number_of_edges_; i++)
		{
			edges_[i]->vertex0_ = vertices_[reduced_surface.edges_[i]->vertex0_->index_];
			edges_[i]->vertex1_ = vertices_[reduced_surface.edges_[i]->vertex1_->index_];
			edges_[i]->face0_ = faces_[reduced_surface.edges_[i]->face0_->index_];
			edges_[i]->face1_ = faces_[reduced_surface.edges_[i]->face1_->index_];
		}
		for (Position i = 0; i < number_of_faces_; i++)
		{
			faces_[i]->vertex0_ = vertices_[reduced_surface.faces_[i]->vertex0_->index_];
			faces_[i]->vertex1_ = vertices_[reduced_surface.faces_[i]->vertex1_->index_];
			faces_[i]->vertex2_ = vertices_[reduced_surface.faces_[i]->vertex2_->index_];
			faces_[i]->edge0_ = edges_[reduced_surface.faces_[i]->edge0_->index_];
			faces_[i]->edge1_ = edges_[reduced_surface.faces_[i]->edge1_->index_];
			faces_[i]->edge2_ = edges_[reduced_surface.faces_[i]->edge2_->index_];
		}
	}


	template <typename T>
	TReducedSurface<T>::TReducedSurface
		 (const ::std::vector< TSphere3<T> >& spheres,
			const T& probe_radius)
		throw()
		:	atom_(),
			probe_radius_(probe_radius),
			grid_(),
			neighbours_(spheres.size()),
			vertices_(),
			edges_(),
			faces_(),
			r_max_((T)0),
			number_of_vertices_(0),
			number_of_edges_(0),
			number_of_faces_(0)
	{
				#ifdef debug_rs
				print.open("ReducedSurfaceDebugInfo.log");
				pre = "  ";
				print << "START\n";
				print << pre << "ReducedSurface(" << spheres.size() << ")\n";
				pre += "  ";
				#endif
		T x_min = spheres[0].p.x;
		T y_min = spheres[0].p.y;
		T z_min = spheres[0].p.z;
		T x_max = spheres[0].p.x;
		T y_max = spheres[0].p.y;
		T z_max = spheres[0].p.z;
		for (Position i = 0; i < spheres.size(); i++)
		{
			atom_.push_back(spheres[i]);
			r_max_ = ((r_max_ < spheres[i].radius) ? spheres[i].radius : r_max_);
			x_min = ((x_min > spheres[i].p.x) ? spheres[i].p.x : x_min);
			y_min = ((y_min > spheres[i].p.y) ? spheres[i].p.y : y_min);
			z_min = ((z_min > spheres[i].p.z) ? spheres[i].p.z : z_min);
			x_max = ((x_max < spheres[i].p.x) ? spheres[i].p.x : x_max);
			y_max = ((y_max < spheres[i].p.y) ? spheres[i].p.y : y_max);
			z_max = ((z_max < spheres[i].p.z) ? spheres[i].p.z : z_max);
		}
		T dist = 2*(r_max_+probe_radius_);
		Position nx = (Position)((x_max-x_min)/dist+5);
		Position ny = (Position)((y_max-y_min)/dist+5);
		Position nz = (Position)((z_max-z_min)/dist+5);
		TVector3<T> origin(x_min-2*dist,y_min-2*dist,z_min-2*dist);
		grid_ = HashGrid3<Position>(origin,nx,ny,nz,dist);
				#ifdef print_rs_debug_info
				std::cout << "HashGrid3( " << origin << " , " << nx << " , "
									<< ny << " , " << nz << " , " << dist << ") erzeugt\n";
				#endif
		for (Position i = 0; i < atom_.size(); i++)
		{
			grid_.insert(atom_[i].p,i);
					#ifdef print_rs_debug_info
					std::cout << "Index " << i << " in Grid eingefügt an Position " << atom_[i].p;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
		}
				#ifdef print_rs_debug_info
				HALT = 0;
				#endif
				#ifdef debug_rs
				for (Position i = 0; i < atom_.size(); i++)
				{
					print << pre << "Atom " << atom_[i] << " eingefügt (" << i << ")\n";
				}
				pre.replace(0,2,"");
				print << pre << "end\n";
				#endif
				#ifdef print_rs_debug_info
				HashGrid3<Position>::BoxIterator b;
				HashGridBox3<Position>::DataIterator d;
				for (b = grid_.beginBox(); b != grid_.endBox(); b++)
				{
					Position x, y, z;
					grid_.getIndices(*b,x,y,z);
					std::cout << "Box(" << x << "," << y << "," << z << ") enthält:\n  ";
					for (d = b->beginData(); d != b->endData(); d++)
					{
						std::cout << *d << "  ";
					}
					std::cout << "\n";
				}
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
	}


	template <typename T>
	TReducedSurface<T>::~TReducedSurface()
		throw()
	{
				#ifdef debug_rs
				print << *this;
				#endif
		clear();
				#ifdef debug_rs
				print << pre << "~ReducedSurface()\n";
				print << pre << "end\n";
				print << "ENDE\n";
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::set(const TReducedSurface<T>& reduced_surface)
		throw(Exception::GeneralException)
	{
		*this = reduced_surface;
	}


	template <typename T>
	void TReducedSurface<T>::operator=(const TReducedSurface<T>& reduced_surface)
		throw(Exception::GeneralException)
	{
		atom_ = reduced_surface.atom_;
		probe_radius_ = reduced_surface.probe_radius_;
		grid_ = reduced_surface.grid_;
		neighbours_ = reduced_surface.neighbours_;
		r_max_ = reduced_surface.r_max_;
		number_of_vertices_ = reduced_surface.number_of_vertices_;
		number_of_edges_ = reduced_surface.number_of_edges_;
		number_of_faces_ = reduced_surface.number_of_faces_;
		TRSVertex<T>* vertex;
		TRSEdge<T>* edge;
		TRSFace<T>* face;
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			if (reduced_surface.vertices_[i] != NULL)
			{
				if (reduced_surface.vertices_[i]->index_ < 0)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY REDUCED SURFACE",
																						"NEGATIVE INDEX FOUND");
				}
				else
				{
					vertex = new TRSVertex<T>(*reduced_surface.vertices_[i],false);
					vertices_.push_back(vertex);
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY REDUCED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		for (Position i = 0; i < number_of_edges_; i++)
		{
			if (reduced_surface.edges_[i] != NULL)
			{
				if (reduced_surface.edges_[i]->index_ < 0)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY REDUCED SURFACE",
																						"NEGATIVE INDEX FOUND");
				}
				else
				{
					edge = new TRSEdge<T>(*reduced_surface.edges_[i],false);
					edges_.push_back(edge);
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY REDUCED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		for (Position i = 0; i < number_of_faces_; i++)
		{
			if (reduced_surface.faces_[i] != NULL)
			{
				if (reduced_surface.faces_[i]->index_ < 0)
				{
					throw Exception::GeneralException(__FILE__,__LINE__,
																						"CAN NOT COPY REDUCED SURFACE",
																						"NEGATIVE INDEX FOUND");
				}
				else
				{
					face = new TRSFace<T>(*reduced_surface.faces_[i],false);
					faces_.push_back(face);
				}
			}
			else
			{
				throw Exception::GeneralException(__FILE__,__LINE__,
																					"CAN NOT COPY REDUCED SURFACE",
																					"NULL-POINTER FOUND");
			}
		}
		HashSet<TRSEdge<T>*>::ConstIterator e;
		HashSet<TRSFace<T>*>::ConstIterator f;
		for (Position i = 0; i < number_of_vertices_; i++)
		{
			for (e = reduced_surface.vertices_[i]->edges_.begin();
					 e != reduced_surface.vertices_[i]->edges_.end();  e++)
			{
				vertices_[i]->edges_.insert(edges_[(*e)->index_]);
			}
			for (f = reduced_surface.vertices_[i]->faces_.begin();
					 f != reduced_surface.vertices_[i]->faces_.end();  f++)
			{
				vertices_[i]->faces_.insert(faces_[(*f)->index_]);
			}
		}
		for (Position i = 0; i < number_of_edges_; i++)
		{
			edges_[i]->vertex0_ = vertices_[reduced_surface.edges_[i]->vertex0_->index_];
			edges_[i]->vertex1_ = vertices_[reduced_surface.edges_[i]->vertex1_->index_];
			edges_[i]->face0_ = faces_[reduced_surface.edges_[i]->face0_->index_];
			edges_[i]->face1_ = faces_[reduced_surface.edges_[i]->face1_->index_];
		}
		for (Position i = 0; i < number_of_faces_; i++)
		{
			faces_[i]->vertex0_ = vertices_[reduced_surface.faces_[i]->vertex0_->index_];
			faces_[i]->vertex1_ = vertices_[reduced_surface.faces_[i]->vertex1_->index_];
			faces_[i]->vertex2_ = vertices_[reduced_surface.faces_[i]->vertex2_->index_];
			faces_[i]->edge0_ = edges_[reduced_surface.faces_[i]->edge0_->index_];
			faces_[i]->edge1_ = edges_[reduced_surface.faces_[i]->edge1_->index_];
			faces_[i]->edge2_ = edges_[reduced_surface.faces_[i]->edge2_->index_];
		}
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
		if (number_of_faces_ > 0)
		{
			while (faces_[number_of_faces_-1] == NULL)
			{
				faces_.pop_back();
				number_of_faces_--;
			}
			for (Position i = 0; i < number_of_faces_; i++)
			{
				if (faces_[i] == NULL)
				{
					faces_[i] = faces_[number_of_faces_-1];
					faces_[i]->index_ = i;
					faces_.pop_back();
					number_of_faces_--;
					while (faces_[number_of_faces_-1] == NULL)
					{
						faces_.pop_back();
						number_of_faces_--;
					}
				}
			}
		}
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
		throw(Exception::IndexOverflow)
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
		throw(Exception::IndexOverflow)
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
		throw(Exception::IndexOverflow)
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
		throw(Exception::IndexOverflow)
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
			rsedge1[0] = face1->edge0_;
			rsedge1[1] = face1->edge1_;
			rsedge1[2] = face1->edge2_;
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
			rsvertex1[0] = face1->vertex0_;
			rsvertex1[1] = face1->vertex1_;
			rsvertex1[2] = face1->vertex2_;
			for (Position j = 0; j < 3; j++)
			{
				for (Position i = 0; i < 3; i++)
				{
					if (face2->getVertex(i)->atom_ == rsvertex1[j]->atom_)
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
					vertices_[rsvertex2[i]->index_] = NULL;
					delete rsvertex2[i];
				}
				rsvertex1[i]->faces_.erase(face1);
				rsvertex1[i]->faces_.erase(face2);
			}
			// correct the edges
			for (Position i = 0; i < 3; i++)
			{
				if (rsedge1[i] == rsedge2[i])
				{
					rsedge1[i]->vertex0_->edges_.erase(rsedge1[i]);
					rsedge1[i]->vertex1_->edges_.erase(rsedge1[i]);
					edges_[rsedge1[i]->index_] = NULL;
					delete rsedge1[i];
				}
				else
				{
					// ACHTUNG: evtl. hier exception
					TRSFace<T>* neighbour2 = rsedge2[i]->other(face2);
					if (rsedge1[i]->face0_ == face1)
					{
						rsedge1[i]->face0_ = neighbour2;
					}
					else
					{
						rsedge1[i]->face1_ = neighbour2;
					}
					for (Position j = 0; j < 3; j++)
					{
						if (neighbour2->getEdge(j) == rsedge2[i])
						{
							neighbour2->setEdge(j,rsedge1[i]);
						}
					}
					rsedge2[i]->vertex0_->edges_.erase(rsedge2[i]);
					rsedge2[i]->vertex1_->edges_.erase(rsedge2[i]);
					edges_[rsedge2[i]->index_] = NULL;
					delete rsedge2[i];
					// ACHTUNG
					// Winkel von rsedge1[i] muss neu berechnet werden
				}
			}
			faces_[face1->index_] = NULL;
			faces_[face2->index_] = NULL;
			delete face1;
			delete face2;
		}
	}


	template <typename T>
	void TReducedSurface<T>::compute()
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
				#ifdef debug_rs
				print << pre << "computing ...\n";
				pre += "  ";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "computing ...\n";
				HALT = 0;
				#endif
		// indices of the atoms that do not belong to reduced surface
		HashSet<Index> indices;
		// all atom indices
		HashSet<Index> all_indices;
		for (Position i = 0; i < atom_.size(); i++)
		{
			indices.insert(i);
			all_indices.insert(i);
		}
		// all new created vertices which are not yet checked for extensions
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
			switch (start)
			{
				case 2 :	extendComponent(indices,new_vertices,vertices);
									break;
				case 3 :
									#ifdef print_rs_debug_info
									std::cout << "Start-Face gefunden: " << *start_face << "\n";
									std::cout << *(start_face->vertex0_) << "\n";
									std::cout << *(start_face->vertex1_) << "\n";
									std::cout << *(start_face->vertex2_) << "\n";
									if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
									#endif
									getRSComponent(start_face,indices,new_vertices,vertices);
									break;
				default :	break;
			}
		//}
		clean();
				#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end        // computing\n";
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::getRSComponent
		 (TRSFace<T>*																	 /*face*/,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
				#ifdef debug_rs
				print << pre << "getRSComponent(face, " << indices.size() << ", "
							<< new_vertices.size() << ", vertices)\n";
				pre += "  ";
				#endif
		for (Position i = 0; i < number_of_faces_; i++)
		{
			if (faces_[i] != NULL)
			{
				treatFace(faces_[i],indices,new_vertices,vertices);
			}
		}
		extendComponent(indices,new_vertices,vertices);
				#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end        // getRSComponent(face, " << indices.size() << ", "
							<< new_vertices.size() << ", vertices)\n";
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::treatFace
		 (TRSFace<T>*																	 face,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw(Exception::GeneralException,Exception::DivisionByZero)
	{
				#ifdef debug_rs
				print << pre << "treatFace(" << *face << "," << indices.size() << ","
							<< new_vertices.size() << ")\n";
				pre += "  ";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "behandle " << *face;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		TRSEdge<T>* edge;
		for (Position i = 0; i < 3; i++)
		{
			edge = face->getEdge(i);
			if (edge->face1_ == NULL)
			{
				treatEdge(edge,indices,new_vertices,vertices);
			}
		}
				#ifdef print_rs_debug_info
				std::cout << "end        // behandle " << *face;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
				#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end        // treatFace(" << *face << "," << indices.size() << ","
							<< new_vertices.size() << ")\n";
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::treatEdge
		 (TRSEdge<T>*																	 edge,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
		// This function rolls the probe sphere over a RSEdge.
		// From all atoms that can be touced by the probe sphere when it touches	
		// the two atoms of the edge is this one selected for witch the rotation	
		// angle is the smallet. A new face is found.
		// If this face already exists the edge exists twice, too. These two
		// edges and their vertices are joined.
		// If the face does not exist yet it will be created. A new vertex and	
		// two new edges will be created, too.
		// In both cases the treated edge will be updated. It has not to be	
		// considerd again.

				#ifdef debug_rs
				print << pre << "treat " << *edge << "\n";
				pre += "  ";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "behandle " << *edge;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		// find third atom
		TAngle<T> phi;
		TSphere3<T> probe;
		TRSFace<T>* start_face(edge->face0_);			// the edge already knows the
				#ifdef print_rs_debug_info
				std::cout << "starting face: " << *start_face;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
				#ifdef debug_rs
				print << pre << "starting face: " << *start_face << "\n";
				#endif
		TRSVertex<T>* vertex1(edge->vertex0_);		// starting face and their
		TRSVertex<T>* vertex2(edge->vertex1_);		// two vertices
		TRSVertex<T>* vertex3(NULL);
		Index atom1(vertex1->atom_);
		Index atom2(vertex2->atom_);
				#ifdef print_rs_debug_info
				std::cout << "Vertex1: " << *vertex1 << " auf " << atom1 << "\n";
				std::cout << "Vertex2: " << *vertex2 << " auf " << atom2;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		Index atom3;
		try
		{
			atom3 = thirdAtom(vertex1,vertex2,start_face,probe,phi,
												indices,new_vertices,vertices);
		}
		catch (Exception::GeneralException e)
		{
			String message = e.getMessage();
			String test_message = "PROBE SPHERE TOUCHES FOUR ATOMS";
			if (message == test_message)
			{
				treatEdge(edge,indices,new_vertices,vertices);
				return;
			}
			else
			{
				throw;
			}
		}				
				#ifdef print_rs_debug_info
				std::cout << "Also:\n";
				std::cout << "Vertex1: " << *vertex1 << " auf " << atom1 << "\n";
				std::cout << "Vertex2: " << *vertex2 << " auf " << atom2 << "\n";
				std::cout << "Vertex3 auf " << atom3;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		if (atom3 == -1)															// no third atom found
		{
			throw Exception::GeneralException(__FILE__,__LINE__,
																				"CanNotTreatEdge",
																				"no third atom found");
		}
				#ifdef debug_rs
				print << pre << "new face: " << atom1 << "  " << atom2 << "  " << atom3 << "\n";
				#endif
		TSphere3<T> sphere1(atom_[atom1]);
		TSphere3<T> sphere2(atom_[atom2]);
		TSphere3<T> sphere3(atom_[atom3]);
				#ifdef print_rs_debug_info
				std::cout << "Atom1: " << sphere1 << "\n";
				std::cout << "Atom2: " << sphere2 << "\n";
				std::cout << "Atom3: " << sphere3;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		// build a new face and two new edges
		vertex3 = new TRSVertex<T>(atom3);
		TRSEdge<T>* edge1;
		TRSEdge<T>* edge2;
		TRSFace<T>* new_face                          // vorlaeufige neue face
				= new TRSFace<T>(vertex1,vertex2,vertex3,NULL,NULL,NULL,
												 probe.p,getFaceNormal(sphere1,sphere2,sphere3,probe),
												 false,-1);
				#ifdef debug_rs
				print << pre << "face " << *new_face << " gebaut\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "neuer Vertex: " << *vertex3 << "\n";
				std::cout << "neue Face: " << *new_face << "\n";
				std::cout << "Teste, ob face schon existiert";
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		TRSFace<T>* test = faceExists(new_face,vertices[vertex1->atom_]);
				#ifdef print_rs_debug_info
				std::cout << "Test erfolgreich";
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		if (test == NULL)
		{
					#ifdef print_rs_debug_info
					std::cout << "Face existiert noch nicht";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			// built face doesn't exist yet
			// The new vertex has to be created since we don't know at this time
			// weater it is a new vertex or not.
			// Attention: one atom can build more than one vertex
			vertex3->index_ = number_of_vertices_;
			vertices_.push_back(vertex3);
			number_of_vertices_++;
			indices.erase(atom3);
			new_vertices.insert(vertex3);
					#ifdef print_rs_debug_info
					std::cout << "Vertex3 in vertices_ gepusht";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			vertices[atom3].push_back(vertex3);
					#ifdef print_rs_debug_info
					std::cout << "Vertex3 in vertices[" << atom3 << "] gepusht";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			edge1 = new TRSEdge<T>;
			edge1->vertex0_ = vertex2;
			edge1->vertex1_ = vertex3;
			edge1->face0_ = new_face;
			edge2 = new TRSEdge<T>;
			edge2->vertex0_ = vertex3;
			edge2->vertex1_ = vertex1;
			edge2->face0_ = new_face;
					#ifdef print_rs_debug_info
					std::cout << "zwei neue Edges erzeugt";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			new_face->edge0_ = edge;
			new_face->edge1_ = edge1;
			new_face->edge2_ = edge2;
			TPlane3<T> plane(sphere1.p,sphere2.p,sphere3.p);
			new_face->singular_ = Maths::isLess(GetDistance(probe.p,plane),probe_radius_);
			new_face->index_ = number_of_faces_;
			faces_.push_back(new_face);
			number_of_faces_++;
					#ifdef print_rs_debug_info
					if (number_of_faces_ == 2365) HALT = 0;
					std::cout << "Face und Edges geupdatet:\n";
					std::cout << *new_face << "\n";
					std::cout << *edge << "\n";
					std::cout << *edge1 << "\n";
					std::cout << *edge2;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			vertex1->faces_.insert(new_face);
			vertex2->faces_.insert(new_face);
			vertex3->faces_.insert(new_face);
			vertex1->edges_.insert(edge2);
			vertex2->edges_.insert(edge1);
			vertex3->edges_.insert(edge1);
			vertex3->edges_.insert(edge2);
					#ifdef print_rs_debug_info
					std::cout << "Vertices geupdatet:\n";
					std::cout << *vertex1 << "\n";
					std::cout << *vertex2 << "\n";
					std::cout << *vertex3;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
					#ifdef debug_rs
					print << pre << "... geupdatet " << *new_face << "\n";
					print << pre << "... in vertices_[" << vertex1->index_ << "], vertices_["
								<< vertex2->index_ << "], " << "vertices_[" << vertex3->index_ << "] gepusht\n";
					print << pre << "neue edges:\n" << pre << *edge1 << "\n" << pre << *edge2 << "\n";
					#endif
		}
		else
		{
					#ifdef debug_rs
					print << pre << "... existiert schon: " << *test << "\n";
					#endif
					#ifdef print_rs_debug_info
					std::cout << "Face existiert schon: " << *test;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			// built face exitsts already
			// the corresponding edge in the existing face has to be found
			Index i = 0;
			TRSEdge<T>* test_edge(test->edge0_);
			while (test_edge->similar(*edge) == false)
			{
				i++;
				if (i > 2)
				{
					throw Exception::GeneralException								// There are only 3
							(__FILE__,__LINE__,"CanNotTreatEdge",				// edges. This should
							 "corresponding edge in face not found");		// never happens.
				}
				test_edge = test->getEdge(i);
			}
					#ifdef print_rs_debug_info
					std::cout << "Korrespondierende Edge gefunden: " << *test_edge;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			// Now the corresponding vertices of the corresponding edges
			// have to be joined and one of them has to be deleted (if they are not
			// equal). This is neccessary since creating a new face always creates
			// a new vertex.
			TRSVertex<T>* test_vertex1 = test_edge->vertex0_;
			TRSVertex<T>* test_vertex2 = test_edge->vertex1_;
					#ifdef print_rs_debug_info
					std::cout << "Vertices der korrespondierenden Edge:\n";
					std::cout << "  " << *test_vertex1 << "\n";
					std::cout << "  " << *test_vertex2;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			if (test_vertex1->atom_ == vertex1->atom_)
			{
						#ifdef print_rs_debug_info
						std::cout << "Korrespondierende Vertices:\n";
						std::cout << *vertex1 << " - " << *test_vertex1 << "\n";
						std::cout << *vertex2 << " - " << *test_vertex2;
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				// now we know which vertices are corresponding
				if (*vertex1 != *test_vertex1)
				{
							#ifdef print_rs_debug_info
							std::cout << "erstes Paar verschieden, vereinige und ersetze";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					// the vertices only have to be joined if they are not equal
					vertex1->join(*test_vertex1);
					test_vertex1->substitute(vertex1);
					vertices_[test_vertex1->index_] = NULL;
					new_vertices.erase(test_vertex1);
					vertices[test_vertex1->atom_].remove(test_vertex1);
							#ifdef print_rs_debug_info
							std::cout << *vertex1;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							std::cout << "lösche " << *test_vertex1;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					delete test_vertex1;
				}
				if (*vertex2 != *test_vertex2)
				{
							#ifdef print_rs_debug_info
							std::cout << "zweites Paar verschieden, vereinige und ersetze";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					// the vertices only have to be joined if they are not equal
					vertex2->join(*test_vertex2);
					test_vertex2->substitute(vertex2);
					vertices_[test_vertex2->index_] = NULL;
					new_vertices.erase(test_vertex2);
					vertices[test_vertex2->atom_].remove(test_vertex2);
							#ifdef print_rs_debug_info
							std::cout << *vertex2;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							std::cout << "lösche " << *test_vertex2;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					delete test_vertex2;
				}
						#ifdef print_rs_debug_info
						std::cout << "lösche korrespondierende Edge aus Vertex1 und Vertex2";
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				vertex1->edges_.erase(test_edge);
				vertex2->edges_.erase(test_edge);
			}
			else
			{
						#ifdef print_rs_debug_info
						std::cout << "Korrespondierende Vertices:\n";
						std::cout << *vertex1 << " - " << *test_vertex2 << "\n";
						std::cout << *vertex2 << " - " << *test_vertex1;
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				// now we know which vertices are corresponding
				if (*vertex1 != *test_vertex2)
				{
							#ifdef print_rs_debug_info
							std::cout << "erstes Paar verschieden, vereinige und ersetze";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					// the vertices only have to be joined if they are not equal
					vertex1->join(*test_vertex2);
					test_vertex2->substitute(vertex1);
					vertices_[test_vertex2->index_] = NULL;
					new_vertices.erase(test_vertex2);
					vertices[test_vertex2->atom_].remove(test_vertex2);
							#ifdef print_rs_debug_info
							std::cout << *vertex1;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							std::cout << "lösche " << *test_vertex2;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					delete test_vertex2;
				}
				if (*vertex2 != *test_vertex1)
				{
							#ifdef print_rs_debug_info
							std::cout << "zweites Paar verschieden, vereinige und ersetze";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					// the vertices only have to be joined if they are not equal
					vertex2->join(*test_vertex1);
					test_vertex1->substitute(vertex2);
					vertices_[test_vertex1->index_] = NULL;
					new_vertices.erase(test_vertex1);
					vertices[test_vertex1->atom_].remove(test_vertex1);
							#ifdef print_rs_debug_info
							std::cout << *vertex2;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							std::cout << "lösche " << *test_vertex1;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					delete test_vertex1;
				}
						#ifdef print_rs_debug_info
						std::cout << "lösche korrespondierende Edge aus Vertex1 und Vertex2";
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				// The vertices should have only one of the two corresponding edges.
				// The other will be deleted later.
				vertex1->edges_.erase(test_edge);
				vertex2->edges_.erase(test_edge);
			}
					#ifdef print_rs_debug_info
					std::cout << "ersetze korrespondierende Edge in existierender Face";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			// The face should have only one of the two corresponding edges, too.
			test->setEdge(i,edge);
					#ifdef debug_rs
					print << pre << i << ". edge ersetzt: " << *test << "\n";
					#endif
					#ifdef print_rs_debug_info
					std::cout << *test; if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			// Now can we delete the build face and vertex and the double edge.
			delete new_face;
			if (test_edge->index_ != -1)
			{
				edges_[test_edge->index_] = NULL;
			}
			delete test_edge;
			delete vertex3;
					#ifdef print_rs_debug_info
					std::cout << "erzeugte face gelöscht";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					std::cout << "korrespondierende Edge gelöscht";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					std::cout << "erzeugter vertex gelöscht";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
					#ifdef debug_rs
					print << pre << "konstruierte face gelöscht\n";
					#endif
			new_face = test;
		}			// face exitsts test
		// update edge
				#ifdef debug_rs
				print << pre << "updating " << *edge << ":\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "update " << *edge;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		TCircle3<T> circle1;
		TCircle3<T> circle2;
		TCircle3<T> circle3;
		getCircles(atom1,atom2,circle1,circle2,circle3);
		TVector3<T> ip1;		// intersection points between
		TVector3<T> ip2;		// the edge and the probe sphere
		TLine3<T> line(sphere1.p,sphere2.p,TLine3<T>::FORM__TWO_POINTS);
		bool singular(GetIntersection(probe,line,ip1,ip2));
		if (singular &&
				Maths::isLess(ip1.getSquareDistance(sphere2.p),
											ip2.getSquareDistance(sphere2.p)))
		{										// ip1 is the intersection point next to the first
			ip1.swap(ip2);		// vertex of the edge
		}
		edge->face1_ = new_face;
		edge->center_of_torus_ = circle1.p;
		edge->radius_of_torus_ = circle1.radius;
		edge->phi_ = phi;
		edge->circle0_ = circle2;
		edge->circle1_ = circle3;
		edge->intersection_point0_ = ip1;
		edge->intersection_point1_ = ip2;
		edge->singular_ = singular;
				#ifdef print_rs_debug_info
				std::cout << "erfolgreich";
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
				#ifdef debug_rs
				print << pre << "... " << *edge << "\n";
				#endif
		if (edge->index_ == -1)
		{
			edge->index_ = number_of_edges_;
			edges_.push_back(edge);
			number_of_edges_++;
		}
				#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end        // treat " << *edge << "\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "Edge in edges_ gepusht";
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				std::cout << "Ergebnis:\n"
						 << *start_face << "\n  "
						 << *start_face->edge0_ << "\n  "
						 << *start_face->edge1_ << "\n  "
						 << *start_face->edge2_ << "\n"
						 << *new_face << "\n  "
						 << *new_face->edge0_ << "\n  "
						 << *new_face->edge1_ << "\n  "
						 << *new_face->edge2_ << "\n"
						 << *vertex1 << "\n" << *vertex2 << "\n"
						 << *start_face->thirdVertex(vertex1,vertex2) << "\n"
						 << *new_face->thirdVertex(vertex1,vertex2) << "\n";
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		//treatFace(edge->face2_,indices,new_vertices,vertices);
	}


	template <typename T>
	void TReducedSurface<T>::correct(Index atom,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw()
	{	
					#ifdef print_rs_debug_info
					std::ofstream rsfile("ReducedSurface.correct.pre.log");
					rsfile << *this;
					rsfile.close();
					HALT = 0;
					std::cout << "bearbeite Atom " << atom;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
		::std::list<TRSVertex<T>*>::iterator v;
		TRSVertex<T>* vertex;
		HashSet<TRSFace<T>*> faces;
		HashSet<TRSFace<T>*> treat_faces;
		HashSet<TRSFace<T>*>::Iterator f;
		//HashSet<TRSEdge<T>*> edges;
		//HashSet<TRSEdge<T>*> delete_edges;
		HashSet<TRSVertex<T>*> test_vertices;
		TRSEdge<T>* edge;
		v = vertices[atom].begin();
		while (v != vertices[atom].end())
		{
			treat_faces.clear();
			test_vertices.clear();
			vertex = *v;
			v++;
					#ifdef print_rs_debug_info
					std::cout << "bearbeite " << *vertex;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			faces = vertex->faces_;
			for (f = faces.begin(); f != faces.end(); f++)
			{
				(*f)->vertex0_->faces_.erase(*f);
				(*f)->vertex1_->faces_.erase(*f);
				(*f)->vertex2_->faces_.erase(*f);
						#ifdef print_rs_debug_info
						std::cout << "  lösche " << **f << "aus\n"
											<< "    " << *(*f)->vertex0_ << ",\n"
											<< "    " << *(*f)->vertex1_ << ",\n"
											<< "    " << *(*f)->vertex2_;
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				for (Position i = 0; i < 3; i++)
				{
					edge = (*f)->getEdge(i);
					if (edge != NULL)
					{
								#ifdef print_rs_debug_info
								std::cout << "  " << i << ". edge: " << *edge << "\n";
								#endif
						if (edge->face1_ == NULL)
						{
							edge->vertex0_->edges_.erase(edge);
							edge->vertex1_->edges_.erase(edge);
									#ifdef print_rs_debug_info
									std::cout << "    hat nur eine face, lösche aus\n      "
														<< *edge->vertex0_ << ",\n      "
														<< *edge->vertex1_ << "\n    füge diese in test_vertices ein\n";
									#endif
							test_vertices.insert(edge->vertex0_);
							test_vertices.insert(edge->vertex1_);
							Index index = edge->index_;
							if (index != -1)
							{
								edges_[index] = NULL;
										#ifdef print_rs_debug_info
										std::cout << "    edges_[" << index << "] = NULL\n";
										#endif
							}
							(*f)->setEdge(i,NULL);
									#ifdef print_rs_debug_info
									std::cout << "    gelöscht aus " << **f << "\n    und zerstört";
									if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
									#endif
							delete edge;
						}
						else
						{
							treat_faces.insert(edge->deleteFace(*f));
									#ifdef print_rs_debug_info
									std::cout << "    face gelöscht aus edge, neighbour face in treat_faces gepusht";
									if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
									#endif
						}
					}
					else
					{
								#ifdef print_rs_debug_info
								std::cout << "  " << i << ". edge: ---";
								if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
								#endif
					}
				}
			}
			for (f = faces.begin(); f != faces.end(); f++)
			{
				treat_faces.erase(*f);
				faces_[(*f)->index_] = NULL;
						#ifdef print_rs_debug_info
						std::cout << "  " << **f << "  aus treat_faces gelöscht,\n"
											<< "    faces_[" << (*f)->index_ << "] = NULL,\n"
											<< "    zerstört";
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				delete *f;
			}
			for (f = treat_faces.begin(); f != treat_faces.end(); f++)
			{
						#ifdef print_rs_debug_info
						std::cout << "  bringe " << **f << " ans Ende:\n"
											<< "    faces_[" << (*f)->index_ << "] = NULL\n";
						#endif
				faces_[(*f)->index_] = NULL;
				faces_.push_back(*f);
				faces_[number_of_faces_]->index_ = number_of_faces_;
						#ifdef print_rs_debug_info
						std::cout << "    faces_[" << number_of_faces_ << "] = " << *faces_[number_of_faces_];
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				number_of_faces_++;
			}
			test_vertices.erase(vertex);
			HashSet<TRSVertex<T>*>::Iterator test;
			for (test = test_vertices.begin(); test != test_vertices.end(); test++)
			{
						#ifdef print_rs_debug_info
						std::cout << "  betrachte " << **test << "(" << *test << ")\n";
						#endif
				if ((*test)->hasEdges() == false)
				{
					vertices_[(*test)->index_] = NULL;
					vertices[(*test)->atom_].remove(*test);
							#ifdef print_rs_debug_info
							std::cout << "    vertices_[" << (*test)->index_ << "] = NULL\n"
												<< "    aus vertices[" << (*test)->atom_ << "] entfernt\n    zerstört";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					new_vertices.erase(*test);
					delete *test;
				}
				else
				{
							#ifdef  print_rs_debug_info
							std::cout << "    bleibt bestehen";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
				}
			}
			vertices_[vertex->index_] = NULL;
			vertices[atom].remove(vertex);
					#ifdef print_rs_debug_info
					std::cout << "  vertices_[" << vertex->index_ << "] = NULL\n"
										<< "  aus vertices[" << atom << "] entfernt\n  zerstört";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			new_vertices.erase(vertex);
			delete vertex;
		}
		indices.insert(atom);
		//atom_[atom].radius -= 10*Constants::EPSILON;
				#ifdef print_rs_debug_info
				HALT = 0;
				std::cout << "Atom " << atom << " bearbeitet\n";
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				rsfile.open("ReducedSurface.correct.post.log");
				rsfile << *this;
				rsfile.close();
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::extendComponent
		 (HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw(Exception::GeneralException,Exception::DivisionByZero)
	{
				#ifdef debug_rs
				print << pre << "extendComponent(" << indices.size() << ","
							<< new_vertices.size() << ")\n";
				pre += "  ";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "extendComponent";
				HALT = 0;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
		while (new_vertices.size() > 0)
		{
//std::cout << new_vertices.size() << "\n";
					#ifdef print_rs_debug_info
					std::cout << new_vertices.size() << " vertices in new_vertices";
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
			TRSFace<T>* face = NULL;
			TRSVertex<T>* vertex1 = *new_vertices.begin();
			new_vertices.erase(new_vertices.begin());
  		Index atom1(vertex1->atom_);
					#ifdef print_rs_debug_info
					std::cout << "suche Erweiterung an Vertex " << *vertex1 << " auf Atom " << atom1;
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
 			::std::list<Index> atom_list1;
 			::std::list<Index> atom_list2;
			if (neighbours_[atom1].size() == 0)
			{
				neighboursOfOneAtom(atom1);
			}
			atom_list1 = neighbours_[atom1];
			::std::list<Index>::iterator i;
					#ifdef debug_rs
					print << pre << "Atom1: " << atom1 << ", Nachbarn: "
								<< atom_list1.size() << "\n";
					#endif
					#ifdef print_rs_debug_info
					std::cout << atom_list1.size() << " Nachbarn gefunden\n";
					for (i = atom_list1.begin(); i != atom_list1.end(); i++)
					{
						std::cout << *i << "  ";
					}
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
					//std::cout << atom_list1.size() << " Nachbarn (1) gefunden\n";
			i = atom_list1.begin();
			::std::list<Index>::iterator i_;
			while (i != atom_list1.end())
			{
				i_ = i;
				i_++;
				if (indices.has(*i) == false)
				{
					atom_list1.erase(i);
				}
				i = i_;
			}
			i = atom_list1.begin();
			while (i != atom_list1.end())
			{
				Index atom2 = *i;
						#ifdef print_rs_debug_info
						std::cout << "suche drittes Atom zu " << atom1 << " und " << atom2 << "\n";
						#endif
				atom_list2.clear();
				neighboursOfTwoAtoms(atom1,atom2,atom_list2);
						//std::cout << atom_list2.size() << " Nachbarn (2) gefunden\n";
						#ifdef print_rs_debug_info
						std::cout << atom_list2.size() << " Kandidaten: ";
						::std::list<Index>::iterator al2;
						for (al2 = atom_list2.begin(); al2 != atom_list2.end(); al2++)
						{
							std::cout << *al2 << "  ";
						}
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
						//std::cout << atom_list2.size() << " Kandidaten:\n";
				std::list< std::pair< Index,TSphere3<T> > > candidates;
				findThirdAtom(atom1,atom2,atom_list2,candidates);
						//std::cout << candidates.size() << " Nachbarn (3) gefunden\n";
						#ifdef debug_rs
						print << pre << "Atom2: " << atom2 << ", Nachbarn: "
									<< candidates.size() << "\n";
						#endif
						#ifdef print_rs_debug_info
						std::cout << candidates.size() << " mögliche probe-Positionen: ";
						std::list< std::pair< Index,TSphere3<T> > >::iterator cit;
						for (cit = candidates.begin(); cit != candidates.end(); cit++)
						{
							std::cout << cit->first << "  ";
						}
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				if (candidates.size() == 0)
				{
					TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
					TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,atom_list2);
					if (edge != NULL)
					{
						edge->index_ = number_of_edges_;
						edges_.push_back(edge);
						number_of_edges_++;
						vertex1->edges_.insert(edge);
						vertex2->edges_.insert(edge);
						vertex2->index_ = number_of_vertices_;
						vertices_.push_back(vertex2);
						number_of_vertices_++;
						indices.erase(atom2);
						vertices[atom2].push_back(vertex2);
						new_vertices.insert(vertex1);
						new_vertices.insert(vertex2);
								#ifdef debug_rs
								print << pre << "freie edge erzeugt: " << *edge << "\n";
								#endif
								#ifdef print_rs_debug_info
								std::cout << "freie edge erzeugt: " << *edge;
								HALT = 0;
								if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
								#endif
						i = atom_list1.end();
					}
					else
					{
								#ifdef debug_rs
								print << pre << "bilden keine freie edge\n";
								#endif
								#ifdef print_rs_debug_info
								std::cout << "bilden keine freie edge";
								if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
								#endif
						delete vertex2;
						i++;
					}
				}
				else
				{
					bool found = false;
					std::list< std::pair< Index,TSphere3<T> > >::iterator j;
					#ifdef print_rs_debug_info
					for (j = candidates.begin(); j != candidates.end(); j++)
					{
						std::cout << j->first << "  ";
					}
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
					j = candidates.begin();
					while (j != candidates.end())
					{
						Index atom3 = j->first;
						TSphere3<T> probe = j->second;
								#ifdef debug_rs
								print << pre << "Atom3: " << atom3
											<< ", probe: " << probe << " ...\n";
								#endif
								#ifdef print_rs_debug_info
								std::cout << "3. Atom: " << atom3 << ", probe: " << probe << "...\n";
								#endif
						if (checkProbe(probe,atom1,atom2,atom3) == true)
						{
									#ifdef debug_rs
									print << pre << "... passt\n";
									#endif
									#ifdef print_rs_debug_info
									std::cout << "... passt, bilde face\n";
									#endif
							face = new TRSFace<T>;
							TRSEdge<T>* edge1 = new TRSEdge<T>;
							TRSEdge<T>* edge2 = new TRSEdge<T>;
							TRSEdge<T>* edge3 = new TRSEdge<T>;
							TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
							TRSVertex<T>* vertex3 = new TRSVertex<T>(atom3);
							vertices[atom2].push_back(vertex2);
							vertices[atom3].push_back(vertex3);
							updateFaceAndEdges(vertex1,vertex2,vertex3,
																 edge1,edge2,edge3,
																 face,probe);
							face->index_ = number_of_faces_;
							faces_.push_back(face);
							number_of_faces_++;
							vertex2->index_ = number_of_vertices_;
							vertices_.push_back(vertex2);
							number_of_vertices_++;
							indices.erase(atom2);
							vertex3->index_ = number_of_vertices_;
							vertices_.push_back(vertex3);
							number_of_vertices_++;
							indices.erase(atom3);
							vertex1->faces_.insert(face);
							vertex2->faces_.insert(face);
							vertex3->faces_.insert(face);
							vertex1->edges_.insert(edge1);
							vertex2->edges_.insert(edge1);
							vertex2->edges_.insert(edge2);
							vertex3->edges_.insert(edge2);
							vertex3->edges_.insert(edge3);
							vertex1->edges_.insert(edge3);
							new_vertices.insert(vertex1);
							new_vertices.insert(vertex2);
							new_vertices.insert(vertex3);
							i = atom_list1.end();
							j = candidates.end();
							found = true;
							#ifdef print_rs_debug_info
							std::cout << *face << "\n  " << *vertex1 << "\n  " << *vertex2 << "\n  " << *vertex3
												<< "\n  " << *edge1 << "\n  " << *edge2 << "\n  " << *edge3;
							HALT = 0;
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
						}
						else
						{
									#ifdef debug_rs
									print << pre << "... passt nicht\n";
									#endif
									#ifdef print_rs_debug_info
									std::cout << "... passt nicht";
									if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
									#endif
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
					#ifdef debug_rs
					pre.replace(0,2,"");
					print << pre << "end\n";
					#endif
		}
	}


	template <typename T>
	Index TReducedSurface<T>::thirdAtom
		 (TRSVertex<T>* 															 vertex1,
			TRSVertex<T>*															 	 vertex2,
			TRSFace<T>*																	 face,
			TSphere3<T>&																 probe,
			TAngle<T>&																	 phi,
			HashSet<Index>&															 indices,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
		// This function chooses from all atoms which can be touced by the probe
		// sphere when it touches the given two vertices this one, for which is
		// the rotation angle the smalest.
		// If the rotation angle equals zero, the probe sphere can touch four
		// atoms an an exception is thrown.
		// If no atom can be found an exception is thrown.
				#ifdef debug_rs
				pre += "  ";
				#endif
		Index atom1(vertex1->atom_);
		Index atom2(vertex2->atom_);
		::std::list<Index> atom_list;
		neighboursOfTwoAtoms(atom1,atom2,atom_list);
		if (atom_list.size() == 0)
		{
					#ifdef debug_rs
					print << "\n\n" << *this << "\n\n";
					print << *vertex1 << "\n";
					print << *vertex2 << "\n";
					#endif
			throw Exception::GeneralException
							(__FILE__,__LINE__,
							 "CanNotFindThirdAtom","no atom close enougth");
		}
		list< pair< Index,TSphere3<T> > > candidates;
		findThirdAtom(atom1,atom2,atom_list,candidates);
		std::list< std::pair< Index,TSphere3<T> > >::iterator k;
		Index back(-1);
				#ifdef debug_rs
				print << pre << atom1 << "  " << atom2 << "\n";
				print << pre << *face << "\n";
				print << pre << "Kandidaten: (" << candidates.size() << ")\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << atom1 << "  " << atom2 << "  "
									<< face->thirdVertex(vertex1,vertex2)->atom_ << "\n";
				std::cout << *face << "\n";
				std::cout << "Kandidaten: (" << candidates.size() << ")\n";
				#endif
		TAngle<double> old_angle(3*Constants::PI,true);
		TAngle<double> new_angle;
		TAngle<double> pi(Constants::PI,true);
		TVector3<T> norm_ = atom_[atom1].p-atom_[atom2].p;
		TVector3<T> test_vector = face->normal_%norm_;
		Index third_face_atom = face->thirdVertex(vertex1,vertex2)->atom_;
		if (Maths::isLess
					(test_vector*atom_[third_face_atom].p,
					 test_vector*atom_[atom1].p)      )
		{
			norm_.negate();
				#ifdef debug_rs
				print << pre << "  Dreahachse: " << atom1 << " --> " << atom2 << "\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "  Dreahachse: " << atom1 << " --> " << atom2 << "\n";
				#endif
		}
		else
		{
				#ifdef debug_rs
				print << pre << "  Dreahachse: " << atom2 << " --> " << atom1 << "\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "  Dreahachse: " << atom2 << " --> " << atom1 << "\n";
				#endif
		}
		TVector3<double> norm((double)norm_.x,(double)norm_.y,(double)norm_.z);
		TSphere3<T> sphere1(atom_[atom1]);
		TSphere3<T> sphere2(atom_[atom2]);
		sphere1.radius += probe_radius_;
		sphere2.radius += probe_radius_;
		TCircle3<T> circle;
		GetIntersection(sphere1,sphere2,circle);
		TVector3<T> start_probe = face->center_;
		TVector3<T> v1_ = start_probe-circle.p;
		TVector3<double> v1((double)v1_.x,(double)v1_.y,(double)v1_.z);
		TVector3<T> face_normal = face->normal_;
		T test_value = face_normal*sphere1.p;
		std::list< pair< Index,TSphere3<T> > > third;
		for (k = candidates.begin(); k != candidates.end(); k++)
		{
			if ((k->first != third_face_atom) ||
					Maths::isLess(face_normal*k->second.p,test_value) )
					//(k->second.p != start_probe))
					// not found the starting face
			{
				TVector3<T> v2_ = k->second.p-circle.p;
				TVector3<double> v2((double)v2_.x,(double)v2_.y,(double)v2_.z);
						// ACHTUNG !!!
				new_angle = getOrientedAngle(v1,v2,norm)+pi;
				//new_angle = getOrientedAngle(v1,v2,norm);
				//if (Maths::isLess(new_angle.toRadian(),(T)0))
				//{
				//	new_angle += pi;
				//	new_angle += pi;
				//}
						#ifdef debug_rs
						print << pre << "{" << k->first << "," << k->second << "} ... getOrientedAngle("
									<< v1 << "," << v2 << "," << norm << ")+pi ... " << new_angle << "\n";
						#endif
						#ifdef print_rs_debug_info
						//std::cout << "{" << k->first << "," << k->second << "} ... getOrientedAngle("
						//					<< v1 << "," << v2 << "," << norm << ")+pi ... " << new_angle << "\n";
						std::cout << "{" << k->first << "," << k->second << "} ... " << new_angle << "\n";
						#endif
				if ((new_angle == TAngle<double>(0,true)) || (new_angle == 2*pi))
				{
							#ifdef debug_surface_processor_verbose
							std::cerr << "    ProbeSphere berührt vier Atome, korrigiere ...\n";
							#endif
					correct(k->first,indices,new_vertices,vertices);
							#ifdef print_rs_debug_info
							std::cout << "verkleinere Atom " << k->first << "\n";
							#endif
					atom_[k->first].radius -= 10*Constants::EPSILON;
							#ifdef debug_surface_processor_verbose
							std::cerr << "    ... ok\n";
							#endif
							#ifdef debug_rs
							pre.replace(0,2,"");
							print << pre << "end ... exception\n";
							#endif
					throw Exception::GeneralException
							(__FILE__,__LINE__,"CAN'T COMPUTE RS","PROBE SPHERE TOUCHES FOUR ATOMS");
				}
				if (new_angle < old_angle)
				{
					old_angle = new_angle;
					third.clear();
					third.push_back(*k);
					//back = k->first;
					//probe = k->second;
					//phi.set((T)new_angle.value,true);
							#ifdef print_rs_debug_info
							std::cout << "... neues Minimum\n";
							#endif
				}
				else
				{
					if (new_angle == old_angle)
					{
						third.push_back(*k);
								#ifdef print_rs_debug_info
								std::cout << "... erneuetes Minimum\n";
								#endif
					}
				}
			}
		}
		if (third.size() > 1)
		{
					#ifdef debug_surface_processor_verbose
					std::cerr << "    ProbeSphere berührt " << third.size()+2 << " Atome, korrigiere ...\n";
					#endif
			k = third.begin();
			k++;
			while (k != third.end())
			{
				correct(k->first,indices,new_vertices,vertices);
						#ifdef print_rs_debug_info
						std::cout << "verkleinere Atom " << k->first << "\n";
						#endif
				atom_[k->first].radius -= 10*Constants::EPSILON;
				k++;
			}
					#ifdef debug_surface_processor_verbose
					std::cerr << "    ... ok\n";
					#endif
					#ifdef debug_rs
					pre.replace(0,2,"");
					print << pre << "end ... exception\n";
					#endif
			throw Exception::GeneralException
					(__FILE__,__LINE__,"CAN'T COMPUTE RS","PROBE SPHERE TOUCHES FOUR ATOMS");
		}
		back = third.front().first;
		probe = third.front().second;
		phi.set((T)old_angle.value,true);
				#ifdef debug_rs
				pre.replace(0,2,"");
				#endif
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
		throw(Exception::DivisionByZero)
	{
//std::cout << "#2\n";
		face = findFirstFace(indices);
//std::cout << "#3\n";
		if (face != NULL)
		{
			TRSVertex<T>* vertex1 = face->vertex0_;
			TRSVertex<T>* vertex2 = face->vertex1_;
			TRSVertex<T>* vertex3 = face->vertex2_;
			new_vertices.insert(vertex1);
			new_vertices.insert(vertex2);
			new_vertices.insert(vertex3);
			vertices[vertex1->atom_].push_back(vertex1);
			vertices[vertex2->atom_].push_back(vertex2);
			vertices[vertex3->atom_].push_back(vertex3);
					#ifdef debug_rs
					print << pre << "starting face: " << vertex1->atom_ << " "
								<< vertex2->atom_ << " "
								<< vertex3->atom_ << "\n";
					#endif
			return 3;
		}
		edge = findFirstEdge(indices);
		if (edge != NULL)
		{
			TRSVertex<T>* vertex1 = edge->vertex0_;
			TRSVertex<T>* vertex2 = edge->vertex1_;
			new_vertices.insert(vertex1);
			new_vertices.insert(vertex2);
			vertices[vertex1->atom_].push_back(vertex1);
			vertices[vertex2->atom_].push_back(vertex2);
			return 2;
		}
		vertex = findFirstVertex(indices);
		if (vertex != NULL)
		{
			new_vertices.insert(vertex);
			vertices[vertex->atom_].push_back(vertex);
			return 1;
		}
		return 0;
	}


	template <typename T>
	TRSFace<T>* TReducedSurface<T>::findFirstFace(HashSet<Index>& indices)
		throw(Exception::DivisionByZero)
	{
//std::cout << "  $1\n";
		for (Byte direction = 0; direction < 3; direction++)
		{
			for (Byte extrem = 0; extrem < 1; extrem++)
			{
//std::cout << "  $2\n";
				TRSFace<T>* face = findFace(indices,direction,extrem);
				if (face != NULL)
				{
//std::cout << "  $3\n";
					return face;
				}
			}
		}
//std::cout << "  $4\n";
		return NULL;
	}


	template <class T>
	TRSFace<T>* TReducedSurface<T>::findFace
		 (HashSet<Index>& indices,
			Byte						direction,
			Byte						extrem)
		throw(Exception::DivisionByZero)
	{
//std::cout << "    §1\n";
		if (indices.size() == 0)
		{
			return NULL;
		}
//std::cout << "    §2\n";
		Index a1 = findFirstAtom(indices,direction,extrem);
//std::cout << "    §3\n";
		Index a2 = findSecondAtom(a1,direction,extrem);
//std::cout << "    §4\n";
		if (a2 == -1)
		{
			return NULL;
		}
//std::cout << "    §5\n";
		::std::list<Index> atom_list;
		neighboursOfTwoAtoms(a1,a2,atom_list);
//std::cout << "    §6\n";
		list< pair< Index,TSphere3<T> > > candidates;
		findThirdAtom(a1,a2,atom_list,candidates);
//std::cout << "    §7\n";
		if (candidates.size() == 0)
		{
			return NULL;
		}
//std::cout << "    §8\n";
		std::list< pair< Index,TSphere3<T> > >::iterator i = candidates.begin();
		Index a3 = candidates.begin()->first;
		TSphere3<T> probe = candidates.begin()->second;
//std::cout << "    §9\n";
		while (checkProbe(probe,a1,a2,a3) == false)
		{
			i++;
			if (i == candidates.end())
			{
				return NULL;
			}
			a3 = i->first;
			probe = i->second;
		}
//std::cout << "    §10\n";
		TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
		TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
		TRSVertex<T>* vertex3 = new TRSVertex<T>(a3);
		TRSEdge<T>* e1 = new TRSEdge<T>;
		TRSEdge<T>* e2 = new TRSEdge<T>;
		TRSEdge<T>* e3 = new TRSEdge<T>;
		TRSFace<T>* face = new TRSFace<T>;
		updateFaceAndEdges(vertex1,vertex2,vertex3,e1,e2,e3,face,probe);
		face->index_ = number_of_faces_;
		faces_.push_back(face);
		number_of_faces_++;
		vertex1->faces_.insert(face);
		vertex2->faces_.insert(face);
		vertex3->faces_.insert(face);
		vertex1->edges_.insert(e1);
		vertex2->edges_.insert(e1);
		vertex2->edges_.insert(e2);
		vertex3->edges_.insert(e2);
		vertex3->edges_.insert(e3);
		vertex1->edges_.insert(e3);
		indices.erase(a1);
		indices.erase(a2);
		indices.erase(a3);
		vertex1->index_ = number_of_vertices_;
		vertices_.push_back(vertex1);
		number_of_vertices_++;
		vertex2->index_ = number_of_vertices_;
		vertices_.push_back(vertex2);
		number_of_vertices_++;
		vertex3->index_ = number_of_vertices_;
		vertices_.push_back(vertex3);
		number_of_vertices_++;
//std::cout << "    §11\n";
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
		Index a2 = findSecondAtom(a1,direction,extrem);
		if (a2 == -1)
		{
			return NULL;
		}
		TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
		TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
		::std::list<Index> atoms;
		neighboursOfTwoAtoms(a1,a2,atoms);
		TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,atoms);
		if (edge != NULL)
		{
			edge->index_ = number_of_edges_;
			edges_.push_back(edge);
			number_of_edges_++;
			vertex1->edges_.insert(edge);
			vertex1->index_ = number_of_vertices_;
			vertices_.push_back(vertex1);
			number_of_vertices_++;
			indices.erase(a1);
			vertex2->edges_.insert(edge);
			vertex2->index_ = number_of_vertices_;
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
					vertex->index_ = number_of_vertices_;
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
			(Index atom,
			 Byte	 direction,
			 Byte	 extrem)
		throw()
	{
		if (neighbours_[atom].size() == 0)
		{
			neighboursOfOneAtom(atom);
			if (neighbours_[atom].size() == 0)
			{
				return -1;
			}
		}
		TSphere3<T> first_atom(atom_[atom]);
		first_atom.radius += probe_radius_;
		TCircle3<T> intersection_circle;
		T x_min = ((extrem == 0) ? first_atom.p[direction]+first_atom.radius
														 : first_atom.p[direction]-first_atom.radius);
		Index a_min = -1;
		TSphere3<T> next_atom;
		std::list<Index>::iterator i;
		for (i = neighbours_[atom].begin(); i != neighbours_[atom].end(); i++)
		{
			next_atom = atom_[*i];
			next_atom.radius += probe_radius_;
			if (GetIntersection(first_atom,next_atom,intersection_circle))
			{
				T next_min = getCircleExtremum(intersection_circle,direction,extrem);
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
	void TReducedSurface<T>::findThirdAtom
		 (Index																						 atom1,
			Index																						 atom2,
			const ::std::list<Index>&												 third,
			::std::list< ::std::pair< Index,TSphere3<T> > >& atoms)
		throw()
	{
		// This function computes a list of all atoms (with its probe positions)	
		// which can be touched by the probe sphere when it touches the two given	
		// atoms
				#ifdef debug_rs
				pre += "    ";
				print << pre << atom1 << ", " << atom2 << ", ...\n";
				#endif
				#ifdef print_rs_debug_info
				std::cout << "  probe positions an " << atom1 << ", " << atom2 << ", ...\n";
				#endif
		::std::pair< Index,TSphere3<T> > candidate;
		::std::list<Index>::const_iterator i = third.begin();
		TVector3<T> center1, center2;
		TSphere3<T> probe;
		while (i != third.end())
		{
					#ifdef debug_rs
					print << pre << "  ... " << *i << ": ";
					#endif
					#ifdef print_rs_debug_info
					std::cout << "    ... " << *i << ": ";
					#endif
			if (centerOfProbe(atom1,atom2,*i,center1,center2))
			{
				if ((Maths::isNan(center1.x) ||
						 Maths::isNan(center1.y) ||
						 Maths::isNan(center1.z)   ) == false)
				{
					probe.set(center1,probe_radius_);
					candidate.first = *i;
					candidate.second = probe;
							#ifdef debug_rs
							print << probe.p << " / ";
							#endif
							#ifdef print_rs_debug_info
							std::cout << probe.p << " / ";
							#endif
					atoms.push_back(candidate);
				}
				if ((Maths::isNan(center2.x) ||
						 Maths::isNan(center2.y) ||
						 Maths::isNan(center2.z)   ) == false)
				{
					probe.p = center2;
					candidate.first = *i;
					candidate.second = probe;
							#ifdef debug_rs
							print << probe.p;
							#endif
							#ifdef print_rs_debug_info
							std::cout << probe.p << "\n";
							#endif
					atoms.push_back(candidate);
				}
			}
					#ifdef print_rs_debug_info
					else
					{
						std::cout << " -- / --\n";
					}
					#endif
					#ifdef debug_rs
					print << "\n";
					#endif
			i++;
		}
				#ifdef debug_rs
				pre.replace(0,4,"");
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfOneAtom(Index atom)
		throw()
	{
				#ifdef print_rs_debug_info
				std::cout << "  suche Nachbarn von " << atom << "\n";
				#endif
		TSphere3<T> first_atom(atom_[atom]);
		TSphere3<T> next_atom;
		T offset = first_atom.radius+2*probe_radius_;
		HashGridBox3<Position>* box = grid_.getBox(first_atom.p);
		HashGridBox3<Position>::BoxIterator b;
		HashGridBox3<Position>::DataIterator d;
		for (b = box->beginBox(); b != box->endBox(); b++)
		{
			for (d = b->beginData(); d != b->endData(); d++)
			{
				if ((Index)*d != atom)
				{
					next_atom = atom_[*d];
					T dist = next_atom.p.getSquareDistance(first_atom.p);
					T max_dist = next_atom.radius+offset;
					max_dist *= max_dist;
							#ifdef print_rs_debug_info
							std::cout << "  " << *d << " ... ";
							#endif
					if (Maths::isLessOrEqual(dist,max_dist))
					{
						neighbours_[atom].push_back(*d);
								#ifdef print_rs_debug_info
								std::cout << "ja\n";
								#endif
					}
							#ifdef print_rs_debug_info
							else
							{
								std::cout << "nein\n";
							}
							#endif
				}
			}
		}
		neighbours_[atom].sort();
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfTwoAtoms
		 (Index								atom1,
			Index								atom2,
			::std::list<Index>& output_list)
		throw()
	{
		if (neighbours_[atom1].size() == 0)
		{
			neighboursOfOneAtom(atom1);
		}
		if (neighbours_[atom2].size() == 0)
		{
			neighboursOfOneAtom(atom2);
		}
		std::list<Index>::iterator i1 = neighbours_[atom1].begin();
		std::list<Index>::iterator i2 = neighbours_[atom2].begin();
		while ((i1 != neighbours_[atom1].end()) && (i2 != neighbours_[atom2].end()))
		{
			if (*i1 == *i2)
			{
				output_list.push_back(*i1);
				i1++;
				i2++;
			}
			else
			{
				if (*i1 < *i2)
				{
					i1++;
				}
				else
				{
					i2++;
				}
			}
		}
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfThreeAtoms
		 (Index								atom1,
			Index								atom2,
			Index								atom3,
			::std::list<Index>& output_list)
		throw()
	{
		if (neighbours_[atom1].size() == 0)
		{
			neighboursOfOneAtom(atom1);
		}
		if (neighbours_[atom2].size() == 0)
		{
			neighboursOfOneAtom(atom2);
		}
		if (neighbours_[atom3].size() == 0)
		{
			neighboursOfOneAtom(atom3);
		}
		std::list<Index>::iterator i1 = neighbours_[atom1].begin();
		std::list<Index>::iterator i2 = neighbours_[atom2].begin();
		std::list<Index>::iterator i3 = neighbours_[atom3].begin();
		while ((i1 != neighbours_[atom1].end()) &&
					 (i2 != neighbours_[atom2].end()) &&
					 (i3 != neighbours_[atom3].end()))
		{
			if ((*i1 == *i2) && (*i1 == *i3))
			{
				output_list.push_back(*i1);
				i1++;
				i2++;
				i3++;
			}
			else
			{
				if (*i1 < *i2)
				{
					i1++;
				}
				else
				{
					if (*i3 < *i2)
					{
						i3++;
					}
					else
					{
						i2++;
					}
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
		if (getCircles(vertex1->atom_,vertex2->atom_,
									 circle1,circle2,circle3) &&
				Maths::isGreater(circle1.radius,probe_radius_)		 )
		{
			TPlane3<T> plane(circle1.p,circle1.n);
			HashSet<Index>::ConstIterator i;
			TCircle3<T> test_circle;
			TSphere3<T> sphere;
			T dist;
			for (i = neighbours.begin(); i != neighbours.end(); i++)
			{
				sphere.set(atom_[*i].p,atom_[*i].radius+probe_radius_);
				if (GetIntersection(sphere,plane,test_circle))
				{
					dist = test_circle.radius-circle1.radius;
					if (Maths::isGreaterOrEqual(dist*dist,
												test_circle.p.getSquareDistance(circle1.p))	)
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
	TRSEdge<T>* TReducedSurface<T>::createFreeEdge
			 (TRSVertex<T>*							vertex1,
				TRSVertex<T>*							vertex2,
				const ::std::list<Index>& neighbours)
		throw()
	{
		TCircle3<T> circle1;
		TCircle3<T> circle2;
		TCircle3<T> circle3;
				#ifdef print_rs_debug_info
				std::cout << "  versuche freie Edge zu bilden mit " << vertex1->atom_
									<< " und " << vertex2->atom_ << "\n";
				#endif
		if (getCircles(vertex1->atom_,vertex2->atom_,circle1,circle2,circle3) &&
				Maths::isGreater(circle1.radius,probe_radius_)											 )
		{
			TPlane3<T> plane(circle1.p,circle1.n);
			::std::list<Index>::const_iterator i;
			TCircle3<T> test_circle;
			TSphere3<T> sphere;
			T dist;
			for (i = neighbours.begin(); i != neighbours.end(); i++)
			{
				sphere.set(atom_[*i].p,atom_[*i].radius+probe_radius_);
				if (GetIntersection(sphere,plane,test_circle))
				{
					dist = test_circle.radius-circle1.radius;
					if (Maths::isGreaterOrEqual(dist*dist,
												test_circle.p.getSquareDistance(circle1.p)) )
					{
						#ifdef print_rs_debug_info
						std::cout << "    Atom " << *i << " zu nahe, keine freie Edge\n";
						#endif
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
				#ifdef print_rs_debug_info
				std::cout << "    " << atom1 << " und " << atom2 << " zu weit auseinander\n";
				#endif
		return false;
	}


	template <typename T>
	TVector3<T> TReducedSurface<T>::getFaceNormal
		 (const TSphere3<T>& atom1,
			const TSphere3<T>& atom2,
			const TSphere3<T>& atom3,
			const TSphere3<T>& probe)
		throw(Exception::DivisionByZero)
	{
		TPlane3<T> plane(atom1.p,atom2.p,atom3.p);
		TVector3<T> norm(plane.n);
		if (Maths::isLess(norm*probe.p,norm*atom1.p))
		{
			norm.negate();
		}
		//norm.normalize();
		return norm;
	}


	template <typename T>
	void TReducedSurface<T>::updateFaceAndEdges
		 (TRSVertex<T>* v1,
			TRSVertex<T>* v2,
			TRSVertex<T>* v3,
			TRSEdge<T>* e1,
			TRSEdge<T>* e2,
			TRSEdge<T>* e3,
			TRSFace<T>* f,
			const TSphere3<T>& probe)
		throw(Exception::DivisionByZero)
	{
		TVector3<T> vector(0,0,0);
		TAngle<T> angle(0,true);
		TCircle3<T> circle(vector,vector,0);
		e1->vertex0_ = v1;
		e1->vertex1_ = v2;
		e1->face0_ = f;
		e2->vertex0_ = v2;
		e2->vertex1_ = v3;
		e2->face0_ = f;
		e3->vertex0_ = v3;
		e3->vertex1_ = v1;
		e3->face0_ = f;
		TCircle3<T> dummy;
		f->vertex0_ = v1;
		f->vertex1_ = v2;
		f->vertex2_ = v3;
		f->edge0_ = e1;
		f->edge1_ = e2;
		f->edge2_ = e3;
		f->center_ = probe.p;
		TPlane3<T> plane(atom_[v1->atom_].p,atom_[v2->atom_].p,atom_[v3->atom_].p);
		f->normal_ = plane.n;
		if (Maths::isLess(f->normal_*probe.p,f->normal_*atom_[v1->atom_].p))
		{
			f->normal_.negate();
		}
		f->normal_.normalize();
		f->singular_ = Maths::isLess(GetDistance(probe.p,plane),probe_radius_);
	}


	template <typename T>
	TRSFace<T>* TReducedSurface<T>::faceExists
		 (TRSFace<T>* face,
			const std::list< TRSVertex<T>* >& vertices)
		throw()
	{
		typename std::list< TRSVertex<T>* >::const_iterator v;
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
		Index tmp;
		if (a1 > a2)
		{
			tmp = a1;
			a1 = a2;
			a2 = tmp;
		}
		if (a1 > a3)
		{
			tmp = a1;
			a1 = a3;
			a3 = tmp;
		}
		if (a2 > a3)
		{
			tmp = a2;
			a2 = a3;
			a3 = tmp;
		}
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
	bool TReducedSurface<T>::checkProbe(const TSphere3<T>& probe,
			Index atom1,
			Index atom2,
			Index atom3)
		throw()
	{
				#ifdef print_rs_debug_info
				std::cout << "check probe sphere " << probe << " with atom ...\n";
				#endif
		std::list<Index> atom_list;
		neighboursOfThreeAtoms(atom1,atom2,atom3,atom_list);
		T dist;
		std::list<Index>::iterator i;
		for (i = atom_list.begin(); i != atom_list.end(); i++)
		{
					#ifdef print_rs_debug_info
					std::cout << "... " << *i;
					#endif
			dist = probe.radius+atom_[*i].radius;
			if (Maths::isLess(probe.p.getSquareDistance(atom_[*i].p),dist*dist))
			{
						#ifdef debug_rs
						print << pre << " ... Konflikt mit Atom " << *i << " ... "
									<< probe.p.getDistance(atom_[*i].p) << " < "
									<< probe.radius+atom_[*i].radius << "\n";
						#endif
						#ifdef print_rs_debug_info
						std::cout << " ... not ok ... " << probe.p.getDistance(atom_[*i].p) << " < "
											<< probe.radius+atom_[*i].radius << "\n";
						if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						#endif
				return false;
			}
					#ifdef print_rs_debug_info
					else
					{
						std::cout << " ... ok ... " << probe.p.getDistance(atom_[*i].p) << " >= "
											<< probe.radius+atom_[*i].radius << "\n";
					}
					#endif
		}
		#ifdef print_rs_debug_info
		if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
		#endif
		return true;
	}


} // namespace BALL

#endif  // BALL_STRUCTURE_REDUCEDSURFACE_H
