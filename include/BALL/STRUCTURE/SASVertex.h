// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SASVertex.h,v 1.3 2002/12/17 14:13:10 anker Exp $

#ifndef BALL_STRUCTURE_SASVERTEX_H
#define BALL_STRUCTURE_SASVERTEX_H

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	class SASEdge;
	class SASFace;
	class SolventAccessibleSurface;
	class TriangulatedSAS;
	class SASTriangulator;

	/** Generic SASVertex Class.	
      {\bf Definition:} \URL{BALL/STRUCTURE/SASVertex.h}
	*/
	class SASVertex	:	public GraphVertex< SASVertex,SASEdge,SASFace >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class SASEdge
					\item class SASFace
					\item class SolventAccessibleSurface
					\item class TriangulatedSAS
					\item class SASTriangulator
				\end{itemize}
		*/
		friend class SASEdge;
		friend class SASFace;
		friend class SolventAccessibleSurface;
		friend class TriangulatedSAS;
		friend class SASTriangulator;

		BALL_CREATE(SASVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASVertex object.
		*/
		SASVertex()
			throw();

		/**	Copy constructor.
				Create a new SASVertex object from another.
				@param	sasvertex	the SASVertex object to be copied
				@param	deep			if deep = false, all pointers are set to NULL		
													(default). Otherwise the new SASVertex object is	
													linked to the neighbours of the old SASVertex object.
		*/
		SASVertex(const SASVertex& sasvertex, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SASVertex object from some nice objects.
				@param	point		assigned to the point representet by the vertex
				@param	index		assigned to the index of the vertex
		*/
		SASVertex(const TVector3<double>& point,
				Index index)
			throw();

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~SASVertex()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another SASVertex.
				@param	sasvertex	the SASVertex object to assign from
				@param	deep			if deep = false, all pointers are set to NULL		
													(default). Otherwise the new SASVertex object is	
													linked to the neighbours of the SASVertex object	
													to assign from.
		*/
		void set(const SASVertex& sasvertex, bool deep = false)
			throw();

		/**	Assign from another SASVertex.
				The new SASVertex object is linked to the neighbours of the SASVertex	
				object to assign from.
				@param	sasvertex	the SASVertex object to assign from
		*/
		SASVertex& operator = (const SASVertex& sasvertex)
			throw();

		/**	Assign to a TVector3<double> and an Index.
				@param	point		assigned to the point represented by the vertex
				@param	index		assigned to the index of the vertex
		*/
		void set(const TVector3<double> point,
				Index index)
			throw();

		/**	Set the point represented by the SASVertex.
				@param	point	the new point
		*/
		void setPoint(const TVector3<double>& point)
			throw();

		/**	Return the point represented by the SASVertex.
				@return	TVector3<double>	the point represented by the vertex
		*/
		TVector3<double> getPoint() const
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true}
		*/
		virtual bool operator == (const SASVertex&) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false}
		*/
		virtual bool operator != (const SASVertex&) const
			throw();

		/**	Similarity operator.
				@return	bool	{\bf true} if the points the SASVertices lie on are	
											equal, {\bf false} otherwise
		*/
		virtual bool operator *= (const SASVertex&) const
			throw();

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		/*_	The coordinates of the vertex.
		*/
		TVector3<double> point_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator << (std::ostream& s, const SASVertex& sasvertex);

	//@}



} // namespace BALL

#endif // BALL_STRUCTURE_SASVERTEX_H
