// $Id: reducedSurface.h,v 1.26 2001/11/08 16:44:45 strobel Exp $

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#define BALL_STRUCTURE_REDUCEDSURFACE_H

//#define print_rs_debug_info
//#define debug_surface_processor
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
#include <BALL/SYSTEM/timer.h>


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
			//Timer RStimer;
			//Position PROBE_COUNTER1;
			//Position PROBE_COUNTER2;
			//Position PROBE_COUNTER3;
			//Position PROBE_COUNTER4;
			//Position COUNTER;


	template <typename T>
	class TSolventExcludedSurface;

	/** Generic ReducedSurface Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/reducedSurface.h}
	*/
	template <typename T>
	class TReducedSurface
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class TSolventExcludedSurface<T>
				\end{itemize}
		*/
		friend class TSolventExcludedSurface<T>;

		BALL_CREATE(TReducedSurface)

		/**	@name	Enums
		*/
		//@{

		/** status of the probe positions of three atoms:
				{\tt 0} ok
				{\tt 1} not ok
				{\tt 2} not tested
				{\tt 2} just treated
		*/
		enum ProbeStatus
		{
			STATUS_OK  = 0,
			STATUS_NOT_OK = 1,
			STATUS_NOT_TESTED = 2,
			STATUS_JUST_TREATED = 3,
			STATUS_NOT_EXISTING = 4
		};

		/** status of an atom
				{\tt 0} on surface
				{\tt 1} inside
				{\tt 2} outside
		*/
		enum AtomStatus
		{
			STATUS_ON_SURFACE  = 0,
			STATUS_INSIDE = 1,
			STATUS_UNKNOWN = 2
		};
		//@}

		struct ProbePosition
		{
			std::vector< ProbeStatus > status;
			std::vector< TVector3<T> > point;
		};

		/**	@name	Constructors and Destructors
		*/
		//@{

		void checkAndPrintStatus()
		{
			for (Position i = 0; i < number_of_atoms_; i++)
			{
				bool found = false;
				for (Position k = 0; k < number_of_vertices_; k++)
				{
					if (vertices_[k] != NULL)
					{
						if (vertices_[k]->atom_ == (Index)i)
						{
							found = true;
						}
					}
				}
				if (found)
				{
					switch (atom_status_[i])
					{
						case STATUS_ON_SURFACE	 :	//std::cout << i << "\t" << atom_[i] << "\t"
																				//					<< "on surface\tok\n";
																				break;
						case STATUS_INSIDE			 :	std::cout << i << "\t" << atom_[i] << "\t"
																									<< "inside molecule\tnot ok\n";
																				break;
						case STATUS_UNKNOWN			 :	std::cout << i << "\t" << atom_[i] << "\t"
																									<< "unknown\tnot ok\n";
																				break;
					}
				}
				else
				{
					switch (atom_status_[i])
					{
						case STATUS_ON_SURFACE	 :	std::cout << i << "\t" << atom_[i] << "\t"
																									<< "on surface\tnot ok\n";
																				break;
						case STATUS_INSIDE			 :	//std::cout << i << "\t" << atom_[i] << "\t"
																				//					<< "inside molecule\tok\n";
																				break;
						case STATUS_UNKNOWN			 :	//std::cout << i << "\t" << atom_[i] << "\t"
																				//					<< "unknown\tok\n";
																				break;
					}
				}
			}
		}
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

		/*_
		*/
		void preProcessing()
			throw();

		/*_ Compute the RSComponent of a given face
			@param	face					the starting face
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void getRSComponent
			 (HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		/*_ treat all edges of a face
			@param	face					the RSFace to be treated
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void treatFace
			 (TRSFace<T>*																	 face,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		/*_ roll over an edge that belongs to onlyone face and find the other one
			@param	edge					the RSEdge to be treated
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void treatEdge
			 (TRSEdge<T>*																	 edge,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		void correct(Index atom,
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  	throw();

		/*_ Check all new created vertices for extensions
			@param	new_vertices	a HashSet of all new created RSVertices
			@param	vertices			a list of RSVertices for each atom
		*/
		void extendComponent
			 (HashSet<TRSVertex<T>*>&											 new_vertices,
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
				HashSet<TRSVertex<T>*>&											 new_vertices,
				::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
			throw(Exception::GeneralException,Exception::DivisionByZero);

		//@}
		/*_ @name Finding a start position
		*/
		//@{

		/*_ Find a start position
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
			 (HashSet<TRSVertex<T>*>&										 new_vertices,
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
			@return	TRSFace<T>*	a pointer to the found face, if a face can be found,
													NULL otherwise
		*/
		TRSFace<T>* findFirstFace()
			throw(Exception::DivisionByZero);

		/*_ Try to find a starting face in a given direction
			@param	direction		search in x-direction, if direction is 0,
													search in y-direction, if direction is 1,
													search in z-direction, if direction is 2
			@param	extrem			search in min direction, if extrem is 0,
													search in max direction, if extrem is 1
			@return	TRSFace<T>*	a pointer to the found face, if a face can be found,
													NULL otherwise
		*/
		TRSFace<T>* findFace(Byte direction, Byte extrem)
			throw(Exception::DivisionByZero);

		//@}
		/*_ @name Finding a first edge
		*/
		//@{

		/*_ Try to find a starting edge
			@return	TRSEdge<T>*	a pointer to the found edge, if a face can be found,
													NULL otherwise
		*/
		TRSEdge<T>* findFirstEdge()
			throw();

		/*_ Try to find a starting edge in a given direction
			@param	direction		search in x-direction, if direction is 0,
													search in y-direction, if direction is 1,
													search in z-direction, if direction is 2
			@param	extrem			search in min direction, if extrem is 0,
													search in max direction, if extrem is 1
			@return	TRSEdge<T>*	a pointer to the found edge, if a face can be found,
													NULL otherwise
		*/
		TRSEdge<T>* findEdge(Byte direction, Byte extrem)
			throw();

		//@}
		/*_ @name Finding a first vertex
		*/
		//@{

		/*_ Try to find a single atom
			@return	TRSVertex<T>*	a pointer to the found vertex, if a vertex can be
														found, NULL otherwise
		*/
		TRSVertex<T>* findFirstVertex()
			throw();

		/*_ Find a single atom in a given direction
			@param	direction	search in x-direction, if direction is 0,
												search in y-direction, if direction is 1,
												search in z-direction, if direction is 2
			@param	extrem		search in min direction, if extrem is 0,
												search in max direction, if extrem is 1
			@return	Index			the index of the found atom
		*/
		Index findFirstAtom(Byte direction, Byte extrem)
			throw();

		/*_ Find a second atom close enougth to the first atom in a given direction
			@param	atom1			the index of the first atom
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

		/*_ Find all atoms close enougth to two given atoms.
			The indices of all atoms which can be touched by the probe sphere when	
			it touches the given atoms are computed.
			@param	atom1				the index of the first given atom
			@param	atom2				the index of the second given atom
			@param	output_list	list of all atoms close enougth to the given atoms
		*/
		void neighboursOfTwoAtoms(Index atom1, Index atom2)
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
		bool checkProbe(const TSphere3<T>& probe,
				Index atom1,
				Index atom2,
				Index atom3)
			throw();

		/*_
		*/
		void correctProbePosition(Position atom)
			throw();

		/*_
		*/
		void correctProbePosition(Position a1, Position a2, Position a3)
			throw();

		//@}

		protected:

    /*_ the number of atoms of the reduced surface
    */
    Size number_of_atoms_;
    /*_ the atoms of the molecule
    */
    std::vector< TSphere3<T> > atom_;
    /*_	probe radius
    */
    T probe_radius_;
		/*_ for each atom a list of its neighbours
		*/
		std::vector< std::list<Index> > neighbours_;
		/*_ for each atom a status
		*/
		std::vector< AtomStatus > atom_status_;
		/*_ for each pair of atoms a list of its neighbours
		*/
		HashMap< Position,HashMap< Position,std::list<Index> > > neighbours_of_two_;
		/*_ for each triple of atoms its probe positions
		*/
		HashMap< Position,
						 HashMap< Position,
											HashMap< Position,
															 ProbePosition*
														 >
										>
					 > probe_positions_;
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
		:	number_of_atoms_(0),
			atom_(),
			probe_radius_((T)0),
			neighbours_(),
			atom_status_(),
			neighbours_of_two_(),
			probe_positions_(),
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
		:	number_of_atoms_(reduced_surface.number_of_atoms_),
			atom_(reduced_surface.atom_),
			probe_radius_(reduced_surface.probe_radius_),
			neighbours_(reduced_surface.neighbours_),
			atom_status_(reduced_surface.atom_status_),
			neighbours_of_two_(reduced_surface.neighbours_of_two_),
			probe_positions_(reduced_surface.probe_positions_),
			vertices_(reduced_surface.number_of_vertices_),
			edges_(reduced_surface.number_of_edges_),
			faces_(reduced_surface.number_of_faces_),
			r_max_(reduced_surface.r_max_),
			number_of_vertices_(reduced_surface.number_of_vertices_),
			number_of_edges_(reduced_surface.number_of_edges_),
			number_of_faces_(reduced_surface.number_of_faces_)
	{			// BAUSTELLE !!! probe_positions_ !!!
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
		typename HashSet<TRSEdge<T>*>::ConstIterator e;
		typename HashSet<TRSFace<T>*>::ConstIterator f;
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
		:	number_of_atoms_(spheres.size()),
			atom_(spheres),
			probe_radius_(probe_radius),
			neighbours_(number_of_atoms_),
			atom_status_(number_of_atoms_,STATUS_UNKNOWN),
			neighbours_of_two_(),
			probe_positions_(),
			vertices_(),
			edges_(),
			faces_(),
			r_max_((T)0),
			number_of_vertices_(0),
			number_of_edges_(0),
			number_of_faces_(0)
	{
		for (Position i = 0; i < number_of_atoms_; i++)
		{
			r_max_ = ((r_max_ < atom_[i].radius) ? atom_[i].radius : r_max_);
		}
				#ifdef print_rs_debug_info
				HALT = 0;
				#endif
				//PROBE_COUNTER1 = 0;
				//PROBE_COUNTER2 = 0;
				//PROBE_COUNTER3 = 0;
				//PROBE_COUNTER4 = 0;
				//COUNTER = 0;
	}


	template <typename T>
	TReducedSurface<T>::~TReducedSurface()
		throw()
	{
				//std::cout << "destructor ... " << RStimer.getUserTime() << "\n";
		clear();
				//std::cout << "/destructor ... " << RStimer.getUserTime() << "\n";
				//std::cout << PROBE_COUNTER1 << "\n" << PROBE_COUNTER2 << "\n"
				//					<< PROBE_COUNTER3 << "\n" << PROBE_COUNTER4 << "\n";
				//std::cout << COUNTER << "\n";
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
	{		//	BAUSTELLE !!!  probe_positions_ !!!
		number_of_atoms_ = reduced_surface.number_of_atoms_;
		atom_status_ = reduced_surface.atom_status_;
		neighbours_of_two_ = reduced_surface.neighbours_of_two_;
		//probe_positions_ = reduced_surface.probe_positions_;
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
		typename HashSet<TRSEdge<T>*>::ConstIterator e;
		typename HashSet<TRSFace<T>*>::ConstIterator f;
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
		typename HashMap< Position,HashMap< Position,HashMap< Position,ProbePosition* > > >::Iterator pp;
		typename HashMap< Position,HashMap< Position,ProbePosition* > >::Iterator pp1;
		typename HashMap< Position,ProbePosition* >::Iterator pp2;
		for (pp = probe_positions_.begin(); pp != probe_positions_.end(); pp++)
		{
			for (pp1 = pp->second.begin(); pp1 != pp->second.end(); pp1++)
			{
				for (pp2 = pp1->second.begin(); pp2 != pp1->second.end(); pp2++)
				{
					delete pp2->second;
				}
			}
		}
		probe_positions_.clear();
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
		return number_of_atoms_;
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
		if (i < number_of_atoms_)
		{
			return atom_[i];
		}
		else
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__,i,number_of_atoms_-1);
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
				#ifdef print_rs_debug_info
				std::cout << "computing ...\n";
				HALT = 0;
				#endif
				//RStimer.start();
				//std::cout << "starte ... " << RStimer.getUserTime() << "\n";
		// find the neighbours of the atoms
		preProcessing();
		// all new created vertices which are not yet checked for extensions
		HashSet<TRSVertex<T>*> new_vertices;
		// start position
		TRSVertex<T>* start_vertex(NULL);
		TRSEdge<T>* start_edge(NULL);
		TRSFace<T>* start_face(NULL);
		// for each atom a list of the rsvertices of the atom
		std::vector< std::list<TRSVertex<T>*> > vertices(number_of_atoms_);
		Byte start = 1;
		while (start != 0)
		{
			start = getStartPosition(new_vertices,start_vertex,start_edge,start_face,vertices);
			switch (start)
			{
				case 2 :	extendComponent(new_vertices,vertices);
									break;
				case 3 :
									#ifdef print_rs_debug_info
									std::cout << "Start-Face gefunden: " << *start_face << "\n";
									std::cout << *(start_face->vertex0_) << "\n";
									std::cout << *(start_face->vertex1_) << "\n";
									std::cout << *(start_face->vertex2_) << "\n";
									if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
									#endif
									getRSComponent(new_vertices,vertices);
									break;
				default :	break;
			}
		}
		clean();
				//std::cout << "fertig ... " << RStimer.getUserTime() << "\n";
	}


	template <typename T>
	void TReducedSurface<T>::getRSComponent
		 (HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
		throw(Exception::GeneralException,Exception::DivisionByZero)
	{
				//std::cout << "getRSComponent ... " << RStimer.getUserTime() << "\n";
		for (Position i = 0; i < number_of_faces_; i++)
		{
			if (faces_[i] != NULL)
			{
				treatFace(faces_[i],new_vertices,vertices);
			}
		}
		//checkAndPrintStatus();
		extendComponent(new_vertices,vertices);
				//std::cout << "/getRSComponent ... " << RStimer.getUserTime() << "\n";
	}


	template <typename T>
	void TReducedSurface<T>::treatFace
		 (TRSFace<T>*																	 face,
			HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw(Exception::GeneralException,Exception::DivisionByZero)
	{
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
				treatEdge(edge,new_vertices,vertices);
			}
		}
				#ifdef print_rs_debug_info
				std::cout << "end        // behandle " << *face;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
	}


	template <typename T>
	void TReducedSurface<T>::treatEdge
		 (TRSEdge<T>*																	 edge,
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
			atom3 = thirdAtom(vertex1,vertex2,start_face,probe,phi,new_vertices,vertices);
		}
		catch (Exception::GeneralException e)
		{
			String message = e.getMessage();
			String test_message = "PROBE SPHERE TOUCHES FOUR ATOMS";
			if (message == test_message)
			{
				treatEdge(edge,new_vertices,vertices);
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
		}
		else
		{
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
			new_face = test;
		}			// face exitsts test
		// update edge
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
		if (edge->index_ == -1)
		{
			edge->index_ = number_of_edges_;
			edges_.push_back(edge);
			number_of_edges_++;
		}
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
	}


	template <typename T>
	void TReducedSurface<T>::correct(Index atom,
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
		typename ::std::list<TRSVertex<T>*>::iterator v;
		TRSVertex<T>* vertex;
		HashSet<TRSFace<T>*> faces;
		HashSet<TRSFace<T>*> treat_faces;
		typename HashSet<TRSFace<T>*>::Iterator f;
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
			typename HashSet<TRSVertex<T>*>::Iterator test;
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
							#ifdef print_rs_debug_info
							std::cout << "verkleinere Atom " << atom << "\n";
							#endif
		atom_[atom].radius -= 10*Constants::EPSILON;
		atom_status_[atom] = STATUS_UNKNOWN;
				#ifdef print_rs_debug_info
				std::cout << "status of atom" << atom << " set to STATUS_UNKNOWN\n";
				#endif
		correctProbePosition(atom);
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
		 (HashSet<TRSVertex<T>*>&											 new_vertices,
			::std::vector< ::std::list<TRSVertex<T>*> >& vertices)
	  throw(Exception::GeneralException,Exception::DivisionByZero)
	{
				#ifdef print_rs_debug_info
				std::cout << "extendComponent";
				HALT = 0;
				if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
				#endif
				//std::cout << "extend ... " << RStimer.getUserTime() << "\n";
		while (new_vertices.size() != 0)
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
			typename ::std::list<Index>::const_iterator i;
					#ifdef print_rs_debug_info
					std::cout << neighbours_[atom1].size() << " Nachbarn gefunden\n";
					for (i = neighbours_[atom1].begin(); i != neighbours_[atom1].end(); i++)
					{
						std::cout << *i << "  ";
					}
					if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
					#endif
					//std::cout << atom_list1.size() << " Nachbarn (1) gefunden\n";
			i = neighbours_[atom1].begin();
			while (i != neighbours_[atom1].end())
			{
				if (atom_status_[*i] == STATUS_UNKNOWN)
				{
					Index atom2 = *i;
							#ifdef print_rs_debug_info
							std::cout << "suche drittes Atom zu " << atom1 << " und " << atom2 << "\n";
							#endif
					neighboursOfTwoAtoms(atom1,atom2);
					std::list< std::pair< Index,TSphere3<T> > > candidates;
					findThirdAtom(atom1,atom2,neighbours_of_two_[atom1][atom2],candidates);
							#ifdef print_rs_debug_info
							std::cout << candidates.size() << " mögliche probe-Positionen: ";
							typename std::list< std::pair< Index,TSphere3<T> > >::iterator cit;
							for (cit = candidates.begin(); cit != candidates.end(); cit++)
							{
								std::cout << cit->first << "  ";
							}
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					if (candidates.size() == 0)
					{
						TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
						TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,neighbours_of_two_[atom1][atom2]);
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
							vertices[atom2].push_back(vertex2);
							new_vertices.insert(vertex1);
							new_vertices.insert(vertex2);
							atom_status_[atom2] = STATUS_ON_SURFACE;
									#ifdef print_rs_debug_info
									std::cout << "freie edge erzeugt: " << *edge;
									HALT = 0;
									if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
									#endif
							i = neighbours_[atom1].end();
						}
						else
						{
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
						typename std::list< std::pair< Index,TSphere3<T> > >::iterator j;
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
							if (atom_status_[j->first] == STATUS_UNKNOWN)
							{
								Index atom3 = j->first;
								TSphere3<T> probe = j->second;
										#ifdef print_rs_debug_info
										std::cout << "3. Atom: " << atom3 << ", probe: " << probe << "...\n";
										#endif
								if (checkProbe(probe,atom1,atom2,atom3) == true)
								{
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
									vertex3->index_ = number_of_vertices_;
									vertices_.push_back(vertex3);
									number_of_vertices_++;
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
									atom_status_[atom2] = STATUS_ON_SURFACE;
									atom_status_[atom3] = STATUS_ON_SURFACE;
									i = neighbours_[atom1].end();
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
											#ifdef print_rs_debug_info
											std::cout << "... passt nicht";
											if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
											#endif
									j++;
								}
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
				}
				else
				{
					i++;
				}
			} // while i
			if (face != NULL)
			{
				getRSComponent(new_vertices,vertices);
			}
		}
				//std::cout << "/extend ... " << RStimer.getUserTime() << "\n";
	}


	template <typename T>
	Index TReducedSurface<T>::thirdAtom
		 (TRSVertex<T>* 															 vertex1,
			TRSVertex<T>*															 	 vertex2,
			TRSFace<T>*																	 face,
			TSphere3<T>&																 probe,
			TAngle<T>&																	 phi,
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
		Index atom1(vertex1->atom_);
		Index atom2(vertex2->atom_);
		//::std::list<Index> atom_list;
		neighboursOfTwoAtoms(atom1,atom2);
		//if (atom_list.size() == 0)
		if (neighbours_of_two_[atom1][atom2].size() == 0)
		{
			throw Exception::GeneralException
							(__FILE__,__LINE__,
							 "CanNotFindThirdAtom","no atom close enougth");
		}
		list< pair< Index,TSphere3<T> > > candidates;
		//findThirdAtom(atom1,atom2,atom_list,candidates);
		findThirdAtom(atom1,atom2,neighbours_of_two_[atom1][atom2],candidates);
		typename std::list< std::pair< Index,TSphere3<T> > >::iterator k;
		Index back(-1);
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
				#ifdef print_rs_debug_info
				std::cout << "  Dreahachse: " << atom1 << " --> " << atom2 << "\n";
				#endif
		}
				#ifdef print_rs_debug_info
				else
				{
					std::cout << "  Dreahachse: " << atom2 << " --> " << atom1 << "\n";
				}
				#endif
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
		std::list< std::pair< Index,TSphere3<T> > > third;
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
					correct(k->first,new_vertices,vertices);
							#ifdef debug_surface_processor_verbose
							std::cerr << "    ... ok\n";
							#endif
					throw Exception::GeneralException
							(__FILE__,__LINE__,"CAN'T COMPUTE RS","PROBE SPHERE TOUCHES FOUR ATOMS");
				}
				if (new_angle < old_angle)
				{
					old_angle = new_angle;
					typename std::list< std::pair< Index,TSphere3<T> > >::iterator t;
					for (t = third.begin(); t != third.end(); t++)
					{
						if (atom_status_[t->first] == STATUS_UNKNOWN)
						{
							atom_status_[t->first] = STATUS_INSIDE;
									#ifdef print_rs_debug_info
									std::cout << "status of atom" << t->first << " set to STATUS_INSIDE\n";
									#endif
						}
					}
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
					else
					{
						if (atom_status_[k->first] == STATUS_UNKNOWN)
						{
							atom_status_[k->first] = STATUS_INSIDE;
									#ifdef print_rs_debug_info
									std::cout << "status of atom" << k->first << " set to STATUS_INSIDE\n";
									#endif
						}
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
				correct(k->first,new_vertices,vertices);
				k++;
			}
					#ifdef debug_surface_processor_verbose
					std::cerr << "    ... ok\n";
					#endif
			throw Exception::GeneralException
					(__FILE__,__LINE__,"CAN'T COMPUTE RS","PROBE SPHERE TOUCHES FOUR ATOMS");
		}
		back = third.front().first;
		probe = third.front().second;
		phi.set((T)old_angle.value,true);
		atom_status_[back] = STATUS_ON_SURFACE;
				#ifdef print_rs_debug_info
				std::cout << "status of atom" << back << " set to STATUS_ON_SURFACE\n";
				#endif
		return back;
	}


	template <typename T>
	Byte TReducedSurface<T>::getStartPosition
		 (HashSet<TRSVertex<T>*>&										 new_vertices,
			TRSVertex<T>*&														 vertex,
			TRSEdge<T>*&															 edge,
			TRSFace<T>*&															 face,
			std::vector< std::list< TRSVertex<T>* > >& vertices)
		throw(Exception::DivisionByZero)
	{
		face = findFirstFace();
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
			atom_status_[vertex1->atom_] = STATUS_ON_SURFACE;
			atom_status_[vertex2->atom_] = STATUS_ON_SURFACE;
			atom_status_[vertex3->atom_] = STATUS_ON_SURFACE;
			return 3;
		}
		edge = findFirstEdge();
		if (edge != NULL)
		{
			TRSVertex<T>* vertex1 = edge->vertex0_;
			TRSVertex<T>* vertex2 = edge->vertex1_;
			new_vertices.insert(vertex1);
			new_vertices.insert(vertex2);
			vertices[vertex1->atom_].push_back(vertex1);
			vertices[vertex2->atom_].push_back(vertex2);
			atom_status_[vertex1->atom_] = STATUS_ON_SURFACE;
			atom_status_[vertex2->atom_] = STATUS_ON_SURFACE;
			return 2;
		}
		vertex = findFirstVertex();
		if (vertex != NULL)
		{
			new_vertices.insert(vertex);
			vertices[vertex->atom_].push_back(vertex);
			atom_status_[vertex->atom_] = STATUS_ON_SURFACE;
			return 1;
		}
		return 0;
	}


	template <typename T>
	TRSFace<T>* TReducedSurface<T>::findFirstFace()
		throw(Exception::DivisionByZero)
	{
		for (Byte direction = 0; direction < 3; direction++)
		{
			for (Byte extrem = 0; extrem < 1; extrem++)
			{
				TRSFace<T>* face = findFace(direction,extrem);
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
		 (Byte						direction,
			Byte						extrem)
		throw(Exception::DivisionByZero)
	{
		Index a1 = findFirstAtom(direction,extrem);
		if (a1 == -1)
		{
			return NULL;
		}
		Index a2 = findSecondAtom(a1,direction,extrem);
		if (a2 == -1)
		{
			return NULL;
		}
		neighboursOfTwoAtoms(a1,a2);
		list< pair< Index,TSphere3<T> > > candidates;
		findThirdAtom(a1,a2,neighbours_of_two_[a1][a2],candidates);
		if (candidates.size() == 0)
		{
			return NULL;
		}
		typename std::list< pair< Index,TSphere3<T> > >::iterator i = candidates.begin();
		Index a3;
		TSphere3<T> probe;
		bool found = false;
		while ((found == false) && (i != candidates.end()))
		{
			a3 = i->first;
			probe = i->second;
			if (atom_status_[a3] == STATUS_UNKNOWN)
			{
				if (checkProbe(probe,a1,a2,a3))
				{
					found = true;
				}
			}
			i++;
		}
		if (found)
		{
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
			vertex1->index_ = number_of_vertices_;
			vertices_.push_back(vertex1);
			number_of_vertices_++;
			vertex2->index_ = number_of_vertices_;
			vertices_.push_back(vertex2);
			number_of_vertices_++;
			vertex3->index_ = number_of_vertices_;
			vertices_.push_back(vertex3);
			number_of_vertices_++;
			return face;
		}
		else
		{
			atom_status_[a1] = STATUS_INSIDE;
			atom_status_[a2] = STATUS_INSIDE;
			return NULL;
			//throw Exception::GeneralException(__FILE__,__LINE__,"XXX","xxx");
		}
	}


	template <typename T>
	TRSEdge<T>* TReducedSurface<T>::findFirstEdge()
		throw()
	{
		for (Byte direction = 0; direction < 3; direction++)
		{
			for (Byte extrem = 0; extrem < 1; extrem++)
			{
				TRSEdge<T>* edge = findEdge(direction,extrem);
				if (edge != NULL)
				{
					return edge;
				}
			}
		}
		return NULL;
	}


	template <typename T>
	TRSEdge<T>* TReducedSurface<T>::findEdge(Byte direction, Byte extrem)
		throw()
	{
		Index a1 = findFirstAtom(direction,extrem);
		if (a1 == -1)
		{
			return NULL;
		}
		Index a2 = findSecondAtom(a1,direction,extrem);
		if (a2 == -1)
		{
			return NULL;
		}
		TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
		TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
		//neighboursOfTwoAtoms(a1,a2);
		//TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,neighbours_of_two_[a1][a2]);
		TCircle3<T> circle1;
		TCircle3<T> circle2;
		TCircle3<T> circle3;
		if (getCircles(a1,a2,circle1,circle2,circle3) &&
				Maths::isGreater(circle1.radius,probe_radius_)		 )
		{
			TVector3<T> vector(0,0,0);
			TRSEdge<T>* edge = new TRSEdge<T>(vertex1,vertex2,NULL,NULL,
					 															circle1.p,circle1.radius,
																				TAngle<T>(2*Constants::PI,true),
																				circle2,circle3,
																				vector,vector,false,number_of_edges_);
			edges_.push_back(edge);
			number_of_edges_++;
			vertex1->edges_.insert(edge);
			vertex1->index_ = number_of_vertices_;
			vertices_.push_back(vertex1);
			number_of_vertices_++;
			vertex2->edges_.insert(edge);
			vertex2->index_ = number_of_vertices_;
			vertices_.push_back(vertex2);
			number_of_vertices_++;
			return edge;
		}
		else
		{
			delete vertex1;
			delete vertex2;
			neighbours_[a1].remove(a2);
			neighbours_[a2].remove(a1);
			return NULL;
		}
	}


	template <typename T>
	TRSVertex<T>* TReducedSurface<T>::findFirstVertex()
		throw()
	{
		for (Position i = 0; i < number_of_atoms_; i++)
		{
			if (atom_status_[i] == STATUS_UNKNOWN)
			{
				if (neighbours_[i].size() == 0)
				{
					TRSVertex<T>* vertex = new TRSVertex<T>(i);
					vertex->index_ = number_of_vertices_;
					vertices_.push_back(vertex);
					number_of_vertices_++;
					return vertex;
				}
			}
		}
		return NULL;
	}


	template <typename T>
	Index TReducedSurface<T>::findFirstAtom(Byte direction, Byte extrem)
		throw()
	{
		Index extrem_atom = -1;
		Index i = 0;
		bool found = false;
		while ((found == false) && (i < (Index)number_of_atoms_))
		{
			if (atom_status_[i] == STATUS_UNKNOWN)
			{
				found = true;
			}
			else
			{
				i++;
			}
		}
		if (found)
		{
			extrem_atom = i;
			TSphere3<T>* next_atom = &atom_[i];
			T extrem_value = ((extrem == 0) ? next_atom->p[direction]-next_atom->radius
																			: next_atom->p[direction]+next_atom->radius);
			i++;
			while (i < (Index)number_of_atoms_)
			{
				if (atom_status_[i] == STATUS_UNKNOWN)
				{
					next_atom = &atom_[i];
					T extremum = ((extrem == 0) ? next_atom->p[direction]-next_atom->radius
																			: next_atom->p[direction]+next_atom->radius);
					if (((extrem == 0) && Maths::isLess(extremum,extrem_value)) ||
							((extrem != 0) && Maths::isGreater(extremum,extrem_value)))
					{
						extrem_value = extremum;
						extrem_atom = i;
					}
				}
				i++;
			}
		}
		return extrem_atom;
	}


	template <typename T>
	Index TReducedSurface<T>::findSecondAtom
			(Index atom,
			 Byte	 direction,
			 Byte	 extrem)
		throw()
	{
		Index second_atom = -1;
		typename std::list<Index>::const_iterator i = neighbours_[atom].begin();
		bool found = false;
		while ((found == false) && (i != neighbours_[atom].end()))
		{
			if (atom_status_[*i] == STATUS_UNKNOWN)
			{
				found = true;
			}
			else
			{
				i++;
			}
		}
		if (found)
		{
			second_atom = *i;
			TSphere3<T> first_atom = atom_[atom];
			first_atom.radius += probe_radius_;
			TCircle3<T> intersection_circle;
			T extrem_value = ((extrem == 0) ? first_atom.p[direction]+first_atom.radius
																			: first_atom.p[direction]-first_atom.radius);
			TSphere3<T> next_atom;
			i++;
			while (i != neighbours_[atom].end())
			{
				if (atom_status_[*i] == STATUS_UNKNOWN)
				{
					next_atom = atom_[*i];
					next_atom.radius += probe_radius_;
					if (GetIntersection(first_atom,next_atom,intersection_circle))
					{
						T next_extrem = getCircleExtremum(intersection_circle,direction,extrem);
						if (((extrem == 0) && Maths::isLess(next_extrem,extrem_value)) ||
								((extrem != 0) && Maths::isGreater(next_extrem,extrem_value)))
						{
							extrem_value = next_extrem;
							second_atom = *i;
						}
					}
				}
				i++;
			}
		}
		return second_atom;
		//if (neighbours_[atom].size() == 0)
		//{
		//	return -1;
		//}
		//TSphere3<T> first_atom(atom_[atom]);
		//first_atom.radius += probe_radius_;
		//TCircle3<T> intersection_circle;
		//T x_min = ((extrem == 0) ? first_atom.p[direction]+first_atom.radius
		//												 : first_atom.p[direction]-first_atom.radius);
		//Index a_min = -1;
		//TSphere3<T> next_atom;
		//for (i = neighbours_[atom].begin(); i != neighbours_[atom].end(); i++)
		//{
		//	next_atom = atom_[*i];
		//	next_atom.radius += probe_radius_;
		//	if (GetIntersection(first_atom,next_atom,intersection_circle))
		//	{
		//		T next_min = getCircleExtremum(intersection_circle,direction,extrem);
		//		if (((extrem == 0) && Maths::isLess(next_min,x_min)) ||
		//				((extrem != 0) && Maths::isGreater(next_min,x_min)))
		//		{
		//			x_min = next_min;
		//			a_min = *i;
		//		}
		//	}
		//}
		//return a_min;
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
				#ifdef print_rs_debug_info
				std::cout << "  probe positions an " << atom1 << ", " << atom2 << ", ...\n";
				#endif
		::std::pair< Index,TSphere3<T> > candidate;
		::std::list<Index>::const_iterator i = third.begin();
		TVector3<T> center1, center2;
		TSphere3<T> probe;
		while (i != third.end())
		{
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
			i++;
		}
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfTwoAtoms(Index atom1, Index atom2)
		throw()
	{
		bool found = false;
		typename HashMap< Position,HashMap< Position,std::list<Index> > >::Iterator n1
			 = neighbours_of_two_.find(atom1);
		if (n1 != neighbours_of_two_.end())
		{
			typename HashMap< Position,std::list<Index> >::Iterator n2 = n1->second.find(atom2);
			if (n2 != n1->second.end())
			{
				found = true;
			}
		}
		if (found == false)
		{
			std::list<Index> empty;
			neighbours_of_two_[atom1][atom2] = empty;
			neighbours_of_two_[atom2][atom1] = empty;
			std::list<Index>::iterator i1 = neighbours_[atom1].begin();
			std::list<Index>::iterator i2 = neighbours_[atom2].begin();
			while ((i1 != neighbours_[atom1].end()) && (i2 != neighbours_[atom2].end()))
			{
				if (*i1 == *i2)
				{
					neighbours_of_two_[atom1][atom2].push_back(*i1);
					neighbours_of_two_[atom2][atom1].push_back(*i1);
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
	}


	template <typename T>
	void TReducedSurface<T>::neighboursOfThreeAtoms
		 (Index								atom1,
			Index								atom2,
			Index								atom3,
			::std::list<Index>& output_list)
		throw()
	{
		//std::list<Index>::iterator i1 = neighbours_[atom1].begin();
		//std::list<Index>::iterator i2 = neighbours_[atom2].begin();
		//std::list<Index>::iterator i3 = neighbours_[atom3].begin();
		neighboursOfTwoAtoms(atom1,atom2);
		neighboursOfTwoAtoms(atom1,atom3);
		typename HashMap< Position,HashMap< Position,std::list<Index> > >::Iterator n1;
		typename HashMap< Position,std::list<Index> >::Iterator n2;
		typename HashMap< Position,std::list<Index> >::Iterator n3;
		n1 = neighbours_of_two_.find(atom1);
		n2 = n1->second.find(atom2);
		n3 = n1->second.find(atom3);
		std::list<Index>::iterator i2 = n2->second.begin();
		std::list<Index>::iterator i3 = n2->second.begin();
		while ((i2 != n2->second.end()) && (i3 != n3->second.end()))
		{
			if (*i2 == *i3)
			{
				output_list.push_back(*i2);
				i2++;
				i3++;
			}
			else
			{
				if (*i2 < *i3)
				{
					i2++;
				}
				else
				{
					i3++;
				}
			}
		}
		/*while ((i1 != neighbours_[atom1].end()) &&
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
		}*/
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
			typename HashSet<Index>::ConstIterator i;
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
		//PROBE_COUNTER1++;
		typename HashMap< Position,HashMap< Position,HashMap< Position,ProbePosition* > > >::Iterator pp1;
		typename HashMap< Position,HashMap< Position,ProbePosition* > >::Iterator pp2;
		typename HashMap< Position,ProbePosition* >::Iterator pp3;
		bool back;
		bool found = false;
		pp1 = probe_positions_.find(a1);
		if (pp1 != probe_positions_.end())
		{
			pp2 = pp1->second.find(a2);
			if (pp2 != pp1->second.end())
			{
				pp3 = pp2->second.find(a3);
				if (pp3 != pp2->second.end())
				{
					found = true;
					if (pp3->second == NULL)
					{
						back = false;
					}
					else
					{
						c1 = pp3->second->point[0];
						c2 = pp3->second->point[1];
						back = true;
					}
				}
			}
		}
		if (found == false)
		{
			//PROBE_COUNTER2++;
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
				c1.set((T)p1.x,(T)p1.y,(T)p1.z);
				c2.set((T)p2.x,(T)p2.y,(T)p2.z);
				ProbePosition* position = new ProbePosition;
				position->status.push_back(STATUS_NOT_TESTED);
				position->status.push_back(STATUS_NOT_TESTED);
				position->point.push_back(c1);
				position->point.push_back(c2);
				probe_positions_[a1][a2][a3] = position;
				back = true;
			}
			else
			{
				probe_positions_[a1][a2][a3] = NULL;
				back = false;
			}
		}
		return back;
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
		Index tmp;
		if (atom1 > atom2)
		{
			tmp = atom1;
			atom1 = atom2;
			atom2 = tmp;
		}
		if (atom1 > atom3)
		{
			tmp = atom1;
			atom1 = atom3;
			atom3 = tmp;
		}
		if (atom2 > atom3)
		{
			tmp = atom2;
			atom2 = atom3;
			atom3 = tmp;
		}
		//PROBE_COUNTER3++;
		Position index;
		ProbePosition* position = probe_positions_[atom1][atom2][atom3];
		if (probe.p == position->point[0])
		{
			index = 0;
		}
		else
		{
			index = 1;
		}
		if (position->status[index] == STATUS_NOT_TESTED)
		{
			///PROBE_COUNTER4++;
			bool back = true;
			std::list<Index> atom_list;
			neighboursOfThreeAtoms(atom1,atom2,atom3,atom_list);
			T dist;
			std::list<Index>::iterator i = atom_list.begin();
			while (back && (i != atom_list.end()))
			{
						#ifdef print_rs_debug_info
						std::cout << "... " << *i;
						#endif
				dist = probe.radius+atom_[*i].radius;
				if (Maths::isLess(probe.p.getSquareDistance(atom_[*i].p),dist*dist))
				{
							#ifdef print_rs_debug_info
							std::cout << " ... not ok ... " << probe.p.getDistance(atom_[*i].p) << " < "
												<< probe.radius+atom_[*i].radius << " ";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
							#endif
					position->status[index] = STATUS_NOT_OK;
					back = false;
				}
						#ifdef print_rs_debug_info
						else
						{
							std::cout << " ... ok ... " << probe.p.getDistance(atom_[*i].p) << " >= "
												<< probe.radius+atom_[*i].radius << " ";
							if (HALT == 0) std::cin >> HALT; else {HALT--; std::cout << "\n";}
						}
						#endif
				i++;
			}
			if (back)
			{
				position->status[index] = STATUS_OK;
			}
		}
		switch (position->status[index])
		{
			case STATUS_OK			:	return true;
			case STATUS_NOT_OK	:	return false;
			default							: throw Exception::GeneralException(__FILE__,__LINE__,"checkProbe","no status");
														// should never happen
		}
	}


	template <typename T>
	void TReducedSurface<T>::correctProbePosition(Position atom)
		throw()
	{
		typename HashMap< Position,HashMap< Position,HashMap< Position,ProbePosition* > > >::Iterator pp1;
		typename HashMap< Position,HashMap< Position,ProbePosition* > >::Iterator pp2;
		typename HashMap< Position,ProbePosition* >::Iterator pp3;
		for (pp1 = probe_positions_.begin(); pp1 != probe_positions_.end(); pp1++)
		{
			if (pp1->first < atom)
			{
				for (pp2 = pp1->second.begin(); pp2 != pp1->second.end(); pp2++)
				{
					if (pp2->first < atom)
					{
						if (pp2->second.has(atom))
						{
							correctProbePosition(pp1->first,pp2->first,atom);
						}
					}
					else
					{
						if (pp2->first == atom)
						{
							for (pp3 = pp2->second.begin(); pp3 != pp2->second.end(); pp3++)
							{
								correctProbePosition(pp1->first,atom,pp3->first);
							}
						}
					}
				}
			}
			else
			{
				if (pp1->first == atom)
				{
					for (pp2 = pp1->second.begin(); pp2 != pp1->second.end(); pp2++)
					{
						for (pp3 = pp2->second.begin(); pp3 != pp2->second.end(); pp3++)
						{
							correctProbePosition(atom,pp2->first,pp3->first);
						}
					}
				}
			}
		}
	}


	template <typename T>
	void TReducedSurface<T>::correctProbePosition(Position a1, Position a2, Position a3)
		throw()
	{
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
			TVector3<T> c1((T)p1.x,(T)p1.y,(T)p1.z);
			TVector3<T> c2((T)p2.x,(T)p2.y,(T)p2.z);
			ProbePosition* position = probe_positions_[a1][a2][a3];
			position->status[0] = STATUS_NOT_TESTED;
			position->status[1] = STATUS_NOT_TESTED;
			position->point[0] = c1;
			position->point[1] = c2;
		}
		else
		{
			delete probe_positions_[a1][a2][a3];
			probe_positions_[a1][a2][a3] = NULL;
		}
	}


	template <class T>
	void TReducedSurface<T>::preProcessing()
		throw()
	{
		T x_min = atom_[0].p.x;
		T y_min = atom_[0].p.y;
		T z_min = atom_[0].p.z;
		T x_max = atom_[0].p.x;
		T y_max = atom_[0].p.y;
		T z_max = atom_[0].p.z;
		for (Position i = 1; i < number_of_atoms_; i++)
		{
			x_min = ((x_min > atom_[i].p.x) ? atom_[i].p.x : x_min);
			y_min = ((y_min > atom_[i].p.y) ? atom_[i].p.y : y_min);
			z_min = ((z_min > atom_[i].p.z) ? atom_[i].p.z : z_min);
			x_max = ((x_max < atom_[i].p.x) ? atom_[i].p.x : x_max);
			y_max = ((y_max < atom_[i].p.y) ? atom_[i].p.y : y_max);
			z_max = ((z_max < atom_[i].p.z) ? atom_[i].p.z : z_max);
		}
		T dist = 2*(r_max_+probe_radius_);
		Position nx = (Position)((x_max-x_min)/dist+5);
		Position ny = (Position)((y_max-y_min)/dist+5);
		Position nz = (Position)((z_max-z_min)/dist+5);
		TVector3<T> origin(x_min-2*dist,y_min-2*dist,z_min-2*dist);
		HashGrid3<Position> grid(origin,nx,ny,nz,dist);
		for (Position i = 0; i < number_of_atoms_; i++)
		{
			grid.insert(atom_[i].p,i);
		}
		TSphere3<T> next_atom;
		T offset;
		HashGridBox3<Position>* box;
		HashGridBox3<Position>::ConstBoxIterator b;
		HashGridBox3<Position>::ConstDataIterator d;
		for (Position i = 0; i < number_of_atoms_-1; i++)
		{
			offset = atom_[i].radius+2*probe_radius_;
			box = grid.getBox(atom_[i].p);
			for (b = box->beginBox(); b != box->endBox(); b++)
			{
				for (d = b->beginData(); d != b->endData(); d++)
				{
					if (*d > i)
					{
						next_atom = atom_[*d];
						T dist = next_atom.p.getSquareDistance(atom_[i].p);
						T max_dist = next_atom.radius+offset;
						max_dist *= max_dist;
						if (Maths::isLess(dist,max_dist))
						{
							neighbours_[i].push_back(*d);
							neighbours_[*d].push_back(i);
						}
					}
				}
			}
			neighbours_[i].sort();
		}
	}




} // namespace BALL

#endif  // BALL_STRUCTURE_REDUCEDSURFACE_H
