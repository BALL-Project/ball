// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RSVertex.h,v 1.20.2.1 2003/01/07 13:19:03 anker Exp $

#ifndef BALL_STRUCTURE_RSVERTEX_H
#define BALL_STRUCTURE_RSVERTEX_H

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif


namespace BALL
{

	class RSEdge;
	class RSFace;
	class ReducedSurface;
	class RSComputer;
	class SESEdge;
	class SESFace;
	class SESVertex;
	class SolventExcludedSurface;
	class SESComputer;
	class SESSingularityCleaner;
	class TriangulatedSES;
	class SESTriangulator;
	class SASEdge;
	class SASFace;
	class SASVertex;
	class SolventAccessibleSurface;
	class TriangulatedSAS;
	class SASTriangulator;

	/** Generic RSVertex Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/RSVertex.h}
	*/
	class RSVertex	:	public GraphVertex< RSVertex,RSEdge,RSFace >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class RSEdge
					\item class RSFace
					\item class ReducedSurface
					\item class RSComputer
					\item class SESEdge
					\item class SESFace
					\item class SESVertex
					\item class SolventExcludedSurface
					\item class SESComputer
					\item class SESSingularityCleaner
					\item class TriangulatedSES
					\item class SESTriangulator
					\item class SASEdge
					\item class SASFace
					\item class SASVertex
					\item class SolventAccessibleSurface
					\item class TriangulatedSAS
					\item class SASTriangulator
				\end{itemize}
		*/
		friend class RSEdge;
		friend class RSFace;
		friend class ReducedSurface;
		friend class RSComputer;
		friend class SESEdge;
		friend class SESFace;
		friend class SESVertex;
		friend class SolventExcludedSurface;
		friend class SESComputer;
		friend class SESSingularityCleaner;
		friend class TriangulatedSES;
		friend class SESTriangulator;
		friend class SASEdge;
		friend class SASFace;
		friend class SASVertex;
		friend class SolventAccessibleSurface;
		friend class TriangulatedSAS;
		friend class SASTriangulator;

		BALL_CREATE(RSVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new RSVertex object.
		*/
		RSVertex()
			throw();

		/**	Copy constructor.
				Create a new RSVertex object from another.
				@param	rsvertex	the RSVertex object to be copied
				@param	deep			if deep = false, all pointers are set to NULL
													(default). Otherwise the new RSVertex object is
													linked to the neighbours of the old RSVertex object.
		*/
		RSVertex(const RSVertex& rsvertex, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new RSVertex object from an index.
				@param	atom	assigned to the index of the atom
		*/
		RSVertex(Index atom)
			throw();

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~RSVertex()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another RSVertex.
				@param	rsvertex	the RSVertex object to assign from
				@param	deep			if deep = false, all pointers are set to NULL
													(default). Otherwise the RSVertex object is
													linked to the neighbours of the RSVertex object to
													assign from.
		*/
		void set(const RSVertex& rsvertex, bool deep = false)
			throw();

		/**	Assign from another RSVertex.
				The RSVertex object is linked to the neighbours of the RSVertex	
				object to assign from.
				@param	rsvertex	the RSVertex object to assign from
		*/
		RSVertex& operator = (const RSVertex& rsvertex)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the index of the atom represented by the vertex.
				@param	atom	the new index
		*/
		void setAtom(Index atom)
			throw();

		/** Return the index of the atom represented by the vertex.
				@return	Index	the index of the atom represented by the vertex
		*/
		Index getAtom() const
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true} if the vertices are equal in all components,
											{\bf false} otherwise
		*/
		virtual bool operator == (const RSVertex& rsvertex) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false} if the vertices are equal in all components,
											{\bf true} otherwise
		*/
		virtual bool operator != (const RSVertex& rsvertex) const
			throw();

		/**	Similarity operator.
				@return	bool	{\bf true} if the atoms are equal, {\bf false} otherwise
		*/
		virtual bool operator *= (const RSVertex& rsvertex) const
			throw();

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		/*_ The index of the atom represented by the RSVertex
		*/
		Index atom_;

		//@}

	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
			prints out a rsvertex
	*/
	std::ostream& operator << (std::ostream& s, const RSVertex& rsvertex);

	//@}


} // namespace BALL

#endif // BALL_STRUCTURE_RSVERTEX_H
