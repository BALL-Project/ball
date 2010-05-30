// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
	\ingroup Surface			
	*/
	class BALL_EXPORT RSVertex	:	public GraphVertex< RSVertex,RSEdge,RSFace >
	{
		public:

		/** @name Class friends

					- class RSEdge
					- class RSFace
					- class ReducedSurface
					- class RSComputer
					- class SESEdge
					- class SESFace
					- class SESVertex
					- class SolventExcludedSurface
					- class SESComputer
					- class SESSingularityCleaner
					- class TriangulatedSES
					- class SESTriangulator
					- class SASEdge
					- class SASFace
					- class SASVertex
					- class SolventAccessibleSurface
					- class TriangulatedSAS
					- class SASTriangulator
				
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
		RSVertex();

		/**	Copy constructor.
				Create a new RSVertex object from another.
				@param	rsvertex	the RSVertex object to be copied
				@param	deep			if deep = false, all pointers are set to NULL
													(default). Otherwise the new RSVertex object is
													linked to the neighbours of the old RSVertex object.
		*/
		RSVertex(const RSVertex& rsvertex, bool deep = false);

		/**	Detailled constructor.
				Create a new RSVertex object from an index.
				@param	atom	assigned to the index of the atom
		*/
		RSVertex(Index atom);

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~RSVertex();
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
		void set(const RSVertex& rsvertex, bool deep = false);

		/**	Assign from another RSVertex.
				The RSVertex object is linked to the neighbours of the RSVertex	
				object to assign from.
				@param	rsvertex	the RSVertex object to assign from
		*/
		RSVertex& operator = (const RSVertex& rsvertex);

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the index of the atom represented by the vertex.
				@param	atom	the new index
		*/
		void setAtom(Index atom);

		/** Return the index of the atom represented by the vertex.
				@return	Index	the index of the atom represented by the vertex
		*/
		Index getAtom() const;

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	<b>true</b> if the vertices are equal in all components,
											<b>false</b> otherwise
		*/
		virtual bool operator == (const RSVertex& rsvertex) const;

		/**	Inequality operator.
				@return	bool	<b>false</b> if the vertices are equal in all components,
											<b>true</b> otherwise
		*/
		virtual bool operator != (const RSVertex& rsvertex) const;

		/**	Similarity operator.
				@return	bool	<b>true</b> if the atoms are equal, <b>false</b> otherwise
		*/
		virtual bool operator *= (const RSVertex& rsvertex) const;

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
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const RSVertex& rsvertex);

	//@}

   
} // namespace BALL

#endif // BALL_STRUCTURE_RSVERTEX_H
