// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
	\ingroup Surface		
	*/
	class BALL_EXPORT SESEdge	:	public GraphEdge< SESVertex,SESEdge,SESFace >
	{
		public:

		/** @name Class friends

					- class SESFace
					- class SESVertex
					- class SolventExcludedSurface
					- class SESComputer
					- class TSESSingularityCleaner
					- class TriangulatedSES
					- class SESTriangulator
				
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
				<tt>0</tt> concave
				<tt>1</tt> convex
				<tt>2</tt> singular
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
			;

		/**	Copy constructor.
				Create a new SESEdge object from another.
				@param	sesedge	the SESEdge object to be copied
				@param	deep		if deep = false, all pointers are set to NULL
												(default). Otherwise the new SESEdge object is linked
												to the neighbours of the old SESEdge object.
		*/
		SESEdge(const SESEdge& sesedge, bool deep = false)
			;

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
			;

		/**	Destructor.
				Destructs the SESEdge object.
		*/
		virtual ~SESEdge()
			;

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
			;

		/**	Assign from another SESEdge.
				The SESEdge object is linked to the neighbours of the SESEdge object	
				to assign from.
				@param	sesedge	the SESEdge object to assign from
		*/
		SESEdge& operator = (const SESEdge& sesedge)
			;

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
			;

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the circle on wich the SESEdge lies.
				@param	center	the new circle
		*/
		void setCircle(const TCircle3<double>& center)
			;

		/** Return the circle on wich the SESEdge lies.
				@return	TCircle3<double>	the circle of The SESEdge
		*/
		TCircle3<double> getCircle() const
			;

		/** Set the corresponding RSEdge.
				@param	rsedge	a pointer to the new RSEdge
		*/
		void setRSEdge(RSEdge* rsedge)
			;

		/** Return the corresponding RSEdge.
				@return	RSEdge*	a pointer to th correwsponding RSEdge
		*/
		RSEdge* getRSEdge() const
			;

		/** Set the type of the SESEdge.
				@param	type	the new type of the SESEdge
		*/
		void setType(Type type)
			;

		/** Get the type of the SESEdge.
				@return	Type	the type of the SESEdge
		*/
		Type getType() const
			;

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	<b>true</b> if all vertices are equal modulo order,
											<b>false</b> otherwise
		*/
		virtual bool operator == (const SESEdge& sesedge) const
			;

		/**	Inequality operator.
				@return	bool	<b>false</b> if all vertices are equal modulo order,
											<b>true</b> otherwise
		*/
		virtual bool operator != (const SESEdge& sesedge) const
			;

		/**	similarity operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator *= (const SESEdge&) const
			;

		/** isFree.
				@return	bool	<b>true</b> if the RSEdge of the SESEdge is free,
											<b>false</b> otherwise
		*/
		bool isFree() const
			;

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
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const SESEdge& sesedge);

	//@}

   
} // namespace BALL

#endif // BALL_STRUCTURE_SESEDGE_H
