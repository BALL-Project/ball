// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SESEdge.h,v 1.17 2002/12/17 14:13:10 anker Exp $

#ifndef BALL_STRUCTURE_SESEDGE_H
#define BALL_STRUCTURE_SESEDGE_H

#ifndef BALL_STRUCTURE_GRAPHEDGE_H
#	include <BALL/STRUCTURE/graphEdge.h>
#endif

#ifndef BALL_STRUCTURE_RSEDGE_H
#	include <BALL/STRUCTURE/RSEdge.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

namespace BALL
{

	class SESFace;
	class SESVertex;
	class SolventExcludedSurface;
	class TriangulatedSES;
	class SESComputer;
	class SESSingularityCleaner;
	class SESTriangulator;

	/** Generic SESEdge Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/SESEdge.h}
	*/
	class SESEdge	:	public GraphEdge< SESVertex,SESEdge,SESFace >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class SESFace
					\item class SESVertex
					\item class SolventExcludedSurface
					\item class SESComputer
					\item class TSESSingularityCleaner
					\item class TriangulatedSES
					\item class SESTriangulator
				\end{itemize}
		*/
		friend class SESFace;
		friend class SESVertex;
		friend class SolventExcludedSurface;
		friend class SESComputer;
		friend class SESSingularityCleaner;
		friend class TriangulatedSES;
		friend class SESTriangulator;

		BALL_CREATE(SESEdge)

		/**	@name	Enums
		*/
		//@{

		/** type of SESEdge:
				{\tt 0} concave
				{\tt 1} convex
				{\tt 2} singular
		*/
		enum Type
		{
			TYPE_CONCAVE  = 0,
			TYPE_CONVEX = 1,
			TYPE_SINGULAR = 2
		};


		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESEdge object.
		*/
		SESEdge()
			throw();

		/**	Copy constructor.
				Create a new SESEdge object from another.
				@param	sesedge	the SESEdge object to be copied
				@param	deep		if deep = false, all pointers are set to NULL
												(default). Otherwise the new SESEdge object is linked
												to the neighbours of the old SESEdge object.
		*/
		SESEdge(const SESEdge& sesedge, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SESEdge object from some nice objects.
				@param	vertex0	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face0		assigned to the first face
				@param	face1		assigned to the second face
				@param	circle	assigned to the circle
				@param	rsedge	assigned to the RSEdge
				@param	type		assigned to the type of the SESEdge
				@param	index		assigned to the index
		*/
		SESEdge
				(SESVertex*			vertex0,
				 SESVertex*			vertex1,
				 SESFace*				face0,
				 SESFace*				face1,
				 const TCircle3<double>&	circle,
				 RSEdge*				rsedge,
				 Type						type,
				 Index					index)
			throw();

		/**	Destructor.
				Destructs the SESEdge object.
		*/
		virtual ~SESEdge()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another SESEdge.
				@param	sesedge	the SESEdge object to assign from
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the SESEdge object is linked to	
												the neighbours of the SESEdge object to assign from.
		*/
		void set(const SESEdge& sesedge, bool deep = false)
			throw();

		/**	Assign from another SESEdge.
				The SESEdge object is linked to the neighbours of the SESEdge object	
				to assign from.
				@param	sesedge	the SESEdge object to assign from
		*/
		SESEdge& operator = (const SESEdge& sesedge)
			throw();

		/**	Assign from some nice objects.
				@param	vertex0	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face0		assigned to the first face
				@param	face1		assigned to the second face
				@param	circle	assigned to the circle
				@param	rsedge	assigned to the RSEdge
				@param	type		assigned to the type of the SESEdge
				@param	index		assigned to the index
		*/
		void set(SESVertex*	vertex0,
				SESVertex*			vertex1,
				SESFace*				face0,
				SESFace*				face1,
				const TCircle3<double>&	circle,
				RSEdge*					rsedge,
				Type						type,
				Index						index)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the circle on wich the SESEdge lies.
				@param	circle	the new circle
		*/
		void setCircle(const TCircle3<double>& center)
			throw();

		/** Return the circle on wich the SESEdge lies.
				@return	TCircle3<double>	the circle of The SESEdge
		*/
		TCircle3<double> getCircle() const
			throw();

		/** Set the corresponding RSEdge.
				@param	rsedge	a pointer to the new RSEdge
		*/
		void setRSEdge(RSEdge* rsedge)
			throw();

		/** Return the corresponding RSEdge.
				@return	RSEdge*	a pointer to th correwsponding RSEdge
		*/
		RSEdge* getRSEdge() const
			throw();

		/** Set the type of the SESEdge.
				@param	type	the new type of the SESEdge
		*/
		void setType(Type type)
			throw();

		/** Get the type of the SESEdge.
				@return	Type	the type of the SESEdge
		*/
		Type getType() const
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	{\bf true} if all vertices are equal modulo order,
											{\bf false} otherwise
		*/
		virtual bool operator == (const SESEdge& sesedge) const
			throw();

		/**	Inequality operator.
				@return	bool	{\bf false} if all vertices are equal modulo order,
											{\bf true} otherwise
		*/
		virtual bool operator != (const SESEdge& sesedge) const
			throw();

		/**	similarity operator.
				@return	bool	{\bf true}
		*/
		virtual bool operator *= (const SESEdge&) const
			throw();

		/** isFree.
				@return	bool	{\bf true} if the RSEdge of the SESEdge is free,
											{\bf false} otherwise
		*/
		bool isFree() const
			throw();

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		/*_ The circle on which the SESEdge lies.
		*/
		TCircle3<double> circle_;
		/*_ A pointer to the corresponding RSEdge.
		*/
		RSEdge* rsedge_;
		/*_ The type of the SESEdge.
		*/
		//int type;
		Type type_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator << (std::ostream& s, const SESEdge& sesedge);

	//@}


} // namespace BALL

#endif // BALL_STRUCTURE_SESEDGE_H
