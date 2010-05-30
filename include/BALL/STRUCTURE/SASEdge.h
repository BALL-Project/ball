// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SASEDGE_H
#define BALL_STRUCTURE_SASEDGE_H

#ifndef BALL_STRUCTURE_GRAPHEDGE_H
#	include <BALL/STRUCTURE/graphEdge.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

namespace BALL
{

	class SolventAccessibleSurface;
	class SASFace;
	class SASVertex;
	class RSEdge;
	class TriangulatedSAS;
	class SASTriangulator;

	/** Generic SASEdge Class.
	\ingroup Surface			
	*/
	class BALL_EXPORT SASEdge	:	public GraphEdge< SASVertex,SASEdge,SASFace >
	{
		public:

		/** @name Class friends
		
					- class SASFace
					- class SASVertex
					- class SolventAccessibleSurface
					- class TriangulatedSAS
					- class SASTriangulator
				
		*/
		friend class SASVertex;
		friend class SASFace;
		friend class SolventAccessibleSurface;
		friend class TriangulatedSAS;
		friend class SASTriangulator;

		BALL_CREATE(SASEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASEdge object.
		*/
		SASEdge()
			;

		/**	Copy constructor.
				Create a new SASEdge object from another.
				@param	sasedge	the SASEdge object to be copied
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the new SASEdge object is linked	
												to the neighbours of the old SASEdge object.
		*/
		SASEdge(const SASEdge& sasedge, bool deep = false)
			;

		/**	Detailled constructor.
				Create a new SASEdge object from some nice objects.
				@param	vertex0	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face0		assigned to the first face
				@param	face1		assigned to the second face
				@param	circle	assigned to the circle
				@param	angle		assigned to the angle
				@param	index		assigned to the index
		*/
		SASEdge(SASVertex* vertex0,
				SASVertex* vertex1,
				SASFace* face0,
				SASFace* face1,
				const TCircle3<double>& circle,
				const TAngle<double>& angle,
				Index index)
			;

		/**	Destructor.
				Destructs the SASEdge object.
		*/
		virtual ~SASEdge()
			;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another SASEdge object.
				@param	sasedge	the SASEdge object to assign from
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the SASEdge object is linked to	
												the neighbours of the SASEdge object to assign from.
		*/
		void set(const SASEdge& sasedge, bool deep = false)
			;

		/**	Assign from another SASEdge object.
				The SASEdge object is linked to the neighbours of the SASEdge object	
				to assign from.
				@param	sasedge	the SASEdge object to assign from
		*/
		SASEdge& operator = (const SASEdge& sasedge)
			;

		/**	Assign from some nice objects.
				@param	vertex0	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face0		assigned to the first face
				@param	face1		assigned to the second face
				@param	circle	assigned to the circle
				@param	angle		assigned to the angle
				@param	index		assigned to the index
		*/
		void set(SASVertex* vertex0,
				SASVertex* vertex1,
				SASFace* face0,
				SASFace* face1,
				const TCircle3<double>& circle,
				const TAngle<double>& angle,
				Index index)
			;

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the circle on which the SASEdge lies.
				@param	center	the new circle
		*/
		void setCircle(const TCircle3<double>& center)
			;

		/** Return the circle on which the SASEdge lies.
				@return	TCircle3<double>	the circle of the SASEdge
		*/
		TCircle3<double> getCircle() const
			;

		/** Set the angle of the SASEdge.
				@param	angle	the new angle
		*/
		void setAngle(const TAngle<double>& angle)
			;

		/** Return the angle of the SASEdge.
				@return	TAngle<double>	the angle of the SASEdge
		*/
		TAngle<double> getAngle() const
			;

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator == (const SASEdge&) const
			;

		/**	Inequality operator.
				@return	bool	<b>false</b>
		*/
		virtual bool operator != (const SASEdge&) const
			;

		/**	Similarity operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator *= (const SASEdge&) const
			;

		/** isFree.
				@return	bool	<b>true</b> if the RSEdge of the SASEdge is free,	
											<b>false</b> otherwise
		*/
		bool isFree() const
			;

		//@}
		
		protected:
		
		/*_	@name	Attributes
		*/
		//@{

		/*_ The circle on which the SASEdge lies.
		*/
		TCircle3<double> circle_;
		/*_ The angle of the SASEdge.
		*/
		TAngle<double> angle_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const SASEdge& sasedge);

	//@}


} // namespace BALL

#endif // BALL_STRUCTURE_SASEDGE_H
