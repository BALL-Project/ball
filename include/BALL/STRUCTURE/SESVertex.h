// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SESVertex.h,v 1.14 2002/12/17 14:13:11 anker Exp $

#ifndef BALL_STRUCTURE_SESVERTEX_H
#define BALL_STRUCTURE_SESVERTEX_H

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	class SESEdge;
	class SESFace;
	class SolventExcludedSurface;
	class SESComputer;
	class SESSingularityCleaner;
	class TriangulatedSES;
	class SESTriangulator;

	/** Generic SESVertex Class.
      {\bf Definition:} \URL{BALL/STRUCTURE/SESVertex.h}
	*/
	class SESVertex : public GraphVertex< SESVertex,SESEdge,SESFace >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class SESEdge
					\item class SESFace
					\item class SolventExcludedSurface
					\item class SESComputer
					\item class TSESSingulatityCleaner<double>
					\item class TriangulatedSES
					\item class SESTriangulator
				\end{itemize}
		*/
		friend class SESEdge;
		friend class SESFace;
		friend class SolventExcludedSurface;
		friend class SESComputer;
		friend class SESSingularityCleaner;
		friend class TriangulatedSES;
		friend class SESTriangulator;

		BALL_CREATE(SESVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESVertex object.
		*/
		SESVertex()
			throw();

		/**	Copy constructor.
				Create a new SESVertex object from another.
				@param	sesvertex	the SESVertex object to be copied
				@param	deep			if deep = false, all pointers are set to NULL
													(default). Otherwise the new SESVertex object is
													linked to the neighbours of the old SESVertex object.
		*/
		SESVertex(const SESVertex& sesvertex, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SESVertex object from some nice objects.
				@param	point		assigned to the point representet by the vertex
				@param	normal	assigned to the normal vector of the vertex
				@param	atom		assigned to the index of the closest atom
				@param	index		assigned to the index of the vertex
		*/
		SESVertex
				(const TVector3<double>&	point,
				 const TVector3<double>&	normal,
				 Index					atom,
				 Index					index)
			throw();

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~SESVertex()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another SESVertex.
				@param	sesvertex	the SESVertex object to assign from
				@param	deep			if deep = false, all pointers are set to NULL
													(default). Otherwise the new SESVertex object is
													linked to the neighbours of the SESVertex object
													to assign from.
		*/
		void set(const SESVertex& sesvertex, bool deep = false)
			throw();

		/**	Assign from another SESVertex.
				The new SESVertex object is linked to the neighbours of the SESVertex	
				object to assign from.
				@param	sesvertex	the SESVertex object to assign from
		*/
		SESVertex& operator = (const SESVertex& sesvertex)
			throw();

		/**	Assign to two TVector3<double>'s and two indices.
				@param	point		assigned to the point represented by the vertex
				@param	normal	assigned to the normal vector of th vertex
				@param	atom		assigned to the index of the closest atom
				@param	index		assigned to the index of the vertex
		*/
		void set(const TVector3<double> point,
				const TVector3<double>& normal,
				Index atom,
				Index index)
			throw();

		/**	Set the point represented by the SESVertex.
				@param	point	the new point
		*/
		void setPoint(const TVector3<double>& point)
			throw();

		/**	Return the point represented by the SESVertex.
				@return	TVector3<double>	the point represented by the vertex
		*/
		TVector3<double> getPoint() const
			throw();

		/**	Set the normal vector of the SESVertex.
				@param	point	the new normal vector
		*/
		void setNormal(const TVector3<double>& normal)
			throw(Exception::DivisionByZero);

		/**	Return the normal vector of the SESVertex.
				@return	TVector3<double>	the normal vector of the vertex
		*/
		TVector3<double> getNormal() const
			throw();

		/**	Set the index of the closest atom of the SESVertex.
				@param	atom	the index of the closest atom
		*/
		void setAtom(Index atom)
			throw();

		/**	Return the index of the closest atom.
				@return	Index	the index of the closest atom
		*/
		Index getAtom() const
			throw();

		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true}
		*/
		virtual bool operator == (const SESVertex&) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false}
		*/
		virtual bool operator != (const SESVertex&) const
			throw();

		/**	Similarity operator.
				@return	bool	{\bf true}
		*/
		virtual bool operator *= (const SESVertex&) const
			throw();

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		/*_	The coordinates of the vertex.
		*/
		TVector3<double> point_;
		/*_	The normal vector of the vertex.
		*/
		TVector3<double> normal_;
		/*_	The index of the closest atom.
		*/
		Index atom_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator << (std::ostream& s, const SESVertex& sesvertex);

	//@}


} // namespace BALL

#endif // BALL_SES_SESVERTEX_H
