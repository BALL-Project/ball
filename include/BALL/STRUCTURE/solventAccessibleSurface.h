// $Id: solventAccessibleSurface.h,v 1.3 2002/12/17 14:13:12 anker Exp $

#ifndef BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H
#define BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H

#ifndef BALL_STRUCTURE_SASEDGE_H
#	include <BALL/STRUCTURE/SASEdge.h>
#endif

#ifndef BALL_STRUCTURE_SASFACE_H
#	include <BALL/STRUCTURE/SASFace.h>
#endif

#ifndef BALL_STRUCTURE_SASVERTEX_H
#	include <BALL/STRUCTURE/SASVertex.h>
#endif

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#	include <BALL/STRUCTURE/reducedSurface.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#include <vector>



namespace BALL
{

	class TriangulatedSAS;
	class SASTriangulator;

	/** Generic SolventAccessibleSurface Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/solventAccessibleSurface.h}	\\
	*/
	class SolventAccessibleSurface
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class TriangulatedSAS
					\item class SASTriangulator
				\end{itemize}
		*/
		friend class TriangulatedSAS;
		friend class SASTriangulator;

		BALL_CREATE(SolventAccessibleSurface)

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
				This method creates a new SolventAccessibleSurface object.
		*/
		SolventAccessibleSurface()
			throw();

		/**	Copy constructor.
				Create a new SolventAccessibleSurface object from another.
				@param	sas		the SolventAccessibleSurface object to be copied
				@param	bool	ignored - just for interface consistency
		*/
		SolventAccessibleSurface(const SolventAccessibleSurface& sas, bool = false)
			throw();

		/** Detailed constructor
		*/
		SolventAccessibleSurface(ReducedSurface* reduced_surface)
			throw();

		/**	Destructor.
				As there are no dynamic	data structures, nothing happens.
		*/
		virtual ~SolventAccessibleSurface()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{
		
		/**
		*/
		void setVertex(SASVertex* vertex, Position i)
			throw(Exception::IndexOverflow);

		/**
		*/
		SASVertex* getVertex(Position i) const
			throw(Exception::IndexOverflow);

		/**
		*/
		Size numberOfVertices() const
			throw();
		
		/**
		*/
		void setEdge(SASEdge* edge, Position i)
			throw(Exception::IndexOverflow);

		/**
		*/
		SASEdge* getEdge(Position i) const
			throw(Exception::IndexOverflow);

		/**
		*/
		Size numberOfEdges() const
			throw();
		
		/**
		*/
		void setFace(SASFace* face, Position i)
			throw(Exception::IndexOverflow);

		/**
		*/
		SASFace* getFace(Position i) const
			throw(Exception::IndexOverflow);

		/**
		*/
		Size numberOfFaces() const
			throw();

		/**
		*/
		void setRS(ReducedSurface* rs)
			throw();

		/**
		*/
		ReducedSurface* getRS() const
			throw();

		/**
		*/
		void clear();

		/**	Computes the solvent excluded surface from a ReducedSurface object
		*/
		void compute()
			throw();

		//@}

		private:

		/*_	@name	SAS computation (private)
		*/
		//@{

		void preProcessing()
			throw();

		void get()
			throw();

		void createFace(Position j)
			throw();

		void createVertex(Position j)
			throw();

		void createEdge(Position j)
			throw();

		//@}


		protected:

		Position number_of_vertices_;
		::std::vector<SASVertex*> vertices_;
		Position number_of_edges_;
		::std::vector<SASEdge*> edges_;
		Position number_of_faces_;
		::std::vector<SASFace*> faces_;
		ReducedSurface* reduced_surface_;

	};


	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator << (std::ostream& s,
														 const SolventAccessibleSurface& sas);

	//@}


} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H

